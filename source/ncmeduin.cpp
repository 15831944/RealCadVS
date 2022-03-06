
#include "ncwin.h"

#if MENUS != STUDENTMENUS

#define MAX_ENTRIES 1000
#define MAX_CLUMP_DEPTH 1000

class MedusaIn
  {private:
     FILE *infile;
     int ncodes,clumpdepth;
     long filelength,swapped,offset,index,length,type,unknown,id,layer;
     double width,pitch,aspect,shear;
     struct CodeDotDat  //  Contains the information in the code.dat file.
       {char entity[20];
        char type[4];
        double p1,p2,p3,p4,p5,p6,p7,p8,p9,p10;
       } codedotdat[MAX_ENTRIES];
     EntityList clumplist[MAX_CLUMP_DEPTH];
     int clumptype[MAX_CLUMP_DEPTH];
   public:
     MedusaIn(void);
     ~MedusaIn();
     int open(RCCHAR *s);
     int movestart(void);
     int readheader(void);
     void readentity(void);
     void ProcessPoints(int i,int use,int startfunction,int endfunction,double x1,double y1,double x2,double y2,double x3,double y3,double w1,double w2,double w3);
     void InsertLine(int type,double x1,double y1,double x2,double y2);
     int match(char *,int type);
     void read(void);
     void close(void);
  };

MedusaIn::MedusaIn()
{ char buffer[300],*s;
RCCHAR filename[300];
  strcpy(filename,home.getpublicfilename("code.dat"));
  infile = fopen(filename,"r");
  ncodes = 0;
  if (infile != 0)
    {  while (fgets(buffer,300,infile) != 0)
         {  s = strtok(buffer," []");
            if (_stricmp(s,"Line") == 0)
              {  strcpy(codedotdat[ncodes].entity,s);
                 s = strtok(0," []");  if (s!= 0) strcpy(codedotdat[ncodes].type,s);
                 s = strtok(0," []");  codedotdat[ncodes].p1 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p2 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p3 = s != 0 ? atof(s) : 0.0;
                 ncodes++;
              }
            else if (_stricmp(s,"Text") == 0)
              {  strcpy(codedotdat[ncodes].entity,s);
                 s = strtok(0," []");  if (s!= 0) strcpy(codedotdat[ncodes].type,s);
                 s = strtok(0," []");  codedotdat[ncodes].p1 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p2 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p3 = s != 0 ? atof(s) : 0.0;
                 ncodes++;
              }
            else if (_stricmp(s,"PARAMS") == 0)
              {  strcpy(codedotdat[ncodes].entity,s);
                 s = strtok(0," []");  if (s!= 0) strcpy(codedotdat[ncodes].type,s);
                 s = strtok(0," []");  codedotdat[ncodes].p1 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p2 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p3 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p4 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p5 = s != 0 ? atof(s) : 0.0;
                 ncodes++;
                 s = strtok(0," []");
                 if (s != 0)
                   {  strcpy(codedotdat[ncodes].entity,"PARAMS");
                      sprintf(codedotdat[ncodes].type,"%d",atoi(codedotdat[ncodes-1].type)+5);
                      codedotdat[ncodes].p1 = s != 0 ? atof(s) : 0.0;
                      s = strtok(0," []");  codedotdat[ncodes].p2 = s != 0 ? atof(s) : 0.0;
                      s = strtok(0," []");  codedotdat[ncodes].p3 = s != 0 ? atof(s) : 0.0;
                      s = strtok(0," []");  codedotdat[ncodes].p4 = s != 0 ? atof(s) : 0.0;
                      s = strtok(0," []");  codedotdat[ncodes].p5 = s != 0 ? atof(s) : 0.0;
                      ncodes++;
                   }
              }
            else if (_stricmp(s,"CONSTANTS") == 0)
              {  strcpy(codedotdat[ncodes].entity,s);
                 s = strtok(0," []");  if (s!= 0) strcpy(codedotdat[ncodes].type,s);
                 s = strtok(0," []");  codedotdat[ncodes].p1 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p2 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p3 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p4 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p5 = s != 0 ? atof(s) : 0.0;
                 ncodes++;
              }
            else if (_stricmp(s,"INDEX") == 0)
              {  strcpy(codedotdat[ncodes].entity,s);
                 s = strtok(0," []");
                 if (s != 0) 
                   {  if (*s == ' ') s++;  strcpy(codedotdat[ncodes].type,s);
                   }
                 s = strtok(0," []");  codedotdat[ncodes].p1 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p2 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p3 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p4 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p5 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p6 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p7 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p8 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p9 = s != 0 ? atof(s) : 0.0;
                 s = strtok(0," []");  codedotdat[ncodes].p10 = s != 0 ? atof(s) : 0.0;
                 ncodes++;
              }
            else if (_stricmp(s,"PEN") == 0)
              {  strcpy(codedotdat[ncodes].entity,s);
                 s = strtok(0," []");  if (s!= 0) strcpy(codedotdat[ncodes].type,s);
                 strtok(0," []");  s = strtok(0," []");  if (s!= 0) codedotdat[ncodes].p1 = atof(s);
                 strtok(0," []");  s = strtok(0," []");  if (s!= 0) codedotdat[ncodes].p2 = atof(s);
                 strtok(0," []");  s = strtok(0," []");  if (s!= 0) codedotdat[ncodes].p3 = atof(s);
                 ncodes++;
              }
            else if (_stricmp(s,"SHAPE") == 0)
              {  strcpy(codedotdat[ncodes].entity,s);
                 s = strtok(0," []");  if (s!= 0) codedotdat[ncodes].p1 = atof(s);
                 s = strtok(0," []");  if (s!= 0) strcpy(codedotdat[ncodes].type,s); else strcpy(codedotdat[ncodes].type,"");
                 ncodes++;
              }
         }
       fclose(infile);
    }
 infile = 0;
}

static int byteorder[] = {  0,1,2,3,4,5,6,7  };

void DoubleByteSwap(int swapped,double *x)
{double y;
  y = *x;
  if (swapped)
    {  ((char *)x)[0] = ((char *)&y)[0];
       ((char *)x)[1] = ((char *)&y)[1];
       ((char *)x)[2] = ((char *)&y)[2];
       ((char *)x)[3] = ((char *)&y)[3];
       ((char *)x)[4] = ((char *)&y)[4];
       ((char *)x)[5] = ((char *)&y)[5];
       ((char *)x)[6] = ((char *)&y)[6];
       ((char *)x)[7] = ((char *)&y)[7];
    }
  else
    {  ((char *)x)[0] = ((char *)&y)[7];
       ((char *)x)[1] = ((char *)&y)[6];
       ((char *)x)[2] = ((char *)&y)[5];
       ((char *)x)[3] = ((char *)&y)[4];
       ((char *)x)[4] = ((char *)&y)[3];
       ((char *)x)[5] = ((char *)&y)[2];
       ((char *)x)[6] = ((char *)&y)[1];
       ((char *)x)[7] = ((char *)&y)[0];
    }
}

void ShortByteSwap(int swapped,short *x)
{short y;
  if (swapped)
    {  y = *x;
       ((char *)x)[0] = ((char *)&y)[1];
       ((char *)x)[1] = ((char *)&y)[0];
    }
}

void UpdateBuffer(int swapped,  char *buffer,double *x,int *i)
{int j,mask;
  mask = buffer[(*i)++];
  for (j = 0 ; j < 8 ; j++)
    if ((mask & (0x80 >> j)) != 0)
      ((char *)x)[swapped ? 7-byteorder[j] : byteorder[j]] = buffer[(*i)++];
}

#define TypeCharacter(x) (char)((x) <= 9 ? '0' + (x) : (x) <= 35 ? 'A' + (x) - 10 : (x) == 36 ? '#' : (x) == 37 ? '$' : (x) == 38 ? '%' : 0)

void ConvertType(int type,char *buffer)
{ if ((type & 0x8000) == 0x8000)
    {  type = (short)type;
       buffer[0] = TypeCharacter(38+((type/40/40)%40));
       buffer[1] = TypeCharacter(-(type/40)%40);
       buffer[2] = TypeCharacter(-type%40);
       buffer[3] = 0;
    }
  else
    {  buffer[0] = TypeCharacter(type/40/40);
       buffer[1] = TypeCharacter((type/40) % 40);
       buffer[2] = TypeCharacter(type % 40);
       buffer[3] = 0;
    }
}

int MedusaIn::movestart(void)
{char magic[9];
   unsigned char buffer[32];

  fseek(infile,0L,0);
  fread(magic,1,9,infile);
  if (strncmp(magic,"(W\027Msheet",9) != 0)  //  Check the magic bytes at the start
    return 0;

  fseek(infile,0x20L,0);
  fread(buffer,32,1,infile);
  swapped = buffer[17] != 0x04 && buffer[17] != 0x03;


//if (MessageBox(cadwindow->gethwnd(),"Swap","?",MB_YESNO) == IDYES)
//  swapped = ! swapped;


  fseek(infile,0,2);
  filelength = ftell(infile);

  offset = 0x129;
  index = 0;

  clumpdepth = -1;

  return 1;

}

int MedusaIn::readheader(void)
{ unsigned  char buffer[9];

  if (offset >= filelength)
    return 0;

  index++;
  fseek(infile,offset,0);
  fread(buffer,9,1,infile);

  ShortByteSwap(swapped,(short *)(buffer + 1));
  ShortByteSwap(swapped,(short *)(buffer + 3));
  ShortByteSwap(swapped,(short *)(buffer + 7));

  length = buffer[1] * 256 + buffer[2];
  type = buffer[3] * 256 + buffer[4];
  unknown = buffer[5] * 256 + buffer[6];
  id = buffer[7] * 256 + buffer[8];

  offset += length*2 + 9;

  return length != 0xFFFF;
}

int MedusaIn::match(char *entity,int type)
{char types[4];
 int i;
  if (entity != 0 && strcmp(entity,"SHAPE") == 0)
    {  for (i = 0 ; i < ncodes ; i++)
         if (_stricmp("SHAPE",codedotdat[i].entity) == 0 && codedotdat[i].p1 == type)
           break;
    }
  else
    {  if (entity != 0 && (_stricmp(entity,"PEN") == 0 || _stricmp(entity,"PARAMS") == 0))
         {  if (type < 10)
              type = type * 40 * 40 + 39 * 40 + 39;
            else
              type = (type / 10) * 40 * 40 + 40 * (type % 10) + 39;
         }
       ConvertType(type,types);
       for (i = 0 ; i < ncodes ; i++)
         if ((entity == 0 || _stricmp(entity,codedotdat[i].entity) == 0) && _stricmp(types,codedotdat[i].type) == 0)
           break;
    }
  return i < ncodes ? i : -1;
}

void MedusaIn::InsertLine(int type,double x1,double y1,double x2,double y2)
{int lindex,pindex,index;
  Line *line = new Line(Point(x1,y1,0.0),Point(x2,y2,0.0));
  char *at = new char[4];

  if ((lindex = match(0,type)) >= 0)
    {  if ((index = match((char*)"PEN",(int)codedotdat[lindex].p2)) >= 0)
         line->EntityHeader::change((int)codedotdat[index].p1,layer,(int)codedotdat[index].p3-1,(int)(codedotdat[index].p2-1.0)/2);
       else
         line->EntityHeader::change(line->getcolour(),layer,line->getstyle(),line->getweight());
       if ((pindex = match((char*)"SHAPE",(int)codedotdat[lindex].p1)) >= 0)
         {//  Pattern line style
          RCCHAR filename[300];
            strcpy(filename,codedotdat[pindex].type);
            strcat(filename,".cad");
            strcpy(filename,home.getpublicfilename(filename));
            if (width <= 0.0) width = 1.0;
            if (aspect <= 0.0) aspect = 1.0;
            PatternLineStyle *pls = new PatternLineStyle(width,aspect,pitch,shear,0.0,0.0,0,0,0,filename);
            ConvertType(type,at);
            if (strcmp(at,"FTL") == 0)
              line->EntityHeader::change(line->getcolour(),layer+400,line->getstyle(),line->getweight());
            line->setpatternlinestyle(pls);
         }

    }
  else
    line->EntityHeader::change(line->getcolour(),layer,line->getstyle(),line->getweight());
  ConvertType(type,at);
  line->addat(_RCT(at));
  db.geometry.add(line);
  if (clumpdepth >= 0)
    clumplist[clumpdepth].add(line);
}

void MedusaIn::ProcessPoints(int i,int use,int startfunction,int endfunction,double x1,double y1,double x2,double y2,double x3,double y3,double w1,double w2,double w3)
{int findex,index,lindex,pindex;
 double xd,yd,dx,dy,w,h,l;

  if ((use & 0x0F) == 2 || (use & 0x0F) == 1 && i == 0)  //  LIN
    {  xd = x3 - x2;
       yd = y3 - y2;
       l = sqrt(xd * xd + yd * yd);
       if (l > 1.0E-10)
         {  dx = xd / l;
            dy = yd / l;
            if (startfunction > 0)
              {  if ((startfunction-1) % 100 <= 10)
                   findex = match((char*)"INDEX",1 * 40 * 40 + 39 * 40 + 39);
                 else
                   findex = match((char*)"INDEX",(((startfunction-1)/10)%10) * 40 * 40 + 1 * 40 + 39);
                 if (findex >= 0)
                   findex = (int)((&codedotdat[findex].p1)[(startfunction-1)%10]);
              }
            else
              findex = 0;




            if ((index = match((char*)"CONSTANTS",(((startfunction+10-1)/10)%10) * 40 * 40 + ((((startfunction-1)/5)%2)*5+1) * 40 + 39)) >= 0)
              w = fabs((&codedotdat[index].p1)[(startfunction-1) % 5]);
            else
              w = 1.0;

            //  Other FUNVs 4, 6, 7, 10, 26, 47 (Trim)

            if ((index = match((char*)"CONSTANTS",1 * 40 * 40 + 39)) >= 0)
              h = w * codedotdat[index].p5;
            else
              h = 0.5;

            if (findex == 207)
              {//  Map 207 into findex = 1
                 findex = 1;
                 w = 6.0;
              }

            if (findex == 1)
              {  x2 += xd * w / l;
                 y2 += yd * w / l;
              }

            if (i > 0 && (findex == 3 || findex == 24))
              {  InsertLine(type,x2,y2,x2 + dx * w - dy * h,y2 + dy * w + dx * h);
                 InsertLine(type,x2 + dx * w - dy * h,y2 + dy * w + dx * h,x2 + dx * w + dy * h,y2 + dy * w - dx * h);
                 InsertLine(type,x2,y2,x2 + dx * w + dy * h,y2 + dy * w - dx * h);
              }


            if (endfunction > 0)
              {  if ((endfunction-1) % 100 <= 10)
                   findex = match((char*)"INDEX",1 * 40 * 40 + 39 * 40 + 39);
                 else
                   findex = match((char*)"INDEX",(((endfunction-1)/10)%10) * 40 * 40 + 1 * 40 + 39);
                 if (findex >= 0)
                   findex = (int)((&codedotdat[findex].p1)[(endfunction-1)%10]);
              }
            else
              findex = 0;

            if ((index = match((char*)"CONSTANTS",(((endfunction+10-1)/10)%10) * 40 * 40 + ((((endfunction-1)/5)%2)*5+1) * 40 + 39)) >= 0)
              w = fabs((&codedotdat[index].p1)[(endfunction-1) % 5]);
            else
              w = 1.0;

            if ((index = match((char*)"CONSTANTS",1 * 40 * 40 + 39)) >= 0)
              h = w * codedotdat[index].p5;
            else
              h = 0.5;

            if (findex == 207)
              {//  Map 207 into findex = 1
                 findex = 1;
                 w = 6.0;
              }

            if (findex == 1)
              {  x3 -= xd * w / l;
                 y3 -= yd * w / l;
              }

            if (i > 0 && (findex == 2 || findex == 23))
              {  InsertLine(type,x3,y3,x3 - dx * w - dy * h,y3 - dy * w + dx * h);
                 InsertLine(type,x3 - dx * w - dy * h,y3 - dy * w + dx * h,x3 - dx * w + dy * h,y3 - dy * w - dx * h);
                 InsertLine(type,x3,y3,x3 - dx * w + dy * h,y3 - dy * w - dx * h);
              }
            else if (findex == 15)
              {Circle *circle = new Circle(Point(x3,y3,0.0),w/4.0);
               char *at = new char[4];
                 circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
                 if ((lindex = match(0,type)) >= 0 &&  (index = match((char*)"PEN",(int)codedotdat[lindex].p2)) >= 0)
                   circle->EntityHeader::change((int)codedotdat[index].p1,layer,circle->getstyle(),circle->getweight());
                 ConvertType(type,at);
                 circle->addat(_RCT(at));
                 db.geometry.add(circle);
                 if (clumpdepth >= 0)
                   clumplist[clumpdepth].add(circle);
              }
            else if (findex == 16)
              {Circle *circle = new Circle(Point(x3,y3,0.0),w/4.0);
               char *at = new char[4];
                 circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
                 if ((lindex = match(0,type)) >= 0 &&  (index = match((char*)"PEN",(int)codedotdat[lindex].p2)) >= 0)
                   circle->EntityHeader::change((int)codedotdat[index].p1,layer,circle->getstyle(),circle->getweight());
                 ConvertType(type,at);
                 circle->addat(_RCT(at));
                 db.geometry.add(circle);
                 if (clumpdepth >= 0)
                   clumplist[clumpdepth].add(circle);
              }


            if (i > 0)
              InsertLine(type,x2,y2,x3,y3);


         }
    }
  else if ((use & 0x0F) == 4 && i > 0)  //  CIR
    {Circle *circle = new Circle(Point(x2,y2,0.0),sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
       char *at = new char[4];
       if ((lindex = match(0,type)) >= 0)
         {  if ((index = match((char*)"PEN",(int)codedotdat[lindex].p2)) >= 0)
              circle->EntityHeader::change((int)codedotdat[index].p1,layer,(int)codedotdat[index].p3-1,(int)(codedotdat[index].p2-1.0)/2);
            else
              circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
            if ((pindex = match((char*)"SHAPE",(int)codedotdat[lindex].p1)) >= 0)
              {//  Pattern line style
               RCCHAR filename[300];
                 strcpy(filename,codedotdat[pindex].type);
                 strcat(filename,".cad");
                 strcpy(filename,home.getpublicfilename(filename));
                 PatternLineStyle *pls = new PatternLineStyle(width,aspect,pitch,shear,0.0,0.0,0,0,0,filename);
                 circle->setpatternlinestyle(pls);
              }
  
         }
       else
         circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
       ConvertType(type,at);
       circle->addat(_RCT(at));
       db.geometry.add(circle);
       if (clumpdepth >= 0)
         clumplist[clumpdepth].add(circle);
    }
  else if ((use & 0x0F) == 8 && i > 0)  //  CEN
    {Circle *circle = new Circle(Point(x3,y3,0.0),sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)));
       char *at = new char[4];
       if ((lindex = match(0,type)) >= 0)
         {  if ((index = match((char*)"PEN",(int)codedotdat[lindex].p2)) >= 0)
              circle->EntityHeader::change((int)codedotdat[index].p1,layer,(int)codedotdat[index].p3-1,(int)(codedotdat[index].p2-1.0)/2);
            else
              circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
            if ((pindex = match((char*)"SHAPE",(int)codedotdat[lindex].p1)) >= 0)
              {//  Pattern line style
               RCCHAR filename[300];
                 strcpy(filename,codedotdat[pindex].type);
                 strcat(filename,".cad");
                 strcpy(filename,home.getpublicfilename(filename));
                 PatternLineStyle *pls = new PatternLineStyle(width,aspect,pitch,shear,0.0,0.0,0,0,0,filename);
                 circle->setpatternlinestyle(pls);
              }
  
         }
       else
         circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
       ConvertType(type,at);
       circle->addat(_RCT(at));
       db.geometry.add(circle);
       if (clumpdepth >= 0)
         clumplist[clumpdepth].add(circle);
    }
  else if ((((use & 0x0F) == 6 || (use & 0x0F) == 7)) && i > 1)  //  CLO or ANT
    {Circle c(Point(x2,y2,0.0),Point(x3,y3,0.0));
     double a1,a2;
       a1 = c.angle(Point(x1,y1,0.0));
       a2 = c.angle(Point(x3,y3,0.0));
       a2 -= a1;
       if (a2 > 0.0 && (use & 0x0F) == 7)
         a2 = a2 - 2.0 * M_PI;
       else if (a2 < 0.0 && (use & 0x0F) == 6)
         a2 = a2 + 2.0 * M_PI;
       if (a2 < 0.0)
         {  a1 += a2;
            while (a1 < 0.0) a1 += 2.0 * M_PI;
              a2 = -a2;
         }

       Circle *circle = new Circle(Point(x2,y2,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),sqrt((x3-x2)*(x3-x2)+(y3-y2)*(y3-y2)),a1,a2);
       char *at = new char[4];
       if ((lindex = match(0,type)) >= 0)
         {  if ((index = match((char*)"PEN",(int)codedotdat[lindex].p2)) >= 0)
              circle->EntityHeader::change((int)codedotdat[index].p1,layer,(int)codedotdat[index].p3-1,(int)(codedotdat[index].p2-1.0)/2);
            else
              circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
            if ((pindex = match((char*)"SHAPE",(int)codedotdat[lindex].p1)) >= 0)
              {//  Pattern line style
               RCCHAR filename[300];
                 strcpy(filename,codedotdat[pindex].type);
                 strcat(filename,".cad");
                 strcpy(filename,home.getpublicfilename(filename));
                 PatternLineStyle *pls = new PatternLineStyle(width,aspect,pitch,shear,0.0,0.0,0,0,0,filename);
                 circle->setpatternlinestyle(pls);
              }
  
         }
       else
         circle->EntityHeader::change(circle->getcolour(),layer,circle->getstyle(),circle->getweight());
       ConvertType(type,at);
       circle->addat(_RCT(at));
       db.geometry.add(circle);
       if (clumpdepth >= 0)
         clumplist[clumpdepth].add(circle);
    }
  else if ((use & 0x0F) == 3 && i > 1)
    {Point p[3];
     double w[3];
     BitMask o(32);
       w[0] = w1;  w[1] = w2;  w[2] = w3;
       p[0] = Point(x1,y1,0.0);  p[1] = Point(x2,y2,0.0);  p[2] = Point(x3,y3,0.0);

       Curve *curve = new Curve(3,2,p,0,w,o);
       char *at = new char[4];
       if ((lindex = match(0,type)) >= 0)
         {  if ((index = match((char*)"PEN",(int)codedotdat[lindex].p2)) >= 0)
              curve->EntityHeader::change((int)codedotdat[index].p1,layer,(int)codedotdat[index].p3-1,(int)(codedotdat[index].p2-1.0)/2);
            else
              curve->EntityHeader::change(curve->getcolour(),layer,curve->getstyle(),curve->getweight());
            if ((pindex = match((char*)"SHAPE",(int)codedotdat[lindex].p1)) >= 0)
              {//  Pattern line style
               RCCHAR filename[300];
                 strcpy(filename,codedotdat[pindex].type);
                 strcat(filename,".cad");
                 strcpy(filename,home.getpublicfilename(filename));
                 PatternLineStyle *pls = new PatternLineStyle(width,aspect,pitch,shear,0.0,0.0,0,0,0,filename);
                 curve->setpatternlinestyle(pls);
              }
  
         }
       else
         curve->EntityHeader::change(curve->getcolour(),layer,curve->getstyle(),curve->getweight());
       ConvertType(type,at);
       curve->addat(_RCT(at));
       db.geometry.add(curve);
       if (clumpdepth >= 0)
         clumplist[clumpdepth].add(curve);
    }
}

void MedusaIn::readentity(void)
{  char *buffer;
 int i,j,n,closed,use,function,lastfunction,firstuse,firstfunction;
 double x1,y1,w1,x2,y2,w2,x3,y3,w3,x4,y4,w4,x3last,y3last,angle,circleradius;
 BitMask o(32);

  layer = 0;

  if (length == 0)
    {  if (type != 0)
         clumpdepth++;
       else
         {  if (clumpdepth >= 0)
              {int index;
                 char *at = new char[4];
                 ConvertType(type,at);
                 Group *group = new Group(clumplist[clumpdepth],"Medusa Clump",at,BitMask(32));
                 if ((index = match(0,type)) >= 0 && (index = match((char*)"PEN",(int)codedotdat[index].p2)) >= 0)
                   group->EntityHeader::change((int)codedotdat[index].p1,layer,(int)codedotdat[index].p3-1,(int)(codedotdat[index].p2-1.0)/2);
                 group->addat(_RCT(at));
                 db.geometry.add(group);
                 clumplist[clumpdepth] = EntityList();
                 if (clumpdepth > 0)
                   clumplist[clumpdepth-1].add(group);
              }
            clumpdepth--;
         }
       return;
    }
  buffer = new   char[length*2];

  fread(buffer,length*2,1,infile);
  layer = 0;

  if (buffer[0] == 0x05 || buffer[0] == 0x07)
    {//  Clump definition
       if (clumpdepth < MAX_CLUMP_DEPTH-1)
         clumpdepth++;
    }
  else if (buffer[0] == 0x0b || buffer[0] == 0x0f)
    {  if (buffer[0] == 0x0f)
         {  DoubleByteSwap(swapped,(double *)(buffer+1));
            DoubleByteSwap(swapped,(double *)(buffer+9));
            DoubleByteSwap(swapped,(double *)(buffer+17));
            DoubleByteSwap(swapped,(double *)(buffer+25));
            aspect = *((double *)(buffer+1));
            pitch = *((double *)(buffer+9));
            shear = *((double *)(buffer+17));
            width = *((double *)(buffer+25));


            ShortByteSwap(swapped,(short *)(buffer + 39));   //  Number of coordinates
            ShortByteSwap(swapped,(short *)(buffer + 35));   //  Layer
            j = 41;
            n = buffer[39]*256 + buffer[40];
            layer = (buffer[35] * 256 + buffer[36]) & 0x3FF;
         }
       else
         {  ShortByteSwap(swapped,(short *)(buffer + 1));    //  Layer
            ShortByteSwap(swapped,(short *)(buffer + 5));    //  Number of coordinates
            layer = (buffer[1] * 256 + buffer[2]) & 0x3FF;
            j = 7;
            n = buffer[5]*256 + buffer[6];
         }
       x1 = 0.0;  y1 = 0.0;  w1 = 1.0;
       x2 = 0.0;  y2 = 0.0;  w2 = 1.0;
       x3 = 0.0;  y3 = 0.0;  w3 = 1.0;
       x3last = 0.0;  y3last = 0.0;
       closed = 0;
       lastfunction = 0;
       for (i = 0 ; i < n ; i++)
         {  w3 = 1.0;
            use = buffer[j++];
            if (i == 0)
              closed = (use & 0x0F) == 2;
            if ((use & 0x80) == 0x80)
              UpdateBuffer(swapped,buffer,&x3last,&j);
            if ((use & 0x40) == 0x40)
              UpdateBuffer(swapped,buffer,&y3last,&j);
            x3 = x3last;  y3 = y3last;
            if ((use & 0x20) == 0x20)
              {  DoubleByteSwap(! swapped,(double *)(buffer+j));
                 w3 = *((double *)(buffer+j));
                 j += 8;
              }

            if ((use & 0x10) == 0x10)
              function = buffer[j++];
            else
              function = 0;
            if (fabs(w3) > 1.0E-10)
              {  x3 /= w3;  y3 /= w3;
              }
            if (i == 0)
              {  x4 = x3;  y4 = y3;  w4 = w3;
                 firstfunction = function;
                 firstuse = use;
              }

            ProcessPoints(i,use,lastfunction,function,x1,y1,x2,y2,x3,y3,w1,w2,w3);

            x1 = x2;  y1 = y2;  w1 = w2;
            x2 = x3;  y2 = y3;  w2 = w3;
            lastfunction = function;
         }
       if (closed)
         ProcessPoints(i,firstuse,lastfunction,firstfunction,x1,y1,x2,y2,x4,y4,w1,w2,w4);
    }
  else if (buffer[0] == 0x0a || buffer[0] == 0x0c)
    {char *t;
     double x,y,height;
     int justification,index,extrabytes;
       extrabytes = buffer[0] == 0x0c ? 26 : 0;

       ShortByteSwap(swapped,(short *)(buffer + 39 + extrabytes));   //  String Length
       ShortByteSwap(swapped,(short *)(buffer + 34 + extrabytes));   //  Justification
       ShortByteSwap(swapped,(short *)(buffer + 35 + extrabytes));   //  Layer
       layer = (buffer[35 + extrabytes] * 256 + buffer[36 + extrabytes]) & 0x3FF;
       DoubleByteSwap(swapped,(double *)(buffer+1));
       DoubleByteSwap(swapped,(double *)(buffer+9));
       DoubleByteSwap(swapped,(double *)(buffer+17));
       DoubleByteSwap(swapped,(double *)(buffer+25));
       if (extrabytes > 0)
         {  DoubleByteSwap(swapped,(double *)(buffer+33));
            DoubleByteSwap(swapped,(double *)(buffer+41));
            DoubleByteSwap(swapped,(double *)(buffer+49));
         }
       x = *((double *)(buffer+1));
       y = *((double *)(buffer+9));
       angle = *((double *)(buffer+17));
       height = *((double *)(buffer+25));
       buffer[43 + extrabytes + buffer[39 + extrabytes]*256 + buffer[40 + extrabytes]] = 0;
       t = (char *)buffer + 43 + extrabytes;
       justification = buffer[34 + extrabytes];
       circleradius = 0.0;
       if (justification / 10 <= 2)
         ;//  Nothing
       else if (justification / 10 == 3)
         o.set(1);
       else
         o.set(2);
       if (justification % 10 <= 2)
         ;//  Nothing
       else if (justification % 10 == 3)
         o.set(3);
       else
         o.set(4);
       char *at = new char[4];
       ConvertType(type,at);
       if ((index = match(0,type)) >= 0)
         {  if (codedotdat[index].p1 >= 1.5 && codedotdat[index].p1 <= 2.5) //  Underlined
              o.set(6);
            else if (codedotdat[index].p1 >= 2.5 && codedotdat[index].p1 <= 4.5) //  Boxed, sausaged
              o.set(8);
            else if (codedotdat[index].p1 >= 4.5 && codedotdat[index].p1 <= 5.5) //  Circled
              circleradius = height;
            if (extrabytes > 0)
              {  height *= 4.0;  //  Default DPARS[1]
                 circleradius *= 4.0;  //  Make the text size a little bit smaller....
                 if (justification / 10 == 5)
                   x -= 0.3333 * height;
                 else if (justification / 10 == 1)
                   x += 0.3333 * height;
                 if (justification % 10 == 5)
                   y -= 0.3333 * height;
                 else if (justification % 10 == 1)
                   y += 0.3333 * height;
              }
            else if ((index = match((char*)"PARAMS",(int)codedotdat[index].p3)) >= 0)
              {  height *= codedotdat[index].p1;  //  Make the text size a little bit smaller....
                 circleradius *= codedotdat[index].p1;  //  Make the text size a little bit smaller....
                 if (justification / 10 == 5)
                   x -= codedotdat[index].p4 * height;
                 else if (justification / 10 == 1)
                   x += codedotdat[index].p4 * height;
                 if (justification % 10 == 5)
                   y -= codedotdat[index].p5 * height;
                 else if (justification % 10 == 1)
                   y += codedotdat[index].p5 * height;
              }
         }
       Text *text = new Text(CHAR2RCCHR("Arial"),Point(x,y,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,height,0.0,angle,0,CHAR2RCCHR(t),0,o);
       if ((index = match(0,type)) >= 0 && (index = match((char*)"PEN",(int)codedotdat[index].p2)) >= 0)
         text->EntityHeader::change((int)codedotdat[index].p1,layer,text->getstyle(),text->getweight());
       else
         text->EntityHeader::change(text->getcolour(),layer,text->getstyle(),text->getweight());
       text->addat(_RCT(at));
       db.geometry.add(text);
       if (clumpdepth >= 0)
         clumplist[clumpdepth].add(text);

       if (circleradius > 0.0)
         {  Circle *circle = new Circle(Point(x,y,0.0),circleradius);
            if ((index = match(0,type)) >= 0 && (index = match((char*)"PEN",(int)codedotdat[index].p2)) >= 0)
              circle->EntityHeader::change((int)codedotdat[index].p1,layer,0,circle->getweight());
            else
              circle->EntityHeader::change(circle->getcolour(),layer,0,circle->getweight());
            db.geometry.add(circle);
            if (clumpdepth >= 0)
              clumplist[clumpdepth].add(circle);
         }

    }
  else if (buffer[0] == 0x09 || buffer[0] == 0x0d)
    {double x,y,angle,scale;
     char at[4],picture[4];

       DoubleByteSwap(swapped,(double *)(buffer+1));
       DoubleByteSwap(swapped,(double *)(buffer+9));
       DoubleByteSwap(swapped,(double *)(buffer+17));
       DoubleByteSwap(swapped,(double *)(buffer+25));
       if (buffer[0] == 0x09)
         {  ShortByteSwap(swapped,(short *)(buffer+33));   //  Layer
            ConvertType(type,picture);
            layer = (buffer[33] * 256 + buffer[34]) & 0x3FF;
         }
       else
         {  ShortByteSwap(swapped,(short *)(buffer+33));   //  Picture
            ShortByteSwap(swapped,(short *)(buffer+35));   //  Layer
            ConvertType(buffer[33] * 256 + buffer[34],picture);
            layer = (buffer[35] * 256 + buffer[36]) & 0x3FF;
         }

       x = *((double *)(buffer+1));
       y = *((double *)(buffer+9));
       angle = *((double *)(buffer+17));
       scale = *((double *)(buffer+25));

       ConvertType(type,at);

       if (strcmp(at,"DTM") == 0)
         {  char *at = new char[4];
            ConvertType(type,at);
            PointE *point = new PointE(Point(x,y,0.0));
            if ((index = match(0,type)) >= 0 && (index = match((char*)"PEN",(int)codedotdat[index].p2)) >= 0)
              point->EntityHeader::change((int)codedotdat[index].p1,layer,point->getstyle(),point->getweight());
            else
              point->EntityHeader::change(point->getcolour(),layer,point->getstyle(),point->getweight());
            point->addat(_RCT(at));
            db.geometry.add(point);
            if (clumpdepth >= 0)
              clumplist[clumpdepth].add(point);
         }
       else
         {RCCHAR library[300];
          BitMask options(32);
            char *at = new char[4];
            ConvertType(type,at);
            strcpy(library,home.getpublicfilename(picture));
            strcat(library,".CAD");
            Figure *figure = new Figure(library,_RCT(""),Point(x,y,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),Point(scale,scale,scale),Point(0.0,0.0,angle),Point(0.0,0.0,0.0),0,0,0,NULL,options);
            if ((index = match(0,type)) >= 0 && (index = match((char*)"PEN",(int)codedotdat[index].p2)) >= 0)
              figure->EntityHeader::change((int)codedotdat[index].p1,layer,figure->getstyle(),figure->getweight());
            else
              figure->EntityHeader::change(figure->getcolour(),layer,figure->getstyle(),figure->getweight());
            figure->addat(_RCT(at));
            db.geometry.add(figure);
            if (clumpdepth >= 0)
              clumplist[clumpdepth].add(figure);
         }
    }
  delete [] buffer;
}

int MedusaIn::open(RCCHAR *s)
{OPENFILENAME ofn;
 RCCHAR szDirName[256],szFilter[256],name[300],title[300];
 UINT  i;
 ResourceString rs0(NCHPGL);
 ResourceString rs1(NCHPGL+1);
 ResourceString rs2(NCHPGL+2);
  if (s == 0)
    {  _getcwd(szDirName, sizeof(szDirName));
       _tcscpy(name,TEXT(""));
       _tcscpy(title,TEXT(""));
       _tcscpy(szFilter,TEXT("All files *.*|*.*|"));
       for (i = 0; szFilter[i] != '\0'; i++)
         if (szFilter[i] == '|') szFilter[i] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = szFilter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile = name;
       ofn.nMaxFile = sizeof(name);
       ofn.lpstrFileTitle = title;
       ofn.nMaxFileTitle = sizeof(title);
       ofn.lpstrInitialDir = szDirName;
       ofn.lpstrTitle = _RCT("Import Medusa File");
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
       if (GetOpenFileName(&ofn))
         infile = fopen(ofn.lpstrFile,TEXT("rb"));
       else
         infile = 0;
    }
  else
    infile = fopen(s,"rb");
  return infile != 0;
}

void MedusaIn::close(void)
{ if (infile != 0)
    fclose(infile);
  infile = 0;
}

MedusaIn::~MedusaIn()
{  close();
}

void MedusaIn::read(void)
{ if (movestart() == 0)
    cadwindow->MessageBox("This is not a valid Medusa Sheet file","Import Medusa",MB_ICONINFORMATION);
  else
    while (readheader())
      readentity();
}

void medusain_command(int *cstate,HighLevelEvent *,void **)
{MedusaIn medusain;
 int i;
  *cstate = ExitState;

  for (i = 500 ; i < 1000 ; i++)
    db.dismask.layer.clear(i);

  if (medusain.open(0))
    medusain.read();
}

#endif
