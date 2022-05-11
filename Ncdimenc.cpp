
#include "ncwin.h"


LineTerminator *LineTerminator::create(double s,const TerminatorInfo &info)
{  switch (info.style & 0x7F)
     {  case 0 : return new NoLineTerminator(s * info.length,s * info.height,info.style & 0x80 ? info.weight : 255);
        case 1 : return new ArrowLineTerminator(s * info.length,s * info.height,info.style & 0x80 ? info.weight : 255);
        case 2 : return new OpenArrowLineTerminator(s * info.length,s * info.height,info.style & 0x80 ? info.weight : 255);
        case 3 : return new FilledArrowLineTerminator(s * info.length,s * info.height,info.style & 0x80 ? info.weight : 255);
        case 4 : return new DotLineTerminator(s * info.length,s * info.height,info.style & 0x80 ? info.weight : 255);
        case 5 : return new FilledDotLineTerminator(s * info.length,s * info.height,info.style & 0x80 ? info.weight : 255);
        case 6 : return new SlashLineTerminator(s * info.length,s * info.height,info.style & 0x80 ? info.weight : 255);
        default : return NULL;
     }
}

static ResourceString *rs0 = NULL;
static ResourceString *rs1 = NULL;
static ResourceString *rs2 = NULL;
static ResourceString *rs3 = NULL;
static ResourceString *rs4 = NULL;
static ResourceString *rs5 = NULL;
static ResourceString *rs6 = NULL;

RCCHAR *LineTerminator::name(const TerminatorInfo &info)
{ if (rs0 == NULL) rs0 = new ResourceString(NCDIMENA + 0);
  if (rs1 == NULL) rs1 = new ResourceString(NCDIMENA + 1);
  if (rs2 == NULL) rs2 = new ResourceString(NCDIMENA + 2);
  if (rs3 == NULL) rs3 = new ResourceString(NCDIMENA + 3);
  if (rs4 == NULL) rs4 = new ResourceString(NCDIMENA + 4);
  if (rs5 == NULL) rs5 = new ResourceString(NCDIMENA + 5);
  if (rs6 == NULL) rs6 = new ResourceString(NCDIMENA + 6);
  if (rs0 == NULL || rs1 == NULL || rs2 == NULL ||
      rs3 == NULL || rs4 == NULL || rs5 == NULL ||
      rs6 == NULL) return NULL;
  switch (info.style)
    {  case 0 : return rs0->getws();
       case 1 : return rs1->getws();
       case 2 : return rs2->getws();
       case 3 : return rs3->getws();
       case 4 : return rs4->getws();
       case 5 : return rs5->getws();
       case 6 : return rs6->getws();
       default : return NULL;
    }
}

struct LTDrawCB_data
  {int drawmode;
   Transform *t;
   View3dSurface *v;
  };

#ifdef USING_WIDGETS
int ltdrawcbGL(Entity *entity,void *data)
{
  LTDrawCB_data *ltdata = (LTDrawCB_data *) data;
  entity->drawGL(ltdata->drawmode,ltdata->t,ltdata->v);
  return 1;
}

void LineTerminator::drawGL(EntityHeader *header,int drawmode,Transform *t,View3dSurface *v,CoordinateSystem *cs,int orient,double x,double y)
{
  LTDrawCB_data ltdata;
  ltdata.drawmode = drawmode;
  ltdata.t = t;
  ltdata.v = v;
  generate(header,ltdrawcbGL,&ltdata,cs,orient,x,y);
}
#endif

int ltdrawcb(Entity *entity,void *data)
{LTDrawCB_data *ltdata = (LTDrawCB_data *) data;
  entity->draw(ltdata->drawmode,ltdata->t,ltdata->v);
  return 1;
}

void LineTerminator::addtodisplaylist(EntityHeader *header,View3dSurface *,CoordinateSystem *,int,double,double)
{  header;
}

void LineTerminator::draw(EntityHeader *header,int drawmode,Transform *t,View3dSurface *v,CoordinateSystem *cs,int orient,double x,double y)
{LTDrawCB_data ltdata;
  ltdata.drawmode = drawmode;
  ltdata.t = t;
  ltdata.v = v;
  generate(header,ltdrawcb,&ltdata,cs,orient,x,y);
}

struct LTPickCB_data
  {View3dSurface *v;
   int x,y;
   Point *pnear;
   double dmin;
  };

int ltpickcb(Entity *entity,void *data)
{LTPickCB_data *ltdata = (LTPickCB_data *) data;
 double d;
  if ((d = entity->pick(ltdata->v,ltdata->x,ltdata->y,ltdata->pnear)) >= 0.0 && (ltdata->dmin < 0.0 || d < ltdata->dmin))
    ltdata->dmin = d;
  return 1;
}

double LineTerminator::pick(EntityHeader *header,View3dSurface *v,int x,int y,Point *pnear,CoordinateSystem *cs,int orient,double xc,double yc)
{LTPickCB_data ltdata;
  ltdata.v = v;
  ltdata.x = x;
  ltdata.y = y;
  ltdata.pnear = pnear;
  ltdata.dmin = -1.0;
  generate(header,ltpickcb,&ltdata,cs,orient,xc,yc);
  return ltdata.dmin;
}

struct LTPickWinCB_data
  {View3dSurface *v;
   int insidew,x1,y1,x2,y2;
   int found;
  };

int ltpickwincb(Entity *entity,void *data)
{LTPickWinCB_data *ltdata = (LTPickWinCB_data *) data;
  if (! entity->pick(ltdata->v,ltdata->insidew,ltdata->x1,ltdata->y1,ltdata->x2,ltdata->y2))
    ltdata->found = 0;
  return 1;
}

int LineTerminator::pick(EntityHeader *header,View3dSurface *v,int insidew,int x1,int y1,int x2,int y2,
                         CoordinateSystem *cs,int orient,double xc,double yc)
{LTPickWinCB_data ltdata;
  ltdata.v = v;
  ltdata.insidew = insidew;
  ltdata.x1 = x1;
  ltdata.y1 = y1;
  ltdata.x2 = x2;
  ltdata.y2 = y2;
  ltdata.found = 1;
  generate(header,ltpickwincb,&ltdata,cs,orient,xc,yc);
  return ltdata.found;
}

void ArrowLineTerminator::generate(EntityHeader *header,DimensionCallBack dcb,void *data,CoordinateSystem *cs,int orient,double x,double y)
{Point p1(x,y,0.0),p2(x-orient*getlength(),y+getheight()/2.0,0.0),p3(x-orient*getlength(),y-getheight()/2.0,0.0);
  p1 = cs->tomodel(p1);
  p2 = cs->tomodel(p2);
  p3 = cs->tomodel(p3);
  Line line1(*header,p1,p2);  
  if (getweight() != 255) line1.setweight(getweight());
  dcb(&line1,data);
  Line line2(*header,p2,p3);  
  if (getweight() != 255) line2.setweight(getweight());
  dcb(&line2,data);
  Line line3(*header,p3,p1);  
  if (getweight() != 255) line3.setweight(getweight());
  dcb(&line3,data);
}

void OpenArrowLineTerminator::generate(EntityHeader *header,DimensionCallBack dcb,void *data,CoordinateSystem *cs,int orient,double x,double y)
{Point p1(x,y,0.0),p2(x-orient*getlength(),y+getheight()/2.0,0.0),p3(x-orient*getlength(),y-getheight()/2.0,0.0);
  p1 = cs->tomodel(p1);
  p2 = cs->tomodel(p2);
  p3 = cs->tomodel(p3);
  Line line1(*header,p2,p1);  
  if (getweight() != 255) line1.setweight(getweight());
  dcb(&line1,data);
  Line line2(*header,p1,p3);  
  if (getweight() != 255) line2.setweight(getweight());
  dcb(&line2,data);
}

void FilledArrowLineTerminator::generate(EntityHeader *header,DimensionCallBack dcb,void *data,CoordinateSystem *cs,int orient,double x,double y)
{Point p1(x,y,0.0),p2(x-orient*getlength(),y+getheight()/2.0,0.0),p3(x-orient*getlength(),y-getheight()/2.0,0.0);
 EntityList list;
 BitMask options(32);
  p1 = cs->tomodel(p1);
  p2 = cs->tomodel(p2);
  p3 = cs->tomodel(p3);
  Line *line1 = new Line(*header,p1,p2);  
  if (getweight() != 255) line1->setweight(getweight());
  dcb(line1,data);
  Line *line2 = new Line(*header,p2,p3);  
  if (getweight() != 255) line2->setweight(getweight());
  dcb(line2,data);
  Line *line3 = new Line(*header,p3,p1);  
  if (getweight() != 255) line3->setweight(getweight());
  dcb(line3,data);
  list.add(line1);  list.add(line2);  list.add(line3);
  Plane plane(*header,list,fabs(getlength()/50.0),fabs(getheight()/50.0),45.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),options);
  if (getweight() != 255) plane.setweight(getweight());
  dcb(&plane,data);
}

void DotLineTerminator::generate(EntityHeader *header,DimensionCallBack dcb,void *data,CoordinateSystem *cs,int,double x,double y)
{Point p1(x,y,0.0);
  Circle circle(*header,cs->tomodel(p1),cs->getxaxis(),cs->getyaxis(),getheight()/2.0,0.0,2.0 * M_PI);
  if (getweight() != 255) circle.setweight(getweight());
  dcb(&circle,data);
}

void FilledDotLineTerminator::generate(EntityHeader *header,DimensionCallBack dcb,void *data,CoordinateSystem *cs,int,double x,double y)
{Point p1(x,y,0.0);
 BitMask options(32);
 EntityList list;
  Circle *circle = new Circle(*header,cs->tomodel(p1),cs->getxaxis(),cs->getyaxis(),getheight()/2.0,0.0,2.0 * M_PI);
  if (getweight() != 255) circle->setweight(getweight());
  dcb(circle,data);  
  list.add(circle);
  Plane plane(*header,list,fabs(getheight()/30.0),fabs(getheight()/30.0),45.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),_RCT(""),options);
  if (getweight() != 255) plane.setweight(getweight());
  dcb(&plane,data);
}

void SlashLineTerminator::generate(EntityHeader *header,DimensionCallBack dcb,void *data,CoordinateSystem *cs,int,double x,double y)
{Point p1(x-getlength()/2.0,y-getheight()/2.0,0.0),p2(x+getlength()/2.0,y+getheight()/2.0,0.0);
  Line line1(*header,cs->tomodel(p1),cs->tomodel(p2));  
  if (getweight() != 255) line1.setweight(getweight());
  dcb(&line1,data);
}

class VerifySubDialogButtonDialogControl : public ButtonDialogControl
{private:
   Dialog *dialog;
 public:
   VerifySubDialogButtonDialogControl(int i,Dialog *d) :
       ButtonDialogControl(i) {  dialog = d;  }
   int select(Dialog *);
};

int VerifySubDialogButtonDialogControl::select(Dialog *parentdialog)
{ dialog->process(parentdialog->gethdlg());
  return 0;
}


//
//  Equation calculation functions
//

enum EquationSymbolType {  EQPlusSym,EQMinusSym,EQDivideSym,EQMultiplySym,EQLBracketSym,EQRBracketSym,
                           EQFunctionNameSym,EQVariableSym,EQEolSym,EQBadSym,EQRealConstantSym  
                        };

static const char *FunctionName[] =
  {  "abs","acos","asin","atan","floor","ceil","cos","exp","log",
     "log10","pow10","tan","sqrt","sin",
     0
  };

Parameter::Parameter(const RCCHAR *name_,double value_)
{
    //Parameter(CHAR2RCCHR(name_),value_);
    strcpy(name,name_);
    value = value_;
}

Parameter::Parameter(const char *name_,double value_)
{
  strcpy(name,name_);
  value = value_;
}

int Parameter::GetValue(const RCCHAR *name_,double *value_)
{ if (_stricmp(name,name_) == 0)
    {  *value_ = value;
       return 1;
    }
  else
    return 0;
}

StringExpression::StringExpression(const RCCHAR *l,GeneralList *dimensionvalues_)
{ line = l;
  dimensionvalues = dimensionvalues_;
  pos = 0;
  nextsymbol();
}

void StringExpression::nextsymbol(void)
{RCCHAR *eptr1;
 long i;
 Parameter *dv;

  symboltype = EQBadSym;
  while (line[pos] == ' ' || line[pos] == '\n' || line[pos] == '\r') pos++;
  if (line[pos] == 0)
    symboltype = EQEolSym;
  else if (line[pos] == '+')
    {  symboltype = EQPlusSym;  pos++;
    }
  else if (line[pos] == '-')
    {  symboltype = EQMinusSym;  pos++;
    }
  else if (line[pos] == '/')
    {  symboltype = EQDivideSym;  pos++;
    }
  else if (line[pos] == '*')
    {  symboltype = EQMultiplySym;  pos++;
    }
  else if (line[pos] == '(')
    {  symboltype = EQLBracketSym;  pos++;
    }
  else if (line[pos] == ')')
    {  symboltype = EQRBracketSym;   pos++;
    }
  else if (tolower(line[pos]) >= 'a' && tolower(line[pos]) <= 'z')
    {  i = 0;
       fname[i++] = (RCCHAR) tolower(line[pos++]);
       while (tolower(line[pos]) >= 'a' && tolower(line[pos]) <= 'z' ||
              line[pos] >= '0' && line[pos] <= '9' ||
              line[pos] == '_' || line[pos] == '$')
         {  if (i < 255)
              fname[i++] = (RCCHAR) tolower(line[pos++]);
            else
              pos++;
         }
       fname[i] = 0;
       for (i = 0 ; FunctionName[i] != 0 ; i++)
         if (strcmp(fname,FunctionName[i]) == 0)
           break;
       if (FunctionName[i] == 0)
         {  for (dimensionvalues->start() ; (dv = (Parameter *)dimensionvalues->next()) != 0 ; )
              if (dv->GetValue(fname,&xvalue))
                {  symboltype = EQRealConstantSym;
                   break;
                }
            if (dv == 0)  
              symboltype = EQBadSym;
         }
       else
         symboltype = EQFunctionNameSym;
    }
  else
    {  xvalue = strtod(line+pos,&eptr1);
       if (pos == eptr1 - line)
         symboltype = EQBadSym;
       else  
         {  symboltype = EQRealConstantSym;
            pos = eptr1 - line;
         }
    }
}

int StringExpression::expression(double *x)
{double sign,x1,x2;
 int op;

  sign = 1.0;

  if (symboltype == EQPlusSym)
    {  sign = 1.0;  nextsymbol();
    }
  else if (symboltype == EQMinusSym)
    {  sign = -1.0;  nextsymbol();
    }

  if (! term(&x1))
    return 0;
  x1 *= sign;
  while (symboltype == EQPlusSym || symboltype == EQMinusSym)
    {  op = symboltype;
       nextsymbol();
       if (! term(&x2))
         return 0;
       if (op == EQPlusSym)
         x1 += x2;
       else
         x1 -= x2;
    }
  *x = x1;
  return 1;
}

int StringExpression::term(double *x)
{double x1,x2;
 int op;

  if (! factor(&x1))
    return 0;
  while (symboltype == EQDivideSym || symboltype == EQMultiplySym)
    {  op = symboltype;
       nextsymbol();
       if (! factor(&x2))
         return 0;
       if (op == EQDivideSym)
         {  if (fabs(x2) < 1.0E-10)
              return 0;
            x1 /= x2;
         }
       else
         x1 *= x2;
    }
  *x = x1;
  return 1;
}

int StringExpression::factor(double *x)
{double x1;
 
  if (symboltype == EQRealConstantSym)
    {  *x = xvalue;  
       nextsymbol();
    }
  else if (symboltype == EQFunctionNameSym)
    {  nextsymbol();
       if (symboltype != EQLBracketSym)
         return 0;
       nextsymbol();
       if (! expression(&x1))
         return 0;
       if (strcmp(fname,"abs") == 0)
         *x = fabs(xvalue);
       else if (strcmp(fname,"acos") == 0)
         {  if (x1 < -1.0 || x1 > 1.0)
              return 0;
            *x = acos(x1) * 180.0 / acos(-1.0);
         }
       else if (strcmp(fname,"asin") == 0)
         {  if (x1 < -1.0 || x1 > 1.0)
              return 0;
            *x = asin(x1) * 180.0 / acos(-1.0);
         }
       else if (strcmp(fname,"atan") == 0)
         *x = atan(x1) * 180.0 / acos(-1.0);
       else if (strcmp(fname,"ceil") == 0)
         *x = ceil(x1);
       else if (strcmp(fname,"floor") == 0)
         *x = floor(x1);
       else if (strcmp(fname,"cos") == 0)
         *x = cos(x1 / 180.0 * acos(-1.0));
       else if (strcmp(fname,"exp") == 0)
         {  if (x1 > log(1E300))
              return 0;
            *x = exp(x1);
         }
       else if (strcmp(fname,"log") == 0)
         {  if (x1 <= 0.0)
              return 0;
            *x = log(x1);
         }
       else if (strcmp(fname,"log10") == 0)
         {  if (x1 <= 0.0)
              return 0;
            *x = log10(x1);
         }
       else if (strcmp(fname,"pow10") == 0)
         {  if (x1 > log10(1E300))
              return 0;
            *x = exp(log(10.0)*x1);
         }
       else if (strcmp(fname,"tan") == 0)
         {  x1 *= acos(-1.0) / 180.0;
            if (fabs(fmod(x1 - acos(-1.0) / 2.0,acos(-1.0))) < 1.0E-10)
              return 0;
            *x = tan(x1);
         }
       else if (strcmp(fname,"sqrt") == 0)
         {  if (x1 < 0.0)
              return 0;
            *x = sqrt(x1);
         }
       else if (strcmp(fname,"sin") == 0)
         *x = sin(x1 / 180.0 * acos(-1.0));
    }
  else if (symboltype == EQLBracketSym)
    {  nextsymbol();
       if (! expression(x))
         return 0;
       if (symboltype != EQRBracketSym)
         return 0;
       nextsymbol();
    }
  else
    return 0;

  return 1;
}

void StretchDimension(Dimension *dimension,int index,double length,EntityList *list)
{int i;
 Entity *e;
 Point p,origin,direction;
 SortedPointTable sptable;
 Transform t;
 EntityList movedlist;
 double currentlength;


  if (index < 0)
    index = 0;

  if (length > 0)
    {  origin = dimension->point(index+2);
       direction = dimension->getxaxis();
       currentlength = (dimension->point(index+2) - dimension->point(index+1)).dot(dimension->getxaxis());
    }
  else
    {  origin = dimension->point(index+1);
       direction = dimension->getxaxis();
       currentlength = -(dimension->point(index+2) - dimension->point(index+1)).dot(dimension->getxaxis());
    }

  db.saveundo(UD_STARTBLOCK,NULL);

  for (list->start() ; (e = list->next()) != NULL ; )
    {  e->setstatus(0);
       e->setcopye(0);
       for (i = 0 ; i < e->npoints() ; i++)
         {  p = e->point(i);
            if (fabs((p - origin).dot(direction)) < db.getptoler())
              sptable.add(p);
         }
    } 
  sptable.endlist();

  t.translate(direction * (length - currentlength));
  for (list->start() ; (e = list->next()) != NULL ; )
    e->stretch(&db,t,&sptable,1);
  for (list->start() ; (e = list->next()) != NULL ; )
    if ((e->getstatus() & 4) == 0)   //  An original entity
      {  if (e != e->getcopye() && e->getcopye() != NULL && e->getalist().empty())
           {AttributeList elist;
              e->getcopye()->substituteat(e->getatlist().copy());
              e->substituteat(elist);
              db.geometry.del(e);
           }
      }
  for (list->start() ; (e = list->next()) != NULL ; )
    if ((e->getstatus() & 4) == 0 && (e->getstatus() & ~8))   //  Original entity has been processed during stretch
      {  movedlist.add(e);
         e->draw(DM_NORMAL);
      }
    else if (e->getstatus() & ~8)
      e->draw(DM_NORMAL);
  db.saveundo(UD_MANYMOVED,(Entity *)&movedlist);
  movedlist.destroy();
  db.saveundo(UD_ENDBLOCK,NULL);

}     

#ifdef _USING_QTCHAR
const RCCHAR *stristr(const RCCHAR *s1,const RCCHAR *s2)
{int i;
 RCCHAR s3[300];
  for (i = 0 ; i <= (signed)(strlen(s1) - strlen(s2)) ; i++)
    {  strncpy(s3,s1+i,strlen(s2));
       s3[strlen(s2)] = 0;
       if (_stricmp(s2,s3) == 0)
         return s1 + i;
    }
  return 0;
}
#endif

const char *stristr(const char *s1,const char *s2)
{int i;
 char s3[300];
  for (i = 0 ; i <= (signed)(strlen(s1) - strlen(s2)) ; i++)
    {  strncpy(s3,s1+i,strlen(s2));
       s3[strlen(s2)] = 0;
       if (_stricmp(s2,s3) == 0)
         return s1 + i;
    }
  return 0;
}

extern BitMask * GetLockedLayerTable(void);

class LeaderVerifyButtonControl : public ButtonDialogControl
  {public:
     LeaderVerifyButtonControl(int id) : ButtonDialogControl(id) {  }
     int select(Dialog *dialog);
  };


int LeaderVerifyButtonControl::select(Dialog *dialog)
{
    int state,colour,halocolour,layer,style,iweight;
    double weight,termweight;

    state = 1001;
    if (id == 1000)
    { // colour
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(100))->currentvalue(dialog,&colour);
        general_property_command(&state,0,(void **)&colour);
        ((IntegerDialogControl *)dialog->getcontrol(100))->change(dialog,colour);
    }
    else if(id == 1001)
    { // layer
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
        //layer = db.header.getlayer();
        ((IntegerDialogControl *)dialog->getcontrol(101))->currentvalue(dialog,&layer);
        general_property_command(&state,0,(void **)&layer);
        ((IntegerDialogControl *)dialog->getcontrol(101))->change(dialog,layer);
    }
    else if(id == 1002)
    { // style
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
        //style = db.header.getstyle();
        ((IntegerDialogControl *)dialog->getcontrol(102))->currentvalue(dialog,&style);
        general_property_command(&state,0,(void **)&style);
        ((IntegerDialogControl *)dialog->getcontrol(102))->change(dialog,style);
    }
    else if(id == 1003)
    { // weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
        //iweight = db.header.getweight();
        //weight = db.lineweights.weighttomm(iweight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->currentvalue(dialog,&weight);
        general_property_command(&state,0,(void **)&weight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(103))->change(dialog,weight);
    }
    else if(id == 1004)
    { // halo colour
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
        ((IntegerDialogControl *)dialog->getcontrol(116))->currentvalue(dialog,&halocolour);
        general_property_command(&state,0,(void **)&halocolour);
        ((IntegerDialogControl *)dialog->getcontrol(116))->change(dialog,halocolour);
    }
    else if(id == 1005)
    { // terminator weight
        WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
        ((WeightAssocRealDialogControl *)dialog->getcontrol(122))->currentvalue(dialog,&termweight);
        general_property_command(&state,0,(void **)&termweight);
        ((WeightAssocRealDialogControl *)dialog->getcontrol(122))->change(dialog,termweight);
    }
    return 0;
}

void Dimension::verify(void)
{

#ifdef USING_WIDGETS
 if (isa(circulardimension_entity) && ((CircularDimension *)this)->getcircle() == 0)
 {
     ::state.sendevent(new NewCommandEvent(424,0));
     return;
 }
 else
 {
     ::state.sendevent(new NewCommandEvent(415,0));
     return;
 }
#else
 ResourceString rs7(NCDIMENA+7),rs8(NCDIMENA+8),rs9(NCDIMENA+9);
 ResourceString rs10(NCDIMENA+10),rs11(NCDIMENA+11),rs12(NCDIMENA+12);
 ResourceString rs13(NCDIMENA+13),rs14(NCDIMENA+14),rs15(NCDIMENA+15);
 ResourceString rs16(NCDIMENA+16),rs17(NCDIMENA+17);
 ResourceString rs40(NCDIMENA+40),rs41(NCDIMENA+41),rs42(NCDIMENA+42);
 ResourceString rs43(NCDIMENA+43),rs44(NCDIMENA+44),rs45(NCDIMENA+45),rs46(NCDIMENA+46);
 RCCHAR *hjustl[] = {  rs7.getws(),rs8.getws(),rs9.getws()  };
 RCCHAR *vjustl[] = {  rs10.getws(),rs11.getws(),rs12.getws()  };
 RCCHAR *tolerancel[] = {  rs13.getws(),rs14.getws(),rs15.getws(),rs16.getws()  };
 RCCHAR *placementl[] = {  rs40.getws(),rs41.getws(),rs42.getws()  };
 RCCHAR *positionl[] = {  rs43.getws(),rs44.getws()  };
 RCCHAR *visiblel[] = {  rs45.getws(),rs46.getws()  };
 Dialog dialog("Dimensionverify_Dialog");
 Dialog termdialog("Dimensionverifyterminator_dialog");
 Dialog textdialog1("Dimensionverifyvalue_dialog");
 Dialog textdialog2("Dimensionverifyprefix_dialog");
 Dialog textdialog3("Dimensionverifyprefix_dialog");
 Dialog textdialog4("Dimensionverifyprefix_dialog");
 Dialog leaderverify("LeaderVerify_dialog");
 RCCHAR tw1[300],tw2[300];

 int c,l,s,hjust,vjust,tolerance,nterms,placement,status,hc,useweight1,termstyle,nfonts,fontid;
 double w,length,valueoffset,weight1;
 TerminatorInfo terminfo;
 RCCHAR **termnames=0;
 EntityList stretchlist;
 RCCHAR **fonts=0,font[300];
 Byte *charsets=0;
 BitMask options1(32);

  for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++);
  nterms = terminfo.style;
  if ((termnames = new RCCHAR *[nterms]) == NULL) return;
  for (terminfo.style = 0 ; LineTerminator::name(terminfo) != NULL ; terminfo.style++)
    termnames[terminfo.style] = LineTerminator::name(terminfo);

  if (isa(circulardimension_entity) && ((CircularDimension *)this)->getcircle() == 0)
    {
       options.toggle(ScaleText);
       //options1.clearall();
       //options1.set(ScaleText,options.test(ScaleText));

       hjust = prefixinfo.options.test(1) + prefixinfo.options.test(2) * 2;
       vjust = prefixinfo.options.test(3) + prefixinfo.options.test(4) * 2;
       hc = prefixinfo.halocolour;

       useweight1 = (term1.style & 0x80) != 0;
       weight1 = db.lineweights.weighttomm(term1.weight);
       termstyle = term1.style & 0x7F;

       EntityHeader::verify(&leaderverify,&c,&l,&s,&w);

       strcpy(font,prefixinfo.font);
       if (! Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid)) return ;

       leaderverify.add(new LeaderVerifyButtonControl(1000));
       leaderverify.add(new LeaderVerifyButtonControl(1001));
       leaderverify.add(new LeaderVerifyButtonControl(1002));
       leaderverify.add(new LeaderVerifyButtonControl(1003));
       leaderverify.add(new LeaderVerifyButtonControl(1004));
       leaderverify.add(new LeaderVerifyButtonControl(1005));

       leaderverify.add(new FontListDialogControl(1033,nfonts,fonts,&fontid));
       //leaderverify.add(new StringDialogControl(1033,prefixinfo.font,strlen(prefixinfo.font) + 1));
       leaderverify.add(new RealDialogControl(105,&prefixinfo.height,ScaleByLength));
       leaderverify.add(new RealDialogControl(106,&prefixinfo.width,ScaleByLength));
       leaderverify.add(new RealDialogControl(109,&prefixinfo.angle));
       //leaderverify.add(new StringDialogControl(107,hjustl[hjust],strlen(hjustl[hjust])+1));
       //leaderverify.add(new StringDialogControl(108,vjustl[vjust],strlen(vjustl[vjust])+1));
       leaderverify.add(new ListDialogControl(104,3,hjustl,&hjust));
       leaderverify.add(new ListDialogControl(108,3,vjustl,&vjust));
       leaderverify.add(new RealDialogControl(107,&prefixinfo.vertspacing));
       leaderverify.add(new IntegerDialogControl(116,&hc));
       leaderverify.add(new CheckBoxDialogControl(110,&prefixinfo.options,5));// bold
       leaderverify.add(new CheckBoxDialogControl(111,&prefixinfo.options,6));// underline
       leaderverify.add(new CheckBoxDialogControl(113,&prefixinfo.options,7));// italic
       leaderverify.add(new CheckBoxDialogControl(114,&prefixinfo.options,8));// boxed
       leaderverify.add(new CheckBoxDialogControl(112,&prefixinfo.options,10));// outlined
       leaderverify.add(new CheckBoxDialogControl(115,&prefixinfo.options,12));// halo
       leaderverify.add(new CheckBoxDialogControl(121,&options,ScaleText));// scale text
       leaderverify.add(new ListDialogControl(117,nterms,termnames,&termstyle));
       //leaderverify.add(new StringDialogControl(117,termnames[term1.style&0x7f],strlen(termnames[term1.style&0x7f])+1));
       leaderverify.add(new RealDialogControl(118,&term1.length,ScaleByLength));
       leaderverify.add(new RealDialogControl(120,&term1.height,ScaleByLength));
       leaderverify.add(new CheckBoxDialogControl(119,&useweight1));// specify terminator weight
       // change button not used
       // change button for testing remove later
       if (GetLockedLayerTable() == 0 || GetLockedLayerTable()->test(getlayer()) == 0)
            leaderverify.add(new ButtonDialogControl(2000));
       //  leaderverify.add(new ButtonDialogControl(121));
       //sprintf(tw1,"%.2lf",db.lineweights.weighttomm(term1.weight));
       //RCSPRINTF(tw1,"%.2lf",db.lineweights.weighttomm(term1.weight));
       //if ((term1.style&0x80) == 0)
       //  strcpy(tw1,"Default");
       leaderverify.add(new WeightAssocRealDialogControl(122,&weight1));


       leaderverify.setposition(DPOK);
       status = leaderverify.process();
       if(status == TRUE)
       {
           db.saveundo(UD_CHANGEATTRIBUTES,this);
           if (getposition() != 0)
               draw(DM_ERASE);
           cadwindow->invalidatedisplaylist(this);

           options.toggle(ScaleText);
           //options.set(ScaleText,options1.test(ScaleText));

           RCSTRCPY(prefixinfo.font,fonts[fontid]);

           prefixinfo.halocolour = hc;

           term1.style = Byte(termstyle | (0x80 * useweight1));
           term1.weight = db.lineweights.mmtoweight(weight1);

           prefixinfo.options.set(1,hjust % 2);
           prefixinfo.options.set(2,hjust / 2);
           prefixinfo.options.set(3,vjust % 2);
           prefixinfo.options.set(4,vjust / 2);

           colour = c;
           style = s;
           layer = l;
           weight = db.lineweights.mmtoweight(w);

           if (getposition() != 0)
             draw(DM_NORMAL);
       }
       else if (status == 2000) // change button not used here
         ::state.sendevent(new NewCommandEvent(424,0));

       for (int i = 0 ; i < nfonts ; i++) delete [] fonts[i];
       delete [] fonts;

       return;
    }

  options.toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
  options.toggle(ScaleText); 
  EntityHeader::verify(&dialog,&c,&l,&s,&w);
  dialog.add(new RealDialogControl(104,&extension,ScaleByLength));
  dialog.add(new RealDialogControl(105,&gap,ScaleByLength));
  dialog.add(new IntegerDialogControl(106,&precision));
  dialog.add(new CheckBoxDialogControl(107,&options,0));
  dialog.add(new CheckBoxDialogControl(108,&options,1));
  dialog.add(new RealDialogControl(109,&stackdis,ScaleByLength));
  tolerance = options.test(5) + options.test(6) * 2;
  dialog.add(new StringDialogControl(110,tolerancel[tolerance],strlen(tolerancel[tolerance])+1));
  dialog.add(new RealDialogControl(111,&uppertol,ScaleByLength));
  dialog.add(new RealDialogControl(112,&lowertol,ScaleByLength));
  dialog.add(new VerifySubDialogButtonDialogControl(113,&termdialog));
  dialog.add(new VerifySubDialogButtonDialogControl(114,&textdialog1));
  dialog.add(new VerifySubDialogButtonDialogControl(115,&textdialog3));
  dialog.add(new VerifySubDialogButtonDialogControl(116,&textdialog4));
  if (GetLockedLayerTable() != 0 && GetLockedLayerTable()->test(getlayer()) == 0)
    {  dialog.add(new ButtonDialogControl(117));
       dialog.add(new ButtonDialogControl(123));
       dialog.add(new ButtonDialogControl(124));
       dialog.add(new ButtonDialogControl(125));
       dialog.add(new ButtonDialogControl(126));
       dialog.add(new ButtonDialogControl(129));
       dialog.add(new ButtonDialogControl(133));
    }
  dialog.add(new CheckBoxDialogControl(118,&options,ToleranceDefined));
  dialog.add(new CheckBoxDialogControl(120,&options,GapFromDimension));
  dialog.add(new VerifySubDialogButtonDialogControl(119,&textdialog2));
  dialog.add(new RealDialogControl(121,&scale));
  dialog.add(new RealDialogControl(128,&textyoffset));
  dialog.add(new CheckBoxDialogControl(122,&options,ScaleText));
  dialog.add(new CheckBoxDialogControl(127,&options,FixedDimensionPosition));
  dialog.add(new CheckBoxDialogControl(130,&options,DimensionReverse));
  dialog.add(new CheckBoxDialogControl(131,&options,HideLeftDimensionLine));
  dialog.add(new CheckBoxDialogControl(132,&options,HideRightDimensionLine));

  termdialog.add(new StringDialogControl(100,termnames[term1.style&0x7f],strlen(termnames[term1.style&0x7f])+1));
  termdialog.add(new RealDialogControl(101,&term1.length,ScaleByLength));
  termdialog.add(new RealDialogControl(102,&term1.height,ScaleByLength));
  termdialog.add(new StringDialogControl(103,termnames[term2.style&0x7f],strlen(termnames[term2.style&0x7f])+1));
  termdialog.add(new RealDialogControl(104,&term2.length,ScaleByLength));
  termdialog.add(new RealDialogControl(105,&term2.height,ScaleByLength));
  termdialog.add(new StringDialogControl(106,positionl[options.test(9)],strlen(positionl[options.test(9)])+1));
  termdialog.add(new StringDialogControl(107,visiblel[options.test(10)],strlen(visiblel[options.test(10)])+1));

  RCSPRINTF(tw1,"%.2lf",db.lineweights.weighttomm(term1.weight));
  if ((term1.style&0x80) == 0)
    RCSTRCPY(tw1,"Default");
  RCSPRINTF(tw2,"%.2lf",db.lineweights.weighttomm(term2.weight));
  if ((term2.style&0x80) == 0)
    RCSTRCPY(tw2,"Default");
  termdialog.add(new StringDialogControl(108,tw1,300));
  termdialog.add(new StringDialogControl(110,tw2,300));

  hjust = valueinfo.options.test(1) + valueinfo.options.test(2) * 2;
  vjust = valueinfo.options.test(3) + valueinfo.options.test(4) * 2;
  placement = options.test(7) + options.test(8) * 2;
  textdialog1.add(new StringDialogControl(100,valueinfo.font,strlen(valueinfo.font) + 1));
  textdialog1.add(new RealDialogControl(101,&valueinfo.height,ScaleByLength));
  textdialog1.add(new RealDialogControl(102,&valueinfo.width,ScaleByLength));
  textdialog1.add(new RealDialogControl(103,&valueinfo.angle));
  textdialog1.add(new StringDialogControl(104,hjustl[hjust],strlen(hjustl[hjust])+1));
  textdialog1.add(new StringDialogControl(105,vjustl[vjust],strlen(vjustl[vjust])+1));
  textdialog1.add(new RealDialogControl(106,&valueinfo.vertspacing));
  textdialog1.add(new CheckBoxDialogControl(107,&valueinfo.options,5));
  textdialog1.add(new CheckBoxDialogControl(108,&valueinfo.options,6));
  textdialog1.add(new CheckBoxDialogControl(109,&valueinfo.options,7));
  textdialog1.add(new CheckBoxDialogControl(110,&valueinfo.options,8));
  textdialog1.add(new CheckBoxDialogControl(111,&options,2));
  textdialog1.add(new ListDialogControl(112,3,placementl,&placement));

  hjust = toleranceinfo.options.test(1) + toleranceinfo.options.test(2) * 2;
  vjust = toleranceinfo.options.test(3) + toleranceinfo.options.test(4) * 2;
  textdialog2.label(_RCT(""));
  textdialog2.add(new StringDialogControl(101,toleranceinfo.text,strlen(toleranceinfo.text) + 1));
  textdialog2.add(new CheckBoxDialogControl(102,&options,ToleranceDefined));
  textdialog2.add(new StringDialogControl(103,toleranceinfo.font,strlen(toleranceinfo.font) + 1));
  textdialog2.add(new RealDialogControl(104,&toleranceinfo.height,ScaleByLength));
  textdialog2.add(new RealDialogControl(105,&toleranceinfo.width,ScaleByLength));
  textdialog2.add(new RealDialogControl(106,&toleranceinfo.angle));
  textdialog2.add(new StringDialogControl(107,hjustl[hjust],strlen(hjustl[hjust])+1));
  textdialog2.add(new StringDialogControl(108,vjustl[vjust],strlen(vjustl[vjust])+1));
  textdialog2.add(new RealDialogControl(109,&toleranceinfo.vertspacing));
  textdialog2.add(new CheckBoxDialogControl(110,&toleranceinfo.options,5));
  textdialog2.add(new CheckBoxDialogControl(111,&toleranceinfo.options,6));
  textdialog2.add(new CheckBoxDialogControl(112,&toleranceinfo.options,7));
  textdialog2.add(new CheckBoxDialogControl(113,&toleranceinfo.options,8));

  hjust = prefixinfo.options.test(1) + prefixinfo.options.test(2) * 2;
  vjust = prefixinfo.options.test(3) + prefixinfo.options.test(4) * 2;
  ResourceString rs24(NCDIMENA+24);
  textdialog3.title(rs24.gets());
  ResourceString rs25(NCDIMENA+25);
  textdialog3.label(rs25.gets());
  textdialog3.add(new StringDialogControl(101,prefixinfo.text,strlen(prefixinfo.text) + 1));
  textdialog3.add(new CheckBoxDialogControl(102,&options,3));
  textdialog3.add(new StringDialogControl(103,prefixinfo.font,strlen(prefixinfo.font) + 1));
  textdialog3.add(new RealDialogControl(104,&prefixinfo.height,ScaleByLength));
  textdialog3.add(new RealDialogControl(105,&prefixinfo.width,ScaleByLength));
  textdialog3.add(new RealDialogControl(106,&prefixinfo.angle));
  textdialog3.add(new StringDialogControl(107,hjustl[hjust],strlen(hjustl[hjust])+1));
  textdialog3.add(new StringDialogControl(108,vjustl[vjust],strlen(vjustl[vjust])+1));
  textdialog3.add(new RealDialogControl(109,&prefixinfo.vertspacing));
  textdialog3.add(new CheckBoxDialogControl(110,&prefixinfo.options,5));
  textdialog3.add(new CheckBoxDialogControl(111,&prefixinfo.options,6));
  textdialog3.add(new CheckBoxDialogControl(112,&prefixinfo.options,7));
  textdialog3.add(new CheckBoxDialogControl(113,&prefixinfo.options,8));

  hjust = suffixinfo.options.test(1) + suffixinfo.options.test(2) * 2;
  vjust = suffixinfo.options.test(3) + suffixinfo.options.test(4) * 2;
  ResourceString rs26(NCDIMENA+26);
  textdialog4.title(rs26.gets());
  ResourceString rs27(NCDIMENA+27);
  textdialog4.label(rs27.gets());
  textdialog4.add(new StringDialogControl(101,suffixinfo.text,strlen(suffixinfo.text) + 1));
  textdialog4.add(new CheckBoxDialogControl(102,&options,4));
  textdialog4.add(new StringDialogControl(103,suffixinfo.font,strlen(suffixinfo.font) + 1));
  textdialog4.add(new RealDialogControl(104,&suffixinfo.height,ScaleByLength));
  textdialog4.add(new RealDialogControl(105,&suffixinfo.width,ScaleByLength));
  textdialog4.add(new RealDialogControl(106,&suffixinfo.angle));
  textdialog4.add(new StringDialogControl(107,hjustl[hjust],strlen(hjustl[hjust])+1));
  textdialog4.add(new StringDialogControl(108,vjustl[vjust],strlen(vjustl[vjust])+1));
  textdialog4.add(new RealDialogControl(109,&suffixinfo.vertspacing));
  textdialog4.add(new CheckBoxDialogControl(110,&suffixinfo.options,5));
  textdialog4.add(new CheckBoxDialogControl(111,&suffixinfo.options,6));
  textdialog4.add(new CheckBoxDialogControl(112,&suffixinfo.options,7));
  textdialog4.add(new CheckBoxDialogControl(113,&suffixinfo.options,8));

  dialog.setposition(DPOK);

  status = dialog.process();
  options.toggle(ToleranceDefined);  //  Need to display 'Use Value Style', not use tolerance style
  options.toggle(ScaleText);
  if (status == 117)
    ::state.sendevent(new NewCommandEvent(415,0));
  else if (status == 123)
    {Dialog dialog(_RCT("Change Length"),_RCT("Length"),&length,ScaleByLength);
     GeneralList dimensionvalues;
     Parameter *dv;
     Entity *e;
     double x;
     int index;

       index = state.getlastend();
       if (index < 0)
         index = 0;

       length = (point(index+2) - point(index+1)).dot(getxaxis());

       //  Make a list of all the dimensions that have a zero length equation that is a simple variable name.
       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->isdimension() && strpbrk(((Dimension *)e)->getequation()," +-/*()") == 0 && strlen(((Dimension *)e)->getequation()) > 0)
           dimensionvalues.add(new Parameter(((Dimension *)e)->getequation(),(((Dimension *)e)->point(2) - ((Dimension *)e)->point(1)).dot(((Dimension *)e)->cs.getxaxis())));

       StringExpression expr(equation,&dimensionvalues);
       if (expr.expression(&x))
         length = x;

       if (dialog.process())
         {  StretchDimension(this,state.getlastend(),length,db.geometry.getlist());
            if (strpbrk(getequation()," +-/*()") == 0 && strlen(getequation()) > 0)
              {//  Stretch any other dimensions that have this variable in it.
                 dimensionvalues.add(new Parameter(getequation(),(point(2) - point(1)).dot(getxaxis())));
                 for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
                   if (e->isdimension() && stristr(((Dimension *)e)->getequation(),equation) != 0)
                     stretchlist.add(e);

                 for (stretchlist.start() ; (e = stretchlist.next()) != 0 ; )
                   {StringExpression expr(((Dimension *)e)->getequation(),&dimensionvalues);
                      if (expr.expression(&x))
                        StretchDimension((Dimension *)e,0,x,db.geometry.getlist());
                   }

                 stretchlist.destroy();

              }
         } 

       for (dimensionvalues.start() ; (dv = (Parameter *)dimensionvalues.next()) != 0 ; )
         delete dv;
       dimensionvalues.destroy();

       state.destroyselection(1);

    }
  else if (status == 133)
    {Dialog dialog(_RCT("Change Offset"),_RCT("Offset"),&valueoffset,ScaleByLength);
     GeneralList dimensionvalues;
     int index;

       index = state.getlastend();
       if (index < 0)
         index = 0;

       valueoffset = getoffset(index);

       if (dialog.process())
         {  draw(DM_ERASE); 
            setoffset(index,valueoffset);
            cadwindow->invalidatedisplaylist(this);
            draw(DM_NORMAL); 
         }

    }
  else if (status == 124)
    {RCCHAR newequation[500];
     Dialog dialog(_RCT("Change Equation"),_RCT("Equation"),newequation,sizeof(newequation));
       strcpy(newequation,equation);
       if (dialog.process())
         {  delete equation;
            equation = new RCCHAR[strlen(newequation)+1];
            strcpy(equation,newequation);
         }
    }
  else if (status == 125)
    state.sendevent(new NewCommandEvent(1812,0));
  else if (status == 126)
    state.sendevent(new NewCommandEvent(1813,0));
  else if (status == 129)
    state.sendevent(new NewCommandEvent(1814,0));
  delete termnames;
#endif
}

struct LDExtentsCB_data
  {View3dSurface *v;
   Point *pmin,*pmax;
   int c;
  };

int ldextentscb(Entity *entity,void *data)
{LDExtentsCB_data *lddata = (LDExtentsCB_data *) data;
 Point pmin,pmax;
  entity->extents(lddata->v,&pmin,&pmax);
  if (lddata->c == 0)
    {  *lddata->pmin = pmin;
       *lddata->pmax = pmax;
    }
  if (pmin.x < lddata->pmin->x) lddata->pmin->x = pmin.x;
  if (pmin.y < lddata->pmin->y) lddata->pmin->y = pmin.y;
  if (pmin.z < lddata->pmin->z) lddata->pmin->z = pmin.z;
  if (pmax.x > lddata->pmax->x) lddata->pmax->x = pmax.x;
  if (pmax.y > lddata->pmax->y) lddata->pmax->y = pmax.y;
  if (pmax.z > lddata->pmax->z) lddata->pmax->z = pmax.z;
  (lddata->c)++;
  return 1;
}

void Dimension::extents(View3dSurface *v,Point *pmin,Point *pmax)
{LDExtentsCB_data lddata;
  lddata.v = v;
  lddata.pmin = pmin;
  lddata.pmax = pmax;
  lddata.c = 0;
  generate(v,ldextentscb,&lddata);
}

int Dimension::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(lineardimension_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

AngularDimension::AngularDimension(Point org,Point xa,Point ya,Point aorg,PointList l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o)  
{ origin = aorg;  list = l;  cs.set(org,xa,ya);
}

AngularDimension::AngularDimension(const EntityHeader &header,Point org,Point xa,Point ya,Point aorg,PointList l,double e,double g,double s,double ut,double lt,double sc,double tyo,int p,int tp,const TerminatorInfo &t1,const TerminatorInfo &t2,const TextInfo &ti1,const TextInfo &ti2,const TextInfo &ti3,const TextInfo &ti4,const BitMask &o) :
  Dimension(header,e,g,s,ut,lt,sc,tyo,p,tp,t1,t2,ti1,ti2,ti3,ti4,o)
{ origin = aorg;  list = l;  cs.set(org,xa,ya);
}

AngularDimension::~AngularDimension()
{ list.destroy();
}

void AngularDimension::generate(View3dSurface *v,DimensionCallBack dcb,void *data)
{Point p1,p2,p3,p4,p5,p6,sp1,sp2,dir1,dir2,xaxis,yaxis,zaxis,startp,startp5,startdir,porigin;
 int i,j,jmin,n,common,stacked,hjust;
 double x,*r,*a,a1,r1,r2,plen,totallen,vlen,slen,tlen,xo,yo,hgt,sina,cosa,aoffset,xoffset,plotscale;
 RCCHAR string[300],tolerancestring[300];
 LineTerminator *term;
 BitMask toptions(32);
 TextInfo *ti;
  common = options.test(0);
  stacked = options.test(1);  if (! common) stacked = 0;
  porigin = cs.frommodel(origin);  porigin.z = 0.0;
  plotscale =  v == 0 ? 1.0 : v->getplotscale();
  r1 = (cs.getorigin() - cs.tomodel(porigin)).length();
  zaxis = cs.getxaxis().cross(cs.getyaxis());
  r = new double[list.length()];
  a = new double[list.length()];
  if (r == NULL || a == NULL) return;

  qDebug() << "linear dim valueinfo : " << QString(valueinfo.text);
  qDebug() << "linear dim toleranceinfo : " << QString(toleranceinfo.text);
  qDebug() << "linear dim prefixinfo : " << QString(prefixinfo.text);
  qDebug() << "linear dim suffixinfo : " << QString(suffixinfo.text);

  for (list.start(),n = 0 ; ! list.atend() ; n++)
    {  p1 = cs.frommodel(list.next());  p1.z = 0.0;
       dir2 = (p1 - porigin);
       r[n] = dir2.length();
       if (r[n] < db.getptoler())
         a[n] = 0.0;
       else
         {  a[n] = acos(dir2.x / r[n]);
            if (dir2.y < 0.0) a[n] = 2.0 * M_PI - a[n];
         }   
       if (n > 0 && a[n] < a[0]) a[n] += 2.0 * M_PI;
    }
  for (i = 0 ; i < n ; i++)
    {  for (j = i+1,jmin = i ; j < n ; j++)
         if (a[j] < a[jmin]) jmin = j;
       x = a[i];  a[i] = a[jmin];  a[jmin] = x;
       x = r[i];  r[i] = r[jmin];  r[jmin] = x;
    }
  for (i = 0,textpointlist.start() ; i < n ; i++)
    {  cosa = cos(a[i]);  sina = sin(a[i]);
       r2 = r[i] + gap * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) * (r1 > r[i] ? 1.0 : -1.0);
       p3.setxyz(porigin.x + cosa * r2,porigin.y + sina * r2,0.0);
       p3 = cs.tomodel(p3);
       if (i == 0)
         {  r2 = r1 + extension * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale);
            if (stacked) r2 += stackdis * (list.length() - 2);
         }
       else
         r2 = r1 + (i - 1) * stackdis * stacked + extension * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale);
       p4.setxyz(porigin.x + cosa * r2,porigin.y + sina * r2,0.0);
       p4 = cs.tomodel(p4);
       r2 -= extension * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale);
       p5.setxyz(porigin.x + cosa * r2,porigin.y + sina * r2,0.0);
       p5 = cs.tomodel(p5);
       Line line((EntityHeader &)(*this),p3,p4);  

       if ((i != 0 || !options.test(HideLeftDimensionLine)) && 
           (i != n-1 || !options.test(HideRightDimensionLine)))
         dcb(&line,data);
       dir2 = (p4 - cs.tomodel(porigin)).normalize();
       if (i == 0)
         {  startp = p2;
            startp5 = p5;
            startdir = dir2;
         }
       else if (i > 0)
         {  Circle circle((EntityHeader &)(*this),origin,cs.getxaxis(),cs.getyaxis(),r2,a[i-1],a[i] - a[i-1]);
            r2 += valueinfo.height / 4.0 * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale);
            if (common)
              a1 = (a[i] + a[i-1]) / 2.0;
            else
              a1 = (a[i] + a[i-1]) / 2.0;

            if (common)
              formatvalue(string,tolerancestring,(a[i] - a[0]) * 180.0 / M_PI * db.getmainunitscale(),FormatNumberNeverShowUnits);
            else
              formatvalue(string,tolerancestring,(a[i] - a[i-1]) * 180.0 / M_PI * db.getmainunitscale(),FormatNumberNeverShowUnits);

            ti = (options.test(3) ? &valueinfo : &prefixinfo);  ti->options.set(9,! options.test(ScaleText));
            if (! Text::extents(v,ti->font,ti->options,prefixinfo.text,
                                ti->width,ti->height,ti->vertspacing,&plen,&hgt,&xo,&yo))
              plen = 0.0;
            valueinfo.options.set(9,! options.test(ScaleText));
            if (! options.test(2) || ! Text::extents(v,valueinfo.font,valueinfo.options,string,
                                valueinfo.width,valueinfo.height,valueinfo.vertspacing,&vlen,&hgt,&xo,&yo))
              vlen = 0.0;
            ti = (options.test(ToleranceDefined) ? &toleranceinfo : &valueinfo);  ti->options.set(9,! options.test(ScaleText));
            if (! options.test(2) || ! Text::extents(v,ti->font,ti->options,tolerancestring,
                                ti->width,ti->height,ti->vertspacing,&tlen,&hgt,&xo,&yo))
              tlen = 0.0;
            ti = (options.test(4) ? &valueinfo : &suffixinfo);  ti->options.set(9,! options.test(ScaleText));
            if (! Text::extents(v,ti->font,ti->options,suffixinfo.text,
                                ti->width,ti->height,ti->vertspacing,&slen,&hgt,&xo,&yo))
              slen = 0.0;
            totallen = plen + vlen + tlen + slen;

            if ((n != 2 || fabs(a[1] - a[0]) * r2 < totallen + term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || fabs(a[1] - a[0]) * r2 > totallen + 3.0 * term1.length * plotscale || options.test(7) || options.test(8)) && ! options.test(10))
              dcb(&circle,data);

            if ((n == 2 && fabs(a[1] - a[0]) * r2 < totallen + term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) && ! options.test(8)) || options.test(7)) //  Left text placement
              {  aoffset = a[1] - a1;
                 xoffset = - term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) * 1.5 - totallen  / 2.0;
              }
            else if (options.test(8))  //  Right text placement
              {  aoffset = a[0] - a1;
                 xoffset = term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) * 1.5 + totallen / 2.0;
              }
            else
              {  aoffset = 0.0;
                 xoffset = 0.0;
              }

            p6.setxyz(porigin.x + cos(a1+aoffset) * r2,porigin.y + sin(a1+aoffset) * r2,0.0);
            p6 = cs.tomodel(p6);
            yaxis = (p6 - cs.tomodel(porigin)).normalize();
            xaxis = yaxis.cross(zaxis);
            if (v != 0)
              {  sp1 = v->modeltoscreen(p6);  sp2 = v->modeltoscreen(p6+yaxis);
              }
            else
              {  sp1 = p6;  sp2 = p6 + yaxis;
              }
            if (sp1.y > sp2.y || fabs(sp1.y - sp2.y) < 0.001 && sp1.x < sp2.x)
              {  r2 += valueinfo.height;
                 p6.setxyz(porigin.x + cos(a1+aoffset) * r2,porigin.y + sin(a1+aoffset) * r2,0.0);
                 p6 = cs.tomodel(p6);
                 yaxis = -yaxis;
              }
            p6 += xaxis * xoffset;  
            if (v != 0)
              {  sp1 = v->modeltoscreen(p6);  sp2 = v->modeltoscreen(p6+xaxis);
              }
            else
              {  sp1 = p6;  sp2 = p6+xaxis;
              }
            if (v != 0 && xaxis.cross(yaxis).dot(v->getuaxis().cross(v->getvaxis())) < 0.0)
              xaxis = -xaxis;
            if (options.test(3))
              {  ti = &valueinfo;
                 hjust = 2;
              }
            else
              {  ti = &prefixinfo;
                 hjust = ti->options.test(1) + ti->options.test(2) * 2;
              }
            toptions = ti->options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
            if ((term = LineTerminator::create((v == 0 || options.test(ScaleText) ? 1.0 : plotscale),term1)) != NULL)
              {CoordinateSystem termcs(p5,dir2.cross(zaxis),dir2);
                 term->generate(this,dcb,data,&termcs,n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9) ? 1 : -1,0.0,0.0);
                 if (n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9))
                   {  Line line((EntityHeader &)(*this),termcs.tomodel(Point(0.0,0.0,0.0)),termcs.tomodel(Point((n == 2 && fabs(a[1] - a[0]) * r2 < totallen + term1.length * (v == 0 ? 1.0 : plotscale) && ! options.test(8)) || options.test(7) ? -term1.length*2.0 * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) - totallen : -term1.length*2.0 * plotscale,0.0,0.0)));
                      dcb(&line,data);
                   }
                 if (! common && i != n - 1 && ! term->symmetric())
                   term->generate(this,dcb,data,&termcs,n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9) ? -1 : 1,0.0,0.0);
                 else if (common && stacked && i != n - 1 && ! term->symmetric())
                   term->generate(this,dcb,data,&termcs,n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9) ? -1 : 1,0.0,stackdis);
                 delete term;
              }

            switch (hjust)
              {  case 0 : x = -totallen / 2.0;  break;
                 case 1 : x = -totallen / 2.0 + plen / 2.0;  break;
                 case 2 : x = -totallen / 2.0 + plen;  break;
              }

            if (! textpointlist.atend())
              p6 += textpointlist.next();

            if (strlen(prefixinfo.text) > 0)
              {Text text1((EntityHeader &)(*this),ti->font,p6+xaxis*x,xaxis,yaxis,
                               ti->width,ti->height,ti->vertspacing,ti->angle,0,prefixinfo.text,0,toptions);
                 dcb(&text1,data);
              }
            if (options.test(2))
              {  hjust = valueinfo.options.test(1) + valueinfo.options.test(2) * 2;
                 switch (hjust)
                   {  case 0 : x = -totallen / 2.0 + plen;  break;
                      case 1 : x = -totallen / 2.0 + plen + vlen / 2.0;  break;
                      case 2 : x = -totallen / 2.0 + plen + vlen;  break;
                   }
                 toptions = valueinfo.options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
                 if (strlen(string) > 0)
                   {Text text2((EntityHeader &)(*this),valueinfo.font,p6+xaxis*x,xaxis,yaxis,
                               valueinfo.width,valueinfo.height,valueinfo.vertspacing,valueinfo.angle,0,
                               string,0,toptions);
                      dcb(&text2,data);
                   }
                 if (options.test(ToleranceDefined))
                   {  ti = &toleranceinfo;
                      hjust = ti->options.test(1) + ti->options.test(2) * 2;
                   }
                 else
                   {  ti = &valueinfo;
                      hjust = 2;
                   }
                 toptions = ti->options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
                 switch (hjust)
                   {  case 0 : x = -totallen / 2.0 + plen + vlen;  break;
                      case 1 : x = -totallen / 2.0 + plen + vlen + tlen / 2.0;  break;
                      case 2 : x = -totallen / 2.0 + plen + vlen + tlen;  break;
                   }
                 if (strlen(tolerancestring) > 0)
                   {Text text3((const EntityHeader &)(*this),ti->font,p6+xaxis*x,xaxis,yaxis,
                                    ti->width,ti->height,ti->vertspacing,ti->angle,0,tolerancestring,0,toptions);
                      dcb(&text3,data); 
                   }
              }
            if (options.test(4))
              {  ti = &valueinfo;
                 hjust = 2;
              }
            else
              {  ti = &suffixinfo;
                 hjust = ti->options.test(1) + ti->options.test(2) * 2;
              }
            toptions = ti->options;  toptions.set(1,hjust % 2);  toptions.set(2,hjust / 2);  toptions.set(9,! options.test(ScaleText));
            switch (hjust)
              {  case 0 : x = -totallen / 2.0 + plen + vlen + tlen;  break;
                 case 1 : x = -totallen / 2.0 + plen + vlen + tlen + slen / 2.0;  break;
                 case 2 : x = -totallen / 2.0 + plen + vlen + tlen + slen;  break;
              }
            if (strlen(suffixinfo.text) > 0)
              {Text text4((const EntityHeader &)(*this),ti->font,p6+xaxis*x,xaxis,yaxis,
                               ti->width,ti->height,ti->vertspacing,ti->angle,0,suffixinfo.text,0,toptions);
                 dcb(&text4,data);
              }

            if (i == 1)
              {  if (common && ! stacked)
                   {CoordinateSystem termcs(startp5,startdir.cross(zaxis),startdir);
                      if ((term = LineTerminator::create(v == 0 || options.test(ScaleText) ? 1.0 : plotscale,term2)) != NULL)
                        {  term->generate(this,dcb,data,&termcs,n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9) ? -1 : 1,0.0,0.0);
                           delete term;
                        }
                      if (n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9))
                        {  Line line((EntityHeader &)(*this),termcs.tomodel(Point(0.0,0.0,0.0)),termcs.tomodel(Point(options.test(8) ? term1.length*2.0 * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) + totallen : term1.length*2.0 * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale),0.0,0.0)));
                           dcb(&line,data);
                        }
                   }
                 else if ((term = LineTerminator::create(v == 0 || options.test(ScaleText) ? 1.0 : plotscale,term1)) != NULL)
                   {CoordinateSystem termcs(startp5,startdir.cross(zaxis),startdir);
                      term->generate(this,dcb,data,&termcs,n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9) ? -1 : 1,0.0,0.0);
                      delete term;
                      if (n == 2 && fabs(a[1]-a[0])*r2 <= totallen + 3.0 * term1.length * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) || options.test(9))
                        {  Line line((EntityHeader &)(*this),termcs.tomodel(Point(0.0,0.0,0.0)),termcs.tomodel(Point(options.test(8) ? term1.length*2.0 * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale) + totallen : term1.length*2.0 * (v == 0 || options.test(ScaleText) ? 1.0 : plotscale),0.0,0.0)));
                           dcb(&line,data);
                        }
                   }
              }
         }
       p1 = p2;
       dir1 = dir2;
    }
  delete [] r;  delete [] a;
}

int AngularDimension::npoints(void)
{ return 1  + list.length();
}

Point AngularDimension::point(int n)
{int c;
 Point p,tmp(0.0,0.0,0.0);
  if (n == 0)
    return cs.getorigin();
  else
    {  for (list.start(),c = 1 ; ! list.atend() ; c++)
         {  p = list.next();  if (c == n) return p;
         }
    }
  return tmp;
}


Point AngularDimension::nearp(Point)
{ return cs.getorigin();
}

int AngularDimension::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{Entity *e,*c;
 int stretched;
 PointList newlist;
 Point p;
  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }
  stretched = sp->match(cs.getorigin());
  if (! stretched)
    {  for (list.start() ; ! stretched && ! list.atend() ; )
         stretched = sp->match(list.next());
    }
  if (! stretched) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 16) == 0)
      {  e->draw(DM_ERASE);  e->setstatus(e->getstatus() | 16);
      }
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      {Transform ident;
         ident.identity();
         if ((c = clone(ident)) != NULL)
           {  c->setstatus((status | 4) & ~8);
              if (! c->stretchent(db,t,sp,erase)) return 0;
              return db == 0 ? 1 : db->geometry.add(c);
           }
      }
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase && (status & 16) == 0) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 2 | 16;
  for (list.start() ; ! list.atend() ; )
    if (sp->match(p = list.next()))
      newlist.add(t.transform(p));
    else
      newlist.add(p);
  list.destroy();
  newlist.reverse();
  list = newlist;
  if (sp->match(cs.getorigin()))
    cs.change(t.transform(cs.getorigin()),cs.getxaxis(),cs.getyaxis());
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

int AngularDimension::move(Database *db,Transform &t,int erase,int save)
{Point zero(0.0,0.0,0.0);
 Entity *e;
 Point p,org,xaxis,yaxis,zaxis;
 PointList l;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return clone(t) != NULL;
  if (erase) draw(DM_ERASE);
  if (db != 0)
    { 
	  if(save)
		db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  status |= 1;
  org = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() > ::db.getptoler() && yaxis.length() > ::db.getptoler())
    cs.change(org,xaxis,yaxis);
  origin = t.transform(origin);
  for (list.start() ; ! list.atend() ; )
    l.add(t.transform(list.next()));
  l.reverse();
  list.destroy();
  list = l;
  copye = this;
  if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
    db->MovedCallback(this);
  return 1;
}

Entity *AngularDimension::clone(Transform &t)
{Point zero(0.0,0.0,0.0);
 Point p,org,xaxis,yaxis,zaxis;
 PointList l;
  if (copye != NULL) return copye;
  org = t.transform(cs.getorigin());
  xaxis = t.transform(cs.getxaxis()) - t.transform(zero);
  yaxis = t.transform(cs.getyaxis()) - t.transform(zero);
  zaxis = xaxis.cross(yaxis);
  yaxis = zaxis.cross(xaxis);
  xaxis.normalize();  yaxis.normalize();
  if (xaxis.length() < db.getptoler() || yaxis.length() < db.getptoler()) return 0;
  for (list.start() ; ! list.atend() ; )
    l.add(t.transform(list.next()));
  l.reverse();
  copye = new AngularDimension(*this,org,xaxis,yaxis,t.transform(origin),l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,
                              precision,tolprecision,term1,term2,valueinfo,toleranceinfo,prefixinfo,suffixinfo,options);
  if (! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  copye->cloneattributes(this);
  return copye;
}

int AngularDimension::project(Database *db,Transform &t,int)
{ return db->geometry.add(clone(t));
}

int AngularDimension::save(CadFile *outfile)
{short type;  
  type = angulardimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! cs.save(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! outfile->write(&gap)) return 0;
  if (! outfile->write(&extension)) return 0;
  if (! outfile->write(&stackdis)) return 0;
  if (! outfile->write(&uppertol)) return 0;
  if (! outfile->write(&lowertol)) return 0;
  if (! outfile->write(&precision)) return 0;
  if (! outfile->write(&tolprecision)) return 0;
  if (! valueinfo.save(outfile)) return 0;
  if (! prefixinfo.save(outfile)) return 0;
  if (! suffixinfo.save(outfile)) return 0;
  if (! term1.save(outfile)) return 0;
  if (! term2.save(outfile)) return 0;
  options.set(ScaleDefined,scale != 1.0);
  options.set(DimensionTextYOffset,textyoffset != 0.0);
  options.set(MoveTextDefined,textpointlist.length() > 0);
  if (! outfile->write(&options)) return 0;
  if (options.test(ToleranceDefined))
    if (! toleranceinfo.save(outfile)) return 0;
  if (options.test(ScaleDefined))
    if (! outfile->write(&scale)) return 0;
  if (options.test(DimensionTextYOffset))
    if (! outfile->write(&textyoffset)) return 0;
  if (options.test(MoveTextDefined))
    if (! textpointlist.save(outfile)) return 0;
  if (options.test(EquationDefined))
    if (! outfile->write(equation)) return 0;
  return 1;
}

Entity *AngularDimension::load(int,CadFile *infile)
{AngularDimension *angulardimension;
 EntityHeader *header;
 Point org,xaxis,yaxis,origin,startp,endp;
 double gap,extension,uppertol,lowertol,stackdis,scale,textyoffset;
 short precision,tolprecision;
 BitMask options(32);
 TerminatorInfo term1,term2;
 TextInfo ti1,ti2,ti3,ti4;
 PointList list;
 PointList textpointlist;
 RCCHAR *equation;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! org.load(infile)) return 0;
  if (! xaxis.load(infile)) return 0;
  if (! yaxis.load(infile)) return 0;
  if (! origin.load(infile)) return 0;
  if (! list.load(infile)) return 0;
  if (! infile->read(&gap)) return 0;
  if (! infile->read(&extension)) return 0;
  if (! infile->read(&stackdis)) return 0;
  if (! infile->read(&uppertol)) return 0;
  if (! infile->read(&lowertol)) return 0;
  if (! infile->read(&precision)) return 0;
  if (! infile->read(&tolprecision)) return 0;
  if (! ti1.load(infile)) return 0;
  if (! ti3.load(infile)) return 0;
  if (! ti4.load(infile)) return 0;
  if (! term1.load(infile)) return 0;
  if (! term2.load(infile)) return 0;
  if (! infile->read(&options)) return 0;
  if (options.test(ToleranceDefined))
    {  if (! ti2.load(infile)) return 0;
    }
  else
    {  ti2 = ti1;
       ti2.font = new RCCHAR [RCSTRLEN(ti1.font)+1];
       ti2.text = new RCCHAR [RCSTRLEN(ti1.font)+1];
       RCSTRCPY(ti2.font,ti1.font);
       RCSTRCPY(ti2.text,ti1.text);
    }
  if (options.test(ScaleDefined))
    {  if (! infile->read(&scale)) return 0;
    }
  else
    scale = 1.0;
  if (options.test(DimensionTextYOffset))
    {  if (! infile->read(&textyoffset)) return 0;
    }
  else
    textyoffset = 0.0;
  if (options.test(MoveTextDefined))
    if (! textpointlist.load(infile)) return 0;
  if (options.test(EquationDefined))
    {
      if(infile->getversion() == 2)
      {
          char *ctemp=0;
          if (! infile->read(&ctemp)) return 0;
          equation = new RCCHAR[strlen(ctemp)+1];
          strcpy(equation,ctemp);
          delete [] ctemp;
      }
      else
          if (! infile->read(&equation)) return 0;
    }
  else
    {  equation = new RCCHAR[1];
       equation[0] = 0;
    }

  angulardimension = new AngularDimension((EntityHeader &)*header,org,xaxis,yaxis,origin,list,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,
                                        precision,tolprecision,term1,term2,ti1,ti2,ti3,ti4,options);
  angulardimension->atlist = header->getatlist();
  angulardimension->textpointlist = textpointlist;
  angulardimension->equation = equation;
  delete header;
  delete ti1.text;  ti1.font;
  delete ti2.text;  ti2.font;
  delete ti3.text;  ti3.font;
  return angulardimension;
}

int AngularDimension::save(UndoFile *outfile)
{short type;
  type = angulardimension_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! textpointlist.save(outfile)) return 0;
  if (! outfile->write(valueinfo.text)) return 0;
  if (! outfile->write(valueinfo.font)) return 0;
  if (! outfile->write(toleranceinfo.text)) return 0;
  if (! outfile->write(toleranceinfo.font)) return 0;
  if (! outfile->write(prefixinfo.text)) return 0;
  if (! outfile->write(prefixinfo.font)) return 0;
  if (! outfile->write(suffixinfo.text)) return 0;
  if (! outfile->write(suffixinfo.font)) return 0;
  if (! outfile->write(&valueinfo.options)) return 0;
  if (! outfile->write(&prefixinfo.options)) return 0;
  if (! outfile->write(&suffixinfo.options)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(equation)) return 0;
  return 1;
}

Entity *AngularDimension::load(UndoFile *infile)
{AngularDimension *ad;
 PointList empty;
 char bm[sizeof(BitMask)];
  if ((ad = new AngularDimension) != NULL)
    {AttributeList al;
       memcpy(bm,&ad->visible,sizeof(bm));
       if (! infile->blockread(ad,sizeof(*ad))) return 0;
       memcpy(&ad->visible,bm,sizeof(bm));
       if (! infile->read(&ad->visible)) return 0;
       if (! al.load(infile)) return 0;  ad->atlist = al;
       ad->list = empty;
       if (! ad->list.load(infile)) return 0;
       ad->textpointlist = empty;
       if (! ad->textpointlist.load(infile)) return 0;
       if (! infile->read(&ad->valueinfo.text)) return 0;
       if (! infile->read(&ad->valueinfo.font)) return 0;
       if (! infile->read(&ad->toleranceinfo.text)) return 0;
       if (! infile->read(&ad->toleranceinfo.font)) return 0;
       if (! infile->read(&ad->prefixinfo.text)) return 0;
       if (! infile->read(&ad->prefixinfo.font)) return 0;
       if (! infile->read(&ad->suffixinfo.text)) return 0;
       if (! infile->read(&ad->suffixinfo.font)) return 0;
       if (! infile->read(&ad->valueinfo.options)) return 0;
       if (! infile->read(&ad->prefixinfo.options)) return 0;
       if (! infile->read(&ad->suffixinfo.options)) return 0;
       if (! infile->read(&ad->options)) return 0;
       if (! infile->read(&ad->equation)) return 0;
    }
  return ad;
}

int AngularDimension::saveattributes(CadFile *outfile)
{ if (! outfile->write(&gap)) return 0;
  if (! outfile->write(&extension)) return 0;
  if (! outfile->write(&stackdis)) return 0;
  if (! outfile->write(&uppertol)) return 0;
  if (! outfile->write(&lowertol)) return 0;
  if (! outfile->write(&scale)) return 0;
  if (! outfile->write(&precision)) return 0;
  if (! outfile->write(&tolprecision)) return 0;
  if (! valueinfo.save(outfile)) return 0;
  if (! prefixinfo.save(outfile)) return 0;
  if (! suffixinfo.save(outfile)) return 0;
  if (! term1.save(outfile)) return 0;
  if (! term2.save(outfile)) return 0;
  if (! outfile->write(&options)) return 0; 
  if (! outfile->write(equation)) return 0; 
  return 1;
}

int AngularDimension::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  delete [] valueinfo.text;   delete [] valueinfo.font;
  delete [] prefixinfo.text;  delete [] prefixinfo.font;
  delete [] suffixinfo.text;  delete [] suffixinfo.font;
  if (! infile->read(&gap)) return 0;
  if (! infile->read(&extension)) return 0;
  if (! infile->read(&stackdis)) return 0;
  if (! infile->read(&uppertol)) return 0;
  if (! infile->read(&lowertol)) return 0;
  if (! infile->read(&scale)) return 0;
  if (! infile->read(&precision)) return 0;
  if (! infile->read(&tolprecision)) return 0;
  if (! valueinfo.load(infile)) return 0;
  if (! prefixinfo.load(infile)) return 0;
  if (! suffixinfo.load(infile)) return 0;
  if (! term1.load(infile)) return 0;
  if (! term2.load(infile)) return 0;
  if (! infile->read(&options)) return 0;
  if(infile->getversion() == 2)
  {
      char *ctemp=0;
      if (! infile->read(&ctemp)) return 0;
      equation = new RCCHAR[strlen(ctemp)+1];
      strcpy(equation,ctemp);
      delete [] ctemp;
  }
  else
    if (! infile->read(&equation)) return 0;
  return 1;
}

static int dxfcb(Entity *entity,void *data)
{ entity->savedxf(0,(DXFOut *)data);
  return 1;
}

int AngularDimension::savedxf(int blocksection,DXFOut *dxfout)
{ if (! blocksection)
    generate(dxfout->getwindow(),dxfcb,dxfout);
  return 1;
}

struct IgesParameters
{FILE *outfile;
 int section,options,*ndirectory,*nparameter,result;
};

static int igescb(Entity *entity,void *data)
{IgesParameters *ip = (IgesParameters *) data;
  if (entity->exportiges(ip->outfile,ip->section,ip->options,ip->ndirectory,ip->nparameter) == 0)
    ip->result = 0;
  return ip->result;
}

int AngularDimension::exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter)
{IgesParameters ip;
  ip.outfile = outfile;
  ip.section = section;
  ip.options = options;
  ip.ndirectory = ndirectory;
  ip.nparameter = nparameter;
  ip.result = 1;
  generate(cadwindow->getcurrentwindow(),igescb,&ip);
  return ip.result;
}


int AngularDimension::savegeometry(CadFile *outfile)
{ if (! cs.save(outfile)) return 0;
  if (! origin.save(outfile)) return 0;
  if (! list.save(outfile)) return 0;
  if (! textpointlist.save(outfile)) return 0;
  return 1;
}

int AngularDimension::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  list.destroy();
  textpointlist.destroy();
  if (! cs.load(infile)) return 0;
  if (! origin.load(infile)) return 0;
  if (! list.load(infile)) return 0;
  if (! textpointlist.load(infile)) return 0;
  return 1;
}

