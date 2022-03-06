
#include "ncwin.h"
#include "mainwindow.h"
#include "RCView.h"
#include "RCCentralWidget.h"
#include "rcprintpreview_dialog.h"
#include "rcsplashscreen_dialog.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include "Styleset_Dialog.h"
/*
#include <QTGlobal>
#if (QT_VERSION < 0x50000)
#pragma message( "QT_VERSION < 0x50000" )
#include <QtGui>
//#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QPageSetupDialog>
#else
#pragma message( "QT_VERSION >= 0x50000" )
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QMainWindow>
#include <QPageSetupDialog>
#endif
*/

//#define STRESS_TESTS

#ifdef USE_UI
#include "ui_mainwindow.h"
#endif

extern void file_open(int nocadfiles,RCCHAR *fname);
extern void open_dwg(RCCHAR *filename);
extern void open_dxf(RCCHAR *filename);
extern int check_license(char *label,char* label1, char *label2,int application,int showmessages);

QtMainWindow::QtMainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent,flags)
#ifdef USE_UI
    , ui(new Ui::QtMainWindow)
#endif
{
    // enable drag and drop to the main window
    setAcceptDrops(true);
	
    m_isClosing = false;
    geomRestored = false;
    stateRestored = false;
    isTiled = false;
    MdiArea = 0;
    centerWidget = 0;
    m_splashScreen = 0;

#ifdef USE_UI
    ui->setupUi(this);
#else
    setupUi(this);
#endif
    //resize(QSize(1200,800));

    setDockNestingEnabled(true);

    createActions();
    //createMenus();

    // init the last context id
    lastContextId = 0;

    //setContextMenuPolicy(Qt::CustomContextMenu);
    setContextMenuPolicy(Qt::DefaultContextMenu);
    //setContextMenuPolicy(Qt::PreventContextMenu);

    setWindowTitle(tr("RealCAD : "));

    if(isBigEndian())
        qDebug() << "This machine is Big Endian";
    else
        qDebug() << "This machine is Little Endian";

    qDebug() << "Size of Byte is    : " << sizeof(Byte);
    qDebug() << "Size of char is    : " << sizeof(char);
    qDebug() << "Size of wchar_t is : " << sizeof(wchar_t);
    qDebug() << "Size of short is   : " << sizeof(short);
    qDebug() << "Size of int is     : " << sizeof(int);
    qDebug() << "Size of long is    : " << sizeof(long);
    qDebug() << "Size of float is   : " << sizeof(float);
    qDebug() << "Size of double is  : " << sizeof(double);
    qDebug() << "Size of QChar is   : " << sizeof(QChar);
    qDebug() << "Size of L macro is : " << sizeof(1L);

    QSettings settings;

    setCorner(Qt::TopLeftCorner,Qt::TopDockWidgetArea);
    setCorner(Qt::TopRightCorner,Qt::TopDockWidgetArea);
    setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);

    //geomRestored = restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    //stateRestored = restoreState(settings.value("mainWindowState").toByteArray());

    // connect accelerator shortcut
    //connect(GridToggleShortCut,SIGNAL(activated()),this,SLOT(on_GridToggleShortCut_activated()));
    //connect(GridDisplayShortCut,SIGNAL(activated()),this,SLOT(on_GridDisplayShortCut_activated()));
    //connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenuRequested(QPoint)));
    //connect(this,SIGNAL(closeTabWindow(int)),this,SLOT(closeTabWindow(int)));
    QObject::connect((QGuiApplication*)app,SIGNAL(primaryScreenChanged(QScreen*)),this,SLOT(screenChanged(QScreen*)));
    QObject::connect((QGuiApplication*)app,SIGNAL(screenAdded(QScreen*)),this,SLOT(screenChanged(QScreen*)));
    QObject::connect((QGuiApplication*)app,SIGNAL(screenRemoved(QScreen*)),this,SLOT(screenChanged(QScreen*)));
    QObject::connect(((QApplication*)app)->desktop(),SIGNAL(screenCountChanged(int)),this,SLOT(screenCountChanged(int)));
}

QtMainWindow::~QtMainWindow()
{
#ifdef USE_UI
    delete ui;
#endif

    if(isFullScreen())
        showMaximized();

    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    settings.setValue("WindowPlacement/Main", saveGeometry());

    settings.setValue("WindowState/MainWindow", ((QMainWindow*)cadwindow->gethwnd())->saveState());
}

void QtMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void QtMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    //qDebug() << "in QtMainWindow::dropEvent";

    // KMJ: if we open files here RealCAD crashes
    // because the drop target is destroyed before this function returns
    // we now convert the drop event for these files into a fileOpen event
    // in the applications notify() function so we just accept the file drop actions here.
    // other types of drag and drop objects could be handled here

    // only looking for files at the moment
    if (mimeData->hasUrls())
    {
        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();
        if(urlList.isEmpty())
            return;

        QString filepath = urlList.first().toLocalFile();
        if(filepath.isEmpty())
            return;

        if(filepath.toLower().right(4) == ".cad")
        {
            /*
             * if we could open a cad file here this is how we could to it
             *
            ResourceString rs2(NCMAIN+2),rs3(NCMAIN+3);
            if(filechanged(rs2.gets(),rs3.gets()))
            {
                db.initialize();
                file_open(0,filepath.data());
            }
            */
            event->acceptProposedAction();
        }
        else if(filepath.toLower().right(4) == ".dwg")
        {
            event->acceptProposedAction();
            // if we could open a dwg file here this is how we could to it
            //open_dwg(filepath.data());
        }
        else if(filepath.toLower().right(4) == ".dxf")
        {
            event->acceptProposedAction();
            // if we could open a dwg file here this is how we could to it
            //open_dxf(filepath.data());
        }
    }
}

void QtMainWindow::showSplashScreen(int licensed)
{
#if 0
    if(m_splashScreen == 0)
    {
        m_splashScreen = new RCSplashScreen_Dialog(licensed,this);
        disconnect(app,SIGNAL(aboutToQuit()),m_splashScreen,SLOT(accept()));
        connect(app,SIGNAL(aboutToQuit()),m_splashScreen,SLOT(accept()));
        QRect screenRect = app->desktop()->screenGeometry(app->desktop()->screenNumber((QWidget*)cadwindow->gethwnd()));
        m_splashScreen->move(screenRect.width()*0.1,screenRect.height()*0.1);
        m_splashScreen->resize(screenRect.width()*0.8,screenRect.height()*0.8);
    }
    // show the splash screen on top
    if(m_splashScreen !=0)
    {
        m_splashScreen->exec();
    }
#else
    // make splash screen dialog with no parent
    if(m_splashScreen == 0)
    {
        m_splashScreen = new RCSplashScreen_Dialog(licensed);
        //disconnect(app,SIGNAL(aboutToQuit()),m_splashScreen,SLOT(accept()));
        //connect(app,SIGNAL(aboutToQuit()),m_splashScreen,SLOT(accept()));
        QRect screenRect = app->desktop()->screenGeometry(app->desktop()->screenNumber((QWidget*)cadwindow->gethwnd()));
        m_splashScreen->move(screenRect.width()*0.1,screenRect.height()*0.1);
        m_splashScreen->resize(screenRect.width()*0.8,screenRect.height()*0.8);
    }
    else
        m_splashScreen->setLicensed(licensed);
    // show the splash screen on top
    if(m_splashScreen !=0)
    {
        m_splashScreen->show();
        m_splashScreen->raise();
        m_splashScreen->activateWindow();
    }
#endif
}

void QtMainWindow::hideSplashScreen()
{
    // this only hides the dialog
    if(m_splashScreen !=0)
    {
        m_splashScreen->hide();
    }
}

void QtMainWindow::deleteSplashScreen()
    // due to problems with QWebEngineView we only close the dialog
{
    // and leave it to be deleted by Qt or the os
    if(m_splashScreen !=0)
    {
        m_splashScreen->accept();
        //delete m_splashScreen;        
        //m_splashScreen = 0;
    }
}

void QtMainWindow::readSettings()
{
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    restoreGeometry(settings.value("WindowPlacement/Main").toByteArray());
    restoreState(settings.value("WindowState/MainWindow").toByteArray());
}

void QtMainWindow::saveSettings()
{
    // KMJ: can't save in full screen at the moment
    // needs fixing??
    if(isFullScreen())
        showMaximized();

    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    settings.setValue("WindowPlacement/Main", saveGeometry());
    settings.setValue("WindowState/MainWindow", ((QMainWindow*)cadwindow->gethwnd())->saveState());
}

void QtMainWindow::closeEvent(QCloseEvent *event)
{
    // calls file_command7
    /*
    int commandid = 107;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
          NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
          state.sendevent(nevent);
          state.process();
          app->processEvents();
          event->accept();
    }
    */
    ResourceString rs6(NCMAIN+6);
    ResourceString rs10(NCMAIN+10);
    if (! filechanged(rs6.gets(),rs10.gets()))
    {
        event->ignore();
        m_isClosing = false;
    }
    else
    {
        event->accept();
        deleteSplashScreen();
        // look for the 3d warehouse app and close it if it running
        if(app->mSketchUpBrowser)
        {
            //QMessageBox::information(0,"QtMainWindow destructor","killing the 3D Warehouse app");
            app->mSketchUpBrowser->kill();
            app->mSketchUpBrowser->deleteLater();
            app->mSketchUpBrowser=0;
        }
        m_isClosing = true;
    }

    saveSettings();

    //QMainWindow::closeEvent(event);
}

void QtMainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    /*
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
    */
}

void QtMainWindow::createActions()
{
    /*
    //newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    //newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newFromAct = new QAction(tr("New from &Template"), this);
    newFromAct->setStatusTip(tr("Create a new file from a template"));
    connect(newFromAct, SIGNAL(triggered()), this, SLOT(newFromTemplate()));

    //openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    QMainWindow::connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    //saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    saveOptionsAct = new QAction(tr("Save &Options"), this);
    //saveOptionsAct->setShortcuts(QKeySequence::SaveOptions);
    saveOptionsAct->setStatusTip(tr("Save the System Options"));
    connect(saveOptionsAct, SIGNAL(triggered()), this, SLOT(saveOptions()));

    */
    //exitAct = new QAction(tr("E&xit"), this);
    //exitAct->setShortcuts(QKeySequence::Quit);
#ifdef USE_UI
    ui->action_File_Exit->setStatusTip(tr("Exit the application"));
    //connect(ui->actionE_xit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
#endif
}

void QtMainWindow::createMenus()
{
    /*
    // file menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(newFromAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveOptionsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // edit menu
    editMenu = menuBar()->addMenu(tr("&Edit"));

    // transform menu
    transformMenu = menuBar()->addMenu(tr("&Transform"));

    // options menu
    optionsMenu = menuBar()->addMenu(tr("&Options"));

    // arrange menu
    arrangeMenu = menuBar()->addMenu(tr("&Arrange"));

    // tools menu
    toolsMenu = menuBar()->addMenu(tr("&Tools"));

    // window menu
    windowMenu = menuBar()->addMenu(tr("&Window"));

    // help menu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    */


}

void QtMainWindow::createToolbars()
{
}

void QtMainWindow::createDockWidgets()
{
}

void QtMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    // put up context menus

    // entity context menu
    //qDebug() << "context menu event" << event->reason();
    //Popup1_Menu->exec(event->globalPos());

    event->ignore();

    QMainWindow::contextMenuEvent(event);
}


void QtMainWindow::popupMenuRequested(const QPoint &pos)
{
    lastContextId=0;
    Popup_Menu->exec(pos);
    Popup_Menu->hide();
}

void QtMainWindow::popup1MenuRequested(const QPoint &pos)
{
    lastContextId=0;

    // disable/enable menu items depending on the tye of entity selected
    // disable the render materials item if not a renderable entity
    // disable the trim commands if not a linear entity

    EntityList selectionlist;
    Entity *e;
    if(! state.getselection().empty())
    {
       selectionlist = state.getselection().copy();
       //state.destroyselection(0);
       for (selectionlist.start() ; (e = selectionlist.next()) != NULL ; )
       {
           // context for the trim menus
           if(e->type() == figure_entity ||
              e->type() == group_entity ||
              e->type() == text_entity ||
              e->type() == lineardimension_entity ||
              e->type() == angulardimension_entity ||
              e->type() == distancebearingdimension_entity ||
              e->type() == circulardimension_entity ||
              e->type() == light_entity ||
              e->type() == wall_entity ||
              e->type() == bitmap_entity ||
              e->type() == beam_entity ||
              e->type() == plane_entity ||
              e->type() == point_entity ||
              e->type() == ruledsurface_entity ||
              e->type() == surfaceofrev_entity ||
              e->type() == patch_entity ||
              e->type() == ncsolid_entity )
           {
               action_Popup1_Menu_Trim->setDisabled(true);
               action_Popup1_Menu_TrimLength->setDisabled(true);
               action_Popup1_Menu_Corner->setDisabled(true);
               menu_Divide->setDisabled(true);
           }
           else
           {
               action_Popup1_Menu_Trim->setDisabled(false);
               action_Popup1_Menu_TrimLength->setDisabled(false);
               action_Popup1_Menu_Corner->setDisabled(false);
               menu_Divide->setDisabled(false);
           }
           // context for the material menu
           if(e->type() == plane_entity||
              e->type() == ruledsurface_entity ||
              e->type() == surfaceofrev_entity ||
              e->type() == patch_entity ||
              e->type() == ncsolid_entity )
           {
               action_Popup1_Menu_Material->setDisabled(false);
           }
           else
           {
               action_Popup1_Menu_Material->setDisabled(true);
           }
           // context for the explode menu
           if(e->type() == light_entity ||
              e->type() == bitmap_entity ||
              e->type() == circle_entity ||
              e->type() == ellipse_entity ||
              e->type() == point_entity ||
              e->type() == line_entity ||
              e->type() == patch_entity ||
              e->type() == surfaceofrev_entity ||
              e->type() == ruledsurface_entity ||
              e->type() == wall_entity ||
              e->type() == beam_entity ||
              e->type() == ncsolid_entity )
           {
               action_Popup1_Menu_Explode->setDisabled(true);
           }
           else
           {
               action_Popup1_Menu_Explode->setDisabled(false);
           }
       }
    }

    Popup1_Menu->exec(pos);
    Popup1_Menu->hide();
}

void QtMainWindow::closeTabWindow(int index)
{
    if(cadwindow != 0) {
        if(qobject_cast<QTabWidget *>((QObject*)cadwindow->subwindow->gethwnd()) != 0) {
            View3dWindow *win = cadwindow->matchview3dwindow(((QTabWidget*)cadwindow->subwindow->gethwnd())->widget(index));
            UINT msg;
            WPARAM wParam;
            LPARAM lParam;

            msg = WM_CLOSE;

            // click OK
            wParam = MAKEWPARAM((WORD)0,(WORD)0);
            lParam = (LPARAM)0;
            int status = MainEventProc((HWND)win->gethwnd(),msg,wParam,lParam);
            if(status == 1) {
                cadwindow->deletesubwindow(win);
                ((QTabWidget*)cadwindow->subwindow->gethwnd())->removeTab(index);
            }
        }
    }
}

void QtMainWindow::currentTabChanged(int index)
{
    if(cadwindow != 0) {
        if(qobject_cast<QTabWidget *>((QObject*)cadwindow->subwindow->gethwnd()) != 0) {
            View3dWindow *window = cadwindow->matchview3dwindow(((QTabWidget*)cadwindow->subwindow->gethwnd())->widget(index));
            if(window != 0) {
                cadwindow->selectwindow(window);
                window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
            }
        }
    }
}

void QtMainWindow::closeSubWindow(QWidget *wdw)
{
    qDebug() << "in QtMainWindow::closeSubWindow";
    if(cadwindow != 0) {
        View3dWindow *win = cadwindow->matchview3dwindow(((QMdiSubWindow*)wdw)->widget());
        UINT msg;
        WPARAM wParam;
        LPARAM lParam;

        msg = WM_CLOSE;

        // click OK
        wParam = MAKEWPARAM((WORD)0,(WORD)0);
        lParam = (LPARAM)0;
        int status = MainEventProc((HWND)win->gethwnd(),msg,wParam,lParam);
        if(status == 1) {
            cadwindow->deletesubwindow(win);
            delete win;
        }
    }
}


void QtMainWindow::subWindowActivated(QMdiSubWindow *sbw)
{
    qDebug() << "in QtMainWindow::subWindowActivated";
    qDebug() << "central widget is a : " << centralWidget();
    if(sbw != 0 && cadwindow != 0) {
        /*
        sbw->raise();
        if(sbw->isMaximized())
        {
            MdiArea->setTiledMode(RCMdiArea::noTiling);
            //return;
        }
        if(sbw->isMinimized())
            return;
        //if(sbw == ((QMdiArea*)centralWidget())->activeSubWindow())
        //if(MdiArea && sbw == MdiArea->activeSubWindow())
        //    return;
        View3dWindow *window = cadwindow->matchview3dwindow(((QMdiSubWindow*)sbw)->widget());
        */
        View3dWindow *window = cadwindow->matchview3dwindow(((QMdiSubWindow*)sbw)->widget());
        if(window != 0) {
            //qDebug() << "activate view name : " << QString(window->getview()->getname());
            cadwindow->selectwindow(window);
            //window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
        }

    }
}

void QtMainWindow::newFile()
{
}

void QtMainWindow::newFromTemplate()
{
}

void QtMainWindow::open()
{

}

void QtMainWindow::save()
{

}

void QtMainWindow::saveAs()
{
    // old version now using triggered func
    int status;
    CadFile outfile;
    QString oldFilePath;
    oldFilePath = QString(db.getname());
    QFileInfo oldFileInfo(oldFilePath);
    QString title;
    title = QString(db.gettitle());

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"),
                       oldFilePath,
                       tr("CAD Files (*.CAD)"));
    QFileInfo newFileInfo(filePath);
    QString fileName = newFileInfo.fileName();
    QChar qcfileName[300];
    strcpy(qcfileName,&filePath);

    if (outfile.open(qcfileName,"wb")) {
        db.setname(filePath);
        status  = db.save(&outfile);
        status &= outfile.close();
        if( ! status ) {
            _unlink(db.getname());
            db.setname(oldFilePath);
        } else {
            //program->filesaveapps();
            if (v.getinteger("db::autosaveenabled") != 0)
                program->setautosavetimer();
            db.settitle(fileName);
            title = "RealCAD : " + fileName;
            setWindowTitle(title);
        }
    }
}

void QtMainWindow::saveOptions()
{
}
#if 1
void QtMainWindow::resizeEvent(QResizeEvent *e)
{
    //qDebug() << "Entering : QtMainWindow::resizeEvent ";
    //QMainWindow::resizeEvent(e);


    /*
    if(cadwindow)
    {
        if(centralWidget())
        {
            //if(MdiArea)saveOptions
            //    if(MdiArea->tiledMode() == RCMdiArea::horizontalTiling)
            //        MdiArea->tileHorizontally();
            //    else if(MdiArea->tiledMode() == RCMdiArea::verticalTiling)
            //        MdiArea->tileVertically();
            cadwindow->resizeEvent(e);
        }
        else
            cadwindow->sizeevent(size().width(),size().height());
        //e->accept();
        //return;
    }
    */

    //qDebug() << "QtMainWindow::resizeEvent old size : " << e->oldSize();
    //qDebug() << "QtMainWindow::resizeEvent new geometry : " << geometry();
    //qDebug() << "QtMainWindow::resizeEvent new size : " << size();

    //qDebug() << "QtMainWindow::resizeEvent new frame geometry : " << frameGeometry();
    //qDebug() << "QtMainWindow::resizeEvent new frame size : " << frameSize();
    //qDebug() << "is full screen? " << QMainWindow::isFullScreen();
    app->processEvents(QEventLoop::ExcludeUserInputEvents);
    //app->processEvents();
    QMainWindow::resizeEvent(e);
    app->processEvents(QEventLoop::ExcludeUserInputEvents);
    //app->processEvents();
    //qDebug() << "is full screen? " << QMainWindow::isFullScreen();
}
#endif
void QtMainWindow::paintEvent(QPaintEvent *e)
{
    //qDebug() << "Entering : QtMainWindow::paintEvent ";
    QMainWindow::paintEvent(e);
    // KMJ: may not need this anymore!
    //if(cadwindow)
    //    cadwindow->paintevent();
    //qDebug() << "Leaving : QtMainWindow::paintEvent ";
}


// file slots

void QtMainWindow::on_action_File_New_triggered()
{
#ifndef STRESS_TESTS
    // calls - file_command1()
    int commandid = 100;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
#else
    testNew();
#endif
}

void QtMainWindow::on_action_File_New_from_Template_triggered()
{
    // calls - file_newfromtemplate()
    int commandid = 117;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Open_triggered()
{
    // calls - file_command2()
    //int savePreview = v.getinteger("db::savepreview");
    //v.setinteger("db::savepreview",1);
    int commandid = 101;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
    //v.setinteger("db::savepreview",savePreview);
}

void QtMainWindow::on_action_File_Save_triggered()
{
    // calls - file_command3()
    //int savePreview = v.getinteger("db::savepreview");
    //v.setinteger("db::savepreview",1);
    int commandid = 102;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
    //v.setinteger("db::savepreview",savePreview);
}

void QtMainWindow::on_action_File_Save_as_triggered()
{
    // calls - file_command4()
    int savePreview = v.getinteger("db::savepreview");
    v.setinteger("db::savepreview",1);
    int commandid = 103;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
    v.setinteger("db::savepreview",savePreview);
}

void QtMainWindow::on_action_File_Save_Options_triggered()
{
    // calls - file_command20()
    int commandid = 126;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Print_triggered()
{
#if 1
    // this is currenty just the same as print window
    // Nigel requested this change it might change again later
    QPrinter printer;

    //printer.setPaperSize(QSizeF(v.getreal("pp::paperwidth"),v.getreal("pp::paperheight")),QPrinter::Millimeter);

    //if( v.getreal("pp::paperheight")  > v.getreal("pp::paperwidth"))
    if( QString(v.getstring("pp::orientation"))  == "portrait" )
        printer.setOrientation(QPrinter::Portrait);
    else
        printer.setOrientation(QPrinter::Landscape);

    printer.setDocName(QString(db.gettitle()));

    RCPrintPreview_Dialog preview(&printer, this, Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
    //QPrintPreviewDialog preview(&printer, this, Qt::WindowFullscreenButtonHint|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);

    preview.move(QPoint(geometry().x(),geometry().y()));
    int dw = frameGeometry().width()-geometry().width();
    int dh = frameGeometry().height()-geometry().height();
    preview.resize(QSize(geometry().width()-dw,geometry().height()-dh));

    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
            this, SLOT(printSheet(QPrinter*)));

    preview.exec();

#if 0
    // this code just puts up the printer selection dialog
    // use this if needed
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);

    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        // draw with the workplane axis and grid off
        db.draw(DM_ERASE,cadwindow->getcurrentsurface(), cadwindow->getcurrentsurface()->getScene());
        db.drawforQtPrint(DM_NORMAL,cadwindow->getcurrentsurface(), cadwindow->getcurrentsurface()->getScene());

        // print, fitting the viewport contents into a full page
        ((QGraphicsView*)cadwindow->getcurrentwindow()->gethwnd())->render(&painter);

        // draw with the workplane axis and grid on
        db.draw(DM_NORMAL,cadwindow->getcurrentsurface(), cadwindow->getcurrentsurface()->getScene());
        ((QGraphicsView*)cadwindow->getcurrentwindow()->gethwnd())->update();
    }
#endif
#else
    // this is the original RealCAD Print command call
    // calls - file_command6()
    int commandid = 104;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
#endif
}

void QtMainWindow::on_action_File_Commands_triggered()
{

}

void QtMainWindow::on_action_File_Exit_triggered()
{
    // look at the close event function
    close();
}

void QtMainWindow::printSheet(QPrinter* printer)
{
    //qDebug() << "print Sheet";
    // new printing method
    if(printer)
    {
        printer->setFullPage(true);
        printer->newPage();
        //QPainter painter(printer);
        QPainter painter;
        painter.begin(printer);

        qDebug() <<  "printer width : " << printer->width();
        qDebug() <<  "printer height : " << printer->height();
        qDebug() <<  "printer paperrect : " << printer->paperRect();
        qDebug() <<  "printer pagerect : " << printer->pageRect();
        qDebug() <<  "printer physicalDpiX : " << printer->physicalDpiX();
        qDebug() <<  "printer physicalDpiy : " << printer->physicalDpiY();
        qDebug() <<  "printer logicalDpiX : " << printer->logicalDpiX();
        qDebug() <<  "printer logicalDpiY : " << printer->logicalDpiY();
        qDebug() <<  "printer orientation : " << printer->orientation();
        qDebug() <<  "page orientation : " << printer->pageLayout().orientation();

        qDebug() <<  "painter width :" << painter.device()->width();
        qDebug() <<  "painter height :" << painter.device()->height();
        qDebug() <<  "painter widthMM :" << painter.device()->widthMM();
        qDebug() <<  "painter heightMM :" << painter.device()->heightMM();
        qDebug() <<  "painter physicalDpiX :" << painter.device()->physicalDpiX();
        qDebug() <<  "painter physicalDpiy :" << painter.device()->physicalDpiY();
        qDebug() <<  "painter logicalDpiX :" << painter.device()->logicalDpiX();
        qDebug() <<  "painter logicalDpiY :" << painter.device()->logicalDpiY();
        qDebug() <<  "painter viewport width :" << painter.viewport().width();
        qDebug() <<  "painter viewport height :" << painter.viewport().height();
        qDebug() <<  "painter window width :" << painter.window().width();
        qDebug() <<  "painter window height :" << painter.window().height();

        // realcad paper sizes
        qreal rcpaperwidth  = v.getreal("pp::paperwidth");
        qreal rcpaperheight = v.getreal("pp::paperheight");
        qreal rcprintscale  = v.getreal("pp::printscale");
        QString rcscalename(v.getstring("pr::scalename"));
        QString rcorientation(v.getstring("pp::orientation"));

        qDebug() <<  "rcpaperwidth : " << rcpaperwidth;
        qDebug() <<  "rcpaperheight : " << rcpaperheight;
        qDebug() <<  "rcprintscale : " << rcprintscale;
        qDebug() <<  "rcscalename : " << QString(rcscalename);
        qDebug() <<  "rcorientation : " << QString(rcorientation);

        //db.savezoomundo(0,cadwindow->getcurrentwindow());

        View3dWindow *view3dwindow = cadwindow->getcurrentwindow();

        qDebug() << "view3dwindow->getumin" << view3dwindow->getumin();
        qDebug() << "view3dwindow->getumax" << view3dwindow->getumax();
        qDebug() << "view3dwindow->getvmin" << view3dwindow->getvmin();
        qDebug() << "view3dwindow->getvmax" << view3dwindow->getvmax();

        RCView3dPrinter *printView = 0;

        if(view3dwindow->getsurfacetype() == View2dSurf)
            printView = new RCView2dPrinter(view3dwindow,printer);
        else
            printView = new RCView3dPrinter(view3dwindow,printer);

        if(rcscalename != "Fit to Paper")
        {
            printView->setprintscale(rcprintscale);
            // dff to 7.5
            printView->setplotscale(view3dwindow->getplotscale());// fix to match v5/6
            //printView->setplotscale(rcprintscale); // 7.6
            printView->zoom(rcprintscale);
        }
        else
        {
            qDebug() << "Fit to paper";
            rcprintscale = printView->getscale();
            printView->setprintscale(rcprintscale);
            printView->setplotscale(view3dwindow->getplotscale());// fix to match v5/6
            //printView->setplotscale(rcprintscale); // 7.6

            //rcprintscale = 1.0;
            //if(view3dwindow->getsurfacetype() == View2dSurf)
            //printView->zoom(1.0);
        }

        db.clearScene(DM_ERASE,printView,printView->getScene());

        printView->setblackandwhite(v.getinteger("pl::blackandwhite"));
        printView->setreverseblackwhite(v.getinteger("pl::reverseblackwhite"));

        painter.setRenderHint(QPainter::Antialiasing,false);
        painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
        painter.setRenderHint(QPainter::TextAntialiasing,true);

        //printView->debug(home.getpublicfilename("PrintSheetDebug.txt"));

        printView->render(&painter,printView->getumin(),printView->getvmin(),
                         printView->getumax(),printView->getvmax());

        painter.end();

        db.clearScene(DM_ERASE,printView,printView->getScene());

        if(view3dwindow->getsurfacetype() == View2dSurf)
            delete (RCView2dPrinter*)printView;
        else
            delete (RCView3dPrinter*)printView;
    }
}

void QtMainWindow::printSheetSelect(QPrinter* printer)
{
    //qDebug() << "print Sheet Select";
    if(printer)
    {
        printer->setFullPage(true);
        printer->newPage();
        //QPainter painter(printer);
        QPainter painter;
        painter.begin(printer);

        qDebug() <<  "printer width : " << printer->width();
        qDebug() <<  "printer height : " << printer->height();
        qDebug() <<  "printer paperrect : " << printer->paperRect();
        qDebug() <<  "printer pagerect : " << printer->pageRect();
        qDebug() <<  "printer physicalDpiX : " << printer->physicalDpiX();
        qDebug() <<  "printer physicalDpiy : " << printer->physicalDpiY();
        qDebug() <<  "printer logicalDpiX : " << printer->logicalDpiX();
        qDebug() <<  "printer logicalDpiY : " << printer->logicalDpiY();
        qDebug() <<  "printer orientation : " << printer->orientation();
        qDebug() <<  "page orientation : " << printer->pageLayout().orientation();

        qDebug() <<  "painter viewport width :" << painter.viewport().width();
        qDebug() <<  "painter viewport height :" << painter.viewport().height();
        qDebug() <<  "painter window width :" << painter.window().width();
        qDebug() <<  "painter window height :" << painter.window().height();
        qDebug() <<  "painter width :" << painter.device()->width();
        qDebug() <<  "painter height :" << painter.device()->height();
        qDebug() <<  "painter widthMM :" << painter.device()->widthMM();
        qDebug() <<  "painter heightMM :" << painter.device()->heightMM();
        qDebug() <<  "painter physicalDpiX :" << painter.device()->physicalDpiX();
        qDebug() <<  "painter physicalDpiy :" << painter.device()->physicalDpiY();
        qDebug() <<  "painter logicalDpiX :" << painter.device()->logicalDpiX();
        qDebug() <<  "painter logicalDpiY :" << painter.device()->logicalDpiY();

        // realcad paper sizes
        qreal rcpaperwidth  = v.getreal("pp::paperwidth");
        qreal rcpaperheight = v.getreal("pp::paperheight");
        qreal rcprintscale  = v.getreal("pp::printscale");
        QString rcscalename(v.getstring("pr::scalename"));
        QString rcorientation(v.getstring("pp::orientation"));

        if(rcscalename == "Fit to Paper")
            rcprintscale = v.getreal("pr::printscale");

        qDebug() <<  "rcpaperwidth : " << rcpaperwidth;
        qDebug() <<  "rcpaperheight : " << rcpaperheight;
        qDebug() <<  "rcprintscale : " << rcprintscale;
        qDebug() <<  "rcscalename : " << QString(rcscalename);
        qDebug() <<  "rcorientation : " << QString(rcorientation);

        View3dWindow *view3dwindow = cadwindow->getcurrentwindow();

        qDebug() << "view3dwindow->getumin" << view3dwindow->getumin();
        qDebug() << "view3dwindow->getumax" << view3dwindow->getumax();
        qDebug() << "view3dwindow->getvmin" << view3dwindow->getvmin();
        qDebug() << "view3dwindow->getvmax" << view3dwindow->getvmax();

        RCView3dPrinter *printView = 0;

        if(view3dwindow->getsurfacetype() == View2dSurf)
            printView = new RCView2dPrinter(view3dwindow,printer);
        else
            printView = new RCView3dPrinter(view3dwindow,printer);

        //RCView3dPrinter printView(view3dwindow,printer);
        if(rcscalename != "Fit to Paper")
        {
            printView->setprintscale(rcprintscale);
            // diff to 7.5
            printView->setplotscale(view3dwindow->getplotscale());// fix to match v5/6
            //printView->setplotscale(rcprintscale);

            qDebug() << "printer->widthMM  : " << (double)printer->widthMM();
            qDebug() << "printer->heightMM : " << (double)printer->heightMM();
            qDebug() << "RC paperwidth  : " << rcpaperwidth;
            qDebug() << "RC paperheight : " << rcpaperheight;

            double pickx = v.getreal("pp:px");
            double picky = v.getreal("pp:py");
            // diff to 7.5
            double pickz = v.getreal("pp:pz");
            Point scp;
            scp.x = pickx;
            scp.y = picky;
            // diff to 7.5
            scp.z = pickz;
            //scp.z = 0.0;;
            scp = view3dwindow->modeltouv(scp);

            double du = rcpaperwidth * rcprintscale / 2.0;// old
            double dv = rcpaperheight * rcprintscale / 2.0;// old
            // this one assumes the pick is left bottom of the sheet
            //printView->setzoom(scp.x,scp.y,scp.x+rcpaperwidth * rcprintscale,scp.y+rcpaperheight * rcprintscale);
            // this one assumes the pick point is centered in the sheet
            printView->setzoom(scp.x-du,scp.y-dv,scp.x+du,scp.y+dv);
            printView->zoom(rcprintscale);
        }
        else
        {
            //printView->setprintscale(rcprintscale);
            // diff to 7.5
            //printView->setplotscale(rcprintscale);

            double pwidth = v.getreal("pp:w");
            double pheight = v.getreal("pp:h");
            double pickx = v.getreal("pp:px");// + paperwidth * printScale  / 2.0;
            double picky = v.getreal("pp:py");// + paperheight * printScale / 2.0;
            // diff to 7.5
            double pickz = v.getreal("pp:pz");// + paperheight * printScale / 2.0; // 7.6
            Point scp;
            scp.x = pickx;
            scp.y = picky;
            // diff to 7.5H
            scp.z = pickz;
            //scp.z = 0.0; // 7.5
            scp = view3dwindow->modeltouv(scp);

            double du = pwidth / 2.0;
            double dv = pheight / 2.0;
            // this one assumes the pick point is centered in the sheet
            printView->setzoom(scp.x-du,scp.y-dv,scp.x+du,scp.y+dv);
            rcprintscale = printView->getscale();
            printView->setprintscale(rcprintscale);
            printView->setplotscale(view3dwindow->getplotscale());// fix to match v5/6
            //printView->setplotscale(rcprintscale); // 7.6
        }

        db.clearScene(DM_ERASE,printView,printView->getScene());

        printView->setblackandwhite(v.getinteger("pl::blackandwhite"));
        printView->setreverseblackwhite(v.getinteger("pl::reverseblackwhite"));

        painter.setRenderHint(QPainter::Antialiasing,false);
        painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
        painter.setRenderHint(QPainter::TextAntialiasing,true);

        //printView->debug(home.getpublicfilename("PrintSheetSelectDebug.txt"));

        printView->render(&painter,printView->getumin(),printView->getvmin(),
                         printView->getumax(),printView->getvmax());

        db.clearScene(DM_ERASE,printView,printView->getScene());

        if(view3dwindow->getsurfacetype() == View2dSurf)
            delete (RCView2dPrinter*)printView;
        else
            delete (RCView3dPrinter*)printView;
     }
}

// print a raytraced 3d view
void QtMainWindow::printRTSheet(QPrinter* printer)
{
    if(printer)
    {
        printer->setFullPage(true);
        printer->newPage();
        QPainter painter;
        painter.begin(printer);
        QRectF pagerect = printer->pageRect();

        // realcad paper sizes
        qreal rcpaperwidth  = v.getreal("pp::paperwidth");
        qreal rcpaperheight = v.getreal("pp::paperheight");
        qreal rcprintscale  = v.getreal("pp::printscale");
        QString rcscalename(v.getstring("pr::scalename"));
        QString rcorientation(v.getstring("pp::orientation"));

        double imagescale = 1.0;
        if(rcscalename.toLower() != "fit to paper")
            imagescale = rcprintscale;

        View3dWindow *view3dwindow = cadwindow->getcurrentwindow();

        QImage printImage = view3dwindow->getScene()->backgroundBrush().textureImage();
        QRectF imagerect = printImage.rect();
        imagerect.setWidth(imagerect.width() / imagescale);
        imagerect.setHeight(imagerect.height() / imagescale);

        // fit a rectangle the same shape as the paper to the print image
        QRectF origimgrect = printImage.rect();
        double paperscalex = origimgrect.width() / rcpaperwidth;
        double paperscaley = origimgrect.height() / rcpaperheight;
        double paperscale = max(paperscalex,paperscaley);
        QRectF fitrect(0,0,rcpaperwidth * paperscale, rcpaperheight * paperscale);
        // create a backimage
        QImage backImage(fitrect.width(),fitrect.height(),printImage.format());
        backImage.fill(QColor(255,255,255));
        // merge the printImage into the back image
        QPainter merger(&backImage);
        double idx = fitrect.width()/2.0 - imagerect.width()/2.0;
        double idy = fitrect.height()/2.0 - imagerect.height()/2.0;
        imagerect.translate(idx,idy);
        merger.drawImage(imagerect,printImage);
        // paint the image to the printer
        painter.drawImage(pagerect,backImage);
    }
}

// print a selected window of a raytraced 3d view
void QtMainWindow::printRTSheetSelect(QPrinter* printer)
{
    if(printer)
    {
        printer->setFullPage(true);
        printer->newPage();
        QPainter painter;
        painter.begin(printer);
        QRect pagerect  = printer->pageRect();

        // realcad paper sizes
        qreal rcpaperwidth  = v.getreal("pp::paperwidth");
        qreal rcpaperheight = v.getreal("pp::paperheight");
        qreal rcprintscale  = v.getreal("pp::printscale");
        QString rcscalename(v.getstring("pr::scalename"));
        QString rcorientation(v.getstring("pp::orientation"));

        double imagescale = 1.0;
        if(rcscalename.toLower() != "fit to paper")
            imagescale = rcprintscale;

        View3dWindow *view3dwindow = cadwindow->getcurrentwindow();

        double pwidth  = v.getreal("pp:w");
        double pheight = v.getreal("pp:h");
        double pickx   = v.getreal("pp:px");
        double picky   = v.getreal("pp:py");
        double pickz   = v.getreal("pp:pz");
        Point scp;
        scp.x = pickx;
        scp.y = picky;
        scp.z = pickz;
        double du = pwidth / 2.0;
        double dv = pheight / 2.0;
        Point scp2;
        scp2.x = pickx + du;
        scp2.y = picky + dv;
        scp2.z = pickz;
        scp.x  = pickx - du;
        scp.y  = picky - dv;
        scp.z  = pickz;
        scp = view3dwindow->uvtoscreen(scp);
        scp2 = view3dwindow->uvtoscreen(scp2);

        QRect pickRect(scp.x,scp2.y,scp2.x-scp.x,scp.y-scp2.y);

        QImage printImage = view3dwindow->getScene()->backgroundBrush().textureImage().copy(pickRect);
        QRectF imagerect = printImage.rect();
        imagerect.setWidth(imagerect.width() / imagescale);
        imagerect.setHeight(imagerect.height() / imagescale);

        // fit a rectangle the same shape as the paper to the print image
        QRectF origimgrect = printImage.rect();
        double paperscalex = origimgrect.width() / pwidth;
        double paperscaley = origimgrect.height() / pheight;
        double paperscale = max(paperscalex,paperscaley);
        QRectF fitrect(0,0,pwidth * paperscale, pheight * paperscale);
        // create a backimage
        QImage backImage(fitrect.width(),fitrect.height(),printImage.format());
        backImage.fill(QColor(255,255,255));
        // merge the printImage into the back image
        QPainter merger(&backImage);
        double idx = fitrect.width()/2.0 - imagerect.width()/2.0;
        double idy = fitrect.height()/2.0 - imagerect.height()/2.0;
        imagerect.translate(idx,idy);
        merger.drawImage(imagerect,printImage);
        // paint the image to the printer
        painter.drawImage(pagerect,backImage);
    }
}

void QtMainWindow::on_action_File_Print_Preview_Window_triggered()
{
    // this is the original RealCAD Print command call
    // calls - printpreview_command1()
    int commandid = 120;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Print_Preview_Selection_triggered()
{
    // calls - printpreview_command2()
    int commandid = 121;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();

        //on_action_File_Print_Preview_Window_triggered();
    }
}

void QtMainWindow::on_action_File_Export_DXF_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 700; // dfxout_command
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Export_DWG_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 704;// dwgout_command
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Export_IGES_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 701;// igesout_command
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Export_Stroked_IGES_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 703;// strokedigesout_command()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Export_HPGL_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 702;// hpglout_command()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Import_DXF_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 750;// dfxin_command()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Import_DWG_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 754;// dwgin_command()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Import_IGES_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 751;// igesin_command
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Import_HPGL_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 752;// hpglin_command()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Import_Medusa_triggered()
{

}

void QtMainWindow::on_action_File_Import_SKP_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 755;// skpfilein_command()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_File_Import_3DWarehouse_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 10020;// file_3DWarehouse()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
// edit slots

void QtMainWindow::on_action_Edit_Undo_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 200;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Redo_triggered()
{
    // this is the realCAD StateMachine method
    int commandid = 201;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Cut_triggered()
{
    int commandid = 202;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Copy_triggered()
{
    // calls - copy_command
    int commandid = 203;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Edit_Copy_2D_triggered()
{
    // calls - copybitmap_toclipboard_command()
    int commandid = 222;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Edit_Copy_2D_Window_triggered()
{
    // call - copybitmapwindow_toclipboard_command()
    int commandid = 223;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Edit_Copy_Bitmap_triggered()
{
    // calls - copybitmap_command()
    int commandid = 214;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Paste_triggered()
{
    // calls - paste_command()
    int commandid = 204;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Edit_Delete_triggered()
{
    //qDebug() << "on_action_Edit_Delete_triggered";
    int commandid = 205;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_New_Attribute_triggered()
{
    int commandid = 206;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Delete_Attribute_triggered()
{
    int commandid = 207;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Attribute_triggered()
{
    int commandid = 208;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_List_Attribute_triggered()
{
    int commandid = 209;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Find_and_replace_triggered()
{
    int commandid = 211;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Find_in_files_triggered()
{
    int commandid = 215;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Edit_Info_triggered()
{
    int commandid = 212;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

// file menu slots
void QtMainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowFilePath(curFile);

    //QSettings settings;
    //QStringList files = settings.value("recentFileList").toStringList();
    QStringList files;

    int i=0;
    RCCHAR key[256],name[300],title[300];
    //settings.setValue("recentFileList", files);
    for (i = MaxRecentFiles-1 ; i >= 0 ; i--) {
        QString qkey = QString("op::file%1").arg(i);
        sprintf(key,qkey.data());
        WritePrivateProfileString(_RCT("NameList"),key,cadwindow->openlist.names[i],home.getinifilename());
        WritePrivateProfileString(_RCT("TitleList"),key,cadwindow->openlist.titles[i],home.getinifilename());
    }

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        QtMainWindow *mainWin = qobject_cast<QtMainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void QtMainWindow::openRecentFile()
{
    ResourceString rs4(NCMAIN+4);
    ResourceString rs5(NCMAIN+5);
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        if (! filechanged(rs4.gets(),rs5.gets()))
            return;

        QFileInfo openFile(action->data().toString());
#if defined(WIN32)
        StorageFile sinfile;
#endif
        CadFile oinfile;
        CadFile *infile;
        RCCHAR fname[500],title[500],*ws;
        const RCCHAR *s;
        //qDebug() << "recent file : " << openFile.fileName();
        //qDebug() << "recent path : " << openFile.filePath();
        strcpy(fname,openFile.filePath().data());
        strcpy(title,openFile.fileName().data());

#if defined(WIN32)
        if (StgIsStorageFile(fname) == S_OK)
            infile = & sinfile;
        else
#endif
            infile = & oinfile;
        program->setbusy(1);
        if (infile->open(fname,TEXT("rb"))) {
            db.clear();
            db.settitle(title);
            cadwindow->updatetitle();
            db.setname(fname);
            if (db.load(infile))
                program->fileopenapps();
            else
                db.initialize();
            infile->close();
            cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
            // clear all pending events
            app->processEvents();
            // repaint all windows
            cadwindow->paintall();
        }
        program->setbusy(0);
    }
}

void QtMainWindow::updateRecentFileActions()
{
    //QSettings settings;
    //QStringList files = settings.value("recentFileList").toStringList();
    QStringList files;
    int i=0;
    RCCHAR key[256],name[300],title[300];
    //for (i = 0 ; i < MaxRecentFiles ; i++)
    for (i = MaxRecentFiles-1 ; i >= 0 ; i--) {
        name[0]=0;
        QString qkey = QString("op::file%1").arg(i);
        sprintf(key,qkey.data());
        GetPrivateProfileString(_RCT("NameList"),key,_RCT(""),name,256,home.getinifilename());
        GetPrivateProfileString(_RCT("TitleList"),key,_RCT(""),title,256,home.getinifilename());
        if (strlen(name) > 0 && strlen(title) > 0)
            files.append(QDir::toNativeSeparators(QString(name)));
    }

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        // this removes the path
        //QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        QString text = files[i];
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString QtMainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void QtMainWindow::on_action_Options_Cursor_on_work_plane_triggered()
{
    int commandid = 400;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Cursor_on_view_triggered()
{
    int commandid = 401;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Grid_triggered()
{
    int commandid = 416;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_GridDisplayShortCut_activated()
{
    //qDebug() << "on_GridDisplayShortCut_activated";
    int commandid = 420;
    /*
    program->setcommandid(commandid);
    state.process();
    app->processEvents();
    */
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}
void QtMainWindow::processShortcutCommand()
{
    qDebug() << "processShortcutCommand";
    int commandid = ((QShortcut*)sender())->property("cmid").toInt();
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_SelectAllShortCut_activated()
{
    qDebug() << "on_SelectAllShortCut_activated";
    int commandid = 2012;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_GridToggleShortCut_activated()
{
    //qDebug() << "on_GridToggleShortCut_activated";
    int commandid = 421;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_SwitchWindowShortCut_activated()
{
    // comes from the keyboard short cut to switch windows
    qDebug() << "on_SwitchWindowShortCut_activated";
    qDebug() << "central widget is a : " << centralWidget();

    //RCView *currentView = ((RCCentralWidget*)centralWidget())->activeWindow();
    RCView *currentView = activeView();
    View3dWindow *window = cadwindow->matchview3dwindow(currentView);
    if(window != 0) {
        qDebug() << "activate view name before switch : " << QString(window->getview()->getname());
        cadwindow->movetoback(window);
        window = activeWindow();
        qDebug() << "activate view name after switch  : " << QString(window->getview()->getname());
        if(window != 0)
            window->paintevent();
    }
}

void QtMainWindow::on_action_Options_Cursor_on_Depth_triggered()
{
    //qDebug() << "on_action_Options_Cursor_on_Depth_triggered";
    int commandid = 523;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Select_workplane_triggered()
{
    //qDebug() << "on_action_Options_Select_workplane_triggered";
    int commandid = 402;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Create_workplane_triggered()
{
    //qDebug() << "on_action_Options_Create_workplane_triggered";
    int commandid = 403;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Delete_workplane_triggered()
{
    //qDebug() << "on_action_Options_Delete_workplane_triggered";
    int commandid = 407;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Change_Workplane_Reset_to_standard_triggered()
{
    //qDebug() << "on_action_Options_Delete_workplane_triggered";
    int commandid = 406;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Change_Workplane_Origin_triggered()
{
    //qDebug() << "on_action_Options_Change_Workplane_Origin_triggered";
    int commandid = 404;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Change_Workplane_Three_points_triggered()
{
    //qDebug() << "on_action_Options_Change_Workplane_Three_points_triggered";
    int commandid = 405;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Change_Workplane_Onto_Entity_triggered()
{
    //qDebug() << "on_action_Options_Change_Workplane_Onto_Entity_triggered";
    int commandid = 425;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
/*
void QtMainWindow::on_action_Options_Line_triggered()
{
    int commandid = 408;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
void QtMainWindow::on_action_Options_Plane_triggered()
{
    int commandid = 409;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Ruled_surface_triggered()
{
    int commandid = 410;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Surface_of_revolution_triggered()
{
    int commandid = 411;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Patch_triggered()
{
    int commandid = 419;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Group_triggered()
{
    int commandid = 414;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Text_triggered()
{
    int commandid = 412;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Figure_triggered()
{
    int commandid = 413;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Bitmap_triggered()
{
    int commandid = 428;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Dimension_triggered()
{
    int commandid = 415;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Options_Leader_triggered()
{
    int commandid = 424;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

// KMJ temporary item for setting light options
void QtMainWindow::on_action_Options_Lights_triggered()
{
    // run the raytrace_command2 for the light setup dialog
    // there is no command registered for this so call the function directly
    // KMJ ToDo: add a command for this???
    int state;
    void *data;
    raytrace_command2(&state,NULL,&data);
/*
    int commandid = 424;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
*/
}

#ifdef _MAC
void QtMainWindow::on_action_Options_Preferences_triggered()
{
    int commandid = 417;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
#endif

void QtMainWindow::on_action_Options_System_triggered()
{
    int commandid = 417;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

// transform slots
void QtMainWindow::on_action_Transform_Stretch_triggered()
{
    int commandid = 300;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Translate_Copy_triggered()
{
    int commandid = 301;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Move_triggered()
{
    int commandid = 306;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Copy_Array_triggered()
{
    int commandid = 307;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Copy_Along_triggered()
{
    int commandid = 308;// runs the - copyalong_command
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Project_triggered()
{
    int commandid = 302;// runs the - project_command
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Scale_triggered()
{
    int commandid = 303;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Mirror_triggered()
{
    int commandid = 304;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Transform_Rotate_triggered()
{
    int commandid = 305;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

// arrange slots
void QtMainWindow::on_action_Arrange_To_front_triggered()
{
    int commandid = 800;// calls arrange_command1
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Arrange_To_back_triggered()
{
    int commandid = 801;// calls arrange_command2
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Arrange_Forward_one_triggered()
{
    int commandid = 802;// calls arrange_command3
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Arrange_Back_one_triggered()
{
    int commandid = 803;// calls arrange_command4
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

// tools slots
void QtMainWindow::on_action_Tools_Customize_Display_custom_menu_horizontal_triggered()
{
    // calls custom_command19()
    int commandid = 918;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Tools_Customize_Display_custom_menu_vertical_triggered()
{
    // calls custom_command15()
    int commandid = 912;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Tools_Customize_Add_to_custom_menu_triggered()
{
    // calls custom_command13()
    int commandid = 913;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Tools_Customize_Delete_from_custom_menu_triggered()
{
    // calls custom_command14()
    int commandid = 914;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Tools_Customize_Keyboard_Shortcuts_triggered()
{
    // calls keyboard_command()
    int commandid = 917;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Tools_Load_application_triggered()
{
    // calls custom_command12()
    int commandid = 911;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Tools_Unload_application_triggered()
{
    // calls custom_command18()
    int commandid = 916;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

// window slots
/*
void QtMainWindow::on_action_Window_Display_3D_view_triggered()
{
    int commandid = 500;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Display_composed_drawing_triggered()
{
    int commandid = 501;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Cascade_triggered()
{
    ((QMdiArea*)centralWidget())->cascadeSubWindows();
}
*/

void QtMainWindow::on_action_Window_Tile_triggered()
{
#ifndef STRESS_TESTS
    ((RCCentralWidget*)centralWidget())->tile();
    isTiled = true;
#else
    testTiling();
#endif
}

void QtMainWindow::on_action_Window_Tile_Horizontal_triggered()
{
#ifndef STRESS_TESTS
    ((RCCentralWidget*)centralWidget())->tileHorizontal();
    isTiled = true;
#else
    testSplitHorizontal();
#endif
}

void QtMainWindow::on_action_Window_Tile_Vertical_triggered()
{
#ifndef STRESS_TESTS
    ((RCCentralWidget*)centralWidget())->tileVertical();
    isTiled = true;
#else
    testSplitVertical();
#endif
}
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Create_triggered()
{
    int commandid = 524;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Delete_triggered()
{
    int commandid = 525;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Rename_triggered()
{
    int commandid = 571;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Add_3D_view_triggered()
{
    int commandid = 526;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Change_3D_view_triggered()
{
    int commandid = 556;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Delete_3D_view_triggered()
{
    int commandid = 527;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Scroll_3D_view_triggered()
{
    int commandid = 530;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Fit_2D_view_triggered()
{
    int commandid = 558;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Align_view_triggered()
{
    int commandid = 566;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Show_menu_triggered()
{
    int commandid = 508;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Compose_Drawing_Copy_triggered()
{
    int commandid = 567;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Create_triggered()
{
    int commandid = 506;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Delete_triggered()
{
    int commandid = 507;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_All_triggered()
{
    int commandid = 508;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Eye_point_triggered()
{
    int commandid = 509;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Reference_point_triggered()
{
    int commandid = 510;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Modify_incremental_triggered()
{

}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Architectural_perspective_triggered()
{
    int commandid = 511;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Focal_distance_triggered()
{
    int commandid = 512;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Clip_planes_triggered()
{
    int commandid = 513;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_ChangeTo_perspective_triggered()
{
    int commandid = 514;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_ChangeTo_parallel_triggered()
{
    int commandid = 515;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Rotate_triggered()
{
    int commandid = 516;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_To_workplane_triggered()
{
    int commandid = 517;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Dynamic_rotate_triggered()
{
    int commandid = 518;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Dynamic_rotate_about_axis_triggered()
{
    int commandid = 561;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Dynamic_360_rotate_triggered()
{
    int commandid = 555;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Dynamic_walk_triggered()
{
    int commandid = 519;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Dynamic_fly_triggered()
{
    int commandid = 520;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Plot_scale_triggered()
{
    int commandid = 521;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_3Dview_Change_Reset_to_standard_triggered()
{
    int commandid = 522;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Zone_Create_triggered()
{
    int commandid = 535;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Zone_Delete_triggered()
{
    int commandid = 536;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Shade_triggered()
{
    int commandid = 5104; // runs viewstyle_command4()
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Hide_triggered()
{
    int commandid = 529;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/
/*
void QtMainWindow::on_action_Window_Hide_and_dash_hidden_lines_triggered()
{
    int commandid = 569;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}
*/

// Popup_Menu actions
void QtMainWindow::on_action_Popup_Menu_LocateReferencePoint_triggered()
{
    lastContextId = 2001;
}

void QtMainWindow::on_action_Popup_Menu_LocateNear_triggered()
{
    lastContextId = 2002;
}

void QtMainWindow::on_action_Popup_Menu_LocateAlong_triggered()
{
    lastContextId = 2003;
}

void QtMainWindow::on_action_Popup_Menu_LocateOrigin_triggered()
{
    lastContextId = 2004;
}

void QtMainWindow::on_action_Popup_Menu_LocateEndPoint_triggered()
{
    lastContextId = 2005;
}

void QtMainWindow::on_action_Popup_Menu_LocateIntersection_triggered()
{
    lastContextId = 2007;
}

void QtMainWindow::on_action_Popup_Menu_LocateOffsetPoint_triggered()
{
    lastContextId = 2008;
}

void QtMainWindow::on_action_Popup_Menu_LocateApparentIntersection_triggered()
{
    lastContextId = 2009;
}

void QtMainWindow::on_action_Popup_Menu_ZoomWindow_triggered()
{
    lastContextId = 2101;
}

void QtMainWindow::on_action_Popup_Menu_ZoomScroll_triggered()
{
    lastContextId = 2102;
}

void QtMainWindow::on_action_Popup_Menu_ZoomIn_triggered()
{
    lastContextId = 2103;
}

void QtMainWindow::on_action_Popup_Menu_ZoomOut_triggered()
{
    lastContextId = 2104;
}

void QtMainWindow::on_action_Popup_Menu_ZoomFit_triggered()
{
    lastContextId = 2106;
}

void QtMainWindow::on_action_Popup_Menu_ZoomPrevious_triggered()
{
    lastContextId = 2107;
}

void QtMainWindow::on_action_Popup_Menu_SelectChain_triggered()
{
    lastContextId = 2006;
}

void QtMainWindow::on_action_Popup_Menu_SelectPolyWindow_triggered()
{
    lastContextId = 2010;
}

void QtMainWindow::on_action_Popup_Menu_ViewByRotation_triggered()
{
    lastContextId = 518;
}

void QtMainWindow::on_action_Popup_Menu_SnapDragSettings_triggered()
{
    lastContextId = 2406;
}

// PopUp_Menu1 actions
void QtMainWindow::on_action_Popup1_Menu_TrimLength_triggered()
{
    lastContextId = 1902;
}

void QtMainWindow::on_action_Popup1_Menu_Trim_triggered()
{
    lastContextId = 1901;
}

void QtMainWindow::on_action_Popup1_Menu_Corner_triggered()
{
    lastContextId = 1910;
}

void QtMainWindow::on_action_Popup1_Menu_Explode_triggered()
{
    lastContextId = 1909;
}

void QtMainWindow::on_action_Popup1_Menu_Properties_triggered()
{
    lastContextId = 60000;
}

void QtMainWindow::on_action_Popup1_Menu_Material_triggered()
{
    lastContextId = 60001;
}

void QtMainWindow::on_action_Popup1_Menu_DimensionAddPoint_triggered()
{
    lastContextId = 1812;
}

void QtMainWindow::on_action_Popup1_Menu_DimensionMovePoint_triggered()
{
    lastContextId = 1814;
}

void QtMainWindow::on_action_Popup1_Menu_DimensionDeletePoint_triggered()
{
    lastContextId = 1813;
}

void QtMainWindow::on_action_Popup1_Menu_DimensionMoveText_triggered()
{
    lastContextId = 1811;
}

void QtMainWindow::on_action_Popup1_Menu_EditText_triggered()
{
    lastContextId = 1602;
}

// select menu actions
void QtMainWindow::on_action_Select_SameColour_triggered()
{
    lastContextId = 60002;
}

void QtMainWindow::on_action_Select_SameLayer_triggered()
{
    lastContextId = 60003;
}

void QtMainWindow::on_action_Select_SameStyle_triggered()
{
    lastContextId = 60004;
}

void QtMainWindow::on_action_Select_SameWeight_triggered()
{
    lastContextId = 60005;
}

void QtMainWindow::on_action_Select_SameEntity_triggered()
{
    lastContextId = 60006;
}

void QtMainWindow::on_action_Select_SameName_triggered()
{
    lastContextId = 60007;
}

// hide menu actions
void QtMainWindow::on_action_Hide_SameColour_triggered()
{
    lastContextId = 60010;
}

void QtMainWindow::on_action_Hide_SameLayer_triggered()
{
    lastContextId = 60011;
}

void QtMainWindow::on_action_Hide_SameStyle_triggered()
{
    lastContextId = 60012;
}

void QtMainWindow::on_action_Hide_SameWeight_triggered()
{
    lastContextId = 60013;
}

void QtMainWindow::on_action_Hide_SameEntity_triggered()
{
    lastContextId = 60014;
}

void QtMainWindow::on_action_Hide_SameName_triggered()
{
    lastContextId = 60015;
}

void QtMainWindow::on_action_Popup1_UnhideAll_triggered()
{
    int commandid = 60016;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

// divide menu actions
void QtMainWindow::on_action_Divide_Entity_triggered()
{
    lastContextId = 1903;
}

void QtMainWindow::on_action_Divide_Length_triggered()
{
    lastContextId = 1904;
}

void QtMainWindow::on_action_Divide_Number_triggered()
{
    lastContextId = 1905;
}

void QtMainWindow::on_action_Delete_Segment_triggered()
{
    lastContextId = 1908;
}

// clone menu actions
void QtMainWindow::on_action_Clone_Colour_triggered()
{
    lastContextId = 225;
}

void QtMainWindow::on_action_Clone_Layer_triggered()
{
    lastContextId = 226;
}

void QtMainWindow::on_action_Clone_Style_triggered()
{
    lastContextId = 227;
}

void QtMainWindow::on_action_Clone_Weight_triggered()
{
    lastContextId = 228;
}

void QtMainWindow::on_action_Clone_All_triggered()
{
    lastContextId = 229;
}

void QtMainWindow::onViewActivated(RCView *view)
{
    View3dWindow *window = cadwindow->matchview3dwindow(view);
    if(window != 0 && window != cadwindow->getcurrentwindow())
    {
        //qDebug() << "activate view name : " << QString(window->getview()->getname());
        //cadwindow->selectwindow(window);
        cadwindow->setcurrentwindow(window);
        window->paintevent();
    }
}

View3dWindow *QtMainWindow::activeWindow()
{
    return cadwindow->getcurrentwindow();
}

RCView *QtMainWindow::activeView()
{
    View3dWindow *window = cadwindow->getcurrentwindow();
    if(window)
        return (RCView*)window->gethwnd();
    else
        return 0;
}

void QtMainWindow::onViewResize(RCView *view)
{
    View3dWindow *window = cadwindow->matchview3dwindow(view);
    if(window != 0) {
        //qDebug() << "resize view name : " << QString(window->getview()->getname());
        //cadwindow->sizeevent(-1,-1);
    }
}

void QtMainWindow::onViewClosed(RCView *view)
{
    qDebug() << "in QtMainWindow::closeSubWindow";
    if(cadwindow != 0)
    {
        centerWidget->debugView3dList();
        View3dWindow *win = cadwindow->matchview3dwindow(view);
        if(win != 0) {
            UINT msg;
            WPARAM wParam;
            LPARAM lParam;

            msg = WM_CLOSE;

            // click OK
            wParam = MAKEWPARAM((WORD)0,(WORD)0);
            lParam = (LPARAM)0;
            int status = MainEventProc((HWND)win->gethwnd(),msg,wParam,lParam);
            if(status == 1) {
                //delete win;
                //view->setView3dWindow(0);
                centerWidget->deleteView(view);
                centerWidget->debugView3dList();
                centerWidget->debug();
            }
        }
    }
}

void QtMainWindow::showFullScreen()
{

    //app->processEvents();
    QMainWindow::showFullScreen();
    app->processEvents();
}

void QtMainWindow::on_action_Window_Show_FullScreen_triggered()
{
    //qDebug() << "Entering Show FullScreen";
#ifdef _MAC
    showFullScreen();
#else
    QMainWindow::showFullScreen();
#endif
    //setWindowState(windowState() ^ Qt::WindowFullScreen);
    //showFullScreen();
    //qDebug() << "Leaving Show FullScreen";
}

void QtMainWindow::on_action_Window_Show_Maximised_triggered()
{
    //qDebug() << "Entering Show Maximised";
    //app->processEvents();
    QMainWindow::showMaximized();
    app->processEvents();
    //qDebug() << "Leaving Show Maximsed";
}

void QtMainWindow::on_action_Window_Show_Normal_triggered()
{
    //qDebug() << "Entering Show Normal";
    //app->processEvents();
    QMainWindow::showNormal();
    app->processEvents();
    //qDebug() << "Leaving Show Normal";
}

void QtMainWindow::on_action_Window_Tool_bars_triggered()

{
    // calls toolmenu_command()
    int commandid = 531;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}


void QtMainWindow::on_action_Help_Contents_and_Index_triggered()
{

}

void QtMainWindow::on_action_Help_Current_command_triggered()
{

}

void QtMainWindow::on_action_Help_View_RealCAD_Reference_command_triggered()
{
    // calls help_command3()
    int commandid = 603;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_View_RealCAD_GettingStarted_command_triggered()
{
    // calls help_command6()
    int commandid = 606;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Help_View_LANDWorksCAD_Reference_command_triggered()
{
    // calls help_command10()
    int commandid = 610;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Help_View_LANDWorksCAD_GettingStarted_command_triggered()
{
    // calls help_command11()
    int commandid = 611;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Help_View_Training_Videos_command_triggered()
{
    // calls help_command4()
    int commandid = 604;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Help_Visit_RealCAD_Website_command_triggered()
{
    // calls help_command7()
    int commandid = 607;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Help_Visit_LANDWorksCAD_Website_command_triggered()
{
    // calls help_command7()
    int commandid = 607;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }

}

void QtMainWindow::on_action_Help_ProgramUpdates_command_triggered()
{
    // calls help_command8()
    int commandid = 608;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_TechnicalSupport_command_triggered()
{
    // calls help_command9()
    int commandid = 609;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_RemoteAssistance_command_triggered()
{
    //calls - help_ShareMyScreen_command()
    int commandid = 609;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
/*
    QString command,program,arguments;
#if defined(Q_OS_WIN)
    if(QFileInfo::exists(QString(home.getpublicfilename(_RCT("CAD-International-Remote.exe")))))
        command = QString(home.getpublicfilename(_RCT("CAD-International-Remote.exe")));
    else if(QFileInfo::exists(QString(home.getexecutablefilename(_RCT("CAD-International-Remote.exe")))))
        command = "\"" + QString(home.getexecutablefilename(_RCT("CAD-International-Remote.exe"))) + "\"";
#elif defined(Q_OS_MAC)
    if(QFileInfo::exists(QString(home.getexecutablefilename(_RCT("TeamViewerQS.app")))))
        program = QString(home.getexecutablefilename(_RCT("TeamViewerQS.app")));
    else if(QFileInfo::exists(QString(home.getpublicfilename(_RCT("TeamViewerQS.app")))))
        program = QString(home.getpublicfilename(_RCT("TeamViewerQS.app")));
    command = QString(_RCT("open ")) + program;
#endif

    QProcess *myProcess = new QProcess(this);
    if( !myProcess->startDetached(command))
        cadwindow->MessageBox(_RCT("Your Remote Assistance executable is missing or corrupted!\nPlease contact CAD International support for further assistance."),_RCT("RealCAD"),MB_ICONEXCLAMATION);
*/
}

// new help menu items
void QtMainWindow::on_action_Help_Manual_triggered()
{
    // calls help_Manual_command()
    int commandid = 606;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_CheatSheet_triggered()
{
    // calls help_CheatSheet_command()
    int commandid = 607;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_GetHelp_triggered()
{
    // calls help_GetHelp_command()
    int commandid = 608;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_WelcomeScreen_triggered()
{
    // show the splash screen dialog
    int licensed = check_license((char*)"Main CAD Application", (char*)"RealCADKey1", (char*)"RealCADKey2",0,0);
    showSplashScreen(licensed);
}

void QtMainWindow::on_action_Help_ShareMyScreen_triggered()
{
    //calls - help_ShareMyScreen_command()
    int commandid = 609;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
/*
    QString command,program,arguments;
#if defined(Q_OS_WIN)
    if(QFileInfo::exists(QString(home.getpublicfilename(_RCT("CAD-International-Remote.exe")))))
        command = QString(home.getpublicfilename(_RCT("CAD-International-Remote.exe")));
    else if(QFileInfo::exists(QString(home.getexecutablefilename(_RCT("CAD-International-Remote.exe")))))
        command = "\"" + QString(home.getexecutablefilename(_RCT("CAD-International-Remote.exe"))) + "\"";
#elif defined(Q_OS_MAC)
    if(QFileInfo::exists(QString(home.getexecutablefilename(_RCT("TeamViewerQS.app")))))
        program = QString(home.getexecutablefilename(_RCT("TeamViewerQS.app")));
    else if(QFileInfo::exists(QString(home.getpublicfilename(_RCT("TeamViewerQS.app")))))
        program = QString(home.getpublicfilename(_RCT("TeamViewerQS.app")));
    command = QString(_RCT("open ")) + program;
#endif

    //cadwindow->MessageBox(command.data(),_RCT("RealCAD"),MB_ICONEXCLAMATION);

    QProcess *myProcess = new QProcess(this);
    if( !myProcess->startDetached(command))
        cadwindow->MessageBox(_RCT("Your Remote Assistance executable is missing or corrupted!\nPlease contact CAD International support for further assistance."),_RCT("RealCAD"),MB_ICONEXCLAMATION);
*/
}

void QtMainWindow::on_action_Help_OnlineTraining_triggered()
{
    //calls - help_OnlineTraining_command()
    int commandid = 613;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_CheckForUpdates_triggered()
{
    // calls help_CheckForUpdates_command()
    int commandid = 610;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_DeactivateLicense_triggered()
{
    // calls help_DeactivateLicense_command()
    int commandid = 611;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_TellaFriend_triggered()
{
    // calls help_TellaFriend_command()
    int commandid = 612;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_action_Help_About_triggered()
{
    // calls the about_command()
    int commandid = 601;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformMoveShortCut_activated()
{
    // calls the move_command()
    int commandid = 306;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformCopyShortCut_activated()
{
    int commandid = 301;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformScaleShortCut_activated()
{
    int commandid = 303;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformRotateShortCut_activated()
{
    int commandid = 305;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformMirrorShortCut_activated()
{
    int commandid = 304;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformStretchShortCut_activated()
{
    int commandid = 300;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformCopyArrayShortCut_activated()
{
    int commandid = 307;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformCopyAlongShortCut_activated()
{
    int commandid = 308;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformProjectShortCut_activated()
{
    int commandid = 302;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformOffsetDigitizeShortCut_activated()
{
    int commandid = 1907;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::on_TransformOffsetDistanceShortCut_activated()
{
    int commandid = 1906;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->processEvents();
    }
}

void QtMainWindow::screenChanged(QScreen *screen)
{
 qDebug() << "in QtMainWindow::screenChanged";
}

void QtMainWindow::screenCountChanged(int count)
{
 qDebug() << "in QtMainWindow::screenCountChanged";
}

void QtMainWindow::testSplitHorizontal(int cycles)
{
    for(int j=0; j<cycles; j++)
    {
        for(int i=0; i<4; i++)
        {
            RCView *pView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();
            if(pView)
                centerWidget->splitView(pView,Qt::Horizontal);
            app->processEvents();
        }
        for(int i=0; i<4; i++)
        {
            RCView *pView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();
            if(pView)
            {
                centerWidget->deleteView(pView);
            }
            app->processEvents();
            centerWidget->debugView3dList();
            centerWidget->debug();
        }
    }
}

void QtMainWindow::testSplitVertical(int cycles)
{
    for(int j=0; j<cycles; j++)
    {
        for(int i=0; i<4; i++)
        {
            RCView *pView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();
            if(pView)
                centerWidget->splitView(pView,Qt::Vertical);
            app->processEvents();
        }
        for(int i=0; i<4; i++)
        {
            RCView *pView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();
            if(pView)
            {
                centerWidget->deleteView(pView);
            }
            app->processEvents();
            centerWidget->debugView3dList();
            centerWidget->debug();
        }
    }
}

void QtMainWindow::testTiling(int cycles)
{
    for(int i=0; i<4; i++)
    {
        RCView *pView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();
        if(pView)
            centerWidget->splitView(pView,Qt::Horizontal);
        app->processEvents();
    }
    app->processEvents();

    for(int j=0; j<cycles; j++)
    {
        ((RCCentralWidget*)centralWidget())->tile();
        app->processEvents();
        ((RCCentralWidget*)centralWidget())->tileVertical();
        app->processEvents();
        ((RCCentralWidget*)centralWidget())->tileHorizontal();
        app->processEvents();
        isTiled = true;
    }
    app->processEvents();

    for(int i=0; i<4; i++)
    {
        RCView *pView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();
        if(pView)
            centerWidget->deleteView(pView);
        app->processEvents();
    }
}

void QtMainWindow::testNew(int cycles)
{
    int commandid = 100;
    for(int i=0; i < 2000; i++)
    {
        qInfo() << "init #" << i;
        if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL) {
            NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
            state.sendevent(nevent);
            state.process();
            app->processEvents();
        }
        app->processEvents();
    }
}

void QtMainWindow::resetButtonMenus()
{
    cadwindow->resetbuttonmenus();
}
