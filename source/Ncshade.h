
class EdgeTable;
class Surface;
class Shader;

class RenderImage
  {private:
     GeneralList shaders;
     View3dSurface *surface;
     EdgeTable *et;
     int clear;
     double ambient,diffuse;
    public:
     RenderImage(View3dSurface *s,int c,double a,double d)
       {  surface = s;  clear = c;  ambient = a;  diffuse = d;
       }
     View3dSurface *getsurface(void)  {  return surface;  }
     double getambient(void)  {  return ambient;  }
     double getdiffuse(void)  {  return diffuse;  }
     EdgeTable *getet(void)  {  return et;  }
     void addshader(Shader *s)  {  shaders.add(s);  }
     int addedges(SurfaceEntity *,int nvert,Point *point);
     void render(void);
#ifdef USING_WIDGETS
     void renderGL(void);
#endif
  };

class FillSegments
  {private:
     int y,x1,x2,r,g,b;
     double z;
     Surface *surface;
     RCHBRUSH hbrush;
   public:
     FillSegments(Surface *);
     ~FillSegments();
     void fill(int,double,double,double,double,int,int,int);
     void flush(void);
  };

class Shader
  {public:
     int in,r,g,b;
     Point normal;
     double D;
//     void *operator new(size_t size);
//     void operator delete(void *a);
     Shader(int,int,int,Point,double,Point,Point,double,double);
     void fill(FillSegments *,int y,double,double,double,double);
     int infrontof(Shader *,double,double);
  };

class Edge : public OrderedObject
  {public:
     double x,dx;
     int ymin,ymax,horizontal;
     Shader *shader;
     //void *operator new(size_t size);
     //void operator delete(void *a);
     Edge(int,Point p1,Point p2,Shader *);
     int nextline(int y);
     int operator<(OrderedObject &oo)
       {  return x < ((Edge *)&oo)->x;  }
  };

class EdgeList : public SortedGeneralList
  {public:
     Edge *next(void)  {  return (Edge *) GeneralList::next();  }
  };

class ActiveEdgeList : public EdgeList
  {public:
     void fill(Surface *,int y);
     void nextline(int y);
  };

class EdgeTable
  {private:
     int size;
     EdgeList *lists;
   public:
     EdgeTable(int);
     ~EdgeTable();
     EdgeList *operator[](int i)  {  return &lists[i];  }
     int getdefined() {  return lists != NULL;  }
     int add(Edge *edge)
       {  if (edge->ymin < size)
            return lists[edge->ymin].add(edge);
          else
            {  delete edge;  return 1;
            }
       }
  };

