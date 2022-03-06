#ifndef RCCENTRALWIDGET_H
#define RCCENTRALWIDGET_H

#include "RCView.h"
#include "RCGraphicsView.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QMainWindow>
#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>

class QGraphicsScene;

class RCCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RCCentralWidget(QWidget *parent = 0);

    RCView* activeWindow() const;
    void initialise(Qt::Orientation orientation=  Qt::Horizontal);
    void initializeSplitter(QSplitter *splitter);
    void deleteAllViews();
    void regenGLlists();
    bool isMaximized(RCView *view);
    int  isTiled();
    void debug() { int count=0; debugSplitter(mSplitter,count); }
    void debugSplitter(QSplitter *mSplitter, int &count);
    void debugView3dList();
    void moveSplitterView(QSplitter *splitter, RCView* moveView, int fromIndex, int toIndex);
    int  viewCount();
    int  splitterCount();
    int  visibleViewCount();
    int  visibleSplitterCount(QSplitter *splitter, int &counter);
    int  visibleSplitterCount();

    QByteArray lastState;
    QSplitter *mSplitter; // main splitter owned by the layout
    // a temporay global which is the splitter to be used when adding a new window
    // if it is null the mSplitter is used by default
    QSplitter *mNewSplitter;
    QVBoxLayout *mLayout; // whole layout
    int tiled;

signals:
    void viewWasMaxmized();
    //void changed(const QMimeData *mimedata = 0);

public slots:
    void tile();
    void tileVertical();
    void tileHorizontal();
    void setSingleView(RCView *view);
    int  addView(RCView *view, int index=-1);
    int  addView(QSplitter *pSplitter, RCView *view,int index=-1);
    void replaceView(int index,RCView *view);
    void insertView(int index, RCView *view, Qt::Orientation orientation=Qt::Horizontal);
    void deleteView(int index);
    void deleteView(RCView *view);
    void hideView(int index);
    void hideView(RCView *view);
    void showView(int index);
    void showView(RCView *view);
    void normalizeView(RCView *view);
    void normalizeView(int index);
    void maximizeView(RCView *view);
    void maximizeView(int index);
    void setActiveWindow(RCView *view);
    void unsetActiveWindow(RCView *view);
    void splitView(RCView *view,Qt::Orientation orientation);
    void purgeSplitters();
private:
    void setupMatrix();
    void showSplitters(QSplitter *mSplitter);
    void hideSplitters(QSplitter *splitter, RCView *view);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    //void dragLeaveEvent(QDragLeaveEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

};

#endif // RCCENTRALWIDGET_H
