#ifndef RCSPLASHSCREEN_DIALOG_H
#define RCSPLASHSCREEN_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QtNetwork>
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QWebEnginePage>
//#include <QWebEngineView>
//#include <QWebEngineSettings>
//#include <QWebEngineDownloadItem>
//#include <QWebEngineProfile>
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QDesktopServices>
#include <QFileInfo>


class RCWebEnginePage : public QWebEnginePage
{
    Q_OBJECT

public:
    RCWebEnginePage(QObject* parent = 0) : QWebEnginePage(parent){}
    RCWebEnginePage(QWebEngineProfile *profile, QObject *parent = 0) : QWebEnginePage(profile,parent){}

protected:
    bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame);
};

namespace Ui {
class RCSplashScreen_Dialog;
}

class RCSplashScreen_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit RCSplashScreen_Dialog(int licensed=0, QWidget *parent = 0);
    ~RCSplashScreen_Dialog();

    void setLicensed(int licensed);
    virtual bool eventFilter(QObject*,QEvent*);
public slots:
    void show();
    void onLoadStarted();
    void onLoadFinished(bool ok);
    void onLoadProgress(int progress);

private slots:
    void on__200_clicked(bool checked);
    void onCloseButton();
private:
    int m_licensed;
    QUrl m_url;
    bool m_urlLoaded;
    int m_progress;
    bool m_netTestOK;
    bool m_netTestFinished;
    QWebEngineView *m_webview;
    QNetworkReply* m_netTestReply;
    QNetworkAccessManager *m_netManager;

    Ui::RCSplashScreen_Dialog* ui;
};

#endif // RCSPLASHSCREEN_DIALOG_H
