
#include "ncwin.h"

OneReal::OneReal(char *n,double v)
{  name = new RCCHAR[strlen(n)+1];
   if (name != NULL) strcpy(name,n);
   value = v;
}

OneReal::OneReal(RCCHAR *n,double v)
{  name = new RCCHAR[strlen(n)+1];
   if (name != NULL) strcpy(name,n);
   value = v;
}

OneReal::~OneReal()
{  delete [] name;
}

int OneReal::save(CadFile *outfile)
{ if (! outfile->write(name)) return 0;
  if (! outfile->write(&value)) return 0;
  return 1;
}

OneReal *OneReal::load(CadFile *infile)
{
   char *cname;
   RCCHAR *name;
   double value;
   OneReal *onr;
   if( infile->getversion() == 2 )
   {
       if (! infile->read(&cname)) return NULL;
       name = new RCCHAR[strlen(cname)+1];
       strcpy(name,cname);
       delete [] cname;
   }
   else
   {
       if (! infile->read(&name))
           return NULL;
   }

   if (! infile->read(&value)) return NULL;
   onr = new OneReal(name,value);
   delete [] name;
   return onr;
}

void OneReal::save(void)
{
  //RCCHAR svalue[256];
  //sprintf(svalue,"%22.16g",value);
  //char buf[256];
  //_snprintf(buf, 256, "%22.16g", value);
  //QString qvalue = QString::fromUtf8(buf);
  //QString qvalue = QString("%1").arg(value,22,'g',16);
  QString qvalue = QString("%1").arg(value,0,'g',16);
#if ! defined(_WIN32_WCE)
  //WritePrivateProfileString(_RCT("Reals"),name,svalue,home.getinifilename());
  WritePrivateProfileString(_RCT("Reals"),name,qvalue.data(),home.getinifilename());
#endif
}

void OneReal::load(void)
{
 RCCHAR svalue[256];
#if ! defined(_WIN32_WCE)
  if( strlen(home.getinifilename()) < 1) return;
  GetPrivateProfileString(_RCT("Reals"),name,_RCT(""),svalue,256,home.getinifilename());
  value=0.0;
  if (strlen(svalue) != 0)
      value = QString(svalue).toDouble();
    //sscanf(svalue,_RCT("%lf"),&value);
#endif
}

OneString::OneString(RCCHAR *n,RCCHAR *v)
{
   name = new RCCHAR[strlen(n)+1];
   if (name != 0) strcpy(name,n);
   value = new RCCHAR[strlen(v)+1];
   if (value != 0)
     strcpy(value,v);
   else
     {  delete [] name;  name = value = 0;
     }
   //qDebug() << QString(name);
   //qDebug() << QString(value);
}

OneRCString::OneRCString(RCCHAR *n,RCCHAR *v) //: OneString(n,"")
{
    //cvalue = NULL;
    name = new RCCHAR[strlen(n)+1];
   if (name != NULL) strcpy(name,n);
   value = new RCCHAR[strlen(v)+1];
   if (value != NULL)
     RCSTRCPY(value,v);
   else
   {
       delete [] name;  name = 0 ; value = 0;
   }
}

OneString::~OneString()
{  delete [] name; name = 0;
   delete [] value; value = 0;
}

OneRCString::~OneRCString()
{  delete [] name; name = 0;
   delete [] value; value = 0;
   //delete [] cvalue;
}

int OneString::save(CadFile *outfile)
{
#if 0
    if( strcmp(name,"tx::font") != 0 &&
        strcmp(name,"tx::text") != 0 &&
        strcmp(name,"tx::counterprefix") != 0 &&
        strcmp(name,"tx::countersuffix") != 0 &&
        strcmp(name,"tx::leadertext") != 0 &&

        strcmp(name,"di::vtextfont") != 0 &&
        strcmp(name,"di::vtexttext") != 0 &&
        strcmp(name,"di::ttextfont") != 0 &&
        strcmp(name,"di::ttexttext") != 0 &&
        strcmp(name,"di::ptextfont") != 0 &&
        strcmp(name,"di::ptexttext") != 0 &&
        strcmp(name,"di::stextfont") != 0 &&
        strcmp(name,"di::stexttext") != 0 &&

        strcmp(name,"le::vtextfont") != 0 &&
        strcmp(name,"le::vtexttext") != 0 &&
        strcmp(name,"le::ttextfont") != 0 &&
        strcmp(name,"le::ttexttext") != 0 &&
        strcmp(name,"le::ptextfont") != 0 &&
        strcmp(name,"le::ptexttext") != 0 &&
        strcmp(name,"le::stextfont") != 0 &&
        strcmp(name,"le::stexttext") != 0
        )
    {
        if (! outfile->write(name)) return 0;
        if (! outfile->write(value)) return 0;
    }
#endif
    if (! outfile->write(name)) return 0;
    if (! outfile->write(value)) return 0;
  return 1;
}

int OneRCString::save(CadFile *outfile)
{ if (! outfile->write(name)) return 0;
  if (! outfile->write(value)) return 0;
  return 1;
}

OneString *OneString::load(CadFile *infile)
{
    char *cname=0,*cvalue=0;
    RCCHAR *name=0,*value=0;
    OneString *os=0;

    if( infile->getversion() == 2)
    {
        if (! infile->read(&cname)) return NULL;
        name = new RCCHAR[strlen(cname)+1];
        strcpy(name,cname);
        delete [] cname;
        if (! infile->read(&cvalue)) return NULL;
        value = new RCCHAR[strlen(cvalue)+1];
        strcpy(value,cvalue);
        delete [] cvalue;
    }
    else
    {
        if (! infile->read(&name))
            return NULL;
        if (! infile->read(&value))
            return NULL;
    }

    os = new OneString(name,value);
    delete [] name;
    delete [] value;
    return os;
}

OneRCString *OneRCString::load(CadFile *infile)
{
    char *cname=0;
    RCCHAR *name=0;
    char *cvalue=0;
    RCCHAR *value=0;
    OneRCString *os=0;
    // for older version files
    //if( infile->getversion() < 3 )
    if( infile->getversion() == 2 )
    {
        // name and value are char
        if (! infile->read(&cname)) return NULL;
        if (! infile->read(&cvalue)) return NULL;
        os = new OneRCString(QString(cname).data(),QString(cvalue).data());
        delete [] cvalue;
        delete [] cname;
    }
    else if( infile->getversion() < 0 ) // temp file versions
    {
        // name is char
        if (! infile->read(&cname)) return NULL;
        // for RCCHAR (Qchar unicode) strings in a new version file
        if( strcmp(cname,"tx::font") == 0 ||
            strcmp(cname,"tx::text") == 0 ||
            strcmp(cname,"tx::counterprefix") == 0 ||
            strcmp(cname,"tx::countersuffix") == 0 ||
            strcmp(cname,"tx::leadertext") == 0 ||

            strcmp(cname,"di::vtextfont") == 0 ||
            strcmp(cname,"di::vtexttext") == 0 ||
            strcmp(cname,"di::ttextfont") == 0 ||
            strcmp(cname,"di::ttexttext") == 0 ||
            strcmp(cname,"di::ptextfont") == 0 ||
            strcmp(cname,"di::ptexttext") == 0 ||
            strcmp(cname,"di::stextfont") == 0 ||
            strcmp(cname,"di::stexttext") == 0 ||

            strcmp(cname,"le::vtextfont") == 0 ||
            strcmp(cname,"le::vtexttext") == 0 ||
            strcmp(cname,"le::ttextfont") == 0 ||
            strcmp(cname,"le::ttexttext") == 0 ||
            strcmp(cname,"le::ptextfont") == 0 ||
            strcmp(cname,"le::ptexttext") == 0 ||
            strcmp(cname,"le::stextfont") == 0 ||
            strcmp(cname,"le::stexttext") == 0
            )
        {
            // only the value is an RC string
            if (! infile->read(&value)) return NULL;
            os = new OneRCString(QString(cname).data(),value);
            delete [] value;
            delete [] cname;
        }
        else
        {
            // only the value is an RC string
            if (! infile->read(&value)) return NULL;
            os = new OneRCString(QString(cname).data(),value);
            delete [] value;
            delete [] cname;
        }
    }
    else // file version > 2
    {
        // name and value are RC strings
        if (! infile->read(&name)) return NULL;
        if (! infile->read(&value)) return NULL;
        delete [] name;
        delete [] value;
    }
    return os;
}

void OneString::save(void)
{
#if ! defined(_WIN32_WCE)
  WritePrivateProfileString(_RCT("Strings"),name,value,home.getinifilename());
#endif
}

void OneRCString::save(void)
{
#if ! defined(_WIN32_WCE)
  WritePrivateProfileString(_RCT("Strings"),name,value,home.getinifilename());
#endif
}

void OneString::load(void)
{RCCHAR svalue[4096],*oldstring=0;
#if ! defined(_WIN32_WCE)
  if(strlen(home.getinifilename()) < 1) return;
  GetPrivateProfileString(_RCT("Strings"),name,_RCT("~|?No$#Value?|~"),svalue,4096,home.getinifilename());
  if (strcmp(svalue,_RCT("~|?No$#Value?|~")) != 0)
    {  oldstring = value;
       if ((value = new RCCHAR[strlen(svalue)+1]) != 0)
         {  strcpy(value,svalue);
            delete [] oldstring;
         }
       else
         value = oldstring;
    }
#endif
}

void OneRCString::load(void)
{
    RCCHAR svalue[4096];//,*oldstring;
    RCCHAR *oldstring=0;
#if ! defined(_WIN32_WCE)
  if(strlen(home.getinifilename()) < 1) return;
  GetPrivateProfileString(_RCT("Strings"),name,_RCT("~|?No$#Value?|~"),svalue,4096,home.getinifilename());
  if (strcmp(svalue,_RCT("~|?No$#Value?|~")) != 0)
    {  oldstring = value;
        int len = strlen(svalue)+1;
       if ((value = new RCCHAR[strlen(svalue)+1]) != 0)
         {
            strcpy(value,svalue);
            //printQChar(value);
            delete [] oldstring;
         }
       else
         value = oldstring;
    }
#endif
}

OneBitMask::OneBitMask(char *n,const BitMask &bitmask)
{  name = new RCCHAR[strlen(n)+1];
   if (name != NULL) strcpy(name,n);
   value = new BitMask(bitmask);
   if (value == NULL)
     {  delete [] name;  name = NULL;  value = NULL;
     }
}

OneBitMask::OneBitMask(RCCHAR *n,const BitMask &bitmask)
{  name = new RCCHAR[strlen(n)+1];
   if (name != NULL) strcpy(name,n);
   value = new BitMask(bitmask);
   if (value == NULL)
     {  delete [] name;  name = NULL;  value = NULL;
     }
}

OneBitMask::~OneBitMask()
{  delete [] name;
   delete value;
}

int OneBitMask::save(CadFile *outfile)
{ if (! outfile->write(name)) return 0;
  if (! outfile->write(value)) return 0;
  return 1;
}

OneBitMask *OneBitMask::load(CadFile *infile)
{
 char *cname=0;
 RCCHAR *name=0;
 BitMask value(32);
 OneBitMask *obm=0;

 if( infile->getversion() == 2 )
 {
     if (! infile->read(&cname)) return NULL;
     name = new RCCHAR[strlen(cname)+1];
     strcpy(name,cname);
     delete [] cname;
 }
 else
 {
     if (! infile->read(&name))
         return NULL;
 }
   if (! infile->read(&value)) return NULL;
   obm = new OneBitMask(name,value);
   delete [] name;
   return obm;
}

void OneBitMask::save(void)
{char svalue[1024];
  value->save(svalue);
#if ! defined(_WIN32_WCE)
  WritePrivateProfileString(_RCT("BitMasks"),name,svalue,home.getinifilename());
#endif
}

void OneBitMask::load(void)
{char svalue[1024];
#if ! defined(_WIN32_WCE)
    if(strlen(home.getinifilename()) < 1) return;
    GetPrivateProfileString(_RCT("BitMasks"),name,_RCT(""),svalue,1024,home.getinifilename());
  if (strlen(svalue) != 0)
    value->load(svalue);
#endif
}

OneInteger::OneInteger(char *n,int v)
{
    name = new RCCHAR[strlen(n)+1];
    if (name != NULL) strcpy(name,n);
    value = v;
}

OneInteger::OneInteger(RCCHAR *n,int v)
{
   name = new RCCHAR[strlen(n)+1];
   if (name != NULL) strcpy(name,n);
   value = v;
}

OneInteger::~OneInteger()
{  delete [] name;
}

int OneInteger::save(CadFile *outfile)
{long v;
  if (! outfile->write(name)) return 0;
  v = value;
  if (! outfile->write(&v)) return 0;
  return 1;
}

OneInteger *OneInteger::load(CadFile *infile)
{
  char *cname;
  RCCHAR *name;
  long value;
  OneInteger *oi;

  if( infile->getversion() >= 3 )
  {
      if (! infile->read(&name))
          return NULL;
  }
  else
  {
      if (! infile->read(&cname)) return NULL;
      name = new RCCHAR[strlen(cname)+1];
      strcpy(name,cname);
      delete cname;
  }
  if (! infile->read(&value)) return NULL;
  oi = new OneInteger(name,int(value));
  delete [] name;
  return oi;
}

void OneInteger::save(void)
{RCCHAR svalue[256];
#if ! defined(_WIN32_WCE)
  sprintf(svalue,"%d",value);
  WritePrivateProfileString(_RCT("Integers"),name,svalue,home.getinifilename());
#endif
}

void OneInteger::load(void)
{
  RCCHAR svalue[256];
  if(strlen(home.getinifilename()) < 1)
      return;
  GetPrivateProfileString(_RCT("Integers"),name,_RCT(""),svalue,256,home.getinifilename());
  value=0;
  if (strlen(svalue) != 0)
    sscanf(svalue,_RCT("%d"),&value);
}

OneLong::OneLong(RCCHAR *n,long v)
{ name = new RCCHAR[strlen(n)+1];
  if (name != NULL) strcpy(name,n);
  value = v;
}

OneLong::~OneLong()
{ delete [] name;
}

int OneLong::save(CadFile *outfile)
{ if (! outfile->write(name)) return 0;
  if (! outfile->write(&value)) return 0;
  return 1;
}

OneLong *OneLong::load(CadFile *infile)
{
 char *cname;
 RCCHAR *name;
 long value;
 OneLong *ol;

  if( infile->getversion() >= 3 )
  {
     if (! infile->read(&name))
         return NULL;
  }
  else

  {
     if (! infile->read(&cname)) return NULL;
     name = new RCCHAR[strlen(cname)+1];
     strcpy(name,cname);
     delete [] cname;
  }
  if (! infile->read(&value)) return NULL;
  ol = new OneLong(name,value);
  delete [] name;
  return ol;
}

void OneLong::save(void)
{RCCHAR svalue[256];
#if ! defined(_WIN32_WCE)
  sprintf(svalue,"%ld",value);
  WritePrivateProfileString(_RCT("Longs"),name,svalue,home.getinifilename());
#endif
}

void OneLong::load(void)
{RCCHAR svalue[256];
#if ! defined(_WIN32_WCE)
  if(strlen(home.getinifilename()) < 1) return;
  GetPrivateProfileString(_RCT("Longs"),name,_RCT(""),svalue,256,home.getinifilename());
  if (strlen(svalue) != 0)
    sscanf(svalue,_RCT("%ld"),&value);
#endif
}

Values::Values(int empty)
{
    if (! empty)
        initialize();
}

void Values::debug(int flag)
{
    OneReal *onr;
    OneInteger *oi;
    OneString *os;
    OneRCString *RCos;
    OneLong *ol;
    OneBitMask *ob;
    int i;

    // dump reals
    qDebug() << "Values : reals length = " << reals.length();
    i=0;
    if(flag==1)
    {
        for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
        {
            qDebug() << "Values : real (" << i << ") name  : " << QString(onr->name);
            qDebug() << "Values : real (" << i << ") value : " << onr->value;
            i++;
        }
    }
    // dump integers
    qDebug() << "Values : integers length = " << integers.length();
    i=0;
    if(flag==1)
    {
        for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
        {
            qDebug() << "Values : integer (" << i << ") name  : " << QString(oi->name);
            qDebug() << "Values : integer (" << i << ") value : " << oi->value;
            i++;
        }
    }
    // dump strings
    qDebug() << "Values : strings length = " << strings.length();
    i=0;
    if(flag==1)
    {
        for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
        {
            qDebug() << "Values : string (" << i << ") name  : " << QString(os->name);
            qDebug() << "Values : string (" << i << ") value : " << QString(os->value);
            i++;
        }
    }
    // dump RCstrings
    qDebug() << "Values : RCstrings length = " << RCstrings.length();
    i=0;
    if(flag==1)
    {
        for (RCstrings.start() ;  (RCos = (OneRCString *) RCstrings.next()) != NULL ; )
        {
            qDebug() << "Values : RCstring (" << i << ") name  : " << QString(RCos->name);
            qDebug() << "Values : RCstring (" << i << ") value : " << QString(RCos->value);
            i++;
        }
    }
    // dump longs
    qDebug() << "Values : longs length = " << longs.length();
    i=0;
    if(flag==1)
    {
        for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
        {
            qDebug() << "Values : long (" << i << ") name  : " << QString(ol->name);
            qDebug() << "Values : long (" << i << ") value : " << ol->value;
            i++;
        }
    }
    // bump bitmasks
    qDebug() << "Values : bitmasks length = " << bitmasks.length();
    i=0;
    if(flag==1)
    {
        for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
        {
            qDebug() << "Values : bitmask (" << i << ") name  : " << QString(ob->name);
            qDebug() << "Values : bitmask (" << i << ") value : " << ob->value;
            i++;
        }
    }
}

void Values::initialize()
{BitMask options(32);
#if (VERSION == TRICAD_STUDENT) || (VERSION == FLEXICAD)
 RCCHAR key[256],value[10000],*s;
#else
    RCCHAR key[256],value[50000]={0},*s;
    RCCHAR temp[600]={0};
#endif
 OneReal *onr;
 OneInteger *oi;
 OneString *os;
 OneRCString *RCos;
 OneLong *ol;
 OneBitMask *ob;
 int i;

 // KMJ new for v7~
 // default file paths
 addstring("db::figures-components-path",home.getpublicfilename("Components"));
 addstring("db::title-sheets-path",home.getpublicfilename("Title-Sheets"));
 addstring("db::template-drawings-path",home.getpublicfilename("Template-Drawings"));
 addstring("db::picture-images-path",home.getpublicfilename("Picture-Images"));
 addstring("db::hatch-patterns-path",home.getpublicfilename("Hatch-Patterns"));
 addstring("db::textures-path",home.getpublicfilename("Textures"));
 addstring("db::line-styles-path",home.getpublicfilename("Line-Styles"));
 addstring("db::my-drawings-path",home.getpublicfilename("CAD-Drawings"));
 addstring("db::dwg-preprocessor-path",home.getpublicfilename(""));
 // KMJ new for v8
 //strcpy(temp,QDir::homePath());
 strcpy(temp,"");
 addstring("db::last-cad-file-path",temp);
 addstring("db::last-dxf-file-path",temp);
 addstring("db::last-dwg-file-path",temp);
 // debug
 //qDebug() << "db::figures-components-path" << QString(getstring("db::figures-components-path"));
 //qDebug() << "db::title-sheets-path" << QString(getstring("db::title-sheets-path"));
 //qDebug() << "db::template-drawings-path" << QString(getstring("db::template-drawings-path"));
 //qDebug() << "db::picture-images-path" << QString(getstring("db::picture-images-path"));
 //qDebug() << "db::hatch-patterns-path" << QString(getstring("db::hatch-patterns-path"));
 //qDebug() << "db::textures-path" << QString(getstring("db::textures-path"));
 //qDebug() << "db::line-styles-path" << QString(getstring("db::line-styles-path"));
 //qDebug() << "db::my-drawings-path" << QString(getstring("db::my-drawings-path"));
 // end new

  addstring("pl::hatchpatterndirectory1","");
  addstring("pl::hatchpatterndirectory2","");
  addstring("pl::hatchpatterndirectory3","");
  addstring("pl::hatchpatterndirectory4","");
  addstring("pl::hatchpatterndirectory5","");
  addstring("pl::hatchpatterndirectory6","");
  addstring("pl::hatchpatterndirectory7","");
  addstring("pl::hatchpatterndirectory8","");
  addstring("pl::hatchpatterndirectory9","");
  addstring("pl::hatchpatterndirectory10","");

  // KMJ new for V7~
  addstring("pl::bitmapfilldirectory1","");
  addstring("pl::bitmapfilldirectory2","");
  addstring("pl::bitmapfilldirectory3","");
  addstring("pl::bitmapfilldirectory4","");
  addstring("pl::bitmapfilldirectory5","");
  addstring("pl::bitmapfilldirectory6","");
  addstring("pl::bitmapfilldirectory7","");
  addstring("pl::bitmapfilldirectory8","");
  addstring("pl::bitmapfilldirectory9","");
  addstring("pl::bitmapfilldirectory10","");
  // KMJ new for v8
  addinteger("pl::showpreview",1);// 0=dont show; 1=show
  addinteger("pl::spacinglinked",0);// 0=not linked; 1=linked
  addinteger("bi::sizelinked",0);// 0=not linked; 1=linked

  // KMJ new for dwg import options
  addinteger("dg::in-use_basic_options",0);
  addinteger("dg::in-use_dwg_layers",0);
  addinteger("dg::in-use_advanced_options",0);
  addinteger("dg::in-purge_dwg",0);
  addinteger("dg::in-visible_layers_only",0);
  addinteger("dg::in-model_space_only",0);
  addinteger("dg::in-explode_blocks",0);
  addinteger("dg::in-xrefs_to_blocks",0);
  addinteger("dg::in-polylines_to_lines",0);
  addinteger("dg::in-proxy_to_blocks",0);
  addinteger("dg::in-explode_hatch_patterns",0);

  addinteger("oc::contour_cut_first",1);
  addinteger("oc::contour_enabled",1);
  addreal("oc::contour_tool_diameter",10.0);
  addreal("oc::contour_offset",5.0);
  addreal("oc::contour_feed_rate",1000.0);
  addreal("oc::contour_spindle_speed",20000.0);
  addinteger("oc::side_enabled",1);
  addreal("oc::side_tool_diameter",10.0);
  addreal("oc::side_feed_rate",1000.0);
  addreal("oc::side_spindle_speed",20000.0);
  addreal("oc::thickness",6.0);

  addstring("nc::postname","");
  addstring("nc::filename","");
  addstring("nc::avifilename","");

  addreal("nc::stepsize",1.0);
  addinteger("nc::linetrails",0);
  addinteger("nc::tooltrails",0);
  addinteger("nc::generatemodel",0);
  addreal("nc::displayevery",40.0);

  addinteger("nc::maxsegments",200);
  addinteger("nc::xmesh",10);
  addinteger("nc::ymesh",10);

  addinteger("nc::toolpathtype",0);
  addinteger("nc::order",0);
  addinteger("nc::approach",0);
  addreal("nc::feed",200.0);
  addreal("nc::spindle",2000.0);
  addreal("nc::retract",10.0);
  addreal("nc::holedepth",-30.0);
  addreal("nc::peck",20.0);
  addinteger("nc::compensation",0);
  addinteger("nc::retractm",0);
  addreal("nc::plungefeed",200.0);
  addreal("nc::retractfeed",600.0);
  addreal("nc::initialz",-10.0);
  addinteger("nc::useentityz",1);
  addreal("nc::finalz",-20.0);
  addreal("nc::depthcut",10.0);
  addreal("nc::surfaceoffset",5.0);
  addinteger("nc::path",1);
  addreal("nc::stepover",10.0);
  addreal("nc::angle",45.0);

  addinteger("nc::deleteentities",1);
  addinteger("nc::reorder",0);
  addreal("nc::millwidth",100.0);
  addreal("nc::millheight",40.0);
  addreal("nc::milllength",120.0);
  addreal("nc::millhomex",140.0);
  addreal("nc::millhomey",120.0);
  addreal("nc::millhomez",100.0);
  addreal("nc::milloriginx",0.0);
  addreal("nc::milloriginy",0.0);
  addreal("nc::milloriginz",-40.0);
  addreal("nc::millpartx",0.0);
  addreal("nc::millparty",0.0);
  addreal("nc::millpartz",0.0);

  for (i = 0 ; i < 20 ; i++)
    {  sprintf(key,"mn::CustomMenu::Id%d",i);
       addinteger(key,-1);
       sprintf(key,"mn::CustomMenu::Name%d",i);
       addstring(key,_RCT(""));
    }

  addinteger("cm::active",0);
  addinteger("cm::orientation",0);

  addinteger("rf::cs",0);


  //  Delete any existing information entries
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    if (strncmp(os->name,_RCT("in::"),4) == 0)
      {  strings.del(os);
         strings.start();
      }

  addstring("in::Author","");
  addstring("in::Date","");
  addstring("in::Description","");
  addstring("in::Cost","");
  addstring("in::Category","");
  addstring("in::SubCategory","");
  addstring("in::Supplier","");
  addstring("in::Address","");

  addstring("fr::find","");
  addstring("fr::replace","");
  addbm("fr::options",options);

  addstring("ff::find","");
  addstring("ff::directory","");
  options.clearall();
  options.set(1);
  addbm("ff::options",options);
  options.clearall();

  addinteger("wn::showstartupdialog",1);
  addinteger("wn::dialogboxautook",1);

  addreal("wn::backgroundcolour.red",1.0);
  addreal("wn::backgroundcolour.green",1.0);
  addreal("wn::backgroundcolour.blue",1.0);

  // KMJ new for testing
  addinteger("wn::screenrefreshdelay",0);
  addinteger("wn::reducedrawdetail",1); // 0=off; 1=on (future use >1 = LOD value)

  addinteger("wn::scrollbars",0);

  // KMJ new for v8 button menu sizes
  addinteger("wn::menusize",0);

  // KMJ new for progress bars
  addinteger("prg::doprogressuptick",1);

  addinteger("wp::display",1);
  addinteger("wp::zdisplay",0);

  addstring("bm::ext","bmp");
  addstring("an::scene","scene-01");
  addstring("an::copyavifile","");
  addinteger("an::animatelayers",0);
  addinteger("an::startlayer",1000);
  addinteger("an::nframestocopy",0);
  addinteger("an::quality",8);
  addinteger("an::initialframe",1);
  addinteger("an::framessec",25);
  addinteger("an::format",0);
  addstring("an::prefix","FRAME");
  addinteger("an::wireframe",1);
  addreal("an::starttime",0.0);
  addreal("an::finishtime",5.0);
  addinteger("an::hide",0);
  addinteger("an::shadebefore",0);
  addinteger("an::shadeafter",0);
  addinteger("an::shading",0);
  addinteger("an::fixref",0);
  addinteger("an::followpath",0);
  addreal("an::startfocaldistance",100.0);
  addreal("an::finishfocaldistance",100.0);
  addinteger("an::reverse",1);
  addreal("an::loops",1.0);
  addreal("an::scalex1",1.0);
  addreal("an::scaley1",1.0);
  addreal("an::scalez1",1.0);
  addreal("an::startheight",0.0);
  addreal("an::rotationx1",0.0);
  addreal("an::rotationy1",0.0);
  addreal("an::rotationz1",0.0);
  addreal("an::scalex2",1.0);
  addreal("an::scaley2",1.0);
  addreal("an::scalez2",1.0);
  addreal("an::finishheight",0.0);
  addreal("an::rotationx2",0.0);
  addreal("an::rotationy2",0.0);
  addreal("an::rotationz2",0.0);

  addinteger("se::type",0);
  addinteger("se::insertonintersection",1);
  addinteger("se::createsectionview",1);
  addstring("se::name","SectionView");

  addinteger("sm::framessec",25);
  addinteger("sm::format",0);
  addstring("sm::prefix","FRAME");
  addinteger("sm::wireframe",1);
  addreal("sm::starttime",0.0);
  addreal("sm::finishtime",5.0);
  addinteger("sm::hide",0);
  addinteger("sm::shadebefore",0);
  addinteger("sm::shadeafter",0);
  addinteger("sm::shading",0);
  addinteger("sm::fixref",0);
  addinteger("sm::followpath",0);
  addreal("sm::startfocaldistance",100.0);
  addreal("sm::finishfocaldistance",100.0);
  addinteger("sm::reverse",1);
  addreal("sm::loops",1.0);
  addreal("sm::scalex1",1.0);
  addreal("sm::scaley1",1.0);
  addreal("sm::scalez1",1.0);
  addreal("sm::startheight",0.0);
  addreal("sm::rotationx1",0.0);
  addreal("sm::rotationy1",0.0);
  addreal("sm::rotationz1",0.0);
  addreal("sm::scalex2",1.0);
  addreal("sm::scaley2",1.0);
  addreal("sm::scalez2",1.0);
  addreal("sm::finishheight",0.0);
  addreal("sm::rotationx2",0.0);
  addreal("sm::rotationy2",0.0);
  addreal("sm::rotationz2",0.0);

  addreal("sm::size",10.0);
  addreal("sm::rate",10.0);
  addreal("sm::ninputsrequired",5.0);
  addreal("sm::length",10.0);
  addreal("sm::width",10.0);
  addreal("sm::height",10.0);
  addreal("sm::xscale",1.0);
  addreal("sm::yscale",1.0);
  addreal("sm::zscale",1.0);
  addreal("sm::speed",10.0);
  addstring("sm::trackname","");
  addstring("sm::name","");
  addstring("sm::producername","");
  addstring("sm::figure","");
  addstring("sm::product","");
  addstring("sm::actions","");
  addstring("sm::destination","");
  addstring("sm::initialise","");
  addstring("sm::moveto","");
  addstring("sm::updateposition","");

  addreal("sm::chartwidth",100.0);
  addreal("sm::chartheight",70.0);
  addreal("sm::xminimum",0.0);
  addreal("sm::yminimum",0.0);
  addreal("sm::xmaximum",20.0);
  addreal("sm::ymaximum",20.0);
  addreal("sm::xstep",5.0);
  addreal("sm::ystep",5.0);
  addreal("sm::xstepsize",5.0);
  addreal("sm::ystepsize",5.0);
  addreal("sm::textheight",2.0);
  addreal("sm::bufferwidth",100.0);
  addreal("sm::bufferheight",50.0);
  addstring("sm::label","");
  addstring("sm::xlabel","");
  addstring("sm::ylabel","");

  addbm("bi::options",options);
  addreal("bi::width",10.0);
  addreal("bi::height",10.0);
  addreal("bi::red",1.0);
  addreal("bi::green",1.0);
  addreal("bi::blue",1.0);
  addreal("bi::opacity",1.0);
  addreal("bi::transparencyrange",0.004);
  addinteger("bi::backstyle",0);

  addreal("ar::chamferlength1",1500.0);
  addreal("ar::chamferlength2",1500.0);
  addreal("ar::windowtopframingallowance",20.0);
  addreal("ar::windowbottomframingallowance",20.0);
  addreal("ar::windowsideframingallowance",20.0);
  addreal("ar::doortopframingallowance",5.0);
  addreal("ar::doorbottomframingallowance",5.0);
  addreal("ar::doorsideframingallowance",5.0);
  addstring("ar::windowlabel","w <width>");
  addstring("ar::doorlabel","d <width>");
  addstring("ar::baywindowlabel","bw <width>");
  addreal("ar::trussspacing",1200.0);
  addreal("ar::dutchgabledistancefromend",1200.0);
  addreal("ar::dutchgableridgelength",1200.0);
  addreal("ar::starttruncatedsetback",2000.0);
  addreal("ar::endtruncatedsetback",2000.0);
  addinteger("ar::startlayout",0);
  addinteger("ar::middlelayout",0);
  addinteger("ar::endlayout",0);
  addinteger("ar::stairlayout",0);
  addinteger("ar::roofstyle",0);
  addreal("ar::stairwidth",1000.0);
  addreal("ar::stairheight",2600.0);
  addreal("ar::stairrise",174.0);
  addreal("ar::stairrun",174.0*2.0);
  addreal("ar::stairdistance",100.0);
  addreal("ar::stairlanding",1000.0);
  addreal("ar::stairdiameter",300.0);
  addreal("ar::notationscale",200.0);
  addreal("ar::width1",15.0);
  addreal("ar::width2",80.0);
  addreal("ar::doorwidth",820.0);
  addreal("ar::doorheight",1800.0);
  addreal("ar::gablesetback",1500.0);
  addstring("ar::doorlibrary","");
  addstring("ar::windowlibrary","");
  addstring("ar::symbollibrary","");
  addinteger("ar::symbolaxisorder",0);
  addstring("ar::doorcategory","");
  addstring("ar::windowcategory","");
  addstring("ar::symbolcategory","");
  addinteger("ar::layer",0);
  addstring("ar::columnname","");
  addreal("ar::offset",100.0);
  addreal("ar::columnwidth",400.0);
  addreal("ar::columnheight",2400.00);
  addreal("ar::columnxspacing",5000.00);
  addreal("ar::columnyspacing",5000.00);
  addstring("ar::roofname","Tiled");
  addstring("ar::ceilingname","Gyprock");
  addstring("ar::floorname","Concrete");
  addstring("ar::wallname","Concrete");
  addstring("ar::sheet","01");
  addinteger("ar::centre",0);
  addinteger("ar::fromend",0);
  addreal("ar::enddistance",200.0);
  addreal("ar::viewheight",1800.0);
  addreal("ar::viewangle",60.0);
  addreal("ar::stairheight",2400.0);
  addreal("ar::stepheight",160.0);
  addinteger("ar::floor",0);
  addreal("ar::floorspacing",2400);
  addinteger("ar::active",0);
  addreal("ar::eavecutback",400.0);
  addreal("ar::roofpitch",25.0);
  addreal("ar::roofeaves",800.0);
  addreal("ar::roofeaveheight",2200.0);
  addreal("ar::wallheight",2400.0);
  addreal("ar::wallwidth",75.0);
  addreal("ar::wallpitch",22.5);
  addreal("ar::wallheight2",2400.0);
  addreal("ar::wallwidth2",70.0);
  addreal("ar::wallgap",55.0);
  addreal("ar::walleaves",800.0);
  addreal("ar::wallpeakdistance",50.0);
  addreal("ar::wallpeakheight",0.0);
  addreal("ar::wallcavity",0);
  addreal("ar::studspacing",600.0);
  addinteger("ar::wallrightcolour",256);
  addinteger("ar::wallmiddlecolour",256);
  addreal("ar::walllhatchspacing",0.0);
  addreal("ar::wallrhatchspacing",0.0);
  addreal("ar::walllhatchangle",0.0);
  addreal("ar::wallrhatchangle",0.0);
  addbm("ar::walloptions",options);
  addreal("ar::ceilingheight",2400.0);
  addreal("ar::floorheight",0.0);
  addreal("ar::windowheight",1000.0);
  addreal("ar::windowwidth",1000.0);
  addreal("ar::windowsillheight",1000.0);
  addreal("ar::plotscale",100.0);
  addreal("ar::xgrid",2000.0);
  addreal("ar::ygrid",2000.0);
  addreal("ar::xextension",200.0);
  addreal("ar::yextension",200.0);
  addinteger("ar::circled",1);
  addstring("ar::dimensionlabel","Bed");
  addreal("ar::floorxspacing",200.0);
  addreal("ar::flooryspacing",200.0);
  addreal("ar::floorangle",0.0);
  addreal("ar::floorcrossangle",90.0);
  addreal("ar::baywindowheadheight",1900.0);
  addreal("ar::baywindowfrontwindowwidth",2000.0);
  addreal("ar::baywindowheight",900.0);
  addreal("ar::baywindowsidewindowwidth",400.0);
  addreal("ar::baywindowdepth",400.0);

  addreal("ar::ceilingxspacing",1000.0);
  addreal("ar::ceilingyspacing",1000.0);
  addreal("ar::ceilingangle",0.0);
  addreal("ar::ceilingcrossangle",90.0);

  addreal("pp::printscale",1.0);
  addreal("pp::paperwidth",297.0);
  addreal("pp::paperheight",210.0);
  addreal("me::scale",1.0);
  addinteger("me::enablescale",0);

  addinteger("pl::npens",6);
  addinteger("pl::blackandwhite",0);
  addinteger("pl::reverseblackwhite",1);// 0=dont reverse;1=automatic;2=reverse

  addreal("zm::scale",1.0);
  addreal("vw::walkdistance",5.0);
  addreal("vw::aspectadjustment",1.0);
  addinteger("vw::bitmapsforcomposedviews",0);
  addreal("cd::nonstdwidth",1000.0);
  addreal("cd::nonstdheight",1000.0);

  addreal("vw::plotscale",1.0);
  addreal("cd::plotscale",1.0);
  addreal("cd::angle",0.0);
  addreal("vw::angle",10.0);
  addreal("vw::depth",0.0);
  addreal("tm::length",10.0);
  addinteger("tm::tolength",0);
  addinteger("tm::sections",5);
  addinteger("tr::remove",1);
  addinteger("tr::groupfigures",1);
  addinteger("tr::stretchmode",0);
  addreal("tr::sx",1.0);
  addreal("tr::sy",1.0);
  addreal("tr::sz",1.0);
  addreal("tr::length",100.0);
  addreal("tr::stretchx",0.0);
  addreal("tr::stretchy",0.0);
  addreal("tr::stretchz",0.0);
  addreal("tr::movex",0.0);
  addreal("tr::movey",0.0);
  addreal("tr::movez",0.0);
  addreal("tr::copyx",0.0);
  addreal("tr::copyy",0.0);
  addreal("tr::copyz",0.0);
  addreal("tr::projectx",0.0);
  addreal("tr::projecty",0.0);
  addreal("tr::projectz",0.0);
  addreal("tr::deltarotationx",5.0);
  addreal("tr::deltarotationy",5.0);
  addreal("tr::deltarotationz",5.0);
  addreal("tr::angle",90.0);
  addinteger("tr::projecttype",0);
  addinteger("tr::copytype",0);
  addinteger("tr::movetype",0);
  addinteger("tr::stretchtype",0);
  addinteger("tr::scaletype",0);
  addinteger("tr::ncopies",1);
  addinteger("tr::nrotatecopies",1);
  addinteger("tr::pncopies",1);
  addinteger("pt::n",5);
  addinteger("pt::spacingmode",0);
  addreal("pt::spacing",10.0);
  addinteger("tr::mtype",0);
  addinteger("tr::rtype",0);
  addinteger("tr::mcopies",1);
  addinteger("pr::papersize",-1);
  addinteger("pr::orientation",-1);
  addreal("pr::xoffset",15.0);
  addreal("pr::yoffset",15.0);
  // KMJ added for printer preview dialog scale
  addreal("pr::printscale",1.0);
  // kmj added for v8 enhancements
  addinteger("pr::uselastprinter",1);
  addstring("pr::lastprinter","");
  addinteger("pr::pdfresolution",1200);
  addinteger("pr::keeppreviewopen",0);
  // KMJ
  addreal("db::atoler",0.0001);
  addreal("db::ptoler",0.01);
  addreal("db::minimumlineweight",0.1);
  addreal("db::ttoler",0.01);
  addreal("db::explodetoler",0.01);
  addreal("db::stoler",4.0);
  addinteger("db::units",0);
  addinteger("db::numericdisplaytolerance",2);
  addinteger("db::versions",5);
  addreal("db::dtoler",2.0);
  addinteger("db::constraindrag",0);
  addinteger("db::dragdetail",0);
  addinteger("db::dragstyle",0);
  addinteger("db::hidemeasuredialog",0);
  addreal("db::autosavetimeout",10.0);
  addinteger("db::autosaveenabled",1);
  addinteger("db::showhandleswhenselected",1);
  addinteger("wn::dynamiccursor",1);
  addinteger("wn::distancetracking",1);
  addinteger("db::incrementalcoordinates",1);
  addinteger("db::disablefastrepaint",0);
  addinteger("db::usewindowscursors",1);
  //addinteger("db::displaylinewidths",0); // 0=no display;1=display
  addinteger("db::zoomstep",1);
  addreal("db::selectcolour.red",1.0);
  addreal("db::selectcolour.green",0.0);
  addreal("db::selectcolour.blue",1.0);
  addinteger("db::lineweightdisplay",0);
  addinteger("db::previewtype",-1);// -1= no preview saved; 0=screen shot type preview; 1=drawn image (saved as a figure)
  addinteger("db::savepreview",1);// 1=save;0=don't save
  addinteger("vw::autoviewrefresh",0);
  addinteger("vw::enableautoplotscale",0);
  addreal("vw::autoplotscale",2.0);
  addreal("ln::autosnapangle",45.0);
  addreal("ln::radius",10.0);
  addreal("ln::chordlength",10.0);
  addinteger("ln::rectmode",0);
  addinteger("ln::rectfixsize",0);
  addinteger("ln::hatch",0);
  addinteger("ln::fixedsize",0);
  addinteger("ln::corners",1);
  addreal("ln::rectheight",10.0);
  addreal("ln::rectwidth",10.0);
  addreal("ln::chamferd1",10.0);
  addreal("ln::chamferd2",10.0);
  addreal("ln::chamferd3",10.0);
  addinteger("ln::type",1);
  addreal("gd::xspacing",10.0);
  addreal("gd::yspacing",10.0);
  addinteger("gd::xreference",10);
  addinteger("gd::yreference",10);
  addreal("gd::angle",0.0);
  // KMJ: new grid colours
  addreal("gd::minor.red",0.5);
  addreal("gd::minor.green",0.5);
  addreal("gd::minor.blue",0.5);
  addreal("gd::major.red",0.0);
  addreal("gd::major.green",0.0);
  addreal("gd::major.blue",0.0);
  //
  addinteger("gd::options",0);
  addinteger("ps::alongtype",0);
  addinteger("ps::fixdistance1",0);
  addinteger("ps::fixdistance2",0);
  addreal("ps::distance1",10.0);
  addreal("ps::distance2",10.0);
  addreal("ps::dalong",10.0);
  addreal("ps::palong",10.0);
  addstring("ps::falong","1/2");
  addreal("vw::distance",5.0);
  addreal("sp::depth",5.0);
  addreal("sp::radius",10.0);
  addreal("sp::transparentred",0.0);
  addreal("sp::transparentgreen",0.0);
  addreal("sp::transparentblue",0.0);
  addinteger("sp::usetransparency",0);
  addinteger("sp::incremental",0);

  addinteger("sp::translatemode",0);
  addreal("sp::movedx",0.0);
  addreal("sp::movedy",0.0);
  addreal("sp::movedz",0.0);
  addreal("sp::scalex",1.0);
  addreal("sp::scaley",1.0);
  addreal("sp::scalez",1.0);
  addreal("sp::rotationangle",0.0);
  addinteger("sp::flattenmode",0);
  addreal("sp::flattenz",0.0);
  addinteger("sp::pointselectionmode",0);
  addreal("sp::smoothingdistance",0.0);

  addinteger("sp::un",4);
  addinteger("sp::vn",4);
  addinteger("sp::udegree",3);
  addinteger("sp::vdegree",3);
  addinteger("sp::triangulated",0);
  addinteger("sp::xmesh",5);
  addinteger("sp::ymesh",5);
  addbm("sp::options",options);
  addlong("pl::gradientcolour",RGB(0,0,0));
  addreal("pl::transparency",0.0);
  addreal("pl::xspacing",20.0);
  addreal("pl::yspacing",20.0);
  addreal("pl::angle",0.0);
  addreal("pl::crossangle",90.0);
  addreal("pl::red",0.0);
  addreal("pl::green",0.0);
  addreal("pl::blue",0.0);
  addreal("pl::transparencyrange",0.0);
  options.clearall();
  addbm("pl::options",options);
  options.clearall();
  addstring("pl::patternname","");
  addstring("pl::bitmapname","");
  addstring("pl::patternfilename","");
  addstring("pl::bitmapfilename","");
  addreal("di::extension",3.0);
  addreal("di::gap",1.0);
  addreal("di::stackdis",10.0);
  addreal("di::uppertol",1.0);
  addreal("di::lowertol",-1.0);
  addreal("di::scale",1.0);
  addreal("di::textyoffset",0.0);
  addinteger("di::precision",2);
  addinteger("di::tolprecision",2);
  addinteger("di::term1style",1);
  addinteger("di::term1weight",2);
  addreal("di::term1length",4.0);
  addreal("di::term1height",2.0);
  addinteger("di::term2style",4);
  addinteger("di::term2weight",2);
  addreal("di::term2length",2.0);
  addreal("di::term2height",2.0);
//#ifdef _USING_QTCHAR
//  addstring("di::vtextfont",CHAR2RCCHR("Arial"));
//  addstring("di::vtexttext",CHAR2RCCHR(""));
//#else
  addstring("di::vtextfont","Arial");
  addstring("di::vtexttext","");
//#endif
  addreal("di::vtextheight",2.0);
  addreal("di::vtextwidth",0.0);
  addreal("di::vtextangle",0.0);
  addreal("di::vtextvertspacing",0.0);
  options.set(1);
  addbm("di::vtextoptions",options);
//#ifdef _USING_QTCHAR
//  addstring("di::ttextfont",CHAR2RCCHR("Arial"));
//  addstring("di::ttexttext",CHAR2RCCHR(""));
//#else
  addstring("di::ttextfont","Arial");
  addstring("di::ttexttext","");
//#endif
  addreal("di::ttextheight",2.0);
  addreal("di::ttextwidth",0.0);
  addreal("di::ttextangle",0.0);
  addreal("di::ttextvertspacing",0.0);
  addbm("di::ttextoptions",options);
//#ifdef _USING_QTCHAR
//  addstring("di::ptextfont",CHAR2RCCHR("Arial"));
//  addstring("di::ptexttext",CHAR2RCCHR(""));
//#else
  addstring("di::ptextfont","Arial");
  addstring("di::ptexttext","");
//#endif
  addreal("di::ptextheight",2.0);
  addreal("di::ptextwidth",0.0);
  addreal("di::ptextangle",0.0);
  addreal("di::ptextvertspacing",0.0);
  addbm("di::ptextoptions",options);
//#ifdef _USING_QTCHAR
//  addstring("di::stextfont",CHAR2RCCHR("Arial"));
//  addstring("di::stexttext",CHAR2RCCHR(""));
//#else
  addstring("di::stextfont","Arial");
  addstring("di::stexttext","");
//#endif
  addreal("di::stextheight",2.0);
  addreal("di::stextwidth",0.0);
  addreal("di::stextangle",0.0);
  addreal("di::stextvertspacing",0.0);
  addbm("di::stextoptions",options);
  options.clear(1);  options.set(2);  options.set(3);  options.set(4);
  addbm("di::options",options);
  addinteger("di::thalocolour",0);
  addinteger("di::phalocolour",0);
  addinteger("di::shalocolour",0);
  addinteger("di::vhalocolour",0);

  options.clearall();
  addreal("le::extension",3.0);
  addreal("le::gap",1.0);
  addreal("le::stackdis",10.0);
  addreal("le::uppertol",1.0);
  addreal("le::lowertol",-1.0);
  addreal("le::scale",1.0);
  addinteger("le::precision",2);
  addinteger("le::tolprecision",2);
  addinteger("le::term1style",1);
  addinteger("le::term1weight",2);
  addreal("le::term1length",4.0);
  addreal("le::term1height",2.0);
  addinteger("le::term2style",4);
  addreal("le::term2length",2.0);
  addreal("le::term2height",2.0);
//#ifdef _USING_QTCHAR
//  addstring("le::vtextfont",CHAR2RCCHR("Arial"));
//  addstring("le::vtexttext",CHAR2RCCHR(""));
//#else
  addstring("le::vtextfont","Arial");
  addstring("le::vtexttext","");
//#endif
  addreal("le::vtextheight",2.0);
  addreal("le::vtextwidth",0.0);
  addreal("le::vtextangle",0.0);
  addreal("le::vtextvertspacing",0.0);
  options.set(1);
  addbm("le::vtextoptions",options);
//#ifdef _USING_QTCHAR
//  addstring("le::ttextfont",CHAR2RCCHR("Arial"));
//  addstring("le::ttexttext",CHAR2RCCHR(""));
//#else
  addstring("le::ttextfont","Arial");
  addstring("le::ttexttext","");
//#endif
  addreal("le::ttextheight",2.0);
  addreal("le::ttextwidth",0.0);
  addreal("le::ttextangle",0.0);
  addreal("le::ttextvertspacing",0.0);
  addinteger("le::thalocolour",0);
  addinteger("le::phalocolour",0);
  addinteger("le::shalocolour",0);
  addinteger("le::vhalocolour",0);
  addbm("le::ttextoptions",options);
//#ifdef _USING_QTCHAR
//  addstring("le::ptextfont",CHAR2RCCHR("Arial"));
//  addstring("le::ptexttext",CHAR2RCCHR(""));
//#else
  addstring("le::ptextfont","Arial");
  addstring("le::ptexttext","");
//#endif
  addreal("le::ptextheight",2.0);
  addreal("le::ptextwidth",0.0);
  addreal("le::ptextangle",0.0);
  addreal("le::ptextvertspacing",0.0);
  addbm("le::ptextoptions",options);
//#ifdef _USING_QTCHAR
//  addstring("le::stextfont",CHAR2RCCHR("Arial"));
//  addstring("le::stexttext",CHAR2RCCHR(""));
//#else
  addstring("le::stextfont","Arial");
  addstring("le::stexttext","");
//#endif
  addreal("le::stextheight",2.0);
  addreal("le::stextwidth",0.0);
  addreal("le::stextangle",0.0);
  addreal("le::stextvertspacing",0.0);
  addbm("le::stextoptions",options);
  options.clear(1);  options.set(2);  options.set(3);  options.set(4);
  addbm("le::options",options);
//#ifdef _USING_QTCHAR
//  addstring("tx::font",CHAR2RCCHR("Arial"));
//#else
  addstring("tx::font","Arial");
//#endif
  options.clearall();
  addbm("tx::options",options);
  addreal("tx::width",0.0);
  addreal("tx::height",10.0);
  addreal("tx::vertspacing",0.0);
  addreal("tx::angle",0.0);
//#ifdef _USING_QTCHAR
//  addstring("tx::text",CHAR2RCCHR(""));
//  addstring("tx::leadertext",CHAR2RCCHR(""));
//#else
  addstring("tx::text","");
  addstring("tx::leadertext","");
//#endif
  addstring("tx::prompt","Enter the component text");
  addstring("tx::label","AT1");
  addbm("rs::options",options);
  addreal("rs::radius",20.0);
  addinteger("rs::xmesh",4);
  addinteger("rs::ymesh",4);
  addinteger("ln::nlines",2);
  addinteger("ln::cstart",1);
  addinteger("ln::cend",1);
  addinteger("ln::nsides",6);
  addinteger("ln::inside",1);
  addinteger("tx::halocolour",0);
  addreal("ln::offsetd",10.0);
  addreal("ln::thickness",0);
  addinteger("ln::offsetpos",0);
  addinteger("ln::cloop",1);
  addreal("ln::angle",45.0);
  addreal("ln::length",100.0);
  addinteger("ln::refline",0);
  addinteger("ln::moverefline",0);
  addinteger("ln::constrainlength",0);
  addinteger("cu::degree",2);
  addinteger("cu::closed",0);
  addinteger("cu::ncontrol",10);
  addinteger("cu::deleteentity",0);
  addreal("sr::angle",360.0);
  addreal("sr::deltaz",0.0);
  addbm("sr::options",options);
  addinteger("sr::xmesh",4);
  addinteger("sr::ymesh",4);
  addinteger("sr::copyprofile",0);
  addinteger("sr::insertsides",0);
  addinteger("sr::ymesh",4);
  addreal("sr::radius",10.0);
  addreal("tr::offsetd",10.0);
  addinteger("tr::cornermode",0);
  addreal("sh::ambient",0.3);
  addreal("sh::diffuse",0.7);
  addinteger("sh::save",0);
  addinteger("sh::openglshading",0);
  addinteger("sh::hiddenlines",0);
  addinteger("hd::dashhiddenlines",0);
  addreal("vw::height",10.0);
  addinteger("vw::papersize",0);
  addstring("vw::titleblock","Title-Sheets/drawing sheet-a4.cad");
  addreal("ar::radius",5.0);
  addstring("fi::lastfilename","");
  addinteger("fi::preview",1);
  addinteger("fi::autoattachdetach",0);
  addreal("fi::scalex",1.0);
  addreal("fi::scaley",1.0);
  addreal("fi::scalez",1.0);
  // v8
  addreal("fi::size",0.0);
  addreal("fi::size",0.0);
  addreal("fi::size",0.0);
  //
  addreal("fi::rotationx",0.0);
  addreal("fi::rotationy",0.0);
  addreal("fi::rotationz",0.0);
  addreal("fi::deltarotationx",5.0);
  addreal("fi::deltarotationy",5.0);
  addreal("fi::deltarotationz",5.0);
  addstring("fi::defaultfigure","");
  addstring("fi::defaultpath","");
  addinteger("fi::state1",0);
  addinteger("fi::state2",0);
  addreal("el::major",20.0);
  addreal("el::minor",10.0);
  addbm("fi::options",options);
  addstring("gp::classname","group");
  addstring("gp::instance","instance");
  addbm("gp::options",options);
  options.clearall();
  addinteger("rt::imageformat",0);
  addinteger("rt::resolution",1);
  addstring("rt::filename","");
  addstring("rt::backimage","");
  addinteger("rt::supersamplingrays",0);
  addinteger("rt::maximumrays",10);
  addbm("rt::options",options);
  addreal("rt::backgroundcolour.red",1.0);
  addreal("rt::backgroundcolour.green",1.0);
  addreal("rt::backgroundcolour.blue",1.0);
  addreal("rt::ambientcolour.red",1.0);
  addreal("rt::ambientcolour.green",1.0);
  addreal("rt::ambientcolour.blue",1.0);
  addreal("rt::ambientstrength",0.3);
  addreal("rt::fogdistance",100.0);
  addreal("rt::fogcolour.red",1.0);
  addreal("rt::fogcolour.green",1.0);
  addreal("rt::fogcolour.blue",1.0);
  addreal("li::height",100.0);
  addreal("li::intensity",1.0);
  addreal("li::size",20.0);
  addreal("li::fadedistance",100.0);
  addreal("li::fadeexponent",2.0);
  addreal("li::concentration",2.0);
  addreal("li::coneangle",20.0);
  addstring("li::city","Adelaide SA");
  addreal("li::longitude",-138.36);
  addreal("li::latitude",-34.56);
  addreal("li::timezone",-9.5);
  BitMask loptions(32);
  loptions.set(1,1);
  addbm("li::options",loptions);
  addinteger("sp::shader",0);
  addbm("sp::options",options);
  addreal("sp::transparency",0.0);
  addreal("sp::ambient",0.3);
  addreal("sp::diffuse",0.6);
  addreal("sp::specular",0.1);
  addreal("sp::exponent",4.0);
  addreal("sp::reflectivity",0.5);
  addreal("sp::smoothness",0.4);
  addreal("sp::metallic",0.4);
  addreal("sp::refractiveindex",1.0);
  addstring("sp::bumpmap","");
  addreal("sp::bmx1",0.0);
  addreal("sp::bmy1",0.0);
  addreal("sp::bmx2",1.0);
  addreal("sp::bmy2",1.0);
  addbm("sp::bmoptions",options);
  addinteger("sp::texturetype",0);
  addstring("sp::texturemap","");
  addreal("sp::tmx1",0.0);
  addreal("sp::tmy1",0.0);
  addreal("sp::tmx2",1.0);
  addreal("sp::tmy2",1.0);
  addbm("sp::tmoptions",options);
  addreal("sp::texturesize",10.0);
  addreal("sp::patternscale",1.0);
  addreal("sp::colourscale",1.0);
  addinteger("dx::units",0);
  addinteger("dx::selected_entities",0);
  options.clearall();
  addbm("dx::options",options);
  addreal("dx::curvelength",1.0);
  addreal("dx::textscale",2.0/3.0);
  addreal("ps::height",5.0);
  addreal("ps::lengthscale",1.0);
  addreal("ps::shear",0.0);
  addreal("ps::gap",0.0);
  addinteger("ps::orientation",0);
  addinteger("ps::fitting",0);
  addinteger("ps::mirror",0);
  addinteger("ps::size",0);
  addstring("ps::filename","");
//#ifdef _USING_QTCHAR
//  addstring("tx::counterprefix",CHAR2RCCHR(""));
//  addstring("tx::countersuffix",CHAR2RCCHR(""));
//#else
  addstring("tx::counterprefix","");
  addstring("tx::countersuffix","");
//#endif
  addinteger("tx::startcounter",1);
  addinteger("tx::incrementcounter",1);

  addinteger("cal::orientation",0);
  addinteger("cal::spacingmode",0);
  addinteger("cal::n",2);
  addreal("cal::spacing",20.0);

  addreal("ca::dx",20.0);
  addreal("ca::dy",10.0);
  addreal("ca::dd",20.0);
  addreal("ca::dc",10.0);
  addinteger("ca::nx",10);
  addinteger("ca::ny",10);
  addinteger("ca::nc",10);
  addinteger("ca::type",0);

  addinteger("df::layer",0);
  addinteger("df::colour",0);
  addinteger("df::style",0);
  addinteger("df::weight",0);

  addinteger("df::viewtab",0);

  addinteger("cb::bitmapsize",0);
  addinteger("cb::savetype",0);
  addinteger("cb::quality",0);

  addinteger("pa::insertmode",0);
  addreal("pa::thickness",10.0);
  addreal("pa::height",100.0);
  addreal("pa::offset",10.0);

  addreal("pa::holedepth",100.0);
  addreal("pa::holedraftangle",10.0);

  addinteger("pa::closed",0);

  addlong("cl::0",RGB(255,0,0));  // red
  addlong("cl::1",RGB(0,255,0));  // green
  addlong("cl::2",RGB(0,0,255));  // blue
  addlong("cl::3",RGB(255,255,0));// yellow
  addlong("cl::4",RGB(0,255,255));// cyan
  addlong("cl::5",RGB(255,0,255));// magenta
  addlong("cl::6",RGB(127,0,0));  // dark red
  addlong("cl::7",RGB(0,127,0));  // dark green
  addlong("cl::8",RGB(0,0,127));  // dark blue
  addlong("cl::9",0L); // black

  for (i = 10 ; i <= 244 ; i++)
  {
       Colour colour;
       sprintf(key,"cl::%d",i);
       colour.sethls(i < 122 + 10 ? (i-10)/122.0*360.0 : (i-10-122)/122.0*360.0,i < 122 + 9 ? 0.5 : 0.25,1.00);
       RCCOLORREF c; colour.get(&c);
       addlong(key,(long)c);
  }

  for (i = 245 ; i <= 255 ; i++)
  {
      Colour colour;
       sprintf(key,"cl::%d",i);
       colour.red = colour.green = colour.blue = int(255.0 * double(i-245)/10.0);
       RCCOLORREF c; colour.get(&c);
       addlong(key,(long)c);
  }

  // debug
  qDebug() << "debug : value::initialize()";
  for (i = 0 ; i <= 255 ; i++)
  {
      sprintf(key,"cl::%d",i);
      //qDebug() << "cl::" << i << "=" << v.getlong(key);
  }
  //

  addinteger("wn::clickcommandid",1101);

  addstring("mt::subdirectory","");



#if ! defined(_WIN32_WCE)
  GetPrivateProfileString(_RCT("Strings"),0,_RCT(""),value,sizeof(value),home.getinifilename());
#endif
  s = value;
  while (*s != 0)
    {
/*
#ifdef _USING_QTCHAR
      if( strcmp(s,"tx::font") == 0 ||
          strcmp(s,"tx::text") == 0 ||
          strcmp(s,"tx::counterprefix") == 0 ||
          strcmp(s,"tx::countersuffix") == 0 ||
          strcmp(s,"tx::leadertext") == 0 ||

          strcmp(s,"di::vtextfont") == 0 ||
          strcmp(s,"di::vtexttext") == 0 ||
          strcmp(s,"di::ttextfont") == 0 ||
          strcmp(s,"di::ttexttext") == 0 ||
          strcmp(s,"di::ptextfont") == 0 ||
          strcmp(s,"di::ptexttext") == 0 ||
          strcmp(s,"di::stextfont") == 0 ||
          strcmp(s,"di::stexttext") == 0 ||

          strcmp(s,"le::vtextfont") == 0 ||
          strcmp(s,"le::vtexttext") == 0 ||
          strcmp(s,"le::ttextfont") == 0 ||
          strcmp(s,"le::ttexttext") == 0 ||
          strcmp(s,"le::ptextfont") == 0 ||
          strcmp(s,"le::ptexttext") == 0 ||
          strcmp(s,"le::stextfont") == 0 ||
          strcmp(s,"le::stexttext") == 0
          )
          addstring(s,_RCT(""));
      else
          addstring(s,_RCT(""));
#else
          addstring(s,"");
#endif
*/
       addstring(s,_RCT(""));
      s += strlen(s) + 1;
    }

  //  Load default values from the .ini file
  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
    onr->load();
  for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
    oi->load();
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
  {
/*
#ifdef _USING_QTCHAR
      if( strcmp(os->name,"tx::font") != 0 &&
          strcmp(os->name,"tx::text") != 0 &&
          strcmp(os->name,"tx::counterprefix") != 0 &&
          strcmp(os->name,"tx::countersuffix") != 0 &&
          strcmp(os->name,"tx::leadertext") != 0 &&

          strcmp(os->name,"di::vtextfont") != 0 &&
          strcmp(os->name,"di::vtexttext") != 0 &&
          strcmp(os->name,"di::ttextfont") != 0 &&
          strcmp(os->name,"di::ttexttext") != 0 &&
          strcmp(os->name,"di::ptextfont") != 0 &&
          strcmp(os->name,"di::ptexttext") != 0 &&
          strcmp(os->name,"di::stextfont") != 0 &&
          strcmp(os->name,"di::stexttext") != 0 &&

          strcmp(os->name,"le::vtextfont") != 0 &&
          strcmp(os->name,"le::vtexttext") != 0 &&
          strcmp(os->name,"le::ttextfont") != 0 &&
          strcmp(os->name,"le::ttexttext") != 0 &&
          strcmp(os->name,"le::ptextfont") != 0 &&
          strcmp(os->name,"le::ptexttext") != 0 &&
          strcmp(os->name,"le::stextfont") != 0 &&
          strcmp(os->name,"le::stexttext") != 0
          )
#endif
*/
        os->load();
  }

  for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
    ol->load();
  for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
    ob->load();

  //  Load the default layernames as well
  db.layernames.clear();
  for (i = 0 ; i < 1000 ; i++)
    if (strlen(db.layernames.getlayername(i)) == 0)
      {RCCHAR key[300],value[300];
         sprintf(key,"Layer%d",i);
         GetPrivateProfileString(_RCT("LayerNames"),key,_RCT(""),value,300,home.getinifilename());
         db.layernames.setlayername(i,value);
      }


  //  Load the dashed line styles as well
  db.linestyles.load(home.getinifilename());
  db.setlineweightdisplay(v.getinteger("db::lineweightdisplay"));

  // load the default colour palette
  db.initColourTable();
  // put the default colours into the longs table
  for (i = 0 ; i < 256 ; i++)
  {
      long c;
      sprintf(key,"cl::%d",i);
      setlong(key,RGB(db.colourtable[i].red,db.colourtable[i].green,db.colourtable[i].blue));
  }

  // load the ini colour palette
  db.loadColourTable(home.getinifilename());


  if (program != 0)
    program->updatecursortype(program->calcCursorFromBackground());

  qDebug() << "select red   : " << getreal("db::selectcolour.red");
  qDebug() << "select green : " << getreal("db::selectcolour.green");
  qDebug() << "select blue  : " << getreal("db::selectcolour.blue");
}

Values::~Values()
{
DebugTraceLine(__FILE__,__LINE__);
 clear();
DebugTraceLine(__FILE__,__LINE__);
}

void Values::save(void)
{OneReal *onr;
 OneInteger *oi;
 OneString *os;
 //OneRCString *RCos;
 OneLong *ol;
 OneBitMask *ob;
 int i;

#if 1
   QString qkey,qvalue;
   QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

   //for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
   //  onr->save();
   for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
   {
       qkey = "Reals/" + QString(onr->name);
       qvalue = QString("%1").arg(onr->value,0,'g',16);
       settings.setValue(qkey,qvalue);
   }

   //for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
   //  oi->save();
   for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
   {
       //sprintf(svalue,"%d",value);
       qkey = "Integers/" + QString(oi->name);
       qvalue = QString("%1").arg(oi->value);
       settings.setValue(qkey,qvalue);
   }

   //for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
   //os->save();
   for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
   {
       qkey = "Strings/" + QString(os->name);
       qvalue = QString(os->value);
       settings.setValue(qkey,qvalue);
   }
   //for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
   //  ol->save();
   for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
   {
       //sprintf(svalue,"%ld",value);
       qkey = "Longs/" + QString(ol->name);
       qvalue = QString("%1").arg(ol->value);
       settings.setValue(qkey,qvalue);
       qDebug() << qvalue;
   }
   //for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
   //  ob->save();
   for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
   {
       char svalue[1024];
       qkey = "BitMasks/" + QString(ob->name);
       ob->value->save(svalue);
       qvalue = QString("%1").arg(svalue);
       settings.setValue(qkey,qvalue);
   }
   //  Save the layertable names as well
   for (i = 0 ; i < 1000 ; i++)
     if (strlen(db.layernames.getlayername(i)) > 0)
     {
          //RCCHAR key[300];
          //sprintf(key,"Layer%d",i);
          qkey = "LayerNames/" + QString("Layer%1").arg(i);
          qvalue = QString(db.layernames.getlayername(i));
          settings.setValue(qkey,qvalue);
     }
#else
/*
  QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
  {
      QString qkey = "Reals/" + QString(onr->name);
      QString qvalue = QString("%1").arg(onr->value,0,'g',16);
      settings.setValue(qkey,qvalue);
  }
  */
  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
    onr->save();
  for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
    oi->save();
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    os->save();
  for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
    ol->save();
  for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
    ob->save();

  //  Save the layertable names as well
  for (i = 0 ; i < 1000 ; i++)
    if (strlen(db.layernames.getlayername(i)) > 0)
      {RCCHAR key[300];
         sprintf(key,"Layer%d",i);
         WritePrivateProfileString( _RCT("LayerNames") , key,db.layernames.getlayername(i),home.getinifilename());
      }
#endif
  //  Save the dashed line styles as well
  db.linestyles.save(home.getinifilename());

  // KMJ: save the colour table
  db.saveColourTable(home.getinifilename());
}

void Values::clear()
{OneReal *onr;
 OneInteger *oi;
 OneString *os;
 OneRCString *RCos;
 OneLong *ol;
 OneBitMask *ob;
  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
    delete onr;
  reals.destroy();
  for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
    delete oi;
  integers.destroy();
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    delete os;
  strings.destroy();

  for (RCstrings.start() ;  (RCos = (OneRCString *) RCstrings.next()) != NULL ; )
    delete RCos;
  RCstrings.destroy();

  for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
    delete ol;
  longs.destroy();
  for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
    delete ob;
  bitmasks.destroy();

}

void Values::addreal(const char *n,double v)
{
    QString qn = QString().fromLatin1(n);
    Values::addreal(qn.data(),v);
}

void Values::addreal(RCCHAR *n,double v)
{
  OneReal *onr;
  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
    if (strcmp(onr->name,n) == 0)
      {
        onr->value = v;
        return;
      }
  if ((onr = new OneReal(n,v)) != NULL && onr->name != NULL)
    reals.add(onr);
  else
    delete onr;
}

double Values::getreal(const char *n)
{
    QString qn = QString().fromLatin1(n);
    return Values::getreal(qn.data());
}

double Values::getreal(RCCHAR *n)
{
  OneReal *onr;
  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
    if (strcmp(onr->name,n) == 0)
    {
        return onr->value;
    }
  return -9999999999.0;
}

void Values::setreal(const char *n,double v)
{
    QString qn = QString().fromLatin1(n);
    setreal(qn.data(),v);
}

void Values::setreal(RCCHAR *n,double v)
{
  OneReal *onr;
  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
    if (strcmp(onr->name,n) == 0)
    {
        onr->value = v;
        return;
    }
  addreal(n,v);
}

void Values::addinteger(const char *n,int v)
{
    QString qn = QString().fromLatin1(n);
    Values::addinteger(qn.data(), v);
}

void Values::addinteger(RCCHAR *n,int v)
{
    //qDebug() << "Values::addinteger : name  : " << QString(n);
    //qDebug() << "Values::addinteger : value : " << v;

  OneInteger *oi;
  for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
    if (strcmp(oi->name,n) == 0)
    {
        //qDebug() << "Values::addinteger : found  : " << QString(oi->name);
        oi->value = v;  return;
    }
  if ((oi = new OneInteger(n,v)) != NULL && oi->name != NULL)
    integers.add(oi);
  else
    delete oi;
}

int Values::getinteger(const char *n)
{
    QString qn = QString().fromLatin1(n);
    return getinteger(qn.data());
}

int Values::getinteger(RCCHAR *n)
{
    //qDebug() << "Values::getinteger : name  : " << QString(n);

  OneInteger *oi;
  for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
    if (strcmp(oi->name,n) == 0)
    {
        //qDebug() << "Values::getinteger : found  : " << QString(oi->name);
        //qDebug() << "Values::getinteger : value  : " << oi->value;
        return oi->value;
    }
  return -32767;
}

void Values::setinteger(const char *n,int v)
{
    QString qn = QString().fromLatin1(n);
    setinteger(qn.data(), v);
}

void Values::setinteger(RCCHAR *n,int v)
{
  OneInteger *oi;
  for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
    if (strcmp(oi->name,n) == 0)
    {
        oi->value = v;
        return;
    }
  addinteger(n,v);
}

void Values::addlong(const char *n,long v)
{
    QString qn = QString().fromLatin1(n);
    addlong(qn.data(),v);
}

void Values::addlong(RCCHAR *n,long v)
{OneLong *ol;
  for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
    if (strcmp(ol->name,n) == 0)
      {  ol->value = v;  return;
      }
  if ((ol = new OneLong(n,v)) != NULL && ol->name != NULL)
    longs.add(ol);
  else
    delete ol;
}

long Values::getlong(const char *n)
{
    QString qn = QString().fromLatin1(n);
    return getlong(qn.data());
}

long Values::getlong(RCCHAR *n)
{OneLong *ol;
  for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
    if (strcmp(ol->name,n) == 0) return ol->value;
  return -MAXLONG;
}

void Values::setlong(const char *n,long v)
{
    QString qn = QString().fromLatin1(n);
    setlong(qn.data(),  v);
}

void Values::setlong(RCCHAR *n,long v)
{OneLong *ol;
  for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
    if (strcmp(ol->name,n) == 0)
      {  ol->value = v;  return;
      }
  addlong(n,v);
}

/// Look for a string in the Strings list with the given name
//  if found replace it with the value v
//  if not found add the new string with the given name and value
void Values::addstring(const char *n,const char *v)
{
    QString qn = QString().fromLatin1(n);
    Values::addstring(qn.data(),QString(v).data());
}

void Values::addstring(const char *n,RCCHAR *v)
{
    QString qn = QString().fromLatin1(n);
    Values::addstring(qn.data(),v);
}

void Values::addstring(RCCHAR *n,RCCHAR *v)
{OneString *os=0;
 RCCHAR *oldstring=0;
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    if (strcmp(os->name,n) == 0)
      {//  Don't overwrite the architect settings of the following :-
         if (strlen(os->value) == 0 ||
             strncmp(os->name,"ar::windowcategory(",strlen("ar::windowcategory(")) != 0 &&
             strncmp(os->name,"ar::windowdesc(",strlen("ar::windowdesc(")) != 0 &&
             strncmp(os->name,"ar::windowname(",strlen("ar::windowname(")) != 0 &&
             strncmp(os->name,"ar::windowwidth(",strlen("ar::windowwidth(")) != 0 &&
             strncmp(os->name,"ar::windowheight(",strlen("ar::windowheight(")) != 0 &&
             strncmp(os->name,"ar::windowheadheight(",strlen("ar::windowheadheight(")) != 0 &&
             strncmp(os->name,"ar::windowlabel(",strlen("ar::windowlabel(")) != 0 &&
             strncmp(os->name,"ar::doorcategory(",strlen("ar::doorcategory(")) != 0 &&
             strncmp(os->name,"ar::doordesc(",strlen("ar::doordesc(")) != 0 &&
             strncmp(os->name,"ar::doorname(",strlen("ar::doorname(")) != 0 &&
             strncmp(os->name,"ar::doorwidth(",strlen("ar::doorwidth(")) != 0 &&
             strncmp(os->name,"ar::doorheight(",strlen("ar::doorheight(")) != 0 &&
             strncmp(os->name,"ar::doorlabel(",strlen("ar::doorlabel(")) != 0 &&
             strncmp(os->name,"ar::symbolcategory(",strlen("ar::symbolcategory(")) != 0 &&
             strncmp(os->name,"ar::symboldesc(",strlen("ar::symboldesc(")) != 0 &&
             strncmp(os->name,"ar::symbolname(",strlen("ar::symbolname(")) != 0)
           {
               // found the name so replace the value with v
               oldstring = os->value;
              if ((os->value = new RCCHAR[strlen(v)+1]) != NULL)
                {  strcpy(os->value,v);
                   delete [] oldstring;
                }
              else
                os->value = oldstring;
           }
         return;
      }

// not found so add the new string an value
  if ((os = new OneString(n,v)) != NULL && os->name != NULL && os->value != NULL)
    strings.add(os);
  else
    delete os;
}

RCCHAR *Values::getstring(const char *n)
{
    QString qn(n);
    return getstring(qn.data());
}

RCCHAR *Values::getstring(RCCHAR *n)
{OneString *os;
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    if (strcmp(os->name,n) == 0)
      return os->value;
  return _RCT("");
}

RCCHAR *Values::getstring(const char *n, RCCHAR *value )
{
    QString qn(n);
    return getstring(qn.data(), value );
}

RCCHAR *Values::getstring(RCCHAR *n, RCCHAR *value )
{OneString *os;
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
  {
    if (strcmp(os->name,n) == 0)
    {
        value = os->value;
        return os->value;
    }
  }
  value = os->value;
  return _RCT("");
}

int Values::getfirststring(const char *n,RCCHAR *name,RCCHAR *value)
{
    QString qn = QString().fromLatin1(n);
    return getfirststring(qn.data(),name,value);
}

int Values::getfirststring(RCCHAR *n,RCCHAR *name,RCCHAR *value)
{OneString *os;
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    if (strncmp(os->name,n,strlen(n)) == 0)
      {  strcpy(name,os->name);
         strcpy(value,os->value);
         return 1;
      }
  return 0;
}

int Values::getnextstring(const char *n,RCCHAR *name,RCCHAR *value)
{
    QString qn = QString().fromLatin1(n);
    return getnextstring(qn.data(),name,value);
}

int Values::getnextstring(RCCHAR *n,RCCHAR *name,RCCHAR *value)
{OneString *os;
  for ( ;  (os = (OneString *) strings.next()) != NULL ; )
    if (strncmp(os->name,n,strlen(n)) == 0)
      {  strcpy(name,os->name);
         strcpy(value,os->value);
         return 1;
      }
  return 0;
}

void Values::setstring(const char *n,RCCHAR *v)
{
    QString qn = QString().fromLatin1(n);
    setstring(qn.data(),v);
}

void Values::setstring(const char *n,const char *v)
{
    QString qn = QString().fromLatin1(n);
    setstring(qn.data(),QString(v).data());
}

void Values::setstring(RCCHAR *n,RCCHAR *v)
{OneString *os=0;
 RCCHAR *oldstring=0;
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    if (strcmp(os->name,n) == 0)
      {  oldstring = os->value;
         if ((os->value = new RCCHAR[strlen(v)+1]) != NULL)
           {  strcpy(os->value,v);
              delete [] oldstring;
           }
         else
           os->value = oldstring;
         return;
      }
  addstring(n,v);
}

void Values::addbm(const char *n,const BitMask &v)
{
    QString qn = QString().fromLatin1(n);
    addbm(qn.data(),v);
}

void Values::addbm(RCCHAR *n,const BitMask &v)
{OneBitMask *ob;
 BitMask *oldbitmask;
  for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
    if (strcmp(ob->name,n) == 0)
      {  oldbitmask = ob->value;
         if ((ob->value = new BitMask(v)) != NULL && ob->value->getdefined())
           {  *ob->value = v;
              delete oldbitmask;
           }
         else
           ob->value = oldbitmask;
         return;
      }
  if ((ob = new OneBitMask(n,v)) != NULL && ob->name != NULL && ob->value != NULL && ob->value->getdefined())
    bitmasks.add(ob);
  else
    delete ob;
}

BitMask *Values::getbm(const char *n)
{
    QString qn = QString(n);
    return getbm(qn.data());
  /*
  OneBitMask *ob;
  for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
    if (strcmp(ob->name,n) == 0) return ob->value;
  return NULL;
  */
}

BitMask *Values::getbm(RCCHAR *n)
{OneBitMask *ob;
  for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
    if (strcmp(ob->name,n) == 0) return ob->value;
  return NULL;
}

void Values::setbm(const char *n,const BitMask &v)
{
    QString qn = QString().fromLatin1(n);
    setbm(qn.data(),v);
}

void Values::setbm(RCCHAR *n,const BitMask &v)
{OneBitMask *os;
 BitMask *oldbitmask;
  for (bitmasks.start() ;  (os = (OneBitMask *) bitmasks.next()) != NULL ; )
    if (strcmp(os->name,n) == 0)
      {  oldbitmask = os->value;
         if ((os->value = new BitMask(v)) != NULL && os->value->getdefined())
           {  *os->value = v;
              delete oldbitmask;
           }
         else
           os->value = oldbitmask;
         return;
      }
  addbm(n,v);
}

int Values::save(CadFile *outfile)
{short type,nvalues;
 OneReal *onr;
 OneInteger *oi;
 OneString *os;
 OneRCString *RCos;
 OneLong *ol;
 OneBitMask *ob;
  type = values_record;
  outfile->write(&type);
  nvalues = short(reals.length());
  if (! outfile->write(&nvalues)) return 0;
  for (reals.start() ;  (onr = (OneReal *) reals.next()) != NULL ; )
    if (! onr->save(outfile)) return 0;
  nvalues = short(integers.length());
  if (! outfile->write(&nvalues)) return 0;
  for (integers.start() ;  (oi = (OneInteger *) integers.next()) != NULL ; )
    if (! oi->save(outfile)) return 0;
#ifdef _USING_QTCHAR
  // NOTE: KMJ change made for using the QChar data type
  nvalues = short(strings.length());
  //nvalues = short(countStrings()); // new function to not count names of QChar items
  if (! outfile->write(&nvalues)) return 0;
  // read the RCCHAR (local encoded) values here
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
  {
#if 0
      if( strcmp(os->name,"tx::font") != 0 &&
          strcmp(os->name,"tx::text") != 0 &&
          strcmp(os->name,"tx::counterprefix") != 0 &&
          strcmp(os->name,"tx::countersuffix") != 0 &&
          strcmp(os->name,"tx::leadertext") != 0 &&

          strcmp(os->name,"di::vtextfont") != 0 &&
          strcmp(os->name,"di::vtexttext") != 0 &&
          strcmp(os->name,"di::ttextfont") != 0 &&
          strcmp(os->name,"di::ttexttext") != 0 &&
          strcmp(os->name,"di::ptextfont") != 0 &&
          strcmp(os->name,"di::ptexttext") != 0 &&
          strcmp(os->name,"di::stextfont") != 0 &&

          strcmp(os->name,"le::stexttext") != 0 &&
          strcmp(os->name,"le::vtextfont") != 0 &&
          strcmp(os->name,"le::vtexttext") != 0 &&
          strcmp(os->name,"le::ttextfont") != 0 &&
          strcmp(os->name,"le::ttexttext") != 0 &&
          strcmp(os->name,"le::ptextfont") != 0 &&
          strcmp(os->name,"le::ptexttext") != 0 &&
          strcmp(os->name,"le::stextfont") != 0 &&
          strcmp(os->name,"le::stexttext") != 0
          )
#endif
        if (! os->save(outfile)) return 0;
  }
  nvalues = short(RCstrings.length());
  if (! outfile->write(&nvalues)) return 0;
  // read the RCCHAR (QChar unicode) values here
  for (RCstrings.start() ;  (RCos = (OneRCString *) RCstrings.next()) != NULL ; )
    if (! RCos->save(outfile)) return 0;
#else
  nvalues = short(strings.length());
  if (! outfile->write(&nvalues)) return 0;
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
    if (! os->save(outfile)) return 0;
#endif
  nvalues = short(longs.length());
  if (! outfile->write(&nvalues)) return 0;
  for (longs.start() ;  (ol = (OneLong *) longs.next()) != NULL ; )
    if (! ol->save(outfile)) return 0;
  nvalues = short(bitmasks.length());
  if (! outfile->write(&nvalues)) return 0;
  for (bitmasks.start() ;  (ob = (OneBitMask *) bitmasks.next()) != NULL ; )
    if (! ob->save(outfile)) return 0;
  return 1;
}

int Values::load(CadFile *infile,int all)
{short nvalues;
 int i;
 OneReal *onr;
 OneInteger *oi;
 OneString *os;
 OneRCString *RCos;
 OneLong *ol;
 OneBitMask *ob;
  clear();
  initialize();
  if (! infile->read(&nvalues)) return 0;
  for (i = 0 ; i < nvalues ; i++)
    {  if ((onr = OneReal::load(infile)) == NULL) return 0;
       if (all || strncmp(onr->name,"in::",4) == 0 || strcmp(onr->name,"vw::plotscale") == 0)
         addreal(onr->name,onr->value);
       delete onr;
    }
  if (! infile->read(&nvalues)) return 0;
  for (i = 0 ; i < nvalues ; i++)
    {  if ((oi = OneInteger::load(infile)) == NULL) return 0;
       if (all ||  strncmp(oi->name,"in::",4) == 0)
         addinteger(oi->name,oi->value);
       delete oi;
    }
#ifdef _USING_QTCHAR
  // NOTE: KMJ changes made for using the QChar data type
  // versions after 2 use QChar unicode
  if( infile->getversion() != 2 )
  {
      // read any RCCHAR (local encoding) values here
      if (! infile->read(&nvalues)) return 0;
        for (i = 0 ; i < nvalues ; i++)
        {
            os = 0;
            if ((os = OneString::load(infile)) == NULL) return 0;
            if (all ||  strncmp(os->name,"in::",4) == 0)
            {
#if 0           // this is obsolete and can be removed
                // do not read these values as RCCHAR here
                if( strcmp(os->name,"tx::font") != 0 &&
                    strcmp(os->name,"tx::text") != 0 &&
                    strcmp(os->name,"tx::counterprefix") != 0 &&
                    strcmp(os->name,"tx::countersuffix") != 0 &&
                    strcmp(os->name,"tx::leadertext") != 0 &&

                    strcmp(os->name,"di::vtextfont") != 0 &&
                    strcmp(os->name,"di::vtexttext") != 0 &&
                    strcmp(os->name,"di::ttextfont") != 0 &&
                    strcmp(os->name,"di::ttexttext") != 0 &&
                    strcmp(os->name,"di::ptextfont") != 0 &&
                    strcmp(os->name,"di::ptexttext") != 0 &&
                    strcmp(os->name,"di::stextfont") != 0 &&
                    strcmp(os->name,"di::stexttext") != 0 &&

                    strcmp(os->name,"le::vtextfont") != 0 &&
                    strcmp(os->name,"le::vtexttext") != 0 &&
                    strcmp(os->name,"le::ttextfont") != 0 &&
                    strcmp(os->name,"le::ttexttext") != 0 &&
                    strcmp(os->name,"le::ptextfont") != 0 &&
                    strcmp(os->name,"le::ptexttext") != 0 &&
                    strcmp(os->name,"le::stextfont") != 0 &&
                    strcmp(os->name,"le::stexttext") != 0
                    )
                {
                    // only reading RCCHAR values here
                    os = new OneString(os->name,os->value);
                    addstring(os->name,os->value);
                }
#endif
                os = new OneString(os->name,os->value);
                addstring(os->name,os->value);
            }
            delete os;
        }
        // read any RCHAR (QChar unicode) values in the file here
        // KMJ: this nvalues is not used and will always be 0
        // remains in the file just incase of issues or possible future use
      if (! infile->read(&nvalues)) return 0;
        for (i = 0 ; i < nvalues ; i++)
        {
            RCos = 0;
            if ((RCos = OneRCString::load(infile)) == NULL) return 0;
            if (all ||  strncmp(RCos->name,"in::",4) == 0)
            {
                RCos = new OneRCString(RCos->name,RCos->value);
                addstring(RCos->name,RCos->value);
            }
            delete RCos;
        }

  }
  else
  {
      if (! infile->read(&nvalues)) return 0;
        for (i = 0 ; i < nvalues ; i++)
        {
            RCos = 0;
            os = 0;
            if ((os = OneString::load(infile)) == NULL) return 0;
            if (all ||  strncmp(os->name,"in::",4) == 0)
            {
#if 0
                // convert these to RCHAR (QChar unicode) values
                if( strcmp(os->name,"tx::font") == 0 ||
                    strcmp(os->name,"tx::text") == 0 ||
                    strcmp(os->name,"tx::counterprefix") == 0 ||
                    strcmp(os->name,"tx::countersuffix") == 0 ||
                    strcmp(os->name,"tx::leadertext") == 0 ||

                    strcmp(os->name,"di::vtextfont") == 0 ||
                    strcmp(os->name,"di::vtexttext") == 0 ||
                    strcmp(os->name,"di::ttextfont") == 0 ||
                    strcmp(os->name,"di::ttexttext") == 0 ||
                    strcmp(os->name,"di::ptextfont") == 0 ||
                    strcmp(os->name,"di::ptexttext") == 0 ||
                    strcmp(os->name,"di::stextfont") == 0 ||
                    strcmp(os->name,"di::stexttext") == 0 ||

                    strcmp(os->name,"le::vtextfont") == 0 ||
                    strcmp(os->name,"le::vtexttext") == 0 ||
                    strcmp(os->name,"le::ttextfont") == 0 ||
                    strcmp(os->name,"le::ttexttext") == 0 ||
                    strcmp(os->name,"le::ptextfont") == 0 ||
                    strcmp(os->name,"le::ptexttext") == 0 ||
                    strcmp(os->name,"le::stextfont") == 0 ||
                    strcmp(os->name,"le::stexttext") == 0
                    )
                {
                    RCos = new OneRCString(os->name,os->value);
                    addstring(RCos->name,RCos->value);
                }
                else
                {
                    // keep these as RCCHAR (local) values
                    addstring(os->name,os->value);
                }
            }
            delete RCos;
#endif
                addstring(os->name,os->value);
            }
            delete os;
        }
  }
#else
  if (! infile->read(&nvalues)) return 0;
  for (i = 0 ; i < nvalues ; i++)
    {  if ((os = OneString::load(infile)) == NULL) return 0;
       if (all ||  strncmp(os->name,"in::",4) == 0)
           addstring(os->name,os->value);
       delete os;
    }
#endif
  if (! infile->read(&nvalues)) return 0;
  for (i = 0 ; i < nvalues ; i++)
    {  if ((ol = OneLong::load(infile)) == NULL) return 0;
       if (all ||  strncmp(ol->name,"in::",4) == 0)
         addlong(ol->name,ol->value);
       delete ol;
    }
  if (! infile->read(&nvalues)) return 0;
  for (i = 0 ; i < nvalues ; i++)
    {  if ((ob = OneBitMask::load(infile)) == NULL) return 0;
       if (all ||  strncmp(ob->name,"in::",4) == 0)
         addbm(ob->name,*ob->value);
       delete ob;
    }

  program->updatecursortype(program->calcCursorFromBackground());

  return 1;
}

#ifdef _USING_QTCHAR
//@KMJ
//! Special count for when RC strings are used in values
short Values::countStrings()
{
  OneString *os;
  short n=0;
  for (strings.start() ;  (os = (OneString *) strings.next()) != NULL ; )
  {
      if( strcmp(os->name,"tx::font") == 0 ||
          strcmp(os->name,"tx::text") == 0 ||
          strcmp(os->name,"tx::counterprefix") == 0 ||
          strcmp(os->name,"tx::countersuffix") == 0 ||
          strcmp(os->name,"tx::leadertext") == 0 ||

          strcmp(os->name,"di::vtextfont") == 0 ||
          strcmp(os->name,"di::vtexttext") == 0 ||
          strcmp(os->name,"di::ttextfont") == 0 ||
          strcmp(os->name,"di::ttexttext") == 0 ||
          strcmp(os->name,"di::ptextfont") == 0 ||
          strcmp(os->name,"di::ptexttext") == 0 ||
          strcmp(os->name,"di::stextfont") == 0 ||
          strcmp(os->name,"di::stexttext") == 0 ||

          strcmp(os->name,"le::vtextfont") == 0 ||
          strcmp(os->name,"le::vtexttext") == 0 ||
          strcmp(os->name,"le::ttextfont") == 0 ||
          strcmp(os->name,"le::ttexttext") == 0 ||
          strcmp(os->name,"le::ptextfont") == 0 ||
          strcmp(os->name,"le::ptexttext") == 0 ||
          strcmp(os->name,"le::stextfont") == 0 ||
          strcmp(os->name,"le::stexttext") == 0
          )
          continue;
      n++;
  }
  return n;
}
#endif
