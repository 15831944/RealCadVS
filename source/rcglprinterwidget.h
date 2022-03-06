#ifndef RCGLPRINTERWIDGET_H
#define RCGLPRINTERWIDGET_H

#include <QWidget>
#include <QtOpenGL>
#include "ncwin.h"

class RCGLPrintWidget : public QOpenGLWidget
{
    Q_OBJECT

    RCView3dPrinter *mrcwindow;

public:
    RCGLPrintWidget(QWidget *parent=Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags() , RCView3dPrinter *w=0)
     : QOpenGLWidget(parent,f)
    {
        mrcwindow = w;
    }
    RCGLPrintWidget(const QSurfaceFormat & format, QWidget * parent = Q_NULLPTR, RCView3dPrinter *w=0, Qt::WindowFlags f = Qt::WindowFlags() )
     : QOpenGLWidget(parent, f)
    {
        setFormat(format);
        mrcwindow = w;
    }

protected:
    virtual void initializeGL();
    virtual void resizeGL();
    virtual void paintGL();
};

#endif // RCGLPRINTERWIDGET_H
