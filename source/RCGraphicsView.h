#ifndef RCGRAPHICSVIEW_H
#define RCGRAPHICSVIEW_H

#include <QDragEnterEvent>
#include <QMimeData>
#include <QGraphicsView>;
#include <QTimer>
#include <QElapsedTimer>

class QWidget;
class QGraphicsView;
class QGraphicsScene;
class QResizeEvent;
class QMouseEvent;
class QWheelEvent;
class QPainter;
class QRectF;
class QTimer;
class QElapsedTimer;

class RCGraphicsView;

/**
 * @brief The RCGraphicsView class
 */
class RCGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    int mcount;
    explicit RCGraphicsView(QWidget *parent = 0);
    explicit RCGraphicsView(QGraphicsScene *scene, QWidget *parent = 0);
    ~RCGraphicsView();

    unsigned int gldlist1; // 3d view wireframe gl display list
    unsigned int gldlist2; // 2d sub gl display list
    unsigned int gldlist3; // 2d view gl display list
    unsigned int gldlist4; // 3d view hidden line gl display list
    unsigned int gldlist5; // 3d view shaded gl display list
    unsigned int gldlist6; // 3d view selection gl display list
    unsigned int gldlist7; // 2d sub view selection gl display list
    unsigned int gldlist8; // 2d view selection gl display list
    std::vector<unsigned int> gldsublists; // layout viewports gl display lists
    QImage *backImage;
    qint64 fRate,lastfRate;
    QElapsedTimer fTimer;
    int refreshDelay,reduceDetail;
#if 0
    QTimer refreshTimer;
#else
    QTimer *refreshTimer;
#endif
    bool resizing;

    void setRefreshDelay(int delay) { refreshDelay=delay; setTimer(delay); }
    void setReduceDetail(int detail) { reduceDetail=detail; }
    void setTimer(int ms=2000);

public slots:
    void regenGLlists();
    void resetGLlists();
    void refresh();

signals:
    void changed(const QMimeData *mimedata = 0);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);
    //void paintEvent(QPaintEvent *event);
    void drawBackground(QPainter * painter, const QRectF & rect);
    void drawForeground(QPainter * painter, const QRectF & rect);
    void enterEvent(QEvent *event);
    /*
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    */

};
#endif // RCGRAPHICSVIEW_H
