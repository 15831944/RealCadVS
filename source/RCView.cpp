
#include "ncwin.h"

#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include <QtOpenGL>
#include <QToolBar>
#include <QGraphicsView>
#include <qmath.h>

#include "RCCentralWidget.h"
#include "RCGraphicsScene.h"
#include "RCGraphicsView.h"
#include "RCView.h"

//#include "qwindowdefs.h"
//#include  <QtGui/qwindowdefs_win.h>
/*
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier)
    {
        if (e->delta() > 0)
            view->zoomIn(6);
        else
            view->zoomOut(6);
        e->accept();
     }
    else if (e->modifiers() & Qt::AltModifier)
    {
        if (e->delta() > 0)
            view->rotateLeft();
        else
            view->rotateRight();
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
*/

RCViewLabel::RCViewLabel(QWidget *parent)
    : QLabel(parent)
{
    //setMouseTracking(true);
    //setAttribute(Qt::WA_Hover);
    connect(this,SIGNAL(clicked()),this,SLOT(onClicked()));
}

RCViewLabel::RCViewLabel(const QString & text, QWidget * parent, Qt::WindowFlags f)
    : QLabel(text,parent,f)
{

}

void RCViewLabel::setPlainText(const QString text)
{
    //if(text.length() == 0)
    //    qDebug() << "view label text is empty";

    mPlainText = text;
    setText(text);
}

QString RCViewLabel::plainText()
{
    return mPlainText;
}

bool RCViewLabel::event(QEvent *e)
{
    //qDebug() << "RCViewLabel::event : " << e->type();
    if(e->type() == QEvent::Enter)
        mouseEnterEvent((QMouseEvent*)e);
    else if(e->type() == QEvent::Leave)
        mouseLeaveEvent((QMouseEvent*)e);

    return QLabel::event(e);
}

void RCViewLabel::mousePressEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    emit clicked();
}

void RCViewLabel::mouseEnterEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    //qDebug() << "RCViewLabel::mouseEnterEvent";
    QString text = "<b>" + mPlainText + "</b>";
    setText(text);
}

void RCViewLabel::mouseLeaveEvent(QMouseEvent * event)
{
    Q_UNUSED(event);
    //qDebug() << "RCViewLabel::mouseLeaveEvent";
    setText(mPlainText);

    program->drawGraphicsCursor();
    /*
    if(cadwindow->gethwnd() != 0)
    {
        POINT p;
        GetCursorPos(&p);
        QPoint pos = ((QtMainWindow*)cadwindow->gethwnd())->mapFromGlobal(QPoint(p.x,p.y));
        if(((QtMainWindow*)cadwindow->gethwnd())->childAt(pos) != 0)
        {
            //qDebug() << "child at pos : " << ((QtMainWindow*)cadwindow->gethwnd())->childAt(pos);
            if(((QtMainWindow*)cadwindow->gethwnd())->childAt(pos)->objectName() == "QGLWidget")
            {
                qDebug() << "parent at pos : " << ((QtMainWindow*)cadwindow->gethwnd())->childAt(pos)->parentWidget();
                if(qobject_cast<RCGraphicsView*>(((QtMainWindow*)cadwindow->gethwnd())->childAt(pos)->parentWidget()) != 0)
                {
                    qDebug() << "parent : " << ((RCGraphicsView*)((QtMainWindow*)cadwindow->gethwnd())->childAt(pos)->parentWidget())->parentWidget();
                    RCView *rcview = (RCView*)((RCGraphicsView*)((QtMainWindow*)cadwindow->gethwnd())->childAt(pos)->parentWidget())->parentWidget();
                    if(rcview != 0)
                        rcview->getView3dWindow()->drawCursor();
                }
            }
        }
    }
    */
}

void RCViewLabel::onClicked()
{
    //qDebug() << "RCViewLabel::onClicked";
}

static int rcviewcount=0;
RCView::RCView(QWidget *parent)
 : QFrame(parent)
{
    setAcceptDrops(false);

    mView3dWindow = 0;
    mView3dOutput = 0;
    mGraphicsView = 0;
    mCentralWidget = 0;
    mGraphicsScene = 0;
    mLabel = 0;
    mLayout = 0;
    mLabelLayout = 0;

    mLayout = new QVBoxLayout(this);
    mLayout->setObjectName("ViewMainlayout");
    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);
    mLayout->addLayout(mLabelLayout);

    mLayout->addWidget(mGraphicsView);
    setLayout(mLayout);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    scale = 0.1;
    angle = 0.0;
}

RCView::RCView(const QString &name, RCGraphicsView *gview, QWidget *parent) :
    QFrame(parent)
{
    mcount = ++rcviewcount;
    qDebug() << "int RCVIEW CTOR : " << mcount;
    //setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    // enable drag and drop
    setAcceptDrops(false);

    mView3dWindow = 0;
    mView3dOutput = 0;
    mGraphicsView = gview;
    mCentralWidget = 0;
    mGraphicsScene = 0;
    mLabel = 0;
    mLayout = 0;
    mLabelLayout = 0;

    setObjectName(name);

    //setFrameStyle(Sunken | StyledPanel);
    setFrameStyle(Sunken | Box);
    setLineWidth(1);
    if(mGraphicsView == 0)
    {
        mGraphicsView = new RCGraphicsView(this);
        mGraphicsView->setObjectName("RCGraphicsView");
    }
    if(mGraphicsView != 0)
    {
        if(mGraphicsView->objectName() != "RCGraphicsView")
            mGraphicsView->setObjectName("RCGraphicsView");
        if(mGraphicsView->parent() != this)
            mGraphicsView->setParent(this);
        if(mGraphicsView->scene() == 0)
        {
            RCGraphicsScene *graphicsScene = new RCGraphicsScene(mGraphicsView);
            graphicsScene->setObjectName("made by RCView");
            mGraphicsView->setScene(graphicsScene);
            mGraphicsScene = graphicsScene;
        }
        mGraphicsView->scene()->setBspTreeDepth(16);
        QChar text[300];
        GetPrivateProfileString("Antialiasing","On","Y",text,300,home.getinifilename());
        if(RCSTRING(text) == "Y")
            mGraphicsView->setRenderHint(QPainter::Antialiasing, true);
        else
            mGraphicsView->setRenderHint(QPainter::Antialiasing, false);

        mGraphicsView->setRenderHint(QPainter::TextAntialiasing, true);

        //mGraphicsView->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
        //mGraphicsView->setDragMode(QGraphicsView::ScrollHandDrag);// << use this for Qt viewing
        mGraphicsView->setDragMode(QGraphicsView::NoDrag);// <<  use this for RC viewing
        mGraphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);// << use this for Qt viewing
        //mGraphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);// << use this for Qt viewing
        mGraphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);// << use this for RC viewing
        //mGraphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);// << use this for Qt viewing
        mGraphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);// << use this for RC viewing
        mGraphicsView->setResizeAnchor(QGraphicsView::NoAnchor);// << use this for RC viewing
        mGraphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);// << use this for RC viewing
        mGraphicsView->setRenderHint(QPainter::SmoothPixmapTransform, false);// <<  use this for RC viewing
        mGraphicsView->setRenderHint(QPainter::Qt4CompatiblePainting, false);// <<  use this for RC viewing
        mGraphicsView->setRenderHint(QPainter::NonCosmeticDefaultPen, false);// <<  use this for RC viewing
        mGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// <<  use this for RC viewing
        mGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// <<  use this for RC viewing
        //mGraphicsView->setCacheMode(QGraphicsView::CacheBackground);// <<  use this for RC viewing
        mGraphicsView->setCacheMode(QGraphicsView::CacheNone);// <<  use this for RC viewing
        //mGraphicsView->setMouseTracking(false);// <<  use this for RC viewing
        mGraphicsView->setMouseTracking(true);// <<  use this for RC viewing
    }

    mLabelLayout = new QHBoxLayout();
    mLabelLayout->setObjectName("ViewLabelLayout");
    mLabelLayout->setMargin(0);
    mLabelLayout->setSpacing(0);
    mLabelLayout->setContentsMargins(4,1,4,1);

    //mLabel = new QLabel(name);
    //mLabel->setFixedHeight(20);
    //mLabelLayout->addWidget(mLabel);

    mViewNameLabel = new RCViewLabel("View - ",this);
    mViewNameLabel->setObjectName("ViewNameLabel");
    mViewNameLabel->setFixedHeight(20);
    mViewNameLabel->setTextFormat(Qt::RichText);
    mLabelLayout->addWidget(mViewNameLabel);
    connect(mViewNameLabel, SIGNAL(clicked()), this, SLOT(NameLabelClicked()));

    mZoomScaleLabel = new RCViewLabel("Zoom ",this);
    mZoomScaleLabel->setObjectName("ZoomScaleLabel");
    mZoomScaleLabel->setFixedHeight(20);
    mLabelLayout->addWidget(mZoomScaleLabel);
    connect(mZoomScaleLabel, SIGNAL(clicked()), this, SLOT(ZoomScaleClicked()));

    mPrintScaleLabel = new RCViewLabel(" Print Scale ",this);
    mPrintScaleLabel->setObjectName("PrintScaleLabel");
    mPrintScaleLabel->setFixedHeight(20);
    mPrintScaleLabel->setTextFormat(Qt::RichText);
    mLabelLayout->addWidget(mPrintScaleLabel);
    connect(mPrintScaleLabel, SIGNAL(clicked()), this, SLOT(PrintScaleClicked()));

    QSize iconSize(16, 16);
    mSplitButton = new QToolButton(this);
    mSplitButton->setObjectName("TileViewButton");
    mSplitButton->setStyleSheet("border: none");
    mSplitButton->setAutoRepeat(false);
    mSplitButton->setIcon(QPixmap(":/images/tileviews-16.png"));
    mSplitButton->setIconSize(iconSize);
    mSplitButton->setFixedSize(20,20);
    mLabelLayout->addStretch();
    mLabelLayout->addWidget(mSplitButton);

    mMaximiseButton = new QToolButton(this);
    mMaximiseButton->setObjectName("MaximizeViewButton");
    mMaximiseButton->setStyleSheet("border: none");
    mMaximiseButton->setAutoRepeat(false);
    mMaximiseButton->setIcon(QPixmap(":/images/maximize-16.png"));
    mMaximiseButton->setIconSize(iconSize);
    mMaximiseButton->setFixedSize(20,20);
    //LabelLayout->addStretch();
    mLabelLayout->addWidget(mMaximiseButton);

    mCloseButton = new QToolButton(this);
    mCloseButton->setObjectName("CloseViewButton");
    mCloseButton->setStyleSheet("border: none");
    mCloseButton->setAutoRepeat(false);
    mCloseButton->setIcon(QPixmap(":/images/closeview-16.png"));
    mCloseButton->setIconSize(iconSize);
    mCloseButton->setFixedSize(20,20);
    mLabelLayout->addWidget(mCloseButton);

    connect(mSplitButton, SIGNAL(clicked()), this, SLOT(splitView()));
    connect(mCloseButton, SIGNAL(clicked()), this, SLOT(closeView()));
    connect(mMaximiseButton, SIGNAL(clicked()), this, SLOT(maximize()));

    mLayout = new QVBoxLayout(this);
    mLayout->setObjectName("ViewMainlayout");
    mLayout->setMargin(0);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);
    mLayout->addLayout(mLabelLayout);

    mLayout->addWidget(mGraphicsView);
    setLayout(mLayout);

    mSplitView_Menu = new QMenu(this);
    mSplitView_Menu->setObjectName("SplitView_Menu");

    // KMJ: the naming is the opposite of the actual action
    QAction *Act_SplitHorizontal;
    Act_SplitHorizontal = new QAction(this);
    Act_SplitHorizontal->setObjectName(QStringLiteral("ACT_SplitHorizontal"));
    Act_SplitHorizontal->setText(QApplication::translate("RCView", "Split Vertical", 0));
    mSplitView_Menu->addAction(Act_SplitHorizontal);
    connect(Act_SplitHorizontal,SIGNAL(triggered()),this,SLOT(splitHorizontal()));

    QAction *Act_SplitVertical;
    Act_SplitVertical = new QAction(this);
    Act_SplitVertical->setObjectName(QStringLiteral("ACT_SplitVertical"));
    Act_SplitVertical->setText(QApplication::translate("RCView", "Split Horizontal", 0));
    mSplitView_Menu->addAction(Act_SplitVertical);
    connect(Act_SplitVertical,SIGNAL(triggered()),this,SLOT(splitVertical()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    scale = 0.1;
    angle = 0.0;

    setStyleSheet("QFrame#RCView {background-color: LightGray;}");

    //setupMatrix();
}

RCView::~RCView()
{
    qDebug() << "in RCView DTOR : " << mcount;
    if(mGraphicsScene)
    {
        qDebug() << "scene count : " << mGraphicsScene->mcount;
        //app->sendPostedEvents(mGraphicsScene,QEvent::DeferredDelete);
    }
    mGraphicsScene = 0;
    if(mGraphicsView)
    {
        qDebug() << "graphicsview count : " << mGraphicsView->mcount;
        //app->sendPostedEvents(mGraphicsView,QEvent::DeferredDelete);
        //delete mGraphicsView;
    }
    mGraphicsView = 0;
    if(mView3dWindow)
        qDebug() << "mView3dWindow count : " << mView3dWindow->mcount;
    if(mView3dWindow != 0)
        delete mView3dWindow;
    mView3dWindow = 0;
    mLayout = 0;
    mLabelLayout = 0;
}

void RCView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
    emit changed(event->mimeData());

}

void RCView::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    qDebug() << "in RCView::dropEvent";

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
}

QGraphicsView *RCView::view() const
{
    //return static_cast<QGraphicsView *>(mGraphicsView);
    return (QGraphicsView*)mGraphicsView;
}

QWidget* RCView::viewport() const
{
    //qDebug() << "graphics view : " << mGraphicsView;
    //qDebug() << "viewport : " << mGraphicsView->viewport();
    return mGraphicsView->viewport();
}

void RCView::setupMatrix()
{
    qreal s = qPow(qreal(2), scale);

    QMatrix matrix;
    matrix.scale(s, s);
    matrix.rotate(angle);

    mGraphicsView->setMatrix(matrix);
}

void RCView::print()
{
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        mGraphicsView->render(&painter);
    }
}

void RCView::zoomIn()
{
    scale += 0.1;
    setupMatrix();
}

void RCView::zoomOut()
{
    scale -= 0.1;
    setupMatrix();
}

void RCView::rotateLeft()
{
    angle -= 10.0;
    setupMatrix();
}

void RCView::rotateRight()
{
    angle += 10.0;
    setupMatrix();
}

void RCView::setViewport(QWidget *view)
{
    if(mGraphicsView != 0)
        mGraphicsView->setViewport(view);
}

void RCView::setView(QGraphicsView *view)
{
    if(view != 0)
    {
        if(mGraphicsView !=0)
        {
            //view->setViewport(mGraphicsView->viewport());
            // view->setScene(mGraphicsView->scene());
            mLayout->replaceWidget(mGraphicsView,(RCGraphicsView*)view);
            mGraphicsView->setParent(0);
            mGraphicsView->deleteLater();
        }
        view->setParent(this);
        mGraphicsView = (RCGraphicsView*)view;
        mGraphicsScene = (RCGraphicsScene*)view->scene();
        if(mView3dWindow != 0)
            mView3dWindow->setScene(mGraphicsScene);
    }
}

void RCView::setScene(QGraphicsScene *scene)
{
    if(scene)
    {
        scene->setSceneRect(0,0,50000,50000);
        if(mGraphicsView != 0)
        {
            if(mGraphicsView->scene() != 0 && mGraphicsView->scene() != program->m_scene)
                mGraphicsView->scene()->deleteLater();
            mGraphicsView->setScene(scene);
            mGraphicsScene = (RCGraphicsScene*)scene;
            if(mView3dWindow != 0)
                mView3dWindow->setScene(scene);
            //mGraphicsView->setSceneRect(0,0,mGraphicsView->viewport()->width(),mGraphicsView->viewport()->height());
        }
    }
    //scene->setSceneRect(QRectF());
    //mGraphicsView->setSceneRect(QRectF());
    //scene->setSceneRect(QRectF(0,0,mGraphicsView->width(),mGraphicsView->height()));
    //mGraphicsView->setSceneRect(QRect(0,0,mGraphicsView->width(),mGraphicsView->height()));
    //qDebug() << "mGraphicsView width  : " << mGraphicsView->width();
    //qDebug() << "mGraphicsView height : " << mGraphicsView->height();
}

void RCView::setSceneRect(const QRectF & rect)
{
    if(mGraphicsView != 0)
        mGraphicsView->setSceneRect(rect);
}

void RCView::setWindowTitle(const QString &str1, const QString &str2, const QString &str3)
{
    QString text;

    if(str1.length() == 0)
        qDebug() << "view label is empty";

    if(isWindow())
        QWidget::setWindowTitle(str1+str3+str3);
    else
    {
        if(mViewNameLabel != 0)
        {
            mViewNameLabel->setPlainText(str1);
            mViewNameLabel->setText(str1);
        }
        if(mZoomScaleLabel != 0)
        {
            mZoomScaleLabel->setPlainText(str2);
            mZoomScaleLabel->setText(str2);
        }
        if(mPrintScaleLabel != 0)
        {
            mPrintScaleLabel->setPlainText(str3);
            mPrintScaleLabel->setText(str3);
        }
    }
}

QString RCView::windowTitle()
{
    if(mViewNameLabel->plainText().length() == 0)
        qDebug() << "view label is empty";

    if(isWindow())
        return windowTitle();
    else if(mViewNameLabel != 0)
        return mViewNameLabel->plainText()+mZoomScaleLabel->plainText()+mPrintScaleLabel->plainText();
    else
        qt_assert("Title Bar Not Implemented!",__FILE__,__LINE__);
}

int RCView::titleMinimumWidth()
{
    return  mViewNameLabel->width()+
            mZoomScaleLabel->width()+
            mPrintScaleLabel->width()+
            mSplitButton->width()+
            mMaximiseButton->width()+
            mCloseButton->width();
}

void RCView::setFocus(Qt::FocusReason reason)
{
    //if(mGraphicsView != 0)
    //    mGraphicsView->setFocus(reason);
    //setStyleSheet("QFrame#RCView {border: 2px solid darkBlue;}");
#ifdef _MAC
    setStyleSheet("QFrame#RCView {background-color: LightGray;}");
#else
    setStyleSheet("QFrame#RCView {background-color: LightGray;}");
#endif
    //view()->setStyleSheet("border: 1px solid red;");

    emit wasActivated(this);
}


void RCView::clearFocus()
{
    //if(mGraphicsView != 0)
    //    mGraphicsView->clearFocus();

#ifdef _MAC
    setStyleSheet("QFrame#RCView {background-color: rgb(237,237,237);}");
#else
    setStyleSheet("QFrame#RCView {background-color: rgb(240,240,240);}");
#endif

}

void RCView::mouseDoubleClickEvent(QMouseEvent * event)
{
    if(mLabelLayout->geometry().contains(event->pos()))
    {
        event->accept();
        maximize();
    }
}

void RCView::setCentralWidget(QWidget *cw)
{
    mCentralWidget = (RCCentralWidget*)cw;
}

QWidget *RCView::centralWidget()
{
    return mCentralWidget;
}

void RCView::maximize()
{
    //if(mCentralWidget && mCentralWidget->mSplitter->count() > 1)
    if(mCentralWidget && mCentralWidget->viewCount() > 1)
    {
        if(mCentralWidget->isMaximized(this))
            mCentralWidget->normalizeView(this);
        else
            mCentralWidget->maximizeView(this);
        //emit wasActivated(this);
    }
}

void RCView::resizeEvent(QResizeEvent * event)
{
#if 0
    // debug
    qDebug() << "RCView::resizeEvent";
    if(mView3dWindow != 0)
    {
        qDebug() << "View3d name      : " << QString(mView3dWindow->getview()->getname());
        qDebug() << "View3D width     : " << mView3dWindow->getwidth();
        qDebug() << "View3D height    : " << mView3dWindow->getheight();
        qDebug() << "View3D vp width  : " << viewport()->width();
        qDebug() << "View3D vp height : " << viewport()->height();
        qDebug() << "event old width  : " << event->oldSize().width();
        qDebug() << "event old height : " << event->oldSize().height();
        qDebug() << "event new width  : " << event->size().width();
        qDebug() << "event new height : " << event->size().height();

    }
#endif
    //viewport()->resize(event->size());
    if(mView3dWindow != 0)
    {
        //mView3dWindow->sizeevent(viewport()->width(),viewport()->height());
        //mView3dWindow->sizeevent(event->size().width(),event->size().height());
        event->accept();
        emit wasResized(this);
    }
    QFrame::resizeEvent(event);
}

void RCView::closeView()
{
#if 1
    // new
    if(mCentralWidget)
    {
        if(mCentralWidget->isTiled())
        {
            qDebug() << "in RCView::closeView View3dWindow : " << mView3dWindow;
            if(mView3dWindow !=0)
                qDebug() << "in RCView::closeView View3dWindow count : " << mView3dWindow->mcount;
            mCentralWidget->debugView3dList();
            mCentralWidget->debug();

            //mCentralWidget->deleteView(this);

            mCentralWidget->debugView3dList();
            mCentralWidget->debug();

            //if(mGraphicsView != 0)
            //    mGraphicsView->setScene(0);
            //mView3dWindow=0;
            emit isClosing(this);

            //mCentralWidget->debugView3dList();
            //mCentralWidget->debug();
        }
        else // just put up a message about not closing the last window
            emit isClosing(this);
    }
#else
    // old
    if(mCentralWidget)
    {
        if(mCentralWidget->isTiled())
        {
            if(mGraphicsView != 0)
                mGraphicsView->setScene(0);
            mCentralWidget->deleteView(this);
            mView3dWindow=0;
            emit isClosing(this);
            mView3dWindow=0;
        }
        else // just put up a message about not closing the last window
            emit isClosing(this);
    }
#endif
}

void RCView::splitView()
{
    //QPoint pos = QCursor::pos(QGuiApplication::primaryScreen());
    mSplitView_Menu->exec(mSplitButton->mapToGlobal(QPoint(0,0)));
    mSplitView_Menu->hide();
}

void RCView::splitHorizontal()
{

    if(mCentralWidget)
    {
        mCentralWidget->debugView3dList();
        mCentralWidget->debug();

        mCentralWidget->splitView(this,Qt::Horizontal);

        mCentralWidget->debugView3dList();
        mCentralWidget->debug();
    }
}

void RCView::splitVertical()
{
    if(mCentralWidget)
    {
        mCentralWidget->debugView3dList();
        mCentralWidget->debug();

        mCentralWidget->splitView(this,Qt::Vertical);

        mCentralWidget->debugView3dList();
        mCentralWidget->debug();
    }
}

void RCView::NameLabelClicked( )
{
    if(mView3dWindow != 0)
        cadwindow->selectwindow(mView3dWindow);

    if(mView3dWindow != 0 && mView3dWindow->getsurfacetype() == View3dSurf)
    {
        int commandid = 5001; // window_command11
        //int commandid = 522;
        //int commandid = 500;
        if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
        {
            NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
            state.sendevent(nevent);
            state.process();
            //app->processEvents();
            app->sendPostedEvents();
        }
    }
    else
    {
        int commandid = 501; // window_command6
        //int commandid = 522;
        //int commandid = 500;
        if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
        {
            NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
            state.sendevent(nevent);
            state.process();
            //app->processEvents();
            app->sendPostedEvents();
        }
    }
}

void RCView::ZoomScaleClicked()
{
    if(mView3dWindow != 0)
        cadwindow->selectwindow(mView3dWindow);
    int commandid = 2105;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
        state.process();
        app->sendPostedEvents();
        //app->processEvents();
    }
}

void RCView::PrintScaleClicked()
{
    if(mView3dWindow->getsurfacetype() == View3dSurf)
    {
        if(mView3dWindow != 0)
            cadwindow->selectwindow(mView3dWindow);
        int commandid = 521; // view_command15
        if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
        {
            NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
            state.sendevent(nevent);
            state.process();
            app->sendPostedEvents();
            //app->processEvents();
        }
        // update the view title text
        setPrintScale(v.getreal("vw::plotscale"));
    }
}

void RCView::regenGLlists()
{
    if(mGraphicsView)
        mGraphicsView->regenGLlists();
}

int RCView::freezeBackImage()
{
#if 1
    //qDebug() << "mGraphicsView scene" << mGraphicsView->scene();
    //if(((QGLWidget*)mGraphicsView->viewport())->devicePixelRatio() == 1)
    //{
    if(mGraphicsView != 0)
    {
        if(mGraphicsView->backImage != 0)
        {
            delete mGraphicsView->backImage;
            mGraphicsView->backImage=0;
        }
        ((QGLWidget*)mGraphicsView->viewport())->swapBuffers();
        mGraphicsView->backImage = new QImage(((QGLWidget*)mGraphicsView->viewport())->grabFrameBuffer());
    }
        // debug
        //if(mGraphicsView->backImage && !mGraphicsView->backImage->isNull())
        //    mGraphicsView->backImage->save("backimage.jpg","JPG");
    //}
#endif
    return 1;
}

void RCView::unFreezeBackImage()
{
#if 1
    //if(((QGLWidget*)mGraphicsView->viewport())->devicePixelRatio() == 1)
    //{
    if(mGraphicsView != 0)
    {
        if(mGraphicsView->backImage != 0)
            delete mGraphicsView->backImage;
        mGraphicsView->backImage = 0;
        mGraphicsView->refresh();
    }
    //}
#endif
}



