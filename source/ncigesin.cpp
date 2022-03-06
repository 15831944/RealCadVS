
#include "ncwin.h"

#if MENUS != STUDENTMENUS

class IGESRecord
  {private:
     long offset;
     char line[82];
     char section;
     long seqno;
   public:
     IGESRecord(long,char *);
     char getsection(void)  {  return section;  }
     long getseqno(void)  {  return seqno;  }
     long getoffset(void)  {  return offset;  }
     char *getline(void)  {  return line;  }
  };

class IGESDERecord
  {private:
     long type,pdata,structure,font,level,view,matrix,
          labelassoc,status,weight,colour,npdata,form,subscript;
     char label[9];
     long readlong(char *);
   public:
     IGESDERecord(IGESRecord *,IGESRecord *);
     long gettype(void)  {  return type;  }
     long getpdata(void)  {  return pdata;  }
     long getstructure(void)  {  return structure;  }
     long getfont(void)  {  return font;  }
     long getlevel(void)  {  return level;  }
     long getmatrix(void)  {  return matrix;  }
     long getlabelassoc(void)  {  return labelassoc;  }
     long getstatus(void)  {  return status;  }
     long getweight(void)  {  return weight;  }
     long getcolour(void)  {  return colour;  }
     long getnpdata(void)  {  return npdata;  }
     long getform(void)  {  return form;  }
     long getsubscript(void)  {  return subscript;  }
     char *getlabel(void)  {  return label;  }
  };

class FreeFormatData
  {private:
     IGESIn *igesin;
     IGESRecord **ir;
     char data[IGES_LINE_LENGTH];
     int pos,len;
   public:
     FreeFormatData(IGESIn *,IGESRecord **,int);
     int discardblanks(void);
     int nextchar(void);
     int readlong(const long,long *);
     int readdouble(const double,double *);
     int readstring(const char *, const int, char *);
     //int readstring(const char *,const int,RCCHAR *);
     //int readstring(const char *,const int,char *);
     int readpointer(const long,long *);
     int readstatement(const char *,const int,char *);
     int readlogical(const long,long *);
  };

IGESRecord::IGESRecord(long off,char *l)
{char *endptr;
  strcpy(line,l);
  offset = off;
  if (strlen(line) < 74)
    section = 'E';
  else
    {  section = line[72];
       seqno = strtol(line+73,&endptr,10);
       if (*endptr != 0 && *endptr != '\n' && *endptr != '\r')
         section = 'E';
    }
}

//
//  IGESIn Implementation
//

IGESIn::~IGESIn()
{ if (infile != NULL) fclose(infile);
}

long IGESIn::ftell(void)
{ if (infile == 0)
    return 0L;
  else
    return ::ftell(infile);
}

void IGESIn::fseek(const long o)
{ if (infile != 0)
    ::fseek(infile,o,0);
}

void IGESIn::fwrite(const char *record)
{ if (infile != 0)
    ::fwrite(record,1,ll,infile);
}

void IGESIn::close(void)
{ if (infile != NULL) fclose(infile);
  infile = NULL;
}

int IGESIn::open(RCCHAR *s)
{RCOPENFILENAME ofn;
 RCCHAR szDirName[256]={0},szFile[256]={0}, szFileTitle[256]={0},title[256]={0},sfilename[256]={0},szFilter[300]={0};
 RCCHAR defExt[256]={0};
 UINT  i;
  if (s == 0)
    {  _getcwd(szDirName, sizeof(szDirName));
       szFile[0] = '\0';
       _tcscpy(szFilter,TEXT("IGES Files  (*.igs *.IGS)"));
       //for (i = 0; szFilter[i] != '\0'; i++)
       //  if (szFilter[i] == '|') szFilter[i] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = szFilter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile= szFile;
       ofn.nMaxFile = sizeof(szFile);
       ofn.lpstrFileTitle = szFileTitle;
       ofn.nMaxFileTitle = sizeof(szFileTitle);
       ofn.lpstrInitialDir = szDirName;
       strcpy(defExt,"IGS");
       ofn.lpstrDefExt = defExt;
       ofn.lpstrFileTitle = title;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
       infile = 0;
       if (! GetOpenFileName(&ofn)) return 0;
       _tcscpy(sfilename,ofn.lpstrFile);
       return (infile = fopen(ofn.lpstrFile,TEXT("rb"))) != NULL;
    }
  else
    {  
#if ! defined(_WIN32_WCE)
       _tcscpy(sfilename,s);
//WCEFIX
#endif
       return (infile = fopen(s,"rb")) != NULL;
    }
}

IGESRecord *IGESIn::getnextrecord(void)
{char line[IGES_LINE_LENGTH];
 long offset;
 IGESRecord *ir;
  if (infile == 0) return 0;
  offset = ::ftell(infile);
  if (fgets(line,IGES_LINE_LENGTH,infile) == 0) return 0;
  line[IGES_LINE_LENGTH-1] = 0;
  ir = new IGESRecord(offset,line);
  if (ir == 0 || ir->getsection() == 'E')
    {  delete ir;
       return 0;
    }
  if (ll == 0)
    ll = int(::ftell(infile) - offset);
  return ir;
}

void IGESIn::processglobalsection(IGESRecord **ir)
{FreeFormatData ffd(this,ir,72);
  ffd.readstring(",",2,parameterdelim);
  ffd.readstring(";",2,recorddelim);
  ffd.readstring("",STRING_LENGTH,productidout);
  ffd.readstring("",STRING_LENGTH,filename);
  ffd.readstring("",STRING_LENGTH,systemid);
  ffd.readstring("",STRING_LENGTH,version);
  ffd.readlong(32,&bits);
  ffd.readlong(38,&sprange);
  ffd.readlong(6,&spprecision);
  ffd.readlong(308,&dprange);
  ffd.readlong(15,&dpprecision);
  ffd.readstring("",STRING_LENGTH,productidin);
  ffd.readdouble(1.0,&modelscale);
  ffd.readlong(2,&units);
  ffd.readstring("MM",STRING_LENGTH,sunits);
  ffd.readlong(1,&nwidths);
  ffd.readdouble(1.0,&maxwidth);
  ffd.readstring("",STRING_LENGTH,datetime);
  ffd.readdouble(1.0E-10,&granularity);
  ffd.readdouble(1.0E-10,&size);
  ffd.readstring("",STRING_LENGTH,author);
  ffd.readstring("",STRING_LENGTH,organization);
  ffd.readlong(6,&igesversion);
  ffd.readlong(0,&standard);
}

void IGESIn::processtext(long,long o1,long seqno)
{IGESRecord *ir;
 int i;
 long o2,type,nstrings,nchars,font,mirror,rotate;
 double width,height,slant,rotation,x,y,z;
 char *string,estring[2];
  o2 = ::ftell(infile);
  ::fseek(infile,o1 + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readlong(0,&nstrings);
      for (i = 0 ; i < nstrings ; i++)
        {  ffd.readlong(0,&nchars);
           ffd.readdouble(0.0,&width);
           ffd.readdouble(0.0,&height);
           ffd.readlong(0,&font);
           ffd.readdouble(0.0,&slant);
           ffd.readdouble(0.0,&rotation);
           ffd.readlong(0,&mirror);
           ffd.readlong(0,&rotate);
           ffd.readdouble(0.0,&x);
           ffd.readdouble(0.0,&y);
           ffd.readdouble(0.0,&z);
           string = new char[int(nchars+1)];
           if (string == 0)
             ffd.readstring("",2,estring);
           else
             {  ffd.readstring("",int(nchars+1),string);
                Point p(x,y,z),xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
                BitMask options(32);
                Text *text = new Text(h,CHAR2RCCHR("Arial"),p,xa,ya,0.0,height,0.0,rotation*180.0/M_PI,0,CHAR2RCCHR(string),0,options);
                if (text != 0 && text->getdefined())
                  db.geometry.add(text);
             }
        }
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::processline(long,long o1,long seqno)
{IGESRecord *ir;
 long o2,type;
 double x1,y1,z1,x2,y2,z2;
  o2 = ::ftell(infile);
  ::fseek(infile,o1 + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readdouble(0.0,&x1);
      ffd.readdouble(0.0,&y1);
      ffd.readdouble(0.0,&z1);
      ffd.readdouble(0.0,&x2);
      ffd.readdouble(0.0,&y2);
      ffd.readdouble(0.0,&z2);
      Point p1(x1,y1,z1),p2(x2,y2,z2);
      Line *line = new Line(h,p1,p2);
      if (line != 0 && line->getdefined())
        db.geometry.add(line);
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::processarc(long deoffset,long pdoffset,long matrix,long seqno)
{IGESRecord *ir,*ir2;
 long o2,type;
 double x1,y1,x2,y2,x3,y3,zt,a1,a2,dx,dy,sweep;
 Transform t;
  o2 = ::ftell(infile);
  if (matrix != 0)
    {  fseek(deoffset + (matrix - 1) * ll);
       ir = getnextrecord();
       if (ir != 0 && ir->getsection() == 'D')
         {  ir2 = getnextrecord();
            if (ir2 != 0 && ir2->getsection() == 'D')
              {IGESDERecord de(ir,ir2);
                 processmatrix(deoffset,pdoffset,de.getpdata(),t.mat.elements);
              }
         }
    }
  ::fseek(infile,pdoffset + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readdouble(0.0,&zt);
      ffd.readdouble(0.0,&x1);
      ffd.readdouble(0.0,&y1);
      ffd.readdouble(0.0,&x2);
      ffd.readdouble(0.0,&y2);
      ffd.readdouble(0.0,&x3);
      ffd.readdouble(0.0,&y3);
      Point xaxis(t.mat(0,0),t.mat(1,0),t.mat(2,0)),
            yaxis(t.mat(0,1),t.mat(1,1),t.mat(2,1)),
            zaxis(t.mat(0,2),t.mat(1,2),t.mat(2,2)),
            origin(t.mat(0,3),t.mat(1,3),t.mat(2,3));
      origin += xaxis * x1 + yaxis * y1 + zaxis * zt;
      dx = x2 - x1;
      dy = y2 - y1;
      Circle c(origin,xaxis,yaxis,sqrt(dx*dx + dy*dy),0.0,M_PI*2.0);
      a1 = c.angle(origin+xaxis*(x2-x1)+yaxis*(y2-y1));
      a2 = c.angle(origin+xaxis*(x3-x1)+yaxis*(y3-y1));
      sweep = a2 - a1;
      while (sweep < 0.0) sweep += M_PI * 2.0;
      if (sweep < 1.0E-5) sweep = 2.0 * M_PI;
      Circle *circle = new Circle(h,origin,xaxis,yaxis,sqrt(dx*dx + dy*dy),a1,sweep);
      if (circle != 0 && circle->getdefined())
        db.geometry.add(circle);
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::processellipse(long deoffset,long pdoffset,long matrix,long seqno)
{IGESRecord *ir,*ir2;
 long o2,type;
 double a,b,c,d,e,f,x1,y1,x2,y2,zt,a1,a2,sweep;
 Transform t;
  o2 = ::ftell(infile);
  if (matrix != 0)
    {  fseek(deoffset + (matrix - 1) * ll);
       ir = getnextrecord();
       if (ir != 0 && ir->getsection() == 'D')
         {  ir2 = getnextrecord();
            if (ir2 != 0 && ir2->getsection() == 'D')
              {IGESDERecord de(ir,ir2);
                 processmatrix(deoffset,pdoffset,de.getpdata(),t.mat.elements);
              }
         }
    }
  ::fseek(infile,pdoffset + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readdouble(0.0,&a);
      ffd.readdouble(0.0,&b);
      ffd.readdouble(0.0,&c);
      ffd.readdouble(0.0,&d);
      ffd.readdouble(0.0,&e);
      ffd.readdouble(0.0,&f);
      ffd.readdouble(0.0,&zt);
      ffd.readdouble(0.0,&x1);
      ffd.readdouble(0.0,&y1);
      ffd.readdouble(0.0,&x2);
      ffd.readdouble(0.0,&y2);
      Point xaxis(t.mat(0,0),t.mat(1,0),t.mat(2,0)),
            yaxis(t.mat(0,1),t.mat(1,1),t.mat(2,1)),
            zaxis(t.mat(0,2),t.mat(1,2),t.mat(2,2)),
            origin(t.mat(0,3),t.mat(1,3),t.mat(2,3));
      origin += xaxis * x1 + yaxis * y1 + zaxis * zt;
      EllipseE e(origin,xaxis,yaxis,sqrt(-f / a),sqrt(-f / c),0.0,M_PI*2.0);
      a1 = e.angle(origin+xaxis*x1+yaxis*y1);
      a2 = e.angle(origin+xaxis*x2+yaxis*y2);
      while (a2 < a1) a2 += M_PI * 2.0;
      if (a2 < 1.0E-5) a2 = 2.0 * M_PI;
      a1 = e.toparametric(a1);
      a2 = e.toparametric(a2);
      sweep = a2 - a1;
      EllipseE *ellipse = new EllipseE(h,origin,xaxis,yaxis,sqrt(-f / a),sqrt(-f / c),a1,sweep);
      if (ellipse != 0 && ellipse->getdefined())
        db.geometry.add(ellipse);
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::processpoint(long deoffset,long pdoffset,long matrix,long seqno)
{IGESRecord *ir,*ir2;
 long o2,type;
 double x,y,z;
 Transform t;
  o2 = ::ftell(infile);
  if (matrix != 0)
    {  fseek(deoffset + (matrix - 1) * ll);
       ir = getnextrecord();
       if (ir != 0 && ir->getsection() == 'D')
         {  ir2 = getnextrecord();
            if (ir2 != 0 && ir2->getsection() == 'D')
              {IGESDERecord de(ir,ir2);
                 processmatrix(deoffset,pdoffset,de.getpdata(),t.mat.elements);
              }
         }
    }
  ::fseek(infile,pdoffset + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readdouble(0.0,&x);
      ffd.readdouble(0.0,&y);
      ffd.readdouble(0.0,&z);
      Point xaxis(t.mat(0,0),t.mat(0,1),t.mat(0,2)),
            yaxis(t.mat(1,0),t.mat(1,1),t.mat(1,2)),
            zaxis(t.mat(2,0),t.mat(2,1),t.mat(2,2)),
            origin(t.mat(0,3),t.mat(1,3),t.mat(2,3));
      origin += xaxis * x + yaxis * y + zaxis * z;
      PointE *point = new PointE(h,origin);
      if (point != 0 && point->getdefined())
        db.geometry.add(point);
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::processbspline(long deoffset,long pdoffset,long matrix,long seqno)
{IGESRecord *ir,*ir2;
 long i,o2,type,n,degree,prop1,prop2,prop3,prop4;
 double *knot,*weight,t1,t2;
 Point *polygon;
 Transform t;
 BitMask o(32);
  o2 = ::ftell(infile);
  if (matrix != 0)
    {  fseek(deoffset + (matrix - 1) * ll);
       ir = getnextrecord();
       if (ir != 0 && ir->getsection() == 'D')
         {  ir2 = getnextrecord();
            if (ir2 != 0 && ir2->getsection() == 'D')
              {IGESDERecord de(ir,ir2);
                 processmatrix(deoffset,pdoffset,de.getpdata(),t.mat.elements);
              }
         }
    }
  ::fseek(infile,pdoffset + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readlong(0,&n);
      ffd.readlong(0,&degree);
      ffd.readlong(0,&prop1);
      ffd.readlong(0,&prop2);
      ffd.readlong(0,&prop3);
      ffd.readlong(0,&prop4);
      knot = new double [n + degree + 2];
      weight = new double [n+1];
      polygon = new Point[n+1];
      for (i = 0 ; i < n + degree + 2 ; i++)
        ffd.readdouble(i / double(n-1),&knot[i]);
      for (i = 0 ; i <= n ; i++)
        ffd.readdouble(1.0,&weight[i]);
      for (i = 0 ; i <= n ; i++)
        {  ffd.readdouble(1.0,&polygon[i].x);
           ffd.readdouble(1.0,&polygon[i].y);
           ffd.readdouble(1.0,&polygon[i].z);
        }
      ffd.readdouble(0.0,&t1);
      ffd.readdouble(1.0,&t2);

      Curve *curve = new Curve(h,n+1,degree,polygon,knot,weight,o);
      if (curve != 0 && curve->getdefined())
        db.geometry.add(curve);
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::processleader(long deoffset,long pdoffset,long matrix,long seqno)
{IGESRecord *ir,*ir2;
 long o2,type,i,n;
 double ad1,ad2,zt,x1,y1,x2,y2;
 Transform t;
 Point p1,p2;
  o2 = ::ftell(infile);
  if (matrix != 0)
    {  fseek(deoffset + (matrix - 1) * ll);
       ir = getnextrecord();
       if (ir != 0 && ir->getsection() == 'D')
         {  ir2 = getnextrecord();
            if (ir2 != 0 && ir2->getsection() == 'D')
              {IGESDERecord de(ir,ir2);
                 processmatrix(deoffset,pdoffset,de.getpdata(),t.mat.elements);
              }
         }
    }
  Point xaxis(t.mat(0,0),t.mat(0,1),t.mat(0,2)),
        yaxis(t.mat(1,0),t.mat(1,1),t.mat(1,2)),
        zaxis(t.mat(2,0),t.mat(2,1),t.mat(2,2)),
        origin(t.mat(0,3),t.mat(1,3),t.mat(2,3));
  ::fseek(infile,pdoffset + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readlong(0,&n);
      ffd.readdouble(0.0,&ad1);
      ffd.readdouble(0.0,&ad2);
      ffd.readdouble(0.0,&zt);
      ffd.readdouble(0.0,&x1);
      ffd.readdouble(0.0,&y1);
      for (i = 0 ; i < n ; i++)
        {  ffd.readdouble(0.0,&x2);
           ffd.readdouble(0.0,&y2);
           p1 = origin + xaxis*x1 + yaxis*y1 + zaxis*zt;
           p2 = origin + xaxis*x2 + yaxis*y2 + zaxis*zt;
           Line *line = new Line(h,p1,p2);
           if (line != 0 && line->getdefined())
             db.geometry.add(line);
           if (i == 0)
             {Point xa,ya;
                xa = (p2 - p1).normalize();
                ya = xa.cross(zaxis).normalize();
                Line *line = new Line(h,p1,p1+xa*ad1+ya*ad2/2.0);
                if (line != 0 && line->getdefined())
                  db.geometry.add(line);
                line = new Line(h,p1,p1+xa*ad1-ya*ad2/2.0);
                if (line != 0 && line->getdefined())
                  db.geometry.add(line);
                line = new Line(h,p1+xa*ad1+ya*ad2/2.0,p1+xa*ad1-ya*ad2/2.0);
                if (line != 0 && line->getdefined())
                  db.geometry.add(line);
             }
        }
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::processmatrix(long,long pdoffset,long seqno,double *matrix)
{IGESRecord *ir;
 long o2,type;
  o2 = ::ftell(infile);
  ::fseek(infile,pdoffset + (seqno - 1) * ll,0);
  ir = getnextrecord();
  if (ir != 0)
    {FreeFormatData ffd(this,&ir,64);
      ffd.readlong(0,&type);
      ffd.readdouble(0.0,matrix);
      ffd.readdouble(0.0,matrix+1);
      ffd.readdouble(0.0,matrix+2);
      ffd.readdouble(0.0,matrix+3);
      ffd.readdouble(0.0,matrix+4);
      ffd.readdouble(0.0,matrix+5);
      ffd.readdouble(0.0,matrix+6);
      ffd.readdouble(0.0,matrix+7);
      ffd.readdouble(0.0,matrix+8);
      ffd.readdouble(0.0,matrix+9);
      ffd.readdouble(0.0,matrix+10);
      ffd.readdouble(0.0,matrix+11);
      delete ir;
    }
  ::fseek(infile,o2,0);
}

void IGESIn::read(int usecolours)
{IGESRecord *ir,*ir2;
 long deoffset,pdoffset;
 long entitycount[1000];
  memset(entitycount,0,sizeof(entitycount));
  db.geometry.start();
  if (db.geometry.next() == 0)
    h.change(0,0,0,h.getweight());
  else
    h.change(1,1,0,h.getweight());
  while ((ir = getnextrecord()) != 0)
    {  while (ir != 0 && ir->getsection() == 'S')
         {  delete ir;
            ir = getnextrecord();
         }
       if (ir != 0 && ir->getsection() == 'G')
         {  processglobalsection(&ir);
         }
       if (ir != 0 && ir->getsection() == 'D')
         {  deoffset = ir->getoffset();
            while (ir != 0 && ir->getsection() != 'P')
              {  ir = getnextrecord();
              }
            if (ir != 0)
              pdoffset = ir->getoffset();
            fseek(deoffset);
            ir = getnextrecord();
            while (ir != 0 && ir->getsection() == 'D')
              {  ir2 = getnextrecord();
                 if (ir2 != 0 && ir2->getsection() == 'D')
                   {IGESDERecord de(ir,ir2);
                      entitycount[de.gettype()]++;
                      if (de.gettype() == 214)
                        {  if (usecolours)
                             h.change(de.getcolour(),de.getlevel(),de.getfont()-1,h.getweight());
                           else
                             h.change(h.getcolour(),h.getlayer(),de.getfont()-1,h.getweight());
                           processleader(deoffset,pdoffset,de.getmatrix(),de.getpdata());
                        }
                      else if (de.gettype() == 212)
                        {  if (usecolours)
                             h.change(de.getcolour(),de.getlevel(),de.getfont()-1,h.getweight());
                           else
                             h.change(h.getcolour(),h.getlayer(),de.getfont()-1,h.getweight());
                           processtext(deoffset,pdoffset,de.getpdata());
                           entitycount[212]++;
                        }
                      else if (de.gettype() == 110)
                        {  if (usecolours)
                             h.change(de.getcolour(),de.getlevel(),de.getfont()-1,h.getweight());
                           else
                             h.change(h.getcolour(),h.getlayer(),de.getfont()-1,h.getweight());
                           processline(deoffset,pdoffset,de.getpdata());
                        }
                      else if (de.gettype() == 100)
                        {  if (usecolours)
                             h.change(de.getcolour(),de.getlevel(),de.getfont()-1,h.getweight());
                           else
                             h.change(h.getcolour(),h.getlayer(),de.getfont()-1,h.getweight());
                           processarc(deoffset,pdoffset,de.getmatrix(),de.getpdata());
                        }
                      else if (de.gettype() == 116)
                        {  if (usecolours)
                             h.change(de.getcolour(),de.getlevel(),de.getfont()-1,h.getweight());
                           else
                             h.change(h.getcolour(),h.getlayer(),de.getfont()-1,h.getweight());
                           processpoint(deoffset,pdoffset,de.getmatrix(),de.getpdata());
                        }
                      else if (de.gettype() == 126)
                        {  if (usecolours)
                             h.change(de.getcolour(),de.getlevel(),de.getfont()-1,h.getweight());
                           else
                             h.change(h.getcolour(),h.getlayer(),de.getfont()-1,h.getweight());
                           processbspline(deoffset,pdoffset,de.getmatrix(),de.getpdata());
                        }
                      else if (de.gettype() == 104 && de.getform() == 1)
                        {  if (usecolours)
                             h.change(de.getcolour(),de.getlevel(),de.getfont()-1,h.getweight());
                           else
                             h.change(h.getcolour(),h.getlayer(),de.getfont()-1,h.getweight());
                           processellipse(deoffset,pdoffset,de.getmatrix(),de.getpdata());
                        }
 //else if (de.gettype() != 124)
 // {FILE *dfile = fopen("c:\\debug.log","a");
 // fprintf(dfile,"%d\n",de.gettype());
 // fclose(dfile);
  //}

                   }
                 delete ir;
                 delete ir2;
                 ir = getnextrecord();
              }
         }
       delete ir;
    }
  close();
//{FILE *dfile = fopen("debug.log","a");
// int i;
//   for (i = 0 ; i < 1000 ; i++)
//     if (entitycount[i] > 0)
//       fprintf(dfile,"Type %d Count %d\n",i,entitycount[i]);
//  fclose(dfile);
//  }
}

//
//  FreeFormatData Implementation
//

FreeFormatData::FreeFormatData(IGESIn *f,IGESRecord **r,int l)
{ strcpy(data,(*r)->getline());  igesin = f;  ir = r;  len = l;  pos = 0;
}

int FreeFormatData::nextchar(void)
{ if (*ir == 0) return 0;
  pos++;
  if (pos >= len)
    {  delete *ir;
       *ir = igesin->getnextrecord();
       if (*ir == NULL) return 0;
       strcpy(data,(*ir)->getline());
       pos = 0;
    }
  return 1;
}

int FreeFormatData::discardblanks(void)
{ if (*ir == 0) return 0;
  while (data[pos] == ' ')
    if (! nextchar()) return 0;
  return 1;
}

int FreeFormatData::readlong(const long def,long *value)
{char *endptr;
  if (! discardblanks()) return 0;
  *value = def;
  if (data[pos] == igesin->getrecorddelim())
    return 1;
  if (data[pos] == igesin->getparameterdelim())
    {  nextchar();
       return 1;
    }
  *value = strtol(data+pos,&endptr,10);
  pos = int(endptr - data);
  if (! discardblanks()) return 0;
  if (data[pos] != igesin->getparameterdelim() &&
      data[pos] != igesin->getrecorddelim())
    return 0;
  if (data[pos] == igesin->getparameterdelim())
    nextchar();
  return 1;
}

/*
int FreeFormatData::readstring(const char *def,const int slength1,char *value)
{
    return readstring(CHAR2RCCHR(def),  slength1,CHAR2RCCHR(value));
}

int FreeFormatData::readstring(const char *def,const int slength1,RCCHAR *value)
{
    return readstring(CHAR2RCCHR(def), slength1, value );
}
*/

int FreeFormatData::readstring(const char *def,const int slength1,char *value)
{char *endptr;
 int i,slength2;
  if (! discardblanks()) return 0;
  strcpy(value,def);
  if (data[pos] == igesin->getrecorddelim())
    return 1;
  if (data[pos] == igesin->getparameterdelim())
    {  nextchar();
       return 1;
    }
  slength2 = int(strtol(data+pos,&endptr,10));
  pos = int(endptr - data);
  if (data[pos] != 'H') return 0;
  nextchar();
  for (i = 0 ; i < slength2 ; i++)
    {  if (i < slength1)
         value[i] = data[pos];
       nextchar();
    }
  if (i >= slength1)
    i = slength1 - 1;
  value[i] = 0;
  if (! discardblanks()) return 0;
  if (data[pos] != igesin->getparameterdelim() &&
      data[pos] != igesin->getrecorddelim())
    return 0;
  if (data[pos] == igesin->getparameterdelim())
    nextchar();
  return 1;
}

int FreeFormatData::readdouble(const double def,double *value)
{char *endptr;
  if (! discardblanks()) return 0;
  *value = def;
  if (data[pos] == igesin->getrecorddelim())
    return 1;
  if (data[pos] == igesin->getparameterdelim())
    {  nextchar();
       return 1;
    }
  *value = strtod(data+pos,&endptr);
  if (*endptr == 'D' || *endptr == 'd')
    {  *endptr = 'E';
       *value = strtod(data+pos,&endptr);
    }
  pos = int(endptr - data);
  if (! discardblanks()) return 0;
  if (data[pos] != igesin->getparameterdelim() &&
      data[pos] != igesin->getrecorddelim())
    return 0;
  if (data[pos] == igesin->getparameterdelim())
    nextchar();
  return 1;
}

int FreeFormatData::readpointer(const long def,long *value)
{char *endptr;
  if (! discardblanks()) return 0;
  *value = def;
  if (data[pos] == igesin->getrecorddelim())
    return 1;
  if (data[pos] == igesin->getparameterdelim())
    {  nextchar();
       return 1;
    }
  *value = strtol(data+pos,&endptr,10);
  pos = int(endptr - data);
  if (! discardblanks()) return 0;
  if (data[pos] != igesin->getparameterdelim() &&
      data[pos] != igesin->getrecorddelim())
    return 0;
  if (data[pos] == igesin->getparameterdelim())
    nextchar();
  return 1;
}

int FreeFormatData::readlogical(const long def,long *value)
{char *endptr;
  if (! discardblanks()) return 0;
  *value = def;
  if (data[pos] == igesin->getrecorddelim())
    return 1;
  if (data[pos] == igesin->getparameterdelim())
    {  nextchar();
       return 1;
    }
  *value = strtol(data+pos,&endptr,10);
  pos = int(endptr - data);
  if (! discardblanks()) return 0;
  if (data[pos] != igesin->getparameterdelim() &&
      data[pos] != igesin->getrecorddelim())
    return 0;
  if (data[pos] == igesin->getparameterdelim())
    nextchar();
  if (*value != 0 && *value != 1) return 0;
  return 1;
}

//
//  IGESDERecord Implementation
//

long IGESDERecord::readlong(char *string)
{char field[9];
 char *endptr;
  strncpy(field,string,8);  field[8] = 0;
  if (strcmp(field,"        ") == 0)
    return 0L;
  else
    return strtol(field,&endptr,10);
}

IGESDERecord::IGESDERecord(IGESRecord *ir1,IGESRecord *ir2)
{ type = readlong(ir1->getline());
  pdata = readlong(ir1->getline()+8);
  structure = readlong(ir1->getline()+16);
  font = readlong(ir1->getline()+24);
  level = readlong(ir1->getline()+32);
  view = readlong(ir1->getline()+40);
  matrix = readlong(ir1->getline()+48);
  labelassoc = readlong(ir1->getline()+56);
  status = readlong(ir1->getline()+64);
  weight = readlong(ir2->getline());
  colour = readlong(ir2->getline()+8);
  npdata = readlong(ir2->getline()+16);
  form = readlong(ir2->getline()+32);
  strncpy(label,ir2->getline()+56,8);  label[8] = 0;
  subscript = readlong(ir2->getline()+64);
}

void igesin_command(int *cstate,HighLevelEvent *,void **)
{IGESIn igesin;
  *cstate = ExitState;
  if (igesin.open(0))
    igesin.read(1);

}

#endif
