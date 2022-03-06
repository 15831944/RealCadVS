
class QGraphicsItem;

class HideImage
  {
#ifdef USING_WIDGETS
public:
     QGraphicsItem *qgi;
#endif
private:
     View3dSurface *surface;
     Transform *transform;
     BitMask *visibility;
     int clear;
     GeneralList list;
     Entity *lastentity;
     int lasthidden;
public:
     int drawmode; // 0=screen; 1=printer
public:
     HideImage(View3dSurface *s,int c)
       {  surface = s;  clear = c;  visibility = 0;  transform = 0; drawmode = 0;
       }
     HideImage(View3dSurface *s,BitMask *v,Transform *t)
       {  surface = s;  visibility = v;  transform = t; drawmode = 0;
       }
     ~HideImage();
     int add(Polygon3d *);
     View3dSurface *getsurface(void)  {  return surface;  }
     void hide(void);
     void hide(Entity *,Point,Point);
     void gettransform(Transform *) const;
     void applytransform(const Transform &);
     void settransform(Transform &);
#ifdef USING_WIDGETS
     void hideGL(void);
     void hideGL(Entity *,Point,Point);
     int getClear() { return clear; }
#endif
  };


