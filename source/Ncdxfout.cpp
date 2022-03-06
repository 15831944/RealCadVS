
#include "ncwin.h"
#ifndef _USING_QT
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if MENUS != STUDENTMENUS

DXFOut::DXFOut()
{int i;
 View3dWindowList *wl;
 View3dWindow *w;

  outfile = 0;  
  error = 0;
  ownerhandle = -1;

  scale = (v.getinteger("dx::units") == 0 ? 1.0 : 1.0 / 25.4);
  textscale = v.getreal("dx::textscale");

  dxfcolourtable[1].set(255,0,0);
  dxfcolourtable[2].set(255,255,0);
  dxfcolourtable[3].set(0,255,0);
  dxfcolourtable[4].set(0,255,255);
  dxfcolourtable[5].set(0,0,255);
  dxfcolourtable[6].set(255,0,255);
  dxfcolourtable[7].set(200,200,200);

  dxfcolourtable[9].set(127,0,0);
  dxfcolourtable[10].set(127,127,0);
  dxfcolourtable[11].set(0,127,0);
  dxfcolourtable[12].set(0,127,127);
  dxfcolourtable[13].set(0,0,127);
  dxfcolourtable[14].set(127,0,127);
  dxfcolourtable[15].set(200,200,200);
  dxfcolourtable[16].set(127,0,0);
  dxfcolourtable[17].set(127,0,0);
  for (i = 18 ; i <= 248 ; i++)
    {  dxfcolourtable[i].set(127,127,127);
       dxfcolourtable[i+1].set(127,127,127);
    }
  for (i = 20 ; i <= 30 ; i += 10)
    {  dxfcolourtable[i].set(255,0,0);
       dxfcolourtable[i+1].set(255,0,0);
       dxfcolourtable[i+2].set(255,0,0);
       dxfcolourtable[i+3].set(255,0,0);
       dxfcolourtable[i+210].set(255,0,0);
       dxfcolourtable[i+211].set(255,0,0);
       dxfcolourtable[i+212].set(255,0,0);
       dxfcolourtable[i+213].set(255,0,0);
    }
  for (i = 40 ; i <= 60 ; i += 10)
    {  dxfcolourtable[i].set(255,255,0);
       dxfcolourtable[i+1].set(255,255,0);
       dxfcolourtable[i+2].set(255,255,0);
       dxfcolourtable[i+3].set(255,255,0);
    }
  for (i = 70 ; i <= 110 ; i += 10)
    {  dxfcolourtable[i].set(0,255,0);
       dxfcolourtable[i+1].set(0,255,0);
       dxfcolourtable[i+2].set(0,255,0);
       dxfcolourtable[i+3].set(0,255,0);
    }
  for (i = 120 ; i <= 140 ; i += 10)
    {  dxfcolourtable[i].set(0,255,255);
       dxfcolourtable[i+1].set(0,255,255);
       dxfcolourtable[i+2].set(0,255,255);
       dxfcolourtable[i+3].set(0,255,255);
    }
  for (i = 150 ; i <= 190 ; i += 10)
    {  dxfcolourtable[i].set(0,0,255);
       dxfcolourtable[i+1].set(0,0,255);
       dxfcolourtable[i+2].set(0,0,255);
       dxfcolourtable[i+3].set(0,0,255);
    }
  for (i = 200 ; i <= 220 ; i += 10)
    {  dxfcolourtable[i].set(255,0,255);
       dxfcolourtable[i+1].set(255,0,255);
       dxfcolourtable[i+2].set(255,0,255);
       dxfcolourtable[i+3].set(255,0,255);
    }
  for (i = 24 ; i <= 34 ; i += 10)
    {  dxfcolourtable[i].set(127,0,0);
       dxfcolourtable[i+1].set(127,0,0);
       dxfcolourtable[i+2].set(127,0,0);
       dxfcolourtable[i+3].set(127,0,0);
       dxfcolourtable[i+210].set(127,0,0);
       dxfcolourtable[i+211].set(127,0,0);
       dxfcolourtable[i+212].set(127,0,0);
       dxfcolourtable[i+213].set(127,0,0);
    }
  for (i = 44 ; i <= 64 ; i += 10)
    {  dxfcolourtable[i].set(127,127,0);
       dxfcolourtable[i+1].set(127,127,0);
       dxfcolourtable[i+2].set(127,127,0);
       dxfcolourtable[i+3].set(127,127,0);
    }
  for (i = 74 ; i <= 114 ; i += 10)
    {  dxfcolourtable[i].set(0,127,0);
       dxfcolourtable[i+1].set(0,127,0);
       dxfcolourtable[i+2].set(0,127,0);
       dxfcolourtable[i+3].set(0,127,0);
    }
  for (i = 124 ; i <= 144 ; i += 10)
    {  dxfcolourtable[i].set(0,127,127);
       dxfcolourtable[i+1].set(0,127,127);
       dxfcolourtable[i+2].set(0,127,127);
       dxfcolourtable[i+3].set(0,127,127);
    }
  for (i = 154 ; i <= 194 ; i += 10)
    {  dxfcolourtable[i].set(0,0,127);
       dxfcolourtable[i+1].set(0,0,127);
       dxfcolourtable[i+2].set(0,0,127);
       dxfcolourtable[i+3].set(0,0,127);
    }
  for (i = 204 ; i <= 224 ; i += 10)
    {  dxfcolourtable[i].set(127,0,127);
       dxfcolourtable[i+1].set(127,0,127);
       dxfcolourtable[i+2].set(127,0,127);
       dxfcolourtable[i+3].set(127,0,127);
    }
  dxfcolourtable[250].set(64,64,64);
  dxfcolourtable[251].set(64,64,64);
  dxfcolourtable[252].set(127,127,127);
  dxfcolourtable[253].set(127,127,127);
  dxfcolourtable[254].set(255,255,255);
  dxfcolourtable[255].set(255,255,255);

  handle = 10000;

    //  Setup a default window most likely to have the correct plot scale
    if (typeid(cadwindow->getcurrentwindow()) == typeid(class View3dWindow) || db.views3d.match("Top") == 0)
    {   window = cadwindow->getcurrentwindow();
    }
    else
    {//  Use a top window first
        window = 0;
        wl = cadwindow->GetView3dList();
        for (wl->start();  (w = (View3dWindow *) wl->next()) != NULL ; )
            if (typeid(*w) == typeid(class View3dWindow) && strcmp(w->getview()->getname(),"Top") == 0)
            {   window = w;
                break;
            }
        if (window == 0)
        {   window = new View3dOutput(cadwindow->gethwnd(),cadwindow->gethdc(),db.views3d.match("Top"),1.0);
        }
    }

}

DXFOut::~DXFOut()
{   if (outfile != 0) 
        fclose(outfile);  
    if (typeid(*window) == typeid(class View3dOutput))
        delete window;
}

int DXFOut::open(char *s)
{
    return open(CHAR2RCCHR(s));
}

int DXFOut::open(RCCHAR *s)
{
  if (s == 0)
  {
    OPENFILENAME ofn;
    RCCHAR szDirName[256], szFile[256], szFileTitle[256],title[256],szFilter[256];
    UINT  i;
    ResourceString rs2(NCDXF+2);
    ResourceString rs3(NCDXF+3);
    ResourceString rs4(NCDXF+4);

    _getcwd(szDirName, sizeof(szDirName));
    szFile[0] = '\0';
    _tcscpy(szFilter,rs2.getws());
    //for (i = 0; szFilter[i] != '\0'; i++)
    //  if (szFilter[i] == '|') szFilter[i] = '\0';

    //qDebug() << "db.name" << QString(db.getname());
    //qDebug() << "db.title" << QString(db.gettitle());

    RCSTRING qTitle(db.getname());
    qTitle = qTitle.left(qTitle.lastIndexOf('.')) + ".dxf";
    strcpy(szFile,qTitle.data());
    qTitle = RCSTRING(db.gettitle());
    qTitle = qTitle.left(qTitle.lastIndexOf('.')) + ".dxf";
    strcpy(szFileTitle,qTitle.data());

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
    ofn.lpstrDefExt =  rs3.getws();
    ofn.lpstrFileTitle = title;
    ofn.lpstrTitle =  rs4.getws();
    ofn.Flags = 0;//OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn))
     outfile = fopen(RCSTRING(ofn.lpstrFile).toLatin1().data(),"w");
    else
     outfile = 0;
  }
  else
    outfile = fopen(s,"w");
  return outfile != 0;
}

void DXFOut::close(void)
{ if (outfile != 0)
    fclose(outfile);
  outfile = 0;
}

int DXFOut::writelong(int groupcode,long v)
{ return fprintf(outfile,"%d\n%ld\n",groupcode,v) != EOF;
}

int DXFOut::writereal(int groupcode,double v)
{char text[300];
  
  sprintf(text,"%d\n%.20lg",groupcode,v);

  if (strchr(text,'.') == 0)
    strcat(text,".0");

  return fprintf(outfile,"%s\n",text) != EOF;
}
int DXFOut::writestring(int groupcode,char *v)
{
    return fprintf(outfile,"%d\n%s\n",groupcode,v) != EOF;
}

int DXFOut::writestring(int groupcode,RCCHAR *v)
{
    return fprintf(outfile,"%d\n%s\n",groupcode,v) != EOF;
}

int DXFOut::writestring(int groupcode,char *v,int l)
{
    int i;
    if (fprintf(outfile,"%d\n",groupcode) == EOF)
        return 0;
    for (i = 0 ; i < l ; i++)
        if (fputc(v[i],outfile) == EOF)
            return 0;
    if (fputc('\n',outfile) == EOF)
        return 0;
    return 1;
}

int DXFOut::writestring(int groupcode,RCCHAR *v,int l)
{int i;
  if (fprintf(outfile,"%d\n",groupcode) == EOF)
    return 0;
  for (i = 0 ; i < l ; i++)
    if (fputc(v[i],outfile) == EOF)
      return 0;
  if (fputc('\n',outfile) == EOF)
    return 0;
  return 1;
}


int DXFOut::writehandle(void)
{ if (writelong(5,handle++)) 
    return handle-1;
  else
    return 0;
}

static double DXFWeight[] = 
  {  0.00, 0.05, 0.09, 0.13, 0.15, 0.18, 0.20,
     0.25, 0.30, 0.35, 0.40, 0.50, 0.53, 0.60,
     0.70, 0.80, 0.90, 1.06, 1.20, 1.40, 1.58,
     2.00, 2.11,-1.0
  };
    
int WeightToDXFWeight(double w)
{int i;
 double wx;

  wx = 0.0;
  for (i = 0 ; DXFWeight[i] >= 0.0 ; i++)
    if (fabs(w - DXFWeight[i]) < fabs(wx - DXFWeight[i]))
      wx = DXFWeight[i];

  return int(wx * 100.0 + 0.5);

}

int DXFOut::writeheader(Entity *e)
{int i,imin;
 double d,dmin,dr,dg,dbl;
 char string[256],layername[300],*s;

  if (! writelong(5,handle++)) return 0;
  if (! writestring(100, (char*)"AcDbEntity")) return 0;

  if (e->getstyle() == 0 || db.linestyles.indextostyle(e->getstyle()) == 0)
    strcpy(string,"CONTINUOUS");
  else
    sprintf(string,"LINESTYLE%d",e->getstyle());

  if (strlen(db.layernames.getlayername(e->getlayer())) == 0)
    sprintf(layername,"%d",e->getlayer());
  else
    strcpy(layername,db.layernames.getlayername(e->getlayer()));

  s = layername;
  while (*s != 0)
    {  if (strchr("<>/\\\":?*|,=`",*s) != 0)
          *s = ' ';   
       s++;
    } 

  if (! writestring(8,layername)) return 0;
  if (! writestring(6,string)) return 0;
  if (! writelong(370,WeightToDXFWeight(db.lineweights.weighttomm(e->getweight())))) return 0;
  dmin = -1.0;
  for (i = 0 ; i < 256 ; i++)
    {  dr = db.colourtable[e->getcolour()].red - dxfcolourtable[i].red;
       dg = db.colourtable[e->getcolour()].green - dxfcolourtable[i].green;
       dbl = db.colourtable[e->getcolour()].blue - dxfcolourtable[i].blue;
       d = dr * dr + dg * dg + dbl * dbl;
       if (i == 0 || d < dmin)
         {  dmin = d;  imin = i;
         }
    }
  if (! writelong(62,imin)) return 0;
  if (ownerhandle >= 0)
    {  if (! writelong(67,1)) return 0;
       if (! writelong(330,ownerhandle)) return 0;
    }
  return 1;
}

const char *dxfinfo1 = 
    "  0\n"
    "SECTION\n"
    "  2\n"
    "HEADER\n"
    "  9\n"
    "$ACADVER\n"
    "  1\n"
    "AC1015\n"
    "  9\n"
    "$ACADMAINTVER\n"
    " 70\n"
    "     6\n"
    "  9\n"
    "$DWGCODEPAGE\n"
    "  3\n"
    "ANSI_1252\n"
    "  9\n"
    "$INSBASE\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$EXTMIN\n"
    " 10\n"
    "%.8lf\n"
    " 20\n"
    "%.8lf\n"
    " 30\n"
    "%.8lf\n"
    "  9\n"
    "$EXTMAX\n"
    " 10\n"
    "%.8lf\n"
    " 20\n"
    "%.8lf\n"
    " 30\n"
    "%.8lf\n"
    "  9\n"
    "$LIMMIN\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    "  9\n"
    "$LIMMAX\n"
    " 10\n"
    "12.0\n"
    " 20\n"
    "9.0\n"
    "  9\n"
    "$ORTHOMODE\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$REGENMODE\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$FILLMODE\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$QTEXTMODE\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$MIRRTEXT\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$LTSCALE\n"
    " 40\n"
    "1.0\n"
    "  9\n"
    "$ATTMODE\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$TEXTSIZE\n"
    " 40\n"
    "0.2\n"
    "  9\n"
    "$TRACEWID\n"
    " 40\n"
    "0.05\n"
    "  9\n"
    "$TEXTSTYLE\n"
    "  7\n"
    "Standard\n"
    "  9\n"
    "$CLAYER\n"
    "  8\n"
    "0\n"
    "  9\n"
    "$CELTYPE\n"
    "  6\n"
    "BYLAYER\n"
    "  9\n"
    "$CECOLOR\n"
    " 62\n"
    "   256\n"
    "  9\n"
    "$CELTSCALE\n"
    " 40\n"
    "1.0\n"
    "  9\n"
    "$DISPSILH\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMSCALE\n"
    " 40\n"
    "1.0\n"
    "  9\n"
    "$DIMASZ\n"
    " 40\n"
    "0.18\n"
    "  9\n"
    "$DIMEXO\n"
    " 40\n"
    "0.0625\n"
    "  9\n"
    "$DIMDLI\n"
    " 40\n"
    "0.38\n"
    "  9\n"
    "$DIMRND\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$DIMDLE\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$DIMEXE\n"
    " 40\n"
    "0.18\n"
    "  9\n"
    "$DIMTP\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$DIMTM\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$DIMTXT\n"
    " 40\n"
    "0.18\n"
    "  9\n"
    "$DIMCEN\n"
    " 40\n"
    "0.09\n"
    "  9\n"
    "$DIMTSZ\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$DIMTOL\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMLIM\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMTIH\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$DIMTOH\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$DIMSE1\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMSE2\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMTAD\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMZIN\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMBLK\n"
    "  1\n"
    "\n"
    "  9\n"
    "$DIMASO\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$DIMSHO\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$DIMPOST\n"
    "  1\n"
    "\n"
    "  9\n"
    "$DIMAPOST\n"
    "  1\n"
    "\n"
    "  9\n"
    "$DIMALT\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMALTD\n"
    " 70\n"
    "     2\n"
    "  9\n"
    "$DIMALTF\n"
    " 40\n"
    "25.4\n"
    "  9\n"
    "$DIMLFAC\n"
    " 40\n"
    "1.0\n"
    "  9\n"
    "$DIMTOFL\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMTVP\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$DIMTIX\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMSOXD\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMSAH\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMBLK1\n"
    "  1\n"
    "\n"
    "  9\n"
    "$DIMBLK2\n"
    "  1\n"
    "\n"
    "  9\n"
    "$DIMSTYLE\n"
    "  2\n"
    "Standard\n"
    "  9\n"
    "$DIMCLRD\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMCLRE\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMCLRT\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMTFAC\n"
    " 40\n"
    "1.0\n"
    "  9\n"
    "$DIMGAP\n"
    " 40\n"
    "0.09\n"
    "  9\n"
    "$DIMJUST\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMSD1\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMSD2\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMTOLJ\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$DIMTZIN\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMALTZ\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMALTTZ\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMUPT\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMDEC\n"
    " 70\n"
    "     4\n"
    "  9\n"
    "$DIMTDEC\n"
    " 70\n"
    "     4\n"
    "  9\n"
    "$DIMALTU\n"
    " 70\n"
    "     2\n"
    "  9\n"
    "$DIMALTTD\n"
    " 70\n"
    "     2\n"
    "  9\n"
    "$DIMTXSTY\n"
    "  7\n"
    "Standard\n"
    "  9\n"
    "$DIMAUNIT\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMADEC\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMALTRND\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$DIMAZIN\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMDSEP\n"
    " 70\n"
    "    46\n"
    "  9\n"
    "$DIMATFIT\n"
    " 70\n"
    "     3\n"
    "  9\n"
    "$DIMFRAC\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$DIMLDRBLK\n"
    "  1\n"
    "\n"
    "  9\n"
    "$DIMLUNIT\n"
    " 70\n"
    "     2\n"
    "  9\n"
    "$DIMLWD\n"
    " 70\n"
    "    -2\n"
    "  9\n"
    "$DIMLWE\n"
    " 70\n"
    "    -2\n"
    "  9\n"
    "$DIMTMOVE\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$LUNITS\n"
    " 70\n"
    "     2\n"
    "  9\n"
    "$LUPREC\n"
    " 70\n"
    "     4\n"
    "  9\n"
    "$SKETCHINC\n"
    " 40\n"
    "0.1\n"
    "  9\n"
    "$FILLETRAD\n"
    " 40\n"
    "0.5\n"
    "  9\n"
    "$AUNITS\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$AUPREC\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$MENU\n"
    "  1\n"
    ".\n"
    "  9\n"
    "$ELEVATION\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$PELEVATION\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$THICKNESS\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$LIMCHECK\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$CHAMFERA\n"
    " 40\n"
    "0.5\n"
    "  9\n"
    "$CHAMFERB\n"
    " 40\n"
    "0.5\n"
    "  9\n"
    "$CHAMFERC\n"
    " 40\n"
    "1.0\n"
    "  9\n"
    "$CHAMFERD\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$SKPOLY\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$TDCREATE\n"
    " 40\n"
    "2453697.736472431\n"
    "  9\n"
    "$TDUCREATE\n"
    " 40\n"
    "2453697.298972430\n"
    "  9\n"
    "$TDUPDATE\n"
    " 40\n"
    "2453697.737544664\n"
    "  9\n"
    "$TDUUPDATE\n"
    " 40\n"
    "2453697.300044664\n"
    "  9\n"
    "$TDINDWG\n"
    " 40\n"
    "0.0010722338\n"
    "  9\n"
    "$TDUSRTIMER\n"
    " 40\n"
    "0.0010722338\n"
    "  9\n"
    "$USRTIMER\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$ANGBASE\n"
    " 50\n"
    "0.0\n"
    "  9\n"
    "$ANGDIR\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$PDMODE\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$PDSIZE\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$PLINEWID\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$SPLFRAME\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$SPLINETYPE\n"
    " 70\n"
    "     6\n"
    "  9\n"
    "$SPLINESEGS\n"
    " 70\n"
    "     8\n"
    "  9\n"
    "$HANDSEED\n"
    "  5\n"
    "2D\n"
    "  9\n"
    "$SURFTAB1\n"
    " 70\n"
    "     6\n"
    "  9\n"
    "$SURFTAB2\n"
    " 70\n"
    "     6\n"
    "  9\n"
    "$SURFTYPE\n"
    " 70\n"
    "     6\n"
    "  9\n"
    "$SURFU\n"
    " 70\n"
    "     6\n"
    "  9\n"
    "$SURFV\n"
    " 70\n"
    "     6\n"
    "  9\n"
    "$UCSBASE\n"
    "  2\n"
    "\n"
    "  9\n"
    "$UCSNAME\n"
    "  2\n"
    "\n"
    "  9\n"
    "$UCSORG\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSXDIR\n"
    " 10\n"
    "1.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSYDIR\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "1.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSORTHOREF\n"
    "  2\n"
    "\n"
    "  9\n"
    "$UCSORTHOVIEW\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$UCSORGTOP\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSORGBOTTOM\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSORGLEFT\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSORGRIGHT\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSORGFRONT\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$UCSORGBACK\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSBASE\n"
    "  2\n"
    "\n"
    "  9\n"
    "$PUCSNAME\n"
    "  2\n"
    "\n"
    "  9\n"
    "$PUCSORG\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSXDIR\n"
    " 10\n"
    "1.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSYDIR\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "1.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSORTHOREF\n"
    "  2\n"
    "\n"
    "  9\n"
    "$PUCSORTHOVIEW\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$PUCSORGTOP\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSORGBOTTOM\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSORGLEFT\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSORGRIGHT\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSORGFRONT\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PUCSORGBACK\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$USERI1\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$USERI2\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$USERI3\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$USERI4\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$USERI5\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$USERR1\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$USERR2\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$USERR3\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$USERR4\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$USERR5\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$WORLDVIEW\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$SHADEDGE\n"
    " 70\n"
    "     3\n"
    "  9\n"
    "$SHADEDIF\n"
    " 70\n"
    "    70\n"
    "  9\n"
    "$TILEMODE\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$MAXACTVP\n"
    " 70\n"
    "    64\n"
    "  9\n"
    "$PINSBASE\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 30\n"
    "0.0\n"
    "  9\n"
    "$PLIMCHECK\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$PEXTMIN\n"
    " 10\n"
    "1.000000000000000E+20\n"
    " 20\n"
    "1.000000000000000E+20\n"
    " 30\n"
    "1.000000000000000E+20\n"
    "  9\n"
    "$PEXTMAX\n"
    " 10\n"
    "-1.000000000000000E+20\n"
    " 20\n"
    "-1.000000000000000E+20\n"
    " 30\n"
    "-1.000000000000000E+20\n"
    "  9\n"
    "$PLIMMIN\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    "  9\n"
    "$PLIMMAX\n"
    " 10\n"
    "12.0\n"
    " 20\n"
    "9.0\n"
    "  9\n"
    "$UNITMODE\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$VISRETAIN\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$PLINEGEN\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$PSLTSCALE\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$TREEDEPTH\n"
    " 70\n"
    "  3020\n"
    "  9\n"
    "$CMLSTYLE\n"
    "  2\n"
    "Standard\n"
    "  9\n"
    "$CMLJUST\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$CMLSCALE\n"
    " 40\n"
    "1.0\n"
    "  9\n"
    "$PROXYGRAPHICS\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$MEASUREMENT\n"
    " 70\n"
    "     0\n"
    "  9\n"
    "$CELWEIGHT\n"
    "370\n"
    "    -1\n"
    "  9\n"
    "$ENDCAPS\n"
    "280\n"
    "     0\n"
    "  9\n"
    "$JOINSTYLE\n"
    "280\n"
    "     0\n"
    "  9\n"
    "$LWDISPLAY\n"
    "290\n"
    "     0\n"
    "  9\n"
    "$INSUNITS\n"
    " 70\n"
    "     1\n"
    "  9\n"
    "$HYPERLINKBASE\n"
    "  1\n"
    "\n"
    "  9\n"
    "$STYLESHEET\n"
    "  1\n"
    "\n"
    "  9\n"
    "$XEDIT\n"
    "290\n"
    "     1\n"
    "  9\n"
    "$CEPSNTYPE\n"
    "380\n"
    "     0\n"
    "  9\n"
    "$PSTYLEMODE\n"
    "290\n"
    "     1\n"
    "  9\n"
    "$FINGERPRINTGUID\n"
    "  2\n"
    "{B1D34980-8703-4819-BCEE-53DB1E82DC9D}\n"
    "  9\n"
    "$VERSIONGUID\n"
    "  2\n"
    "{EB8F25E2-E751-4D14-AEAD-6326C3829309}\n"
    "  9\n"
    "$EXTNAMES\n"
    "290\n"
    "     1\n"
    "  9\n"
    "$PSVPSCALE\n"
    " 40\n"
    "0.0\n"
    "  9\n"
    "$OLESTARTUP\n"
    "290\n"
    "     0\n"
    "  0\n"
    "ENDSEC\n"
    "  0\n"
    "SECTION\n"
    "  2\n"
    "CLASSES\n"
    "  0\n"
    "CLASS\n"
    "  1\n"
    "ACDBDICTIONARYWDFLT\n"
    "  2\n"
    "AcDbDictionaryWithDefault\n"
    "  3\n"
    "ObjectDBX Classes\n"
    " 90\n"
    "        0\n"
    "280\n"
    "     0\n"
    "281\n"
    "     0\n"
    "  0\n"
    "CLASS\n"
    "  1\n"
    "ACDBPLACEHOLDER\n"
    "  2\n"
    "AcDbPlaceHolder\n"
    "  3\n"
    "ObjectDBX Classes\n"
    " 90\n"
    "        0\n"
    "280\n"
    "     0\n"
    "281\n"
    "     0\n"
    "  0\n"
    "CLASS\n"
    "  1\n"
    "LAYOUT\n"
    "  2\n"
    "AcDbLayout\n"
    "  3\n"
    "ObjectDBX Classes\n"
    " 90\n"
    "        0\n"
    "280\n"
    "     0\n"
    "281\n"
    "     0\n"
    "  0\n"
    "ENDSEC\n"
    "  0\n"
    "SECTION\n"
    "  2\n"
    "TABLES\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "VPORT\n"
    "  5\n"
    "8\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     4\n"
    "  0\n"
    "VPORT\n"
    "  5\n"
    "2C\n"
    "330\n"
    "8\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbViewportTableRecord\n"
    "  2\n"
    "*Active\n"
    " 70\n"
    "     0\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 11\n"
    "1.0\n"
    " 21\n"
    "1.0\n"
    " 12\n"
    "0.0\n"
    " 22\n"
    "0.0\n"
    " 13\n"
    "0.0\n"
    " 23\n"
    "0.0\n"
    " 14\n"
    "0.5\n"
    " 24\n"
    "0.5\n"
    " 15\n"
    "0.5\n"
    " 25\n"
    "0.5\n"
    " 16\n"
    "0.0\n"
    " 26\n"
    "0.0\n"
    " 36\n"
    "1.0\n"
    " 17\n"
    "%.8lf\n"
    " 27\n"
    "%.8lf\n"
    " 37\n"
    "%.8lf\n"
    " 40\n"
    "%.8lf\n"
    " 41\n"
    "1.5\n"
    " 42\n"
    "50.0\n"
    " 43\n"
    "0.0\n"
    " 44\n"
    "0.0\n"
    " 50\n"
    "0.0\n"
    " 51\n"
    "0.0\n"
    " 71\n"
    "     0\n"
    " 72\n"
    "   100\n"
    " 73\n"
    "     1\n"
    " 74\n"
    "     3\n"
    " 75\n"
    "     0\n"
    " 76\n"
    "     0\n"
    " 77\n"
    "     0\n"
    " 78\n"
    "     0\n"
    "281\n"
    "     0\n"
    " 65\n"
    "     1\n"
    "110\n"
    "0.0\n"
    "120\n"
    "0.0\n"
    "130\n"
    "0.0\n"
    "111\n"
    "1.0\n"
    "121\n"
    "0.0\n"
    "131\n"
    "0.0\n"
    "112\n"
    "0.0\n"
    "122\n"
    "1.0\n"
    "132\n"
    "0.0\n"
    " 79\n"
    "     0\n"
    "146\n"
    "0.0\n"
    "  0\n"
    "ENDTAB\n";

static const char *dxfinfo2 =
    "  0\n"
    "TABLE\n"
    "  2\n"
    "LAYER\n"
    "  5\n"
    "2\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     1\n"
    "  0\n"
    "LAYER\n"
    "  5\n"
    "10\n"
    "330\n"
    "2\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbLayerTableRecord\n"
    "  2\n"
    "0\n"
    " 70\n"
    "     0\n"
    " 62\n"
    "     7\n"
    "  6\n"
    "CONTINUOUS\n"
    "370\n"
    "    -3\n"
    "390\n"
    "F\n"
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "STYLE\n"
    "  5\n"
    "3\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     1\n"
    "  0\n"
    "STYLE\n"
    "  5\n"
    "11\n"
    "330\n"
    "3\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbTextStyleTableRecord\n"
    "  2\n"
    "Standard\n"
    " 70\n"
    "     0\n"
    " 40\n"
    "0.0\n"
    " 41\n"
    "1.0\n"
    " 50\n"
    "0.0\n"
    " 71\n"
    "     0\n"
    " 42\n"
    "0.2\n"
    "  3\n"
    "txt\n"
    "  4\n"
    "\n"
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "VIEW\n"
    "  5\n"
    "6\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     0\n"
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "UCS\n"
    "  5\n"
    "7\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     0\n"
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "APPID\n"
    "  5\n"
    "9\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     1\n"
    "  0\n"
    "APPID\n"
    "  5\n"
    "12\n"
    "330\n"
    "9\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbRegAppTableRecord\n"
    "  2\n"
    "ACAD\n"
    " 70\n"
    "     0\n"
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "DIMSTYLE\n"
    "  5\n"
    "A\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     1\n"
    "100\n"
    "AcDbDimStyleTable\n"
    " 71\n"
    "     0\n"
    "  0\n"
    "DIMSTYLE\n"
    "105\n"
    "27\n"
    "330\n"
    "A\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbDimStyleTableRecord\n"
    "  2\n"
    "Standard\n"
    " 70\n"
    "     0\n"
    "340\n"
    "11\n"
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "TABLE\n"
    "  2\n"
    "BLOCK_RECORD\n"
    "  5\n"
    "1\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbSymbolTable\n"
    " 70\n"
    "     1\n"
    "  0\n"
    "BLOCK_RECORD\n"
    "  5\n"
    "1F\n"
    "330\n"
    "1\n"
    "100\n"
    "AcDbSymbolTableRecord\n"
    "100\n"
    "AcDbBlockTableRecord\n"
    "  2\n"
    "*Model_Space\n"
    "340\n"
    "22\n";

static const char *dxfinfo3 = 
    "  0\n"
    "ENDTAB\n"
    "  0\n"
    "ENDSEC\n";

static const char *dxfinfo4 = 
    "  0\n"
    "SECTION\n"
    "  2\n"
    "OBJECTS\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "C\n"
    "330\n"
    "0\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "     1\n"
    "  3\n"
    "ACAD_GROUP\n"
    "350\n"
    "D\n"
    "  3\n"
    "ACAD_LAYOUT\n"
    "350\n"
    "1A\n"
    "  3\n"
    "ACAD_MLINESTYLE\n"
    "350\n"
    "17\n"
    "  3\n"
    "ACAD_PLOTSETTINGS\n"
    "350\n"
    "19\n"
    "  3\n"
    "ACAD_PLOTSTYLENAME\n"
    "350\n"
    "E\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "D\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "C\n"
    "102\n"
    "}\n"
    "330\n"
    "C\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "     1\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "1A\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "C\n"
    "102\n"
    "}\n"
    "330\n"
    "C\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "     1\n"
    "  3\n"
    "Model\n"
    "350\n"
    "22\n";

static const char *dxfinfo5 = 
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "17\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "C\n"
    "102\n"
    "}\n"
    "330\n"
    "C\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "     1\n"
    "  3\n"
    "Standard\n"
    "350\n"
    "18\n"
    "  0\n"
    "DICTIONARY\n"
    "  5\n"
    "19\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "C\n"
    "102\n"
    "}\n"
    "330\n"
    "C\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "     1\n"
    "  0\n"
    "ACDBDICTIONARYWDFLT\n"
    "  5\n"
    "E\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "C\n"
    "102\n"
    "}\n"
    "330\n"
    "C\n"
    "100\n"
    "AcDbDictionary\n"
    "281\n"
    "     1\n"
    "  3\n"
    "Normal\n"
    "350\n"
    "F\n"
    "100\n"
    "AcDbDictionaryWithDefault\n"
    "340\n"
    "F\n"
    "  0\n"
    "LAYOUT\n"
    "  5\n"
    "22\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "1A\n"
    "102\n"
    "}\n"
    "330\n"
    "1A\n"
    "100\n"
    "AcDbPlotSettings\n"
    "  1\n"
    "\n"
    "  2\n"
    "Brother HL-1440 series\n"
    "  4\n"
    "\n"
    "  6\n"
    "\n"
    " 40\n"
    "0.0\n"
    " 41\n"
    "0.0\n"
    " 42\n"
    "0.0\n"
    " 43\n"
    "0.0\n"
    " 44\n"
    "0.0\n"
    " 45\n"
    "0.0\n"
    " 46\n"
    "0.0\n"
    " 47\n"
    "0.0\n"
    " 48\n"
    "0.0\n"
    " 49\n"
    "0.0\n"
    "140\n"
    "0.0\n"
    "141\n"
    "0.0\n"
    "142\n"
    "1.0\n"
    "143\n"
    "1.0\n"
    " 70\n"
    "  1712\n"
    " 72\n"
    "     0\n"
    " 73\n"
    "     0\n"
    " 74\n"
    "     0\n"
    "  7\n"
    "\n"
    " 75\n"
    "     0\n"
    "147\n"
    "1.0\n"
    "148\n"
    "0.0\n"
    "149\n"
    "0.0\n"
    "100\n"
    "AcDbLayout\n"
    "  1\n"
    "Model\n"
    " 70\n"
    "     1\n"
    " 71\n"
    "     0\n"
    " 10\n"
    "0.0\n"
    " 20\n"
    "0.0\n"
    " 11\n"
    "12.0\n"
    " 21\n"
    "9.0\n"
    " 12\n"
    "0.0\n"
    " 22\n"
    "0.0\n"
    " 32\n"
    "0.0\n"
    " 14\n"
    "0.0\n"
    " 24\n"
    "0.0\n"
    " 34\n"
    "0.0\n"
    " 15\n"
    "0.0\n"
    " 25\n"
    "0.0\n"
    " 35\n"
    "0.0\n"
    "146\n"
    "0.0\n"
    " 13\n"
    "0.0\n"
    " 23\n"
    "0.0\n"
    " 33\n"
    "0.0\n"
    " 16\n"
    "1.0\n"
    " 26\n"
    "0.0\n"
    " 36\n"
    "0.0\n"
    " 17\n"
    "0.0\n"
    " 27\n"
    "1.0\n"
    " 37\n"
    "0.0\n"
    " 76\n"
    "     0\n"
    "330\n"
    "1F\n"
    "  0\n"
    "MLINESTYLE\n"
    "  5\n"
    "18\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "17\n"
    "102\n"
    "}\n"
    "330\n"
    "17\n"
    "100\n"
    "AcDbMlineStyle\n"
    "  2\n"
    "STANDARD\n"
    " 70\n"
    "     0\n"
    "  3\n"
    "\n"
    " 62\n"
    "   256\n"
    " 51\n"
    "90.0\n"
    " 52\n"
    "90.0\n"
    " 71\n"
    "     2\n"
    " 49\n"
    "0.5\n"
    " 62\n"
    "   256\n"
    "  6\n"
    "BYLAYER\n"
    " 49\n"
    "-0.5\n"
    " 62\n"
    "   256\n"
    "  6\n"
    "BYLAYER\n"
    "  0\n"
    "ACDBPLACEHOLDER\n"
    "  5\n"
    "F\n"
    "102\n"
    "{ACAD_REACTORS\n"
    "330\n"
    "E\n"
    "102\n"
    "}\n"
    "330\n"
    "E\n";

char *FixFigureName(char *n)
{
 static char figurename[300];
 char *s1;

 qDebug() << "FixFigureName" << QString(n);

  // KMJ: Note. the figures paths are kept internally with
  // the Qt platform neutral separator '/'
  //if (strrchr(n,'\\') != 0)
  //  n = strrchr(n,'\\') + 1;
  if (strrchr(n,'/') != 0)
    n = strrchr(n,'/') + 1;

  if (*n >= 'A' && *n <= 'Z' || *n >= 'a' && *n <= 'z')
    strcpy(figurename,n);
  else
    {  figurename[0] = 'A';
       strcpy(figurename+1,n);
    }
   
  s1 = figurename;
  while (*s1 != 0)
    { if (*s1 == ' ' || *s1 == '.')
        *s1 = '-'; 
      s1++;
    }
  return figurename;
}

void DXFOut::write(EntityList *list)
{Entity *e;
 CStringList figures;
 char *s,figurename[300],paperspacename[300];
 Point o(0.0,0.0,0.0),xa(1.0,0.0,0.0),ya(0.0,1.0,0.0),scale(1.0,1.0,1.0),rotation(0.0,0.0,0.0),pmin,pmax;
 BitMask options(32);
 int i,j,k,index,blockhandle,viewhandle[MaxViews],viewno,ncomposeddrawings;
 OneView *ov;
 View2d *view2d;


  for (i = 0 ; i < MaxViews ; i++)
    viewhandle[i] = -1;

  if (list == 0)
    {  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         e->setstatus(1); 
    }
  else
    {  db.geometry.clearstatus();
       for (list->start() ; (e = list->next()) != NULL ; )
         {  e->setstatus(1 | 16);
            e->dbsetassociated();
            e->draw(DM_ERASE);
         }
    }

  for (db.geometry.start(),index = 10000 ; (e = db.geometry.next()) != 0 ; index++)
    e->setindex(index);

  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->getstatus() && e->isa(figure_entity))
      {  if (((Figure *)e)->getparameterlistlength() == 0)
            figures.add(RC2C(((Figure *)e)->getlibrary()));
         ((Figure *)e)->getsubfigures(&figures);
      }

  db.geometry.extents(0,&pmin,&pmax);
  fprintf(outfile,dxfinfo1,pmin.x,pmin.y,pmin.z,pmax.x,pmax.y,pmax.z,
                           (pmin.x+pmax.x)/2.0,(pmin.y+pmax.y)/2.0,(pmin.z+pmax.z)/2.0,(pmax.y - pmin.y)*1.5 + 1.0);
  db.linestyles.savedxf(this);
  fprintf(outfile,"%s",dxfinfo2);

  //  Write out the paper space block info
  ncomposeddrawings = 0;
  for (i = 0, j = 0 ; i < MaxViews ; i++)
    if ((view2d = db.views2d.match(i)) != 0 && view2d->getstatus())
      {  viewhandle[i] = 9000 + j * 5; 
         writestring(0, (char*)"BLOCK_RECORD");
         writelong(5,9000 + j *5);
         writestring(330, (char*)"1");
         writestring(100, (char*)"AcDbSymbolTableRecord");
         writestring(100, (char*)"AcDbBlockTableRecord");
         if (j == 0)
           sprintf(paperspacename,"*Paper_Space"); 
         else
           sprintf(paperspacename,"*Paper_Space%d",j); 
         writestring(2,paperspacename);
         writelong(340,9004+j*5);
         ncomposeddrawings++;
         j++;
      } 

  if (ncomposeddrawings == 0)
    {
     writestring(0, (char*)"BLOCK_RECORD");
     writelong(5,9000);
     writestring(330, (char*)"1");
     writestring(100, (char*)"AcDbSymbolTableRecord");
     writestring(100, (char*)"AcDbBlockTableRecord");
     writestring(2, (char*)"*Paper_Space");
     writelong(340,9004);
    } 


  //  Write out the new BLOCK_RECORD Info
  blockhandle = handle;

  for (figures.start() ; ((s = figures.next()) != 0) ; )
    {  writestring(0, (char*)"BLOCK_RECORD");
       writehandle();
       writestring(330, (char*)"1");
       writestring(100, (char*)"AcDbSymbolTableRecord");
       writestring(100, (char*)"AcDbBlockTableRecord");
       writestring(2,FixFigureName(s));
       writestring(330, (char*)"0");
    } 

  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->getstatus() && e->isa(figure_entity) && ((Figure *)e)->getparameterlistlength() != 0)
      {  writestring(0, (char*)"BLOCK_RECORD");
         writehandle();
         writestring(330, (char*)"1");
         writestring(100, (char*)"AcDbSymbolTableRecord");
         writestring(100, (char*)"AcDbBlockTableRecord");
         strcpy(figurename,FixFigureName(RC2C(((Figure *)e)->getlibrary())));
         sprintf(figurename+strlen(figurename),"-%8.8lx",e);
         writestring(2,figurename);
         writestring(330, (char*)"0");
      } 

  fprintf(outfile,"%s",dxfinfo3);
  writestring(0, (char*)"SECTION");
  writestring(2, (char*)"BLOCKS");

  writestring(0, (char*)"BLOCK");
  writelong(5,900);
  writestring(330, (char*)"1F");
  writestring(100, (char*)"AcDbEntity");
  writelong(8,0);
  writestring(100, (char*)"AcDbBlockBegin");
  writestring(2, (char*)"*Model_Space");
  writelong(70,0);
  writelong(71,0);
  writereal(10,0.0);
  writereal(20,0.0);
  writereal(30,0.0);
  writestring(3, (char*)"*Model_Space");
  writestring(1, (char*)"");
  writestring(0, (char*)"ENDBLK");
  writelong(5,901);
  writestring(330, (char*)"1F");
  writestring(100, (char*)"AcDbEntity");
  writelong(8,0);
  writestring(100, (char*)"AcDbBlockEnd");


  for (i = 0,j = 0 ; i < MaxViews ; i++)
    if ((view2d = db.views2d.match(i)) != 0 && view2d->getstatus() || ncomposeddrawings == 0 && i == 0)
      {  if (j == 0)
           sprintf(paperspacename,"*Paper_Space"); 
         else
           sprintf(paperspacename,"*Paper_Space%d",j); 
         if (ncomposeddrawings == 0)
            view2d = 0;
         writestring(0, (char*)"BLOCK");
         writelong(5,9001 + j*5);
         writelong(330,9000 + j*5);
         writestring(100, (char*)"AcDbEntity");
         writelong(67,1);
         writelong(8,0);
         writestring(100, (char*)"AcDbBlockBegin");
         writestring(2,paperspacename);
         writelong(70,0);
         writelong(71,0);
         writereal(10,0.0);
         writereal(20,0.0);
         writereal(30,0.0);
         writestring(3,paperspacename);
         writestring(1, (char*)"");

         if (j > 0)
           {//  Add the viewport records
            writestring(0, (char*)"VIEWPORT");
            writelong(5,9003+j*5);
            writelong(330,9000+j*5);
            writestring(100, (char*)"AcDbEntity");
            writelong(67,1);
            writelong(8,0);
            writestring(100, (char*)"AcDbViewport");
            writereal(10,view2d == 0 ? 105.0 : view2d->getpaperwidth()/2.0);
            writereal(20,view2d == 0 ? 148.5 : view2d->getpaperheight()/2.0);
            writereal(30,0.0);
            writereal(40,view2d == 0 ? 210.0 : view2d->getpaperwidth());
            writereal(41,view2d == 0 ? 297.0 : view2d->getpaperheight());
            writelong(68,1);
            writelong(69,1);
            writereal(12,view2d == 0 ? 105.0 : view2d->getpaperwidth()/2.0);
            writereal(22,view2d == 0 ? 148.5 : view2d->getpaperheight()/2.0);

            writereal(13,0.0);
            writereal(23,0.0);
            writereal(14,1.0);
            writereal(24,1.0);
            writereal(15,10.0);
            writereal(25,10.0);

            writereal(16,0.0);
            writereal(26,0.0);
            writereal(36,1.0);
            writereal(17,0.0);
            writereal(27,0.0);
            writereal(37,0.0);
            writereal(42,50.0);
            writereal(43,0.0);
            writereal(44,0.0);
            writereal(45,view2d == 0 ? 297.0 : view2d->getpaperheight());
            writereal(50,0.0);
            writereal(51,0.0);
            writelong(72,100);
            writelong(90,32800);
            writestring(1, (char*)"");
            writelong(281,0);
            writelong(71,1);
            writelong(74,0);
            writereal(110,0.0);
            writereal(120,0.0);
            writereal(130,0.0);
            writereal(111,1.0);
            writereal(121,0.0);
            writereal(131,0.0);
            writereal(112,0.0);
            writereal(122,1.0);
            writereal(132,0.0);

            writelong(79,0);
            writereal(146,0.0);

            if (view2d != 0)
             for (view2d->start(),k = 0 ; (ov = view2d->next()) != 0 ; k++)
              {//  Create a viewport for each view on a drawing sheet
               writestring(0, (char*)"VIEWPORT");
               writelong(5,handle++);
               writelong(330,9000+j*5);
               writestring(100, (char*)"AcDbEntity");
               writelong(67,1);
               writelong(8,0);
               writestring(100, (char*)"AcDbViewport");

               Point uaxis = ((ov->getview()->getref() - ov->getview()->geteye()).cross(ov->getview()->getup())).normalize();
               Point vaxis = (ov->getview()->getup()).normalize();
               writereal(10,ov->getxpos() + (ov->getumin() + (ov->getumax() - ov->getumin()) / 2.0) / ov->getscale());  //  Centre point in WCS
               writereal(20,ov->getypos() + (ov->getvmin() + (ov->getvmax() - ov->getvmin()) / 2.0) / ov->getscale());
               writereal(30,0.0);
               writereal(40,(ov->getumax() - ov->getumin()) / ov->getscale());      //  Width in paper space units
               writereal(41,(ov->getvmax() - ov->getvmin()) / ov->getscale());      //  Height in paper space units
               writelong(68,1);
               writelong(69,2 + k);

               writereal(12,ov->getumin() + (ov->getumax() - ov->getumin()) / 2.0);      //  View center point in DCS
               writereal(22,ov->getvmin() + (ov->getvmax() - ov->getvmin()) / 2.0);

               writereal(13,0.0);                              //  Snap base point
               writereal(23,0.0);
               writereal(14,1.0);                              //  Snap spacing
               writereal(24,1.0);
               writereal(15,10.0);                             //  Grid spacing
               writereal(25,10.0);

               Point dir = (ov->getview()->geteye() - ov->getview()->getref()).normalize();
               writereal(16,dir.x);                           //  View direction
               writereal(26,dir.y);
               writereal(36,dir.z);
               writereal(17,ov->getview()->getref().x);       //  View target
               writereal(27,ov->getview()->getref().y);
               writereal(37,ov->getview()->getref().z);
               writereal(42,(ov->getview()->geteye() - ov->getview()->getref()).length());
               writereal(43,ov->getview()->getfront());       //  Clip planes
               writereal(44,ov->getview()->getback());
               writereal(45,ov->getvmax() - ov->getvmin());    //  View height (model space)
               writereal(50,0.0);
               writereal(51,0.0);
               writelong(72,100);
               writelong(90,ov->getview()->getperspective() * 1 + 32768);
               writestring(1, (char*)"");
               writelong(281,1);
               writelong(71,1);           //  UCS information
               writelong(74,0);
               writereal(110,0.0);
               writereal(120,0.0);
               writereal(130,0.0);
               writereal(111,1.0);
               writereal(121,0.0);
               writereal(131,0.0);
               writereal(112,0.0);
               writereal(122,1.0);
               writereal(132,0.0);

               writelong(79,0);
               writereal(146,0.0);        //  Elevation
              }  

            for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
              {  setownerhandle(-1);
                 viewno = -2;
                 for (k = 0 ; k < MaxViews ; k++)
                   if (e->getvisible()->test(k) && (viewhandle[k] == -1 || viewhandle[k] == 9000 + j*5))
                     {  if (viewno == -2)
                          {  setownerhandle(viewhandle[k]);  //  Assume for now this is a paper space entity - visible in only one view
                             viewno = k;
                          }   
                        else
                          {  viewno = -1;
                             setownerhandle(-1);             //  This is visible in more than one view - make it a model entity
                             break;
                          }
                     } 
                 if (viewno >= 0 && e->getstatus())  //  Only save entities on this composed drawing
                   e->savedxf(0,this);
              }

           }

         writestring(0, (char*)"ENDBLK");
         writelong(5,9002 + j*5);
         writelong(330,9000+j*5);
         writestring(100, (char*)"AcDbEntity");
         writelong(8,0);
         writestring(100, (char*)"AcDbBlockEnd");
         j++;
      } 

  for (figures.start() ; ((s = figures.next()) != 0) ; )
    {  setownerhandle(blockhandle);
       writestring(0, (char*)"BLOCK");
       writehandle();
       writelong(330,blockhandle++);
       writestring(100, (char*)"AcDbEntity");
       writelong(8,0);
       writestring(100, (char*)"AcDbBlockBegin");
       writestring(2,FixFigureName(s));
       writelong(70,0);
       writereal(10,0.0);
       writereal(20,0.0);
       writereal(30,0.0);
       Figure figure(_RCT(s),_RCT(""),o,xa,ya,scale,rotation,Point(0.0,0.0,0.0),0,0,0,0,options);
       writestring(3,FixFigureName(s));
       writestring(1, (char*)"");
       figure.savedxf(1,this);
       writestring(0, (char*)"ENDBLK");
       writehandle();
       writestring(100, (char*)"AcDbEntity");
       writestring(100, (char*)"AcDbBlockEnd");
    }

  //  Also save the parametric figures.
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->getstatus() && e->isa(figure_entity) && ((Figure *)e)->getparameterlistlength() != 0)
      {  setownerhandle(blockhandle);
         writestring(0, (char*)"BLOCK");
         writehandle();
         writelong(330,blockhandle++);
         writestring(100, (char*)"AcDbEntity");
         writelong(8,0);
         writestring(100, (char*)"AcDbBlockBegin");
         strcpy(figurename,FixFigureName(RC2C(((Figure *)e)->getlibrary())));
         sprintf(figurename+strlen(figurename),"-%8.8lx",e);
         writestring(2,figurename);
         writelong(70,0);
         writereal(10,0.0);
         writereal(20,0.0);
         writereal(30,0.0);
         writestring(3,figurename);
         writestring(1, (char*)"");
         ((Figure *)e)->savedxf(1,this);
         writestring(0, (char*)"ENDBLK");
         writehandle();
         writestring(100, (char*)"AcDbEntity");
         writestring(100, (char*)"AcDbBlockEnd");
      }

  writestring(0, (char*)"ENDSEC");
  writestring(0, (char*)"SECTION");
  writestring(2, (char*)"ENTITIES");
       
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    {  setownerhandle(-1);
       viewno = -2;
       for (i = 0 ; i < MaxViews ; i++)
         if (e->getvisible()->test(i) && (viewhandle[i] == -1 || viewhandle[i] == 9000))
           {  if (viewno == -2)
                {  setownerhandle(viewhandle[i]);  //  Assume for now this is a paper space entity - visible in only one view
                   viewno = i;
                }   
              else
                {  viewno = -1;
                   setownerhandle(-1);             //  This is visible in more than one view - make it a model entity
                   break;
                }
           } 
       if (viewno >= -1 && e->getstatus())  //  Don't save invisible entities
         e->savedxf(0,this);
    }

  for (i = 0,j = 0 ; i < MaxViews ; i++)
    if ((view2d = db.views2d.match(i)) != 0 && view2d->getstatus() || ncomposeddrawings == 0 && i == 0)
      {  if (j == 0)
           sprintf(paperspacename,"*Paper_Space"); 
         else
           sprintf(paperspacename,"*Paper_Space%d",j); 
         if (ncomposeddrawings == 0)
            view2d = 0;
         if (j == 0)
           {//  Add the viewport records
            writestring(0, (char*)"VIEWPORT");
            writelong(5,9003+j*5);
            writelong(330,9000+j*5);
            writestring(100, (char*)"AcDbEntity");
            writelong(67,1);
            writelong(8,0);
            writestring(100, (char*)"AcDbViewport");
            writereal(10,view2d == 0 ? 105.0 : view2d->getpaperwidth()/2.0);
            writereal(20,view2d == 0 ? 148.5 : view2d->getpaperheight()/2.0);
            writereal(30,0.0);
            writereal(40,view2d == 0 ? 210.0 : view2d->getpaperwidth());
            writereal(41,view2d == 0 ? 297.0 : view2d->getpaperheight());
            writelong(68,1);
            writelong(69,1);
            writereal(12,view2d == 0 ? 148.5 : view2d->getpaperwidth()/2.0);
            writereal(22,view2d == 0 ? 297.0 : view2d->getpaperheight()/2.0);

            writereal(13,0.0);
            writereal(23,0.0);
            writereal(14,1.0);
            writereal(24,1.0);
            writereal(15,10.0);
            writereal(25,10.0);

            writereal(16,0.0);
            writereal(26,0.0);
            writereal(36,1.0);
            writereal(17,0.0);
            writereal(27,0.0);
            writereal(37,0.0);
            writereal(42,50.0);
            writereal(43,0.0);
            writereal(44,0.0);
            writereal(45,view2d == 0 ? 297.0 : view2d->getpaperheight());
            writereal(50,0.0);
            writereal(51,0.0);
            writelong(72,100);
            writelong(90,32800);
            writestring(1, (char*)"");
            writelong(281,0);
            writelong(71,1);
            writelong(74,0);
            writereal(110,0.0);
            writereal(120,0.0);
            writereal(130,0.0);
            writereal(111,1.0);
            writereal(121,0.0);
            writereal(131,0.0);
            writereal(112,0.0);
            writereal(122,1.0);
            writereal(132,0.0);

            writelong(79,0);
            writereal(146,0.0);

            if (view2d != 0)
             for (view2d->start(),k = 0 ; (ov = view2d->next()) != 0 ; k++)
              {//  Create a viewport for each view on a drawing sheet
               writestring(0, (char*)"VIEWPORT");
               writelong(5,handle++);
               writelong(330,9000+j*5);
               writestring(100, (char*)"AcDbEntity");
               writelong(67,1);
               writelong(8,0);
               writestring(100, (char*)"AcDbViewport");

               Point uaxis = ((ov->getview()->getref() - ov->getview()->geteye()).cross(ov->getview()->getup())).normalize();
               Point vaxis = (ov->getview()->getup()).normalize();
               writereal(10,ov->getxpos() + (ov->getumin() + (ov->getumax() - ov->getumin()) / 2.0) / ov->getscale());  //  Centre point in WCS
               writereal(20,ov->getypos() + (ov->getvmin() + (ov->getvmax() - ov->getvmin()) / 2.0) / ov->getscale());
               writereal(30,0.0);
               writereal(40,(ov->getumax() - ov->getumin()) / ov->getscale());      //  Width in paper space units
               writereal(41,(ov->getvmax() - ov->getvmin()) / ov->getscale());      //  Height in paper space units
               writelong(68,1);
               writelong(69,2 + k);

               writereal(12,ov->getumin() + (ov->getumax() - ov->getumin()) / 2.0);      //  View center point in DCS
               writereal(22,ov->getvmin() + (ov->getvmax() - ov->getvmin()) / 2.0);

               writereal(13,0.0);                              //  Snap base point
               writereal(23,0.0);
               writereal(14,1.0);                              //  Snap spacing
               writereal(24,1.0);
               writereal(15,10.0);                             //  Grid spacing
               writereal(25,10.0);

               Point dir = (ov->getview()->geteye() - ov->getview()->getref()).normalize();
               writereal(16,dir.x);                           //  View direction
               writereal(26,dir.y);
               writereal(36,dir.z);
               writereal(17,ov->getview()->getref().x);       //  View target
               writereal(27,ov->getview()->getref().y);
               writereal(37,ov->getview()->getref().z);
               writereal(42,(ov->getview()->geteye() - ov->getview()->getref()).length());
               writereal(43,ov->getview()->getfront());       //  Clip planes
               writereal(44,ov->getview()->getback());
               writereal(45,ov->getvmax() - ov->getvmin());    //  View height (model space)
               writereal(50,0.0);
               writereal(51,0.0);
               writelong(72,100);
               writelong(90,ov->getview()->getperspective() * 1 + 32768);
               writestring(1, (char*)"");
               writelong(281,1);
               writelong(71,1);           //  UCS information
               writelong(74,0);
               writereal(110,0.0);
               writereal(120,0.0);
               writereal(130,0.0);
               writereal(111,1.0);
               writereal(121,0.0);
               writereal(131,0.0);
               writereal(112,0.0);
               writereal(122,1.0);
               writereal(132,0.0);

               writelong(79,0);
               writereal(146,0.0);        //  Elevation
              }  
           }
         j++;
      } 

  writestring(0, (char*)"ENDSEC");
  fprintf(outfile,"%s",dxfinfo4);

  //  Add the dictionary entries
  for (i = 0,j = 0 ; i < MaxViews ; i++)
    if ((view2d = db.views2d.match(i)) != 0 && view2d->getstatus() || ncomposeddrawings == 0 && i == 0)
      {  if (ncomposeddrawings == 0)
            view2d = 0;
         writestring(3,(char*)(view2d == 0 ? "Layout" : RC2C(view2d->getname())));
         writelong(350,9004+j*5);
         j++;
      }

  fprintf(outfile,"%s",dxfinfo5);

  //  Layout belongs in the objects section
  j = 0;
  for (i = 0 ; i < MaxViews ; i++)
    if ((view2d = db.views2d.match(i)) != 0 && view2d->getstatus() || ncomposeddrawings == 0 && i == 0)
      {  if (ncomposeddrawings == 0)
            view2d = 0;
         writestring(0, (char*)"LAYOUT");
         writelong(5,9004+j*5);
         writestring(102, (char*)"{ACAD_XDICTIONARY");
         writelong(360,0);
         writestring(102, (char*)"}");
         writestring(102, (char*)"{ACAD_REACTORS");
         writestring(330, (char*)"1A");          //  Soft-pointer ID/handle to owner object
         writestring(102, (char*)"}");
         writestring(330, (char*)"1A");          //  Soft-pointer ID/handle to owner object

         writestring(100, (char*)"AcDbPlotSettings");  //  Plot Settings
         writestring(1, (char*)"");
         writestring(2, (char*)"none_device");
         writestring(4, (char*)"Custom");
         writestring(6, (char*)"");
         writereal(40,0.0);
         writereal(41,0.0);
         writereal(42,0.0);
         writereal(43,0.0);
         writereal(44,view2d == 0 ? 210.0 : view2d->getpaperwidth());
         writereal(45,view2d == 0 ? 297.0 : view2d->getpaperheight());
         writereal(46,0.0);
         writereal(47,0.0);
         writereal(48,0.0);
         writereal(49,0.0);
         writereal(140,0.0);
         writereal(141,0.0);
         writereal(142,1.0);
         writereal(143,1.0);
         writelong(70,688);
         writelong(72,1);                   //  Work in mm
         writelong(73,0);
         writelong(74,4);
         writestring(7, (char*)"");
         writelong(75,15);
         writereal(147,1.0);
         writereal(148,0.0);
         writereal(149,0.0);

         writestring(100, (char*)"AcDbLayout");  //  Soft-pointer ID/handle to owner object
         writestring(1,(char*)(view2d == 0 ? "Layout" : RC2C(view2d->getname())));
         writelong(70,1);  
         writelong(71,j);  

         writereal(10,0.0);   //  Minimum limits
         writereal(20,0.0);  
         writereal(11,view2d == 0 ? 210.0 : view2d->getpaperwidth());  
         writereal(21,view2d == 0 ? 297.0 : view2d->getpaperheight());  
         writereal(12,0.0);   //  Insertion point
         writereal(22,0.0);  
         writereal(32,0.0);  
         writereal(14,0.0);   //  Extents
         writereal(24,0.0);  
         writereal(34,-0.001);  
         writereal(15,view2d == 0 ? 210.0 : view2d->getpaperwidth());  
         writereal(25,view2d == 0 ? 297.0 : view2d->getpaperheight());  
         writereal(35,0.001);  
         writereal(146,0.0);  //  Elevation
         writereal(13,0.0);   //  UCS
         writereal(23,0.0);  
         writereal(33,0.0);  
         writereal(16,1.0);  
         writereal(26,0.0);  
         writereal(36,0.0);  
         writereal(17,0.0);  
         writereal(27,1.0);  
         writereal(37,0.0);  
         writelong(76,0);  

         writelong(330,9000+j*5);               // ID/handle to this layout’s associated paper space block table record
         writelong(331,9003+j*5);              //  ID/handle to the viewport that was last active in this layout when the layout was current
         j++;

      }

  writestring(0, (char*)"ENDSEC");
  writestring(0, (char*)"EOF");

}

class DXFCDListDialogControl : public ListDialogControl
{private:
 public:
    DXFCDListDialogControl(int id,int n,RCCHAR **list) : ListDialogControl(id,n,(RCCHAR**)list,0,0) {}
    void load(Dialog *);
    int store(Dialog *);
};

class DXFCDButtonDialogControl : public ButtonDialogControl
{private:
 public:
    DXFCDButtonDialogControl(int id) : ButtonDialogControl(id)  {     }
    int select(Dialog *);
};

int DXFCDButtonDialogControl::select(Dialog *dialog)
{   if (id == 103)
        SendDlgItemMessage(dialog->gethdlg(),102,LB_SELITEMRANGE,1,MAKELPARAM(0,-1));
    else
        SendDlgItemMessage(dialog->gethdlg(),102,LB_SELITEMRANGE,0,MAKELPARAM(0,-1));
    return 0;
}

void DXFCDListDialogControl::load(Dialog *dialog)
{   ListDialogControl::load(dialog);
    SendDlgItemMessage(dialog->gethdlg(),id,LB_SELITEMRANGE,1,MAKELPARAM(0,-1));
}

int DXFCDListDialogControl::store(Dialog *dialog)
{int i,nselecteditems,*selecteditems;
 View2d *view2d;
 RCCHAR name[300];
 
    nselecteditems = SendDlgItemMessage(dialog->gethdlg(),102,LB_GETSELCOUNT,0,0);
    selecteditems = new int[nselecteditems];
    SendDlgItemMessage(dialog->gethdlg(),102,LB_GETSELITEMS,nselecteditems,(LPARAM)selecteditems);
    for (i = 0 ; i < nselecteditems ; i++)
    {   SendDlgItemMessage(dialog->gethdlg(),102,LB_GETTEXT,selecteditems[i],(LPARAM)name);
        if ((view2d = db.views2d.match(name)) != NULL)
        {   view2d->setstatus(1);
        }
    }
    delete [] selecteditems;

    return 1;
}


#if 1
// this is RealCAD v5 code
int DXFOut::setscale(void)
{
 Dialog dialog("DXFOut_Dialog");
 DialogControl *dc;
 int units,selected_entities;
 double curvelength;
 int i,n2d,index;

    units = v.getinteger("dx::units");
    curvelength = v.getreal("dx::curvelength");
    textscale = v.getreal("dx::textscale");

    dialog.add(new RadioButtonDialogControl(100,100,101,&units));
    dialog.add(new RadioButtonDialogControl(101,100,101,&units));
    dialog.add(dc = new RealDialogControl(102,&curvelength,ScaleByLength));
    dialog.add(dc = new RealDialogControl(103,&textscale,DontScale,0.0001,1000000.0));

    if (dialog.process() == TRUE)
    {   scale = (units == 0 ? 1.0 : 1.0 / 25.4);
        v.setinteger("dx::units",units);
        v.setreal("dx::curvelength",curvelength);
        v.setreal("dx::textscale",textscale);
        return 1;
    }
    else
        return 0;
}
#else
// this is RealCAD V 7 code let's try it later
int DXFOut::setscale(void)
{Dialog dialog("DXFOut_Dialog");
 DialogControl *dc;
 int units,selected_entities;
 double curvelength;
 int i,n2d,index;
 RCCHAR **view2dlist;
 View2d *view2d;

    //   Deselect all views
    for (i = 0 ; i < MaxViews ; i++)
        if ((view2d = db.views2d.match(i)) != 0)
            view2d->setstatus(0);

    units = v.getinteger("dx::units");
    selected_entities = v.getinteger("dx::selected_entities");
    curvelength = v.getreal("dx::curvelength");
    textscale = v.getreal("dx::textscale");

    db.views2d.names(&view2dlist,&n2d,&index);

    dialog.add(new RadioButtonDialogControl(100,100,101,&selected_entities));
    dialog.add(new RadioButtonDialogControl(101,100,101,&selected_entities));
    dialog.add(new DXFCDListDialogControl(102,n2d,view2dlist));
    dialog.add(new DXFCDButtonDialogControl(103));
    dialog.add(new DXFCDButtonDialogControl(104));
    dialog.add(new RadioButtonDialogControl(105,105,106,&units));
    dialog.add(new RadioButtonDialogControl(106,105,106,&units));
    dialog.add(dc = new RealDialogControl(107,&curvelength,ScaleByLength));
    dialog.add(new ScrollBarDialogControl(1107,dc));
    dialog.add(dc = new RealDialogControl(108,&textscale,DontScale,0.0001,1000000.0));
    dialog.add(new ScrollBarDialogControl(1108,dc));

    if (dialog.process() == TRUE)
    {   scale = (units == 0 ? 1.0 : 1.0 / 25.4);
        v.setinteger("dx::units",units);
        v.setreal("dx::curvelength",curvelength);
        v.setreal("dx::textscale",textscale);
        v.setinteger("dx::selected_entities",selected_entities);
        delete [] view2dlist;
        return 1;
    }
    else
    {   delete [] view2dlist;
        return 0;
    }

}
#endif

typedef int (*Convert)(int todxf,char *infilename/*,char *outfilename*/);
//typedef int (*Convert)(int todxf,char *infilename,char *outfilename);
//typedef int (*Convert)(int todxf,RCCHAR *infilename,RCCHAR *outfilename);

#if 1
// using the new dwg/dxf exporter
void dxfout_command(int *cstate,HighLevelEvent *,void **)
{
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libExporter.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Exporter.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
            convert(0,0); // dxf export
    }

    *cstate = ExitState;
}
#else
#if 1
// this is RealCAD v5 code
void dxfout_command(int *cstate,HighLevelEvent *,void **)
{
    DXFOut dxffile;
    if (cadwindow->getcurrentwindow() == 0)
        cadwindow->MessageBox("Please select a window","Export DXF",MB_ICONINFORMATION);
    else if (dxffile.setscale() && dxffile.open((RCCHAR*)0))
    {
        program->setbusy(1);
        dxffile.write(0);
        program->setbusy(0);
        dxffile.close();
    }
    *cstate = ExitState;
}
#else
// this is RealCAD v7 code let's try it later
void dxfout_command(int *cstate,HighLevelEvent *,void **)
{DXFOut dxffile;
    if (cadwindow->getcurrentwindow() == 0)
        cadwindow->MessageBox("Please select a window","Export DXF",MB_ICONINFORMATION);
    else if (dxffile.setscale() && dxffile.open((char*)0))
    {EntityList *list;
        if (v.getinteger("dx::selected_entities") )
        {   list = new EntityList();
            *list = state.getselection();
            if (list->length() == 0)
            {   cadwindow->MessageBox("The entities to export must be selected first","Export DXF",MB_OK);
                return;
            }
        }
        else
            list = 0;
        program->setbusy(1);
        dxffile.write(list);
        program->setbusy(0);
        dxffile.close();
        delete list;
    }

    *cstate = ExitState;
 
}
#endif
#endif


#if 1
void dwgout_command(int *cstate,HighLevelEvent *,void **)
{
    QLibrary *appinstance;
    Convert convert;

#ifdef _MAC
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("libExporter.1.0.0.dylib")));
#else
    appinstance = (QLibrary*)program->loadcadmodule(home.getexecutablefilename(_RCT("Exporter.dll")));
#endif

    if(appinstance)
    {
        convert = (Convert) appinstance->resolve("Convert");
        if(convert)
            convert(1,0); // dwg export
/*
#ifdef _MAC
        program->unloadcadmodule(home.getexecutablefilename(_RCT("libExporter.1.0.0.dylib")));
#else
        program->unloadcadmodule(home.getexecutablefilename(_RCT("Exporter.dll")));
#endif
*/
    }

    *cstate = ExitState;
}
#else
#if 1
// this is RealCAD v 5 code using this now
void dwgout_command(int *cstate,HighLevelEvent *,void **)
{OPENFILENAME ofn;
 RC_STAT sbuf;
 STARTUPINFO si;
 PROCESS_INFORMATION pi;
 RCCHAR filename[300],dirname[300],filter[300],title[300],szFileTitle[300],pfilename[300],command[300],parameter[300];
 char dwgfilename[300],dxffilename[300];
 int i;
 Convert convert;
 //RCHINSTANCE hinstance;
 ResourceString rs1(4073);

  *cstate = ExitState;

  qDebug() << "dll path :" << QString(home.getexecutablefilename(_RCT("libRealCADOpenDWG.1.0.0.dylib")));
#ifdef _MAC
  QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("libRealCADOpenDWG.1.0.0.dylib"))),app);
#else
  QLibrary *hinstance = new QLibrary(QString(home.getexecutablefilename(_RCT("RealCADOpenDWG.dll"))),app);
#endif

  convert = 0;

  if (hinstance != 0 && hinstance->load())
    convert = (Convert) hinstance->resolve("Convert");
    //convert = (Convert) GetProcAddress(hinstance,"Convert");

  /*
  if (convert == 0)
    {  strcpy(pfilename,home.getexecutablefilename(_RCT("dconvert.exe")));
       strcpy(parameter,"DWG");
       if (stat(pfilename,&sbuf) != 0)
         {  strcpy(pfilename,home.getexecutablefilename(_RCT("dconvertcon.exe")));
            strcpy(parameter,"-DWG");
            if (stat(pfilename,&sbuf) != 0)
              {  cadwindow->MessageBox("DWG batch convert requires dconvert (DWG Convertor) utility.\nIt can be downloaded from the OpenDWG web site for free.\nhttp://www.opendwg.org/downloads/guest.htm\nSave the converter into the same directory as this application exe.","Import DWG",MB_OK);
                 return;
              }
         }
    }
  */

  if (cadwindow->getcurrentwindow() == 0)
    cadwindow->MessageBox("Please select a window","Export DXF",MB_ICONINFORMATION);
  else
    {
      _getcwd(dirname, sizeof(dirname));
       strcpy(filename,"");
       strcpy(filter,"AutoCAD DWG Files (*.DWG *.dwg)");
       //for (i = 0; filter[i] != '\0'; i++)
       //  if (filter[i] == '|')
       //    filter[i] = '\0';

       RCSTRING qTitle(db.getname());
       qTitle = qTitle.left(qTitle.lastIndexOf('.')) + ".dwg";
       strcpy(filename,qTitle.data());
       qTitle = RCSTRING(db.gettitle());
       qTitle = qTitle.left(qTitle.lastIndexOf('.')) + ".dwg";
       strcpy(szFileTitle,qTitle.data());

       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = filter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile = filename;
       ofn.nMaxFile = sizeof(filename);
       ofn.lpstrFileTitle = szFileTitle;
       ofn.nMaxFileTitle = sizeof(szFileTitle);
       ofn.lpstrInitialDir = dirname;
       ofn.lpstrDefExt = _RCT("dwg");
       ofn.lpstrFileTitle = title;
       ofn.lpstrTitle =  rs1.getws();
       ofn.Flags = 0;//OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

       if (GetSaveFileName(&ofn))
         {
            DXFOut dxffile;
            _strlwr(filename);
            if (strstr(filename,".dwg") != 0)
              {
                 strstr(filename,".dwg")[0] = 0;
                 strcat(filename,".dxf");

                 if (dxffile.setscale() && dxffile.open(filename))
                   {  program->setbusy(1);

                      dxffile.write(0);
                      dxffile.close();


                      strstr(filename,".dxf")[0] = 0;

                      strcpy(dwgfilename,filename);  strcat(dwgfilename,".dwg");
                      strcpy(dxffilename,filename);  strcat(dxffilename,".dxf");

                      if (convert != 0)
                        convert(0,dxffilename,dwgfilename);
                      /*
                      else
                        {  sprintf(command,"%s \"%s.DXF\" %s",pfilename,filename,parameter);

                           memset(&si,0,sizeof(si));
                           si.cb = sizeof(si);
                           si.dwFlags = STARTF_USESHOWWINDOW;
                           si.wShowWindow = SW_HIDE;
                           CreateProcess(0,command,0,0,FALSE,CREATE_NEW_PROCESS_GROUP,0,0,&si,&pi);
                                       WaitForSingleObject(pi.hProcess,100000L);
                         }
                      */

                      strcat(filename,".dxf");
                      _unlink(filename);

                      program->setbusy(0);

                   }
              }
         }
    }

  FreeLibrary(hinstance);
  *cstate = ExitState;
}
#else
// this is RealCAD version 7 code let's try it later
void dwgout_command(int *cstate,HighLevelEvent *,void **)
{OPENFILENAME ofn;
 RC_STAT sbuf;
 STARTUPINFO si;
 PROCESS_INFORMATION pi;
 RCCHAR filename[300],dirname[300],filter[300],pfilename[300],command[300],parameter[300],dwgfilename[300],dxffilename[300];
 int i;
 Convert convert;
 RCHINSTANCE hinstance;
   
  *cstate = ExitState;

  hinstance = LoadLibrary(home.getexecutablefilename(_RCT("RealCADOpenDWG.dll")));
  convert = 0;

  if (hinstance != 0)
    convert = (Convert) GetProcAddress(hinstance,"Convert");

  if (convert == 0)
    {  strcpy(pfilename,home.getexecutablefilename(_RCT("dconvert.exe")));
       strcpy(parameter,"DWG");
       if (stat(pfilename,&sbuf) != 0)
         {  strcpy(pfilename,home.getexecutablefilename(_RCT("dconvertcon.exe")));
            strcpy(parameter,"-DWG");
            if (stat(pfilename,&sbuf) != 0)
              {  cadwindow->MessageBox("DWG batch convert requires dconvert (DWG Convertor) utility.\nIt can be downloaded from the OpenDWG web site for free.\nhttp://www.opendwg.org/downloads/guest.htm\nSave the converter into the same directory as this application exe.","Import DWG",MB_OK);
                 return;
              }
         }
    }

  if (cadwindow->getcurrentwindow() == 0)
    cadwindow->MessageBox("Please select a window","Export DXF",MB_ICONINFORMATION);
  else
    {  _getcwd(dirname, sizeof(dirname));
       strcpy(filename,"");
       strcpy(filter,"DWG Files *.DWG|*.DWG|");
       for (i = 0; filter[i] != '\0'; i++)
         if (filter[i] == '|') 
           filter[i] = '\0';
       memset(&ofn, 0, sizeof(OPENFILENAME));
       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = filter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile = filename;
       ofn.nMaxFile = sizeof(filename);
       ofn.lpstrInitialDir = dirname;
       ofn.lpstrDefExt = _RCT("*.dwg");
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
       if (GetSaveFileName(&ofn))
         {DXFOut dxffile;
            _strlwr(filename);
            if (strstr(filename,".dwg") != 0)
              {  strstr(filename,".dwg")[0] = 0;
                 strcat(filename,".dxf");

                 if (dxffile.setscale() && dxffile.open(filename))
                   {EntityList *list;
                      if (v.getinteger("dx::selected_entities") )
                      { list = new EntityList();
                        *list = state.getselection();
                        if (list->length() == 0)
                        {   cadwindow->MessageBox("The entities to export must be selected first","Export DXF",MB_OK);
                            return;
                        }
                      }
                      else
                        list = 0;
                      program->setbusy(1);
                      dxffile.write(list);
                      dxffile.close();
                      delete list;

                      strstr(filename,".dxf")[0] = 0;

                      strcpy(dwgfilename,filename);  strcat(dwgfilename,".dwg");
                      strcpy(dxffilename,filename);  strcat(dxffilename,".dxf");

                      if (convert != 0)
                        convert(0,dxffilename,dwgfilename);
                      else
                        {  sprintf(command,"%s \"%s.DXF\" %s",pfilename,filename,parameter);

                           memset(&si,0,sizeof(si));
                           si.cb = sizeof(si);
                           si.dwFlags = STARTF_USESHOWWINDOW;
                           si.wShowWindow = SW_HIDE;
                           CreateProcess(0,command,0,0,FALSE,CREATE_NEW_PROCESS_GROUP,0,0,&si,&pi);
                                       WaitForSingleObject(pi.hProcess,100000L);
                         }

                      strcat(filename,".dxf");
                      _unlink(filename);

                      program->setbusy(0);
                    
                   }
              }
         }
    }
  *cstate = ExitState;
}
#endif
#endif
#endif
