#ifndef RCGRAPHICSITEMS_H
#define RCGRAPHICSITEMS_H

#include "ncwin.h"

class Plane;
class BitMapE;
class View3dSurface;
class Patch;
class Transform;

class  RCGraphicsPlaneItem : public QGraphicsItem
{
protected:
    // protected member data

public:
    // public member data
    Plane *rcplane;
    BitMapE *rcbitmap;
    View3dSurface *rcsurface;
    Transform transform;

    QPainterPath qpath;
    QGraphicsItem *qpattern;
    QGraphicsItem *qedges;
    int rcdrawmode;

public:
    // public methods
    RCGraphicsPlaneItem(Plane *mplane,const Transform &transf, View3dSurface *msurface=0, bool clipto=true, QGraphicsItem * parent=0);
    virtual ~RCGraphicsPlaneItem();

    virtual QRectF boundingRect(void) const;
    virtual QPainterPath shape(void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

class  RCGraphicsBitmapItem : public QGraphicsItem
{
protected:
    // protected member data

public:
    // public member data
    BitMapE *rcbitmap;
    View3dSurface *rcsurface;

    QPainterPath qpath;
    int rcdrawmode;

public:
    // public methods
    RCGraphicsBitmapItem(BitMapE *mbitmap=0, View3dSurface *msurface=0, QGraphicsItem * parent=0);
    virtual ~RCGraphicsBitmapItem();

    virtual QRectF boundingRect(void) const;
    virtual QPainterPath shape(void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

class RCGraphicsPatchItem : public QGraphicsItem
{
protected:
    // protected member data

public:
    // public member data
    Patch *rcpatch;
    View3dSurface *rcsurface;

    QPainterPath qpath;
    int rcdrawmode;

public:
    // public methods
    RCGraphicsPatchItem(Patch *mpatch=0, View3dSurface *msurface=0, QGraphicsItem * parent=0);
    virtual ~RCGraphicsPatchItem();

    virtual QRectF boundingRect(void) const;
    virtual QPainterPath shape(void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};


class RCGraphicsLineItem : public QGraphicsItem
{
protected:
    // protected member data

public:
    // public member data
    Line *rcline;
    View3dSurface *rcsurface;

    int rcdrawmode;

public:
    // public methods
    RCGraphicsLineItem(Line *mline=0, View3dSurface *msurface=0, QGraphicsItem * parent=0);
    virtual ~RCGraphicsLineItem();

    virtual QRectF boundingRect(void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

class RCGraphicsCircleItem : public QGraphicsItem
{
protected:
    // protected member data

public:
    // public member data
    Circle *rccircle;
    View3dSurface *rcsurface;

    int rcdrawmode;

public:
    // public methods
    RCGraphicsCircleItem(Circle *mcircle=0, View3dSurface *msurface=0, QGraphicsItem * parent=0);
    virtual ~RCGraphicsCircleItem();

    virtual QRectF boundingRect(void) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};
#endif // RCGRAPHICSITEMS_H
