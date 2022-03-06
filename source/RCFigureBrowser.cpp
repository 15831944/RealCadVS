
//#include <QObject>
#include <QtEvents>
#include <QGridLayout>
#include <QSettings>
#include <QDir>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "RCView.h"
#include "RCGraphicsView.h"
#include "rcopenglwidget.h"
#include "rcgraphicspreview.h"
#include "RCFigureBrowser.h"
#include "ui_RCFigureBrowser.h"

//static QMutex lockit;

QPixmap getOneImage(const QString &qfigurename)
{
    // set up the temporary preview storage path
    QString appTitle(home.getApplicationTitle());
    QFileInfo figureFileInfo(qfigurename);

    // get the file type, add new type as required
    QString qfigureExt = figureFileInfo.suffix();
    char imageType[4]={"jpg"};
    if(qfigureExt.contains("jpg",Qt::CaseInsensitive))
        strcpy(imageType,"JPG");
    else if(qfigureExt.contains("jpeg",Qt::CaseInsensitive))
        strcpy(imageType,"JPEG");
    else if(qfigureExt.contains("png",Qt::CaseInsensitive))
        strcpy(imageType,"PNG");

    QString qfigurePath(QDir::toNativeSeparators(figureFileInfo.dir().absolutePath()));
    QString qpreviewname = figureFileInfo.fileName().append(".png");
    QString tempPath = QDir::toNativeSeparators(QDir::tempPath());
    int index = qfigurePath.indexOf(appTitle);
    if(index < 0)
        index = qfigurePath.indexOf(QDir::rootPath()) + QDir::rootPath().length();
    else
        index += appTitle.length();
    if(qfigurePath.mid(index).startsWith(QDir::separator()))
        index++;
    if( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    tempPath += appTitle +  QDir::toNativeSeparators("/PreviewCache/") + qfigurePath.mid(index);
    if( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    // create the temporary folder if needed
    QDir imageDir;
    if(!QFileInfo::exists(tempPath))
        imageDir.mkpath(tempPath);
    // add the preview file name
    tempPath = QDir::toNativeSeparators(tempPath + qpreviewname);

    QPixmap emptyImage(100,100);
    emptyImage.fill(QColor(255,255,255));

    //if(qfigurename.contains(".cad",Qt::CaseInsensitive))
    if(qfigureExt.contains("cad",Qt::CaseInsensitive))
    {
        bool tempExists = QFileInfo::exists(tempPath);
        if(tempExists)
        {
            QFileInfo figureInfo(qfigurename);
            QFileInfo previewInfo(tempPath);

            if(previewInfo.created() < figureInfo.lastModified())
            {
                QFile::remove(tempPath);
                //QPixmapCache::remove(tempPath);
            }
        }

        if(tempExists)
        {
            QPixmap image;
            //if(!QPixmapCache::find(tempPath,image))
            {
                if(image.load(tempPath,"PNG"))
                {
                    if(!image.isNull())
                    {
                        //QPixmapCache::insert(tempPath,image);
                        return image;
                    }
                }
            }
        }
        else
        {
            // get the header image
            uchar *preview=0;
            int width=0,height=0,bpp=3,status=0;
            // init the default images
            //QImage image(256,256,QImage::Format_RGB888);
            QImage image;//(100,100,QImage::Format_RGB888);
            //image.fill(QColor(255,255,255));
            //QPixmap pixmap(256,256);
            QPixmap pixmap(100,100);
            pixmap.fill(QColor(255,255,255));
            //lockit.lock();
            CadFile infile;
            //lockit.unlock();

            //if(!QPixmapCache::find(tempPath,pixmap))
            {
                //lockit.lock();
                if(infile.open(qfigurename.data(),"rb"))
                {
                    if(infile.readheaderimage(preview, width, height, bpp) >= 0)
                    {
                        if((width * height) != 0 && (bpp == 3 || bpp == 4))
                        {
                            //int stride = ((width*3 + 3)/4)*4;
                            int stride = width * 3;
                            int gw = 100;
                            int gh = 100;
                            image = QImage(preview,width,height,stride,QImage::Format_RGB888).scaled(gw,gh,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                            //image.save("preview.bmp","bmp");
                        }
                    }
                }
                infile.close();
                //lockit.unlock();
                if(!image.isNull())
                {
                    pixmap.fromImage(image);
                    if(!pixmap.isNull())
                    {
                        pixmap.save(tempPath,"PNG");
                        //QPixmapCache::insert(tempPath,pixmap);
                        return pixmap;
                    }
                }
                else
                {
                    double gw = 100;
                    double gh = 100;
                    //lockit.lock();
                    View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,-1E10,1E10);
                    View3dBitmapOutput output(&top,1.0,gw,gh);
                    //lockit.unlock();
                    output.getScene()->setBackgroundBrush(QBrush(QColor(255,255,255)));

                    //db.geometry.extents(&output,&pmin,&pmax);

                    double lastdbr,lastdbg,lastdbb;
                    db.colourtable[0].get(&lastdbr,&lastdbg,&lastdbb);
                    if(output.getsurfacetype() == View3dSurf)
                    {
                        // get the current defaults for the header
                        //lockit.lock();
                        int l  = db.header.getlayer();
                        int c  = db.header.getcolour();
                        int st = db.header.getstyle();
                        int w  = db.header.getweight();
                        //lockit.unlock();
                        EntityHeader header(0,0,0,0);
                        header.change(c,l,st,w);
                        BitMask options(32);

                        //options.set(FigureOriginalColour,ui->originalColour->isChecked());
                        //options.set(FigureOriginalStyle,ui->originalStyle->isChecked());
                        //options.set(FigureOriginalWeight,ui->originalWeight->isChecked());

                        options.set(FigureOriginalColour,1);
                        options.set(FigureOriginalStyle,0);
                        options.set(FigureOriginalWeight,0);

                        Figure figure1((RCCHAR*)qfigurename.data(),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
                        Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
                        figure1.extents(0,&pmin,&pmax);

                        double s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
                        if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
                          s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
                        if (s < 0.00001)
                          s = 1.0;

                        s *= 1.1;

                        header.change(0,0,0,0);
                        //lockit.lock();
                        db.colourtable[0].set(0.0,0.0,0.0);
                        //lockit.unlock();
                        Point org;
                        org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
                        Figure figure2(header,(RCCHAR*)qfigurename.data(),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
                        //Figure figure2(_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
                        //lockit.lock();
                        db.dismask.layer.set(db.header.getlayer(),1);
                        //lockit.unlock();

                        //figure2.extents(0,&pmin,&pmax);

                        //double du = (pmax.x - pmin.x) * 0.02;
                        //double dv = (pmax.y - pmin.y) * 0.02;

                        //output.zoom(pmin.x-du,pmin.y-dv,pmax.x+du,pmax.y+dv);
                        //double dtol = db.getdtoler();
                        //db.setdtoler(50.0);

                        figure2.qgi=0;
                        figure2.draw(DM_NORMAL,NULL,&output);
                    }
                    output.render();
                    //lockit.lock();
                    db.colourtable[0].set(lastdbr,lastdbg,lastdbb);
                    //lockit.unlock();
                    QPixmap pixmap = *(QPixmap*)output.getBitmap();
                    if(!pixmap.isNull())
                    {
                        //pixmap.save("preview.jpg","JPG");
                        // save it to the temp cache for next time around
                        pixmap.save(tempPath,"PNG");
                        //QPixmapCache::insert(tempPath,pixmap);
                        return pixmap;
                    }
                }
            }
        }
    }
    else
    {
        bool tempExists = QFileInfo::exists(tempPath);
        if(tempExists)
        {
            QFileInfo figureInfo(qfigurename);
            QFileInfo previewInfo(tempPath);

            if(previewInfo.created() < figureInfo.lastModified())
            {
                QFile::remove(tempPath);
                tempExists = false;
                //QPixmapCache::remove(tempPath);
            }
        }

        if(!tempExists)
        {
            QPixmap image;
            if(image.load(qfigurename))
            {
                if(!image.isNull())
                {
                    int gw = 100;
                    int gh = 100;
                    image = image.scaled(gw,gh,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                    if(!image.isNull())
                    {
                        image.save(tempPath,"PNG");
                        //QPixmapCache::insert(tempPath,image);
                        return image;
                    }
                }
            }
        }
        else
        {
            QPixmap image;
            //if(!QPixmapCache::find(tempPath,image))
            {
                if(image.load(tempPath,"PNG"))
                {
                    if(!image.isNull())
                    {
                        //QPixmapCache::insert(tempPath,image);
                        return image;
                    }
                }
            }
        }
    }
    return emptyImage;
}

BaseWidget::BaseWidget(QWidget *browserwidget, QWidget *parent)
    : QWidget(parent), browser(browserwidget)
{
    thumbnail = new QFutureWatcher<QPixmap>;

    connect(thumbnail, SIGNAL(resultReadyAt(int)), SLOT(showImage(int)));
    connect(thumbnail, SIGNAL(finished()), SLOT(finished()));

}

BaseWidget::~BaseWidget()
{
    thumbnail->cancel();
    thumbnail->waitForFinished();
}


void BaseWidget::getImages(const QStringList &fileList)
{
    // Cancel and wait if we are already loading images.
    if (thumbnail->isRunning()) {
        thumbnail->cancel();
        thumbnail->waitForFinished();
    }

    QStringList files = fileList;

    // Use mapped to run the thread safe scale function on the files.
    thumbnail->setFuture(QtConcurrent::mapped(files, getOneImage));
}

void BaseWidget::showImage(int num)
{
    if(!thumbnail->resultAt(num).isNull())
    {
        //views[num]->scene()->addPixmap(thumbnail->resultAt(num));

        QWidget *widget = views[num];
        QGraphicsScene *scene=0;
        QPixmap image = thumbnail->resultAt(num);

        if(((QGraphicsView*)widget)->scene() == 0)
        {
            scene = new QGraphicsScene();
            scene->setSceneRect(image.rect());
            ((QGraphicsView*)widget)->setScene(scene);
            ((QGraphicsView*)widget)->setSceneRect(image.rect());
        }
        else
        {
            scene = ((QGraphicsView*)widget)->scene();
            //scene->clear();
            scene->setSceneRect(image.rect());
            qDebug() << "scene Rect"<< scene->sceneRect();
            ((QGraphicsView*)widget)->setSceneRect(image.rect());
        }

        //qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
        //qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
        //qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
        //qDebug() << "scene Rect"<< scene->sceneRect();

        QGraphicsPixmapItem *gitem = ((QGraphicsView*)widget)->scene()->addPixmap(image);
        scene->setSceneRect(gitem->boundingRect());
        qDebug() << scene->sceneRect();
        QRectF srect = scene->sceneRect();
        QRectF nrect = srect;

        if(srect.height() >= srect.width())
        {
            nrect.setWidth(srect.height());
            nrect.setHeight(srect.height());
        }
        else
        {
            nrect.setWidth(srect.width());
            nrect.setHeight(srect.width());
        }
        scene->setSceneRect(nrect);
        nrect = scene->sceneRect();
        //qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
        //qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
        //qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
        //qDebug() << "scene Rect"<< scene->sceneRect();
        ((QGraphicsView*)widget)->setProperty("bbox",scene->itemsBoundingRect());
        ((QGraphicsView*)widget)->fitInView(nrect,Qt::KeepAspectRatio);
        ((QGraphicsView*)widget)->centerOn(nrect.center());
        ((QGraphicsView*)widget)->show();
    }
}

void BaseWidget::finished()
{
    RCCHAR text[600];
    //app->processEvents();
    int currentFileIndex = ((RCFigureBrowser*)browser)->GetCurrentFileIndex();
    sprintf(text,"FilePreviewItem-%d",currentFileIndex >= 0 ? currentFileIndex : 0);
    ((RCFigureBrowser*)browser)->selectFigureListItem(text);
    ((RCFigureBrowser*)browser)->SyncFolderTree(((RCFigureBrowser*)browser)->m_rootDir);
    show();
}

void previewData::handleFinished()
{
    qDebug() << "in handleFinished()";

    RCHWND hwnd = ((QFutureWatcher<previewData*>)sender()).result()->mhwnd;
    QGraphicsScene *scene = ((QGraphicsView*)hwnd)->scene();

    if(scene == 0)
        scene = new QGraphicsScene();
    scene->clear();
    ((QGraphicsView*)hwnd)->setScene(0);

    QGraphicsItem *gitem = ((QFutureWatcher<previewData*>)sender()).result()->mgitem;

    if(gitem != 0)
    {
        scene->addItem(gitem);
        ((QGraphicsView*)hwnd)->setScene(scene);
        gitem->setData(0,"figure");
        ((QGraphicsView*)hwnd)->fitInView(gitem,Qt::KeepAspectRatio);
        ((QGraphicsView*)hwnd)->centerOn(gitem);
        ((QGraphicsView*)hwnd)->show();
    }
    ((QGraphicsView*)hwnd)->setProperty("bbox",scene->sceneRect());

}

void someThreadedFunction()
{
    qDebug() << "Start someThreadedFunction()";

    qDebug() << "End someThreadedFunction()";
}

previewData * PreviewFigureMTWorker(previewData *pData)
{
    qDebug() << "Start PreviewFigureMTWorker()";

    View top(_RCT("Top"),pData->mpersp,0, pData->meye.x, pData->meye.y, pData->meye.z, pData->mref.x, pData->mref.y, pData->mref.z, pData->mup.x, pData->mup.y, pData->mup.z,1E10,-1E10);
    View3dOutput output(pData->mhwnd,0,&top,1.0);
    EntityHeader header(0,0,0,0);
    Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation;
    Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
    BitMask options(32);
    double s;
    int l,c,st,w;

    //QGraphicsScene *scene = new QGraphicsScene();
    QGraphicsScene *scene = ((QGraphicsView*)pData->mhwnd)->scene();
    pData->mscene = scene;

    output.setScene(scene);
    scene->setSceneRect(QRect());

    // get the current defaults for the header
    l  = db.header.getlayer();
    c  = db.header.getcolour();
    st = db.header.getstyle();
    w  = db.header.getweight();
    header.change(c,l,st,w);

    //options.set(FigureOriginalColour,ui->originalColour->isChecked());
    //options.set(FigureOriginalStyle,ui->originalStyle->isChecked());
    //options.set(FigureOriginalWeight,ui->originalWeight->isChecked());

    Figure figure1(_RCT(pData->mfigurename),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
    figure1.extents(0,&pmin,&pmax);

    s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
    if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
      s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
    if (s < 0.00001)
      s = 1.0;

    s *= 1.1;

    org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
    //Figure figure2(header,_RCT(pData.mfigurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
    Figure figure2(_RCT(pData->mfigurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
    db.dismask.layer.set(db.header.getlayer(),1);

    //  Prepare the surface for OpenGl drawing

    //double dtol = db.getdtoler();
    //db.setdtoler(50.0);

    figure2.qgi=0;
    figure2.draw(DM_NORMAL,NULL,&output);

    pData->mgitem = figure2.qgi;

    QGraphicsItem *gitem = figure2.qgi;

    if(gitem != 0)
    {
        scene->addItem(gitem);
        ((QGraphicsView*)pData->mhwnd)->setScene(scene);
        gitem->setData(0,"figure");
        ((QGraphicsView*)pData->mhwnd)->fitInView(gitem,Qt::KeepAspectRatio);
        ((QGraphicsView*)pData->mhwnd)->centerOn(gitem);
        ((QGraphicsView*)pData->mhwnd)->show();
    }
    ((QGraphicsView*)pData->mhwnd)->setProperty("bbox",scene->sceneRect());

    qDebug() << "End someThreadedFunction()";

    return pData;    
}

QString RCFigureBrowser::fixSeperators(const QString & string)
{
    QString out = string;
    QChar sep = QDir::separator();
    if(sep == '\\')
        out = out.replace('/',sep);
    else if(sep == '/')
        out = out.replace('\\',sep);

    return out;
}

QString RCFigureBrowser::fixFileName(const QString & string)
{
    return QDir::toNativeSeparators(fixSeperators(string));
}

RCFigureBrowser::RCFigureBrowser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RCFigureBrowser)
{
    char text[300],recent[600];

    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    m_FileTreeMenu = new QMenu(this);
    m_FileTreeMenu->setObjectName("FileTreeContext_Menu");

    CreateContextMenu();

    m_Prefix = "Browser";
    strcpy(m_rootDir,"");
    strcpy(m_filter,"*.cad;*.jpg;*.jpeg;*.png;");
    strcpy(m_SelectedFileName,"");
    strcpy(m_currentDisplayFolder,"");
    selectedItem = 0;
    selectedView = 0;
    m_Interrupted = false;
    m_initializing=true;
    m_insertingFolderCB=false;
    m_Title = "Select Figure";
    drawtimer = 0;
    stoptimer = 0;
    thumbProgressDlg = 0;
    // v8
    m_UseSize=false;
    mLibrarySize = Point(1000.0,1000.0,0.0);

    connect(ui->ID_OK,  SIGNAL(clicked(bool)),  this,  SLOT(accept()));
    connect(ui->ID_CANCEL,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));

    //ui->IDC_STATICPIC->installEventFilter(this);
    QPixmapCache::setCacheLimit(20000);

}

RCFigureBrowser::RCFigureBrowser(QString rootdir, QString file, QStringList recent, QString filters, QString title, QString prefix, RCFigureBrowser::settingType type,QWidget *parent)
 : QDialog(parent),
   ui(new Ui::RCFigureBrowser)
{
    ui->setupUi(this);

    setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    CreateContextMenu();

    m_Prefix = prefix;
    strcpy(m_rootDir,rootdir.data());
    strcpy(m_filter,filters.data());
    strcpy(m_SelectedFileName,file.data());
    strcpy(m_currentDisplayFolder,m_rootDir);
    selectedItem = 0;
    selectedView = 0;
    m_Interrupted = false;
    m_initializing=true;
    m_insertingFolderCB=false;
    m_RecentPaths = recent;
    m_Title = title;
    drawtimer = 0;
    stoptimer = 0;
    thumbProgressDlg = 0;
    // v8
    m_UseSize=false;
    mLibrarySize = Point(1000.0,1000.0,0.0);

    QPixmapCache::setCacheLimit(20000);
}

void RCFigureBrowser::CreateContextMenu()
{
    Act_ShowHidden = new QAction(this);
    Act_ShowHidden->setObjectName("ACT_ShowHidden");
    Act_ShowHidden->setText("Show hidden files");
    Act_ShowHidden->setCheckable(true);
    m_FileTreeMenu->addAction(Act_ShowHidden);
    Act_ShowHidden->setChecked(false);
    connect(Act_ShowHidden,SIGNAL(triggered()),this,SLOT(SetShowHidden()));
}


RCFigureBrowser::~RCFigureBrowser()
{
    delete ui;
    drawtimer->deleteLater();
}

bool RCFigureBrowser::eventFilter(QObject *obj, QEvent *event)
{
    double angle=0;
    char text[300];

    if (event->type() == QEvent::MouseButtonPress)
    {
        if(((QMouseEvent*)event)->buttons() & Qt::LeftButton)
        {
            if(obj->parent() && obj->objectName().contains("FilePreviewItem"))
            {
                selectFigureListItem((QWidget*)obj);
                if(ui->IDC_FILENAME_COMBO->currentText() != ((QGraphicsView*)obj)->property("filename").toString())
                    ui->IDC_FILENAME_COMBO->setCurrentText(((QGraphicsView*)obj)->property("filename").toString());
                else
                    ui->IDC_FILENAME_COMBO->currentIndexChanged(((QGraphicsView*)obj)->property("filename").toString());
                return true;
            }
        }
        if(((QMouseEvent*)event)->buttons() & Qt::RightButton)
        {
            if(obj->parent() && (obj->objectName().contains("FilePreviewItem") ||
                    obj->objectName() == "IDC_STATICPIC"))
            {
                m_Interrupted = true;
                return true;
            }
        }
    }
    else if(event->type() == QEvent::KeyPress)
    {
        if(((QKeyEvent*)event)->key() == Qt::Key_Space)
        {
            program->setinterrupt(true);
            m_Interrupted = true;
            return true;
        }
    }
    else if(event->type() == QEvent::Wheel)
    {
        // eat the wheel events for the big preview
        if(obj->objectName().contains("IDC_FIGURE_PICTURE"))
            return true;
    }
    return QObject::eventFilter(obj, event);
}

void RCFigureBrowser::showEvent(QShowEvent * event)
{
    qDebug() << "root Dir" << QString(m_rootDir);
    on_IDC_FILENAME_COMBO_currentIndexChanged(QString(m_SelectedFileName));
    ShowImageListWithProgress();
    RCCHAR text[300];
    sprintf(text,"FilePreviewItem-%d",ui->IDC_FILENAME_COMBO->currentIndex());
    selectFigureListItem(text);
    SyncFolderTree(ui->IDC_FOLDER_COMBO->currentText().data());

    // use a timer to record the first position on show
    // this is used to stop the dialog being maximised on the mac
    QTimer::singleShot(200, this, SLOT(getFirstPos()));
}

void RCFigureBrowser::OnInitDialog()
{
    RCCHAR text[300],recent[600];

    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    qDebug() << "ini file path" << QString(home.getinifilename());

    m_initializing = true;

    //setWindowFlag(Qt::WindowSystemMenuHint,true);
    //setWindowFlag(Qt::CustomizeWindowHint,true);
    //setWindowFlag(Qt::WindowMaximizeButtonHint,false);
    //setWindowFlag(Qt::WindowMinimizeButtonHint,false);

    // init the folder to the last used one or the default
    strcpy(text,settings.value(QString("FigureBrowser/%1_Filepath").arg(m_Prefix),QString(home.getFigureFilepath(_RCT("/")))).toString().data());
    if( strlen(m_rootDir) == 0 )
        SetRootDir(text);

    qDebug() << "rootDir : " << QString(text);

    // get the first 10 recent paths
    if(m_RecentPaths.length() < 1)
    {
        for(int i=0; i < 10; i++  )
        {
            QString qrecent = QString("%1_Path%2").arg(m_Prefix).arg(i+1);
            strcpy(recent,qrecent.data());
            strcpy(text,settings.value("FigureBrowser/"+QString(recent),"").toString().data());
            qDebug() << "path : " << QString(text);
            if(strlen(text) > 0)
                m_RecentPaths.append(QString(text));
        }
    }
    for(int i=0; i < m_RecentPaths.length(); i++  )
    {
        if(m_RecentPaths.at(i).length() > 0 && ui->IDC_FOLDER_COMBO->findText(m_RecentPaths.at(i)) < 0)
            ui->IDC_FOLDER_COMBO->addItem(m_RecentPaths.at(i));
    }
    if(m_RecentPaths.length() > 0)
        strcpy(m_rootDir,ui->IDC_FOLDER_COMBO->itemText(0).data());
    // init the folder tree to start at the current Folder
    qDebug() << QString(m_rootDir);
    SetFolderTree(m_rootDir);

    // uncheck the link button
    ui->_1008->setChecked(false);

    if(m_UseSize)
    {
        ui->SizeCheckBox->setChecked(true);
        on_SizeCheckBox_clicked();
    }
    else
    {
        ui->ScaleCheckBox->setChecked(true);
        on_ScaleCheckBox_clicked();
    }

    // get the filenames in the current folder
    SetFileNames();

    m_initializing = false;

    // init the figure file to the last used one or 0
    strcpy(text,settings.value(QString("FigureBrowser/%1_Filename").arg(m_Prefix),"").toString().data());
    int pos = ui->IDC_FILENAME_COMBO->findText(QString(text),Qt::MatchFixedString);
    if( pos >= 0 )
    {
        ui->IDC_FILENAME_COMBO->setCurrentIndex(pos);
        strcpy(m_SelectedFileName,ui->IDC_FILENAME_COMBO->currentText().data());
    }
    else
    {
        ui->IDC_FILENAME_COMBO->setCurrentIndex(0);
        strcpy(m_SelectedFileName,ui->IDC_FILENAME_COMBO->currentText().data());
    }

    sprintf(text,"FilePreviewItem-%d",ui->IDC_FILENAME_COMBO->currentIndex());
    selectFigureListItem(text);

    // reset the scale to the default if use size not set
    if(m_UseSize == false)
    {
        ui->XScale->setValue(v.getreal("fi::scalex"));
        ui->YScale->setValue(v.getreal("fi::scaley"));
        ui->ZScale->setValue(v.getreal("fi::scalez"));
    }

    ui->IDC_FIGURE_PICTURE->installEventFilter(this);

}

void RCFigureBrowser::accept()
{
    RCCHAR fullPath[600];
    RCCHAR selectedPath[600];
    RCCHAR selectedFile[300];
    strcpy(fullPath,"");
    strcpy(selectedFile,"");
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    //recurInterrupt = true;

    //m_FolderCombo.GetWindowText(selectedPath,600);
    strcpy(selectedPath,ui->IDC_FOLDER_COMBO->currentText().data());

    //int sel = m_FileNameCombo.GetCurSel();
    int sel = ui->IDC_FILENAME_COMBO->currentIndex();
    if( sel < 0 )
        sel = 0;

    //if( m_FileNameCombo.GetLBText(sel,selectedFile) != CB_ERR )
    if( ui->IDC_FILENAME_COMBO->itemText(sel).length() > 0)
    {
        strcpy(selectedFile,ui->IDC_FILENAME_COMBO->itemText(sel).data());
        if( strlen(selectedPath)  > 0 )
        {
            strcpy(fullPath,selectedPath);
            strcat(fullPath,QString(QDir::separator()).data());
            strcat(fullPath,selectedFile);
            strcpy(m_SelectedFileName,fullPath);
        }
    }

    strcpy(selectedPath,fixFileName(QString(selectedPath)).data());
    settings.setValue(QString("FigureBrowser/%1_Filepath").arg(m_Prefix),QString(selectedPath));
    strcpy(selectedFile,fixFileName(QString(selectedFile)).data());
    settings.setValue(QString("FigureBrowser/%1_Filename").arg(m_Prefix),QString(selectedFile));

    for( int i=0; i<10; i++ )
    {
        RCCHAR text[300];
        QString qrecent = QString("%1_Path%2").arg(m_Prefix).arg(i+1);
        strcpy(text,qrecent.data());
        qDebug() << "path : " << QString(text);
        if( i < ui->IDC_FOLDER_COMBO->count() )
            strcpy(selectedPath,fixFileName(ui->IDC_FOLDER_COMBO->itemText(i)).data());
        else
            strcpy(selectedPath,"");
        settings.setValue("FigureBrowser/"+QString(text),QString(selectedPath));
    }

    QDialog::accept();
}

RCCHAR* RCFigureBrowser::GetFileName()
{
    return m_SelectedFileName;
}

int RCFigureBrowser::GetCurrentFileIndex()
{
    return ui->IDC_FILENAME_COMBO->currentIndex();
}

RCCHAR* RCFigureBrowser::GetCurrentPath()
{
    return m_rootDir;
}

void RCFigureBrowser::clearPreview(QWidget *widget)
{
    if(qobject_cast<QGraphicsView *>(widget) != 0)
    {
        if(((QGraphicsView*)widget)->scene() !=0 )
            ((QGraphicsView*)widget)->scene()->clear();
    }
}

void RCFigureBrowser::PreviewImage(QWidget *widget, RCCHAR *filepath )
{
    if(qobject_cast<QGraphicsView *>(widget) != 0)
    {
        QGraphicsScene *scene=0;
        QString qfilepath(filepath);
        QPixmap image;

        if(!image.load(qfilepath))
        {
            image = QPixmap(100,100);
            image.fill(QColor(255,255,255));
        }

        if(!image.isNull())
        {
            if(((QGraphicsView*)widget)->scene() == 0)
            {
                scene = new QGraphicsScene();
                scene->setSceneRect(image.rect());
                ((QGraphicsView*)widget)->setScene(scene);
                ((QGraphicsView*)widget)->setSceneRect(image.rect());
            }
            else
            {
                scene = ((QGraphicsView*)widget)->scene();
                scene->clear();
                scene->setSceneRect(image.rect());
                qDebug() << "scene Rect"<< scene->sceneRect();
                ((QGraphicsView*)widget)->setSceneRect(image.rect());
            }

            //qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
            //qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
            //qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
            //qDebug() << "scene Rect"<< scene->sceneRect();

            QGraphicsPixmapItem *gitem = ((QGraphicsView*)widget)->scene()->addPixmap(image);
            scene->setSceneRect(gitem->boundingRect());
            //qDebug() << scene->sceneRect();
            QRectF srect = scene->sceneRect();
            QRectF nrect = srect;

            if(srect.height() >= srect.width())
            {
                nrect.setWidth(srect.height());
                nrect.setHeight(srect.height());
            }
            else
            {
                nrect.setWidth(srect.width());
                nrect.setHeight(srect.width());
            }
            scene->setSceneRect(nrect);
            nrect = scene->sceneRect();

            //qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
            //qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
            //qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
            //qDebug() << "scene Rect"<< scene->sceneRect();

            ((QGraphicsView*)widget)->fitInView(nrect,Qt::KeepAspectRatio);
            ((QGraphicsView*)widget)->centerOn(nrect.center());
            ((QGraphicsView*)widget)->show();
            ((QGraphicsView*)widget)->setProperty("bbox",scene->itemsBoundingRect());
        }
    }
}

void RCFigureBrowser::PreviewImageEx(const QString &figurename)
{
    qDebug() << " in PreviewImageEx : " << figurename;
}

void RCFigureBrowser::PreviewImageEx(QWidget *widget, RCCHAR *figurename )
{
    // set up the temporary preview storage path
    QString qfigurename(figurename);
    QString tempPath = GetPreviewCacheName(qfigurename);

    if(qobject_cast<QGraphicsView *>(widget) != 0)
    {
        QGraphicsScene *scene=0;
        QString qfilepath(figurename);
        QPixmap image;

        if(QFileInfo::exists(tempPath))
        {
            QFileInfo figureInfo(qfigurename);
            QFileInfo previewInfo(tempPath);

            if(previewInfo.created() < figureInfo.lastModified())
            {
                QFile::remove(tempPath);
                QPixmapCache::remove(tempPath);
            }
        }

        if(!QFileInfo::exists(tempPath))
        {
            image.load(qfigurename);
            if(!image.isNull())
            {
                int gw = ((QGraphicsView*)widget)->width();
                int gh = ((QGraphicsView*)widget)->height();
                image = image.scaled(gw,gh,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                image.save(tempPath);
                QPixmapCache::insert(tempPath,image);
            }
        }
        else
        {
            if(!QPixmapCache::find(tempPath,image))
            {
                image.load(tempPath);
                QPixmapCache::insert(tempPath,image);
            }
        }

        if(!image.isNull())
        {
            if(((QGraphicsView*)widget)->scene() == 0)
            {
                scene = new QGraphicsScene();
                scene->setSceneRect(image.rect());
                ((QGraphicsView*)widget)->setScene(scene);
                ((QGraphicsView*)widget)->setSceneRect(image.rect());
            }
            else
            {
                scene = ((QGraphicsView*)widget)->scene();
                scene->clear();
                scene->setSceneRect(image.rect());
                qDebug() << "scene Rect"<< scene->sceneRect();
                ((QGraphicsView*)widget)->setSceneRect(image.rect());
            }

            //qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
            //qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
            //qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
            //qDebug() << "scene Rect"<< scene->sceneRect();

            QGraphicsPixmapItem *gitem = ((QGraphicsView*)widget)->scene()->addPixmap(image);
            scene->setSceneRect(gitem->boundingRect());
            //qDebug() << scene->sceneRect();
            QRectF srect = scene->sceneRect();
            QRectF nrect = srect;

            if(srect.height() >= srect.width())
            {
                nrect.setWidth(srect.height());
                nrect.setHeight(srect.height());
            }
            else
            {
                nrect.setWidth(srect.width());
                nrect.setHeight(srect.width());
            }
            scene->setSceneRect(nrect);
            nrect = scene->sceneRect();

            //qDebug() << "Widget Rect" << ((QGraphicsView*)widget)->rect();
            //qDebug() << "Viewport Rect"<< ((QGraphicsView*)widget)->viewport()->rect();
            //qDebug() << "Widget scene Rect"<< ((QGraphicsView*)widget)->sceneRect();
            //qDebug() << "scene Rect"<< scene->sceneRect();

            ((QGraphicsView*)widget)->fitInView(nrect,Qt::KeepAspectRatio);
            ((QGraphicsView*)widget)->centerOn(nrect.center());
            ((QGraphicsView*)widget)->show();
            ((QGraphicsView*)widget)->setProperty("bbox",scene->itemsBoundingRect());
        }
    }
}

void RCFigureBrowser::ShowSettingsGroup(settingType type)
{
    switch(type)
    {
    case none :
        ui->stackedWidget->setCurrentIndex(2);
        break;
    case figures :
    default:
        ui->stackedWidget->setCurrentIndex(0);
         break;
    case bitmaps :
        ui->stackedWidget->setCurrentIndex(1);
        break;
    }
}

void RCFigureBrowser::SetTitle(RCCHAR *title)
{
    setWindowTitle(QString(title));
}

void RCFigureBrowser::SetFilter(RCCHAR *filter)
{
    if( strstr(filter,".jpg") != NULL || strstr(filter,".JPG") != NULL ||
        strstr(filter,".jpeg") != NULL || strstr(filter,".JPEG") != NULL ||
        strstr(filter,".png") != NULL || strstr(filter,".png") != NULL ||
        strstr(filter,".bmp") != NULL || strstr(filter,".BMP") )
    {
        if( strstr(filter,".jpg") != NULL || strstr(filter,".JPG") != NULL ||
            strstr(filter,".jpeg") != NULL || strstr(filter,".JPEG") != NULL )
            strcpy(m_filter,"*.jpg");
        if( strstr(filter,".png") != NULL || strstr(filter,".png") != NULL)
            strcat(m_filter,";*.png");
        if( strstr(filter,".bmp") != NULL || strstr(filter,".BMP") != NULL)
            strcat(m_filter,";*.bmp");
    }
    else if( strstr(filter,".cad") != NULL || strstr(filter,".CAD") != NULL )
        strcpy(m_filter,"*.cad");
    else
        strcpy(m_filter,"*.cad");
}

void RCFigureBrowser::SetRootDir(RCCHAR *rootdir)
{
   if(strlen(rootdir) > 0 && ui->IDC_FOLDER_COMBO->findText(QString(rootdir)) < 0)
   {
       //ui->IDC_FOLDER_COMBO->addItem(QString(rootdir));
       ui->IDC_FOLDER_COMBO->insertItem(ui->IDC_FOLDER_COMBO->count()-1,QString(rootdir));
       int sel = ui->IDC_FOLDER_COMBO->findText(QString(rootdir));
       ui->IDC_FOLDER_COMBO->setCurrentIndex(sel);
   }
   else
   {
       int sel = ui->IDC_FOLDER_COMBO->findText(QString(rootdir));
       ui->IDC_FOLDER_COMBO->setCurrentIndex(sel);
   }
}

void RCFigureBrowser::SetSelFileName(RCCHAR *filename)
{
    strcpy(m_SelectedFileName,filename);
    ui->IDC_FILENAME_COMBO->setCurrentText(QString(filename));
}

void RCFigureBrowser::SetRecentPaths(QStringList paths)
{
    RCCHAR text[300],recent[600];
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    ui->IDC_FOLDER_COMBO->clear();
    for(int i=0; i < paths.count(); i++  )
    {
        sprintf(recent,"%s:Path%d",m_Prefix.data(),i);
        //GetPrivateProfileString(_T("FigureBrowser"),recent,_T(""),text,300,getinifilename());
        strcpy(text,settings.value("FigureBrowser/"+QString(recent),"").toString().data());
        if( strlen(text) > 0 && ui->IDC_FOLDER_COMBO->findText(QString(text)) < 0)
            ui->IDC_FOLDER_COMBO->addItem(QString(text));
    }
}

QStringList RCFigureBrowser::GetRecentPaths()
{
    QStringList paths;
    for(int i=0; i < ui->IDC_FOLDER_COMBO->count(); i++  )
        paths.append(ui->IDC_FOLDER_COMBO->itemText(i));
    return paths;
}

extern previewData * PreviewFigureMTWorker(previewData *pdata);

void RCFigureBrowser::handleFinished()
{
    qDebug() << "in handleFinished";
#if 0
    RCHWND hwnd = ((QFutureWatcher<previewData>)sender()).result().mhwnd;
    QGraphicsScene *scene = ((QGraphicsView*)hwnd)->scene();

    if(scene == 0)
        scene = new QGraphicsScene();
    scene->clear();
    ((QGraphicsView*)hwnd)->setScene(0);

    QGraphicsItem *gitem = ((QFutureWatcher<previewData>)sender()).result().mgitem;

    if(gitem != 0)
    {
        scene->addItem(gitem);
        ((QGraphicsView*)hwnd)->setScene(scene);
        gitem->setData(0,"figure");
        ((QGraphicsView*)hwnd)->fitInView(gitem,Qt::KeepAspectRatio);
        ((QGraphicsView*)hwnd)->centerOn(gitem);
        ((QGraphicsView*)hwnd)->show();
    }
    ((QGraphicsView*)hwnd)->setProperty("bbox",scene->sceneRect());
#endif
}


void RCFigureBrowser::PreviewFigureMTController(RCHWND hwnd,Point eye,Point ref,Point up,int persp,RCCHAR *figurename)
{
    qDebug() << "Start PreviewFigureMTController";
#if 0
    for(int i=0; i < 10; i++)
    {
        //QFutureWatcher<void> * watcher = new QFutureWatcher<void>;
        //connect(watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
        //QFuture<void> future;
        //watcher->setFuture(future);
        //future = QtConcurrent::run(someThreadedFunction);

        QFutureSynchronizer<void> synchronizer;
        synchronizer.addFuture(QtConcurrent::run(someThreadedFunction));
        qDebug() << "before waitForFinished()";
        synchronizer.waitForFinished();
        qDebug() << "after waitForFinished()";
    }
#endif

    // local args
    Point leye = eye, lref = ref, lup = up;
    RCCHAR *lfigurename = figurename;
    int lpersp = persp;

    QGraphicsScene *scene = ((QGraphicsView*)hwnd)->scene();

    if(scene == 0)
        scene = new QGraphicsScene();
    scene->clear();
    ((QGraphicsView*)hwnd)->setScene(scene);

    previewData *prevdata = new previewData(hwnd,eye,ref,up,persp,figurename);

#if 0
    QFuture<previewData*> future = QtConcurrent::run(PreviewFigureMTWorker, prevdata);

    QGraphicsItem *gitem = future.result()->mgitem;

    if(gitem != 0)
    {
        scene->addItem(gitem);
        ((QGraphicsView*)hwnd)->setScene(scene);
        gitem->setData(0,"figure");
        ((QGraphicsView*)hwnd)->fitInView(gitem,Qt::KeepAspectRatio);
        ((QGraphicsView*)hwnd)->centerOn(gitem);
        ((QGraphicsView*)hwnd)->show();
    }
    ((QGraphicsView*)hwnd)->setProperty("bbox",scene->sceneRect());
    delete prevdata;
#endif
#if 1
    QFuture<previewData*> future;
    future = QtConcurrent::run(PreviewFigureMTWorker, prevdata);
#endif
#if 0
    QFutureWatcher<previewData*> * watcher = new QFutureWatcher<previewData*>;
    connect(watcher, SIGNAL(finished()), prevdata, SLOT(handleFinished()));
    QFuture<previewData*> future;
    watcher->setFuture(future);
    future = QtConcurrent::run(PreviewFigureMTWorker, prevdata);
    //QFutureSynchronizer<void> synchronizer;
    //synchronizer.addFuture(future);
    //synchronizer.waitForFinished();
#endif
    qDebug() << "End PreviewFigureMTController";
}

void RCFigureBrowser::PreviewFigureGL(RCHWND hwnd,Point eye,Point ref,Point up,int persp,RCCHAR *figurename)
{
    View top(_RCT("Top"),persp,0, eye.x, eye.y, eye.z, ref.x, ref.y, ref.z, up.x, up.y, up.z,1E10,-1E10);
    View3dGLOutput output(hwnd,0,&top,1.0);
    //View3dOutput output(hwnd,0,&top,1.0);
    RECT rect;
    RCHPEN hpen,oldhpen;
    EntityHeader header(0,0,0,0);
    Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation;
    Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
    BitMask options(32);
    double s;
    int l,c,st,w;


    // get the current defaults for the header
    l  = db.header.getlayer();
    c  = db.header.getcolour();
    st = db.header.getstyle();
    w  = db.header.getweight();
    header.change(c,l,st,w);

    QGraphicsScene *scene=0;

    if(qobject_cast<RCGraphicsPreview *>((QObject*)hwnd) != 0)
    {
        ((RCGraphicsPreview*)hwnd)->mview3dsurface = &output;

        if((scene = ((RCGraphicsPreview*)hwnd)->scene()) == 0)
        {
            scene = new QGraphicsScene();
            ((RCGraphicsPreview*)hwnd)->setScene(scene);
            ((RCGraphicsPreview*)hwnd)->setSceneRect(QRect());
        }
        else
        {
            scene = ((RCGraphicsPreview*)hwnd)->scene();
            scene->clear();
            ((QGraphicsView*)hwnd)->setScene(scene);
            ((QGraphicsView*)hwnd)->setSceneRect(QRect());
        }
        output.setScene(scene);

        //((RCGraphicsPreview*)((QGraphicsView*)hwnd)->viewport())->mview3dsurface = &output;
        //((QGraphicsView*)hwnd)->resize((output.getumax() - output.getumin()),(output.getvmax() - output.getvmin()));

        options.set(FigureOriginalColour,ui->originalColour->isChecked());
        options.set(FigureOriginalStyle,ui->originalStyle->isChecked());
        options.set(FigureOriginalWeight,ui->originalWeight->isChecked());

        Figure figure1(_RCT(figurename),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
        figure1.extents(0,&pmin,&pmax);

        s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
        if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
          s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
        if (s < 0.00001)
          s = 1.0;

        s *= 1.1;

        org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
        Figure figure2(header,_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
        //Figure figure2(_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
        db.dismask.layer.set(db.header.getlayer(),1);


        //  Prepare the surface for OpenGl drawing

        //double dtol = db.getdtoler();
        //db.setdtoler(50.0);

        figure2.qgi=0;
        //figure2.draw(DM_NORMAL,NULL,&output);
        figure2.drawGL(DM_NORMAL,NULL,&output);


        QGraphicsItem *gitem = figure2.qgi;
        if(gitem != 0)
        {
            gitem->setData(0,"figure");
            ((RCGraphicsPreview*)hwnd)->fitInView(gitem,Qt::KeepAspectRatio);
            ((RCGraphicsPreview*)hwnd)->centerOn(gitem);
            ((RCGraphicsPreview*)hwnd)->show();
        }

        //db.setdtoler(dtol);
        ((RCGraphicsPreview*)hwnd)->setProperty("bbox",scene->sceneRect());
        //((RCGraphicsPreview*)hwnd)->setProperty("bbox",((RCGraphicsView*)hwnd)->viewport()->rect());
    }
}

void RCFigureBrowser::PreviewFigure(RCHWND hwnd,Point eye,Point ref,Point up,int persp,RCCHAR *figurename)
{
    View top(_RCT("Top"),persp,0, eye.x, eye.y, eye.z, ref.x, ref.y, ref.z, up.x, up.y, up.z,1E10,-1E10);
    View3dOutput output(hwnd,0,&top,1.0);
    RECT rect;
    RCHPEN hpen,oldhpen;
    EntityHeader header(0,0,0,0);
    Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
    Point pmin1(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax1(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
    Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation;
    BitMask options(32);
    double s;
    int l,c,st,w;

    if(m_initializing)
        return;

    // get the current defaults for the header
    l  = db.header.getlayer();
    c  = db.header.getcolour();
    st = db.header.getstyle();
    w  = db.header.getweight();
    header.change(c,l,st,w);

    QGraphicsScene *scene=0;

    if(qobject_cast<QGraphicsView *>((QWidget*)hwnd) != 0)
    {
        int pixmapWidth = ((QGraphicsView*)hwnd)->rect().width();
        int pixmapHeight = ((QGraphicsView*)hwnd)->rect().height();
        QRectF drawRect(0,0,pixmapWidth,pixmapHeight);

        if((scene = ((QGraphicsView*)hwnd)->scene()) == 0)
        {
            scene = new QGraphicsScene();
            scene->setSceneRect(QRectF());
            ((QGraphicsView*)hwnd)->setScene(scene);
            ((QGraphicsView*)hwnd)->setSceneRect(QRectF());
        }
        else
        {
            scene = ((QGraphicsView*)hwnd)->scene();
            scene->clear();
            scene->setSceneRect(QRectF());
            ((QGraphicsView*)hwnd)->setSceneRect(QRectF());
        }
        output.setScene(scene);

        // get the header image
        uchar *preview=0;
        int width=0,height=0,bpp=3,status=0;
        // init the default images
        QImage image(pixmapWidth,pixmapHeight,QImage::Format_RGB888);
        image.fill(QColor(255,255,255));
        QPixmap pixmap(pixmapWidth,pixmapHeight);
        pixmap.fill(QColor(255,255,255));
        CadFile infile;
        int previewType=-1;

        if(infile.open(figurename,"rb"))
        {
            if((previewType=infile.readheaderimage(preview, width, height, bpp)) == 0)
            {
                if((width * height) != 0 && (bpp == 3 || bpp == 4))
                {
                    //int stride = ((width*3 + 3)/4)*4;
                    int stride = width * 3;
                    int gw = ((QGraphicsView*)hwnd)->width();
                    int gh = ((QGraphicsView*)hwnd)->height();
                    image = QImage(preview,width,height,stride,QImage::Format_RGB888).scaled(gw,gh,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                    //image.save("preview.bmp","bmp");
                    status = 1;
                }
            }
        }
        infile.close();


        // if there is a screen shot type preview use that
        // else this was saved as a figure so draw the figure
        if(status == 1 && previewType == 0)
        {
            pixmap = QPixmap::fromImage(image);

            scene->setSceneRect(pixmap.rect());
            ((QGraphicsView*)hwnd)->setScene(scene);
            ((QGraphicsView*)hwnd)->setSceneRect(image.rect());

            QGraphicsPixmapItem *gitem = ((QGraphicsView*)hwnd)->scene()->addPixmap(pixmap);
            ((QGraphicsPixmapItem *)gitem)->setTransformationMode(Qt::SmoothTransformation);
            gitem->setData(0,"figure");
            scene->setSceneRect(gitem->boundingRect());
            qDebug() << scene->sceneRect();
            QRectF srect = scene->sceneRect();
            QRectF nrect = srect;

            if(srect.height() >= srect.width())
            {
                nrect.setWidth(srect.height());
                nrect.setHeight(srect.height());
            }
            else
            {
                nrect.setWidth(srect.width());
                nrect.setHeight(srect.width());
            }
            scene->setSceneRect(nrect);
            nrect = scene->sceneRect();
            ((QGraphicsView*)hwnd)->fitInView(nrect,Qt::KeepAspectRatio);
            ((QGraphicsView*)hwnd)->centerOn(nrect.center());
            ((QGraphicsView*)hwnd)->show();
            ((QGraphicsView*)hwnd)->setProperty("bbox",scene->itemsBoundingRect());
        }
        else
        {
            //qDebug() << "alignment : " << ((QGraphicsView*)hwnd)->alignment();
            //qDebug() << "is interactive : " << ((QGraphicsView*)hwnd)->isInteractive();

            options.set(FigureOriginalColour,ui->originalColour->isChecked() ? 1 : 0);
            options.set(FigureOriginalStyle,ui->originalStyle->isChecked() ? 1 : 0);
            options.set(FigureOriginalWeight,ui->originalWeight->isChecked() ? 1 : 0);
            options.set(FigureOriginalLayer,ui->originalLayer->isChecked() ? 1 : 0);

            output.setplotscale(1.0);

            Figure figure1(header,_RCT(figurename),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
            figure1.extents(0,&pmin,&pmax);

            double aspect=1;
            double du = fabs(pmax.x - pmin.x) / 1.9;
            double dv = fabs(pmax.y - pmin.y) / 1.9;

            if (du > db.getptoler() || dv > db.getptoler())
            {
               aspect = output.getwidthmm() / output.getheightmm();
               if (du / dv > aspect) dv = du / aspect;
               if (du / dv < aspect) du = dv * aspect;
               pmin.x = (pmin.x + pmax.x) / 2.0 - du;
               pmin.y = (pmin.y + pmax.y) / 2.0 - dv;
               pmax.x = (pmin.x + pmax.x) / 2.0 + du;
               pmax.y = (pmin.y + pmax.y) / 2.0 + dv;
               output.zoom(pmin.x,pmin.y,pmax.x,pmax.y);

               //qDebug() << "zoom width  : " << pmax.x - pmin.x;
               //qDebug() << "zoom height : " << pmax.y - pmin.y;
            }

            output.updatematrix();
            s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
            if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
              s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
            if (s < 0.00001)
              s = 1.0;

            //s *= 1.1;

            org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
            Figure figure2(header,_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
            //Figure figure2(_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
            db.dismask.layer.set(db.header.getlayer(),1);

            //double dtol = db.getdtoler();
            //db.setdtoler(50.0);

            // give the figure and identity transform
            // before getting the entity list
            Transform oldTransf = *figure2.gettransform();
            Transform ident;
            figure2.settransform(ident);
            EntityList elist = figure2.getentitylist();
            int numEnts = elist.length();
            Entity *e=0;
            for (elist.end() ; (e = elist.last()) != NULL ;)
            {
                if(e->isa(figure_entity))
                {
                    Transform Tf = *((Figure*)e)->gettransform();
                    Transform iTf;
                    ((Figure*)e)->settransform(iTf);
                    numEnts += ((Figure*)e)->getentitylist().length();
                    ((Figure*)e)->settransform(Tf);
                }
            }
            // put the transform back
            figure2.settransform(oldTransf);

            // set up the progress dialog
#if 1
            QProgressDialog progress("Creating Preview", "Abort", 0, numEnts, this);
            progress.setWindowModality(Qt::WindowModal);
            program->setprogressbardlg(&progress);
            v.setinteger("prg::doprogressuptick",1);
            progress.setValue(0);
            app->processEvents();
#endif
            //QTimer timer(this);
            //connect(&timer, SIGNAL(timeout()), this, SLOT(onIdle()));
            //timer.start(100);

            // draw the figure
            figure2.qgi=0;
            figure2.draw(DM_PRINT,NULL,&output);
            QGraphicsItem *gitem = figure2.qgi;

            //timer.stop();
#if 1
            progress.setValue(numEnts);
            app->processEvents();
            program->setprogressbardlg(0);
#endif
            if(gitem != 0)
            {
                gitem->setData(0,"figure");
                scene->setSceneRect(gitem->boundingRect());
                qDebug() << scene->sceneRect();
                QRectF srect = scene->sceneRect();
                ((QGraphicsView*)hwnd)->fitInView(srect,Qt::KeepAspectRatio);
                ((QGraphicsView*)hwnd)->centerOn(srect.center());
                ((QGraphicsView*)hwnd)->show();
                ((QGraphicsView*)hwnd)->setProperty("bbox",scene->sceneRect());
            }
            //db.setdtoler(dtol);
        }
    }
}

void RCFigureBrowser::PreviewFigureEx(const QString &figurename)
{
    qDebug() << " in PreviewFigureEx : " << figurename;
}

/**
 * @brief RCFigureBrowser::PreviewFigureEx
 * @param hwnd
 * @param figurename
 *
 * show the preview of the given figure
 * If there is an image in the header show that image
 * If there is no image in the header look in the temp folder image cache and
 * show the image from the temp folder
 * If there is no image in the temp folder create a new image in the temp folder and show that image
 * A last resort is to draw the vector version of the preview
 *
 */
void RCFigureBrowser::PreviewFigureEx(RCHWND hwnd,RCCHAR *figurename)
{
 //View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,1E10,-1E10);
 //View3dOutput output(hwnd,0,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation;
 Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
 BitMask options(32);
 double s;
 int l,c,st,w;

 // set up the temporary preview storage path
 QString qfigurename(figurename);
 QString tempPath = GetPreviewCacheName(qfigurename);

 // get the current defaults for the header
 l  = db.header.getlayer();
 c  = db.header.getcolour();
 st = db.header.getstyle();
 w  = db.header.getweight();
 header.change(c,l,st,w);
 //double lastdbr,lastdbg,lastdbb;
 //db.colourtable[0].get(&lastdbr,&lastdbg,&lastdbb);

  if(qobject_cast<QGraphicsView *>((QWidget*)hwnd) != 0)
  {
      // preview use/creation priority
      // 1. check the cached file is up to date if not remove it
      // 2. if there is a chached file and it is cached in the QPixmapCache use that
      // 3. else if there is a cached file use that
      // 4. else if there is a drawn header preview use that and cache it
      // 5. else if there is a screen shot preview use that and cache it
      // 6. else create a new preview and use that and cache it

      // 1. check the cached file is up to date if not remove it
      if(QFileInfo::exists(tempPath))
      {
          QFileInfo figureInfo(qfigurename);
          QFileInfo previewInfo(tempPath);

          if(previewInfo.created() < figureInfo.lastModified())
          {
              QFile::remove(tempPath);
              QPixmapCache::remove(tempPath);
          }
      }

      int pixmapWidth = ((QGraphicsView*)hwnd)->rect().width();
      int pixmapHeight = ((QGraphicsView*)hwnd)->rect().height();
      if(pixmapWidth > pixmapHeight)
          pixmapWidth = pixmapHeight;

      // init the default pixmap
      QPixmap pixmap(pixmapWidth,pixmapHeight);
      pixmap.fill(QColor(255,255,255));
      pixmap.load(":/images/NoPreview.svg");

      // 2. if there is a chached file and it is cached in the QPixmapCache use that
      if(QFileInfo::exists(tempPath) && QPixmapCache::find(tempPath,pixmap))
      {
          ; // find loads the pixmap so nothing else to do here
      }
      // 3. else if there is a cached file use that
      else if (QFileInfo::exists(tempPath))
      {
          pixmap.load(tempPath,"PNG");
          QPixmapCache::insert(tempPath,pixmap);
      }
      else
      {
          // get the header image
          CadFile infile;
          int previewType=-1;
          uchar *preview=0;
          int width=0,height=0,bpp=3,status=0;

          // init the default image
          QImage image(pixmapWidth,pixmapHeight,QImage::Format_RGB888);
          image.fill(QColor(255,255,255));

          if(infile.open(figurename,"rb"))
          {
              if((previewType=infile.readheaderimage(preview, width, height, bpp)) >= 0)
              {
                  if((width * height) != 0 && (bpp == 3 || bpp == 4))
                  {
                      //int stride = ((width*3 + 3)/4)*4;
                      int stride = width * 3;
                      int gw = ((QGraphicsView*)hwnd)->width();
                      int gh = ((QGraphicsView*)hwnd)->height();
                      image = QImage(preview,width,height,stride,QImage::Format_RGB888).scaled(gw,gh,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                      //image.save("preview.bmp","bmp");
                      status = 1;
                  }
              }
          }
          infile.close();
          // 4. else if there is a drawn header preview use that and cache it
          if(status == 1 && previewType == 1)
          {
              pixmap = QPixmap::fromImage(image);
              pixmap.save(tempPath,"PNG");
              QPixmapCache::insert(tempPath,pixmap);
          }
          // 5. else if there is a screen shot preview use that and cache it
          else if(status == 1 && previewType == 0)
          {
              pixmap = QPixmap::fromImage(image);
              pixmap.save(tempPath,"PNG");
              QPixmapCache::insert(tempPath,pixmap);
          }
          // 6. else create a new preview and use that and cache it
          else
          {
              View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,-1E10,1E10);
              double gw = pixmapWidth;//((QGraphicsView*)hwnd)->rect().width();
              double gh = pixmapHeight;//((QGraphicsView*)hwnd)->rect().height();
              View3dBitmapOutput output(&top,1.0,gw,gh);
              output.getScene()->setBackgroundBrush(QBrush(QColor(255,255,255)));
              output.setplotscale(1.0);

              if(output.getsurfacetype() == View3dSurf)
              {
                  options.set(FigureOriginalColour,1);
                  options.set(FigureOriginalStyle,1);
                  options.set(FigureOriginalWeight,1);
                  options.set(FigureOriginalLayer,1);

                  Figure figure1(_RCT(figurename),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
                  figure1.extents(0,&pmin,&pmax);

                  double aspect=1;
                  double du = fabs(pmax.x - pmin.x) / 1.9;
                  double dv = fabs(pmax.y - pmin.y) / 1.9;

                  if (du > db.getptoler() || dv > db.getptoler())
                  {
                       aspect = output.getwidthmm() / output.getheightmm();
                       if (du / dv > aspect) dv = du / aspect;
                       if (du / dv < aspect) du = dv * aspect;
                       pmin.x = (pmin.x + pmax.x) / 2.0 - du;
                       pmin.y = (pmin.y + pmax.y) / 2.0 - dv;
                       pmax.x = (pmin.x + pmax.x) / 2.0 + du;
                       pmax.y = (pmin.y + pmax.y) / 2.0 + dv;
                       output.zoom(pmin.x,pmin.y,pmax.x,pmax.y);
                  }

                  s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
                  if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
                    s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
                  if (s < 0.00001)
                    s = 1.0;

                  //s *= 1.1;

                  org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
                  Figure figure2(header,_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
                  db.dismask.layer.set(db.header.getlayer(),1);

                  figure2.qgi=0;
                  figure2.draw(DM_PRINT,NULL,&output);

                  QGraphicsItem *gitem = figure2.qgi;


                  if(gitem != 0)
                      output.getScene()->setSceneRect(gitem->boundingRect());

              }
              output.render();
              pixmap = *(QPixmap*)output.getBitmap();
              //pixmap.save("preview.jpg","JPG");
              // save it to the temp cache for next time around
              pixmap.save(tempPath,"PNG");
              QPixmapCache::insert(tempPath,pixmap);
          }
      }

      // draw the pixmap
      QGraphicsScene *scene=0;
      QRectF drawRect(0,0,pixmapWidth,pixmapHeight);

      if(!pixmap.isNull())
          drawRect = pixmap.rect();

      if((scene = ((QGraphicsView*)hwnd)->scene()) == 0)
      {
          scene = new QGraphicsScene();
          scene->setSceneRect(drawRect);
          ((QGraphicsView*)hwnd)->setScene(scene);
          ((QGraphicsView*)hwnd)->setSceneRect(drawRect);
      }
      else
      {
          scene = ((QGraphicsView*)hwnd)->scene();
          scene->clear();
          scene->setSceneRect(drawRect);
          ((QGraphicsView*)hwnd)->setSceneRect(drawRect);
      }

      if(!pixmap.isNull())
      {
          View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,1E10,-1E10);
          View3dOutput output(hwnd,0,&top,1.0);
          ((QGraphicsView*)hwnd)->setRenderHints(((QGraphicsView*)hwnd)->renderHints() |
                                                 QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
          output.setScene(scene);

          QGraphicsPixmapItem *gitem = ((QGraphicsView*)hwnd)->scene()->addPixmap(pixmap);
          ((QGraphicsPixmapItem *)gitem)->setTransformationMode(Qt::SmoothTransformation);
          gitem->setData(0,"figure");
          QRectF brect(gitem->boundingRect());
          scene->setSceneRect(brect);
          qDebug() << scene->sceneRect();
          QRectF srect = scene->sceneRect();
          QRectF nrect = srect;

          if(srect.height() >= srect.width())
          {
              nrect.setWidth(srect.height());
              nrect.setHeight(srect.height());
          }
          else
          {
              nrect.setWidth(srect.width());
              nrect.setHeight(srect.width());
          }
          scene->setSceneRect(nrect);
          nrect = scene->sceneRect();
          ((QGraphicsView*)hwnd)->fitInView(nrect,Qt::KeepAspectRatio);
          ((QGraphicsView*)hwnd)->centerOn(nrect.center());
          ((QGraphicsView*)hwnd)->show();
          ((QGraphicsView*)hwnd)->setProperty("bbox",scene->itemsBoundingRect());
      }

#if 0
      if(!QPixmapCache::find(tempPath,pixmap))
      {
          if(infile.open(figurename,"rb"))
          {
              if((previewType=infile.readheaderimage(preview, width, height, bpp)) >= 0)
              {
                  if((width * height) != 0 && (bpp == 3 || bpp == 4))
                  {
                      //int stride = ((width*3 + 3)/4)*4;
                      int stride = width * 3;
                      int gw = ((QGraphicsView*)hwnd)->width();
                      int gh = ((QGraphicsView*)hwnd)->height();
                      image = QImage(preview,width,height,stride,QImage::Format_RGB888).scaled(gw,gh,Qt::KeepAspectRatio,Qt::SmoothTransformation);
                      //image.save("preview.bmp","bmp");
                      status = 1;
                  }
              }
          }
          infile.close();
      }

      if(status == 1 && previewType >= 0)
      {
          View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,1E10,-1E10);
          View3dOutput output(hwnd,0,&top,1.0);
          output.setScene(scene);

          if(!QPixmapCache::find(tempPath,pixmap))
          {
              pixmap = QPixmap::fromImage(image);
              QPixmapCache::insert(tempPath,pixmap);
          }

          QGraphicsPixmapItem *gitem = ((QGraphicsView*)hwnd)->scene()->addPixmap(pixmap);
          scene->setSceneRect(gitem->boundingRect());
          qDebug() << scene->sceneRect();
          QRectF srect = scene->sceneRect();
          QRectF nrect = srect;

          if(srect.height() >= srect.width())
          {
              nrect.setWidth(srect.height());
              nrect.setHeight(srect.height());
          }
          else
          {
              nrect.setWidth(srect.width());
              nrect.setHeight(srect.width());
          }
          scene->setSceneRect(nrect);
          nrect = scene->sceneRect();
          ((QGraphicsView*)hwnd)->fitInView(nrect,Qt::KeepAspectRatio);
          ((QGraphicsView*)hwnd)->centerOn(nrect.center());
          //((QGraphicsView*)hwnd)->show();
          ((QGraphicsView*)hwnd)->setProperty("bbox",scene->itemsBoundingRect());
      }
      else if(QFileInfo::exists(tempPath))
      {
          QPixmap pixmap;
          if(!QPixmapCache::find(tempPath,pixmap))
          {
              pixmap.load(tempPath,"PNG");
              QPixmapCache::insert(tempPath,pixmap);
          }

          if(!pixmap.isNull())
          {
              View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,1E10,-1E10);
              View3dOutput output(hwnd,0,&top,1.0);
              ((QGraphicsView*)hwnd)->setRenderHints(((QGraphicsView*)hwnd)->renderHints() |
                                                     QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
              output.setScene(scene);

              QGraphicsPixmapItem *gitem = ((QGraphicsView*)hwnd)->scene()->addPixmap(pixmap);
              ((QGraphicsPixmapItem *)gitem)->setTransformationMode(Qt::SmoothTransformation);
              QRectF brect(gitem->boundingRect());
              scene->setSceneRect(brect);
              qDebug() << scene->sceneRect();
              QRectF srect = scene->sceneRect();
              QRectF nrect = srect;

              if(srect.height() >= srect.width())
              {
                  nrect.setWidth(srect.height());
                  nrect.setHeight(srect.height());
              }
              else
              {
                  nrect.setWidth(srect.width());
                  nrect.setHeight(srect.width());
              }
              scene->setSceneRect(nrect);
              nrect = scene->sceneRect();
              ((QGraphicsView*)hwnd)->fitInView(nrect,Qt::KeepAspectRatio);
              ((QGraphicsView*)hwnd)->centerOn(nrect.center());
              //((QGraphicsView*)hwnd)->show();
              ((QGraphicsView*)hwnd)->setProperty("bbox",scene->itemsBoundingRect());
          }
      }
      else// create a new image preview if this was saved as a figure
      {
          View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,-1E10,1E10);
          double gw  = ((QGraphicsView*)hwnd)->rect().width();
          double gh = ((QGraphicsView*)hwnd)->rect().height();
          View3dBitmapOutput output(&top,1.0,gw,gh);
          output.getScene()->setBackgroundBrush(QBrush(QColor(255,255,255)));

          //db.geometry.extents(&output,&pmin,&pmax);

          if(output.getsurfacetype() == View3dSurf)
          {
              //options.set(FigureOriginalColour,ui->originalColour->isChecked());
              //options.set(FigureOriginalStyle,ui->originalStyle->isChecked());
              //options.set(FigureOriginalWeight,ui->originalWeight->isChecked());

              options.set(FigureOriginalColour,1);
              options.set(FigureOriginalStyle,0);
              options.set(FigureOriginalWeight,0);

              Figure figure1(_RCT(figurename),_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
              figure1.extents(0,&pmin,&pmax);

              s = (pmax.x - pmin.x) / (output.getumax() - output.getumin());
              if (s < (pmax.y - pmin.y) / (output.getvmax() - output.getvmin()))
                s = (pmax.y - pmin.y) / (output.getvmax() - output.getvmin());
              if (s < 0.00001)
                s = 1.0;

              s *= 1.1;

              header.change(0,0,0,0);
              db.colourtable[0].set(0.0,0.0,0.0);
              org.setxyz((output.getumin() + output.getumax()) / 2.0 - (pmin.x + pmax.x) / 2.0 / s,(output.getvmin() + output.getvmax()) / 2.0 - (pmin.y + pmax.y) / 2.0 / s,0.0);
              Figure figure2(header,_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
              //Figure figure2(_RCT(figurename),_RCT(""),org,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0/s,1.0/s,1.0/s),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
              db.dismask.layer.set(db.header.getlayer(),1);

              //figure2.extents(0,&pmin,&pmax);

              //double du = (pmax.x - pmin.x) * 0.02;
              //double dv = (pmax.y - pmin.y) * 0.02;

              //output.zoom(pmin.x-du,pmin.y-dv,pmax.x+du,pmax.y+dv);
              //double dtol = db.getdtoler();
              //db.setdtoler(50.0);

              figure2.qgi=0;
              figure2.draw(DM_NORMAL,NULL,&output);
          }
          output.render();
          db.colourtable[0].set(lastdbr,lastdbg,lastdbb);
          QPixmap pixmap = *(QPixmap*)output.getBitmap();
          //pixmap.save("preview.jpg","JPG");
          // save it to the temp cache for next time around
          pixmap.save(tempPath,"PNG");
          QPixmapCache::insert(tempPath,pixmap);

          QGraphicsPixmapItem *gitem = ((QGraphicsView*)hwnd)->scene()->addPixmap(pixmap);
          scene->setSceneRect(gitem->boundingRect());
          qDebug() << scene->sceneRect();
          //QRectF srect = scene->temsBoundingRect();
          QRectF srect = scene->sceneRect();
          //QRectF srect = image.rect();
          QRectF nrect = srect;

          if(srect.height() >= srect.width())
          {
              nrect.setWidth(srect.height());
              nrect.setHeight(srect.height());
          }
          else
          {
              nrect.setWidth(srect.width());
              nrect.setHeight(srect.width());
          }
          scene->setSceneRect(nrect);
          nrect = scene->sceneRect();
          ((QGraphicsView*)hwnd)->fitInView(nrect,Qt::KeepAspectRatio);
          ((QGraphicsView*)hwnd)->centerOn(nrect.center());
          //((QGraphicsView*)hwnd)->show();
          ((QGraphicsView*)hwnd)->setProperty("bbox",scene->itemsBoundingRect());
      }
#endif
  }
}

void RCFigureBrowser::PreviewFigure(RCHWND hwnd,RCCHAR *figurename)
{
 View top(_RCT("Top"),0,0, 0, 0, 1.0, 0, 0,0, 0,1,0,1E10,-1E10);
 View3dOutput output(hwnd,0,&top,1.0);
 RECT rect;
 RCHPEN hpen,oldhpen;
 EntityHeader header(0,0,0,0);
 Point org(0.0,0.0,0.0),xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),scale,rotation;
 Point pmin(RCDBL_MAX,RCDBL_MAX,RCDBL_MAX),pmax(-RCDBL_MAX,-RCDBL_MAX,-RCDBL_MAX);
 BitMask options(32);
 double s;
 int l,c,st,w;

 // get the current defaults for the header
 l  = db.header.getlayer();
 c  = db.header.getcolour();
 st = db.header.getstyle();
 w  = db.header.getweight();
 header.change(c,l,st,w);

  if(qobject_cast<QGraphicsView *>((QWidget*)hwnd) != 0)
  {
      QGraphicsScene *scene=0;

      // get the header image
      uchar *preview=0;
      int width=0,height=0,bpp=3,status=0;
      // init the default images
      QImage image(256,256,QImage::Format_RGB888);
      image.fill(QColor(255,255,255));
      QPixmap pixmap(256,256);
      pixmap.fill(QColor(255,255,255));
      CadFile infile;
      if(infile.open(figurename,"rb"))
      {
          if(infile.readheaderimage(preview, width, height, bpp) >= 0)
          {
              if(width * height != 0 && bpp == 3 || bpp == 4)
              {
                  //int stride = ((width*3 + 3)/4)*4;
                  int stride = width * 3;
                  image = QImage(preview, width,height,stride,QImage::Format_RGB888);
                  image.save("preview.bmp","bmp");
                  status = 1;
              }
          }
      }
      infile.close();

      if((scene = ((QGraphicsView*)hwnd)->scene()) == 0)
      {
          scene = new QGraphicsScene();
          scene->setSceneRect(image.rect());
          ((QGraphicsView*)hwnd)->setScene(scene);
          ((QGraphicsView*)hwnd)->setSceneRect(image.rect());
      }
      else
      {
          scene = ((QGraphicsView*)hwnd)->scene();
          scene->clear();
          scene->setSceneRect(image.rect());
          //QGraphicsView*)hwnd)->setScene(scene);
          ((QGraphicsView*)hwnd)->setSceneRect(image.rect());
      }
      output.setScene(scene);

      if(status)
          pixmap = QPixmap::fromImage(image);

      QGraphicsPixmapItem *gitem = ((QGraphicsView*)hwnd)->scene()->addPixmap(pixmap);
      scene->setSceneRect(gitem->boundingRect());
      qDebug() << scene->sceneRect();
      //QRectF srect = scene->temsBoundingRect();
      QRectF srect = scene->sceneRect();
      //QRectF srect = image.rect();
      QRectF nrect = srect;

      if(srect.height() >= srect.width())
      {
          nrect.setWidth(srect.height());
          nrect.setHeight(srect.height());
      }
      else
      {
          nrect.setWidth(srect.width());
          nrect.setHeight(srect.width());
      }
      //scene->setSceneRect(nrect);
      //nrect.translate(srect.center()-nrect.center());
      //QGraphicsRectItem *ritem = scene->addRect(nrect.left(),nrect.top(),nrect.width(),nrect.height(),QPen(QColor(0,0,0)),Qt::NoBrush);
      //nrect = scene->itemsBoundingRect();
      //scene->setSceneRect(ritem->boundingRect());
      scene->setSceneRect(nrect);
      nrect = scene->sceneRect();
      //gitem->moveBy(nrect.center().x()-srect.center().x(),nrect.center().y()-srect.center().y());
      // fit it to the window and center it
      //((QGraphicsView*)widget)->setSceneRect(QRect());
      qDebug() << "Widget Rect" << ((QGraphicsView*)hwnd)->rect();
      qDebug() << "Viewport Rect"<< ((QGraphicsView*)hwnd)->viewport()->rect();
      qDebug() << "Widget scene Rect"<< ((QGraphicsView*)hwnd)->sceneRect();
      qDebug() << "scene Rect"<< scene->sceneRect();
      ((QGraphicsView*)hwnd)->fitInView(nrect,Qt::KeepAspectRatio);
      ((QGraphicsView*)hwnd)->centerOn(nrect.center());
      ((QGraphicsView*)hwnd)->show();
      ((QGraphicsView*)hwnd)->setProperty("bbox",scene->itemsBoundingRect());
  }
}

void RCFigureBrowser::on_IDC_FILENAME_COMBO_currentIndexChanged(const QString &arg1)
{
    strcpy(m_SelectedFileName,arg1.data());
    RCCHAR filePath[600];
    strcpy(filePath,ui->IDC_FOLDER_COMBO->currentText().data());
    if(strlen(filePath) < 1)
        strcpy(filePath,m_rootDir);
    strcat(filePath,QString(QDir::separator()).data()/*"/"*/);
    strcat(filePath,m_SelectedFileName);
    selectFigureListItem(ui->IDC_FILENAME_COMBO->currentData().toString().data());

    if(arg1.length() < 1)
    {
        clearPreview(ui->IDC_FIGURE_PICTURE);
    }
    else if(strstr(m_SelectedFileName,".cad") || strstr(m_SelectedFileName,".CAD"))
    {
        double dtol = v.getreal("db::dtoler");
        v.setreal("db::dtoler",20.0);
        //it->PreviewFigure(ui->IDC_FIGURE_PICTURE,::Point(0.0,0.0,1.0),::Point(0.0,0.0,0.0),::Point(0.0,1.0,0.0),0,filePath);
        // high quality preview drwing the cad file
#if 1
        // normal drawing
        PreviewFigure(ui->IDC_FIGURE_PICTURE,Point(0.0,0.0,1.0),Point(0.0,0.0,0.0),Point(0.0,1.0,0.0),0,filePath);
        /*
        if(m_Prefix == "Figure")
        {
            GetFigureExtents(filePath,mPmin,mPmax);
            mLibrarySize = mPmax - mPmin;
        }
        */
#endif
#if 0
        // threaded drawing
        PreviewFigureMTController(ui->IDC_FIGURE_PICTURE,Point(0.0,0.0,1.0),Point(0.0,0.0,0.0),Point(0.0,1.0,0.0),0,filePath);
#endif
#if 0
        // low quality preview using the header preview image
        PreviewFigure(ui->IDC_FIGURE_PICTURE,filePath);
#endif
        v.setreal("db::dtoler",dtol);
    }
    else
    {
        PreviewImage(ui->IDC_FIGURE_PICTURE,filePath);
    }
}

void RCFigureBrowser::on_IDC_FILENAME_COMBO_currentTextChanged(const QString &arg1)
{
    if(!m_initializing)
    {
        QString filePath = QString(m_rootDir) + QDir::separator() + arg1;
        GetFigureExtents(filePath.data(),mPmin,mPmax);
        mLibrarySize = mPmax - mPmin;
        if(m_UseSize)
        {
            ui->XSize->setValue(mLibrarySize.x * ui->XScale->value());
            ui->YSize->setValue(mLibrarySize.y * ui->XScale->value());
            ui->ZSize->setValue(mLibrarySize.z * ui->XScale->value());

            ui->XScale->setValue(1.0);
            ui->YScale->setValue(1.0);
            ui->ZScale->setValue(1.0);
        }
        else
        {
            ui->XSize->setValue(mLibrarySize.x * ui->XScale->value());
            ui->YSize->setValue(mLibrarySize.y * ui->YScale->value());
            ui->ZSize->setValue(mLibrarySize.z * ui->ZScale->value());
        }
    }
    //on_IDC_FILENAME_COMBO_currentIndexChanged(arg1);
}

void RCFigureBrowser::on_IDC_FOLDER_COMBO_currentIndexChanged(const QString &arg1)
{
    qDebug() << "on_IDC_FOLDER_COMBO_currentIndexChanged";
    //if(arg1.length() > 0 && QString(m_rootDir) != arg1)
    if(arg1.length() > 0 && !m_insertingFolderCB)
    {
        strcpy(m_rootDir,arg1.data());

        SetFileNames();

        if(!isVisible())
            return;
#if 0
        UpdateImageList();
        selectFigureListItem(_RCT("FilePreviewItem-0"));
        SyncFolderTree((RCCHAR*)arg1.data());
#endif
#if 1
        ShowImageListWithProgress();
        if(m_Interrupted == false)
        {
            selectFigureListItem(_RCT("FilePreviewItem-0"));
            SyncFolderTree((RCCHAR*)arg1.data());
        }
#endif
#if 0
        // threaded version using QFutureWatcher and QConcurrent mapped
        // not good because many RealCAD functions are not thread safe
        UpdateImageListT();
#endif
#if 0
        initGrid();
        selectFigureListItem(_RCT("FilePreviewItem-0"));
        SyncFolderTree((RCCHAR*)arg1.data());
#endif
    }
}

void RCFigureBrowser::on_IDC_FOLDER_COMBO_currentTextChanged(const QString &arg1)
{
    //qDebug() << "on_IDC_FOLDER_COMBO_currentTextChanged";
}

void RCFigureBrowser::on_IDC_BROWSE_BTN_clicked()
{
    m_Interrupted = false;
    QString selFoldername = fixFileName(QFileDialog::getExistingDirectory(this,"Select A folder",QString(home.getpublicfilename(_RCT(""))),
                                                            QFileDialog::ShowDirsOnly |
                                                            /*QFileDialog::DontUseNativeDialog |*/
                                                            QFileDialog::DontResolveSymlinks));

    int lastdirsel = ui->IDC_FOLDER_COMBO->currentIndex();
    int lastfilesel = ui->IDC_FILENAME_COMBO->currentIndex();
    if(selFoldername.length() > 0 && ui->IDC_FOLDER_COMBO->findText(selFoldername) < 0)
    {
        // add
        //ui->IDC_FOLDER_COMBO->addItem(selFoldername);
        ui->IDC_FOLDER_COMBO->insertItem(ui->IDC_FOLDER_COMBO->count()-1,selFoldername);
        int sel = ui->IDC_FOLDER_COMBO->findText(selFoldername);
        ui->IDC_FOLDER_COMBO->setCurrentIndex(sel);
        selectFigureListItem(_RCT("FilePreviewItem-0"));
        SyncFolderTree(selFoldername.data());
    }
    else if(selFoldername.length() > 0 && ui->IDC_FOLDER_COMBO->findText(selFoldername) >= 0)
    {
        int sel = ui->IDC_FOLDER_COMBO->findText(selFoldername);
        ui->IDC_FOLDER_COMBO->setCurrentIndex(sel);
        selectFigureListItem(_RCT("FilePreviewItem-0"));
        SyncFolderTree(selFoldername.data());
    }
    else if(selFoldername.length() == 0)
    {
        // select
        ui->IDC_FOLDER_COMBO->setCurrentIndex(lastdirsel);
        //selectFigureListItem(_RCT("FilePreviewItem-0"));
        selectFigureListItem(ui->IDC_FILENAME_COMBO->itemData(lastfilesel).toString().data());
        SyncFolderTree(ui->IDC_FOLDER_COMBO->currentText().data());
    }
 }

void RCFigureBrowser::SetCurrentFolder(RCCHAR *text)
{
}

void RCFigureBrowser::SetFolderTree(RCCHAR * rootDir)
{

    m_FileSystemModel.setRootPath(QString(rootDir));
    m_FileSystemModel.setFilter(QDir::System|QDir::Drives|QDir::AllDirs|QDir::Dirs|QDir::NoDotAndDotDot);
    ui->IDC_FOLDER_TREE->setModel(&m_FileSystemModel);
    ui->IDC_FOLDER_TREE->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->IDC_FOLDER_TREE->setExpandsOnDoubleClick(true);
    ui->IDC_FOLDER_TREE->setAllColumnsShowFocus(true);
    QHeaderView * header = ui->IDC_FOLDER_TREE->header();
    header->setSectionsMovable(true);
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    // find the size column header and hide it
    for(int i=0; i < header->count(); i++)
    {
        QString text = ui->IDC_FOLDER_TREE->model()->headerData(i,header->orientation()).toString();
        if(text == "Size")
        {
            header->setSectionHidden(i,true);
            break;
        }
    }
    SyncFolderTree(rootDir);
}

void RCFigureBrowser::SyncFolderTree(RCCHAR *selDir)
{
    QString path(selDir);
    QModelIndex index = m_FileSystemModel.index(path,0);
    qDebug() << "index" << index.data().toString();
    ui->IDC_FOLDER_TREE->setExpanded(ui->IDC_FOLDER_TREE->currentIndex(),false);
    ui->IDC_FOLDER_TREE->setCurrentIndex(index);
    ui->IDC_FOLDER_TREE->setExpanded(ui->IDC_FOLDER_TREE->currentIndex(),true);
    ui->IDC_FOLDER_TREE->scrollTo(ui->IDC_FOLDER_TREE->currentIndex(),QAbstractItemView::EnsureVisible);
}

void RCFigureBrowser::SetFileNames()
{
    // put the file names in the current folder into the combobox
    RCCHAR filepath[600];

    strcpy(filepath,ui->IDC_FOLDER_COMBO->currentText().data());

    ui->IDC_FILENAME_COMBO->clear();

    if( strlen(filepath) > 0 )
    {
        QString qfilepath(filepath);
        QDir finder(qfilepath);
        finder.setSorting(QDir::IgnoreCase | QDir::LocaleAware);
        QStringList nameFilters;
        nameFilters << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp" << "*.cad";

        if(strlen(m_filter) > 0)
           nameFilters = QString(m_filter).split(';',QString::SkipEmptyParts);

        QStringList files = finder.entryList(nameFilters,QDir::Files,QDir::Name|QDir::LocaleAware);
        for(int i=0; i<files.size(); i++)
        {
            if( ! files.at(i).contains("-BAK"))
            {
                ui->IDC_FILENAME_COMBO->addItem(files.at(i));
                QString itemname = "FilePreviewItem" + QString("-%1").arg(i);
                ui->IDC_FILENAME_COMBO->setItemData(i,itemname);
            }
        }
        ui->IDC_FILENAME_COMBO->setCurrentIndex(0);
    }
}

void RCFigureBrowser::selectFigureListItem(RCCHAR *itemname)
{
    // select the item in the figure list view
    QGraphicsView *itemview;
    if(ui->IDC_STATICPIC->widget() == 0)
        return;
    if((itemview=ui->IDC_STATICPIC->findChild<QGraphicsView *>(QString(itemname))) !=0)
    {
        selectFigureListItem(itemview);
        //ui->IDC_STATICPIC->ensureWidgetVisible(itemview,0,50);
        QRect itemRect = itemview->rect().adjusted(2,2,-2,-2);
        QPoint bottom = itemview->mapTo(ui->IDC_STATICPIC->viewport(),itemRect.bottomLeft());
        QRect viewRect = ui->IDC_STATICPIC->viewport()->rect();
        if( bottom.y() > viewRect.bottom() )
            ui->IDC_STATICPIC->ensureWidgetVisible(itemview,0,itemview->height());
        else
            ui->IDC_STATICPIC->ensureWidgetVisible(itemview,0,0);
    }
}

void RCFigureBrowser::selectFigureListItem(QWidget *listitem)
{
    // select the item in the figure list view
    QRectF brect;
    if(ui->IDC_STATICPIC->widget() == 0)
        return;
    if(selectedView != 0)
    {
        QList<QGraphicsItem*> items = selectedView->scene()->items();
        qDebug() << items;

        if(selectedItem != 0)
        {
            selectedView->scene()->removeItem(selectedItem);
            delete selectedItem;
            selectedItem = 0;
        }
    }
    QGraphicsScene *scene = ((QGraphicsView*)listitem)->scene();
    if(scene != 0)
    {
        QList<QGraphicsItem*> items = scene->items();
        qDebug() << items;
        QRectF srect = scene->itemsBoundingRect();
        QRectF nrect = srect;
        if(srect.height() >= srect.width())
        {
            nrect.setWidth(srect.height());
            nrect.setHeight(srect.height());
        }
        else
        {
            nrect.setWidth(srect.width());
            nrect.setHeight(srect.width());
        }
        nrect.translate(srect.center()-nrect.center());
        int penwidth = (int)ceil(fmax(nrect.width(),nrect.height()) * 0.02);
        if(penwidth < 2) penwidth = 2;
        delete selectedItem;
        selectedItem = scene->addRect(nrect.left(),nrect.top(),nrect.width(),nrect.height(),QPen(QColor(255,0,0),penwidth),Qt::NoBrush);
        selectedView = (QGraphicsView*)listitem;
    }
}

void RCFigureBrowser::initGrid()
{
    // draw the previews in the central graphics view
    int imageWidth = 100, imageHeight = 100;
    int margin = 2;
    int maxcols = 4;
    int row = 0;
    int col = 0;
    QPoint pos(0,0);
    selectedItem = 0;
    selectedView = 0;

    QWidget *baseWidget=0;
    QGridLayout *grid=0;
    if(ui->IDC_STATICPIC->widget() != 0)
    {
        QWidget *w = ui->IDC_STATICPIC->takeWidget();
        w->deleteLater();
    }

    if(ui->IDC_FILENAME_COMBO->count() == 0)
        return;

    baseWidget = new QWidget(this);
    int imgcount = ui->IDC_FILENAME_COMBO->count();
    int rwidth = imgcount%4 != 0 ? imgcount/4+1 : imgcount/4;
    baseWidget->setGeometry(0,0,imageWidth*4,(imgcount%4 != 0 ? imgcount/4+1 : imgcount/4)*imageHeight);
    qDebug() << "base widget rect" << baseWidget->rect();
    if(baseWidget->rect().height() < 1)
        baseWidget->setGeometry(0,0,imageWidth*4,imageHeight);
    grid = new QGridLayout(baseWidget);
    grid->setSpacing(0);
    grid->setContentsMargins(2,2,2,2);

    ui->IDC_STATICPIC->setWidget(baseWidget);
    ui->IDC_STATICPIC->installEventFilter(this);

    m_Interrupted = false;
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(100);

    for(int i=0; i<ui->IDC_FILENAME_COMBO->count(); i++)
    {
        if(col >= maxcols)
        {
            pos = QPoint(0,pos.y()+imageHeight);
            col=0;
            row++;
        }

        if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".cad",Qt::CaseInsensitive))
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView();
            tile->installEventFilter(this);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            //tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setProperty("filename","");
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *scene = new QGraphicsScene();
            tile->setScene(scene);
            tile->setSceneRect(QRect());
            grid->addWidget(tile,row,col);
            //baseWidget->show();
        }
        else
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView();
            tile->setRenderHint(QPainter::Antialiasing,false);
            tile->setRenderHint(QPainter::SmoothPixmapTransform,true);
            tile->setAlignment(Qt::AlignCenter);
            tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            tile->installEventFilter(this);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            //tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setProperty("filename","");
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *scene = new QGraphicsScene();
            tile->setScene(scene);
            tile->setSceneRect(QRect());

            grid->addWidget(tile,row,col);
            //baseWidget->show();
        }
        pos = QPoint(pos.x()+imageWidth,pos.y());
        col++;

    }
    baseWidget->show();
    if(drawtimer == 0)
        drawtimer = new QTimer(this);
    connect(drawtimer, SIGNAL(timeout()), this, SLOT(updateGrid()));
    drawtimer->start();
}

void RCFigureBrowser::updateGrid()
{
    qDebug() << "in updateGrid()";
    // draw the previews in the central graphics view
    int imageWidth = 100, imageHeight = 100;
    int margin = 2;
    int maxcols = 4;
    int row = 0;
    int col = 0;
    QPoint pos(0,0);
    //selectedItem = 0;
    //selectedView = 0;

    QWidget *baseWidget = ui->IDC_STATICPIC->widget();
    QGridLayout *grid=0;
    //if(ui->IDC_STATICPIC->widget() != 0)
    //    ui->IDC_STATICPIC->widget()->deleteLater();

    if(ui->IDC_FILENAME_COMBO->count() == 0)
        return;

    //baseWidget = new QWidget(this);
    int imgcount = ui->IDC_FILENAME_COMBO->count();
    int rwidth = imgcount%4 != 0 ? imgcount/4+1 : imgcount/4;
    // baseWidget->setGeometry(0,0,imageWidth*4,(imgcount%4 != 0 ? imgcount/4+1 : imgcount/4)*imageHeight);
    qDebug() << "base widget rect" << baseWidget->rect();
    //if(baseWidget->rect().height() < 1)
    //    baseWidget->setGeometry(0,0,imageWidth*4,imageHeight);
    grid = (QGridLayout*)baseWidget->layout();
    //grid->setSpacing(0);
    //grid->setContentsMargins(2,2,2,2);

    //ui->IDC_STATICPIC->setWidget(baseWidget);
    //ui->IDC_STATICPIC->installEventFilter(this);

    m_Interrupted = false;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(interrupted()));
    //stoptimer = new QTimer(this);
    //connect(stoptimer, SIGNAL(timeout()), this, SLOT(update()));
    //stoptimer->start(100);

    int itemIndex =0;
    for(int i=0; i<ui->IDC_FILENAME_COMBO->count(); i++)
    {
        if(col >= maxcols)
        {
            pos = QPoint(0,pos.y()+imageHeight);
            col=0;
            row++;
        }


        if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".cad",Qt::CaseInsensitive))
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = (QGraphicsView*)grid->itemAt(itemIndex)->widget();
            //tile->installEventFilter(this);
            //QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            //tile->setObjectName(itemName);
            //ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            //tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            //tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            //tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            //tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            //QGraphicsScene *scene = tile->scene();
            //tile->setScene(scene);
            //tile->setSceneRect(QRect());
            //grid->addWidget(tile,row,col);

            qDebug() << "property : " << tile->property("filename").toString().length();
            if(!tile->property("filename").toString().length() < 1)
            {
                itemIndex++;
                continue;
            }

            if(m_Interrupted)
            {
                m_Interrupted=false;
                return;
            }

            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            //tile->scene()->clear();

            double dtol = v.getreal("db::dtoler");
#if 1
            // fast preview using the header preview image
            //PreviewFigure(tile,qfilepath.data());
            PreviewFigureEx(tile,qfilepath.data());
#else
#if 1
            program->resetinterrupt();
            timer->start(10);
            // slow preview using the drawing data
            v.setreal("db::dtoler",20.0);
            //it->PreviewFigure(tile,::Point(0.0,0.0,1.0),::Point(0.0,0.0,0.0),::Point(0.0,1.0,0.0),0,qfilepath.toLatin1().data());
            PreviewFigure(tile,Point(0.0,0.0,1.0),Point(0.0,0.0,0.0),Point(0.0,1.0,0.0),0,qfilepath.data());
#else
            PreviewFigureMTController(tile,Point(0.0,0.0,1.0),Point(0.0,0.0,0.0),Point(0.0,1.0,0.0),0,qfilepath.data());
#endif
#endif
            timer->stop();
            v.setreal("db::dtoler",dtol);
            //baseWidget->show();
            tile->show();
            program->resetinterrupt();
            m_Interrupted=false;
            return;
        }
        else
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView();
            //tile->setRenderHint(QPainter::Antialiasing,false);
            //tile->setRenderHint(QPainter::SmoothPixmapTransform,true);
            //tile->setAlignment(Qt::AlignCenter);
            //tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            //tile->installEventFilter(this);
            //QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            //tile->setObjectName(itemName);
            //ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            //tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            //tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            //tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            //tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            //QGraphicsScene *scene = new QGraphicsScene();
            //tile->setScene(scene);
            //tile->setSceneRect(QRect());
            //grid->addWidget(tile,row,col);

            qDebug() << "property : " << tile->property("filename").toString().length();
            if(!tile->property("filename").toString().length() < 1)
            {
                itemIndex++;
                continue;
            }

            if(m_Interrupted)
            {
                m_Interrupted=false;
                return;
            }

            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            //tile->scene()->clear();

#if 0
            PreviewImage(tile,qfilepath.data());
#else
            PreviewImageEx(tile,qfilepath.data());
#endif
            timer->stop();
            //baseWidget->show();
            tile->show();
            program->resetinterrupt();
            m_Interrupted=false;
            return;
        }
        pos = QPoint(pos.x()+imageWidth,pos.y());
        col++;
        itemIndex++;

        if(m_Interrupted)
        {
            m_Interrupted=false;
            break;
        }
    }
    baseWidget->show();
}

// threaded image getter
void RCFigureBrowser::UpdateImageListT()
{
    // draw the previews in the central graphics view
    int imageWidth = 100, imageHeight = 100;
    int margin = 2;
    int maxcols = 4;
    int row = 0;
    int col = 0;
    QPoint pos(0,0);
    selectedItem = 0;
    selectedView = 0;

    BaseWidget *baseWidget=0;
    QGridLayout *grid=0;
    if(ui->IDC_STATICPIC->widget() != 0)
    {
        QWidget *w = ui->IDC_STATICPIC->takeWidget();
        w->deleteLater();
    }

    if(ui->IDC_FILENAME_COMBO->count() == 0)
        return;

    baseWidget = new BaseWidget(this);
    ui->IDC_STATICPIC->setWidget(baseWidget);
    ui->IDC_STATICPIC->installEventFilter(this);
    int imgcount = ui->IDC_FILENAME_COMBO->count();
    int rwidth = imgcount%4 != 0 ? imgcount/4+1 : imgcount/4;
    baseWidget->setGeometry(0,0,imageWidth*4,(imgcount%4 != 0 ? imgcount/4+1 : imgcount/4)*imageHeight);
    qDebug() << "base widget rect" << baseWidget->rect();
    if(baseWidget->rect().height() < 1)
        baseWidget->setGeometry(0,0,imageWidth*4,imageHeight);
    baseWidget->grid = grid;
    grid = new QGridLayout(baseWidget);
    grid->setSpacing(0);
    grid->setContentsMargins(2,2,2,2);


    m_Interrupted = false;

    for(int i=0; i<ui->IDC_FILENAME_COMBO->count(); i++)
    {
        if(col >= maxcols)
        {
            pos = QPoint(0,pos.y()+imageHeight);
            col=0;
            row++;
        }

        if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".cad",Qt::CaseInsensitive))
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView();
            tile->installEventFilter(this);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            //tile->setProperty("filename","");
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *scene = new QGraphicsScene();
            tile->setScene(scene);
            tile->setSceneRect(QRect());
            grid->addWidget(tile,row,col);
            baseWidget->views.push_back(tile);
        }
        else
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView(baseWidget);
            tile->setRenderHint(QPainter::Antialiasing,false);
            tile->setRenderHint(QPainter::SmoothPixmapTransform,true);
            tile->setAlignment(Qt::AlignCenter);
            tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            tile->installEventFilter(this);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            //tile->setProperty("filename","");
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *scene = new QGraphicsScene();
            tile->setScene(scene);
            tile->setSceneRect(QRect());
            grid->addWidget(tile,row,col);
            baseWidget->views.push_back(tile);
        }
        pos = QPoint(pos.x()+imageWidth,pos.y());
        col++;
    }
    QStringList files;
    for(int i=0; i<ui->IDC_FILENAME_COMBO->count(); i++)
        files.push_back(ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i));
    baseWidget->getImages(files);
    baseWidget->show();
}

void RCFigureBrowser::ShowImageListWithProgress()
{
    // draw the previews in the central graphics view
    int imageWidth = 100, imageHeight = 100;
    int margin = 2;
    //int maxcols = 4;
    int maxcols = ui->IDC_STATICPIC->width() / imageWidth;
    int minrows = ui->IDC_STATICPIC->height() / imageHeight;
    int row = 0;
    int col = 0;
    QPoint pos(0,0);
    selectedItem = 0;
    selectedView = 0;
    QWidget *baseWidget=0;
    QGridLayout *grid=0;

    if(ui->IDC_STATICPIC->widget() != 0)
    {
        QWidget *tpw = ui->IDC_STATICPIC->takeWidget();
        tpw->hide();
        tpw->deleteLater();
        app->sendPostedEvents(Q_NULLPTR,QEvent::DeferredDelete);
        app->processEvents();
    }
    if(ui->IDC_FILENAME_COMBO->count() == 0)
        return;

    baseWidget = new QWidget(ui->IDC_STATICPIC);
    int imgcount = ui->IDC_FILENAME_COMBO->count();
    //int rwidth = imgcount%4 != 0 ? imgcount/4+1 : imgcount/4;
    baseWidget->setGeometry(0,0,imageWidth*maxcols,(imgcount%maxcols != 0 ? imgcount/maxcols+1 : imgcount/maxcols)*imageHeight);
    //baseWidget->setGeometry(0,0,imageWidth*4,(imgcount%4 != 0 ? imgcount/4+1 : imgcount/4)*imageHeight);
    qDebug() << "base widget rect" << baseWidget->rect();
    if(baseWidget->rect().height() < 1)
        baseWidget->setGeometry(0,0,imageWidth*maxcols,imageHeight);
        //baseWidget->setGeometry(0,0,imageWidth*4,imageHeight);
    else if(baseWidget->rect().height() < ui->IDC_STATICPIC->height())
        baseWidget->setGeometry(0,0,imageWidth*maxcols,minrows*imageHeight);

    int maxrows = baseWidget->height()/imageHeight;

    grid = new QGridLayout(baseWidget);
    grid->setSpacing(0);
    grid->setContentsMargins(2,2,2,2);

    ui->IDC_STATICPIC->setWidget(baseWidget);
    ui->IDC_STATICPIC->removeEventFilter(this);
    ui->IDC_STATICPIC->installEventFilter(this);

    //m_Interrupted = false;

    int numFiles = ui->IDC_FILENAME_COMBO->count();

    // test preview icons
    //QPixmap pixmap(imageWidth,imageHeight);
    //pixmap.load(":/images/NoPreview.svg");
    //pixmap.load(":/images/ATTACHMENT-image.svg");
    //pixmap = pixmap.scaled(imageWidth-8,imageHeight-8,Qt::KeepAspectRatio,Qt::SmoothTransformation);

    //for(int i=0; i<numFiles; i++)
    for(int i=0; i<maxrows*maxcols; i++)
    {
        if(col >= maxcols)
        {
            pos = QPoint(0,pos.y()+imageHeight);
            col=0;
            row++;
        }

        if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".cad",Qt::CaseInsensitive))
        {
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView(baseWidget);
            tile->setRenderHints(tile->renderHints() | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            tile->installEventFilter(this);
            tile->setAlignment(Qt::AlignCenter);
            tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setToolTip(ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            // add the default preview icon
            QGraphicsScene *scene = new QGraphicsScene();
            //QGraphicsPixmapItem *gitem = scene->addPixmap(pixmap);
            //gitem->setPos(4,4);
            if(i < numFiles)
            {
                QGraphicsSimpleTextItem *gitem = scene->addSimpleText("?");
                QFont font = gitem->font();
                font.setPixelSize(80);
                gitem->setFont(font);
            }
            tile->setScene(scene);
            //
            grid->addWidget(tile,row,col);
            qDebug() << "tile rect : " << tile->rect();
        }
        else if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".png",Qt::CaseInsensitive) ||
                ui->IDC_FILENAME_COMBO->itemText(i).contains(".jpg",Qt::CaseInsensitive) ||
                ui->IDC_FILENAME_COMBO->itemText(i).contains(".jpeg",Qt::CaseInsensitive)||
                ui->IDC_FILENAME_COMBO->itemText(i).contains(".bmp",Qt::CaseInsensitive))
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView(baseWidget);
            tile->setRenderHints(tile->renderHints() | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            tile->setAlignment(Qt::AlignCenter);
            tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            tile->installEventFilter(this);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setToolTip(ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            // add the default preview icon
            QGraphicsScene *scene = new QGraphicsScene();
            //QGraphicsPixmapItem *gitem = scene->addPixmap(pixmap);
            //gitem->setPos(4,4);
            if(i < numFiles)
            {
                QGraphicsSimpleTextItem *gitem = scene->addSimpleText("?");
                QFont font = gitem->font();
                font.setPixelSize(80);
                gitem->setFont(font);
            }
            tile->setScene(scene);
            grid->addWidget(tile,row,col);
        }
        else
        {
            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView();
            tile->setRenderHints(tile->renderHints() | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            tile->setAlignment(Qt::AlignCenter);
            tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *scene = new QGraphicsScene();
            tile->setScene(scene);
            grid->addWidget(tile,row,col);
        }
        pos = QPoint(pos.x()+imageWidth,pos.y());
        col++;

    }
    baseWidget->show();
    //ui->IDC_STATICPIC->update();
    app->processEvents();

    QProgressDialog progress("Creating Thumbnails...", "Abort", 0, numFiles, this);
    thumbProgressDlg = &progress;
    progress.setWindowModality(Qt::WindowModal);
    //progress.setMinimumDuration(0);
    //program->setprogressbardlg(&progress);
    //v.setinteger("prg::doprogressuptick",0);

    //QTimer timer(this);
    //connect(&timer, SIGNAL(timeout()), this, SLOT(onIdle()));
    //timer.start(100);

    int itemIndex =0;

    for(int i=0; i<numFiles; i++)
    {
        progress.setValue(i);
        app->processEvents();

        if (progress.wasCanceled())
        {
            program->setinterrupt(true);
            app->processEvents();
            break;
        }
/*
        if(m_Interrupted)
        {
            m_Interrupted=false;
            break;
        }
*/
        if(col >= maxcols)
        {
            pos = QPoint(0,pos.y()+imageHeight);
            col=0;
            row++;
        }

        qDebug() << "filename : " << ui->IDC_FILENAME_COMBO->itemText(i);

        if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".cad",Qt::CaseInsensitive))
        {
            if(grid->count() > itemIndex && grid->itemAt(itemIndex)->widget() != 0)
            {
                qDebug() << "grid : " << grid;
                qDebug() << "grid count : " << grid->count();
                qDebug() << "item at : " << grid->itemAt(itemIndex)->widget();

                QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);
                QGraphicsView *tile = (QGraphicsView*)grid->itemAt(itemIndex)->widget();

                double dtol = v.getreal("db::dtoler");
                PreviewFigureEx(tile,qfilepath.data());
                v.setreal("db::dtoler",dtol);
                tile->update();
            } else
                break;
        }
        else
        {
            if(grid->count() > itemIndex && grid->itemAt(itemIndex)->widget() != 0)
            {
                qDebug() << "grid : " << grid;
                qDebug() << "grid count : " << grid->count();
                qDebug() << "item at : " << grid->itemAt(itemIndex)->widget();

                QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);
                QGraphicsView *tile = (QGraphicsView*)grid->itemAt(itemIndex)->widget();

                PreviewImageEx(tile,qfilepath.data());
                tile->update();
            } else
                break;
        }
        pos = QPoint(pos.x()+imageWidth,pos.y());
        col++;
        itemIndex++;

        progress.setValue(i);
        baseWidget->update();
        //ui->IDC_STATICPIC->update();
    }
    //progress.setValue(numFiles);
    thumbProgressDlg=0;
    //program->setprogressbardlg(0);
    //timer.stop();
    //baseWidget->update();
    //ui->IDC_STATICPIC->update();
    //app->processEvents();
}

void RCFigureBrowser::UpdateImageList()
{
    // draw the previews in the central graphics view
    int imageWidth = 100, imageHeight = 100;
    int margin = 2;
    int maxcols = 4;
    int row = 0;
    int col = 0;
    QPoint pos(0,0);
    selectedItem = 0;
    selectedView = 0;
    QWidget *baseWidget=0;
    QGridLayout *grid=0;

    if(ui->IDC_STATICPIC->widget() != 0)
        ui->IDC_STATICPIC->widget()->deleteLater();

    if(ui->IDC_FILENAME_COMBO->count() == 0)
        return;

    baseWidget = new QWidget(this);
    int imgcount = ui->IDC_FILENAME_COMBO->count();
    int rwidth = imgcount%4 != 0 ? imgcount/4+1 : imgcount/4;
    baseWidget->setGeometry(0,0,imageWidth*4,(imgcount%4 != 0 ? imgcount/4+1 : imgcount/4)*imageHeight);
    qDebug() << "base widget rect" << baseWidget->rect();
    if(baseWidget->rect().height() < 1)
        baseWidget->setGeometry(0,0,imageWidth*4,imageHeight);
    grid = new QGridLayout(baseWidget);
    grid->setSpacing(0);
    grid->setContentsMargins(2,2,2,2);

    ui->IDC_STATICPIC->setWidget(baseWidget);
    ui->IDC_STATICPIC->installEventFilter(this);

    m_Interrupted = false;
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(interrupted()));

    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(100);

    int numFiles = ui->IDC_FILENAME_COMBO->count();

    for(int i=0; i<numFiles; i++)
    {
        if(col >= maxcols)
        {
            pos = QPoint(0,pos.y()+imageHeight);
            col=0;
            row++;
        }

        if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".cad",Qt::CaseInsensitive))
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView();
            tile->setRenderHints(tile->renderHints() | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            tile->installEventFilter(this);
            tile->setAlignment(Qt::AlignCenter);
            tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *scene = new QGraphicsScene();
            tile->setScene(scene);
            tile->setSceneRect(QRect());
            grid->addWidget(tile,row,col);

            //double dtol = v.getreal("db::dtoler");
#if 1
            // fast preview using the header preview image
            //PreviewFigure(tile,qfilepath.data());
            //PreviewFigureEx(tile,qfilepath.data());
#else
#if 1
            // slow preview using the drawing data
            v.setreal("db::dtoler",20.0);
            //it->PreviewFigure(tile,::Point(0.0,0.0,1.0),::Point(0.0,0.0,0.0),::Point(0.0,1.0,0.0),0,qfilepath.toLatin1().data());
            PreviewFigure(tile,Point(0.0,0.0,1.0),Point(0.0,0.0,0.0),Point(0.0,1.0,0.0),0,qfilepath.data());
#else
            v.setreal("db::dtoler",20.0);
            PreviewFigureGL(tile,Point(0.0,0.0,1.0),Point(0.0,0.0,0.0),Point(0.0,1.0,0.0),0,qfilepath.data());
            //PreviewFigureMTController(tile,Point(0.0,0.0,1.0),Point(0.0,0.0,0.0),Point(0.0,1.0,0.0),0,qfilepath.data());
#endif
#endif
            //v.setreal("db::dtoler",dtol);
            //baseWidget->show();
        }
        else
        {
            //QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + "/" + ui->IDC_FILENAME_COMBO->itemText(i);
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);

            grid->setColumnMinimumWidth(col,imageWidth);
            grid->setColumnStretch(col,0);
            grid->setRowMinimumHeight(row,imageHeight);
            grid->setRowStretch(row,0);

            QGraphicsView *tile = new QGraphicsView();
            tile->setRenderHints(tile->renderHints() | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            //tile->setRenderHint(QPainter::Antialiasing,false);
            //tile->setRenderHint(QPainter::SmoothPixmapTransform,true);
            tile->setAlignment(Qt::AlignCenter);
            tile->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            tile->installEventFilter(this);
            QString itemName = "FilePreviewItem" + QString("-%1").arg(i);
            tile->setObjectName(itemName);
            ui->IDC_FILENAME_COMBO->setItemData(i,itemName);
            tile->setProperty("filename",ui->IDC_FILENAME_COMBO->itemText(i));
            tile->setGeometry(QRect(pos.x(),pos.y(),imageWidth,imageHeight));
            tile->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            tile->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            QGraphicsScene *scene = new QGraphicsScene();
            tile->setScene(scene);
            tile->setSceneRect(QRect());

            grid->addWidget(tile,row,col);
#if 0
            PreviewImage(tile,qfilepath.data());
#else
            //PreviewImageEx(tile,qfilepath.data());
#endif
            //baseWidget->show();
        }
        pos = QPoint(pos.x()+imageWidth,pos.y());
        col++;
     }

    int itemIndex =0;
    //timer->start();

    for(int i=0; i<numFiles; i++)
    {
        if(m_Interrupted)
        {
            m_Interrupted=false;
            break;
        }

        if(col >= maxcols)
        {
            pos = QPoint(0,pos.y()+imageHeight);
            col=0;
            row++;
        }

        if(ui->IDC_FILENAME_COMBO->itemText(i).contains(".cad",Qt::CaseInsensitive))
        {
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);
            QGraphicsView *tile = (QGraphicsView*)grid->itemAt(itemIndex)->widget();

            double dtol = v.getreal("db::dtoler");
            PreviewFigureEx(tile,qfilepath.data());
            v.setreal("db::dtoler",dtol);
        }
        else
        {
            QString qfilepath =  ui->IDC_FOLDER_COMBO->currentText() + QDir::separator() + ui->IDC_FILENAME_COMBO->itemText(i);
            QGraphicsView *tile = (QGraphicsView*)grid->itemAt(itemIndex)->widget();

            PreviewImageEx(tile,qfilepath.data());
        }
        pos = QPoint(pos.x()+imageWidth,pos.y());
        col++;
        itemIndex++;

        app->processEvents();

    }
    baseWidget->show();
}

void RCFigureBrowser::on_IDC_FOLDER_TREE_pressed(const QModelIndex &index)
{
    m_Interrupted = false;
    QString selPath = fixFileName(m_FileSystemModel.filePath(index));
    if(thumbProgressDlg != 0)
    {
        thumbProgressDlg->cancel();
        m_Interrupted = true;
    }
    int pos = ui->IDC_FOLDER_COMBO->findText(selPath);
    if(pos < 0)
    {
        //ui->IDC_FOLDER_COMBO->addItem(selPath);
        //ui->IDC_FOLDER_COMBO->setCurrentIndex(ui->IDC_FOLDER_COMBO->count()-1);
        //pos = ui->IDC_FOLDER_COMBO->currentIndex();
        //ui->IDC_FOLDER_COMBO->insertItem(ui->IDC_FOLDER_COMBO->count()-1,selPath);
        m_insertingFolderCB = true;
        ui->IDC_FOLDER_COMBO->insertItem(pos,selPath);
        pos = ui->IDC_FOLDER_COMBO->findText(selPath);
        m_insertingFolderCB = false;
        //if(pos != ui->IDC_FOLDER_COMBO->currentIndex())
        //    ui->IDC_FOLDER_COMBO->setCurrentIndex(pos);
        //return;
    }
    if(pos != ui->IDC_FOLDER_COMBO->currentIndex())
        ui->IDC_FOLDER_COMBO->setCurrentIndex(pos);
    else
       on_IDC_FOLDER_COMBO_currentIndexChanged(selPath);
}

void RCFigureBrowser::resizeEvent(QResizeEvent *e)
{
    //QDialog::resizeEvent(e);

    // if the dialog has been maximised
    // set it to the old size and first position
    // this has no efect on windows
    // this will be removed when the dialog
    // can be made to maximise correctly (v8)
    if(e->size().height() > maximumHeight())
    {
        move(mFirstPos);
        resize(e->oldSize());
    }
}

void RCFigureBrowser::moveEvent(QMoveEvent *e)
{
    QDialog::moveEvent(e);
}

void RCFigureBrowser::SetFigureXScale(double scale)
{
    ui->XScale->setValue(scale);
    on_XScale_valueChanged(scale);
}

void RCFigureBrowser::SetFigureYScale(double scale)
{
    ui->YScale->setValue(scale);
}

void RCFigureBrowser::SetFigureZScale(double scale)
{
    ui->ZScale->setValue(scale);
}

void RCFigureBrowser::SetBitmapWidth(double width)
{
    ui->Width->setValue(width);
}

void RCFigureBrowser::SetBitmapHeight(double height)
{
    ui->Height->setValue(height);
}

double RCFigureBrowser::FigureXScale()
{
    return ui->XScale->value();
}

double RCFigureBrowser::FigureYScale()
{
    return ui->YScale->value();
}

double RCFigureBrowser::FigureZScale()
{
    return ui->ZScale->value();
}

double RCFigureBrowser::BitmapWidth()
{
    return ui->Width->value();
}

double RCFigureBrowser::BitmapHeight()
{
    return ui->Height->value();
}

void RCFigureBrowser::SetPrefix(RCCHAR *prefix)
{
    m_Prefix = QString(prefix);
}

RCCHAR* RCFigureBrowser::GetPrefix()
{
    return m_Prefix.data();
}

void RCFigureBrowser::SetFigureOriginalStyle(bool OnOff)
{
    ui->originalStyle->setChecked(OnOff);
}

void RCFigureBrowser::SetFigureOriginalColour(bool OnOff)
{
    ui->originalColour->setChecked(OnOff);
}

void RCFigureBrowser::SetFigureOriginalLayer(bool OnOff)
{
    ui->originalLayer->setChecked(OnOff);
}

void RCFigureBrowser::SetFigureOriginalWeight(bool OnOff)
{
    ui->originalWeight->setChecked(OnOff);
}

void RCFigureBrowser::SetFigureOrientToView(bool OnOff)
{
    ui->orientToView->setChecked(OnOff);
}

void RCFigureBrowser::SetBitmapOriginalShape(bool OnOff)
{
    ui->originalShape->setChecked(OnOff);
}

void RCFigureBrowser::SetBitmapLinkFile(bool OnOff)
{
    ui->linkToFile->setChecked(OnOff);
}

bool RCFigureBrowser::GetFigureOriginalStyle()
{
    return ui->originalStyle->isChecked();
}

bool RCFigureBrowser::GetFigureOriginalColour()
{
    return ui->originalColour->isChecked();
}

bool RCFigureBrowser::GetFigureOriginalLayer()
{
    return ui->originalLayer->isChecked();
}

bool RCFigureBrowser::GetFigureOriginalWeight()
{
    return ui->originalWeight->isChecked();
}

bool RCFigureBrowser::GetFigureOrientToView()
{
    return ui->orientToView->isChecked();
}

bool RCFigureBrowser::GetBitmapOriginalShape()
{
    return ui->originalShape->isChecked();
}

bool RCFigureBrowser::GetBitmapLinkFile()
{
    return ui->linkToFile->isChecked();
}

void RCFigureBrowser::SetShowHidden()
{
    if(Act_ShowHidden->isChecked())
    {
        QDir::Filters flags = m_FileSystemModel.filter();
        qDebug() << flags;
        flags |= QDir::Hidden;
        qDebug() << flags;
        m_FileSystemModel.setFilter(m_FileSystemModel.filter() | QDir::Hidden);
    }
    else
    {
        QDir::Filters flags = m_FileSystemModel.filter();
        qDebug() << flags;
        flags &= ~QDir::Hidden;
        qDebug() << flags;
        m_FileSystemModel.setFilter(flags);
    }
    ui->IDC_FOLDER_TREE->reset();
    m_FileSystemModel.setRootPath(QString(QDir::separator())/*"/"*/);
    ui->IDC_FOLDER_TREE->setModel(&m_FileSystemModel);
    SyncFolderTree(m_rootDir);
}

void RCFigureBrowser::on_IDC_FOLDER_TREE_customContextMenuRequested(const QPoint &pos)
{
    m_FileTreeMenu->exec(ui->IDC_FOLDER_TREE->mapToGlobal(pos));
    m_FileTreeMenu->hide();
}

void RCFigureBrowser::on_originalLayer_clicked()
{
    //UpdateImageList();
    ui->IDC_FILENAME_COMBO->currentIndexChanged(ui->IDC_FILENAME_COMBO->currentText());
}

void RCFigureBrowser::on_originalColour_clicked()
{
    //UpdateImageList();
    ui->IDC_FILENAME_COMBO->currentIndexChanged(ui->IDC_FILENAME_COMBO->currentText());
}

void RCFigureBrowser::on_originalStyle_clicked()
{
    //UpdateImageList();
    ui->IDC_FILENAME_COMBO->currentIndexChanged(ui->IDC_FILENAME_COMBO->currentText());
}

void RCFigureBrowser::on_originalWeight_clicked()
{
    //UpdateImageList();
    ui->IDC_FILENAME_COMBO->currentIndexChanged(ui->IDC_FILENAME_COMBO->currentText());
}

QString RCFigureBrowser::GetPreviewCacheName(const QString &figurename)
{
    // set up the temporary preview storage path
    QString appTitle(home.getApplicationTitle());
    QFileInfo figureFileInfo(figurename);
    QString qfigurePath(QDir::toNativeSeparators(figureFileInfo.dir().absolutePath()));
    QString qpreviewname = figureFileInfo.fileName().append(".png");
#ifdef _MAC
    //QString tempPath = QDir::toNativeSeparators("/Library/Caches/");
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
#else
    //QString tempPath = QDir::toNativeSeparators(QDir::tempPath());
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation);
    int index = qfigurePath.indexOf(":");
    qfigurePath = qfigurePath.mid(index+1);
#endif
#if 0
    int index = qfigurePath.indexOf(appTitle);
    if(index < 0)
        index = qfigurePath.indexOf(QDir::rootPath()) + QDir::rootPath().length();
    else
        index += appTitle.length();
    if(qfigurePath.mid(index).startsWith(QDir::separator()))
        index++;
    if( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    tempPath += appTitle +  QDir::toNativeSeparators("/PreviewCache/") + qfigurePath.mid(index);
    if( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    // create the temporary folder if needed
    QDir imageDir;
    if(!QFileInfo::exists(tempPath))
        imageDir.mkpath(tempPath);
    // add the preview file name
    tempPath = QDir::toNativeSeparators(tempPath + qpreviewname);
#else
    // add the path of the cache folder to the beginning of the filename
    // if on windows remove the drive letter
    if( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    tempPath = QDir::toNativeSeparators(tempPath + appTitle+ "/PreviewCache" +  qfigurePath);
    if( !tempPath.endsWith(QDir::separator()) )
        tempPath += QDir::separator();
    QDir imageDir;
    // create the temporary folder if needed
    if(!QFileInfo::exists(tempPath))
        imageDir.mkpath(tempPath);
    // add the preview file name
    tempPath = QDir::toNativeSeparators(tempPath + qpreviewname);
#endif
    qInfo() << "tempPath : " << tempPath;
    return tempPath;
}

QPixmap RCFigureBrowser::GetOnePreviewImage(const QString &previewFileName, int width, int height)
{
    QPixmap pixmap;
    if(QFileInfo::exists(previewFileName))
    {
        if(!QPixmapCache::find(previewFileName,pixmap))
        {
            pixmap.load(previewFileName,"PNG");
            QPixmapCache::insert(previewFileName,pixmap);
            return pixmap;
        }
    }
    pixmap = QPixmap(width,height);
    pixmap.fill(QColor(255,255,255));
    return pixmap;
}

void RCFigureBrowser::CreateOneFigurePreviewFile(const QString &figurePath, int width, int height)
{
    QPixmap image;
    QString tempPath = GetPreviewCacheName(figurePath);

}

void RCFigureBrowser::CreateFigurePreviewFiles(const QStringList &figurePaths, int width, int height)
{

}

void RCFigureBrowser::CreateOneImagePreviewFile(const QString &imagePath, int width, int height)
{
    QPixmap image;
    QString tempPath = GetPreviewCacheName(imagePath);

    image.load(imagePath);
    if(!image.isNull())
    {
        image = image.scaled(width,height,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        image.save(tempPath);
        QPixmapCache::insert(tempPath,image);
    }
}

void RCFigureBrowser::CreateImagePreviewFiles(const QStringList &imagesPaths, int width, int height)
{

}

void RCFigureBrowser::GetPreviewPage(int page)
{

}

void RCFigureBrowser::ShowPreviewFiles()
{

}

void RCFigureBrowser::on_RCFigureBrowser_accepted()
{

}

// set the position of the dialog when first shown
void RCFigureBrowser::getFirstPos()
{
    mFirstPos = pos();
    activateWindow();
    raise();
}

void RCFigureBrowser::onIdle()
{
    if(ui->IDC_STATICPIC->widget() != 0)
        ui->IDC_STATICPIC->widget()->update();
    app->processEvents();
}


void RCFigureBrowser::on_IDC_FOLDER_COMBO_activated(const QString &arg1)
{
    //on_IDC_FOLDER_COMBO_currentIndexChanged(arg1);
    ui->IDC_FOLDER_COMBO->currentIndexChanged(arg1);
}

void RCFigureBrowser::on_IDC_FILENAME_COMBO_activated(const QString &arg1)
{
    //on_IDC_FILENAME_COMBO_currentTextChanged(arg1);
    ui->IDC_FILENAME_COMBO->currentIndexChanged(arg1);
}

// v8 enhancements
void RCFigureBrowser::SetFigureXSize(double size)
{
    ui->XSize->setValue(size);
}

void RCFigureBrowser::SetFigureYSize(double size)
{
    ui->YSize->setValue(size);
}

void RCFigureBrowser::SetFigureZSize(double size)
{
    ui->ZSize->setValue(size);
}

double RCFigureBrowser::FigureXSize()
{
    return ui->XSize->value();
}

double RCFigureBrowser::FigureYSize()
{
    return ui->YSize->value();
}

double RCFigureBrowser::FigureZSize()
{
    return ui->ZSize->value();
}

void RCFigureBrowser::on_SizeCheckBox_clicked()
{
    switch(ui->SizeCheckBox->checkState())
    {
    case Qt::Unchecked :
        ui->ScaleCheckBox->setCheckState(Qt::Checked);
        ui->XSize->setEnabled(false);
        ui->YSize->setEnabled(false);
        ui->ZSize->setEnabled(false);

        ui->XScale->setEnabled(true);
        ui->YScale->setEnabled(true);
        ui->ZScale->setEnabled(true);
        break;
    case Qt::Checked :
        ui->ScaleCheckBox->setCheckState(Qt::Unchecked);
        ui->XSize->setEnabled(true);
        ui->YSize->setEnabled(true);
        ui->ZSize->setEnabled(true);

        ui->XScale->setEnabled(false);
        ui->YScale->setEnabled(false);
        ui->ZScale->setEnabled(false);
        break;
    case Qt::PartiallyChecked :
        ui->ScaleCheckBox->setCheckState(Qt::Checked);
        ui->XSize->setEnabled(false);
        ui->YSize->setEnabled(false);
        ui->ZSize->setEnabled(false);

        ui->XScale->setEnabled(false);
        ui->YScale->setEnabled(false);
        ui->ZScale->setEnabled(false);
        break;
    };
}

void RCFigureBrowser::on_ScaleCheckBox_clicked()
{
    switch(ui->ScaleCheckBox->checkState())
    {
    case Qt::Unchecked :
        ui->SizeCheckBox->setCheckState(Qt::Checked);
        ui->XScale->setEnabled(false);
        ui->YScale->setEnabled(false);
        ui->ZScale->setEnabled(false);

        ui->XSize->setEnabled(true);
        ui->YSize->setEnabled(true);
        ui->ZSize->setEnabled(true);
        break;
    case Qt::Checked :
        ui->SizeCheckBox->setCheckState(Qt::Unchecked);
        ui->XScale->setEnabled(true);
        ui->YScale->setEnabled(true);
        ui->ZScale->setEnabled(true);

        ui->XSize->setEnabled(false);
        ui->YSize->setEnabled(false);
        ui->ZSize->setEnabled(false);
        break;
    case Qt::PartiallyChecked :
        ui->SizeCheckBox->setCheckState(Qt::Checked);
        ui->XScale->setEnabled(false);
        ui->YScale->setEnabled(false);
        ui->ZScale->setEnabled(false);

        ui->XSize->setEnabled(false);
        ui->YSize->setEnabled(false);
        ui->ZSize->setEnabled(false);
        break;
    };
}

void RCFigureBrowser::SetSizeChecked(bool checked)
{
    m_UseSize = checked;
    ui->SizeCheckBox->setChecked(m_UseSize);
    on_SizeCheckBox_clicked();
}

bool RCFigureBrowser::SizeIsChecked()
{
    return ui->SizeCheckBox->isChecked();
}

void RCFigureBrowser::on_SizeCheckBox_clicked(bool checked)
{
    m_UseSize = checked;
    on_ScaleCheckBox_clicked();
}

void RCFigureBrowser::on_ScaleCheckBox_clicked(bool checked)
{
    m_UseSize = !checked;
    on_SizeCheckBox_clicked();
}

void RCFigureBrowser::on_XSize_valueChanged(double v)
{
    if(!ui->ScaleCheckBox->isChecked())
       ui->XScale->setValue(v / mLibrarySize.x);

    if(ui->_1008->isChecked())
        ui->YSize->setValue(v);
}

void RCFigureBrowser::on_YSize_valueChanged(double v)
{
    if(!ui->ScaleCheckBox->isChecked())
       ui->YScale->setValue(v / mLibrarySize.y);

    if(ui->_1008->isChecked())
        ui->XSize->setValue(v);
}

void RCFigureBrowser::on_ZSize_valueChanged(double v)
{
    if(!ui->ScaleCheckBox->isChecked())
       ui->ZScale->setValue(v / mLibrarySize.z);
}

void RCFigureBrowser::on_XScale_valueChanged(double v)
{
    if(!ui->SizeCheckBox->isChecked())
       ui->XSize->setValue(mLibrarySize.x * v);

    if(ui->_1008->isChecked())
        ui->YScale->setValue(v);

}

void RCFigureBrowser::on_YScale_valueChanged(double v)
{
    if(!ui->SizeCheckBox->isChecked())
       ui->YSize->setValue(mLibrarySize.y * v);

    if(ui->_1008->isChecked())
        ui->XScale->setValue(v);
}

void RCFigureBrowser::on_ZScale_valueChanged(double v)
{
    if(!ui->SizeCheckBox->isChecked())
       ui->ZSize->setValue(mLibrarySize.z * v);
}

void RCFigureBrowser::GetFigureExtents(RCCHAR *filepath, Point &pmin, Point &pmax)
{
    BitMask options(32);
    Figure figure(filepath,_RCT(""),Point(0.0,0.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
    figure.extents(0,&pmin,&pmax);
}
