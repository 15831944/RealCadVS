
#include "ncwin.h"
#include <QProcess>
#include "dxfin_dialog.h"
#include "dwgimportoptions_dialog.h"

#ifndef _USING_QT
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define MAX_LAYOUTS 200

extern void open_dwgdxf(RCCHAR *filename);
extern void open_dwg(RCCHAR *filename);

class PaperSpaceLayouts
  {private:
     int npaperspace;
     char PaperSpaceNames[MAX_LAYOUTS][300];
     char LayoutNames[MAX_LAYOUTS][300];
     double Width[MAX_LAYOUTS];
     double Height[MAX_LAYOUTS];
     double OffsetX[MAX_LAYOUTS];
     double OffsetY[MAX_LAYOUTS];
     char Handles[MAX_LAYOUTS][50];
   public:
     PaperSpaceLayouts();
     void Clear(void);
     void AddPaperSpace(char *paperspacename,char *handle);
     void ChangeLayout(char *handle,char *layoutname,double width,double height,double offsetx,double offsety);
     char *GetLayout(char *papername,double *width,double *height,double *offsetx,double *offsety);
  } PaperSpaceLayouts;


PaperSpaceLayouts::PaperSpaceLayouts()
{ Clear();
}

void PaperSpaceLayouts::Clear(void)
{ npaperspace = 0;
}

void PaperSpaceLayouts::AddPaperSpace(char *paperspacename,char *handle)
{ 
  if (npaperspace < MAX_LAYOUTS)
    {  strcpy(PaperSpaceNames[npaperspace],paperspacename);
       strcpy(LayoutNames[npaperspace],paperspacename);
       strcpy(Handles[npaperspace],handle);
       npaperspace++;
    }
}


void PaperSpaceLayouts::ChangeLayout(char *handle,char *layoutname,double width,double height,double offsetx,double offsety)
{int i;

  for (i = 0 ; i < npaperspace ; i++)
    if (_stricmp(handle,Handles[i]) == 0)
      {  strcpy(LayoutNames[i],layoutname);
         Width[i] = width;
         Height[i] = height;
         OffsetX[i] = offsetx;
         OffsetY[i] = offsety;
      }

}

char *PaperSpaceLayouts::GetLayout(char *papername,double *width,double *height,double *offsetx,double *offsety)
{int i;
  for (i = 0 ; i < npaperspace ; i++)
    if (_stricmp(papername,PaperSpaceNames[i]) == 0)
      {*width = Width[i];
       *height = Height[i];
       *offsetx = OffsetX[i];
       *offsety = OffsetY[i];
       return LayoutNames[i];
      }
  return 0;
}

#define MAX_BLOCKS 8000

static int nblocks;
static char BlockName[MAX_BLOCKS][300];
static char XREFPathName[MAX_BLOCKS][300];
static Point BlockOrigin[MAX_BLOCKS];

#if MENUS != STUDENTMENUS

DXFInGroup::DXFInGroup(int c,long v)
{ code = c;  i = v;  type = 0;
}

DXFInGroup::DXFInGroup(int c,double v)
{ code = c;  x = v;  type = 1;
}

DXFInGroup::DXFInGroup(int c,char *v)
{ code = c;
  if ((s = new char [strlen(v) + 1]) != 0)
    strcpy(s,v);
  type = 2;
}

DXFInGroup::DXFInGroup(int c,int t,long iv,double xv,char *sv)
{ code = c;
  type = t;
  if (type == 0)
    i = iv;
  else if (type == 1)
    x = xv;
  else if (type == 2)
    {  if ((s = new char [strlen(sv) + 1]) != 0)
         strcpy(s,sv);
    }
}

void DXFInGroup::format(char *s1)
{ if (type == 0)
    sprintf(s1," (%d . %ld) ",code,i);
  else if (type == 1)
    sprintf(s1," (%d . %.6lf) ",code,x);
  else if (type == 2)
    sprintf(s1," (%d . \"%s\")",code,(s != 0 ? s : ""));
}

DXFInGroup::~DXFInGroup()
{ if (type == 2)
    delete s;
}

long *DXFInGroup::geti(void)
{ if (type == 0)
    return &i;
  else
    return 0;
}

double *DXFInGroup::getx(void)
{ if (type == 1)
    return &x;
  else
    return 0;
}

char *DXFInGroup::gets(void)
{ if (type == 2)
    return s;
  else
    return 0;
}

DXFInGroupList::~DXFInGroupList()
{DXFInGroup *g;
  for (start() ; (g = next()) != 0 ; )
    delete g;
}

void DXFInGroupList::destroy(void)
{DXFInGroup *g;
  for (start() ; (g = next()) != 0 ; )
    delete g;
  GeneralList::destroy();
}

void DXFInGroupList::format(char *s)
{DXFInGroup *g;
  *s = 0;
  for (start() ; (g = next()) != 0 ; )
    g->format(s + strlen(s));
}

long *DXFInGroupList::associ(int c,int index)
{DXFInGroup *g;
 int i;
  for (start(),i = 0 ; (g = next()) != 0 ; )
    if (g->match(c))
      {  if (i == index)  
           return g->geti();
         i++;
      }
  return 0;
}

double *DXFInGroupList::assocx(int c,int index)
{DXFInGroup *g;
 int i;
  for (start(),i = 0 ; (g = next()) != 0 ; )
    if (g->match(c))
      {  if (i == index) 
           return g->getx();
         i++;
      }
  return 0;
}

//
//  Use another group number as the index for this group number
//     '42' values are not always present
//
double *DXFInGroupList::assocx(int c,int assocc,int index)
{DXFInGroup *g;
 int i;
  for (start(),i = 0 ; (g = next()) != 0 ; )
    if (g->match(assocc))
      {  if (i == index) 
           {  while ((g = next()) != 0)
                {  if (g->match(c))
                     return g->getx();
                   else if (g->match(assocc) || g->match(0))
                     return 0;
                } 
           }  
         i++;
      }
  return 0;
}

char *DXFInGroupList::assocs(int c,int index)
{DXFInGroup *g;
 int i;
  for (start(),i = 0 ; (g = next()) != 0 ; )
    if (g->match(c))
      {  if (i == index) return g->gets();
         i++;
      }
  return 0;
}

int DXFInGroupList::add(int c,long i)
{DXFInGroup *g;
  if ((g = new DXFInGroup(c,i)) == 0 ) return 0;
  return GeneralList::add(g);
}

int DXFInGroupList::add(int c,double x)
{DXFInGroup *g;
  if ((g = new DXFInGroup(c,x)) == 0) return 0;
  return GeneralList::add(g);
}

int DXFInGroupList::add(int c,char *s)
{DXFInGroup *g;
  if ((g = new DXFInGroup(c,s)) == 0) return 0;
  return GeneralList::add(g);
}

int DXFInGroupList::add(int c,int t,long i,double x,char *s)
{DXFInGroup *g;
  if ((g = new DXFInGroup(c,t,i,x,s)) == 0) return 0;
  return GeneralList::add(g);
}

DXFInListOfGroupList::~DXFInListOfGroupList()
{DXFInGroupList *gl;
  for (start() ; (gl = next()) != 0 ; )
    delete gl;
}

int DXFInListOfGroupList::add(DXFInGroupList *gl)
{DXFInGroupList *newgl;
  if ((newgl = new DXFInGroupList) == 0) return 0;
  *newgl = *gl;
  return GeneralList::add(newgl);
}

DXFInListOfListOfGroupList::~DXFInListOfListOfGroupList()
{DXFInListOfGroupList *lgl;
  for (start() ; (lgl = next()) != 0 ; )
    delete lgl;
}

int DXFInListOfListOfGroupList::add(DXFInListOfGroupList *lgl)
{DXFInListOfGroupList *newlgl;
  if ((newlgl = new DXFInListOfGroupList) == 0) return 0;
  *newlgl = *lgl;
  return GeneralList::add(newlgl);
}

DXFInLayerInfo::DXFInLayerInfo(char *n,int l,int a,int c,int w,char *lt)
{ layernumber = l;
  attributes = a;
  colour = c;
  weight = w;
  linetype = new char [strlen(lt) + 1];
  name = new char [strlen(n) + 1];
  strcpy(linetype,lt);
  strcpy(name,n);
}

DXFInLayerInfo::~DXFInLayerInfo()
{ delete linetype;
  delete name;
}

DXFInLayerList::~DXFInLayerList()
{DXFInLayerInfo *li;
  for (start() ; (li = next()) != 0 ; )
    delete li;
}

void DXFInLayerList::loadheader(char *name,EntityHeader *e,DXFStyles *dxfstyles)
{DXFInLayerInfo *li;
  for (start() ; (li = next()) != 0 ; )
    if (strcmp(name,li->name) == 0)
      {  e->change(li->colour,li->layernumber,dxfstyles->match(li->linetype),li->weight);
         return;
      }
  if ((li = new DXFInLayerInfo(name,length(),0,7,0, (char*)"CONTINUOUS")) != 0)
    {  db.layernames.setlayername(length(),name);
       add(li);
    }
  e->change(li->colour,li->layernumber,dxfstyles->match(li->linetype),e->getweight());
}

int DXFInFile::open(RCCHAR *s)
{RCOPENFILENAME ofn;
 RCCHAR szDirName[256],szFilter[256],rcname[256],rctitle[265];
 RCUINT  i;
 ResourceString rs0(NCDXF);
 ResourceString rs2(NCDXF+2);
 ResourceString rs3(NCDXF+3);

#ifdef USING_WIDGETS
  blankvisibility.clearall();

  if (s == 0)
  {
       _getcwd(szDirName, sizeof(szDirName));
       strcpy(name,"");
       strcpy(rcname,"");
       strcpy(title,"");
       strcpy(rctitle,"");
       _tcscpy(szFilter,rs2.getws());
       //for (i = 0; szFilter[i] != '\0'; i++)
       //  if (szFilter[i] == '|') szFilter[i] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = szFilter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile = rcname;
       ofn.nMaxFile = sizeof(rcname);
       ofn.lpstrFileTitle = rctitle;
       ofn.nMaxFileTitle = sizeof(rctitle);
       ofn.lpstrInitialDir = szDirName;
       ofn.lpstrTitle =  rs0.getws();
       ofn.Flags = 0;//OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

       if (GetOpenFileName(&ofn))
       {
         infile = fopen(QString(ofn.lpstrFile).toLatin1().data(),"rb");
         strcpy(name,rcname);
         strcpy(title,rctitle);
       }
       else
         infile = 0;
       if (strchr(title,'.') != 0)
         *strchr(title,'.') = 0;
       strcat(title,".BKS");
  }
  else
  {
       infile = fopen(s,"rb");
       strcpy(title,s);
       if (strchr(title,'.') != 0)
         *strchr(title,'.') = 0;
       strcat(title,".BKS");
  }
  return infile != 0;
#else
 blankvisibility.clearall();


  if (s == 0)
    {
       _getcwd(szDirName, sizeof(szDirName));
       strcpy(name,"");
       strcpy(title,"");
       _tcscpy(szFilter,rs2.getws());
       for (i = 0; szFilter[i] != '\0'; i++)
         if (szFilter[i] == '|') szFilter[i] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = szFilter;
       ofn.nFilterIndex = 1;
#if ! defined(_WIN32_WCE)
       ofn.lpstrFile = name;
       ofn.nMaxFile = sizeof(name);
       ofn.lpstrFileTitle = title;
       ofn.nMaxFileTitle = sizeof(title);
//WCEFIX
#endif
       ofn.lpstrInitialDir = szDirName;
       ofn.lpstrTitle =  rs0.getws();
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
       if (GetOpenFileName(&ofn))
         infile = fopen(ofn.lpstrFile,TEXT("rb"));
       else
         infile = 0;
       if (strchr(title,'.') != 0)
         *strchr(title,'.') = 0;
       strcat(title,".BKS");
    }
  else
    {  infile = fopen(s,"rb");
       strcpy(title,s);
       if (strchr(title,'.') != 0)
         *strchr(title,'.') = 0;
       strcat(title,".BKS");
    }
  return infile != 0;
#endif
}

void DXFInFile::close(void)
{ if (infile != 0)
    fclose(infile);
  infile = 0;
}

void DXFInFile::nextgroup(void)
{char line[MAX_DXF_LINE_LENGTH];
 char *endptr;
 int i,c,cold;
  if (error != 0 || infile == 0)
    {  groupcode = -1;  return;
    }
  do {  i = 0;
        while (i < MAX_DXF_LINE_LENGTH-1)
          {  c = getc(infile);
             if (i == 0 && c == EOF)
               {  error = 1;  return;
               }
             if (c == '\n' || c == '\r')
               {  cold = c;
                  c = getc(infile);
                  if (c != '\n' && c != '\r' || c == cold)
                    ungetc(c,infile);
                  break;
               }
             line[i++] = c;
          }
        line[i] = 0;
        endptr = line;  while (*endptr == ' ') *endptr++;
        groupcode = int(strtol(endptr,&endptr,10));
        while (*endptr == ' ') *endptr++;
        if (*endptr != 0 && *endptr != '\n' && *endptr != '\r' && *endptr != '\t')
          {  groupcode = -1;  
             error = 1;  
             return;
          }
        i = 0;
        while (i < MAX_DXF_LINE_LENGTH-1)
          {  c = getc(infile);
             if (i == 0 && c == EOF)
               {  error = 1;  return;
               }
             if (c == '\n' || c == '\r')
               {  c = getc(infile);
                  if (c != '\n' && c != '\r')
                    ungetc(c,infile);
                  break;
               }
             line[i++] = c;
          }
        line[i] = 0;
        if (strlen(line) > 0 && line[strlen(line)-1] == '\r')
          line[strlen(line)-1] = 0;
        if (strlen(line) > 0 && line[strlen(line)-1] == '\n')
          line[strlen(line)-1] = 0;
     } while (groupcode == 999);

  if (groupcode >=    0 && groupcode <=    9 ||
      groupcode >=  100 && groupcode <=  105 ||
      groupcode >=  300 && groupcode <=  369 ||
      groupcode >=  390 && groupcode <=  399 ||
      groupcode >=  410 && groupcode <=  419 ||
      groupcode >=  430 && groupcode <=  439 ||
      groupcode >=  470 && groupcode <=  479 ||
      groupcode >= 1000 && groupcode <= 1009)
    {  strcpy(groups,line);
       grouptype = 2;
       lineno += 2;
    }
  else if (groupcode >=   10 && groupcode <=   59 ||
           groupcode >=  110 && groupcode <=  149 ||
           groupcode >=  210 && groupcode <=  239 ||
           groupcode >=  370 && groupcode <=  389 ||
           groupcode >=  460 && groupcode <=  469 ||
           groupcode >= 1010 && groupcode <= 1059)
    {  groupx = strtod(line,&endptr);  while (*endptr == ' ') *endptr++;
       grouptype = 1;
       lineno += 2;
    }
  else if (groupcode >=   60 && groupcode <=   79 ||
           groupcode >=   90 && groupcode <=   99 ||
           groupcode >=  170 && groupcode <=  179 ||
           groupcode >=  270 && groupcode <=  299 ||
           groupcode >=  400 && groupcode <=  409 ||
           groupcode >=  420 && groupcode <=  429 ||
           groupcode >=  440 && groupcode <=  459 ||
           groupcode >= 1060 && groupcode <= 1079)
    {  endptr = line;  while (*endptr == ' ') *endptr++;
       groupi = strtol(endptr,&endptr,10);  while (*endptr == ' ') *endptr++;
       grouptype = 0;
       lineno += 2;
    }
  else
    {//  Unknown group type -- Return a string
       strcpy(groups,line);
       grouptype = 2;
       lineno += 2;
    }
}

void DXFInFile::readheader(void)
{DXFInGroupList gl;
  while (groupcode == 9)
    {  gl.add(groupcode,grouptype,groupi,groupx,groups);
       nextgroup();
       while (groupcode > 0)
         {  gl.add(groupcode,grouptype,groupi,groupx,groups);
            nextgroup();
         }
       processheader(&gl);
       gl.destroy();
    }
  if (groupcode == 0 && strcmp(groups,"ENDSEC") == 0)
    nextgroup();
}

void DXFInFile::readblocks(void)
{DXFInGroupList gl;
 int i,finished;
  while (groupcode == 0 && strcmp(groups,"BLOCK") == 0)
    {  finished = 0;
       //  Reset model visibility
       modelvisibility.clearall();
       for (i = 0 ; i < MaxViews ; i++)
         if (db.views3d.match(i) != 0)
           modelvisibility.set(i);

       while (! finished)
         {  finished = groupcode != 0 || strcmp(groups,"ENDBLK") == 0;
            gl.add(groupcode,grouptype,groupi,groupx,groups);
            nextgroup();
            while (groupcode > 0)
              {  gl.add(groupcode,grouptype,groupi,groupx,groups);
                 nextgroup();
              }
            processblock(&gl);
            gl.destroy();
         }
    }
  if (groupcode == 0 && strcmp(groups,"ENDSEC") == 0)
    nextgroup();
}

void DXFInFile::readobjects(void)
{DXFInGroupList gl;
 char text[300],*handle,*name,*objectid,*nextobjectid;
 double *x1,*y1,*x2,*y2;

  while (groupcode == 0 && strcmp(groups,"ENDSEC") != 0)
    {  strcpy(text,groups);
       gl.add(groupcode,grouptype,groupi,groupx,groups);
       nextgroup();

       while (groupcode > 0)
         {  gl.add(groupcode,grouptype,groupi,groupx,groups);
            nextgroup();
         }
       gl.reverse();

       if (strcmp(text,"LAYOUT") == 0)
         {DXFInGroup *g;

            name = handle = 0;

            for (gl.start() ; (g = gl.next()) != 0 ; )
              if (g->match(100) && strcmp(g->gets(),"AcDbLayout") == 0)
                break;

            while ((g = gl.next()) != 0)
              if (g->match(1))
                name = g->gets();

            handle = gl.assocs(5);

            x1 = gl.assocx(10);
            y1 = gl.assocx(20);
            x2 = gl.assocx(11);
            y2 = gl.assocx(21);

            if (name != 0 && handle != 0 && x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0)
              PaperSpaceLayouts.ChangeLayout(handle,name,*x2 - *x1,*y2 - *y1,*x1,*y1);
              
         }
       else if (strcmp(text,"DICTIONARY") == 0)
         {DXFClipBoundaryInfo *cbi;
            name = gl.assocs(3);
            objectid = gl.assocs(5);
            nextobjectid = gl.assocs(360);
            if (name != 0 && strcmp(name,"ACAD_FILTER") == 0)
              {  for (clipboundarylist.start() ; cbi = (DXFClipBoundaryInfo *) clipboundarylist.next() ; )
                   if (strcmp(cbi->dictionaryobjectid1,objectid) == 0)
                     {  strcpy(cbi->dictionaryobjectid2,nextobjectid);
                        break;
                     }
              }
            else if (name != 0 && strcmp(name,"SPATIAL") == 0)
              {  for (clipboundarylist.start() ; cbi = (DXFClipBoundaryInfo *) clipboundarylist.next() ; )
                   if (strcmp(cbi->dictionaryobjectid2,objectid) == 0)
                     {  strcpy(cbi->spacialfilterobjectid,nextobjectid);
                        break;
                     }
              }
         }
       else if (strcmp(text,"SPATIAL_FILTER") == 0)
         {DXFClipBoundaryInfo *cbi;
            objectid = gl.assocs(5);
            for (clipboundarylist.start() ; cbi = (DXFClipBoundaryInfo *) clipboundarylist.next() ; )
              if (strcmp(cbi->spacialfilterobjectid,objectid) == 0)
                {//  Process the spatial filter
                 long i,*nclipboundary;
                 Point *clipboundary;
                 double *x,*y,a,b,c,d,e,f,x1,y1;

                   if ((nclipboundary = gl.associ(70)) != 0)
                     {  if (*nclipboundary == 2)
                          {  *nclipboundary = 4;
                             clipboundary = new Point[4];
                             x = gl.assocx(10,0);
                             y = gl.assocx(20,0);
                             clipboundary[0].setxyz(x != 0 ? *x : 0.0,y != 0 ? *y : 0.0,0.0);
                             x = gl.assocx(10,1);
                             y = gl.assocx(20,0);
                             clipboundary[1].setxyz(x != 0 ? *x : 0.0,y != 0 ? *y : 0.0,0.0);
                             x = gl.assocx(10,1);
                             y = gl.assocx(20,1);
                             clipboundary[2].setxyz(x != 0 ? *x : 0.0,y != 0 ? *y : 0.0,0.0);
                             x = gl.assocx(10,0);
                             y = gl.assocx(20,1);
                             clipboundary[3].setxyz(x != 0 ? *x : 0.0,y != 0 ? *y : 0.0,0.0);
                          }
                        else 
                          {  clipboundary = new Point[*nclipboundary];
                             for (i = 0 ; i < *nclipboundary ; i++)
                               {  x = gl.assocx(10,i);
                                  y = gl.assocx(20,i);
                                  clipboundary[i].setxyz(x != 0 ? *x : 0.0,y != 0 ? *y : 0.0,0.0);
                               }
                          }

                        //  Transform the clipboundary back to its original position on ure
                        x = gl.assocx(40,0);
                        a = x != 0 ? *x : 1.0;
                        x = gl.assocx(40,1);
                        b = x != 0 ? *x : 0.0;
                        x = gl.assocx(40,3);
                        c = x != 0 ? *x : 0.0;
                        x = gl.assocx(40,4);
                        d = x != 0 ? *x : 0.0;
                        x = gl.assocx(40,5);
                        e = x != 0 ? *x : 1.0;
                        x = gl.assocx(40,7);
                        f = x != 0 ? *x : 0.0;

                        for (i = 0 ; i < *nclipboundary ; i++)
                          {  x1 = a * clipboundary[i].x + b * clipboundary[i].y + c;
                             y1 = d * clipboundary[i].x + e * clipboundary[i].y + f;
                             clipboundary[i].setxyz(x1,y1,0.0);

                             //  Now transform to the new figures location
                             clipboundary[i] = cbi->figure->gettransform()->transform(clipboundary[i]);
                          }


                        cbi->figure->SetClipBoundary(*nclipboundary,clipboundary);

                     }

 
                   break;
                }
         }

       gl.destroy();

    }
  if (groupcode == 0 && strcmp(groups,"ENDSEC") == 0)
    nextgroup();
}

void DXFInFile::readtables(void)
{DXFInGroupList gl;
  while (groupcode == 0 && strcmp(groups,"TABLE") == 0)
    {  nextgroup();
       if (groupcode == 2)
         nextgroup();
       else
         {  printerror(_RCT("Table name expected"));
            break;
         }
       while (groupcode >= 0 && (groupcode != 0 || strcmp(groups,"ENDTAB") != 0))
         {  gl.add(groupcode,grouptype,groupi,groupx,groups);
            nextgroup();
            while (groupcode > 0)
              {  gl.add(groupcode,grouptype,groupi,groupx,groups);
                 nextgroup();
              }
            processtable(&gl);
            gl.destroy();
         }
       if (groupcode == 0 && strcmp(groups,"ENDTAB") == 0)
         nextgroup();
    }
  if (groupcode == 0 && strcmp(groups,"ENDSEC") == 0)
    nextgroup();
}


void DXFInFile::readentities(void)
{DXFInGroupList gl;
  while (groupcode == 0 && strcmp(groups,"ENDSEC") != 0)
    {  gl.add(groupcode,grouptype,groupi,groupx,groups);
       nextgroup();
       while (groupcode >= 0 && groupcode != 0)
         {  gl.add(groupcode,grouptype,groupi,groupx,groups);
            nextgroup();
         }
       processentity(&gl);
       gl.destroy();
    }
  if (groupcode == 0 && strcmp(groups,"ENDSEC") == 0)
    nextgroup();
}

int DXFInFile::read(void)
{ nextgroup();
  while (groupcode >= 0)
    {  while (strlen(groups) > 1 && groups[strlen(groups)-1] == ' ')
         groups[strlen(groups)-1] = 0;
       if (groupcode == 0 && strcmp(groups,"EOF") == 0)
         break;
       if (groupcode == 0 && strcmp(groups,"SECTION") == 0)
         {  nextgroup();
            if (groupcode == 2 && strcmp(groups,"HEADER") == 0)
              {  nextgroup();
                 readheader();
                 // KMJ assuming the header is finished so set the scale
                 if(measurement == 0) // inches
                 {
                     // the default scale is already set for millimeters or inches
                     // if not imperial or engineering units and not inches
                     // scale by the insunits value
                     if((lunits != 4 && lunits != 3) && insunits != 1)
                     {
                         switch(insunits)
                         {
                         case 0: // unitless
                             scale = 1.0; // millimeters
                             break;
                         case 2: // feet
                             scale = 304.8;
                             break;
                         case 3: // miles
                             scale = 1.609e+6;
                             break;
                         case 4: // Millimeters
                             scale = 1.0;
                             break;
                         case 5: // Centimeters
                             scale = 10.0;
                             break;
                         case 6: // Meters
                             scale = 1000.0;
                             break;
                         case 7: // Kilometers
                             scale = 1e+6;
                             break;
                         case 8: // microinch
                             scale = 0.0000254;
                             break;
                         case 9: // Mils (Thou=1/1000inch)
                             scale = 0.0254;
                             break;
                         case 10: // yards
                             scale = 914.4;
                             break;
                         case 11: // Angstroms
                             scale = 1e-7;
                             break;
                         case 12: // Nanometers
                             scale = 1e-6;
                             break;
                         case 13: // microns
                             scale = 0.001;
                             break;
                         case 14: // Decimeters
                             scale = 100.0;
                             break;
                         case 15: // Decameters
                             scale = 10000.0;
                             break;
                         case 16: // Hectometers
                             scale = 100000.0;
                             break;
                         case 17: // Gigameters
                             scale = 1e+12;
                             break;
                         case 18: // Astronimical units
                             scale = 1.496e+14;
                             break;
                         case 19: // light years
                             scale = 9.461e+18;
                             break;
                         case 20: // parsecs
                             scale = 3.086e+19;
                             break;
                         }
                     }
                 }
                 //
              }
            else if (groupcode == 2 && strcmp(groups,"TABLES") == 0)
              {  nextgroup();
                 readtables();
              }
            else if (groupcode == 2 && strcmp(groups,"BLOCKS") == 0)
              {  nextgroup();
                 readblocks();
              }
            else if (groupcode == 2 && strcmp(groups,"ENTITIES") == 0)
              {  clipboundarylist.destroy();
                 modelvisibility.clearandset(0,end_list);
                 papervisibility.clearandset(MaxViews-1,end_list);
                 nextgroup();
                 readentities();
              }
            else if (groupcode == 2 && strcmp(groups,"OBJECTS") == 0)
              {  nextgroup();
                 readobjects();
              }
            else
              {  nextgroup();
                 while (groupcode >= 0 && (groupcode != 0 || strcmp(groups,"ENDSEC") != 0))
                   nextgroup();
                 nextgroup();
              }
         }
       else
         {  printerror(_RCT("End of file, or start of section expected"));  break;
         }
    }
  if (! error)
    {Entity *e;
     View2d *view2d;
     BitMask visible(MaxViews);
     int i,viewno;
     double x,y,w,h;
     char *paperspacename;

       visible.clearall();
       for (i = 0 ; i < MaxViews ; i++)
         if (db.views3d.match(i))
           visible.set(i);

       for (composeddrawingentitylist.start() ;  (e = composeddrawingentitylist.next()) != 0 ; )
          db.geometry.add(e);

       db.header.setvisible(visible);
       
       view2d = db.views2d.match(_RCT("*Paper_Space"));

       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->getvisible()->test(MaxViews-1))
           {  if (view2d == 0 && (paperspacename = PaperSpaceLayouts.GetLayout((char*)"*Paper_Space",&w,&h,&x,&y)) != 0)
                {  if ((viewno = db.getnextviewno()) >= 0)
                     if ((view2d = new View2d(_RCT("*Paper_Space"),w * scale,h * scale,short(viewno))) != NULL)
                       {  db.views2d.add(view2d);
                       }
                }
              if (view2d != 0)
                e->getvisible()->clearandset(view2d->getviewno(),end_list);
           }   
         else if (e->getvisible()->test(0))
           e->setvisible(visible);

       for (i = 0 ; i < MaxViews ; i++)
         if ((view2d = db.views2d.match(i)) != 0 && PaperSpaceLayouts.GetLayout(RC2C(view2d->getname()),&w,&h,&x,&y) != 0)
           view2d->change(_RCT(PaperSpaceLayouts.GetLayout(RC2C(view2d->getname()),&w,&h,&x,&y)),w,h,x,y);

       db.geometry.reverse();

    }

  // KMJ for setting the RealCAD units to match the AutoCAD settings
  if(measurement == 0) // inches
  {
      switch(lunits)
      {
      case 1: // Scientific
      case 2: // Decimal
      case 5: // Factional
      case 3: // Engineering
      default:
          // decimal inches display
          v.setinteger("db::units",2);
          db.setunits(2);
          break;
      case 4: // Architecture
          // (' and ")display
          v.setinteger("db::units",4);
          db.setunits(4);
          break;
      }
  }
  else // metric
  {
      switch(lunits)
      {
      case 1: // Scientific
      case 2: // Decimal
      case 5: // Factional
      case 4: // Architecture
      case 3: // Engineering
      default:
          // decimal metric display
          v.setinteger("db::units",2);
          db.setunits(2);
          break;
          break;
      }
  }
  return ! error;
}

void DXFInFile::printerror(RCCHAR *s)
{ error = 1;
  cadwindow->MessageBox(s,"DXFIn In",MB_ICONSTOP);
}

DXFInConvert::DXFInConvert()
{ blockname = 0;
  blockisxref = 0;
  nvertices = -1;
  nblocks = 0;
  scale = 1.0;
  leadersize = 2.5; 

  pdmode = 0;
  pdsize = 0.0;

  PaperSpaceLayouts.Clear();

}

static void setplotscale(int,void *scale,View3dSurface *vs)
{
  if (typeid(vs) == typeid(class View3dWindow) && ((View3dWindow *)vs)->getoffscreenbitmap() != 0)
    ((View3dWindow *)vs)->getoffscreenbitmap()->displaylist.clear();
  vs->setplotscale(*((double *)scale));
  InvalidateRect(vs->gethwnd(),0,TRUE);
}


void DXFInConvert::processheader(DXFInGroupList *gl)
{DXFInGroup *dxfgroup;

    // KMJ: Note
    // $INSUNITS = default drawing units for inserts; inches, Millimeters etc...
    // $MEASUREMENT sets the drawing units; 0= English; 1= Metric
  gl->reverse();

  for (gl->start() ; (dxfgroup = gl->next()) != 0 ; )
    if (dxfgroup->match(9) && strcmp(dxfgroup->gets(),"$LTSCALE") == 0)
      {  dxfgroup = gl->next();
         if (dxfgroup != 0 && dxfgroup->match(40))
           {  v.setreal("vw::plotscale",*dxfgroup->getx());
              cadwindow->drawcallback(0,dxfgroup->getx(),setplotscale);
           }
      }
    else if (dxfgroup->match(9) && strcmp(dxfgroup->gets(),"$PDMODE") == 0)
      {  dxfgroup = gl->next();
         if (dxfgroup != 0 && dxfgroup->match(70))
           {  pdmode = *dxfgroup->geti();
           }
      }
    else if (dxfgroup->match(9) && strcmp(dxfgroup->gets(),"$PDSIZE") == 0)
      {  dxfgroup = gl->next();
         if (dxfgroup != 0 && dxfgroup->match(40))
           {  pdsize = *dxfgroup->getx();
           }
      }
   // KMJ
    else if (dxfgroup->match(9) && strcmp(dxfgroup->gets(),"$MEASUREMENT") == 0)
      {  dxfgroup = gl->next();
         if (dxfgroup != 0 && dxfgroup->match(70))
           {  measurement = *dxfgroup->geti();
             if(measurement == 0)
                 scale = 25.4; // inches
             else
                 scale = 1.0; // millimeters
           }
      }
    else if (dxfgroup->match(9) && strcmp(dxfgroup->gets(),"$LUNITS") == 0)
      {  dxfgroup = gl->next();
         if (dxfgroup != 0 && dxfgroup->match(70))
           {  lunits = *dxfgroup->geti();
           }
      }
    else if (dxfgroup->match(9) && strcmp(dxfgroup->gets(),"$INSUNITS") == 0)
      {  dxfgroup = gl->next();
         if (dxfgroup != 0 && dxfgroup->match(70))
           {  insunits = *dxfgroup->geti();
           }
      }
    // KMJ

}

void DXFInConvert::processblock(DXFInGroupList *gl)
{char *s,*bname,*xref_path;
 double *x1,*y1,*z1;
  if ((s = gl->assocs(0)) != 0)
    {  if (strcmp(s,"BLOCK") == 0)
         {  if (nblocks == 0)
              {//  Save the current file - it contains colours and line styles 
               CadFile outfile;
                 strcpy(templatefilename,title);  strcat(templatefilename,"/Template.CAD");
                 _mkdir(QString(title).data());
                 if (outfile.open(RCSTRING(templatefilename).data(),_RCT("wb")))
                   {  db.save(&outfile);
                      outfile.close();
                   }
              }
            if ((bname = gl->assocs(2)) != 0 && strcmp(bname,"*MODEL_SPACE") != 0 && strcmp(bname,"*PAPER_SPACE") != 0)
              {  

 
                 blockisxref = gl->associ(70) != 0 && (*gl->associ(70)&4) == 4;     
 
                 if ((blockname = new char [strlen(title)+strlen(bname)+6]) != 0)
                   {  strcpy(blockname,title);  
                      if (blockisxref && strrchr(blockname,'/') != 0)
                      {
                          *strrchr(blockname,'/') = 0;  //  Place XREFS into the same level directory as the original file.
                      }
                      strcat(blockname,"/");
                      strcat(blockname,bname);
                      strcat(blockname,".CAD");
                      for (char *s = blockname ; *s != 0 ; s++)
                        if (*s == '*') *s = '$';
                   }

                 x1 = gl->assocx(10);
                 y1 = gl->assocx(20);
                 z1 = gl->assocx(30);

                 strcpy(BlockName[nblocks],blockname);
                 strcpy(XREFPathName[nblocks],"");
 
                 xref_path = gl->assocs(1);
                 if (strlen(xref_path) > 0)
                 {  strcpy(XREFPathName[nblocks],title);  
                    if (strrchr(XREFPathName[nblocks],'/') != 0)
                    {   *strrchr(XREFPathName[nblocks],'/') = 0;  //  Place XREFS into the same level directory as the original file.
                    }
                    strcat(XREFPathName[nblocks],"/");
                    if (strrchr(xref_path,'.') != 0)
                        strrchr(xref_path,'.')[0] = 0;
                    if (strrchr(xref_path,'/') != 0)
                        strcat(XREFPathName[nblocks],strrchr(xref_path,'/') + 1);
                    else
                        strcat(XREFPathName[nblocks],xref_path);
                    strcat(XREFPathName[nblocks],".CAD");
                    for (char *s = XREFPathName[nblocks] ; *s != 0 ; s++)
                        if (*s == '*') *s = '$';

                 }

                 BlockOrigin[nblocks].setxyz(x1 != 0 ? *x1 : 0.0,y1 != 0 ? *y1 : 0.0,z1 != 0 ? *z1 : 0.0);
                 if (nblocks < MAX_BLOCKS) 
                   nblocks++;

              }
         }
       else if (strcmp(s,"ENDBLK") == 0)
         {CadFile outfile;
          ResourceString rs0(NCMAIN);
          ResourceString rs1(NCMAIN+1);
          EntityList *list;
          Entity *e;

            nblocks++;
            
            if (! blockisxref)
              {  //  For XREFs don't save the block  

                 if (blockname != 0 && ! outfile.open(_RCT(blockname),_RCT("wb")))
                   cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
                 else if (blockname != 0)
                   {
                      db.save(&outfile);
                      outfile.close();

                   }
              }

            if (blockname != 0) 
              {  //  Delete all the existing entities only - rather than a File New.
                 list = db.geometry.getlist(); 
                 for (list->start() ;  (e = list->next()) != 0 ; )
                   if (e->getvisible()->test(0) == 0)
                     {  e->dbdelete();
                        composeddrawingentitylist.add(e);
                        list->GeneralList::del(e);
                     }

                 db.geometry.clear();
              }

            delete blockname;
            blockname = 0;
         }
       else
         processentity(gl);
    }
}


void FixupHatchList(EntityHeader *h,EntityList *list)
{Entity *e1,*e2;
 EntityList l;
 Point start,end,minstart,minend,s,e;
 double dstart,dend,d;
 Line *line1;

  for (list->start() ; (e1 = list->next()) != 0 ; )
    {  //  Make sure there is an entity connected to the start and end of this entity.  
       start = ((LinearEntity *)e1)->start();
       end = ((LinearEntity *)e1)->end();
       dstart = dend = -1.0; 

       l = *list;

       for (l.start() ; (e2 = l.next()) != 0 ; )
         {  s = ((LinearEntity *)e2)->start();
            e = ((LinearEntity *)e2)->end();

            if (e1 != e2)
              {  d = (s - start).length();
                 if (d < dstart || dstart == -1.0)  
                   {  dstart = d;  minstart = s;
                   }
                 d = (e - end).length();
                 if (d < dend || dend == -1.0)  
                   {  dend = d;  minend = e; 
                   }
              } 
 
            d = (e - start).length();
            if (d < dstart || dstart == -1.0)  
              {  dstart = d;  minstart = e;
              }
            d = (s - end).length();
            if (d < dend || dend == -1.0)  
              {  dend = d;  minend = s;
              }

         }

       if (dstart > db.getttoler())
         {  line1 = new Line(*h,start,minstart);
            if (line1 != 0 && line1->getdefined())
              {  db.geometry.add(line1);
                 list->add(line1); 
                 list->start();
              }
         }  

       if (dend > db.getttoler())
         {  line1 = new Line(*h,end,minend);
            if (line1 != 0 && line1->getdefined())
              {  db.geometry.add(line1);
                 list->add(line1); 
                 list->start();
              }
         }  

    }


}


void GetAxes(DXFInGroupList *gl,Point *xa,Point *ya,Point *za)
{double *x3,*y3,*z3;

  x3 = gl->assocx(210); y3 = gl->assocx(220); z3 = gl->assocx(230);
  if (x3 == 0 || y3 == 0 || z3 == 0)
    {  xa->setxyz(1.0,0.0,0.0);
       ya->setxyz(0.0,1.0,0.0);
       za->setxyz(0.0,0.0,1.0);
    }
  else
    {  za->setxyz(*x3,*y3,*z3);
       if (fabs(za->x) < 1.0/64.0 && fabs(za->y) < 1.0/64.0)
         *xa = Point(0.0,1.0,0.0).cross(*za).normalize();
       else
         *xa = Point(0.0,0.0,1.0).cross(*za).normalize();
       *ya = (za->cross(*xa)).normalize();
       *za = (xa->cross(*ya)).normalize();
    }

}
    

void DXFInConvert::processentity(DXFInGroupList *gl)
{static EntityHeader hpolyline;
 EntityHeader h;
 char *s,*textstring,*name;
 long *c,*generationflags,*mjustification,*hjustification,*vjustification,*pspace,*i1,*i2,*i3,*i4;
 double *x1,*y1,*z1,*x2,*y2,*z2,*x3,*y3,*z3,*x4,*y4,*z4,*x5,*y5,*z5,*thickness,*radius,*start,*end,*height,*xscale,*angle,*slant,a,*mtextwidth;
 Point p1,p2,p3,p4,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 char patternfilename[300],txt[100000],newtxt[100000],*s1,*s2,*s3,font[300],label[3000];
 RCCHAR rcpatternfilename[300];
 static Point xa,ya,za,tdir,origin;
 static double pthickness,startwidth,endwidth;
 BitMask visible(MaxViews);
 RCCOLORREF gradientcolour;
 int degree,rational,periodic,nnknots,n,k;
 double *knot,*cweight;
 Point *polygon;
 Curve *curve1;



  gl->reverse();
  s = gl->assocs(8);
  if (s != 0)
    layerlist.loadheader(s,&h,&dxfstyles);
  s = gl->assocs(6);
  if (s != 0)
    h.change(h.getcolour(),h.getlayer(),dxfstyles.match(s),h.getweight());

  x1 = gl->assocx(370);
  if (x1 != 0)
    h.change(h.getcolour(),h.getlayer(),h.getstyle(),db.lineweights.mmtoweight(*x1/100.0));

  if ((c = gl->associ(62)) != 0)
    h.change(int(*c),h.getlayer(),h.getstyle(),h.getweight());
  if ((x1 = gl->assocx(48)) != 0)
    h.setlinestylescale((float)*x1);
  pspace = gl->associ(67);
  if (pspace != 0 && *pspace == 1)
    {  //
       h.setvisible(papervisibility);
    }
  else if ((i1 = gl->associ(60)) != 0 && *i1 == 1)
    h.setvisible(blankvisibility);
  else
    h.setvisible(modelvisibility);

  s = gl->assocs(0);

  if (s != 0 && strcmp(s,"POINT") == 0)
    {PointE *point;
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       if (x1 != 0 && y1 != 0)
         {  p1.setxyz(*x1,*y1,(z1 == 0 ? 0.0 : *z1));
            point = new PointE(h,p1 * scale);
            if (point != 0 && point->getdefined())
              db.geometry.add(point);

            if (pdmode > 1 && pdsize > 0.0)
            {Circle *circle = new Circle(h,p1 * scale,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),pdsize * scale,0.0,acos(-1.0)*2.0);
                db.geometry.add(circle);
            }

         }
    }
  else if (s != 0 && strcmp(s,"LINE") == 0)
    {Line *line1,*line2;
     int xmesh,ymesh;
     BitMask options(32);
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       x2 = gl->assocx(11);  y2 = gl->assocx(21);  z2 = gl->assocx(31);
       if (x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0)
         {  x3 = gl->assocx(210); y3 = gl->assocx(220); z3 = gl->assocx(230);
            za.setxyz((x3 == 0 ? 0.0 : *x3),(y3 == 0 ? 0.0 : *y3),(z3 == 0 ? 1.0 : *z3));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();
            p1.setxyz(*x1,*y1,(z1 == 0 ? 0.0 : *z1));
            p2.setxyz(*x2,*y2,(z2 == 0 ? 0.0 : *z2));
            //h.getvisible()->set(0);
            line1 = new Line(h,p1 * scale,p2 * scale);
            if (line1 != 0 && line1->getdefined())
              {  db.geometry.add(line1);
                 if ((thickness = gl->assocx(39)) != 0 && *thickness != 0.0)
                   {  line2 = new Line(h,(p1 + za * *thickness) * scale,(p2 + za * *thickness) * scale);
                      if (line2 != 0 && line2->getdefined())
                        {  db.geometry.add(line2);
                           RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                           RuledSurface *surface = new RuledSurface(h,line1,line2,xmesh,ymesh,options);
                           if (surface != 0 && surface->getdefined())
                             db.geometry.add(surface);
                        }
                   }
             }
         }
    }
  else if (s != 0 && strcmp(s,"LEADER") == 0)
    {Point p1,p2,direction;
     Line *line1;
     BitMask options(32);
     long *nvertices;
     int i;
     double *x1,leaderscale;

       nvertices = gl->associ(76);
       leaderscale = gl->assocx(1040) != 0 ? *gl->assocx(1040) : 1.0;
       leadersize = 1.0; 
       if (nvertices != 0)
         {  x1 = y1 = z1 = 0;
            for (i = 0 ; i < *nvertices ; i++)
              {  x2 = gl->assocx(10,i);  y2 = gl->assocx(20,i);  z2 = gl->assocx(30,i);
                 if (x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0)
                   {  p1.setxyz(*x1,*y1,(z1 == 0 ? 0.0 : *z1));
                      p2.setxyz(*x2,*y2,(z2 == 0 ? 0.0 : *z2));
                      line1 = new Line(h,p1 * scale,p2 * scale);
                      if (line1 != 0 && line1->getdefined())
                        db.geometry.add(line1);
                      if (i == 1)
                        {  direction = (p2 - p1).normalize();
                           db.geometry.add(new Line(h,p1,p1 + direction * leadersize * leaderscale + direction.cross(Point(0.0,0.0,1.0)) * leadersize * leaderscale / 4.0));
                           db.geometry.add(new Line(h,p1,p1 + direction * leadersize * leaderscale - direction.cross(Point(0.0,0.0,1.0)) * leadersize * leaderscale / 4.0));
                           db.geometry.add(new Line(h,p1 + direction * leadersize * leaderscale + direction.cross(Point(0.0,0.0,1.0)) * leadersize * leaderscale / 4.0,p1 + direction * leadersize * leaderscale - direction.cross(Point(0.0,0.0,1.0)) * leadersize * leaderscale / 4.0));
                        } 
                   }
                 x1 = x2;  y1 = y2;  z1 = z2; 
              }
         }

    }
  else if (s != 0 && strcmp(s,"LWPOLYLINE") == 0)
    {Line *line1,*line2,*line3,*line4;
     int xmesh,ymesh;
     BitMask options(32);
     int i;

       if (gl->assocx(40) != 0)
         startwidth = *gl->assocx(40);
       else
         startwidth = 0.0;
       if (gl->assocx(41) != 0)
         endwidth = *gl->assocx(41);
       else
         endwidth = 0.0;

       if (gl->assocx(43) != 0)
         startwidth = endwidth = *gl->assocx(43);

       x1 = gl->assocx(10,0);  y1 = gl->assocx(20,0);  z1 = gl->assocx(30,0);
       bulge = gl->assocx(42,10,0) == 0 ? 0.0 : *gl->assocx(42,10,0); 
       x2 = gl->assocx(10,1);  y2 = gl->assocx(20,1);  z2 = gl->assocx(30,1);
       i = 2;
       while (i >= 0 && i != 1 && x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0)
         {  x3 = gl->assocx(210); y3 = gl->assocx(220); z3 = gl->assocx(230);
            za.setxyz((x3 == 0 ? 0.0 : *x3),(y3 == 0 ? 0.0 : *y3),(z3 == 0 ? 1.0 : *z3));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();
            p1.setxyz(*x1,*y1,(z1 == 0 ? 0.0 : *z1));
            p2.setxyz(*x2,*y2,(z2 == 0 ? 0.0 : *z2));

            if (bulge == 0.0)
              {  if (startwidth != 0.0 || endwidth != 0.0)
                   {Point o1,o2;
                      o1 = (za.cross(p2 - p1).normalize()) * startwidth/2.0;
                      o2 = (za.cross(p2 - p1).normalize()) * endwidth/2.0;
                      line1 = new Line(h,(p1+o1) * scale,(p2+o2) * scale);
                      line2 = new Line(h,(p2+o2) * scale,(p2-o2) * scale);
                      line3 = new Line(h,(p2-o2) * scale,(p1-o1) * scale);
                      line4 = new Line(h,(p1-o1) * scale,(p1+o1) * scale);
                      if (line1 != 0 && line2 != 0 && line3 != 0 && line4 != 0)
                        {EntityList list;
                         BitMask o(32);

                           o.set(PlaneFilled,1);

                           db.geometry.add(line1);
                           db.geometry.add(line2);
                           db.geometry.add(line3);
                           db.geometry.add(line4);
                           list.add(line1);   
                           list.add(line2);  
                           list.add(line3);  
                           list.add(line4);  

                           Plane *plane = new Plane(h,list,100.0,100.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),rcpatternfilename,o);
                           db.geometry.add(plane);

                        }
                   }
                 else
                   {  line1 = new Line(h,p1 * scale,p2 * scale);
                      if (line1 != 0 && line1->getdefined())
                        {  db.geometry.add(line1);
                           if ((thickness = gl->assocx(39)) != 0 && *thickness != 0.0)
                             {  line2 = new Line(h,(p1 + za * *thickness) * scale,(p2 + za * *thickness) * scale);
                                if (line2 != 0 && line2->getdefined())
                                  {  db.geometry.add(line2);
                                     RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                                     RuledSurface *surface = new RuledSurface(h,line1,line2,xmesh,ymesh,options);
                                     if (surface != 0 && surface->getdefined())
                                       db.geometry.add(surface);
                                  }
                             }
                        }
                   }
              }
            else
              {double r,d,a1,a2;
               Point dir1,dir2,o;
               Circle *circle1,*circle2;
               Line *line1,*line2;
               int xmesh,ymesh;
               BitMask options(32);

                 dir1 = p2 - p1;
                 d = dir1.length();
                 r = (bulge * bulge * d * d + d * d) / 4.0 / bulge / d;
                 dir2 = dir1.cross(za);
                 dir2.normalize();
                 o = (p1 + p2) / 2.0 - dir2 * (r - bulge * d / 2.0);
                 Circle c(o,xa,ya,fabs(r),0.0,M_PI * 2.0);
                 if (bulge > 0.0)
                   {  a1 = c.angle(p1);
                      a2 = c.angle(p2) - a1;
                   }
                 else
                   {  a1 = c.angle(p2);
                      a2 = c.angle(p1) - a1;
                   }
                 while (a2 < 0.0) a2 += 2.0 * M_PI;
 
                 if (startwidth != 0.0 || endwidth != 0.0)
                   {  circle1 = new Circle(h,o * scale,xa,ya,(fabs(r)+startwidth/2.0) * scale,a1,a2);
                      circle2 = new Circle(h,o * scale,xa,ya,(fabs(r)-startwidth/2.0) * scale,a1,a2);
                      line1 = new Line(h,circle1->start(),circle2->start());
                      line2 = new Line(h,circle1->end(),circle2->end());

                      if (circle1 != 0 && circle2 != 0 && line1 != 0 && line2 != 0)
                        {EntityList list;
                         BitMask o(32);

                           o.set(PlaneFilled,1);

                           db.geometry.add(circle1);
                           db.geometry.add(line2);
                           db.geometry.add(circle2);
                           db.geometry.add(line1);
                           list.add(circle1);   
                           list.add(line2);  
                           list.add(circle2);  
                           list.add(line1);  

                           Plane *plane = new Plane(h,list,100.0,100.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),rcpatternfilename,o);
                           db.geometry.add(plane);

                        }

                   }
                 else
                   {  circle1 = new Circle(h,o * scale,xa,ya,fabs(r) * scale,a1,a2);
                      if (circle1 != 0 && circle1->getdefined())
                        {  db.geometry.add(circle1);
                           if ((thickness = gl->assocx(39)) != 0 && *thickness != 0.0)
                             {  circle2 = new Circle(h,(o + za * *thickness) * scale,xa,ya,fabs(r) * scale,a1,a2);
                                if (circle2 != 0 && circle2->getdefined())
                                  {  db.geometry.add(circle2);
                                     RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                                     RuledSurface *surface = new RuledSurface(h,circle1,circle2,xmesh,ymesh,options);
                                     if (surface != 0 && surface->getdefined())
                                       db.geometry.add(surface);
                                  }  
                             }
                        }
                   }
              }

            x1 = x2;  y1 = y2;  z1 = z2;
            x2 = gl->assocx(10,i);  y2 = gl->assocx(20,i);  z2 = gl->assocx(30,i);
            bulge = gl->assocx(42,10,i-1) == 0 ? 0.0 : *gl->assocx(42,10,i-1); 
            if (x2 == 0 && gl->associ(70) != 0 && (*gl->associ(70)&1) == 1)
              {  x2 = gl->assocx(10,0);  y2 = gl->assocx(20,0);  z2 = gl->assocx(30,0);
                 i = 0;
              }
            else
              i++;
         }
    }
  else if (s != 0 && strcmp(s,"WIPEOUT") == 0)
    {Point origin,xaxis,yaxis,p0,p1,p2;
     double *x1,*y1,*z1,width,height;
     int shape,nvertices;
     EntityList list;
     BitMask options(32);
     DXFInGroup *dxfgroup;
     Line *line1;

       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       if (x1 == 0 || y1 == 0 || z1 == 0)
         origin.setxyz(0.0,0.0,0.0);
       else
         origin.setxyz(*x1,*y1,*z1);

       x1 = gl->assocx(11);  y1 = gl->assocx(21);  z1 = gl->assocx(31);
       if (x1 == 0 || y1 == 0 || z1 == 0)
         xaxis.setxyz(1.0,0.0,0.0);
       else
         xaxis.setxyz(*x1,*y1,*z1);

       x1 = gl->assocx(12);  y1 = gl->assocx(22);  z1 = gl->assocx(32);
       if (x1 == 0 || y1 == 0 || z1 == 0)
         yaxis.setxyz(1.0,0.0,0.0);
       else
         yaxis.setxyz(*x1,*y1,*z1);

       x1 = gl->assocx(13);
       width = x1 != 0 ? *x1 : 1.0;
       x1 = gl->assocx(23);
       height = x1 != 0 ? *x1 : 1.0;

       //  Wipeouts appear to be defined from top to bottom - this reverses the image
       origin += yaxis;
       yaxis = -yaxis;


       shape = gl->associ(71) == 0 ? 1 : *gl->associ(71);
       nvertices = gl->associ(91) == 0 ? 1 : *gl->associ(91);


       //  Find the start of the boundary data
       for (gl->start() ; (dxfgroup = gl->next()) != 0 ; )
         if (dxfgroup->match(14))
           break;   

       if (dxfgroup != 0)
         {  p0.x = *dxfgroup->getx() + 0.5;
            dxfgroup = gl->next();
            p0.y = *dxfgroup->getx() + 0.5;
            dxfgroup = gl->next();
            p0.z = 0.0; 
            p1 = p0;
         }
              

       while (dxfgroup != 0)
         {  if (dxfgroup->match(14))
              p2.x = *dxfgroup->getx() + 0.5;
            else
              break;      
            dxfgroup = gl->next();

            if (dxfgroup->match(24))
              p2.y = *dxfgroup->getx() + 0.5;
            else
              break;      
            dxfgroup = gl->next();

            p2.z = 0.0;

            line1 = new Line(h,(origin + xaxis * p1.x + yaxis * p1.y) * scale,(origin + xaxis * p2.x + yaxis * p2.y) * scale);
            if (line1 != 0 && line1->getdefined())
              {  db.geometry.add(line1);
                 list.add(line1); 
              }

            p1 = p2;

         }

       line1 = new Line(h,(origin + xaxis * p1.x + yaxis * p1.y) * scale,(origin + xaxis * p0.x + yaxis * p0.y) * scale);
       if (line1 != 0 && line1->getdefined())
         {  db.geometry.add(line1);
            list.add(line1); 
         }

       options.set(0,1);
       options.set(PlanePatterned,0);
       options.set(PlaneGradientFilled,0);

       Plane *plane = new Plane(h,list,100.0,100.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(255,255,255),_RCT(""),options);
       if (plane != 0 && plane->getdefined())
         {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),0,h.getweight());
            db.geometry.add(plane);
         }



    }
  else if (s != 0 && strcmp(s,"HATCH") == 0)
    {double elevation;
     char name[300];
     int counterclockwise,i,j,solidfill,nloops,nedges,hatchstyle,patterntype,npatterndefinitionlines,pathtype,bulgeflag,closed;
     double hatchscale,angle,x0,y0,x1,y1,x2,y2,bulge,nextbulge,radius,starta,enda;
     double xspacing,yspacing,crossangle,transparency,red,green,blue,transparencyrange;
     RCCOLORREF gradientcolour;
     char patternfilename[300];
     RCCHAR rcpatternfilename[300];
     BitMask options(32);
     DXFInGroup *dxfgroup;
     Point p1,p2,za;
     EntityList list;
     Line *line1;
     Circle *circle1;
     EllipseE *ellipse;
     Transform t,et;


       elevation = gl->assocx(30) == 0 ? 0.0 : *gl->assocx(30);
       GetAxes(gl,&xa,&ya,&za);
       t.tocoordinatesystem(xa,ya,za);
       et.translate(za * elevation);
       t.apply(et);
       

       //  For now, hatching must be drawn in solid line style
       h.change(h.getcolour(),h.getlayer(),0,h.getweight());


       strcpy(name,gl->assocs(2) == 0 ? "" : gl->assocs(2));
       solidfill = gl->associ(70) == 0 ? 0 : *gl->associ(70);
       nloops = gl->associ(91) == 0 ? 0 : *gl->associ(91);

       //  Find the start of the boundary data
       for (gl->start() ; (dxfgroup = gl->next()) != 0 ; )
         if (dxfgroup->match(91))
           break;     

       visible = *h.getvisible();
       h.getvisible()->clearall();

       for (i = 0 ; i < nloops ; i++)
         {  while (dxfgroup != 0 && ! dxfgroup->match(92))
              dxfgroup = gl->next();
            if (dxfgroup == 0)
              break;
            pathtype = *dxfgroup->geti();
            if (i > 0)
              list.add(0);    //  Start a new loop
            if (pathtype & 2)
              {//  Process a polyline boundary
                 dxfgroup = gl->next();
                 if (dxfgroup->match(72))
                   bulgeflag = *dxfgroup->geti();
                 else
                   bulgeflag = 0;
                 dxfgroup = gl->next();
                 if (dxfgroup->match(73))
                   closed = *dxfgroup->geti();
                 else
                   closed = 0;
                 dxfgroup = gl->next();
                 if (dxfgroup->match(93))
                   nedges = *dxfgroup->geti();
                 else
                   nedges = 0;

                 bulge = 0.0;
                 for (j = 0 ; j < nedges + closed ; j++)
                   {  if (j == nedges)
                        {  x2 = x0;
                           y2 = y0;
                        }
                      else
                        {  dxfgroup = gl->next();
                           if (dxfgroup->match(10))
                             x2 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(20))
                             y2 = *dxfgroup->getx();
                           if (((DXFInGroup *)gl->peek())->match(42))
                             {  dxfgroup = gl->next();
                                nextbulge = *dxfgroup->getx();
                             }
                           else
                             nextbulge = 0;

                        }
                      
                      if (j == 0) 
                        {  x0 = x2;
                           y0 = y2;
                        }
                      else
                        {  p1.setxyz(x1,y1,0.0);
                           p2.setxyz(x2,y2,0.0);

                           if (bulge == 0.0)
                             {  line1 = new Line(h,p1 * scale,p2 * scale);
                                line1->move(0,t,0);
                                if (line1 != 0 && line1->getdefined())
                                  {  db.geometry.add(line1);
                                     list.add(line1); 
                                  }
                             }
                           else
                             {double r,d,a1,a2;
                              Point dir1,dir2,o;
                              Circle *circle1;
                              BitMask options(32);

                              dir1 = p2 - p1;
                              d = dir1.length();
                              r = (bulge * bulge * d * d + d * d) / 4.0 / bulge / d;
                              dir2 = dir1.cross(za);
                              dir2.normalize();
                              o = (p1 + p2) / 2.0 - dir2 * (r - bulge * d / 2.0);
                              Circle c(o,xa,ya,fabs(r),0.0,M_PI * 2.0);
                              if (bulge > 0.0)
                                {  a1 = c.angle(p1);
                                   a2 = c.angle(p2) - a1;
                                }
                              else
                                {  a1 = c.angle(p2);
                                   a2 = c.angle(p1) - a1;
                                }
                              while (a2 < 0.0) a2 += 2.0 * M_PI;
                              circle1 = new Circle(h,o * scale,xa,ya,fabs(r) * scale,a1,a2);
                              circle1->move(0,t,0);
                              if (circle1 != 0 && circle1->getdefined())
                                {  db.geometry.add(circle1);
                                   list.add(circle1); 
                                }
                            }
                       }

                     x1 = x2;  y1 = y2;
                     bulge = nextbulge; 
                  } 
              }
            else
              {//  Process an individual entity boundary
                 dxfgroup = gl->next();
                 if (dxfgroup->match(93))
                   nedges = *dxfgroup->geti();
                 else
                   nedges = 0;
  
                 for (j = 0 ; j < nedges ; j++)
                   {  while (dxfgroup != 0 && ! dxfgroup->match(72))
                        dxfgroup = gl->next();
                      if (dxfgroup == 0)
                        break;
 
                      switch (*dxfgroup->geti())
                        {case 1 : //  Line
                           dxfgroup = gl->next();
                           if (dxfgroup->match(10))
                             x1 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(20))
                             y1 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(11))
                             x2 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(21))
                             y2 = *dxfgroup->getx();

                           p1.setxyz(x1,y1,0.0);
                           p2.setxyz(x2,y2,0.0);

                           line1 = new Line(h,p1 * scale,p2 * scale);
                           line1->move(0,t,0);
                           if (line1 != 0 && line1->getdefined())
                             {  db.geometry.add(line1);
                                list.add(line1); 
                             }
                           break;
                         case 2 : //  Arc
                           dxfgroup = gl->next();
                           if (dxfgroup->match(10))
                             x1 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(20))
                             y1 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(40))
                             radius = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(50))
                             starta = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(51))
                             enda = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(73))
                             counterclockwise = *dxfgroup->geti();
                           else
                             counterclockwise = 0;

                           p1.setxyz(x1,y1,0.0);

                           if (! counterclockwise)
                             {  starta = 360.0 - starta;
                                enda = 360.0 - enda;
                             }
                            
                           enda -= starta;
                           circle1 = new Circle(h,p1 * scale,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),radius * scale,starta * M_PI / 180.0,enda * M_PI / 180.0);
                           circle1->move(0,t,0);
                           if (circle1 != 0 && circle1->getdefined())
                             {  db.geometry.add(circle1);
                                list.add(circle1); 
                             }
                           break;
                         case 3 : //  Elliptical Arc
                           dxfgroup = gl->next();
                           if (dxfgroup->match(10))
                             x1 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(20))
                             y1 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(11))
                             x2 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(21))
                             y2 = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(40))
                             radius = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(50))
                             starta = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(51))
                             enda = *dxfgroup->getx();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(73))
                             counterclockwise = *dxfgroup->geti();
                           else
                             counterclockwise = 0;

                           p1.setxyz(x1,y1,0.0);

                           while (enda < starta) enda += 360.0;

                           {EllipseE e(p1 * scale,Point(x2,y2,0.0).normalize(),Point(-y2,x2,0.0).normalize(),Point(x2,y2,0.0).length() * scale,Point(x2,y2,0.0).length() * radius * scale,0.0,acos(-1.0) * 2.0);

                            if (starta < 0.0)
                              starta = e.toparametric(starta  * M_PI / 180.0) - acos(-1.0) * 2.0;
                            else
                              starta = e.toparametric(starta  * M_PI / 180.0);

                            if (enda < 0.0)
                              enda = e.toparametric(enda * M_PI / 180.0) - acos(-1.0) * 2.0;
                            else
                              enda = e.toparametric(enda * M_PI / 180.0);

                           }

                           if (! counterclockwise)
                             {  starta = acos(-1.0) * 2.0 - starta;
                                enda = acos(-1.0) * 2.0 - enda;
                             }
                           enda -= starta;  

                           ellipse = new EllipseE(h,p1 * scale,Point(x2,y2,0.0).normalize(),Point(-y2,x2,0.0).normalize(),Point(x2,y2,0.0).length() * scale,Point(x2,y2,0.0).length() * radius * scale,starta,enda);
                           ellipse->move(0,t,0);
                           if (ellipse != 0 && ellipse->getdefined())
                             {  db.geometry.add(ellipse);
                                list.add(ellipse); 
                             }
                           break;
                         case 4 : //  Spline

                           dxfgroup = gl->next();

                           if (dxfgroup->match(94))
                             degree = *dxfgroup->geti();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(73))
                             rational = *dxfgroup->geti();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(74))
                             periodic = *dxfgroup->geti();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(95))
                             nnknots = *dxfgroup->geti();
                           dxfgroup = gl->next();
                           if (dxfgroup->match(96))
                             n = *dxfgroup->geti();
                           dxfgroup = gl->next();

                           knot = new double[nnknots];
                           cweight = new double[n];
                           polygon = new Point[n];

                           for (k = 0 ; k < nnknots ; k++)
                             if (dxfgroup->match(40))
                               {  knot[k] = *dxfgroup->getx();
                                  dxfgroup = gl->next();
                               }

                           for (k = 0 ; k < nnknots ; k++)
                             knot[k] /= knot[nnknots-1];  

                           for (k = 0 ; k < n ; k++)
                             {  if (dxfgroup->match(10))
                                  {  polygon[k].x = *dxfgroup->getx();
                                     dxfgroup = gl->next();
                                  }
                                if (dxfgroup->match(20))
                                  {  polygon[k].y = *dxfgroup->getx();
                                     dxfgroup = gl->next();
                                  }
                                polygon[k].z = 0.0;  
                                if (dxfgroup->match(42))
                                  {  cweight[k] = *dxfgroup->getx();
                                     dxfgroup = gl->next();
                                  }
                                else
                                  cweight[k] = 1.0; 
                             } 

                           curve1 = new Curve(h,n,degree,polygon,knot,cweight,options);
                           curve1->move(0,t,0);
                           if (curve1 != 0 && curve1->getdefined())
                             {  db.geometry.add(curve1);
                                list.add(curve1); 
                             }

                           break;
                        }
                   }
              } 
         }

       FixupHatchList(&h,&list);
       Loops loops(1);
       loops.create(&list,1);
       list = loops.getloops();

       h.setvisible(visible);

       Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,rcpatternfilename,&options);
       options.set(0,solidfill);
       options.set(PlanePatterned,0);
       options.set(PlaneGradientFilled,0);
       crossangle = 0.0;
       strcpy(patternfilename,"");

       hatchstyle = gl->associ(75) == 0 ? 0 : *gl->associ(75);   //  0 - Odd parity,  1 - Outermost only, 2 - Entire area
       patterntype = gl->associ(76) == 0 ? 0 : *gl->associ(76);  //  0 - Userdefined, 1 - Predefined, 2 = Custom
       angle = gl->assocx(52) == 0 ? 0 : *gl->assocx(52);
       hatchscale = gl->assocx(41) == 0 ? 0 : *gl->assocx(41);
       npatterndefinitionlines =  gl->associ(78) == 0 ? 0 : *gl->associ(78); 
       //  Read pattern data
       for (gl->start() ; (dxfgroup = gl->next()) != 0 ; )
         if (dxfgroup->match(78))
           break;     

       for (i = 0 ; i < npatterndefinitionlines ; i++)
         {  while (dxfgroup != 0 && ! dxfgroup->match(53))
              dxfgroup = gl->next();
            if (dxfgroup == 0)
              break;
            if (i == 0)
              angle = *dxfgroup->getx();
            else
              crossangle = *dxfgroup->getx();
            dxfgroup = gl->next();  //  Discard line base point
            dxfgroup = gl->next();
            dxfgroup = gl->next();  //  Get spacing
            x1 = *dxfgroup->getx(); 
            dxfgroup = gl->next();
            x2 = *dxfgroup->getx(); 
            if (i == 0)
              xspacing = sqrt(x1 * x1 + x2 * x2); 
            else
              yspacing = sqrt(x1 * x1 + x2 * x2); 

         }

       if (gl->associ(450) != 0 && *gl->associ(450) == 1 && gl->associ(453) != 0 && *gl->associ(453) == 2 && gl->associ(421) != 0)
         {int i,imin,c;
          double d,dmin;

            gradientcolour = ((*gl->associ(421) >> 16)&0xFF) | (*gl->associ(421) &0x0000FF00) | ((*gl->associ(421)<<16) &0x00FF0000);
            c = ((*gl->associ(421,1) >> 16)&0xFF) | (*gl->associ(421,1) &0x0000FF00) | ((*gl->associ(421,1)<<16) &0x00FF0000);

            imin = h.getcolour();
            dmin = ((c & 0xFF) - db.colourtable[imin].red)*((c & 0xFF) - db.colourtable[imin].red) + (((c>>8) & 0xFF) - db.colourtable[imin].green) * (((c>>8) & 0xFF) - db.colourtable[imin].green) + (((c>>16) & 0xFF) - db.colourtable[imin].blue)*(((c>>16) & 0xFF) - db.colourtable[imin].blue);
            for (i = 0 ; i < 256 ; i++)
              {//  Find the closest colour 
                 d = ((c & 0xFF) - db.colourtable[i].red)*((c & 0xFF) - db.colourtable[i].red) + (((c>>8) & 0xFF) - db.colourtable[i].green) * (((c>>8) & 0xFF) - db.colourtable[i].green) + (((c>>16) & 0xFF) - db.colourtable[i].blue)*(((c>>16) & 0xFF) - db.colourtable[i].blue);
                 if (d < dmin)
                   {  imin = i;
                      dmin = d;
                   }
              }
            h.change(imin,h.getlayer(),0,h.getweight());
            

            options.set(PlaneNoFill,0);
            options.set(PlanePatterned,0);
            options.set(PlaneFilled,0);
            options.set(PlaneBitmapFilled,0);
            options.set(PlaneGradientFilled,1);

            if (gl->assocs(470) != 0 && _stricmp(gl->assocs(470),"INVSPHERICAL") == 0)
              {  options.set(PlaneGradientStyle1,1);
                 options.set(PlaneGradientStyle2,0);
                 options.set(PlaneGradientStyle3,0);
                 options.set(PlaneGradientStyle4,0); 
              }
            else if (gl->assocs(470) != 0 && _stricmp(gl->assocs(470),"CYLINDER") == 0)
              {  options.set(PlaneGradientStyle1,0);
                 options.set(PlaneGradientStyle2,1);
                 options.set(PlaneGradientStyle3,0);
                 options.set(PlaneGradientStyle4,1); 
              }
            else
              {  options.set(PlaneGradientStyle1,0);
                 options.set(PlaneGradientStyle2,0);
                 options.set(PlaneGradientStyle3,0);
                 options.set(PlaneGradientStyle4,0); 
              }

         }


       Plane *plane = new Plane(h,list,xspacing,yspacing,angle,crossangle,transparency,0.0,0.0,0.0,0.0,gradientcolour,rcpatternfilename,options);
       if (plane != 0 && plane->getdefined())
         {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),0,h.getweight());
            db.geometry.add(plane);
         }

    }
  else if (s != 0 && strcmp(s,"SPLINE") == 0)
    {Curve *curve1,*curve2;
     int i,degree,n,nknots;
     BitMask options(32);
     Point *polygon;
     double *knots,a,b;
     int xmesh,ymesh;

       if (gl->associ(74) != 0)
         n = *gl->associ(74);
       else
         n = 0;

       if (gl->associ(73) != 0 && *gl->associ(73) > n)
         n = *gl->associ(73);                                 

       if (n != 0 && gl->associ(71) != 0)
         {  degree = *gl->associ(71);
            nknots = *gl->associ(72);
            if (nknots != 0)
              {  knots = new double[nknots];
                 for (i = 0 ; i < nknots ; i++)
                   {  x1 = gl->assocx(40,i); 
                      if (x1 == 0)
                        {  nknots = 0;
                           delete knots;
                           knots = 0;
                           break;
                        }
                      else
                        knots[i] = *x1;
                   }

                 if (knots != 0)
                   {  a = knots[0];
                      b = knots[nknots-1];
                      for (i = 0 ; i < nknots ; i++)
                        knots[i] = (knots[i] - a) / (b - a);
                   }  
 
               }
            else
              knots = 0;  
             
            polygon = new Point[n];
            for (i = 0 ; i < n ; i++)
              {  x1 = gl->assocx(10,i);  y1 = gl->assocx(20,i);  z1 = gl->assocx(30,i);
                 if (x1 == 0)
                   {  x1 = gl->assocx(11,i);  y1 = gl->assocx(21,i);  z1 = gl->assocx(31,i);
                   }
                 polygon[i].setxyz(x1 != 0 ? *x1 : 0.0,y1 != 0 ? *y1 : 0.0,z1 != 0 ? *z1 : 0.0);
                 polygon[i] *= scale;
              }
            curve1 = new Curve(h,n,degree,polygon,knots,0,options);
            if (curve1 != 0 && curve1->getdefined())
              {  db.geometry.add(curve1);
                 if ((thickness = gl->assocx(39)) != 0 && *thickness != 0.0)
                   {  x3 = gl->assocx(210); y3 = gl->assocx(220); z3 = gl->assocx(230);
                      za.setxyz((x3 == 0 ? 0.0 : *x3),(y3 == 0 ? 0.0 : *y3),(z3 == 0 ? 1.0 : *z3));
                      for (i = 0 ; i < n ; i++)
                        polygon[i] += za * *thickness;

                      curve2 = new Curve(h,n,degree,polygon,knots,0,options);
                      if (curve1 != 0 && curve1->getdefined())
                        {  db.geometry.add(curve2);
                           RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                           RuledSurface *surface = new RuledSurface(h,curve1,curve2,xmesh,ymesh,options);
                           if (surface != 0 && surface->getdefined())
                             db.geometry.add(surface);
                        }
                   }
             }
         }
    }
  else if (s != 0 && strcmp(s,"SOLID") == 0 || strcmp(s,"3DFACE") == 0)
    {Line *line;
     Plane *plane;
     EntityList list;
     double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
     BitMask options(32);
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       x2 = gl->assocx(11);  y2 = gl->assocx(21);  z2 = gl->assocx(31);
       x3 = gl->assocx(12);  y3 = gl->assocx(22);  z3 = gl->assocx(32);
       x4 = gl->assocx(13);  y4 = gl->assocx(23);  z4 = gl->assocx(33);
       if (x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0 &&
           x3 != 0 && y3 != 0 && x4 != 0 && y4 != 0)
         {  x5 = gl->assocx(210);  y5 = gl->assocx(220);  z5 = gl->assocx(230);
            za.setxyz((x5 == 0 ? 0.0 : *x5),(y5 == 0 ? 0.0 : *y5),(z5 == 0 ? 1.0 : *z5));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();

            p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1);
            p2 = xa * *x2 + ya * *y2 + za * (z2 == 0 ? 0.0 : *z2);

            if (strcmp(s,"SOLID") == 0)
              {  p3 = xa * *x4 + ya * *y4 + za * (z4 == 0 ? 0.0 : *z4);
                 p4 = xa * *x3 + ya * *y3 + za * (z3 == 0 ? 0.0 : *z3);
              }
            else
              {  p3 = xa * *x3 + ya * *y3 + za * (z3 == 0 ? 0.0 : *z3);
                 p4 = xa * *x4 + ya * *y4 + za * (z4 == 0 ? 0.0 : *z4);
              }
            visible = *db.header.getvisible();
            db.header.getvisible()->clearall();
            if ((line = new Line(h,p1 * scale,p2 * scale)) != 0 && line->getdefined())
              {  db.geometry.add(line);
                 list.add(line);
              }
            if ((line = new Line(h,p2 * scale,p3 * scale)) != 0 && line->getdefined())
              {  db.geometry.add(line);
                 list.add(line);
              }
            if ((p4-p3).length() > db.getptoler())
              {  if ((line = new Line(h,p3 * scale,p4 * scale)) != 0 && line->getdefined())
                   {  db.geometry.add(line);
                      list.add(line);
                   }
                 if ((line = new Line(h,p4 * scale,p1 * scale)) != 0 && line->getdefined())
                   {  db.geometry.add(line);
                      list.add(line);
                   }
              }
            else
              {  if ((line = new Line(h,p3 * scale,p1 * scale)) != 0 && line->getdefined())
                   {  db.geometry.add(line);
                      list.add(line);
                   }
              }
            db.header.setvisible(visible);

            Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,rcpatternfilename,&options);
            if (strcmp(db.layernames.getlayername(h.getlayer()),"ELEMENT 3D") == 0 || strcmp(s,"3DFACE") == 0)
              options.set(PlaneNoFill);
            else
              options.set(PlaneFilled);
            
            plane = new Plane(h,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,rcpatternfilename,options);
            if (plane != 0 && plane->getdefined())
              {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                 db.geometry.add(plane);
              }
         }
    }
  else if (s != 0 && strcmp(s,"ARC") == 0)
    {Circle *circle1,*circle2;
     int xmesh,ymesh;
     BitMask options(32);
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       radius = gl->assocx(40);  start = gl->assocx(50);  end = gl->assocx(51);
       if (x1 != 0 && y1 != 0 && radius != 0 && start != 0 && end != 0)
         {  x2 = gl->assocx(210);  y2 = gl->assocx(220);  z2 = gl->assocx(230);
            za.setxyz((x2 == 0 ? 0.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 1.0 : *z2));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();
            p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1);
            *end -= *start;
            if (*end < 0) *end += 360.0;
            circle1 = new Circle(h,p1 * scale,xa,ya,*radius * scale,*start * M_PI / 180.0,*end * M_PI / 180.0);
            if (circle1 != 0 && circle1->getdefined())
              {  db.geometry.add(circle1);
                 if ((thickness = gl->assocx(39)) != 0 && *thickness != 0.0)
                   {  circle2 = new Circle(h,(p1 +za * *thickness) * scale,xa,ya,*radius * scale,*start * M_PI / 180.0,*end * M_PI / 180.0);
                      if (circle2 != 0 && circle2->getdefined())
                        {  db.geometry.add(circle2);
                           RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                           RuledSurface *surface = new RuledSurface(h,circle1,circle2,xmesh,ymesh,options);
                           if (surface != 0 && surface->getdefined())
                             db.geometry.add(surface);
                        }
                   }
             }
         }
    }
  else if (s != 0 && strcmp(s,"ELLIPSE") == 0)
    {EllipseE *ellipse1,*ellipse2;
     int xmesh,ymesh;
     BitMask options(32);
     double major,minor;
 
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       x2 = gl->assocx(11);  y2 = gl->assocx(21);  z2 = gl->assocx(31);
       radius = gl->assocx(40);  start = gl->assocx(41);  end = gl->assocx(42);
       if (x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0 && radius != 0 && start != 0 && end != 0)
         {  x3 = gl->assocx(210);  y3 = gl->assocx(220);  z3 = gl->assocx(230);
            za.setxyz((x3 == 0 ? 0.0 : *x3),(y3 == 0 ? 0.0 : *y3),(z3 == 0 ? 1.0 : *z3));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();

            //p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1);
            p1.setxyz(*x1,*y1,z1 == 0 ? 0.0 : *z1);
            
            xa.setxyz(*x2,*y2,(z2 == 0 ? 0.0 : *z2));
            major = xa.length();
            minor = major * *radius;
            xa.normalize();
            ya = za.cross(xa);
 
            *end -= *start;
            if (*end < 0) *end += M_PI * 2.0;
            ellipse1 = new EllipseE(h,p1 * scale,xa,ya,major * scale,minor * scale,*start,*end);
            if (ellipse1 != 0 && ellipse1->getdefined())
              {  db.geometry.add(ellipse1);
                 if ((thickness = gl->assocx(39)) != 0 && *thickness != 0.0)
                   {  ellipse2 = new EllipseE(h,p1 * scale + za * *thickness,xa,ya,major * scale,minor * scale,*start,*end);
                      if (ellipse2 != 0 && ellipse2->getdefined())
                        {  db.geometry.add(ellipse2);
                           RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                           RuledSurface *surface = new RuledSurface(h,ellipse1,ellipse2,xmesh,ymesh,options);
                           if (surface != 0 && surface->getdefined())
                             db.geometry.add(surface);
                        }
                   }
             }
         }
    }
  else if (s != 0 && strcmp(s,"CIRCLE") == 0)
    {Circle *circle1,*circle2;
     int xmesh,ymesh;
     BitMask options(32);
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       radius = gl->assocx(40);
       if (x1 != 0 && y1 != 0 && radius != 0)
         {  x2 = gl->assocx(210); y2 = gl->assocx(220); z2 = gl->assocx(230);
            za.setxyz((x2 == 0 ? 0.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 1.0 : *z2));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();
            p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1);
            circle1 = new Circle(h,p1 * scale,xa,ya,*radius * scale,0.0,M_PI * 2.0);
            if (circle1 != 0 && circle1->getdefined())
              {  db.geometry.add(circle1);
                 if ((thickness = gl->assocx(39)) != 0 && *thickness != 0.0)
                   {  circle2 = new Circle(h,(p1 +za * *thickness) * scale,xa,ya,*radius * scale,0.0,M_PI * 2.0);
                      if (circle2 != 0 && circle2->getdefined())
                        {  db.geometry.add(circle2);
                           RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                           RuledSurface *surface = new RuledSurface(h,circle1,circle2,xmesh,ymesh,options);
                           if (surface != 0 && surface->getdefined())
                             db.geometry.add(surface);
                        }
                   }
              }
         }
    }
  else if (s != 0 && (strcmp(s,"TEXT") == 0 || strcmp(s,"ATTRIB") == 0 || strcmp(s,"ATTDEF") == 0))
    {Text *text;
     BitMask options(32);
     int underline;
     char *prompt,*tag,*l;

       height = gl->assocx(40);
       angle = gl->assocx(50);
       xscale = gl->assocx(41);
       slant = gl->assocx(51);
       textstring = gl->assocs(1);
       prompt = gl->assocs(3);
       tag = gl->assocs(2);
       generationflags = gl->associ(71);
       hjustification = gl->associ(72);
       vjustification = gl->associ(73);

       if (hjustification != 0 && *hjustification != 0 ||
           vjustification != 0 && *vjustification != 0)
         {  x1 = gl->assocx(11);  y1 = gl->assocx(21);  z1 = gl->assocx(31);
         }
       else
         {  x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
         }

       if (x1 != 0 && y1 != 0 && height != 0)
         {  x2 = gl->assocx(210); y2 = gl->assocx(220); z2 = gl->assocx(230);
            za.setxyz((x2 == 0 ? 0.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 1.0 : *z2));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();
            p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1);

            if (hjustification != 0 && *hjustification == 1)
              options.set(1);
            else if (hjustification != 0 && *hjustification == 2)
              options.set(2);
            else if (hjustification != 0 && *hjustification == 4 && (vjustification == 0 || *vjustification == 0))
              {  options.set(3);  options.set(1);
              }
            else if (hjustification != 0 && *hjustification == 5)
              {  x1 = gl->assocx(11);  y1 = gl->assocx(21);  z1 = gl->assocx(31);
                 x2 = gl->assocx(10);  y2 = gl->assocx(20);  z2 = gl->assocx(30);
                 
                 if (x1 != 0 && x2 != 0 && y1 != 0 && y2 != 0)
                   p1 = xa * (*x1 + *x2) / 2.0 + ya * (*y1 + *y2) / 2.0 + za * (z1 == 0 ? 0.0 : *z1);

                 options.set(1);

              }

            if (vjustification != 0 && *vjustification == 2)
              options.set(3);
            else if (vjustification != 0 && *vjustification == 3)
              options.set(4);

            if (slant != 0 && *slant != 0)
              options.set(7);
            if (generationflags != 0 && (*generationflags & 2) == 2)
              xa = -xa;
            if (generationflags != 0 && (*generationflags & 4) == 4)
              ya = -ya;


            // Interpret the text
            s1 = textstring;
            s2 = newtxt;
            memset(newtxt,0,sizeof(newtxt));
            while (*s1!=0)
              {
                if (*s1 == '\\' && (*(s1 +1) == 'M' || *(s1 +1) == 'U'))
                   {//  Start of advanced processing
                    WCHAR unicode;
                    char hex[5],*ep;
                    BOOL used;
                    int i;

                      hex[0] = s1[3];
                      hex[1] = s1[4];
                      hex[2] = s1[5];
                      hex[3] = s1[6];
                      hex[4] = 0;
                     
                      //unicode = (WCHAR)strtol(hex,&ep,16);
                      //s2 += WideCharToMultiByte(CP_ACP,0,&unicode,1,s2,5,0,&used);

                      for (i = 0 ; *s1 != 0 && i < 7 ; i++)
                        s1++;  //  discard the unicode character

                   }
                 else if (*s1 == '^' && *(s1+1) == 'I')
                   {  s1 += 2;
                   }
                 else if (*s1 == '%' && *(s1+1) == '%')
                   {  s1 += 2;
                      if (*s1 == 'C' || *s1 == 'c')
                        {  *s2++ = (char)216;
                           s1++;
                        }
                      else if (*s1 == 'D' || *s1 == 'd')
                        {  *s2++ = (char)176;
                           s1++;
                        }
                      else if (*s1 == 'u')
                        {  underline = 1;
                           s1++;
                        }
                      else if (*s1 == 'O' || *s1 == 'o')
                        {  //  overline not supported
                           s1++;
                        }
                      else if (*s1 == 'P' || *s1 == 'p' || *s1 == '%')
                        {  *s2++ = '%';
                           s1++; 
                        }
                      else if (isdigit(*s1) && isdigit(*(s1+1)) && isdigit(*(s1+2)))
                        {
                           //*s2++ = (*s1 - '0') * 100 + (*(s1+1) - '0') * 10 + (*(s1+2) - '0');
                           s1 += 3;  
                        }
                   }
                 else
                   *s2++ = *s1++;
              }
            *s2 = 0;


            if (strncmp(newtxt,"%%U",3) == 0)
              {  s2 = newtxt + 3;
                 options.set(6,1);
              }
            else
              s2 = newtxt;
              
 
            if (prompt != 0 && tag != 0 && strcmp(s,"TEXT") != 0)
              {  sprintf(label,"%s (%s)",prompt,tag);
                 if(strlen(label) > 0)
                    l = label;
                 else
                    l= 0;
              }
            else
              l = 0;

            text = new Text(h,(v.getstring("tx::font") != 0 && strlen(v.getstring("tx::font")) > 0 ? v.getstring("tx::font") : _RCT("Arial")),
                              p1 * scale,xa,ya,
                              (xscale == 0 ? 0.0 : *height * *xscale) * scale * 1.5 * 0.0,
                              *height * scale * 1.5,0.0,(angle == 0 ? 0.0 : *angle),0,
                              CHAR2RCCHR(s2),l == 0 ? (RCCHAR*)0 : CHAR2RCCHR(l),options);
            if (text != 0 && text->getdefined())
              db.geometry.add(text);

            if (strcmp(s,"ATTRIB") == 0)
              {
                 //char *at = new char[7];
                 RCCHAR *rcat = new RCCHAR[7];
                 strcpy(rcat,"ATTRIB");
                 text->addat(rcat);
              }

         }
     }
  else if (s != 0 && strcmp(s,"MTEXT") == 0)
    {Text *text;
     BitMask options(32);
     int i,underline;

       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       height = gl->assocx(40);
       angle = gl->assocx(50);
       mtextwidth = gl->assocx(41);
       slant = gl->assocx(51);
       textstring = gl->assocs(1);

       underline = 0;
       mjustification = gl->associ(71);
       if (x1 != 0 && y1 != 0 && height != 0)
         {  x2 = gl->assocx(210); y2 = gl->assocx(220); z2 = gl->assocx(230);
            za.setxyz((x2 == 0 ? 0.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 1.0 : *z2));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();

            p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1);
            p1 = Point(*x1,*y1,z1 == 0 ? 0.0 : *z1);

            if (mjustification != 0 && (*mjustification == 1 || *mjustification == 4 || *mjustification == 7))
              {//  Nothing
              }
            else if (mjustification != 0 && (*mjustification == 2 || *mjustification == 5 || *mjustification == 8))
              options.set(1);
            else if (mjustification != 0 && (*mjustification == 3 || *mjustification == 6 || *mjustification == 9))
              {  options.set(2);
              }
            if (mjustification != 0 && (*mjustification == 7 || *mjustification == 8 || *mjustification == 9))
              {//  Nothing
              }
            else if (mjustification != 0 && (*mjustification == 4 || *mjustification == 5 || *mjustification == 6))
              options.set(3);
            else if (mjustification != 0 && (*mjustification == 1 || *mjustification == 2 || *mjustification == 3))
              {  options.set(4);
              }
            if (slant != 0 && *slant != 0)
              options.set(7);

            x2 = gl->assocx(11); y2 = gl->assocx(21); z2 = gl->assocx(31);
            tdir.setxyz((x2 == 0 ? 1.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 0.0 : *z2));

            strcpy(txt,"");
            for (i = 0 ; gl->assocs(3,i) != 0 ; i++)
              strcat(txt,gl->assocs(3,i));
            if (gl->assocs(1) != 0)
              strcat(txt,gl->assocs(1));

            if (angle != 0)
              a = *angle;
            else
              {  a = acos(tdir.dot(xa)) * 180.0 / acos(-1.0);
                 if (tdir.cross(xa).dot(za) > 0.001)
                   a = -a; 
              } 

            strcpy(font,v.getstring("tx::font") != 0 && strlen(v.getstring("tx::font")) > 0 ? v.getstring("tx::font") : _RCT("Arial"));

            // Interpret the text
            s1 = txt;
            s2 = newtxt;
            memset(newtxt,0,sizeof(newtxt));
            while (*s1!=0)
              {  if (*s1 == '{' || *s1 == '}')
                   {  //  Ignore  the brackets for now
                      s1++;
                   }
                 else if (*s1 == '\\')
                   {//  Start of advanced processing
                      s1++;
                      if (*s1 == 'L')
                        {  s1++;
                        } 
                      else if (*s1 == 'f')
                        {  //  This is a truetype font selection 
                           s1++; 

                           //  Get the font name
                           s3 = font;
                           while (*s1 != '|' && *s1 != ';' && *s1 != 0)
                             *s3++ = *s1++;
                           *s3 = 0;
                           if (*s1 == '|')
                             s1++;

                           //  Get the bold, italic and font size if specified.
                           while (*s1 != ';' && *s1 != 0)
                             {  if (*s1 == 'b')
                                  options.set(5,s1[1] != '0');
                                else if (*s1 == 'i')
                                  options.set(7,s1[1] != '0');
                                else if (*s1 == 'p')
                                  {  //options.set(13,1);
                                     //*height = atof(s1+1) / scale / 1.5; 
                                  }
                                s1++;
                             }
                           if (*s1 == ';')
                             s1++;
                        }
                      else if (*s1 == 'F' || *s1 == 'A' || *s1 == 'C' || *s1 == 'T' || *s1 == 'W' || *s1 == 'H' || *s1 == 'S' || *s1 == 'p')
                        {  //  Ingore the font file selection, and the text size adjustment
                           while (*s1 != ';' && *s1 != 0) s1++;
                           if (*s1 == ';')
                             s1++;
                        }
                      else if (*s1 == 'P')
                        {  s1++;
                           *s2++ = '\r';  *s2++ = '\n';
                        }
                      else if (*s1 == 'M' || *s1 == 'U')
                        {  s1 += 5;  //  discard the character
                           *s2 = '?';  
                        }
                      else
                        s1++;  // Ignore any other escapes
                   }
                 else if (*s1 == '^' && *(s1+1) == 'I')
                   {  s1 += 2;
                   }
                 else if (*s1 == '%' && *(s1+1) == '%')
                   {  s1 += 2;
                      if (*s1 == 'C' || *s1 == 'c')
                        {  *s2++ = (char)216;
                           s1++;
                        }
                      else if (*s1 == 'D' || *s1 == 'd')
                        {  *s2++ = (char)176;
                           s1++;
                        }
                      else if (*s1 == 'u')
                        {  underline = 1;
                           s1++;
                        }
                      else if (*s1 == 'O' || *s1 == 'o')
                        {  //  overline not supported
                           s1++;
                        }
                      else if (*s1 == 'P' || *s1 == 'p' || *s1 == '%')
                        {  *s2++ = '%';
                           s1++; 
                        }
                      else if (isdigit(*s1) && isdigit(*(s1+1)) && isdigit(*(s1+2)))
                        {
                          //*s2++ = (*s1 - '0') * 100 + (*(s1+1) - '0') * 10 + (*(s1+2) - '0');
                           s1 += 3;  
                        }
                   }
                 else
                   *s2++ = *s1++;
              }
            *s2 = 0;

            if (mtextwidth != 0 && *mtextwidth > 0)
              {int i,ll,l;
                 s1 = txt;
                 s2 = newtxt;
                 ll = 0;
                 l = strlen(newtxt);
                 memset(txt,0,sizeof(txt));
                 for (i = 0 ; i < l ; i++)
                   {  if (ll > *mtextwidth / *height && newtxt[i] == ' ')
                        {  *s1++ = '\r';  
                           *s1++ = '\n';  
                           ll = 0; 
                        } 
                      else
                        {  *s1++ = newtxt[i];
                           ll++;
                           if (newtxt[i] == 10 || newtxt[i] == 13)
                             ll = 0;
                        }
                   }
                 *s1 = 0;
              }
            else
              strcpy(txt,newtxt);
             
            text = new Text(h,CHAR2RCCHR(font),p1 * scale,xa,ya,0.0,
                              *height * scale * 1.5,0.0,a,0,
                              CHAR2RCCHR(txt),(RCCHAR*)0,options);
            if (text != 0 && text->getdefined())
              db.geometry.add(text);

         }
     }
  else if (s != 0 && strcmp(s,"INSERT") == 0)
    {BitMask options(32);
     char *blockname;
     int i,j,block_index;
       name = gl->assocs(2);
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       if (x1 != 0 && y1 != 0 && name != 0 && (blockname = new char [strlen(title) + strlen(name) + 6]) != 0)
         {  
            block_index = -1;
            for (j = 0 ; block_index == -1 && j < 2 ; j++)
            {   strcpy(blockname,title);  
                if (j == 1 && strrchr(blockname,'/') != 0)
                    strrchr(blockname,'/')[0] = 0;  //  Look for the XREF name instead
                strcat(blockname,"/");
                strcat(blockname,name);
                strcat(blockname,".CAD");
                for (char *s = blockname ; *s != 0 ; s++)
                    if (*s == '*') *s = '$';

                for (i = 0 ; i < nblocks ; i++)
                if (_stricmp(blockname,BlockName[i]) == 0)
                    {  block_index = i;
                        if (strlen(XREFPathName[i]) > 0)
                        {   delete [] blockname;
                            blockname = new char[strlen(XREFPathName[i]) + 1];
                            strcpy(blockname,XREFPathName[i]);
                        }
                       break;
                    }
            }

            x2 = gl->assocx(210); y2 = gl->assocx(220); z2 = gl->assocx(230);
            za.setxyz((x2 == 0 ? 0.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 1.0 : *z2));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();
            p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1);
            x2 = gl->assocx(41);  y2 = gl->assocx(42);  z2 = gl->assocx(43);
            z3 = gl->assocx(50);
            p2.setxyz((x2 == 0 ? 1.0 : *x2),(y2 == 0 ? 1.0 : *y2),(z2 == 0 ? 1.0 : *z2));
            p3.setxyz(0.0,0.0,(z3 == 0 ? 0.0 : *z3));

            if (block_index >= 0)
                p1 -= Point(BlockOrigin[block_index].x*p2.x,BlockOrigin[block_index].y*p2.y,BlockOrigin[block_index].z*p2.z);

            options.set(FigureOriginalLayer);
            options.set(FigureOriginalColour);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight); 
            //h.getvisible()->set(0);
            Figure *figure = new Figure(h,_RCT(blockname),_RCT(""),p1 * scale,xa,ya,p2,p3,Point(0.0,0.0,0.0),0,0,0,NULL,options);
            if (figure != 0 && figure->getdefined())
              db.geometry.add(figure);
            delete blockname;

            //  Save the clip boundary info for later if necessary
            if (gl->assocs(360) != 0)
              clipboundarylist.add(new DXFClipBoundaryInfo(figure,gl->assocs(360)));

         }
    }
  else if (s != 0 && strcmp(s,"DIMENSION") == 0)
    {BitMask options(32);
     char *blockname;
       name = gl->assocs(2);
       if (name != 0 && (blockname = new char [strlen(title) + strlen(name) + 6]) != 0)
         {  strcpy(blockname,title);  strcat(blockname,"/");
            strcat(blockname,name);
            strcat(blockname,".CAD");
            for (char *s = blockname ; *s != 0 ; s++)
              if (*s == '*') *s = '$';
            x2 = gl->assocx(210); y2 = gl->assocx(220); z2 = gl->assocx(230);
            za.setxyz((x2 == 0 ? 0.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 1.0 : *z2));
            if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
              xa = yaxis.cross(za).normalize();
            else
              xa = zaxis.cross(za).normalize();
            ya = za.cross(xa).normalize();
            za = xa.cross(ya).normalize();
            p1.setxyz(0.0,0.0,0.0);
            if (gl->assocx(12)) p1.x = *gl->assocx(12);
            if (gl->assocx(22)) p1.y = *gl->assocx(22);
            if (gl->assocx(32)) p1.z = *gl->assocx(32);
            p2.setxyz(1.0,1.0,1.0);
            p3.setxyz(0.0,0.0,0.0);
            options.set(FigureOriginalLayer);
            options.set(FigureOriginalColour);
            options.set(FigureOriginalStyle);
            options.set(FigureOriginalWeight); 
            Figure *figure = new Figure(h,_RCT(blockname),_RCT(""),p1 * scale,xa,ya,p2,p3,Point(0.0,0.0,0.0),0,0,0,NULL,options);
            if (figure != 0 && figure->getdefined())
              db.geometry.add(figure);
            delete blockname;
         }
    }
  else if (s != 0 && strcmp(s,"POLYLINE") == 0)
    {  nvertices = 0;
       if (gl->associ(70) != 0)
         polylineflags = *gl->associ(70);
       else
         polylineflags = 0;
       if ((polylineflags & 16) == 16)
         {  meshm = int(gl->associ(71) == 0 ? 0 : *gl->associ(71));
            meshn = int(gl->associ(72) == 0 ? 0 : *gl->associ(72));
            points = new Point [meshm * meshn];
         }
       else if ((polylineflags & 64) == 64)
         {  meshm = int(gl->associ(71) == 0 ? 0 : *gl->associ(71));
            meshn = int(gl->associ(72) == 0 ? 0 : *gl->associ(72)) + 1;
            points = (Point *) malloc(sizeof(Point) * meshm);
         }
       else
         points = 0;
       hpolyline = h;
       if ((thickness = gl->assocx(39)) != 0)
         pthickness = *thickness;
       else
         pthickness = 0.0;
       if (gl->assocx(40) != 0)
         startwidth = *gl->assocx(40);
       else
         startwidth = 0.0;
       if (gl->assocx(41) != 0)
         endwidth = *gl->assocx(41);
       else
         endwidth = 0.0;
       x2 = gl->assocx(210); y2 = gl->assocx(220); z2 = gl->assocx(230);
       za.setxyz((x2 == 0 ? 0.0 : *x2),(y2 == 0 ? 0.0 : *y2),(z2 == 0 ? 1.0 : *z2));
       if (fabs(za.x) < 1.0/64.0 && fabs(za.y) < 1.0/64.0)
         xa = yaxis.cross(za).normalize();
       else
         xa = zaxis.cross(za).normalize();
       ya = za.cross(xa).normalize();
       za = xa.cross(ya).normalize();
       origin.setxyz(0.0,0.0,gl->assocx(30) ? *gl->assocx(30) : 0.0);
    }
  else if (s != 0 && strcmp(s,"VERTEX") == 0)
    {Line *line1,*line2,*line3,*line4;
     int xmesh,ymesh;
     BitMask options(32);
       h = hpolyline;
       x1 = gl->assocx(10);  y1 = gl->assocx(20);  z1 = gl->assocx(30);
       if (gl->assocx(40) != 0)
         startwidth = *gl->assocx(40);
       else
         startwidth = 0.0;
       if (gl->assocx(41) != 0)
         endwidth = *gl->assocx(41);
       else
         endwidth = 0.0;
       if (x1 != 0 && y1 != 0)
         {  p1 = xa * *x1 + ya * *y1 + za * (z1 == 0 ? 0.0 : *z1) + origin;

            if (points != 0)
              {  if ((polylineflags & 16) == 16)
                   points[(nvertices % meshn) * meshm + (nvertices / meshn)] = p1;
                 else if ((polylineflags & 64) != 64 || gl->associ(71) == 0 || gl->associ(72) == 0 || gl->associ(73) == 0)
                   {  if (nvertices >= meshm)
                        {  points = (Point *)realloc(points,sizeof(Point) * (nvertices + 1));
                           meshm++;
                        }
                      points[nvertices] = p1;
                   }
              }
            nvertices++;
            if (nvertices > 1 && points == 0)
              {  if (bulge == 0.0)
                   {  if (startwidth != 0.0 || endwidth != 0.0)
                        {Point o1,o2;

                           o1 = (za.cross(p1 - lastvertex).normalize()) * startwidth;
                           o2 = (za.cross(p1 - lastvertex).normalize()) * endwidth;
                           line1 = new Line(h,(lastvertex+o1) * scale,(p1+o2) * scale);
                           line2 = new Line(h,(p1+o2) * scale,(p1-o2) * scale);
                           line3 = new Line(h,(p1-o2) * scale,(lastvertex-o1) * scale);
                           line4 = new Line(h,(lastvertex-o1) * scale,(lastvertex+o1) * scale);
                           if (line1 != 0 && line2 != 0 && line3 != 0 && line4 != 0)
                             {EntityList list;
                              BitMask o(32);

                                o.set(PlaneFilled,1);
                                db.geometry.add(line1);
                                db.geometry.add(line2);
                                db.geometry.add(line3);
                                db.geometry.add(line4);
                                list.add(line1);   
                                list.add(line2);  
                                list.add(line3);  
                                list.add(line4);  
                                Plane *plane = new Plane(h,list,100.0,100.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),rcpatternfilename,o);
                                db.geometry.add(plane);
                             }
                        }
                      else
                        {  line1 = new Line(h,lastvertex * scale,p1 * scale);
                           if (line1 != 0 && line1->getdefined())
                             {  db.geometry.add(line1);
                                if (pthickness != 0.0)
                                  {  line2 = new Line(h,(lastvertex + za * pthickness) * scale,(p1 + za * pthickness) * scale);
                                     if (line2 != 0 && line2->getdefined())
                                       {  db.geometry.add(line2);
                                          RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                                          RuledSurface *surface = new RuledSurface(h,line1,line2,xmesh,ymesh,options);
                                          if (surface != 0 && surface->getdefined())
                                            db.geometry.add(surface);
                                       }
                                  }
                             }
                        }
                   }
                 else
                   {double r,d,a1,a2;
                    Point dir1,dir2,o;
                    Circle *circle1,*circle2;
                    int xmesh,ymesh;
                    BitMask options(32);
                      dir1 = p1 - lastvertex;
                      d = dir1.length();
                      r = (bulge * bulge * d * d + d * d) / 4.0 / bulge / d;
                      dir2 = dir1.cross(za);
                      dir2.normalize();
                      o = (p1 + lastvertex) / 2.0 - dir2 * (r - bulge * d / 2.0);
                      Circle c(o,xa,ya,fabs(r),0.0,M_PI * 2.0);
                      if (bulge > 0.0)
                        {  a1 = c.angle(lastvertex);
                           a2 = c.angle(p1) - a1;
                        }
                      else
                        {  a1 = c.angle(p1);
                           a2 = c.angle(lastvertex) - a1;
                        }
                      while (a2 < 0.0) a2 += 2.0 * M_PI;
                      circle1 = new Circle(h,o * scale,xa,ya,fabs(r) * scale,a1,a2);
                      if (circle1 != 0 && circle1->getdefined())
                        {  db.geometry.add(circle1);
                           if (pthickness != 0.0)
                             {  circle2 = new Circle(h,(o + za * pthickness) * scale,xa,ya,fabs(r) * scale,a1,a2);
                                if (circle2 != 0 && circle2->getdefined())
                                  {  db.geometry.add(circle2);
                                     RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                                     RuledSurface *surface = new RuledSurface(h,circle1,circle2,xmesh,ymesh,options);
                                     if (surface != 0 && surface->getdefined())
                                       db.geometry.add(surface);
                                  }
                             }
                        }
                   }
              }
            else
              firstvertex = p1;

            if ((polylineflags & 64) == 64 && (i1 = gl->associ(71)) != 0 && (i2 = gl->associ(72)) != 0 && (i3 = gl->associ(73)) != 0 && (i4 = gl->associ(74)) != 0)
              {Line *line1,*line2,*line3,*line4,*line5;
               Plane *plane; 
               EntityList list,list1,list2;
               double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
               char patternfilename[300];
               RCCHAR rcpatternfilename[300];
               BitMask options(32);

                 p1 = points[labs(*i1)-1];
                 p2 = points[labs(*i2)-1];
                 p3 = points[labs(*i3)-1];
                 p4 = points[labs(*i4)-1];

                 if ((line1 = new Line(h,p1 * scale,p2 * scale)) != 0 && line1->getdefined())
                   {  db.geometry.add(line1);
                      list.add(line1);
                   }
                 if ((line2 = new Line(h,p2 * scale,p3 * scale)) != 0 && line2->getdefined())
                   {  db.geometry.add(line2);
                      list.add(line2);
                   } 
                 if ((p4-p3).length() > db.getptoler())
                   {  if ((line3 = new Line(h,p3 * scale,p4 * scale)) != 0 && line3->getdefined())
                        {  db.geometry.add(line3);
                           list.add(line3);
                        }
                      if ((line4 = new Line(h,p4 * scale,p1 * scale)) != 0 && line4->getdefined())
                        {  db.geometry.add(line4);
                           list.add(line4);
                        }
                   }
                 else
                   {  if ((line3 = new Line(h,p3 * scale,p1 * scale)) != 0 && line3->getdefined())
                        {  db.geometry.add(line3);
                           list.add(line3);
                        }
                      line4 = 0;
                   }

                 Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,rcpatternfilename,&options);
                 options.set(0);
                 plane = new Plane(h,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,rcpatternfilename,options);
                 if (plane != 0 && plane->getdefined())
                   {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                      db.geometry.add(plane);
                   }
                 else if (line4 != 0)
                   {  delete plane;
                      list1.add(line1);
                      list1.add(line2);
                      if ((line5 = new Line(h,p1 * scale,p3 * scale)) != 0 && line5->getdefined())
                        {  db.geometry.add(line5);
                           list1.add(line5);
                        } 
                      plane = new Plane(h,list1,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,rcpatternfilename,options);
                      if (plane != 0 && plane->getdefined())
                        {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                           db.geometry.add(plane);
                        }
                      else
                        delete plane;

                      list2.add(line3);
                      list2.add(line4);
                      list2.add(line5);
                      plane = new Plane(h,list2,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,rcpatternfilename,options);
                      if (plane != 0 && plane->getdefined())
                        {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                           db.geometry.add(plane);
                        }
                      else
                        delete plane;

                   }
              }

            lastvertex = p1;
            if (gl->assocx(42) != 0)
              bulge = *gl->assocx(42);
            else
              bulge = 0.0;
         }
    }
  else if (s != 0 && strcmp(s,"SEQEND") == 0 && nvertices >= 0)
    {int i,j,xmesh,ymesh;
     Line *line1,*line2;
     Line **lines;
     Plane *plane,*plane1,*plane2;
     EntityList list,emptylist;
     double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
     Point origin;
     BitMask options(32);
       h = hpolyline;
       if (points == 0)
         {  if ((polylineflags & 1) == 1)
              {  if (bulge == 0.0)
                   {  line1 = new Line(h,lastvertex * scale,firstvertex * scale);
                      if (line1 != 0 && line1->getdefined())
                        {  db.geometry.add(line1);
                           if (pthickness != 0)
                             {  line2 = new Line(h,(lastvertex + za * pthickness) * scale,(firstvertex + za * pthickness) * scale);
                                if (line2 != 0 && line2->getdefined())
                                  {  db.geometry.add(line2);
                                     RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                                     RuledSurface *surface = new RuledSurface(h,line1,line2,xmesh,ymesh,options);
                                     if (surface != 0 && surface->getdefined())
                                       db.geometry.add(surface);
                                  }
                             }
                        }
                   }
                 else
                   {double r,d,a1,a2;
                    Point dir1,dir2,o;
                    Circle *circle1,*circle2;
                    int xmesh,ymesh;
                    BitMask options(32);
                      dir1 = firstvertex - lastvertex;
                      d = dir1.length();
                      r = (bulge * bulge * d * d + d * d) / 4.0 / bulge / d;
                      dir2 = dir1.cross(za);
                      dir2.normalize();
                      o = (firstvertex + lastvertex) / 2.0 - dir2 * (r - bulge * d / 2.0);
                      Circle c(o,xa,ya,fabs(r),0.0,M_PI * 2.0);
                      if (bulge > 0.0)
                        {  a1 = c.angle(lastvertex);
                           a2 = c.angle(firstvertex) - a1;
                        }
                      else
                        {  a1 = c.angle(firstvertex);
                           a2 = c.angle(lastvertex) - a1;
                        }
                      while (a2 < 0.0) a2 += 2.0 * M_PI;
                      circle1 = new Circle(h,o * scale,xa,ya,fabs(r) * scale,a1,a2);
                      if (circle1 != 0 && circle1->getdefined())
                        {  db.geometry.add(circle1);
                           if (pthickness != 0)
                             {  circle2 = new Circle(h,(o +za * pthickness) * scale,xa,ya,fabs(r) * scale,a1,a2);
                                if (circle2 != 0 && circle2->getdefined())
                                  {  db.geometry.add(circle2);
                                     RuledSurface::getdefaults(&xmesh,&ymesh,&options);
                                     RuledSurface *surface = new RuledSurface(h,circle1,circle2,xmesh,ymesh,options);
                                     if (surface != 0 && surface->getdefined())
                                       db.geometry.add(surface);
                                  }
                             }
                        }
                   }
              }
         }
       else if ((polylineflags & 16) == 16)
         {  lines = new Line *[meshm * meshn * 2];
            for (i = 0 ; i < meshn ; i++)
              {  for (j = 0 ; j < meshm - ((polylineflags & 1) == 0) ; j++)
                   {  line1 = new Line(h,points[i * meshm + j] * scale,points[i * meshm + ((j + 1) % meshm)] * scale);
                      if (line1 != 0 && line1->getdefined())
                        db.geometry.add(line1);
                      if (lines != 0)
                        lines[i * meshm + j] = line1;
                   }
              }
            for (i = 0 ; i < meshm ; i++)
              {  for (j = 0 ; j < meshn - ((polylineflags & 32) == 0) ; j++)
                   {  line1 = new Line(h,points[j * meshm + i] * scale,points[((j+1) % meshn) * meshm + i] * scale);
                      if (line1 != 0 && line1->getdefined())
                        db.geometry.add(line1);
                      if (lines != 0)
                        lines[meshm * meshn + j * meshm + i] = line1;
                   }
              }
            if (lines != 0)
              for (i = 0 ; i < meshm - ((polylineflags & 1) == 0) ; i++)
                for (j = 0 ; j < meshn  - ((polylineflags & 32) == 0) ; j++)
                  {  list = emptylist;
                     list.add(lines[j * meshm + i]);
                     list.add(lines[meshm * meshn + j * meshm + ((i + 1) % meshm)]);
                     list.add(lines[((j + 1) % meshn) * meshm + i]);
                     list.add(lines[meshm * meshn + j * meshm + i]);
                     Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,rcpatternfilename,&options);
                     plane = new Plane(h,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,rcpatternfilename,options);
                     if (plane != 0)
                       {  if (plane->getdefined())
                            {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                               db.geometry.add(plane);
                            }
                          else
                            {Line *line;
                               delete plane;
                               if ((line = new Line(h,points[j*meshm+i] * scale,points[((j+1)%meshn)*meshm+(i+1)%meshm] * scale)) != 0)
                                 {  list = emptylist;
                                    list.add(lines[j * meshm + i]);
                                    list.add(lines[meshm * meshn + j * meshm + ((i + 1) % meshm)]);
                                    list.add(line);
                                    plane = new Plane(h,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,rcpatternfilename,options);
                                    if (plane != 0 && plane->getdefined())
                                      {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                                         db.geometry.add(plane);
                                         plane1 = 0;
                                      }
                                    else
                                      plane1 = plane;
                                    list = emptylist;
                                    list.add(line);
                                    list.add(lines[((j + 1) % meshn) * meshm + i]);
                                    list.add(lines[meshm * meshn + j * meshm + i]);
                                    plane = new Plane(h,list,xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,rcpatternfilename,options);
                                    if (plane != 0 && plane->getdefined())
                                      {  plane->EntityHeader::change(h.getcolour(),h.getlayer(),h.getstyle(),h.getweight());
                                         db.geometry.add(plane);
                                         plane2 = 0;
                                      }
                                    else
                                      plane2 = plane;
                                    //delete plane1;
                                    //delete plane2;
                                 }
                            }
                       }
                  }
            delete [] points;
            delete [] lines;
            points = 0;
            lines = 0;
         }
       else
         {  delete [] points;
            points = 0;
         }

       nvertices = -1;
    }
  else if (s != 0 && strcmp(s,"VIEWPORT") == 0)
    {double *pwidth,*pheight;
     long *viewportid,*status;
     int i,viewno;
     View2d *view2d;
     OneView *ov;
     BitMask options(32),visible(MaxViews);
     char vname[256],paperspacename[300];
     View *view;


       if (blockname == 0 || strrchr(blockname,'/') == 0)
         strcpy(paperspacename,"*Paper_Space"); 
       else
         {  strcpy(paperspacename,strrchr(blockname,'/')+1);
            if (strrchr(paperspacename,'.') != 0)
              *strrchr(paperspacename,'.') = 0;
            if (paperspacename[0] == '$')
              paperspacename[0] = '*';

         }

       x1 = gl->assocx(10);  y1 = gl->assocx(20);
       pwidth = gl->assocx(40);  pheight = gl->assocx(41);
       viewportid = gl->associ(69);

       if (x1 != 0 && y1 != 0 && pwidth != 0 && pheight != 0 && viewportid != 0)
         {  view2d = db.views2d.match(_RCT(paperspacename));
            if (*viewportid == 1)
              {  if ((viewno = db.getnextviewno()) >= 0)
                   if ((view2d = new View2d(_RCT(paperspacename),*pwidth * scale,*pheight * scale,short(viewno))) != NULL)
                     {  db.views2d.add(view2d);
                        if (orphan_view_list.length() != 0)
                        {   for (orphan_view_list.start() ; (ov = (OneView *)orphan_view_list.next()) != 0 ; )
                                view2d->add(ov);
                            orphan_view_list.destroy();
                        }
                        papervisibility.clearandset(viewno,end_list);
                        if (blockname != 0) 
                          modelvisibility.clearandset(viewno,end_list);
                        else
                          {  modelvisibility.clearall();
                             for (i = 0 ; i < MaxViews ; i++)
                               if (db.views3d.match(i) != 0)
                                 modelvisibility.set(i);
                          } 
                     }
              }
            else
              {  
                   {double *twist,*ex,*ey,*ez,*rx,*ry,*rz,*f,*b,*focal,front,back,*cx,*cy,*h;
                    Point eye,ref,up,yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);

                      rx = gl->assocx(17);
                      ry = gl->assocx(27);
                      rz = gl->assocx(37);
                      ex = gl->assocx(16);
                      ey = gl->assocx(26);
                      ez = gl->assocx(36);
                      h = gl->assocx(45);
                      cx = gl->assocx(12);
                      cy = gl->assocx(22);
                      focal = gl->assocx(42);
                      f = gl->assocx(43);
                      b = gl->assocx(44);
                      twist = gl->assocx(51);
                      status = gl->associ(90);

                      if (twist != 0 && rx != 0 && ry != 0 && rz != 0 && ex != 0 && ey != 0 &&
                          ez != 0 && focal != 0 && f != 0 && b != 0 &&
                          h != 0 && cx != 0 && cy != 0 && (viewno = db.getnextviewno()) >= 0)
                        {  eye.setxyz(*ex + *rx,*ey + *ry,*ez + *rz);
                           ref.setxyz(*rx,*ry,*rz);
                           up = (ref - eye).normalize().cross(zaxis);
                           if (up.length() < 1.0E-2)
                             up = (ref - eye).cross(yaxis);
                           up = up.cross(ref - eye).normalize();
 
                           Transform t;
                           t.rotate(Point(0.0,0.0,0.0),(ref - eye).normalize(),*twist / 180.0 * acos(-1.0));
                           up = t.transform(up);   

                           front = *f;
                           back = *b;
                           if (front == 0.0 || status != 0 && (*status&2) == 0)
                             front = -1.0E6;
                           if (back == 0.0 || status != 0 && (*status&4) == 0)
                             back = 1.0E6;
                           if (fabs(front - back) < 1.0E-4)
                             front = back - 1.0E-4;
                           if (*focal != 0.0)
                             {  ref = eye + (ref - eye).normalize() * *focal;
                           //     if (front < -*focal * 0.99)
                           //       front = -*focal * 0.99;
                             }
                           sprintf(vname,"MVIEW%2.2d",viewno);
                           if ((view = new View(_RCT(vname),0,viewno,eye,ref,up,front,back)) != 0)
                             {
                                ov = new OneView(view,
                                          (-*pwidth / 2.0 * *h / *pheight + *cx) * scale,
                                          (-*h / 2.0  + *cy)* scale,
                                          (*pwidth / 2.0 * *h / *pheight  + *cx)* scale,
                                          (*h / 2.0  + *cy)* scale,
                                          *h / *pheight,(*x1 - *cx / (*h / *pheight)) * scale,(*y1 - *cy  / (*h / *pheight)) * scale,0.0,options,0);
                                if (ov != NULL)
                                  {  db.views3d.add(view);
                                     if (view2d != 0)
                                     view2d->add(ov);
                                     else
                                        orphan_view_list.add(ov);
                                  }
                             }
                        }
                   }
              }
         }
    }
//  else if (s != 0)
//    {FILE *dfile = fopen("debug.log","a");
//       fprintf(dfile,"TYPE '%s' ignored\n",s);
//       fclose(dfile);
//    }
}

DXFStyleInfo::DXFStyleInfo(char *n,int sno)
{ if ((name = new char[strlen(n) + 1]) != 0)
    strcpy(name,n);
  styleno = sno;
}

DXFStyles::~DXFStyles()
{DXFStyleInfo *dxfsi;
  for (list.start() ; (dxfsi = (DXFStyleInfo *) list.next()) != 0 ; )
    delete dxfsi;
  list.destroy();
}

int DXFStyles::add(char *n,int sno)
{DXFStyleInfo *dxfsi;
  if ((dxfsi = new DXFStyleInfo(n,sno)) != 0 && dxfsi->name != 0)
    return list.add(dxfsi);
  return 0;
}

int DXFStyles::match(char *name)
{DXFStyleInfo *dxfsi;
  for (list.start() ; (dxfsi = (DXFStyleInfo *) list.next()) != 0 ; )
    if (strcmp(name,dxfsi->name) == 0 && dxfsi->styleno >= 0 && dxfsi->styleno < 255)
      return dxfsi->styleno;
  return 0;
}

void DXFInConvert::processtable(DXFInGroupList *gl)
{char *s,*name,*linetype;
 long *attributes,*colour,*nitems;
 DXFInLayerInfo *li;
 int i,j,w;
 double *x;
 LineStyle *ls;
  s = gl->assocs(0);
  if (s != 0 && strcmp(s,"LAYER") == 0)
    {  name = gl->assocs(2);
       linetype = gl->assocs(6);
       attributes = gl->associ(70);
       colour = gl->associ(62);
       x = gl->assocx(370);
       if (x != 0)
         w = db.lineweights.mmtoweight(*x/100.0);
       else
         w = 0;

       if (name != 0 && linetype != 0 && attributes != 0 && colour != 0)
         {  for (i = 0 ; i < MAX_LAYERS ; i++)
              {  if (strlen(db.layernames.getlayername(i)) == 0)
                   {  if ((li = new DXFInLayerInfo(name,i,int(*attributes),abs(int(*colour)),w,linetype)) != 0)
                        {  db.dismask.layer.set(i,*colour >= 0 && (*attributes & 1) == 0);
                           db.layernames.setlayername(i,name);
                           layerlist.add(li);
                        }
                      break;
                   }   
              }
         }

    }
  else if (s != 0 && strcmp(s,"LTYPE") == 0)
    {  name = gl->assocs(2);
       if (name != 0 && strcmp(name,"CONTINUOUS") == 0)
          dxfstyles.add(name,0);
       else if (name != 0 && (ls = new LineStyle) != 0 && (nitems = gl->associ(73)) != 0)
         if (*nitems == 0)
           dxfstyles.add(name,0);
         else
           {  for (i = 0,j = 0 ; i < int(*nitems) ; i++)
                if ((x = gl->assocx(49,i)) != 0)
                  {  if (*x < 0.0 && (j % 2) == 0 || *x >= 0.0 && (j % 2) == 1)
                       {  ls->add(0.0);  j++;
                       }
                     ls->add(fabs(*x) * scale);
                     j++;
                  }
              dxfstyles.add(name,db.linestyles.add(ls));
           }
    }
  else if (s != 0 && strcmp(s,"DIMSTYLE") == 0)
    {double *x1,*x2;
       name = gl->assocs(2);
       if (name != 0 && strncmp(name,"DET_LEADER",10) == 0)
         {  if ((x1 = gl->assocx(41)) != 0)
              leadersize = *x1;
         } 
       else if (name != 0 && strncmp(name,"STANDARD$7",10) == 0)
         {  if ((x1 = gl->assocx(41)) != 0 && (x2 = gl->assocx(40)) != 0 )
              leadersize = *x1 * *x2;
         } 
    }
  else if (s != 0 && strcmp(s,"BLOCK_RECORD") == 0)
    {char *handle;
       name = gl->assocs(2);
       handle = gl->assocs(340);
       if (name != 0 && handle != 0 && _strnicmp(name,"*paper_space",12) == 0)
         PaperSpaceLayouts.AddPaperSpace(name,handle);
         
    }

}

void DXFInConvert::loadcolourtable(void)
{
#if 1
  int i;
  db.colourtable[1].set(255,0,0);
  db.colourtable[2].set(255,255,0);
  db.colourtable[3].set(0,255,0);
  db.colourtable[4].set(0,255,255);
  db.colourtable[5].set(0,0,255);
  db.colourtable[6].set(255,0,255);
  db.colourtable[7].set(200,200,200);
  db.colourtable[8].set(128,128,128);
  db.colourtable[9].set(192,192,192);

  db.colourtable[250].set(51,51,51);
  db.colourtable[251].set(91,91,91);
  db.colourtable[252].set(132,132,132);
  db.colourtable[253].set(173,173,173);
  db.colourtable[254].set(214,214,214);
  db.colourtable[255].set(220,220,220);

  for (i = 10 ; i <= 249 ; i++)
    db.colourtable[i].sethls(15.0 * ((i/10)-1),i % 10 > 5 ? 0.9 - 0.2*((i/2)%5) : 0.8 - 0.2*((i/2)%5),(i & 1) ? 0.5 : 1.0);

  for (i = 0 ; i < 256 ; i++)
    {char key[256];
       sprintf(key,"cl::%d",i);
       v.setlong(key,(long(db.colourtable[i].red) << 16) | (long(db.colourtable[i].green) << 8) | db.colourtable[i].blue);
    }
#else
    // the hand built ACAD colour table
    int i;
    //db.colourtable[	0	].set( 	0	,	0	,	0	);
    db.colourtable[	1	].set( 	255	,	0	,	0	);
    db.colourtable[	2	].set( 	255	,	255	,	0	);
    db.colourtable[	3	].set( 	0	,	255	,	0	);
    db.colourtable[	4	].set( 	0	,	255	,	255	);
    db.colourtable[	5	].set( 	0	,	0	,	255	);
    db.colourtable[	6	].set( 	255	,	0	,	255	);
    db.colourtable[	7	].set( 	0	,	0	,	0	);// or db.colourtable[	7	].set( 	255	,	255	,	255	);
    db.colourtable[	8	].set( 	65	,	65	,	65	);
    db.colourtable[	9	].set( 	128	,	128	,	128	);
    db.colourtable[	10	].set( 	255	,	0	,	0	);
    db.colourtable[	11	].set( 	255	,	170	,	170	);
    db.colourtable[	12	].set( 	189	,	0	,	0	);
    db.colourtable[	13	].set( 	189	,	126	,	126	);
    db.colourtable[	14	].set( 	129	,	0	,	0	);
    db.colourtable[	15	].set( 	129	,	86	,	86	);
    db.colourtable[	16	].set( 	104	,	0	,	0	);
    db.colourtable[	17	].set( 	104	,	69	,	69	);
    db.colourtable[	18	].set( 	79	,	0	,	0	);
    db.colourtable[	19	].set( 	79	,	53	,	53	);
    db.colourtable[	20	].set( 	255	,	63	,	0	);
    db.colourtable[	21	].set( 	255	,	191	,	170	);
    db.colourtable[	22	].set( 	189	,	46	,	0	);
    db.colourtable[	23	].set( 	189	,	141	,	126	);
    db.colourtable[	24	].set( 	129	,	31	,	0	);
    db.colourtable[	25	].set( 	129	,	96	,	86	);
    db.colourtable[	26	].set( 	104	,	25	,	0	);
    db.colourtable[	27	].set( 	104	,	78	,	69	);
    db.colourtable[	28	].set( 	79	,	19	,	0	);
    db.colourtable[	29	].set( 	79	,	59	,	53	);
    db.colourtable[	30	].set( 	255	,	127	,	0	);
    db.colourtable[	31	].set( 	255	,	212	,	170	);
    db.colourtable[	32	].set( 	189	,	94	,	0	);
    db.colourtable[	33	].set( 	189	,	157	,	126	);
    db.colourtable[	34	].set( 	129	,	64	,	0	);
    db.colourtable[	35	].set( 	129	,	107	,	86	);
    db.colourtable[	36	].set( 	104	,	52	,	0	);
    db.colourtable[	37	].set( 	104	,	86	,	69	);
    db.colourtable[	38	].set( 	79	,	39	,	0	);
    db.colourtable[	39	].set( 	79	,	66	,	53	);
    db.colourtable[	40	].set( 	255	,	191	,	0	);
    db.colourtable[	41	].set( 	255	,	234	,	170	);
    db.colourtable[	42	].set( 	189	,	141	,	0	);
    db.colourtable[	43	].set( 	189	,	173	,	126	);
    db.colourtable[	44	].set( 	129	,	96	,	0	);
    db.colourtable[	45	].set( 	129	,	118	,	86	);
    db.colourtable[	46	].set( 	104	,	78	,	0	);
    db.colourtable[	47	].set( 	104	,	95	,	69	);
    db.colourtable[	48	].set( 	79	,	59	,	0	);
    db.colourtable[	49	].set( 	79	,	73	,	53	);
    db.colourtable[	50	].set( 	255	,	255	,	0	);
    db.colourtable[	51	].set( 	255	,	255	,	170	);
    db.colourtable[	52	].set( 	189	,	189	,	0	);
    db.colourtable[	53	].set( 	189	,	189	,	126	);
    db.colourtable[	54	].set( 	129	,	129	,	0	);
    db.colourtable[	55	].set( 	129	,	129	,	86	);
    db.colourtable[	56	].set( 	104	,	104	,	0	);
    db.colourtable[	57	].set( 	104	,	104	,	69	);
    db.colourtable[	58	].set( 	79	,	79	,	0	);
    db.colourtable[	59	].set( 	79	,	79	,	53	);
    db.colourtable[	60	].set( 	191	,	255	,	0	);
    db.colourtable[	61	].set( 	234	,	255	,	170	);
    db.colourtable[	62	].set( 	141	,	189	,	0	);
    db.colourtable[	63	].set( 	173	,	189	,	126	);
    db.colourtable[	64	].set( 	96	,	129	,	0	);
    db.colourtable[	65	].set( 	118	,	129	,	86	);
    db.colourtable[	66	].set( 	78	,	104	,	0	);
    db.colourtable[	67	].set( 	95	,	104	,	69	);
    db.colourtable[	68	].set( 	59	,	79	,	0	);
    db.colourtable[	69	].set( 	73	,	79	,	53	);
    db.colourtable[	70	].set( 	127	,	255	,	0	);
    db.colourtable[	71	].set( 	212	,	255	,	170	);
    db.colourtable[	72	].set( 	94	,	189	,	0	);
    db.colourtable[	73	].set( 	157	,	189	,	126	);
    db.colourtable[	74	].set( 	64	,	129	,	0	);
    db.colourtable[	75	].set( 	107	,	129	,	86	);
    db.colourtable[	76	].set( 	52	,	104	,	0	);
    db.colourtable[	77	].set( 	86	,	104	,	69	);
    db.colourtable[	78	].set( 	39	,	79	,	0	);
    db.colourtable[	79	].set( 	66	,	79	,	53	);
    db.colourtable[	80	].set( 	63	,	255	,	0	);
    db.colourtable[	81	].set( 	191	,	255	,	170	);
    db.colourtable[	82	].set( 	46	,	189	,	0	);
    db.colourtable[	83	].set( 	141	,	189	,	126	);
    db.colourtable[	84	].set( 	31	,	129	,	0	);
    db.colourtable[	85	].set( 	96	,	129	,	86	);
    db.colourtable[	86	].set( 	25	,	104	,	0	);
    db.colourtable[	87	].set( 	78	,	104	,	69	);
    db.colourtable[	88	].set( 	19	,	79	,	0	);
    db.colourtable[	89	].set( 	59	,	79	,	53	);
    db.colourtable[	90	].set( 	0	,	255	,	0	);
    db.colourtable[	91	].set( 	170	,	255	,	170	);
    db.colourtable[	92	].set( 	0	,	189	,	0	);
    db.colourtable[	93	].set( 	126	,	189	,	126	);
    db.colourtable[	94	].set( 	0	,	129	,	0	);
    db.colourtable[	95	].set( 	86	,	129	,	86	);
    db.colourtable[	96	].set( 	0	,	104	,	0	);
    db.colourtable[	97	].set( 	69	,	104	,	69	);
    db.colourtable[	98	].set( 	0	,	79	,	0	);
    db.colourtable[	99	].set( 	53	,	79	,	53	);
    db.colourtable[	100	].set( 	0	,	255	,	63	);
    db.colourtable[	101	].set( 	170	,	255	,	191	);
    db.colourtable[	102	].set( 	0	,	189	,	46	);
    db.colourtable[	103	].set( 	126	,	189	,	141	);
    db.colourtable[	104	].set( 	0	,	129	,	31	);
    db.colourtable[	105	].set( 	86	,	129	,	96	);
    db.colourtable[	106	].set( 	0	,	104	,	25	);
    db.colourtable[	107	].set( 	69	,	104	,	78	);
    db.colourtable[	108	].set( 	0	,	79	,	19	);
    db.colourtable[	109	].set( 	53	,	79	,	59	);
    db.colourtable[	110	].set( 	0	,	255	,	127	);
    db.colourtable[	111	].set( 	170	,	255	,	212	);
    db.colourtable[	112	].set( 	0	,	189	,	94	);
    db.colourtable[	113	].set( 	126	,	189	,	157	);
    db.colourtable[	114	].set( 	0	,	129	,	64	);
    db.colourtable[	115	].set( 	86	,	129	,	107	);
    db.colourtable[	116	].set( 	0	,	104	,	52	);
    db.colourtable[	117	].set( 	69	,	104	,	86	);
    db.colourtable[	118	].set( 	0	,	79	,	39	);
    db.colourtable[	119	].set( 	53	,	79	,	66	);
    db.colourtable[	120	].set( 	0	,	255	,	191	);
    db.colourtable[	121	].set( 	170	,	255	,	234	);
    db.colourtable[	122	].set( 	0	,	189	,	141	);
    db.colourtable[	123	].set( 	126	,	189	,	173	);
    db.colourtable[	124	].set( 	0	,	129	,	96	);
    db.colourtable[	125	].set( 	86	,	129	,	118	);
    db.colourtable[	126	].set( 	0	,	104	,	78	);
    db.colourtable[	127	].set( 	69	,	104	,	95	);
    db.colourtable[	128	].set( 	0	,	79	,	59	);
    db.colourtable[	129	].set( 	53	,	79	,	73	);
    db.colourtable[	130	].set( 	0	,	255	,	255	);
    db.colourtable[	131	].set( 	170	,	255	,	255	);
    db.colourtable[	132	].set( 	0	,	189	,	189	);
    db.colourtable[	133	].set( 	126	,	189	,	189	);
    db.colourtable[	134	].set( 	0	,	129	,	129	);
    db.colourtable[	135	].set( 	86	,	129	,	129	);
    db.colourtable[	136	].set( 	0	,	104	,	104	);
    db.colourtable[	137	].set( 	69	,	104	,	104	);
    db.colourtable[	138	].set( 	0	,	79	,	79	);
    db.colourtable[	139	].set( 	53	,	79	,	79	);
    db.colourtable[	140	].set( 	0	,	191	,	255	);
    db.colourtable[	141	].set( 	170	,	234	,	255	);
    db.colourtable[	142	].set( 	0	,	141	,	189	);
    db.colourtable[	143	].set( 	126	,	173	,	189	);
    db.colourtable[	144	].set( 	0	,	96	,	129	);
    db.colourtable[	145	].set( 	86	,	118	,	129	);
    db.colourtable[	146	].set( 	0	,	78	,	104	);
    db.colourtable[	147	].set( 	69	,	95	,	104	);
    db.colourtable[	148	].set( 	0	,	59	,	79	);
    db.colourtable[	149	].set( 	53	,	73	,	79	);
    db.colourtable[	150	].set( 	0	,	127	,	255	);
    db.colourtable[	151	].set( 	170	,	212	,	255	);
    db.colourtable[	152	].set( 	0	,	94	,	189	);
    db.colourtable[	153	].set( 	126	,	157	,	189	);
    db.colourtable[	154	].set( 	0	,	64	,	129	);
    db.colourtable[	155	].set( 	86	,	107	,	129	);
    db.colourtable[	156	].set( 	0	,	52	,	104	);
    db.colourtable[	157	].set( 	69	,	86	,	104	);
    db.colourtable[	158	].set( 	0	,	39	,	79	);
    db.colourtable[	159	].set( 	53	,	66	,	79	);
    db.colourtable[	160	].set( 	0	,	63	,	255	);
    db.colourtable[	161	].set( 	170	,	191	,	255	);
    db.colourtable[	162	].set( 	0	,	46	,	189	);
    db.colourtable[	163	].set( 	126	,	141	,	189	);
    db.colourtable[	164	].set( 	0	,	31	,	129	);
    db.colourtable[	165	].set( 	86	,	96	,	129	);
    db.colourtable[	166	].set( 	0	,	25	,	104	);
    db.colourtable[	167	].set( 	69	,	78	,	104	);
    db.colourtable[	168	].set( 	0	,	19	,	79	);
    db.colourtable[	169	].set( 	53	,	59	,	79	);
    db.colourtable[	170	].set( 	0	,	0	,	255	);
    db.colourtable[	171	].set( 	170	,	170	,	255	);
    db.colourtable[	172	].set( 	0	,	0	,	189	);
    db.colourtable[	173	].set( 	126	,	126	,	189	);
    db.colourtable[	174	].set( 	0	,	0	,	129	);
    db.colourtable[	175	].set( 	86	,	86	,	129	);
    db.colourtable[	176	].set( 	0	,	0	,	104	);
    db.colourtable[	177	].set( 	69	,	69	,	104	);
    db.colourtable[	178	].set( 	0	,	0	,	79	);
    db.colourtable[	179	].set( 	53	,	53	,	79	);
    db.colourtable[	180	].set( 	63	,	0	,	255	);
    db.colourtable[	181	].set( 	191	,	170	,	255	);
    db.colourtable[	182	].set( 	46	,	0	,	189	);
    db.colourtable[	183	].set( 	141	,	126	,	189	);
    db.colourtable[	184	].set( 	31	,	0	,	129	);
    db.colourtable[	185	].set( 	96	,	86	,	129	);
    db.colourtable[	186	].set( 	25	,	0	,	104	);
    db.colourtable[	187	].set( 	78	,	69	,	104	);
    db.colourtable[	188	].set( 	19	,	0	,	79	);
    db.colourtable[	189	].set( 	59	,	53	,	79	);
    db.colourtable[	190	].set( 	127	,	0	,	255	);
    db.colourtable[	191	].set( 	212	,	170	,	255	);
    db.colourtable[	192	].set( 	94	,	0	,	189	);
    db.colourtable[	193	].set( 	157	,	126	,	189	);
    db.colourtable[	194	].set( 	64	,	0	,	129	);
    db.colourtable[	195	].set( 	107	,	86	,	129	);
    db.colourtable[	196	].set( 	52	,	0	,	104	);
    db.colourtable[	197	].set( 	86	,	69	,	104	);
    db.colourtable[	198	].set( 	39	,	0	,	79	);
    db.colourtable[	199	].set( 	66	,	53	,	79	);
    db.colourtable[	200	].set( 	191	,	0	,	255	);
    db.colourtable[	201	].set( 	234	,	170	,	255	);
    db.colourtable[	202	].set( 	141	,	0	,	189	);
    db.colourtable[	203	].set( 	173	,	126	,	189	);
    db.colourtable[	204	].set( 	96	,	0	,	129	);
    db.colourtable[	205	].set( 	118	,	86	,	129	);
    db.colourtable[	206	].set( 	78	,	0	,	104	);
    db.colourtable[	207	].set( 	95	,	69	,	104	);
    db.colourtable[	208	].set( 	59	,	0	,	79	);
    db.colourtable[	209	].set( 	73	,	53	,	79	);
    db.colourtable[	210	].set( 	255	,	0	,	255	);
    db.colourtable[	211	].set( 	255	,	170	,	255	);
    db.colourtable[	212	].set( 	189	,	0	,	189	);
    db.colourtable[	213	].set( 	189	,	126	,	189	);
    db.colourtable[	214	].set( 	129	,	0	,	129	);
    db.colourtable[	215	].set( 	129	,	86	,	129	);
    db.colourtable[	216	].set( 	104	,	0	,	104	);
    db.colourtable[	217	].set( 	104	,	69	,	104	);
    db.colourtable[	218	].set( 	79	,	0	,	79	);
    db.colourtable[	219	].set( 	79	,	53	,	79	);
    db.colourtable[	220	].set( 	255	,	0	,	191	);
    db.colourtable[	221	].set( 	255	,	170	,	234	);
    db.colourtable[	222	].set( 	189	,	0	,	141	);
    db.colourtable[	223	].set( 	189	,	126	,	173	);
    db.colourtable[	224	].set( 	129	,	0	,	96	);
    db.colourtable[	225	].set( 	129	,	86	,	118	);
    db.colourtable[	226	].set( 	104	,	0	,	78	);
    db.colourtable[	227	].set( 	104	,	69	,	95	);
    db.colourtable[	228	].set( 	79	,	0	,	59	);
    db.colourtable[	229	].set( 	79	,	53	,	73	);
    db.colourtable[	230	].set( 	255	,	0	,	127	);
    db.colourtable[	231	].set( 	255	,	170	,	212	);
    db.colourtable[	232	].set( 	189	,	0	,	94	);
    db.colourtable[	233	].set( 	189	,	126	,	157	);
    db.colourtable[	234	].set( 	129	,	0	,	64	);
    db.colourtable[	235	].set( 	129	,	86	,	107	);
    db.colourtable[	236	].set( 	104	,	0	,	52	);
    db.colourtable[	237	].set( 	104	,	69	,	86	);
    db.colourtable[	238	].set( 	79	,	0	,	39	);
    db.colourtable[	239	].set( 	79	,	53	,	66	);
    db.colourtable[	240	].set( 	255	,	0	,	63	);
    db.colourtable[	241	].set( 	255	,	170	,	191	);
    db.colourtable[	242	].set( 	189	,	0	,	46	);
    db.colourtable[	243	].set( 	189	,	126	,	141	);
    db.colourtable[	244	].set( 	129	,	0	,	31	);
    db.colourtable[	245	].set( 	129	,	86	,	96	);
    db.colourtable[	246	].set( 	104	,	0	,	25	);
    db.colourtable[	247	].set( 	104	,	69	,	78	);
    db.colourtable[	248	].set( 	79	,	0	,	19	);
    db.colourtable[	249	].set( 	79	,	53	,	59	);
    db.colourtable[	250	].set( 	51	,	51	,	51	);
    db.colourtable[	251	].set( 	80	,	80	,	80	);
    db.colourtable[	252	].set( 	105	,	105	,	105	);
    db.colourtable[	253	].set( 	130	,	130	,	130	);
    db.colourtable[	254	].set( 	190	,	190	,	190	);
    db.colourtable[	255	].set( 	255	,	255	,	255	);

for (i = 0 ; i < 256 ; i++)
{
     char key[256];
     sprintf(key,"cl::%d",i);
     v.setlong(key,(long(db.colourtable[i].red) << 16) | (long(db.colourtable[i].green) << 8) | db.colourtable[i].blue);
}

#endif
}

int DXFInFile::setscale(void)
{Dialog dialog("DXFIn_Dialog");
 int i,units;
 BitMask options(32);

  units = v.getinteger("dx::units");
  options = v.getinteger("dx::options");
  dialog.add(new RadioButtonDialogControl(100,100,101,&units));
  dialog.add(new RadioButtonDialogControl(101,100,101,&units));
  dialog.add(new CheckBoxDialogControl(102,&options,DXFOptionClearLayerNames));
  if (dialog.process() == TRUE)
    {  scale = (units == 0 ? 1.0 : 25.4);
       v.setinteger("dx::units",units);
       v.setbm("dx::options",options);

       if (options.test(DXFOptionClearLayerNames))
         {  for (i = 0 ; i < MAX_LAYERS ; i++)
              db.layernames.setlayername(i,_RCT(""));
         }

       return 1;
    }
  else
    return 0;

}

static void regenerateall(int,void *,View3dSurface *vs)
{
    if (typeid(vs) == typeid(class View3dWindow) && ((View3dWindow *)vs)->getoffscreenbitmap() != 0)
    {
        ((View3dWindow *)vs)->getoffscreenbitmap()->displaylist.clear();
       ((View3dWindow *)vs)->getoffscreenbitmap()->displaylist.setorgtransform(vs->getumin(),vs->getvmin(),vs->getumax(),vs->getvmax());
    }
    if (vs->getsurfacetype() == View3dSurf)
    {
        db.clearScene(DM_ERASE,vs);
    }
    else if (vs->getsurfacetype() == View2dSurf)
    {
        db.clearScene(DM_ERASE,vs);
    }
    InvalidateRect(vs->gethwnd(),0,TRUE);
}

//typedef int (*Convert)(int todxf,char *infilename,char *outfilename);
//typedef int (*Recover)(int todxf,char *infilename,char *outfilename);
typedef int (WINAPI *Convert)(int inType,char *infilename/*,char *outfilename*/);
//typedef char *(WINAPI *startfn)(InterfaceTable *it);

// KMJ new combined dwg/dxf open function function
// uses the options dialog and uses the new dwg/dxf importer
// links to the DWGConvert application
//
void open_dwgdxf(RCCHAR *fileName)
{
    // enhancement to use the DWGConvert application to preprocess DWG/DXF files
    // assuming the DWGConvert application path is in the ini file
    // the options dialog is used on mac and windows
    // the preprocessor is only available on windows
    // ont the mac...
    // 1. put up the options dialog
    // 2. ask for the input dwg file name
    // run convert with that file name
    // on windows...
    // 1. put up the options dialog
    // 2. ask for the input file name
    // 3. if preprocessing is selected
    // 4.   preprocess the dwg file
    // 5.   run convert with the pp output file
    // 6.   rename the converted file to the <selected file name>.cad
    // 8. else if not preprocessing
    // 9.   run convert with the selected file
    //
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libImporter.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Importer.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
        {
            // copy the in filename
            QString infileName(fileName);
            QFileInfo infileInfo(infileName);

            if( ! infileName.isEmpty() )
            {
                // put up the DWG import options dialog
                Dialog dialog("DWGImportOptions_Dialog");
                if(infileInfo.suffix().toUpper() == "DWG")
                    dialog.title(_RCT("DWG Import Options"));
                else if(infileInfo.suffix().toUpper() == "DXF")
                    dialog.title(_RCT("DXF Import Options"));
                else
                    return;

                if(dialog.process())
                {
                    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

                    // basic settings are used by the importer through the integer values
                    // set the basic settings into the values before the importer starts
                    v.setinteger("dg::in-use_advanced_options",settings.value("Integers/dg::in-use_advanced_options",0).toInt());
                    v.setinteger("dg::in-use_dwg_layers",settings.value("Integers/dg::in-use_dwg_layers",0).toInt());

                    QString dcwinapp(v.getstring("db::dwg-preprocessor-path"));
                    QFileInfo appFileInfo(dcwinapp);
                    if( !appFileInfo.isDir() && appFileInfo.exists(dcwinapp) )
                    {
                        //QString outfileName = infileName.toUpper().replace(".DWG","-fixed.dwg");
                        QString outfileName = infileName;
                        if(infileInfo.suffix().toUpper() == "DWG")
                            outfileName = infileName.toUpper().replace(".DWG","-fixed.dwg");
                        else if(infileInfo.suffix().toUpper() == "DXF")
                            outfileName = infileName.toUpper().replace(".DXF","-fixed.dxf");
                        else
                            return;

                        QStringList arguments;

                        // build the arguments for DWGConvert from the dialog settings
                        int iExplode = 0;
                        QString switchesStr;
                        //program->setbusy(1);

                        if(settings.value("Integers/dg::in-use_advanced_options",0).toInt() == 1)
                        {
                            arguments << infileName << outfileName;

                            if(settings.value("Integers/dg::in-purge_dwg",0).toInt() == 1);
                                arguments << "/PURGE=2"; // purging all and empty blocks
                            if(settings.value("Integers/dg::in-visible_layers_only",0).toInt() == 1)
                                arguments <<  "/INVIS=1";
                            if(settings.value("Integers/dg::in-model_space_only",0).toInt() == 1)
                                arguments <<  "/LAYOUT=Model";
                            if(settings.value("Integers/dg::in-polylines_to_lines",0).toInt() == 1)
                                arguments <<  "/XP=0";
                            if(settings.value("Integers/dg::in-explode_blocks",0).toInt() == 1)
                                iExplode = 16;
                            if(settings.value("Integers/dg::in-proxy_to_blocks",0).toInt() == 1)
                                iExplode += 256;
                            if(settings.value("Integers/dg::in-explode_hatch_patterns",0).toInt() == 1)
                                iExplode += 512;
                            if(settings.value("Integers/dg::in-xrefs_to_blocks",0).toInt() == 1)
                                iExplode += 33554432;

                            if(iExplode > 0)
                                switchesStr += QString("/x=%1").arg(iExplode);

                            arguments << switchesStr;

                            qDebug() << "switchesStr : " << switchesStr;
                            qDebug() << "arguments : " << arguments;

                            // delete the output file
                            QFileInfo outFileInfo(outfileName);
                            if(outFileInfo.exists() && !outFileInfo.isDir())
                                QFile::remove(outfileName);

                            //QProcess::startDetached(dcwinapp, arguments);
                            QProcess process(app);
                            process.start(dcwinapp,arguments);
                            if(process.waitForStarted(5000))
                            {
                                qDebug() << "process started";
                                cadwindow->prompt->normalprompt("Preprocessing started");
                                if(process.waitForFinished(-1))
                                {
                                    qDebug() << "process finished OK";
                                    // look for the output file
                                    if(QFileInfo::exists(outfileName))
                                    {
                                        qDebug() << "Output file created OK";
                                        cadwindow->prompt->normalprompt("Preprocessing finished");
                                        infileName = outfileName;
                                    }
                                    else
                                    {
                                        qDebug() << "Ouput file not created";
                                        cadwindow->prompt->normalprompt("Preprocessing failed");
                                        QMessageBox::critical(0,"DWG Import","The preprocessor application failed to convert the file.\n\nCheck your setup and try again or\ncontact CAD International support for assistance.");
                                        infileName.clear();
                                    }
                                }
                                else
                                {
                                    qDebug() << "process finished error";
                                    cadwindow->prompt->normalprompt("Preprocessing error");
                                    QMessageBox::critical(0,"DWG Import","The preprocessor application finshed with an error.\n\nCheck your setup and try again or\ncontact CAD International support for assistance.");
                                    infileName.clear();
                                }
                            }
                            else
                            {
                                qDebug() << "process not started";
                                cadwindow->prompt->normalprompt("Preprocessor could not start");
                                QMessageBox::critical(0,"DWG Import","The preprocessor application could not start.\n\nCheck your setup and try again or\ncontact CAD International support for assistance.");
                                infileName.clear();
                            }
                        }
                    }
                    // try to import the file
                    if( ! infileName.isEmpty() )
                    {
                        try
                        {
                            char chinfileName[600]={0};
                            strcpy(chinfileName,infileName.data());

                            cadwindow->prompt->normalprompt("Importer starting");
                            convert(1,chinfileName); // dwg import
                            //convert(1,0); // dwg import
                        }
                        catch(...)
                        {
                            qDebug() << "caught exeption in dwgin_command";
                        }
                        cadwindow->prompt->normalprompt("Importer finished");
                    }
                }
            }
        }
    }
}

#if 1
// v8  version
void dxfin_command(int *cstate,HighLevelEvent *,void **)
{
    // get the file name
    QString ftype;
    QString path(v.getstring("db::last-dxf-file-path"));
    if(path.isEmpty())
        path = QString(v.getstring("db::my-drawings-path"));
    if(path.isEmpty())
        path = QString(QDir::homePath());
#ifdef _MAC
    QString infileName = QFileDialog::getOpenFileName(0,"DXF Import", path, "DXF Files (*.dxf *.DXF)",&ftype);
    //QString infileName = QFileDialog::getOpenFileName(0,"DXF Import", "/", "DXF Files (*.dxf *.DXF)",&ftype,QFileDialog::DontUseNativeDialog);
#else
    QString infileName = QFileDialog::getOpenFileName(0,"DXF Import", path, "DXF Files (*.dxf *.DXF)");
#endif
    if( ! infileName.isEmpty() )
    {
        open_dwgdxf(infileName.data());
        path = QFileInfo(infileName).filePath();
        v.setstring("db::last-dxf-file-path",path.data());
        WritePrivateProfileString(_RCT("Strings"),_RCT("db::last-dxf-file-path"),
                                  path.data(),home.getinifilename());

    }
    *cstate = ExitState;
}
#endif
#if 0
    void dxfin_command(int *cstate,HighLevelEvent *,void **)
    {
    // v7 version
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libImporter.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Importer.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
            convert(0,0); // dxf import
    }

    *cstate = ExitState;
}
#endif
#if 0
// v5 version
void dxfin_command(int *cstate,HighLevelEvent *,void **)
{
    RCCHAR cadfilename[300],oldcadname[300],cadtitle[300];
    ResourceString rs(NCDXF);
    ResourceString rs1(NCDXF+1);
    *cstate = ExitState;
    nblocks = 0;

    if (! filechanged(rs.gets(),rs1.gets())) return;

    // get the current filename and title
    strcpy(oldcadname,db.getname());
    strcpy(cadtitle,db.gettitle());

    db.initialize();

    // put the old name and title back
    db.setname(oldcadname);
    db.settitle(cadtitle);
    cadwindow->updatetitle();

    DXFInConvert dxffile;
    if (dxffile.setscale() && dxffile.open(0))
    {
        program->setbusy(1);
        dxffile.loadcolourtable();
        dxffile.read();
        dxffile.close();
        program->fileopendxfapps();
        program->setbusy(0);
    }
    if (cadwindow->getcurrentsurface() != NULL)
    zoom_command6(cstate,0,0);
    cadwindow->drawcallback(0,NULL,regenerateall);

    strcpy(cadfilename,dxffile.getname());
    if (strchr(cadfilename,'.') != 0)
        *strchr(cadfilename,'.') = 0;
    strcat(cadfilename,".cad");

    QString msg = QString("This file already exists.  Overwrite?\n%1").arg(QString(cadfilename));
    if ( !QFile(QString(cadfilename)).exists() || RCMessageBox(cadwindow->gethwnd(),msg.data(),_RCT("Save"),MB_YESNO) == IDYES )
    {
        strcpy(oldcadname,db.getname());
        QString qcadfilename(cadfilename);
        qcadfilename = qcadfilename.mid(qcadfilename.lastIndexOf(QDir::separator())+1);
        qcadfilename = qcadfilename.left(qcadfilename.length()-4);
        strcpy(cadtitle,qcadfilename.data());
        strcpy(oldcadname,db.getname());
        CadFile outfile;
        db.setname(cadfilename);
        if (outfile.open(db.getname(),"wb"))
        {
            db.save(&outfile);
            outfile.close();
            db.settitle(cadtitle);
            cadwindow->updatetitle();
            cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
        }
        else
            db.setname(oldcadname);

    }
}
#endif

#if 1
// v8 version
void dwgin_command(int *cstate,HighLevelEvent *,void **)
{
#if 0
    QLibrary *appinstance;
    Convert convert;
    // unit test
    cadwindow->startdrawingbatch();
    for(int i=0 ; i<100; i++)
    {
        qInfo() << "init #" << i;
        try
        {
            db.initialize();
        }
        catch(...)
        {
            qDebug() << "caught exception at" << i;
        }
    }
    cadwindow->enddrawingbatch();
    *cstate = ExitState;
#endif // unit test

    // get the file name
    QString ftype;
    QString path(v.getstring("db::last-dwg-file-path"));
    if(path.isEmpty())
        path = QString(v.getstring("db::my-drawings-path"));
    if(path.isEmpty())
        path = QString(QDir::homePath());
#ifdef _MAC
    QString infileName = QFileDialog::getOpenFileName(0,"DWG Import", path, "DWG Files (*.dwg *.DWG)",&ftype);
    //QString infileName = QFileDialog::getOpenFileName(0,"DWG Import", "/", "DWG Files (*.dwg *.DWG)",&ftype,QFileDialog::DontUseNativeDialog);
#else
    QString infileName = QFileDialog::getOpenFileName(0,"DWG Import", path, "DWG Files (*.dwg *.DWG)");
#endif
    if( ! infileName.isEmpty() )
    {
        open_dwgdxf(infileName.data());
        path = QFileInfo(infileName).filePath();
        v.setstring("db::last-dwg-file-path",path.data());
        WritePrivateProfileString(_RCT("Strings"),_RCT("db::last-dwg-file-path"),
                                  path.data(),home.getinifilename());
    }
    *cstate = ExitState;
}
#endif
#if 0
void dwgin_command(int *cstate,HighLevelEvent *,void **)
{
    // moved to open_dwgdfx() function
    // enhancement to use the DWGConvert application to preprocess DWG/DXF files
    // assuming the DWGConvert application path is in the ini file
    // the options dialog is used on mac and windows
    // the preprocessor is only available on windows
    // ont the mac...
    // 1. put up the options dialog
    // 2. ask for the input dwg file name
    // run convert with that file name
    // on windows...
    // 1. put up the options dialog
    // 2. ask for the input file name
    // 3. if preprocessing is selected
    // 4.   preprocess the dwg file
    // 5.   run convert with the pp output file
    // 6.   rename the converted file to the <selected file name>.cad
    // 8. else if not preprocessing
    // 9.   run convert with the selected file
    //

    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libImporter.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Importer.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
        {
            // get the file name
#ifdef _MAC
            QString infileName = QFileDialog::getOpenFileName(0,"DWG Import", "/", "DWG Files (*.dwg *.DWG)",QFileDialog::DontUseNativeDialog);
#else
            QString infileName = QFileDialog::getOpenFileName(0,"DWG Import", "/", "DWG Files (*.dwg *.DWG)");
#endif
            if( ! infileName.isEmpty() )
            {
                // put up the DWG import options dialog
                Dialog dialog("DWGImportOptions_Dialog");
                if(dialog.process())
                {
                    QString dcwinapp(v.getstring("db::dwg-preprocessor-path"));
                    if(QFileInfo::exists(dcwinapp))
                    {
                        QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
                        QString outfileName = infileName.toUpper().replace(".DWG","-fixed.DWG");
                        QStringList arguments;

                        // build the arguments for DWGConvert from the dialog settings
                        int iExplode = 0;
                        QString switchesStr;

                        if(settings.value("dg::in-use_advanced_options",0).toInt() == 1)
                        {
                            arguments << infileName << outfileName;

                            if(settings.value("dg::in-purge_dwg",0).toInt() == 1);
                                arguments << "/PURGE=2"; // purging all and empty blocks
                            if(settings.value("dg::in-visible_layers_only",0).toInt() == 1)
                                arguments <<  "/INVIS=1";
                            if(settings.value("dg::in-model_space_only",0).toInt() == 1)
                                arguments <<  "/LAYOUT=Model";
                            if(settings.value("dg::in-polylines_to_lines",0).toInt() == 1)
                                arguments <<  "/XP=0";
                            if(settings.value("dg::in-explode_blocks",0).toInt() == 1)
                                iExplode = 16;
                            if(settings.value("dg::in-proxy_to_blocks",0).toInt() == 1)
                                iExplode += 256;
                            if(settings.value("dg::in-explode_hatch_patterns",0).toInt() == 1)
                                iExplode += 512;
                            if(settings.value("dg::in-xrefs_to_blocks",0).toInt() == 1)
                                iExplode += 33554432;

                            if(iExplode > 0)
                                switchesStr += QString("/x=%1").arg(iExplode);

                            arguments << switchesStr;

                            qDebug() << "switchesStr : " << switchesStr;
                            qDebug() << "arguments : " << arguments;

                            // delete the output file
                            QFileInfo outFileInfo(outfileName);
                            if(outFileInfo.exists() && !outFileInfo.isDir())
                                QFile::remove(outfileName);

                            //QProcess::startDetached(dcwinapp, arguments);
                            QProcess process(app);
                            process.start(dcwinapp,arguments);
                            if(process.waitForStarted(5000))
                            {
                                qDebug() << "process started";
                                cadwindow->prompt->normalprompt("Preprocessing started");
                                if(process.waitForFinished(-1))
                                {
                                    qDebug() << "process finished OK";
                                    // look for the output file
                                    if(QFileInfo::exists(outfileName))
                                    {
                                        qDebug() << "Output file created OK";
                                        cadwindow->prompt->normalprompt("Preprocessing finished");
                                        infileName = outfileName;
                                    }
                                    else
                                    {
                                        qDebug() << "Ouput file not created";
                                        cadwindow->prompt->normalprompt("Preprocessing failed");
                                        QMessageBox::critical(0,"DWG Import","The preprocessor application failed to convert the file.\n\nCheck your setup and try again or\ncontact CAD International support for assistance.");
                                        infileName.clear();
                                    }
                                }
                                else
                                {
                                    qDebug() << "process finished error";
                                    cadwindow->prompt->normalprompt("Preprocessing error");
                                    QMessageBox::critical(0,"DWG Import","The preprocessor application finshed with an error.\n\nCheck your setup and try again or\ncontact CAD International support for assistance.");
                                    infileName.clear();
                                }
                            }
                            else
                            {
                                qDebug() << "process not started";
                                cadwindow->prompt->normalprompt("Preprocessor could not start");
                                QMessageBox::critical(0,"DWG Import","The preprocessor application could not start.\n\nCheck your setup and try again or\ncontact CAD International support for assistance.");
                                infileName.clear();
                            }
                        }
                    }
                    // try to import the file
                    if( ! infileName.isEmpty() )
                    {
                        open_dwg(infileName.data());
                        /*
                        try
                        {
                            char chinfileName[600]={0};
                            strcpy(chinfileName,infileName.data());

                            cadwindow->prompt->normalprompt("Importer starting");
                            convert(1,chinfileName); // dwg import
                            //convert(1,0); // dwg import
                        }
                        catch(...)
                        {
                            qDebug() << "caught exeption in dwgin_command";
                        }
                        */
                        cadwindow->prompt->normalprompt("Importer finished");
                    }
                }
            }
        }
    }
    *cstate = ExitState;
}
#endif
#if 0
void dwgin_command(int *cstate,HighLevelEvent *,void **)
{
    // v7 version
    if( ! infileName.isEmpty() )
    {
#ifdef _MAC
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libImporter.1.0.0.dylib")));
#else
        appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Importer.dll")));
#endif

        if(appinstance)
        {
            convert = (Convert) appinstance->resolve("Convert");
            if(convert)
            {
                try
                {
                    char chinfileName[600];
                    strcpy(chinfileName,infileName.data());

                    convert(1,chinfileName); // dwg import
                    //convert(1,0); // dwg import
                    //state.setdragging(2);
                    //cadwindow->paintall();
                }
                catch(...)
                {
                    qDebug() << "caught exeption in dwgin_command";
                }
            }
        }
    }
    *cstate = ExitState;
}
#endif
#if 0
// v5 version
void dwgin_command(int *cstate,HighLevelEvent *,void **)
{
    RCOPENFILENAME ofn;
    ResourceString rs(NCDXF);
    ResourceString rs1(NCDXF+1);
    ResourceString rs2(4072);
    RCCHAR dirname[300],pfilename[300],lfilename[300],filename[300],allfilenames[60000],filter[300];
    RCCHAR oldcadname[300],cadtitle[300];
    char cadfilename[300],command[300],parameter[300],dwgfilename[300],dxffilename[300];
    RCUINT  i;
    RC_STAT sbuf;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    //RCHINSTANCE hinstance;
    Convert convert;
    //Recover recover;
    RCCHAR *next_file;


    *cstate = ExitState;

    if (! filechanged(rs.gets(),rs1.gets())) 
        return;


    //qDebug() << "dll path :" << QString(home.getexecutablefilename(_RCT("libRealCADOpenDWG.1.0.0.dylib")));
#ifdef _MAC
    //QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("libRealCADOpenDWG.1.0.0.dylib"))),app);
    QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("libImporter.1.0.0.dylib"))),app);
#else
    //QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("RealCADOpenDWG.dll"))),app);
    QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("libImporter.dll"))),app);
#endif

    convert = 0;
    //recover = 0;

    if(hinstance != 0 && hinstance->load())
    {
        //convert = (Convert) GetProcAddress(hinstance,"Convert");
        //recover = (Recover) GetProcAddress(hinstance,"Recover");
        //convert = (Convert) hinstance->resolve("Convert");
        //recover = (Recover) hinstance->resolve("Recover");
        convert = (Convert) hinstance->resolve("Convert");
    }

    if (convert == 0 /*|| recover == 0*/)
    {
        cadwindow->MessageBox("DWG import requires the DWG Converter plugin.\nPlease contact CAD International for more information","Import DWG",MB_OK);
        return;
    }
    /*
     * // KMJ: pending
    if (convert == 0 || recover == 0)
    {   strcpy(pfilename,home.getexecutablefilename(_RCT("dconvert.exe")));
        strcpy(parameter,"DXF");
        if (stat(pfilename,&sbuf) != 0)
        {   strcpy(pfilename,home.getexecutablefilename(_RCT("dconvertcon.exe")));
            strcpy(parameter,"-DXF");
            if (stat(pfilename,&sbuf) != 0)
              {  cadwindow->MessageBox("DWG batch convert requires dconvert (DWG Convertor) utility.\nIt can be downloaded from the OpenDWG web site for free.\nhttp://www.opendwg.org/downloads/guest.htm\nSave the converter into the same directory as this application exe.","Import DWG",MB_OK);
                 return;
              }
        }
    }
    */

    _getcwd(dirname, sizeof(dirname));
    strcpy(filename,"");
    strcpy(filter,"AutoCAD DWG Files (*.DWG *.dwg)");
    //for (i = 0; filter[i] != '\0'; i++)
    //    if (filter[i] == '|')
    //        filter[i] = '\0';
    memset(&ofn, 0, sizeof(OPENFILENAME)); 
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = cadwindow->gethwnd();
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = allfilenames;
    ofn.nMaxFile = sizeof(allfilenames);
    ofn.lpstrInitialDir = dirname;
    ofn.lpstrDefExt = _RCT("dwg");
    ofn.lpstrTitle =  rs2.gets();
    //ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT |  OFN_EXPLORER;
    ofn.Flags = MAKEWPARAM(0,QFileDialog::ExistingFiles);

    if (GetOpenFileName(&ofn))
    {  

        qDebug() << "allfilenames" << QString(allfilenames);
        next_file = allfilenames + ofn.nFileOffset;
        qDebug() << "next_file" << QString(next_file);
        while (*next_file != 0)
        {
            if (strlen(allfilenames) > ofn.nFileOffset)
            {
                strcpy(filename,allfilenames);
                next_file = allfilenames + strlen(allfilenames);
            }
            else
            {
                strcpy(filename,allfilenames);
                strcat(filename,"/");
                strcat(filename,next_file);
                next_file += strlen(next_file) + 1;
            }

            nblocks = 0;

            // get the current filename and title
            strcpy(oldcadname,db.getname());
            strcpy(cadtitle,db.gettitle());

            db.initialize();

            // put the old name and title back
            db.setname(oldcadname);
            db.settitle(cadtitle);
            cadwindow->updatetitle();

            strcpy(lfilename,filename);
            _strlwr(lfilename);
            if (strstr(lfilename,".dwg") != 0)
            {
                filename[strstr(lfilename,".dwg")-lfilename] = 0;
                // convert to local separators
                strcpy(filename,QDir::toNativeSeparators(QString(filename)).data());

                strcpy(dwgfilename,filename);  strcat(dwgfilename,".dwg");
                strcpy(dxffilename,filename);  strcat(dxffilename,".dxf");
                strcpy(cadfilename,filename);  strcat(cadfilename,".cad");

                if (convert != 0)
                {
                    program->setbusy(1);
                    // KMJ using char for the moment
                    if (convert(1,dwgfilename,dxffilename) == 0)
                    {   if (cadwindow->MessageBox("An error occured while reading the file.  Do you want to try to recover the file?","Recover",MB_YESNO) == IDYES)
                        {   if (recover(1,dwgfilename,dxffilename) == 0)
                            {   cadwindow->MessageBox("An error occured while recovering the file.  The file cannot be opened.","Recover",MB_OK);
                                return; 
                            }
                        }
                   }
                   program->setbusy(0);
                }
                /*
                 * KMJ: pending
                else
                {
                    sprintf(command,"%s \"%s.DWG\" %s",pfilename,filename,parameter);

                    memset(&si,0,sizeof(si));
                    si.cb = sizeof(si);
                    si.dwFlags = STARTF_USESHOWWINDOW;
                    si.wShowWindow = SW_HIDE;
                    CreateProcess(0,command,0,0,FALSE,CREATE_NEW_PROCESS_GROUP,0,0,&si,&pi);
                    WaitForSingleObject(pi.hProcess,100000L);
                }
                */
           
                strcat(filename,".dxf");
                DXFInConvert dxffile;
                if (dxffile.setscale())
                {
                    program->setbusy(1);
                    if (dxffile.open(filename))
                    {   dxffile.loadcolourtable();
                        dxffile.read();
                        dxffile.close();
                        program->fileopendxfapps();
                     }
                    program->setbusy(0);
                }
                if (cadwindow->getcurrentsurface() != NULL)
                    zoom_command6(cstate,0,0);
                cadwindow->drawcallback(0,NULL,regenerateall);

                QString msg = QString("This file already exists.  Overwrite?\n%1").arg(QString(cadfilename));
                if ( !QFile(QString(cadfilename)).exists() || RCMessageBox(cadwindow->gethwnd(),msg.data(),_RCT("Save"),MB_YESNO) == IDYES )
                {
                    strcpy(oldcadname,db.getname());
                    QString qcadfilename(cadfilename);
                    qcadfilename = qcadfilename.mid(qcadfilename.lastIndexOf(QDir::separator())+1);
                    qcadfilename = qcadfilename.left(qcadfilename.length()-4);
                    strcpy(cadtitle,qcadfilename.data());
                    CadFile outfile;
                    db.setname(cadfilename);
                    if (outfile.open(db.getname(),"wb"))
                    {
                        db.save(&outfile);
                        outfile.close();
                        db.settitle(cadtitle);
                        cadwindow->updatetitle();
                        cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
                    }
                    else
                        db.setname(oldcadname);

                }
                _unlink(filename);
            }
        } 
    }

    FreeLibrary(hinstance);
}
#endif



void open_dxf(RCCHAR *fileName)
{
#if 1
    open_dwgdxf(fileName);
#else
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libImporter.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Importer.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
            convert(0,QString(filename).toLatin1().data()); // dwg import
    }
#endif
}

void open_dwg(RCCHAR *fileName)
{
#if 1
    open_dwgdxf(fileName);
#else
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libImporter.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Importer.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
        {
            try
            {
                qInfo() << "open_dwg start";
                convert(1,QString(filename).toLatin1().data()); // dwg import
                qInfo() << "open_dwg end";
            }
            catch(...)
            {
                qDebug() << "caught exeption in open_dwg()";
            }
        }
    }
#endif
}
#else
void open_dwg(RCCHAR *filename)
{
 RCCHAR pfilename[300],lfilename[300],command[300],parameter[300];
 RCCHAR oldcadname[300],cadtitle[300];
 char dwgfilename[300],dxffilename[300];
 RC_STAT sbuf;
 RCSTARTUPINFO si;
 RCPROCESS_INFORMATION pi;
 //RCHINSTANCE hinstance;
 Convert convert;
 //Recover recover;
 ResourceString rs(NCDXF);
 ResourceString rs1(NCDXF+1);
 int cstate;

  nblocks = 0;
  if (! filechanged(rs.gets(),rs1.gets())) return;

  db.initialize();

#ifdef _MAC
    //QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("libRealCADOpenDWG.1.0.0.dylib"))),app);
    QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("liblibImporter.1.0.0.dylib"))),app);
#else
    //QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("RealCADOpenDWG.dll"))),app);
    QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("libImporter.dll"))),app);
#endif

  convert = 0;
  //recover =0;

  if (hinstance != 0 && hinstance->load())
  {
       //convert = (Convert) GetProcAddress(hinstance,"Convert");
       //recover = (Recover) GetProcAddress(hinstance,"Recover");
       convert = (Convert) hinstance->resolve("Convert");
       //recover = (Recover) hinstance->resolve("Recover");
  }

  if (convert == 0 /*|| recover == 0*/)
  {
      cadwindow->MessageBox("DWG import requires the DWG Converter plugin.\nPlease contact CAD International for more information","Import DWG",MB_OK);
      return;
  }


  /*
  if (convert == 0 || recover == 0)
    {  strcpy(pfilename,home.getexecutablefilename(_RCT("dconvert.exe")));
       strcpy(parameter,"DXF");
       if (stat(pfilename,&sbuf) != 0)
         {  strcpy(pfilename,home.getexecutablefilename(_RCT("dconvertcon.exe")));
            strcpy(parameter,"-DXF");
            if (stat(pfilename,&sbuf) != 0)
              {  cadwindow->MessageBox("DWG batch convert requires dconvert (DWG Convertor) utility.\nIt can be downloaded from the OpenDWG web site for free.\nhttp://www.opendwg.org/downloads/guest.htm\nSave the converter into the same directory as this application exe.","Import DWG",MB_OK);
                 return;
              }
         }
    }
    */

#if 0 // old version
   strcpy(lfilename,filename);
   _strlwr(lfilename);
   if (strstr(lfilename,".dwg") != 0)
     {
        program->setbusy(1);
        filename[strstr(lfilename,".dwg")-lfilename] = 0;

        strcpy(dwgfilename,filename);  strcat(dwgfilename,".dwg");
        strcpy(dxffilename,filename);  strcat(dxffilename,".dxf");

        if (convert != 0)
          {  if (convert(1,dwgfilename,dxffilename) == 0)
               {  if (cadwindow->MessageBox("An error occured while reading the file.  Do you want to try to recover the file?","Recover",MB_YESNO) == IDYES)
                    {  if (recover(1,dwgfilename,dxffilename) == 0)
                         {  cadwindow->MessageBox("An error occured while recovering the file.  The file cannot be opened.","Recover",MB_OK);
                            return;
                         }
                    }
               }
          }
        /*
        else
          {  sprintf(command,"%s \"%s.DWG\" %s",pfilename,filename,parameter);

             memset(&si,0,sizeof(si));
             si.cb = sizeof(si);
             si.dwFlags = STARTF_USESHOWWINDOW;
             si.wShowWindow = SW_HIDE;
             CreateProcess(0,command,0,0,FALSE,CREATE_NEW_PROCESS_GROUP,0,0,&si,&pi);
             WaitForSingleObject(pi.hProcess,100000L);
          }
        */

        strcat(filename,".dxf");
        DXFInConvert dxffile;
        if (dxffile.setscale())
          {  if (dxffile.open(filename))
               {  dxffile.loadcolourtable();
                  dxffile.read();
                  dxffile.close();
                  program->fileopendxfapps();
               }
          }
        program->setbusy(0);
        cstate = InitialState;
        if (cadwindow->getcurrentsurface() != NULL)
          zoom_command6(&cstate,0,0);
        cadwindow->drawcallback(0,NULL,regenerateall);

        _unlink(filename);

     }


  FreeLibrary(hinstance);
#endif
}
#endif
