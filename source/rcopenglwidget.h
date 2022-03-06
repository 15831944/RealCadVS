#ifndef RCOPENGLWIDGET_H
#define RCOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class View3dSurface;

class RCOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    View3dSurface * mview3dsurface;

public:
    explicit RCOpenGLWidget(QWidget *parent = nullptr);
protected:
      void initializeGL();
      void resizeGL(int w, int h);
      void paintGL();

signals:

public slots:
};

#endif // RCOPENGLWIDGET_H
