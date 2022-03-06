
#define MAX_DXF_LINE_LENGTH 1024

class DXFStyleInfo
  {public:
     char *name;
     int styleno;
     DXFStyleInfo(char *,int);
     ~DXFStyleInfo() {  delete [] name;  }
  };

class DXFStyles
  {private:
     GeneralList list;
   public:
     ~DXFStyles();
     int add(char *,int);
     int match(char *);
  };

class DXFInLayerInfo
  {public:
     int layernumber,attributes,colour,weight;
     char *linetype,*name;
     DXFInLayerInfo(char *,int,int,int,int,char *);
     ~DXFInLayerInfo();
  };

class DXFInLayerList : public GeneralList
  {public:
     ~DXFInLayerList();
     int add(DXFInLayerInfo *li) {  return GeneralList::add(li);  }
     void loadheader(char *name,EntityHeader *,DXFStyles *);
     DXFInLayerInfo *next(void)  {  return (DXFInLayerInfo *) GeneralList::next();  }
  };

class DXFInGroup
  {private:
     int code,type;
     union
       {long i;
        double x;
        char *s;
       };
    public:
      DXFInGroup(int,long);
      DXFInGroup(int,double);
      DXFInGroup(int,char *);
      DXFInGroup(int,int,long,double,char *);
      ~DXFInGroup();
      long *geti(void);
      double *getx(void);
      char *gets(void);
      int match(int c) {  return c == code;  }
      void format(char *);
  };

class DXFInGroupList : public GeneralList
  {public:
     ~DXFInGroupList();
     int add(int,long);
     int add(int,char *);
     int add(int,double);
     int add(int,int,long,double,char *);
     DXFInGroup *next(void)  {  return (DXFInGroup *) GeneralList::next();  }
     void destroy(void);
     void format(char *s);
     long *associ(int,int index = 0);
     double *assocx(int,int index = 0);
     double *assocx(int,int,int);
     char *assocs(int,int index = 0);
  };

class DXFInListOfGroupList : public GeneralList
  {public:
     ~DXFInListOfGroupList();
     int add(DXFInGroupList *);
     DXFInGroupList *next(void)  {  return (DXFInGroupList *) GeneralList::next();  }
  };

class DXFInListOfListOfGroupList : public GeneralList
  {public:
     ~DXFInListOfListOfGroupList();
     int add(DXFInListOfGroupList *);
     DXFInListOfGroupList *next(void)  {  return (DXFInListOfGroupList *) GeneralList::next();  }
  };

class DXFClipBoundaryInfo
  {public:
     Figure *figure;
     char dictionaryobjectid1[20];
     char dictionaryobjectid2[20];
     char spacialfilterobjectid[20];
     DXFClipBoundaryInfo(Figure *f,char *id)  {  figure = f;  strcpy(dictionaryobjectid1,id);  strcpy(dictionaryobjectid2,"");  strcpy(spacialfilterobjectid,"");  }
  };

class DXFInFile
  {protected:
     char title[256];
     char name[256];
     FILE *infile;
     int groupcode,grouptype,error;
     long lineno;
     char groups[MAX_DXF_LINE_LENGTH];
     double groupx,scale;
     long groupi;
     BitMask options;
     // KMJ
     int measurement; // $MEASUREMENT
     int lunits; // $LUNITS
     int insunits; // $INSUNITS
     //

     BitMask papervisibility;
     BitMask modelvisibility;
     BitMask blankvisibility;
     EntityList composeddrawingentitylist;
     GeneralList clipboundarylist;

   public:
     DXFInFile() : papervisibility(MaxViews),modelvisibility(MaxViews),blankvisibility(MaxViews),options(32) {  infile = 0;  error = 0;  lineno = 1;  scale = 1.0;  }
     ~DXFInFile()  {  if (infile != 0) fclose(infile);  }
     int setscale(void);
     int open(RCCHAR *);
     void close(void);
     int read(void);
     int convert(void);
     void printerror(RCCHAR *);
     void readheader(void);
     void readblocks(void);
     void readtables(void);
     void readentities(void);
     void readobjects(void);
     void nextgroup();
     char *getname() { return name; }
     char *getttitle() { return title; }
     virtual void processheader(DXFInGroupList *) = 0;
     virtual void processtable(DXFInGroupList *) = 0;
     virtual void processblock(DXFInGroupList *) = 0;
     virtual void processentity(DXFInGroupList *) = 0;
  };

class DXFInConvert : public DXFInFile
  {private:
     DXFInLayerList layerlist;
     int nvertices,meshm,meshn,nblocks,pdmode;
     double pdsize;
     Point firstvertex,lastvertex;
     Point *points;
     long polylineflags;
     double bulge;
     char *blockname;
     int blockisxref;
     char templatefilename[300];
     DXFStyles dxfstyles;
     double leadersize;

     GeneralList orphan_view_list;

   public:
     DXFInConvert();
     void loadcolourtable(void);
     void processheader(DXFInGroupList *);
     void processtable(DXFInGroupList *);
     void processblock(DXFInGroupList *);
     void processentity(DXFInGroupList *);
     void processobject(DXFInGroupList *);
  };

class DXFOut
  {private:
     FILE *outfile;
     Colour dxfcolourtable[256];
     int error,handle,ownerhandle;
     double scale,textscale;
     View3dSurface *window;
   public:
     DXFOut();
     ~DXFOut();
     int open(RCCHAR *);
     int open(char *);
     int setscale(void);
     double getscale(void)  {  return scale;  }
     double gettextscale(void)  {  return textscale;  }
     void close(void);
     void write(EntityList *list);
     int writehandle(void);
     int writelong(int,long);
     int writereal(int,double);
     int writestring(int,RCCHAR *);
     int writestring(int,char *);
     int writestring(int,RCCHAR *,int);
     int writestring(int,char *,int);
     int writeheader(Entity *);
     void setownerhandle(int handle)  {  ownerhandle = handle;  }
     int getownerhandle(void)  {  return ownerhandle;  }
     View3dSurface *getwindow(void)  {  return window;  }
  };


enum {  DXFOptionClearLayerNames  };
