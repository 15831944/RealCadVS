
#include "ncwin.h"
#include "QDebug.h"
#include "mainwindow.h"
#include "RCView.h"
#include "RCGraphicsScene.h"
#include "RCGraphicsView.h"
#include "RCCentralwidget.h"

#include <QHBoxLayout>
#include <QSplitter>

extern void file_open(int nocadfiles,RCCHAR *fname);
extern void open_dwg(RCCHAR *filename);
extern void open_dxf(RCCHAR *filename);

void RCCentralWidget::debugView3dList()
{
    View3dWindowList list = cadwindow->GetView3dList()->copy();
    //list->reverse();
    View3dWindow *window;
    int i=0;
    qDebug() <<  "*** view3d list start ***";
    for(list.start(); (window = list.next()) != 0; )
    {
        if(window->getview())
            qDebug() << "view3d [" << i << "] - " << window << " " << QString(window->getview()->getname());
        i++;
    }
    qDebug() << "*** view3d list end ***";
    list.destroy();
}

RCCentralWidget::RCCentralWidget(QWidget *parent) :
    QWidget(parent)
{
    //qDebug() << "mainwindow central widget : " << ((QtMainWindow*)parent)->centralWidget();

    // enable drag and drop to the main window
    setAcceptDrops(false);

    setMouseTracking(false);
    //mGraphicsScene = new RCGraphicsScene;
    //mGraphicsScene = 0;

    /*
    h1Splitter = new QSplitter;
    h1Splitter->setHandleWidth(1);
    h2Splitter = new QSplitter;
    h2Splitter->setHandleWidth(1);

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setHandleWidth(1);
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);
    vSplitter->addWidget(h2Splitter);

    MyView *view = new MyView("Top left view");
    view->view()->setScene(graphicsScene);
    h1Splitter->addWidget(view);

    view = new MyView("Top right view");
    view->view()->setScene(graphicsScene);
    h1Splitter->addWidget(view);

    view = new MyView("Bottom left view");
    view->view()->setScene(graphicsScene);
    h2Splitter->addWidget(view);

    view = new MyView("Bottom right view");
    view->view()->setScene(graphicsScene);
    h2Splitter->addWidget(view);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(vSplitter);
    setLayout(layout);
    */

    // initial set up is a single view in the workspace
    // with no splitters
#if 1
    QGLWidget *glw = new QGLWidget(QGLFormat(QGL::SampleBuffers));
#else
    RCGLWidget *glw = new RCGLWidget(QGLFormat(QGL::SampleBuffers));
#endif
    glw->setObjectName("QGLWidget");
    RCGraphicsView  *graphicsView = new RCGraphicsView();
    graphicsView->setObjectName("RCGraphicsView");
    graphicsView->setViewport(glw);
    qDebug() << "graphics view parent : " << graphicsView->parent();
    qDebug() << "glw parent : " << glw->parent();
    RCGraphicsScene *mGraphicsScene = new RCGraphicsScene(graphicsView);
    graphicsView->setScene(mGraphicsScene);
    RCView *view = new RCView("InitialView",graphicsView,this);
    view->setObjectName("InitialView");
    qDebug() << "view parent : " << view->parent();
    //view->setView(graphicsView);
    setGeometry(view->geometry());

    mSplitter = 0;
    mNewSplitter = 0;
    //mSplitter = new QSplitter;
    //mSplitter->addWidget(view);

    mLayout = new QVBoxLayout;
    mLayout->setMargin(0);
    mLayout->setContentsMargins(0,0,0,0);
    mLayout->setSpacing(0);
    //QWidget *dummy = new QWidget;
    mLayout->addWidget(view);
    //layout->addWidget(mSplitter);
    setLayout(mLayout);

    qDebug() << "RCCentralWidget layout geometry  : " << mLayout->geometry();
    qDebug() << "RCCentralWidget view width  : " << view->width();
    qDebug() << "RCCentralWidget view height : " << view->height();
    qDebug() << "RCCentralWidget geometry    : " << geometry();
    qDebug() << "RCCentralWidget scenerect     : " << mGraphicsScene->sceneRect();
    qDebug() << "RCCentralWidget gviewportrect : " << graphicsView->viewport()->geometry();

    tiled = 0;
}

void RCCentralWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "in RCCentralWidget::dragEnterEvent";
    qDebug() << "proposed drop action : " << event->proposedAction();
    if(event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
    //emit changed(event->mimeData());
}
/*
void RCCentralWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    qDebug() << "in RCCentralWidget::dragLeaveEvent";
    event->accept();
}
*/
/*
void RCCentralWidget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "in RCCentralWidget::dragMoveEvent";
    qDebug() << "proposed drop action : " << event->proposedAction();
    event->acceptProposedAction();
}
*/
void RCCentralWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    qDebug() << "in RCGraphicsView::dropEvent";

#if 1
    // only looking for files at the moment
    if (mimeData->hasUrls())
    {
        qDebug() << "in RCCentralWidget::dropEvent";

        QStringList pathList;
        QList<QUrl> urlList = mimeData->urls();
        if(urlList.isEmpty())
            return;

        QString filepath = urlList.first().toLocalFile();
        if(filepath.isEmpty())
            return;

        /*
        for (int i = 0; i < urlList.size() && i < 32; ++i)
        {
            pathList.append(urlList.at(i).toLocalFile());
        }
        */

        //if(pathList.length() > 0)
        {
            //QString filepath = pathList.at(0);
            if(filepath.toLower().right(4) == ".cad")
            {
                qDebug() << "proposed drop action : " << event->proposedAction();
                app->processEvents();
                event->acceptProposedAction();
                ResourceString rs2(NCMAIN+2),rs3(NCMAIN+3);
                if(filechanged(rs2.gets(),rs3.gets()))
                {
                    //db.initialize();
                    //app->processEvents();
                    file_open(0,filepath.data());

                }
                app->processEvents();
            }
            else if(filepath.toLower().right(4) == ".dwg")
            {
                event->acceptProposedAction();
                open_dwg(filepath.data());
            }
            else if(filepath.toLower().right(4) == ".dxf")
            {
                event->acceptProposedAction();
                open_dxf(filepath.data());
            }
            //else if(filepath.toLower().right(4) == ".igs")
           // {
            //    open_iges(filepath.data());
            //}
        }
    }
#else
    if (mimeData->hasImage())
    {
        qDebug() << "image data was droped";
        //setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    }
    else if (mimeData->hasHtml())
    {
        qDebug() << "rich text data was droped";
        qDebug() << mimeData->html();
        //setText(mimeData->html());
        //setTextFormat(Qt::RichText);
    }
    else if (mimeData->hasText())
    {
        qDebug() << "plain text data was droped";
        qDebug() << mimeData->text();
        //setText(mimeData->text());
        //setTextFormat(Qt::PlainText);
    }
    else if (mimeData->hasUrls())
    {
        qDebug() << "a file(s) was droped";
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            text += url + QString("\n");
            qDebug() << text;
        }
        //setText(text);
    }
    else
    {
        qDebug() << "Cannot display data";
        //setText(tr("Cannot display data"));
    }

    event->acceptProposedAction();
#endif
}

void RCCentralWidget::initializeSplitter(QSplitter *splitter)
{
    int count = splitter->count();
    for(int i=0; i < count; i++)
    {
        if(qobject_cast<RCView *>(splitter->widget(i)) != 0)
        {
            //RCView *view = (RCView*)splitter->widget(i);
            //deleteView(view);
            splitter->widget(i)->hide();
            splitter->widget(i)->setParent(0);
            //splitter->widget(i)->deleteLater();
        }
        else if(qobject_cast<QSplitter *>(splitter->widget(i)) != 0)
        {
            QSplitter *thisSplitter = (QSplitter*)splitter->widget(i);
            initializeSplitter(thisSplitter);
            thisSplitter->setParent(0);
            thisSplitter->deleteLater();
            //splitter->widget(i)->setParent(0);
            //splitter->widget(i)->deleteLater();
        }
        count = splitter->count();
        i=-1;
    }
}

void RCCentralWidget::initialise(Qt::Orientation orientation)
{
#if 1
    if(mSplitter != 0)
    {
        //mSplitter->hide();
        int count = mSplitter->count();
        for(int i=0; i < count; i++)
        {
            if(qobject_cast<RCView *>(mSplitter->widget(i)) != 0)
            {
                //RCView *view = (RCView*)mSplitter->widget(i);
                //deleteView(view);
                qDebug() << "count before set parent null " << mSplitter->count();
                mSplitter->widget(i)->hide();
                mSplitter->widget(i)->setParent(0);
                qDebug() << "after set parent null " << mSplitter->widget(i);
                qDebug() << "count after set parent null " << mSplitter->count();
                //mSplitter->widget(i)->deleteLater();
                //mSplitter->widget(i)->close();
                //mSplitter->widget(i)->deleteLater();
                //mSplitter->widget(i)->setParent(0);
                //delete mSplitter->widget(i);
                //deleteView(view);
            }
            else if(qobject_cast<QSplitter *>(mSplitter->widget(i)) != 0)
            {
                initializeSplitter((QSplitter*)mSplitter->widget(i));
                mSplitter->widget(i)->setParent(0);
                mSplitter->widget(i)->deleteLater();
            }
            count = mSplitter->count();
            i=-1;
        }
        mSplitter->setOrientation(orientation);
        mSplitter->show();
    }

    if(mSplitter != 0)
        qDebug() << "splitter count : " << mSplitter->count();

    debug();

    tiled = 0;
#else
    if(mSplitter != 0)
    {
        mSplitter->hide();
        int count = mSplitter->count();
        for(int i=0; i < count; i++)
        {
            //RCView *view = (RCView*)mSplitter->widget(i);
            //deleteView(view);
            mSplitter->widget(i)->deleteLater();
            mSplitter->widget(i)->setParent(0);
            //deleteView(view);
            count = mSplitter->count();
            i=-1;
        }
        mSplitter->setOrientation(orientation);
        mSplitter->show();
    }

    qDebug() << "splitter count : " << mSplitter->count();

    tiled = 0;
#endif
}

void RCCentralWidget::purgeSplitters()
{
    if(mSplitter == 0)
        return;
    QList<QSplitter *> allSplitters = mSplitter->findChildren<QSplitter *>();

    for(int i=0; i<allSplitters.size(); i++)
    {
        QSplitter *thisSplitter = allSplitters.at(i);
        QList<RCView *> views = thisSplitter->findChildren<RCView *>(QString(),Qt::FindDirectChildrenOnly);
        if(views.size() < 1)
        {
            if(thisSplitter != mSplitter)
            {
                QList<QSplitter *> splitters = thisSplitter->findChildren<QSplitter *>(QString(),Qt::FindDirectChildrenOnly);
                if(splitters.count() < 1)
                {
                    thisSplitter->setParent(0);
                    thisSplitter->deleteLater();
                }
            }
        }
    }
#if 0
    QList<QSplitter *> allSplitters = findChildren<QSplitter *>();
    for(int i=0; i<allSplitters.size(); i++)
    {
        QSplitter *thisSplitter = allSplitters.at(i);
        if(thisSplitter == mSplitter)
            continue;
        QList<RCView *> views = thisSplitter->findChildren<RCView *>();
        if(views.size() < 1)
        {
            thisSplitter->deleteLater();
            thisSplitter->setParent(0);
        }
    }
#endif
}

int RCCentralWidget::viewCount()
{
    QList<RCView *> allViews;
    if(mSplitter != 0)
        allViews = mSplitter->findChildren<RCView *>();
    return allViews.size();
}

int RCCentralWidget::visibleViewCount()
{
    QList<RCView *> allViews;
    int count=0;
    if(mSplitter != 0)
    {
        allViews = mSplitter->findChildren<RCView *>();
        for(int j=0; j<allViews.size(); j++)
        {
            if(allViews.at(j)->isVisible())
                count++;
        }
    }
    return count;
}

int RCCentralWidget::visibleSplitterCount(QSplitter *splitter, int &counter)
{
    QList<RCView *> views;
    int thisCount=0;
    if(splitter != 0)
    {
        QList<int> sizes = splitter->sizes();
        for(int j=0; j<sizes.size(); j++)
        {
            if(sizes.at(j) > 0)
                thisCount++;
        }
        counter += thisCount;
        for(int k=0; k<splitter->count(); k++)
        {
            if(qobject_cast<QSplitter*>(splitter->widget(k)) != 0 && sizes.at(k) > 0)
                visibleSplitterCount((QSplitter*)splitter->widget(k),counter);
        }
    }
    return counter;
}

int RCCentralWidget::visibleSplitterCount()
{
    int counter=0;
    return visibleSplitterCount(mSplitter,counter);
}

int RCCentralWidget::splitterCount()
{
    //purgeSplitters();
    QList<QSplitter *> allSplitters;
    if(mSplitter != 0)
        allSplitters = mSplitter->findChildren<QSplitter *>();
    return allSplitters.size();
}

void RCCentralWidget::tile()
{
    // even number of views
    // tiled according to the width of the view
    // if the width will be less that the width of the text
    // on the left side of the view an extra row is added
    //
    // *********
    // *   *   *
    // *   *   *
    // *********
    // *   *   *
    // *   *   *
    // *********
    //
    // odd number of views
    // tiled with large view on the left and small horizontal views on the right
    //
    // ***********
    // *     *   *
    // *     *   *
    // *     *****
    // *     *   *
    // *     *   *
    // ***********

    debugView3dList();

    int i=0;
    View3dWindow *window;
    View3dWindowList list = cadwindow->GetView3dList()->copy();
    int count = list.length();
    if(count == 1)
        return;
    initialise(Qt::Horizontal);
    purgeSplitters();
    app->sendPostedEvents(Q_NULLPTR,QEvent::DeferredDelete);
    app->processEvents();
    mSplitter->setProperty("lastState",mSplitter->saveState());
    if(count % 2 == 0)
    {
        if(count == 2)
        {
            mSplitter->setOrientation(Qt::Horizontal);
            for(list.start(); (window = list.next()) != 0;)
            {
                RCView *view = (RCView*)window->gethwnd();
                qDebug() << "view name - " << QString(view->getView3dWindow()->getview()->getname());
                addView(view,0);
            }
        }
        else
        {
            mSplitter->setOrientation(Qt::Vertical);
            QSplitter *topSplitter = new QSplitter(Qt::Horizontal,mSplitter);
            mSplitter->addWidget(topSplitter);
            QSplitter *bottomSplitter = new QSplitter(Qt::Horizontal,mSplitter);
            mSplitter->addWidget(bottomSplitter);
            int height = rect().height()/2;
            QList<int> sizes;
            sizes.append(height);
            sizes.append(height);
            mSplitter->setSizes(sizes);

            debugView3dList();

            i=0;
            list.start();
            while((window = list.next()) != 0)
            {
                RCView *view = (RCView*)window->gethwnd();
                qDebug() << "view name - " << QString(view->getView3dWindow()->getview()->getname());
                addView(bottomSplitter,view,0);
                i++;
                if(i >= count/2)
                    break;
            }
            i=0;
            while((window = list.next()) != 0)
            {
                RCView *view = (RCView*)window->gethwnd();
                qDebug() << "view name - " << QString(view->getView3dWindow()->getview()->getname());
                addView(topSplitter,view,0);
                i++;
                if(i >= count/2)
                    break;
            }
        }
    }
    else
    {
        mSplitter->setOrientation(Qt::Horizontal);
        QSplitter *rightSplitter = new QSplitter(Qt::Vertical,mSplitter);
        int width = rect().width()/2;
        //int height = rect().height()/((count-1)/2);
        QList<int> sizes;
        sizes.append(width);
        sizes.append(width);

        // set the large window as the current window
        RCView *currentView = (RCView*)cadwindow->getcurrentwindow()->gethwnd();

        // add the left side view
        addView(mSplitter,currentView);
        // add the right side splitter
        mSplitter->addWidget(rightSplitter);
        mSplitter->setSizes(sizes);

        // add the right side windows
        list.start();
        while((window = list.next()) != 0)
        {
            RCView *view = (RCView*)window->gethwnd();
            qDebug() << "view name - " << QString(view->getView3dWindow()->getview()->getname());
            if(view != currentView)
                addView(rightSplitter,view,0);
        }
    }
    if(viewCount() > 1)
        tiled = 1;
    else
        tiled = 0;
    mSplitter->show();
    mSplitter->setProperty("lastState",mSplitter->saveState());
    window = cadwindow->getcurrentwindow();
    if(tiled == 1)
        normalizeView((RCView*)window->gethwnd());
    else
        maximizeView((RCView*)window->gethwnd());
    //cadwindow->selectwindow(0);
    //cadwindow->selectwindow(window);
    tiled = 1;
    list.destroy();
}

void RCCentralWidget::tileVertical()
{
    int i=0;
    initialise(Qt::Vertical);
    purgeSplitters();
    app->sendPostedEvents(Q_NULLPTR,QEvent::DeferredDelete);
    mSplitter->setProperty("lastState",mSplitter->saveState());
    debugView3dList();
    View3dWindowList list = cadwindow->GetView3dList()->copy();
    View3dWindow *window;
    for(list.start(); (window = list.next()) != 0; )
    {
        RCView *view = (RCView*)window->gethwnd();
        addView(view,0);
    }
    if(viewCount() > 1)
        tiled = 1;
    else
        tiled = 0;
    mSplitter->show();
    mSplitter->setProperty("lastState",mSplitter->saveState());
    window = cadwindow->getcurrentwindow();
    if(tiled == 1)
        normalizeView((RCView*)window->gethwnd());
    else
        maximizeView((RCView*)window->gethwnd());
    //cadwindow->selectwindow(0);
    //cadwindow->selectwindow(window);
    list.destroy();
}


void RCCentralWidget::tileHorizontal()
{
    int i=0;
    initialise(Qt::Horizontal);
    purgeSplitters();
    app->sendPostedEvents(Q_NULLPTR,QEvent::DeferredDelete);
    mSplitter->setProperty("lastState",mSplitter->saveState());
    debugView3dList();
    debug();
    View3dWindowList list = cadwindow->GetView3dList()->copy();
    //list->reverse();
    View3dWindow *window;
    for(list.start(); (window = list.next()) != 0; )
    {
        RCView *view = (RCView*)window->gethwnd();
        addView(view,0);
    }
    if(viewCount() > 1)
        tiled = 1;
    else
        tiled = 0;
    mSplitter->show();
    mSplitter->setProperty("lastState",mSplitter->saveState());
    window = cadwindow->getcurrentwindow();
    debugView3dList();
    if(tiled == 1)
        normalizeView((RCView*)window->gethwnd());
    else
        maximizeView((RCView*)window->gethwnd());
    debugView3dList();
    //cadwindow->selectwindow(0);
    //cadwindow->selectwindow(window);
    debugView3dList();
    list.destroy();
}

void RCCentralWidget::setSingleView(RCView *view)
{
    int count = mSplitter->count();
    for(int i=1; i < count; i++)
    {
        mSplitter->widget(i)->hide();
        mSplitter->handle(i)->deleteLater();
    }
    //mSplitter->deleteLater();
    //mSplitter = 0;
    replaceView(0,view);
    tiled =0;
}

void RCCentralWidget::replaceView(int index, RCView *view)
{
    if(mSplitter != 0)
    {
        if(index >= 0 && index < mSplitter->count())
        {
            if(mSplitter->widget(index) != view)
            {

                mSplitter->widget(index)->hide();
                mSplitter->widget(index)->deleteLater();
                for(int i=0; i < mSplitter->count(); i++)
                {
                    mSplitter->insertWidget(index,view);
                }
            }
        }
    }
}

int  RCCentralWidget::addView(RCView *view, int index)
{
    //purgeSplitters();
    debugView3dList();

    if(mSplitter == 0 && mNewSplitter == 0)
    {
        mSplitter = new QSplitter;
        qDebug() << "mSplitter count 1 : " << mSplitter->count();
        if(mSplitter->orientation() == Qt::Horizontal)
            mSplitter->setObjectName("mainSplitter-H");
        else
            mSplitter->setObjectName("mainSplitter-V");
        mSplitter->setMouseTracking(true);
        mSplitter->setHandleWidth(3);
        for(int i=0; i < mLayout->count(); i++)
        {
           QLayoutItem *item = mLayout->itemAt(i);
           if(item->widget() != 0)
           {
               if(item->widget()->objectName() == "InitialView")
               {
                   QWidget *widget = item->widget();
                   mLayout->removeWidget(widget);
                   widget->deleteLater();
                   break;
               }
           }
        }
        QRect rect = QWidget::rect();
        view->resize(rect.size());
        if(index < 0)
            mSplitter->addWidget(view);
        else
            mSplitter->insertWidget(index,view);
        mLayout->addWidget(mSplitter);
        qDebug() << "mSplitter parent : " << mSplitter->parent();
        view->setCentralWidget(this);
         view->show();
        //view->setFocus();
        qDebug() << "mSplitter count 2 : " << mSplitter->count();
    }
    else if(mSplitter != 0 && mNewSplitter == 0)
    {
        qDebug() << "mSplitter count 3 : " << mSplitter->count();
        int i=0;
        for(i=0; i < mSplitter->count(); i++)
        {
            if( !mSplitter->widget(i)->isVisible())
            {
                if((RCView*)mSplitter->widget(i) == view)
                {
                    //view->show();
                    view->setCentralWidget(this);
                    view->show();
                    return mSplitter->count();
                }
            }
        }

        if(index < 0)
            mSplitter->addWidget(view);
        else
            mSplitter->insertWidget(index,view);
        view->setCentralWidget(this);
        view->show();

        // rearrange the widths
        QList<int> sizes;
        int splitsize = rect().width()/mSplitter->count();
        if(mSplitter->orientation() == Qt::Vertical)
            splitsize = rect().height()/mSplitter->count();
        for(i=0; i < mSplitter->count(); i++)
             sizes.append(splitsize);

        mSplitter->setSizes(sizes);
        qDebug() << "mSplitter count 4 : " << mSplitter->count();
    }
    else if(mSplitter != 0 && mNewSplitter != 0)
    {
        qDebug() << "mSplitter count 5 : " << mSplitter->count();
        int i=0;
        for(i=0; i < mNewSplitter->count(); i++)
        {
            if( !mNewSplitter->widget(i)->isVisible())
            {
                if((RCView*)mNewSplitter->widget(i) == view)
                {
                    view->show();
                    view->setCentralWidget(this);
                    view->show();
                    return mNewSplitter->count();
                }
            }
        }

        if(index < 0)
            mNewSplitter->addWidget(view);
        else
            mNewSplitter->insertWidget(index,view);
        view->setCentralWidget(this);
        view->show();

        // rearrange the widths
        QList<int> sizes;
        int splitsize = rect().width()/mNewSplitter->count();
        if(mNewSplitter->orientation() == Qt::Vertical)
            splitsize = rect().height()/mNewSplitter->count();
        for(i=0; i < mNewSplitter->count(); i++)
             sizes.append(splitsize);

        mNewSplitter->setSizes(sizes);
        qDebug() << "mSplitter count 6 : " << mSplitter->count();
    }
    //qDebug() << "RCCentralWidget::addView view width  : " << view->width();
    //qDebug() << "RCCentralWidget::addView view height : " << view->height();
    //qDebug() << "mSplitter count : " << mSplitter->count();

    //if(mSplitter->count() > 1)
    /*
    if(viewCount() > 1 || splitterCount() > 1)
        tiled = 1;
    else
        tiled = 0;
    */
    //qDebug() << "mSplitter count 7 : " << mSplitter->count();
    //debugView3dList();
    int count=0;
    //debugSplitter(mSplitter,count);

    return mSplitter == 0 ? 0 : mSplitter->count();
}

int  RCCentralWidget::addView(QSplitter *splitter, RCView *view, int index)
{
    //purgeSplitters();
    //debugView3dList();

    if(splitter != 0)
    {
        int i=0;
        for(i=0; i < splitter->count(); i++)
        {
            if( !splitter->widget(i)->isVisible())
            {
                if((RCView*)splitter->widget(i) == view)
                {
                    if((index >= 0) && (index != i))
                    {
                        view->show();
                        splitter->insertWidget(index,view);
                        view->setCentralWidget(this);
                        view->regenGLlists();
                        view->show();
                    }
                    else
                    {
                        //view->show();
                        view->setCentralWidget(this);
                        view->regenGLlists();
                        view->show();
                        //view->setFocus();
                    }
                    return splitter->count();
                }
            }
        }

        if(index < 0)
            splitter->addWidget(view);
        else
            splitter->insertWidget(index,view);
        view->setCentralWidget(this);
        view->regenGLlists();
        view->show();
        //view->setFocus();

        // rearrange the widths
        QList<int> sizes;
        int splitsize = rect().width()/splitter->count();
        if(splitter->orientation() == Qt::Vertical)
            splitsize = rect().height()/splitter->count();
        for(int i=0; i < splitter->count(); i++)
             sizes.append(splitsize);

        splitter->setSizes(sizes);
    }
    else
        return 0;
    //qDebug() << "RCCentralWidget::addView view width  : " << view->width();
    //qDebug() << "RCCentralWidget::addView view height : " << view->height();

    //if(mSplitter && mSplitter->count() > 1)
    /*
    if(viewCount() > 1 || splitterCount() > 1)
        tiled = 1;
    else
        tiled = 0;
    */
    //qDebug() << "splitter count 8 : " << splitter->count();
    //debugView3dList();
    return splitter->count();
}

void RCCentralWidget::insertView(int index, RCView *view, Qt::Orientation orientation)
{
    QSplitterHandle *handle = mSplitter->handle(index);
    if(handle && handle->orientation() == orientation)
        mSplitter->insertWidget(index,(QWidget*)view);
    else
    {
        QSplitter *splitter = new QSplitter;
        splitter->setOrientation(orientation);
        if(orientation == Qt::Horizontal)
            mSplitter->setObjectName("mainSplitter-H");
        else
            mSplitter->setObjectName("mainSplitter-V");
        splitter->addWidget((QWidget*)view);
        mSplitter->addWidget(splitter);
    }
    /*
    if(mSplitter->count() > 1)
        tiled = 1;
    else
        tiled =0;
    */
}

void RCCentralWidget::deleteView(int index)
{
    QWidget *view = mSplitter->widget(index);
    int lastCount = mSplitter->count();
    view->hide();
    view->deleteLater();
    mSplitter->handle(index)->deleteLater();
    lastCount--;

    if(lastCount > 1)
        tiled = 1;
    else
        tiled = 0;
}

void RCCentralWidget::deleteAllViews()
{
    if(mSplitter != 0)
    {
        // mSplitter will be deleted so get the count first
        int count = mSplitter->count();
        for(int i=0; i < count; i++)
        {
            if(mSplitter)
                deleteView((RCView*)mSplitter->widget(i));
        }
    }
    else
        tiled = 0;
}

void RCCentralWidget::deleteView(RCView *view)
{

    int dcount=0;
    debugView3dList();
    debugSplitter(mSplitter,dcount);
    // changed to handle multiple splitters   
    QList<QSplitter *> allSplitters;
    if(mSplitter != 0)
        allSplitters = mSplitter->findChildren<QSplitter *>();
    allSplitters.push_front(mSplitter);

    for(int i=0; i<allSplitters.size(); i++)
    {
        QSplitter *thisSplitter = allSplitters.at(i);
        //thisSplitter->setProperty("lastState",thisSplitter->saveState());
        for(int j=0; j < thisSplitter->count(); j++)
        {
            if(thisSplitter->widget(j) == (QWidget*)view)
            {
                //view->hide();
                //view->setParent(0);
                //view->deleteLater();

                if(view && view->getView3dWindow() !=0)
                {
                    //cadwindow->deletesubwindow(view->getView3dWindow());
                    delete view->getView3dWindow();
                    view->setView3dWindow(0);
                }

                ((RCView*)thisSplitter->widget(j))->hide();
                ((RCView*)thisSplitter->widget(j))->deleteLater();
                ((RCView*)thisSplitter->widget(j))->setParent(0);
                app->sendPostedEvents(Q_NULLPTR,QEvent::DeferredDelete);
                break;
            }
        }
    }

    qDebug() << "View count after delete : " << viewCount();

    dcount=0;
    debugView3dList();
    debugSplitter(mSplitter,dcount);

    purgeSplitters();

    debugView3dList();
    mSplitter->show();
    for(int i=0; i < mSplitter->count(); i++)
    {
        mSplitter->widget(i)->show();
    }

    dcount=0;
    debugView3dList();
    debug();

    if(viewCount() > 1)
        tiled = 1;
    else
        tiled = 0;
}

void RCCentralWidget::hideView(int index)
{
    mSplitter->widget(index)->hide();
}

void RCCentralWidget::showView(int index)
{
    mSplitter->widget(index)->show();
}

void RCCentralWidget::hideView(RCView *view)
{
    for(int i=0; i < mSplitter->count(); i++)
        if(mSplitter->widget(i) == (QWidget*)view)
           mSplitter->widget(i)->hide();
}

void RCCentralWidget::showView(RCView *view)
{
    for(int i=0; i < mSplitter->count(); i++)
        if(mSplitter->widget(i) == (QWidget*)view)
            mSplitter->widget(i)->show();
}

bool RCCentralWidget::isMaximized(RCView *view)
{
    if(view->isHidden())
        return false;
    if(!tiled)
        return true;

    return false;
}

int RCCentralWidget::isTiled()
{
    return visibleSplitterCount() > 1 ? 1 : 0;
}

void RCCentralWidget::maximizeView(RCView *view)
{
#if 1
    if(mSplitter == 0)
        return;
    //purgeSplitters();
    //mSplitter->setProperty("lastState",mSplitter->saveState());
    QList<RCView *> allViews = mSplitter->findChildren<RCView *>();
    QList<QSplitter *> allSplitters = mSplitter->findChildren<QSplitter *>();
    allSplitters.push_front(mSplitter);

    for(int i=0; i<allSplitters.size(); i++)
        allSplitters.at(i)->setProperty("lastState",allSplitters.at(i)->saveState());
    // hide all views if not the given view
    for(int i=0; i<allViews.size(); i++)
    {
        RCView *thisView = allViews.at(i);
        if(thisView == view)
        {
            thisView->show();
            thisView->setFocus();
        }
        else
        {
            thisView->hide();
            thisView->clearFocus();
        }
    }
    // hide all splitters (except the root splitter) which do not own the given view
    for(int i=0; i<allSplitters.size(); i++)
    {
        QSplitter *thisSplitter = allSplitters.at(i);
        if(thisSplitter == mSplitter)
            continue;
        for(int j=0; j < thisSplitter->count(); j++)
        {
            //if(thisSplitter == mSplitter)
            //    continue;
            //if(qobject_cast<QSplitter*>(thisSplitter->widget(j)) != 0)
            //{
                //QList<RCView *> views = thisSplitter->findChildren<RCView *>(QString(),Qt::FindDirectChildrenOnly);
                QList<RCView *> views = thisSplitter->findChildren<RCView *>();
                if(!views.contains(view))
                {
                        thisSplitter->hide();
                }
                else
                {
                    int count=0;
                    for(int k=0; k<views.size(); k++)
                    {
                        if(view->isVisible())
                            count++;
                    }
                    if(count < 1)
                    {
                            thisSplitter->hide();
                    }
                }
            //}
        }
    }
    view->setFocus();
    tiled = 0;
#else
    for(int i=0; i < mSplitter->count(); i++)
    {
        if(mSplitter->widget(i) != (QWidget*)view)
            ((RCView*)mSplitter->widget(i))->hide();
    }
    view->setFocus();
    tiled = 0;
#endif
}

// this is a recursive function
void RCCentralWidget::showSplitters(QSplitter *Splitter)
{
    Splitter->show();
    for(int i=0; i < Splitter->count(); i++)
    {
        if(qobject_cast<RCView *>(Splitter->widget(i)) != 0)
            ((RCView*)Splitter->widget(i))->show();
    }
    for(int i=0; i < Splitter->count(); i++)
    {
        if(qobject_cast<QSplitter *>(Splitter->widget(i)) != 0)
            showSplitters((QSplitter*)Splitter->widget(i));
    }
    Splitter->restoreState(Splitter->property("lastState").toByteArray());
}

void RCCentralWidget::normalizeView(RCView *view)
{
#if 1
    //purgeSplitters();
    if(mSplitter != 0)
    {
        showSplitters(mSplitter);
        //view->setFocus();
        //if(mSplitter->count() > 1 )
        if(viewCount() > 1 )
            tiled = 1;
        else
            tiled = 0;
    }
#else
    QVector<QWidget*> wlist;
    int i;


    for(i=0; i < mSplitter->count(); i++)
        wlist.append(mSplitter->widget(i));

    for(i=0; i < mSplitter->count(); i++)
    {
        mSplitter->widget(i)->hide();
        mSplitter->widget(i)->setParent(0);
    }

    for(i=0; i < wlist.size(); i++)
    {
        mSplitter->insertWidget(i,wlist.at(i));
        mSplitter->widget(i)->show();
        if((QWidget*)view == mSplitter->widget(i))
            ((RCView*)mSplitter->widget(i))->setFocus();
        else
            ((RCView*)mSplitter->widget(i))->clearFocus();
    }

    // rearrange the widths
    QList<int> sizes;
    int splitsize = rect().width()/mSplitter->count();
    for(i=0; i < mSplitter->count(); i++)
         sizes.append(splitsize);

    mSplitter->setSizes(sizes);


    if(mSplitter->count() >1 )
        tiled = 1;
    else
        tiled = 0;
#endif
}

void RCCentralWidget::maximizeView(int index)
{
    RCView *view = 0;
    for(int i=0; i < mSplitter->count(); i++)
        if( i != index)
           mSplitter->widget(i)->hide();

    view = (RCView*)mSplitter->widget(index);
    view->setFocus();
    tiled = 0;
}

void RCCentralWidget::normalizeView(int index)
{
    QVector<QWidget*> wlist;
    int i;
    RCView *view = (RCView*)mSplitter->widget(index);

    for(i=0; i < mSplitter->count(); i++)
        wlist.append(mSplitter->widget(i));

    for(i=0; i < mSplitter->count(); i++)
    {
        mSplitter->widget(i)->hide();
        mSplitter->widget(i)->setParent(0);
    }

    for(i=0; i < wlist.size(); i++)
    {
        mSplitter->insertWidget(i,wlist.at(i));
        mSplitter->widget(i)->show();
    }

    // rearrange the widths
    QList<int> sizes;
    int splitsize = rect().width()/mSplitter->count();
    for(i=0; i < mSplitter->count(); i++)
         sizes.append(splitsize);

    mSplitter->setSizes(sizes);

    view->setFocus();

    if(mSplitter->count() >1 )
        tiled = 1;
    else
        tiled = 0;
}

RCView* RCCentralWidget::activeWindow() const
{
    if(mSplitter == 0)
        return 0;
    for(int i=0; i < mSplitter->count(); i++)
    {
        if(mSplitter->widget(i)->hasFocus() ||
           mSplitter->widget(i) == ((QtMainWindow*)parent())->activeView())
            return (RCView*)mSplitter->widget(i);
    }
    return 0;
    /*
    if(mSplitter == 0)
        return 0;
    for(int i=0; i < mSplitter->count(); i++)
    {
        if(mSplitter->widget(i)->hasFocus())
            return (RCView*)mSplitter->widget(i);
    }
    return 0;
    */
}

void RCCentralWidget::setActiveWindow(RCView *view)
{
#if 1
    // changed to handle multiple splitters
    if(mSplitter == 0)
        return;
    QList<QSplitter *> allSplitters = mSplitter->findChildren<QSplitter *>();
    allSplitters.push_front(mSplitter);

    for(int i=0; i<allSplitters.size(); i++)
    {
        QSplitter *thisSplitter = allSplitters.at(i);
        for(int j=0; j < thisSplitter->count(); j++)
        {
            if(thisSplitter->widget(j) == view)
            {
                if(!tiled)
                    maximizeView(view);
                else
                    ((RCView*)thisSplitter->widget(j))->setFocus();
                view->regenGLlists();
                thisSplitter->widget(j)->show();
            }
            else
                ((RCView*)thisSplitter->widget(j))->clearFocus();
        }
    }
#else
    if(mSplitter == 0)
        return;
    for(int i=0; i < mSplitter->count(); i++)
    {
        if(mSplitter->widget(i) == view)
        {
            if(!tiled)
                maximizeView(view);
            else
                ((RCView*)mSplitter->widget(i))->setFocus();
            mSplitter->widget(i)->show();
        }
        else
            ((RCView*)mSplitter->widget(i))->clearFocus();
    }
#endif
    // debug
    /*
    QList<int> sizes = mSplitter->sizes();
    for(int i=0; i < sizes.count(); i++)
    {
        qDebug() << "sizes [" << i << "] = " << sizes.at(i);
        if(mSplitter->widget(i)->isHidden())
            qDebug() << "is hidden";
        else
            qDebug() << "is visible";
    }
    */
}

void RCCentralWidget::unsetActiveWindow(RCView *view)
{
#if 1
    if(mSplitter == 0)
        return;
    QList<QSplitter *> allSplitters = mSplitter->findChildren<QSplitter *>();
    allSplitters.push_front(mSplitter);

    for(int i=0; i<allSplitters.size(); i++)
    {
        QSplitter *thisSplitter = allSplitters.at(i);
        for(int j=0; j < thisSplitter->count(); j++)
        {
            if(thisSplitter->widget(j) == view)
                ((RCView*)thisSplitter->widget(j))->clearFocus();
        }
    }
#else
    if(mSplitter == 0)
        return;
    for(int i=0; i < mSplitter->count(); i++)
    {
        if(mSplitter->widget(i) == view)
            ((RCView*)mSplitter->widget(i))->clearFocus();
    }
#endif
}

void RCCentralWidget::debugSplitter(QSplitter *splitter, int &count)
{
    qDebug() << "== splitter debug start == " << count;
    qDebug() << "splitter : " << splitter;
    if(count ==0)
    {
        qDebug() << "total splitters : " << splitterCount();
        qDebug() << "total views     : " << viewCount();
    }
    qDebug() << "splitter (" <<  count << ") count = " << splitter->count();
    if(splitter->orientation() == Qt::Horizontal)
        qDebug() << "splitter is Horizontal";
    else
        qDebug() << "splitter is Vertical";
    for(int i=0; i<splitter->count(); i++)
    {
        if(qobject_cast<QSplitter*>(splitter->widget(i)) != 0)
            qDebug() << "widget (" << i << ") is a splitter";
        else if(qobject_cast<RCView*>(splitter->widget(i)) != 0)
        {
            qDebug() << "widget (" << i << ") is a view";
            qDebug() << "RCView : " << splitter->widget(i);
            if(((RCView*)splitter->widget(i))->getView3dWindow() != 0 )
                if(((RCView*)splitter->widget(i))->getView3dWindow()->getview() != 0 )
                {
                   //qDebug() << "view name - " << QString(((RCView*)splitter->widget(i))->getView3dWindow()->getview()->getname());
                   qDebug() << "view3d ptr  - " << ((RCView*)splitter->widget(i))->getView3dWindow();
                }
        }
        if(qobject_cast<QSplitter*>(splitter->widget(i)) != 0)
        {
            count++;
            debugSplitter((QSplitter*)splitter->widget(i),count);
        }
    }
    qDebug() << "== splitter debug end   ==" << count;
}

void RCCentralWidget::moveSplitterView(QSplitter* splitter, RCView* moveView, int toIndex, int fromIndex)
{
    if(splitter != 0)
    {
        //splitter->widget(fromIndex)->setParent(0);
        splitter->insertWidget(toIndex,moveView);
    }
}

#if 1
void RCCentralWidget::splitView(RCView *inView,Qt::Orientation orientation)
{
    View3dWindowList *list = cadwindow->GetView3dList();
    View3dWindow *window=0, *newWindow=0;
    RCView *view=0;
    //int count = list->length();
    list->start();
    window = list->next();
    while (window != 0)
    {
        view = (RCView*)window->gethwnd();
        if(view == inView)
            break;
        window = list->next();
    }

    // View2dSurf not working yet only using View3dSurf type
    if(window == 0 || window->getsurfacetype() != View3dSurf || window->getsurfacetype() == View2dSurf)
        return;

    purgeSplitters();

    if(mSplitter == 0)
    {
        int param=1; // don't show the new window
        if(window->getsurfacetype() == View3dSurf)
        {
            newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                     0,0,view->width(),view->height(),25,(RCPVOID)&param);
            newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
            newWindow->setplotscale(window->getplotscale());
            newWindow->setscale(window->getscale());
        }
        else if(window->getsurfacetype() == View2dSurf)
        {
            cadwindow->create2dsubwindow(((View2dWindow*)window)->getview2d());
            newWindow = cadwindow->getcurrentwindow();
        }


        ((RCView*)newWindow->gethwnd())->setView3dWindow(newWindow);
        addView((RCView*)newWindow->gethwnd());
    }
    else
    {
        // get a list of splitter children in the main splitter
        QList<QSplitter *> allSplitters = mSplitter->findChildren<QSplitter *>();
        // add the main splitter to the front of the list
        allSplitters.push_front(mSplitter);

        // find the splitter which owns the view to be split
        for(int i=0; i<allSplitters.size(); i++)
        {
            QSplitter *thisSplitter = allSplitters.at(i);
            qDebug() << "thisSplitter count : " << thisSplitter->count();
            for(int j=0; j < thisSplitter->count(); j++)
            {
                if(thisSplitter->widget(j) == view)
                {
                    // if the orientation is different from the current splitter
                    // create a new splitter and add it to the parent splitter
                    // otherwise just add a new view to the splitter
                    if(thisSplitter->orientation() != orientation)
                    {
                            qDebug() << "thisSplitter count : " << thisSplitter->count();
                            qDebug() << "mSplitter count : " << mSplitter->count();
                            if(thisSplitter->count() <= 1)
                            {
                                thisSplitter->setOrientation(orientation);
                                mNewSplitter = thisSplitter;

                                int param=1; // don't show the new window
                                /*
                                newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                                             0,0,view->width(),view->height(),25,(RCPVOID)&param);
                                newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                                newWindow->setplotscale(window->getplotscale());
                                newWindow->setscale(window->getscale());
                                */

                                if(window->getsurfacetype() == View3dSurf)
                                {
                                    newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                                             0,0,view->width(),view->height(),25,(RCPVOID)&param);
                                    newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                                    newWindow->setplotscale(window->getplotscale());
                                    newWindow->setscale(window->getscale());
                                }
                                else if(window->getsurfacetype() == View2dSurf)
                                {
                                    cadwindow->create2dsubwindow(((View2dWindow*)window)->getview2d());
                                    newWindow = cadwindow->getcurrentwindow();
                                }

                                if(newWindow != 0)
                                {
                                    ((RCView*)newWindow->gethwnd())->setView3dWindow(newWindow);
                                    addView(thisSplitter,(RCView*)newWindow->gethwnd());
                                }
                                //thisSplitter->addWidget(newSplitter);
                                mNewSplitter=0;
                                break;
                            }
                            else
                            {
                                QSplitter *newSplitter = new QSplitter(orientation);
                                if(newSplitter != 0)
                                {
                                    newSplitter->setObjectName("newSplitter");
                                    // put this view into the new splitter
                                    //thisSplitter->widget(j)->hide();
                                    //thisSplitter->widget(j)->setParent(0);
                                    qDebug() << "moved viewparent" << thisSplitter->widget(j)->parent();
                                    qDebug() << "moved viewparent" << thisSplitter->widget(j);
                                    qDebug() << "newSplitter->count()" << newSplitter->count();
                                    // put the new splitter where the old view was
                                    newSplitter->addWidget(view);
                                    //newSplitter->insertWidget(0,view);
                                    qDebug() << "thisSplitter->count()" << thisSplitter->count();
                                    qDebug() << "moved viewparent" << thisSplitter->widget(j);

                                    // add the new view to the new splitter
                                    mNewSplitter = newSplitter;
                                    int param=1; // don't show the new window
                                    /*
                                    qDebug() << "mSplitter count : " << mSplitter->count();
                                    newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                                                 0,0,view->width(),view->height(),25,(RCPVOID)&param);
                                    newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                                    newWindow->setplotscale(window->getplotscale());
                                    newWindow->setscale(window->getscale());
                                    */
                                    if(window->getsurfacetype() == View3dSurf)
                                    {
                                        newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                                                 0,0,view->width(),view->height(),25,(RCPVOID)&param);
                                        newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                                        newWindow->setplotscale(window->getplotscale());
                                        newWindow->setscale(window->getscale());
                                    }
                                    else if(window->getsurfacetype() == View2dSurf)
                                    {
                                        cadwindow->create2dsubwindow(((View2dWindow*)window)->getview2d());
                                        newWindow = cadwindow->getcurrentwindow();
                                    }

                                    qDebug() << "newSplitter->count()" << newSplitter->count();
                                    //mNewSplitter=0;
                                    qDebug() << "mSplitter count : " << mSplitter->count();

                                    if(newWindow != 0)
                                    {
                                        ((RCView*)newWindow->gethwnd())->setView3dWindow(newWindow);
                                        addView(newSplitter,(RCView*)newWindow->gethwnd());
                                    }
                                    qDebug() << "mSplitter count : " << mSplitter->count();

                                    thisSplitter->insertWidget(j,newSplitter);
                                    //thisSplitter->addWidget(newSplitter);
                                    qDebug() << "newSplitter->count()" << newSplitter->count();
                                    qDebug() << "thisSplitter->count()" << thisSplitter->count();
                                    mNewSplitter=0;
                                    int dcount=0;
                                    debugSplitter(mSplitter,dcount);
                                    break;
                                }
                                else
                                    return;
                            }
                    }
                    else
                    {
                        mNewSplitter = thisSplitter;
                        qDebug() << "mSplitter count : " << mSplitter->count();

                        int param=1; // don't show the new window
                        /*
                        newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                                     0,0,view->width(),view->height(),25,(RCPVOID)&param);
                        newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                        newWindow->setplotscale(window->getplotscale());
                        newWindow->setscale(window->getscale());
                        */
                        if(window->getsurfacetype() == View3dSurf)
                        {
                            newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                                     0,0,view->width(),view->height(),25,(RCPVOID)&param);
                            newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
                            newWindow->setplotscale(window->getplotscale());
                            newWindow->setscale(window->getscale());

                        }
                        else if(window->getsurfacetype() == View2dSurf)
                        {
                            cadwindow->create2dsubwindow(((View2dWindow*)window)->getview2d());
                            newWindow = cadwindow->getcurrentwindow();
                        }

                        qDebug() << "mSplitter count : " << mSplitter->count();

                        if(newWindow != 0)
                        {
                            ((RCView*)newWindow->gethwnd())->setView3dWindow(newWindow);
                            addView(thisSplitter,(RCView*)newWindow->gethwnd(),j+1);
                        }
                        qDebug() << "mSplitter count : " << mSplitter->count();
                        //if(thisSplitter->count()-1 > j+1)
                        //    moveSplitterView(thisSplitter,(RCView*)newWindow->gethwnd(),j+1,thisSplitter->count()-1);

                        mNewSplitter=0;
                        break;
                    }
                }
            }
        }
    }

    purgeSplitters();

    // debug
    int dcount=0;
    debugSplitter(mSplitter,dcount);

    tiled = 1;

    debugView3dList();
    if (list->add(newWindow))
    {
        list->start();
        while((window = list->next()) != 0)
        {
            ((RCView*)window->gethwnd())->setFocus();
            //window->UpdateWindow();
        }
        cadwindow->selectwindow(newWindow);
    }
    debugView3dList();
}
#else
void RCCentralWidget::splitView(RCView *inView,Qt::Orientation orientation)
{
    View3dWindowList *list = cadwindow->GetView3dList();
    View3dWindow *window, *newWindow;
    RCView *view;
    int count = list->length();
    list->start();
    window = list->next();
    while (window != 0)
    {
        view = (RCView*)window->gethwnd();
        if(view == inView)
            break;
        window = list->next();
    }

    int param=1; // don't show the new window
    //int param=0; // don't show the new window
    newWindow = new View3dWindow(cadwindow,cadwindow->subwindow,window->getview(),
                                 0,0,view->width(),view->height(),25,(RCPVOID)&param);

    newWindow->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
    newWindow->setplotscale(window->getplotscale());
    newWindow->setscale(window->getscale());

    for(int i=0; i < mSplitter->count(); i++)
    {
        if(mSplitter->widget(i) == view)
        {
            if(mSplitter->orientation() != orientation)
            {
                if(mSplitter->count() == 1)
                {
                    //not working yet
                    return;
                    //mSplitter->widget(i)->hide();
                    //mSplitter->setOrientation(orientation);
                    //mSplitter->widget(i)->show();
                }
            }
            else
            {
                ((RCView*)newWindow->gethwnd())->setView3dWindow(newWindow);
                addView((RCView*)newWindow->gethwnd());
            }
        }
    }
    if (list->add(newWindow))
        cadwindow->selectwindow(window);

    tiled = 1;
}
#endif

void RCCentralWidget::regenGLlists()
{
    if(mSplitter == 0)
        return;
    QList<QSplitter *> allSplitters = mSplitter->findChildren<QSplitter *>();
    allSplitters.push_front(mSplitter);

    for(int i=0; i<allSplitters.size(); i++)
    {
        QSplitter *thisSplitter = allSplitters.at(i);
        for(int j=0; j < thisSplitter->count(); j++)
        {
            if(qobject_cast<RCView*>(thisSplitter->widget(j)) != 0)
                ((RCView*)thisSplitter->widget(j))->regenGLlists();
        }
    }
}
