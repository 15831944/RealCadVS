#include "ncwin.h"
#include <QtNetwork>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "rcsplashscreen_dialog.h"
#include "ui_rcsplashscreen_dialog.h"

extern Values v;
extern void* __stdcall RCGetModuleHandle( const char * lpModuleName );
extern bool checkInetUrlAvailable(const char *urlstring);
extern int check_license(char *label,char* label1, char *label2,int application,int showmessages);

bool RCWebEnginePage::acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
     //qDebug() << "acceptNavigationRequest("<<url << "," << type << "," << isMainFrame<<")";
    if (type == QWebEnginePage::NavigationTypeLinkClicked)
    {
        QDesktopServices::openUrl(url);
        return false;
    }
    return true;
}

RCSplashScreen_Dialog::RCSplashScreen_Dialog(int licensed, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RCSplashScreen_Dialog)
{
    ui->setupUi(this);
    installEventFilter(this);
    setWindowFlags(windowFlags() |= Qt::WindowMinMaxButtonsHint);

    m_licensed = licensed;
    m_urlLoaded = false;
    m_progress = 0;
    m_netTestOK = false;
    m_netTestFinished = false;
    m_netManager = 0;
    m_netTestReply = 0;

    m_webview = ui->_300;
    RCWebEnginePage *page = new RCWebEnginePage(QWebEngineProfile::defaultProfile(),m_webview);
    //RCWebEnginePage *page = new RCWebEnginePage(m_webview);
    page->profile()->clearHttpCache();
    m_webview->setPage(page);
    m_webview->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls,true);

    // set the default internet url
    if(m_licensed)
    {
        if (RCGetModuleHandle("landworks") != 0)
            m_url = QUrl("http://www.cad.com.au/splash-screen/landworkscad-v8.html");
        else if (RCGetModuleHandle("arborcad") != 0)
            m_url = QUrl("http://www.cad.com.au/splash-screen/arborcad-v8.html");
        else
            m_url = QUrl("http://www.cad.com.au/splash-screen/realcad-v8.html");
    }
    else
    {
        if (RCGetModuleHandle("landworks") != 0)
            m_url = QUrl("http://www.cad.com.au/splash-screen/landworkscad-v8-trial.html");
        else if (RCGetModuleHandle("arborcad") != 0)
            m_url = QUrl("http://www.cad.com.au/splash-screen/arborcad-v8-trial.html");
        else
            m_url = QUrl("http://www.cad.com.au/splash-screen/realcad-v8-trial.html");
    }

    // test for an internet connection
    m_netTestOK = checkInetUrlAvailable(m_url.toString().toLatin1());

    // if no internet set the fallback local urls
    if(!m_netTestOK)
    {
        if(m_licensed)
        {
            if (RCGetModuleHandle("landworks") != 0)
                m_url = QUrl::fromLocalFile(QString(home.getpublicfilename("Documents/splash-screen-offline/landworkscad-v8-offline.html")));
            else if (RCGetModuleHandle("arborcad") != 0)
                m_url = QUrl::fromLocalFile(QString(home.getpublicfilename("Documents/splash-screen-offline/arborcad-v8-offline.html")));
            else
                m_url = QUrl::fromLocalFile(QString(home.getpublicfilename("Documents/splash-screen-offline/realcad-v8-offline.html")));
        }
        else
        {
            if (RCGetModuleHandle("landworks") != 0)
                m_url = QUrl::fromLocalFile(QString(home.getpublicfilename("Documents/splash-screen-offline/landworkscad-v8-offline-trial.html")));
            else if (RCGetModuleHandle("arborcad") != 0)
                m_url = QUrl::fromLocalFile(QString(home.getpublicfilename("Documents/splash-screen-offline/arborcad-v8-offline-trial.html")));
            else
                m_url = QUrl::fromLocalFile(QString(home.getpublicfilename("Documents/splash-screen-offline/realcad-v8-offline-trial.html")));
        }
    }

    // alternative sites for testing etc.
    //m_url = QUrl("http://www.cad.com.au/campaigns/realcad-v7-50-great-reasons.html");
    //m_url = QUrl("qrc:///DefaultSplashPage.html");

    // debugging for offline
    //m_url = QUrl("http://www.cad.com.au/splash-screen/doesnotexist.html");
    m_webview->setUrl(m_url);

    if(m_licensed)
    {
        ui->_200->setEnabled(true);
        if(v.getinteger("wn::showstartupdialog"))
            ui->_200->setChecked(true);
        else
            ui->_200->setChecked(false);
    }
    else
    {
        v.setinteger("wn::showstartupdialog",1);
        WritePrivateProfileString("Integers","wn::showstartupdialog","1",home.getinifilename());
        ui->_200->setChecked(true);
        ui->_200->setEnabled(false);
    }
    connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(accept()));
   // connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(onCloseButton()));
    connect(m_webview, SIGNAL(loadStarted()),this, SLOT(onLoadStarted()));
    connect(m_webview, SIGNAL(loadFinished(bool)),this, SLOT(onLoadFinished(bool)));
    connect(m_webview->page(), SIGNAL(loadProgress(int)),this, SLOT(onLoadProgress(int)));
}
void RCSplashScreen_Dialog::onCloseButton(){
     accept();
}
void RCSplashScreen_Dialog::show()
{
    QDialog::show();
}

RCSplashScreen_Dialog::~RCSplashScreen_Dialog()
{
    delete ui;
}

void RCSplashScreen_Dialog::setLicensed(int licensed)
{
    m_licensed = licensed;
    if(m_licensed)
    {
        ui->_200->setEnabled(true);
        if(v.getinteger("wn::showstartupdialog"))
            ui->_200->setChecked(true);
        else
            ui->_200->setChecked(false);
    }
    else
    {
        v.setinteger("wn::showstartupdialog",1);
        WritePrivateProfileString("Integers","wn::showstartupdialog","1",home.getinifilename());
        ui->_200->setChecked(true);
        ui->_200->setEnabled(false);
    }
}

bool RCSplashScreen_Dialog::eventFilter(QObject* obj,QEvent* evt){
    if(evt->type()==QEvent::Close){
        evt->ignore();
        hide();
        return true;
      }else return QDialog::eventFilter(obj,evt);
}

void RCSplashScreen_Dialog::on__200_clicked(bool checked)
{
    if(checked)
        v.setinteger("wn::showstartupdialog",1);
    else
        v.setinteger("wn::showstartupdialog",0);

    WritePrivateProfileString("Integers","wn::showstartupdialog",checked ? "1" : "0",home.getinifilename());
}

void RCSplashScreen_Dialog::onLoadStarted()
{
    m_urlLoaded = false;
    m_progress = 0;
}

void RCSplashScreen_Dialog::onLoadFinished(bool ok)
{
    m_urlLoaded = ok;
    if(!ok)
        qInfo() << "url failed to load are you connected to the Internet?";
    else
        qInfo() << "url succeded to load";
    qInfo() << "onLoadFinished - url :" << m_webview->url();

}

void RCSplashScreen_Dialog::onLoadProgress(int progress)
{
    m_progress = progress;
}


