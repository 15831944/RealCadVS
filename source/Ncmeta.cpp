
#include "ncwin.h"

#if ! defined(_WIN32_WCE)
static double height,scale;

#define MAX_OBJECTS 100

static int curpen,curbrush;
static Point lastpoint;
static struct objects
         {int type,style,width;
          RCCOLORREF colour;
         } objects[MAX_OBJECTS];

void mt_loadheader(EntityHeader *hd)
{int c,l,s,w;
 double d,dmin,r,g,b;
 int i;
  l = hd->getlayer();
  if (curpen >= 0 && objects[curpen].type == 0)
    {  r = GetRValue(objects[curpen].colour) / 255.0;
       g = GetGValue(objects[curpen].colour) / 255.0;
       b = GetBValue(objects[curpen].colour) / 255.0;
       s = objects[curpen].style;
       w = objects[curpen].width;
    }
  else
    {  r = g = b = 0.0;
       s = w = 0;
    }
  for (i = 0,dmin = -1.0 ; i < 256 ; i++)
    {  d = sqrt((r - db.colourtable[i].red) * (r - db.colourtable[i].red) +
                (g - db.colourtable[i].green) * (g - db.colourtable[i].green) +
                (b - db.colourtable[i].blue) * (b - db.colourtable[i].blue));
       if (i == 0 || d < dmin)
         {  dmin = d;  c = i;
         }
    }
  hd->change(c,l,s,w);
}

//#if defined(WIN32)
//int CALLBACK EnumMetaFileProc(HDC,HANDLETABLE *,METARECORD *,int,LPARAM)
//{
//#else
/*
int CALLBACK EnumMetaFileProc(HDC,HANDLETABLE *,METARECORD *lpmr,int,LPARAM)
{int i;
 Point p1,p2,p3,p4,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0);
 EntityHeader hd;
 Line *line;
 EllipseE *ellipse;
 double a1,a2;

   switch (lpmr->rdFunction)
     {case META_SETBKCOLOR :  break;
      case META_SETBKMODE :  break;
      case META_SETMAPMODE :  break;
      case META_SETROP2 :  break;
      case META_SETRELABS :  break;
      case META_SETPOLYFILLMODE :  break;
      case META_SETSTRETCHBLTMODE :  break;
      case META_SETTEXTCHAREXTRA :  break;
      case META_SETTEXTCOLOR :  break;
      case META_SETTEXTJUSTIFICATION :  break;
      case META_SETWINDOWORG :
        break;
      case META_SETWINDOWEXT :
        break;
      case META_SETVIEWPORTORG :  break;
      case META_SETVIEWPORTEXT :  break;
      case META_OFFSETWINDOWORG :  break;
      case META_SCALEWINDOWEXT :  break;
      case META_OFFSETVIEWPORTORG :  break;
      case META_SCALEVIEWPORTEXT :  break;
      case META_LINETO :
        mt_loadheader(&hd);
        p1.setxyz(lpmr->rdParm[1]*scale,height - lpmr->rdParm[2]*scale,0.0);
        line = new Line(hd,lastpoint,p1);
        if (line != 0 && line->getdefined())
          db.geometry.add(line);
        lastpoint = p1;
        break;
      case META_MOVETO :
        lastpoint.setxyz(lpmr->rdParm[0]*scale,height - lpmr->rdParm[1]*scale,0.0);
        break;
      case META_EXCLUDECLIPRECT :  break;
      case META_INTERSECTCLIPRECT :  break;
      case META_ARC :
        mt_loadheader(&hd);
        p1.setxyz(lpmr->rdParm[7]*scale,height - lpmr->rdParm[6]*scale,0.0);
        p2.setxyz(lpmr->rdParm[5]*scale,height - lpmr->rdParm[4]*scale,0.0);
        p3.setxyz(lpmr->rdParm[3]*scale,height - lpmr->rdParm[2]*scale,0.0);
        p4.setxyz(lpmr->rdParm[1]*scale,height - lpmr->rdParm[0]*scale,0.0);
        ellipse = new EllipseE(hd,(p1+p2)/2.0,xaxis,yaxis,(p2.x - p1.x)/2.0,(p1.y - p2.y)/2.0,0.0,2.0*M_PI);
        if (ellipse != 0 && ellipse->getdefined())
          {  a1 = ellipse->toparametric(ellipse->angle(p3));
             a2 = ellipse->toparametric(ellipse->angle(p4)) - a1;
             while (a2 < 0.0)
               a2 += 2.0 * M_PI;
             while (a2 > 2.0 * M_PI)
               a2 -= 2.0 * M_PI;
             delete ellipse;
             ellipse = new EllipseE(hd,(p1+p2)/2.0,xaxis,yaxis,(p2.x - p1.x)/2.0,(p1.y - p2.y)/2.0,a1,a2);
             if (ellipse != 0 && ellipse->getdefined())
               db.geometry.add(ellipse);
          }
        break;
      case META_ELLIPSE :
        mt_loadheader(&hd);
        p1.setxyz(lpmr->rdParm[3]*scale,height - lpmr->rdParm[2]*scale,0.0);
        p2.setxyz(lpmr->rdParm[1]*scale,height - lpmr->rdParm[0]*scale,0.0);
        ellipse = new EllipseE(hd,(p1+p2)/2.0,xaxis,yaxis,(p2.x - p1.x)/2.0,(p1.y - p2.y)/2.0,0.0,2.0*M_PI);
        if (ellipse != 0 && ellipse->getdefined())
          db.geometry.add(ellipse);
        break;
      case META_FLOODFILL :  break;
      case META_PIE :  break;
      case META_RECTANGLE :  break;
      case META_ROUNDRECT :  break;
      case META_PATBLT :  break;
      case META_SAVEDC :   break;
      case META_SETPIXEL :  break;
      case META_OFFSETCLIPRGN :   break;
      case META_TEXTOUT :   break;
      case META_BITBLT :  break;
      case META_STRETCHBLT :   break;
      case META_POLYGON :
        break;
      case META_POLYLINE :
        mt_loadheader(&hd);
        p1.setxyz(lpmr->rdParm[1]*scale,height - lpmr->rdParm[2]*scale,0.0);
        for (i = 1 ; i < lpmr->rdParm[0] ; i++)
          {  p2.setxyz(lpmr->rdParm[i*2+1]*scale,height - lpmr->rdParm[i*2+2]*scale,0.0);
             Line *line = new Line(hd,p1,p2);
             if (line != 0 && line->getdefined())
               db.geometry.add(line);
             p1 = p2;
          }
        break;
      case META_ESCAPE : break;
      case META_RESTOREDC :  break;
      case META_FILLREGION :  break;
      case META_FRAMEREGION :  break;
      case META_INVERTREGION :  break;
      case META_PAINTREGION :  break;
      case META_SELECTCLIPREGION :  break;
      case META_SELECTOBJECT :
        if (lpmr->rdParm[0] < MAX_OBJECTS)
          {  if (objects[lpmr->rdParm[0]].type == 0)
               curpen = lpmr->rdParm[0];
             else
               curbrush = lpmr->rdParm[0];
          }
        break;
      case META_SETTEXTALIGN :  break;
      case META_DRAWTEXT :  break;

      case META_CHORD :  break;
      case META_SETMAPPERFLAGS :  break;
      case META_EXTTEXTOUT :  break;
      case META_SETDIBTODEV :  break;
      case META_SELECTPALETTE :  break;
      case META_REALIZEPALETTE :  break;
      case META_ANIMATEPALETTE :  break;
      case META_SETPALENTRIES :  break;
      case META_POLYPOLYGON :  break;
      case META_RESIZEPALETTE :  break;

      case META_DIBBITBLT :  break;
      case META_DIBSTRETCHBLT :  break;
      case META_DIBCREATEPATTERNBRUSH :  break;
      case META_STRETCHDIB :  break;

      case META_EXTFLOODFILL :  break;

      case META_RESETDC :  break;
      case META_STARTDOC :  break;
      case META_STARTPAGE :  break;
      case META_ENDPAGE :  break;
      case META_ABORTDOC :  break;
      case META_ENDDOC :  break;

      case META_DELETEOBJECT :
        if (lpmr->rdParm[0] < MAX_OBJECTS)
          objects[lpmr->rdParm[0]].type = -1;
        break;

     case META_CREATEPENINDIRECT :
        for (i = 0 ; i < MAX_OBJECTS ; i++)
          if (objects[i].type < 0)
            {  objects[i].type = 0;
               objects[i].colour = *((DWORD *)&lpmr->rdParm[3]);
               objects[i].style = lpmr->rdParm[0];
               objects[i].width = lpmr->rdParm[1];
            }
        break;
      case META_CREATEBRUSHINDIRECT :
        for (i = 0 ; i < MAX_OBJECTS ; i++)
          if (objects[i].type < 0)
            {  objects[i].type = 1;
               objects[i].colour = *((DWORD *)&lpmr->rdParm[1]);
               objects[i].style = 0;
               objects[i].width = 0;
            }
        break;
      case META_CREATEPALETTE :
      case META_CREATEBRUSH :
      case META_CREATEPATTERNBRUSH :
      case META_CREATEBITMAPINDIRECT :
      case META_CREATEBITMAP :
      case META_CREATEREGION :
      case META_CREATEFONTINDIRECT :
        for (i = 0 ; i < MAX_OBJECTS ; i++)
          if (objects[i].type < 0)
            {  objects[i].type = 2;
               objects[i].colour = RGB(0,0,0);
               objects[i].style = 0;
               objects[i].width = 0;
            }
        break;
      default :  break;
     }
#endif
*/
//  return 1;
//}

static char buffer[2048];

void metafilein_command(int *cstate,HighLevelEvent *,void **)
{int i,nread;
 FILE *infile,*outfile;
 OPENFILENAME ofn;
 RCCHAR szDirName[256];
 RCCHAR szFile[256], szFileTitle[256],title[256],filename[256],*mffilename;
 RCCHAR  szFilter[256];
 RCHMETAFILE hmf;
 MFENUMPROC metaproc;
  _getcwd(szDirName, sizeof(szDirName));
  szFile[0] = '\0';
  strcpy(szFilter,"Metafiles *.WMF|*.WMF");
  for (i = 0; szFilter[i] != '\0'; i++)
    if (szFilter[i] == '|') szFilter[i] = '\0';
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
  ofn.lpstrDefExt = _RCT("WMF");
  ofn.lpstrFileTitle = title;
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (! GetOpenFileName(&ofn)) return;
  strcpy(filename,ofn.lpstrFile);

  i = 0;
  infile = fopen(filename,"rb");
  outfile = fopen(mffilename = home.gettempfilename(_RCT("MF")),"wb");
  scale = 1.0;
  height = 200.0;
  while ((nread = fread(buffer,1,2048,infile)) > 0)
    {  if (i == 0 && *((DWORD *)buffer) == 0x9AC6CDD7L)
         {  fwrite(buffer+22,1,nread-22,outfile);
            scale = 1.0 / *((short *)(buffer+14)) * 25.4;
            height = *((short *)(buffer+12)) * scale;
         }
       else
         fwrite(buffer,1,nread,outfile);
    }
  fclose(infile);
  fclose(outfile);

  curpen = curbrush = -1;
  for (i = 0 ; i < MAX_OBJECTS ; i++)
    objects[i].type = -1;
  hmf = GetMetaFile(mffilename);
  if (hmf == 0)
   RCMessageBox(cadwindow->gethwnd(),"Metafile not opened","",MB_ICONSTOP);
  else
    {  metaproc = (MFENUMPROC)MakeProcInstance( EnumMetaFileProc,program->gethinst());
       EnumMetaFile(cadwindow->gethdc(),(RCHMETAFILE)hmf,metaproc,(LPARAM)0);
       FreeProcInstance( metaproc);
       DeleteMetaFile(hmf);
    }
  _unlink(mffilename);
  *cstate = ExitState;
}

#else
void metafilein_command(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
}
#endif
