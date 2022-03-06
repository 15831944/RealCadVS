
class TextureMapInfo
  {public:
     int texturetype;
     RCCHAR filename[300];
     double x1,y1,x2,y2,size,patternscale,colourscale;
     Point origin,xaxis,yaxis;
     RealColour transparentcolour;
     BitMask options;
     TextureMapInfo() : options(32) {  }
  };

class BumpMapInfo
  {public:
     RCCHAR filename[300];
     double x1,y1,x2,y2;
     BitMask options;
     BumpMapInfo() : options(32) {  }
  };

class TextureMap;
class BumpMap;
class BumpMaps;
class Textures;
class SurfaceInfo;
class RayTraceImage;

class SurfaceProperties
  {public:
     int shader;
     double transparency,ambient,diffuse,specular,exponent,reflectivity,smoothness,metallic,refractiveindex;
     BitMask options,chang;
     TextureMapInfo tmi;
     BumpMapInfo bmi;
     void loada(RCCHAR *);
     void loadb(RCCHAR *);
     void loadc(RCCHAR *);
     void update(SurfaceProperties *);
     void save(Entity *,int set);
     SurfaceProperties() : options(32),chang(64) {  }
     void getdefaults(void);
     void setdefaults(void);
     int setup(int);
     void change(Entity *,int);
     void verify(Entity *,int);
     int copyinfo(Entity *,int,RayTraceImage *rti,SurfaceInfo **);
  };

struct TMapInfo
  {int texturetype;
   TextureMap *texturemap;
   double x1,y1,x2,y2,size,patternscale,colourscale;
   Point origin,xaxis,yaxis,zaxis;
   RealColour transparentcolour;
   int swapxy,usetransparentcolour;
  };

struct BMapInfo
  {BumpMap *bumpmap;
   double x1,y1,x2,y2;
   int swapxy;
  };

class Noise
  {private:
     int tablesize,voxelbits,voxelextents,pointspervoxel;
     double *table,scale;
     Point p;
     Point *buffer[4];
     int ibuffer[4],jbuffer[4],kbuffer[4];
     void calcpointsinvoxel(int i,int j,int k,Point *);
   public:
     Noise(int,int,int);
     double getnoise(int,Point);
     double getturbulence(Point);
     Point getdnoise(int,Point);
  };

class SurfaceInfo
  {public:
     int white,shader,reflective;
     double transparency,ambient,diffuse,specular,exponent,reflectivity,smoothness,metallic,refractiveindex;
     Noise *noise;
     TMapInfo *tmi;
     BMapInfo *bmi;
     ~SurfaceInfo() {  delete tmi;  delete bmi;  }
  };

struct LightInfo
  {public:
     Point origin,direction;
     int distant,fade,cone;
     RealColour colour;
     double intensity,fadedistance,fadeexponent,coneangle,concentration;
  };

class sunlightInfo
{
public:
    QString Longitude;
    QString Latitude;
    QString Date;
    QString Time;
    QString TimeZone;
public:
    sunlightInfo()
    {
        Longitude.clear();
        Latitude.clear();
        Date.clear();
        Time.clear();
        TimeZone.clear();
    }
};

class TextureMap
  {private:
     int width,height;
     unsigned char **rgbdata;
     //unsigned char *rgbdata;
     RGBQUAD *rgbquad;
     RCCHAR name[256];
   public:
     TextureMap(RCCHAR *);
     ~TextureMap();
     int match(RCCHAR *filename)  {  return strcmp(name,filename) == 0;  }
     void getcolour(double x,double y,RealColour *c);
  };

class Textures
  {private:
     GeneralList list;
   public:
     TextureMap *load(RCCHAR *);
     ~Textures();
  };

class BumpMap
  {private:
     int width,height;
     signed char **normaldata;
     RCCHAR name[256];
   public:
     BumpMap(RCCHAR *);
     ~BumpMap();
     int match(RCCHAR *filename)  {  return strcmp(name,filename) == 0;  }
     void getnormal(double x,double y,Point *normal);
  };

class BumpMaps
  {private:
     GeneralList list;
   public:
     BumpMap *load(RCCHAR *);
     ~BumpMaps();
  };

#if defined(WIN32)
#define NUM_VOXELS 30
#else
#define NUM_VOXELS 16
#endif

class VoxelPolygon
  {public:
     int nloops;
     int *nvert;
     Point *vertices,**verts,pmin,pmax;
     VoxelPolygon(int,int);
     ~VoxelPolygon();
     void clip(int,double,VoxelPolygon *,VoxelPolygon *);
     VoxelPolygon& operator=(const VoxelPolygon&);
  };

class RayTraceImage
  {private:
     View3dSurface *surface;
     GeneralList polygons;
     GeneralList lights;
     GeneralList surfaceinfos;
     Textures textures;
     BumpMaps bumpmaps;
     int imageformat,maximumrays,supersamplingrays,resolution;
     RCCHAR filename[256],backgroundimage[256];
     RealColour backgroundcolour,ambientcolour,fogcolour;
     BitMask options;
     GeneralList voxels[NUM_VOXELS][NUM_VOXELS][NUM_VOXELS];
     double ambientstrength,fogdistance;
     SurfaceInfo *defaultsi;
     Point vmin,vmax,dvoxel;
     long rayid;
     void startbackgroundimage(FILE **,BITMAPINFOHEADER *,unsigned char **);
   public:
     Noise noise;
     RayTraceImage(View3dSurface *s);
    ~RayTraceImage();
     SurfaceInfo *getdefaultsi() {  return defaultsi;  }
     BumpMap *loadbumpmap(RCCHAR *name)  {  return bumpmaps.load(name);  }
     TextureMap *loadtexturemap(RCCHAR *name)  {  return textures.load(name);  }
     int add(RayTracePolygon *rtp);
     int add(SurfaceInfo *si);
     View3dSurface *getwindow(void)  {  return surface;  }
     int setup(void);
     int loadpolygons(void);
     int castray(int,Point,Point,RealColour *,Point *);
     int castoneray(Point,Point,UV *,RayTracePolygon **);
     int raytrace(void);
     void addsurfaceinfo(void *si)  {  surfaceinfos.add(si);  }
  };

struct CallBackInfo
  {RayTraceImage *rti;
   SurfaceInfo *si;
   Entity *se;
  };


