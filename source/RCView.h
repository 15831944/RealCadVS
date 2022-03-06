#ifndef RCVIEW_H
#define RCVIEW_H

#include <QMenu>
#include <QFrame>
#include <QLabel>
#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QMimeData>

QT_BEGIN_NAMESPACE
class QLabel;
class QToolBar;
class QToolButton;
class GraphicsView;
class QVBoxLayout;
class QHBoxLayout;
QT_END_NAMESPACE

class RCView;
class RCGraphicsView;
class RCGraphicsScene;
class RCCentralWidget;
class View3dWindow;
class View3dGLOutput;

/*
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(MyView *v) : QGraphicsView(), view(v) { }

protected:
    void wheelEvent(QWheelEvent *);

private:
    MyView *view;
};
*/

class RCViewLabel : public QLabel
{
    Q_OBJECT
public:
    explicit RCViewLabel(QWidget *parent = 0);
    explicit RCViewLabel(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0);
    //~RCView();

    void setPlainText(const QString text);
    QString plainText();
    void mousePressEvent(QMouseEvent * event);
    void mouseEnterEvent(QMouseEvent * event);
    void mouseLeaveEvent(QMouseEvent * event);
    bool event(QEvent *e);

signals:
    void clicked();

public slots:
    void onClicked();

private:
    QString mPlainText;

};

class RCView : public QFrame
{
    Q_OBJECT
public:
    int mcount;
    RCView(QWidget *parent = 0);
    explicit RCView(const QString &name, RCGraphicsView *gview=0,  QWidget *parent = 0);
    ~RCView();
    QGraphicsView *view() const;
    QWidget* viewport() const;
    void setWindowTitle(const QString &str1, const QString &str2, const QString &str3);
    QString windowTitle();
    void setCentralWidget(QWidget *cw);
    QWidget *centralWidget();
    void mouseDoubleClickEvent(QMouseEvent * event);
    inline void setView3dWindow(View3dWindow *window) { mView3dWindow = window; }
    inline View3dWindow *getView3dWindow() { return mView3dWindow; }
    inline void setView3dOutput(View3dGLOutput *output) { mView3dOutput = output; }
    inline View3dGLOutput *getView3dOutput() { return mView3dOutput; }
    int titleMinimumWidth();
    void setPrintScale(double scale) { mPrintScaleLabel->setText(QString(" Print Scale %1").arg(scale)); }
    void updateLabel()
    {
        QString tmp = mViewNameLabel->plainText();
        mViewNameLabel->clear();
        mViewNameLabel->setPlainText(tmp);
        mViewNameLabel->update();
    }
    void regenGLlists();

public:
signals:
    void titleDoubleClicked();
    void wasActivated(RCView *view);
    void wasDeActivated(RCView *view);
    void wasResized(RCView *view);
    void isClosing(RCView *view);
    void changed(const QMimeData *mimedata = 0);

protected:
    void resizeEvent(QResizeEvent * event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

signals:

public slots:
    void zoomIn( );
    void zoomOut( );
    void rotateLeft();
    void rotateRight();
    void setView(QGraphicsView *view);
    void setViewport(QWidget *view);
    void setScene(QGraphicsScene *scene);
    void setSceneRect(const QRectF & rect);
    void setFocus(Qt::FocusReason reason=Qt::OtherFocusReason);
    void clearFocus();
    void maximize();
    void closeView();
    void splitView();
    void splitHorizontal();
    void splitVertical();
    void NameLabelClicked();
    void ZoomScaleClicked();
    void PrintScaleClicked();
    //
    int freezeBackImage();
    void unFreezeBackImage();

private slots:
    void setupMatrix();
    void print();


private:
    QToolButton *mSplitButton;
    QToolButton *mMaximiseButton;
    QToolButton *mCloseButton;
    QMenu *mSplitView_Menu;
    qreal scale;
    qreal angle;
    RCGraphicsView *mGraphicsView;
    RCGraphicsScene *mGraphicsScene;
    QLabel *mLabel;
    QToolBar *mToolBar;
    RCViewLabel *mViewNameLabel;
    RCViewLabel *mZoomScaleLabel;
    RCViewLabel *mPrintScaleLabel;
    QVBoxLayout *mLayout; // whole view layout
    QHBoxLayout *mLabelLayout; // title bar
    RCCentralWidget *mCentralWidget;
    View3dWindow *mView3dWindow;
    View3dGLOutput *mView3dOutput;
};

#endif // RCVIEW_H
