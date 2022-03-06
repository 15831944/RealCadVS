#ifndef _RCWORKPLANES_H
#define _RCWORKPLANES_H

class View3dSurface;
class QGraphicsItem;
class RCView3dPrinter;

class Workplane
  {
#ifdef USING_WIDGETS
public:
    QGraphicsItem *qgi;
#endif
private:
     RCCHAR name[128];
     Point origin,xaxis,yaxis,zaxis;
     double visible;
     int defined;
   public:
     Workplane()
       {
         strcpy(name,"");  origin.setxyz(0.0,0.0,0.0);  xaxis.setxyz(1.0,0.0,0.0);  yaxis.setxyz(0.0,1.0,0.0);  defined = 1;  visible = 0; qgi=0;
       }
     Workplane(RCCHAR *,double,double,double,double,double,double,double,double,double,double);
     Workplane(RCCHAR *,Point,Point,Point,double);
     Workplane(RCCHAR *,int,double);
     int getdefined(void)  {  return defined;  }
     int getvisible(void)  {  return (int)visible;  }
     void setvisible(int v)  {  visible = (double)v;  }
     RCCHAR *getname(void)  {  return name;  }
     Point getorigin(void)  {  return origin;  }
     Point getxaxis(void)  {  return xaxis;  }
     Point getyaxis(void)  {  return yaxis;  }
     Point getzaxis(void)  {  return zaxis;  }
     int change(Point,Point,Point);
     void standard(int);
#ifdef USING_WIDGETS
     void drawGL(int drawmode,View3dSurface *surface);
     void draw(int drawmode,View3dSurface *surface, QPainter *painter);
#endif
     void draw(int drawmode,View3dSurface *surface);
     Point workplanetomodel(Point);
     Point modeltoworkplane(Point);
     int save(CadFile *);
     static Workplane *load(CadFile *);
     int save(UndoFile *);
     int load(UndoFile *);
     int pick(int x,int y,View3dSurface *);
  };

class WorkplaneList : public GeneralList
  {public:
     Workplane *next(void) {  return (Workplane *) GeneralList::next();  }
     Workplane *last(void) {  return (Workplane *) GeneralList::last();  }
  };

class Database;

class Workplanes
  {private:
     WorkplaneList list;
     Workplane *current;
     Workplane dynamic;
   public:
     Workplanes() {  current = NULL;  }
     int add(Workplane *w) {  return list.add(w);  }
     void del(Workplane *w)  {  list.del(w);  }
     void setcurrent(Workplane *w);
     Workplane *getcurrent(void)  {  return current;  }
     Workplane *match(const RCCHAR *);
     Workplane *match(const char *);
     Workplane *match(int);
     Workplane *match(Workplane *);
     void names(RCCHAR ***,int *,int *);
     void clear(void);
     void initialize(void);
     int save(CadFile *);
#ifdef USING_WIDGETS
     void drawGL(int,View3dSurface *);
     void draw(int,View3dSurface *, QPainter *painter);
     void clearScene(int drawmode,View3dSurface *surface);
     void clearScene(int drawmode,RCView3dPrinter *surface);
#endif
     void draw(int,View3dSurface *);
     void pick(int verify,int x,int y,View3dSurface *);
     static int load(CadFile *,Database *);
  };

#endif
