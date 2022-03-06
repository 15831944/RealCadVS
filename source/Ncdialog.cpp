
#include "ncwin.h"
#include "RCDialog.h"

#if !defined(COMPILE_MULTIMON_STUBS) && (WINVER < 0x0500)
//
// this needs to be here so that we get the wrapper stubs that deal with win95 platforms
#define COMPILE_MULTIMON_STUBS
//#include "multimon.h"
//
#endif // !defined(COMPILE_MULTIMON_STUBS) && (WINVER < 0x0500)

/*
static RCCHAR *NEAR FunctionName[] =
  {  _RCT("abs"),_RCT("acos"),_RCT("asin"),_RCT("atan"),_RCT("atan2"),_RCT("ceil"),_RCT("int"),
     _RCT("cos"),_RCT("exp"),_RCT("log"),_RCT("log10"),_RCT("fmod"),_RCT("pow"),_RCT("pow10"),_RCT("random"),
     _RCT("randomize"),_RCT("tan"),_RCT("sqrt"),_RCT("sin"),_RCT("pi"),_RCT("")
  };
*/
static const char *NEAR FunctionName[] =
  {  "abs","acos","asin","atan","atan2","ceil","int",
     "cos","exp","log","log10","fmod","pow","pow10","random",
     "randomize","tan","sqrt","sin","pi",0
  };

#define MINDOUBLE 1.0E-60

Expression::Expression(char *s)
{
  string = _strlwr(s);
  pos = 0;
}

Expression::Expression(RCCHAR *s)
{
  rcs = _strlwr(s);
  strcpy(ctemp,rcs);
  string = ctemp;
  pos = 0;
}

int Expression::evaluate(long *l)
{double x;
  nextsymbol();
  if (! expression(&x))
    return 0;
  if (fabs(x - long(x +0.5)) > 0.00001)
    return 0;
  *l = (long)(x + 0.5);
  return stype == EolSym;
}

int Expression::evaluate(double *x)
{ nextsymbol();
  if (! expression(x))
    return 0;
  return stype == EolSym;
}

void Expression::nextsymbol(void)
{int i;
 //RCCHAR *eptr;
 char *eptr;
 double units,numerator,denominator;

  stype = BadSym;
  while (string[pos] == ' ') pos++;
  if (string[pos] == 0)
    stype = EolSym;
  else if (string[pos] == '+')
    {  stype = PlusSym;  pos++;
    }
  else if (string[pos] == '-')
    {  stype = MinusSym;  pos++;
    }
  else if (string[pos] == '=')
    {  stype = EqualsSym;  pos++;
    }
  else if (string[pos] == ';')
    {  stype = SemiColonSym;  pos++;
    }
  else if (string[pos] == '/')
    { stype = DivideSym;  pos++;
    }
  else if (string[pos] == '*')
    {  stype = MultiplySym;  pos++;
    }
  else if (string[pos] == '(')
    {  stype = LBracketSym;  pos++;
    }
  else if (string[pos] == ')')
    {  stype = RBracketSym;   pos++;
    }
  else if (string[pos] == ',')
    {  stype = CommaSym;   pos++;
    }
  else if (string[pos] >= 'a' && string[pos] <= 'z')
    {  i = 0;
       sname[i++] = string[pos++];
       while (string[pos] >= 'a' && string[pos] <= 'z' ||
              string[pos] >= '0' && string[pos] <= '9' ||
              string[pos] == '_')
         {  if (i < 255)
              sname[i++] = string[pos++];
            else
              pos++;
         }
       sname[i] = 0;
       for (i = 0 ; FunctionName[i] != 0 ; i++)
         if (strcmp(sname,FunctionName[i]) == 0)
           break;
       if (FunctionName[i] == 0)
         stype = VariableSym;
       else
         stype = FunctionSym;
    }
  else
    {  svalue = strtod(string+pos,&eptr);
       //  Don't interpret the letter d as an exponent.
       if (strchr(string+pos,'d') != 0 && strchr(string+pos,'d') < eptr)
         {char *s;
            s = strchr(string+pos,'d');
            *s = '!';
            svalue = strtod(string+pos,&eptr);
            *s = 'd';
         }
       pos = int(eptr - string);


       /*
       // KMJ: this code was in V7 but not V5
       // it does not work as is
       // it does not look for the whole fraction part before dividing
       // it does not find a decimal point in th fraction number
       // i.e. 10.5/2.5 does not work

       //  Look for a fraction following the number
       if (string[pos] == '/')
       {   denominator = strtol(eptr+1,&eptr,10);
           if (denominator != 0.0)
           {   svalue = svalue / denominator;
               pos = int(eptr - string);
           }
       }

       //  Look for a whole fraction and add it in if possible
       if ((tolower(string[pos]) == 0) && isdigit(string[pos+1]))
       {//  This is a fraction of an inch
           numerator = strtol(string+pos+1,&eptr,10);
           if (*eptr == '/')
           {   denominator = strtol(eptr+1,&eptr,10);
               if (denominator != 0.0)
               {   svalue += numerator / denominator;
                   pos = int(eptr - string);
               }
           }
       }

       while (string[pos] == ' ')
         pos++;
       */

       if (tolower(string[pos]) == '#')
         {  svalue += strtod(string+pos+1,&eptr) / 60.0;
            pos = int(eptr - string);
            if (tolower(string[pos]) == '\'')
              {  svalue += strtod(string+pos+1,&eptr) / 60.0 / 60.0;
                 pos = int(eptr - string);
                 if (tolower(string[pos]) == '"')
                   pos++;
              }
         }
       else if (tolower(string[pos]) == '\'')
         {
            numerator = strtol(string+pos+1,&eptr,10);
            if (*eptr == '/')
            {   denominator = strtol(eptr+1,&eptr,10);
                if (denominator != 0.0)
                {   svalue += numerator / denominator / 12.0;
                    pos = int(eptr - string);
                }
                else
                {   svalue += strtod(string+pos+1,&eptr) / 12.0;
                    pos = int(eptr - string);
                }
            }
            else
            {   svalue += strtod(string+pos+1,&eptr) / 12.0;
                pos = int(eptr - string);
            }

            //  Look for a fraction and add it in if possible
            if (tolower(string[pos]) == ' ' && isdigit(string[pos+1]))
            {//  This is a fraction of an inch
                numerator = strtol(string+pos+1,&eptr,10);
                if (*eptr == '/')
                {   denominator = strtol(eptr+1,&eptr,10);
                    if (numerator != 0.0)
                    {   svalue += numerator / denominator / 12.0;
                        pos = int(eptr - string);
                    }
                }
            }

            if (tolower(string[pos]) == '"')
              pos++;
            units = v.getinteger("db::units");
            if (units == 0) //mm
                svalue *= 12.0 * 25.4;
            else if (units == 1) //m
                svalue *= 12.0 * 25.4 / 1000.0;
            else if (units == 2) //in
                svalue *= 12.0;
            else if (units == 3 || units == 4) //ft/in
                svalue *= 1.0;
            else if (units == 5) //cm
                svalue *= 12.0 * 25.4 / 10.0;
         }
       else if (tolower(string[pos]) == '\"')
         {  svalue /= 12.0;
            pos++;
            units = v.getinteger("db::units");
            if (units == 0) //mm
                svalue *= 12.0 * 25.4;
            else if (units == 1) //m
                svalue *= 12.0 * 25.4 / 1000.0;
            else if (units == 2) //in
                svalue *= 12.0;
            else if (units == 3 || units == 4) //ft/in
                svalue *= 1.0;
            else if (units == 5) //cm
                svalue *= 12.0 * 25.4 / 10.0;

         }
       else if (tolower(string[pos]) == ' ') //might be a partial fraction
         {
           if (isdigit(string[pos+1]))
           {
               numerator = strtol(string+pos+1,&eptr,10);
               if (*eptr == '/')
               {   denominator = strtol(eptr+1,&eptr,10);
                   if (denominator != 0.0)
                   {   svalue += numerator / denominator;
                       pos = int(eptr - string);
                   }
                   else
                   {   svalue += strtod(string+pos+1,&eptr);
                       pos = int(eptr - string);
                   }
               }
               else
               {
                   pos = int(eptr - string);
               }
               if (tolower(string[pos]) == '\'') {pos++;}
               else if (tolower(string[pos]) == '\"') {pos++;}
           }
         }
       stype = RealConstantSym;
    }
}

int Expression::expression(double *e)
{double t1,t2,sign;
 SymbolType s;
  sign = 1.0;
  if (stype == PlusSym)
    {  sign = 1.0;  nextsymbol();
    }
  else if (stype == MinusSym)
    {  sign = -1.0;  nextsymbol();
    }
  if (! term(&t1))
    return 0;
  t1 *= sign;
  while (stype == PlusSym || stype == MinusSym)
    {  s = stype;
       nextsymbol();
       if (! term(&t2))
         return 0;
       if (s == PlusSym)
         t1 += t2;
       else
         t1 -= t2;
    }
  *e = t1;
  return 1;
}

int Expression::term(double *t)
{double f1,f2;
 SymbolType s;
  if (! factor(&f1))
    return 0;
  while (stype == DivideSym || stype == MultiplySym)
    {  s = stype;
       nextsymbol();
       if (! factor(&f2))
         return 0;
       if (s == DivideSym)
         {  if (fabs(f2) < 1.0E-10)
              return 0;
            f1 /= f2;
         }
       else
         f1 *= f2;
    }
  *t = f1;
  return 1;
}

int Expression::factor(double *f)
{double p1,p2;
 char fname[256];
  if (stype == RealConstantSym)
    {  *f = svalue;  nextsymbol();
    }
  else if (stype == FunctionSym)
    {  strcpy(fname,sname);
       nextsymbol();
       if (strcmp(fname,"pi") != 0 && strcmp(fname,"random") != 0)
         {  if (stype != LBracketSym)
              return 0;
            nextsymbol();
            if (! expression(&p1))
              return 0;
            if (strcmp(fname,"fmod") == 0 || strcmp(fname,"pow") == 0 || strcmp(fname,"atan2") == 0)
              {  if (stype != CommaSym)
                   return 0;
                 nextsymbol();
                 if (! expression(&p2))
                   return 0;
              }
            if (stype != RBracketSym)
              return 0;
            nextsymbol();
         }
       if (strcmp(fname,"abs") == 0)
         *f = fabs(p1);
       else if (strcmp(fname,"acos") == 0)
         {  if (p1 < -1.0 || p1 > 1.0)
              return 0;
            *f = acos(p1) * 180.0 / M_PI;
         }
       else if (strcmp(fname,"asin") == 0)
         {  if (p1 < -1.0 || p1 > 1.0)
              return 0;
            *f = asin(p1) * 180.0 / M_PI;
         }
       else if (strcmp(fname,"atan") == 0)
         *f = atan(p1) * 180.0 / M_PI;
       else if (strcmp(fname,"atan2") == 0)
         *f = atan2(p1,p2) * 180.0 / M_PI;
       else if (strcmp(fname,"ceil") == 0)
         *f = ceil(p1);
       else if (strcmp(fname,"int") == 0)
         {  if (p1 < -MAXLONG || p1 > MAXLONG)
              return 0;
            *f = long(p1);
         }
       else if (strcmp(fname,"cos") == 0)
         *f = cos(p1 / 180.0 * M_PI);
       else if (strcmp(fname,"exp") == 0)
         {  if (p1 > log(MAXDOUBLE))
              return 0;
            *f = exp(p1);
         }
       else if (strcmp(fname,"log") == 0)
         {  if (p1 <= 0.0)
              return 0;
            *f = log(p1);
         }
       else if (strcmp(fname,"log10") == 0)
         {  if (p1 <= 0.0)
              return 0;
            *f = log10(p1);
         }
       else if (strcmp(fname,"fmod") == 0)
         {  if (fabs(p2) < 1.0E-10)
              return 0;
            *f = fmod(p1,p2);
         }
       else if (strcmp(fname,"pow") == 0)
         {  if (p1 <= 0.0 || p2 * log(p1) > log(MAXDOUBLE))
              return 0;
            *f = pow(p1,p2);
         }
       else if (strcmp(fname,"pow10") == 0)
         {  if (p1 > log10(MAXDOUBLE))
              return 0;
            *f = exp(log(10.0)*p1);
         }
       else if (strcmp(fname,"random") == 0)
         *f = double(rand()) / double(RAND_MAX);
       else if (strcmp(fname,"randomize") == 0)
         {  *f = 0.0;
            srand(timeGetTime());
            //randomize();
         }
       else if (strcmp(fname,"tan") == 0)
         {  p1 *= M_PI / 180.0;
            if (fabs(fmod(p1 - acos(-1.0) / 2.0,acos(-1.0))) < 1.0E-10)
              return 0;
            *f = tan(p1);
         }
       else if (strcmp(fname,"sqrt") == 0)
         {  if (p1 < 0.0)
              return 0;
            *f = sqrt(p1);
         }
       else if (strcmp(fname,"sin") == 0)
         *f = sin(p1 / 180.0 * M_PI);
       else if (strcmp(fname,"pi") == 0)
         *f = M_PI;
    }
  else if (stype == LBracketSym)
    {  nextsymbol();
       if (! expression(f))
         return 0;
       if (stype != RBracketSym)
         return 0;
       nextsymbol();
    }
  else
    return 0;
  return 1;
}

void DialogControl::selectitem(Dialog *dialog)
{
   SetFocus(GetDlgItem(dialog->gethdlg(),id));
#if defined(WIN32)
   SendDlgItemMessage(dialog->gethdlg(),id,EM_SETSEL,0,-1);
#else
   SendDlgItemMessage(dialog->gethdlg(),id,EM_SETSEL,0,MAKELONG(0, -1));
#endif
}

void DialogControl::enable(Dialog *dialog,int enable)
{  EnableWindow(GetDlgItem(dialog->gethdlg(),id),enable);
}

void DialogControl::changefocus(Dialog *dialog,int updatescreen)
{DisplayDialogControl *ddc;
 GeneralList list;

  if (updatescreen == 0)
    return;

  list = dialog->getdisplaylist();
  for (list.start() ; (ddc = (DisplayDialogControl *)list.next()) != NULL ; )
    {  ddc->sethwnd(GetDlgItem(dialog->gethdlg(),ddc->getid()));
       if (ddc->gethwnd() != 0)
         ddc->draw(dialog,0);
    }
}

void DialogControl::lbuttondblclkevent(Dialog *dialog,int,int,int)
{
    if (dialog->validate())
    {
       dialog->store();
       EndDialog(dialog->gethdlg(),TRUE);
    }
}

void ScrollBarDialogControl::vscrollevent(Dialog *dialog,int scrollid,int scrollcode,int pos)
{DisplayDialogControl *ddc;
 DialogControl *cbdc;
 GeneralList list;
  SetFocus(GetDlgItem(dialog->gethdlg(),dc->getid()));
  dc->vscrollevent(dialog,scrollid,scrollcode,pos);
  list = dialog->getdisplaylist();
  for (list.start() ; (ddc = (DisplayDialogControl *)list.next()) != NULL ; )
    {  ddc->sethwnd(GetDlgItem(dialog->gethdlg(),ddc->getid()));
             ddc->draw(dialog,0);
       }
  if ((cbdc = dialog->getcontrol(scrollid - 1100 + 200)) != 0)
    ((CheckBoxDialogControl *)cbdc)->change(dialog,1);
}

void ScrollBarDialogControl::load(Dialog *dialog)
{
    SetScrollRange(GetDlgItem(dialog->gethdlg(),id),SB_CTL,0,255,TRUE);
}


ScaleDialogControl::ScaleDialogControl(int i,double *s) : DialogControl(i)
{ scale = s;
}

void ScaleDialogControl::load(Dialog *dialog)
{int i;

        for (i = 0 ; DefaultPlotScales[i].label != 0 ; i++)
       SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPSTR)DefaultPlotScales[i].label.data()));

  dialog->SetDlgItemText(id,MatchPlotScale(*scale));

}


int ScaleDialogControl::store(Dialog *dialog)
{int i;
 RCCHAR string[300],*eptr;
 ResourceString rs0(NCDIALOG);
 double x;

        dialog->GetDlgItemText(id,string,300);
        for (i = 0 ; DefaultPlotScales[i].label != 0 ; i++)
       {  if (_stricmp(string,DefaultPlotScales[i].label.data()) == 0)
         {  *scale = DefaultPlotScales[i].scale;
            break;
         }
    }

  if (DefaultPlotScales[i].label == 0)
    {ResourceString rs1(NCDIALOG+1);
     ResourceString rs2(NCDIALOG+2);

       if (string[0] == '1' && string[1] == ':')
         x = strtod(string+2,&eptr);
       else
         x = strtod(string,&eptr);

       if (*eptr != 0)
         {  cadwindow->MessageBox(rs1.gets(),"",MB_ICONSTOP);
            return FALSE;
         }
       if (x < 1.0E-10 || x > 1.0E10)
         {  cadwindow->MessageBox(rs2.gets(),"",MB_ICONSTOP);
            return FALSE;
         }
       *scale = x;
    }

  return TRUE;
}


void ScaleDialogControl::change(Dialog *dialog,double s)
{ dialog->SetDlgItemText(id,MatchPlotScale(s));
}

int ScaleDialogControl::validate(Dialog *dialog)
{int i;
 RCCHAR string[300],*eptr;
 ResourceString rs0(NCDIALOG);
 double x;

        dialog->GetDlgItemText(id,string,300);
        for (i = 0 ; DefaultPlotScales[i].label != 0 ; i++)
       {  if (_stricmp(string,DefaultPlotScales[i].label.data()) == 0)
         break;
    }

  if (DefaultPlotScales[i].label == 0)
    {ResourceString rs1(NCDIALOG+1);
     ResourceString rs2(NCDIALOG+2);

       if (string[0] == '1' && string[1] == ':')
         x = strtod(string+2,&eptr);
       else
         x = strtod(string,&eptr);

       if (*eptr != 0)
         {  cadwindow->MessageBox(rs1.gets(),"",MB_ICONSTOP);
            return FALSE;
         }
       if (x < 1.0E-10 || x > 1.0E10)
         {  cadwindow->MessageBox(rs2.gets(),"",MB_ICONSTOP);
            return FALSE;
         }

    }

  return TRUE;
}

int ScaleDialogControl::currentvalue(Dialog *,double *)
{ return TRUE;
}

#ifdef _USING_QTCHAR
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l) : DialogControl(i)
{  n = num;  list = l;  value = 0;  svalue = 0;  shvalue = 0; ctype=1;
}

#else
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l) : DialogControl(i)
{  n = num;  list = l;  value = 0;  svalue = 0;  shvalue = 0;
}
#endif

#ifdef _USING_QTCHAR
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l,int *v) : DialogControl(i)
{  n = num;  list = l;  value = v;  svalue = 0;  shvalue = 0; ctype=1;
}

ListDialogControl::ListDialogControl(int i,int num,QString *l,int *v) : DialogControl(i)
{
    n = num;  list = 0;  value = v;  svalue = 0;  shvalue = 0; ctype=1;
    list = new RCCHAR*[num];
    for(int i=0; i<num; i++)
        list[i] = l[i].data();
}

#else
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l,int *v) : DialogControl(i)
{  n = num;  list = l;  value = v;  svalue = 0;  shvalue = 0;
}
#endif

#ifdef _USING_QTCHAR
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l,short *v) : DialogControl(i)
{  n = num;  list = l;  shvalue = v;  svalue = 0;  value = 0; ctype=1;
}

#else
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l,short *v) : DialogControl(i)
{  n = num;  list = l;  shvalue = v;  svalue = 0;  value = 0;
}
#endif

#ifdef _USING_QTCHAR
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l,RCCHAR *s,int len) : DialogControl(i)
{  n = num;  list = l;  value = 0;  shvalue = 0;  svalue = s;  length = len; ctype=1;
}

#else
ListDialogControl::ListDialogControl(int i,int num,RCCHAR **l,RCCHAR *s,int len) : DialogControl(i)
{  n = num;  list = l;  value = 0;  shvalue = 0;  svalue = s;  length = len;
}
#endif

void ListDialogControl::load(Dialog *dialog)
{
  int i;
  if (value == 0 && shvalue == 0 && svalue == 0)
  {
      if (RCWORD(SendDlgItemMessage(dialog->gethdlg(),id,LB_GETCOUNT,0,0)) == 0)
      {
          for (i = 0 ; i < n ; i++)
              SendDlgItemMessageW(dialog->gethdlg(),id,LB_ADDSTRING,0,(RCLPARAM)((RCLPSTR)list[i]));
      }
  }
  else
  {
      if (RCWORD(SendDlgItemMessage(dialog->gethdlg(),id,CB_GETCOUNT,0,0)) == 0)
      {
          for (i = 0 ; i < n ; i++)
              SendDlgItemMessageW(dialog->gethdlg(),id,CB_ADDSTRING,0,(RCLPARAM)((RCLPSTR)list[i]));
      }
  }
  if (value != 0 && n > 0)
       dialog->SetDlgItemText(id,list[*value]);
  else if (shvalue != 0 && n > 0)
       dialog->SetDlgItemText(id,list[*shvalue]);
  else if (svalue != 0)
       dialog->SetDlgItemText(id,svalue);
  //else if (csvalue != 0)
    //   dialog->SetDlgItemText(id,csvalue);
}

#ifdef _USING_QTCHAR
void ListDialogControl::additem(Dialog *dialog,RCCHAR *item)
{
    qDebug() << "*item=" << QString(item);
  if (value == 0 && shvalue == 0 && svalue == 0)
    SendDlgItemMessageW(dialog->gethdlg(),id,LB_ADDSTRING,0,(LPARAM)((RCPCHAR)item));
  else
    SendDlgItemMessageW(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((RCPCHAR)item));
}

#else
void ListDialogControl::additem(Dialog *dialog,RCCHAR *item)
{ if (value == 0 && shvalue == 0 && svalue == 0)
    SendDlgItemMessage(dialog->gethdlg(),id,LB_ADDSTRING,0,(LPARAM)((LPSTR)item));
  else
    SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPSTR)item));
}
#endif


#ifdef _USING_QTCHAR
int ListDialogControl::getitem(Dialog *dialog,int index,RCCHAR *text,int)
{ return SendDlgItemMessageW(dialog->gethdlg(),id,LB_GETTEXT,index,(LPARAM)text) != LB_ERR;
}

#else
int ListDialogControl::getitem(Dialog *dialog,int index,RCCHAR *text,int)
{ return SendDlgItemMessage(dialog->gethdlg(),id,LB_GETTEXT,index,(LPARAM)text) != LB_ERR;
}
#endif


#ifdef _USING_QTCHAR
void ListDialogControl::removeitem(Dialog *dialog,RCCHAR *item)
{long index;
  if (value == 0 && shvalue == 0 && svalue == 0)
    {  index = SendDlgItemMessageW(dialog->gethdlg(),id,LB_FINDSTRING, 0, (LPARAM) ((RCPCHAR) item));
       if (index != CB_ERR)
          SendDlgItemMessage(dialog->gethdlg(),id,LB_DELETESTRING,(WPARAM)index,(LPARAM)0);
    }
  else
    {  index = SendDlgItemMessageW(dialog->gethdlg(),id,CB_FINDSTRING, 0, (LPARAM) ((RCPCHAR) item));
       if (index != CB_ERR)
          SendDlgItemMessage(dialog->gethdlg(),id,CB_DELETESTRING,(WPARAM)index,(LPARAM)0);
    }
}

#else
void ListDialogControl::removeitem(Dialog *dialog,RCCHAR *item)
{long index;
  if (value == 0 && shvalue == 0 && svalue == 0)
    {  index = SendDlgItemMessage(dialog->gethdlg(),id,LB_FINDSTRING, 0, (LPARAM) ((LPSTR) item));
       if (index != CB_ERR)
          SendDlgItemMessage(dialog->gethdlg(),id,LB_DELETESTRING,(WPARAM)index,(LPARAM)0);
    }
  else
    {  index = SendDlgItemMessage(dialog->gethdlg(),id,CB_FINDSTRING, 0, (LPARAM) ((LPSTR) item));
       if (index != CB_ERR)
          SendDlgItemMessage(dialog->gethdlg(),id,CB_DELETESTRING,(WPARAM)index,(LPARAM)0);
    }
}
#endif

void ListDialogControl::removeallitems(Dialog *dialog)
{ if (value == 0 && shvalue == 0 && svalue == 0)
    SendDlgItemMessage(dialog->gethdlg(),id,LB_RESETCONTENT,0,0);
  else
    SendDlgItemMessage(dialog->gethdlg(),id,CB_RESETCONTENT,0,0);
}

void ListDialogControl::change(Dialog *dialog,int newvalue)
{  dialog->SetDlgItemText(id,list[newvalue]);
}


#ifdef _USING_QTCHAR
void ListDialogControl::change(Dialog *dialog,RCCHAR *string)
{ dialog->SetDlgItemText(id,string);
}

#else
void ListDialogControl::change(Dialog *dialog,RCCHAR *string)
{ dialog->SetDlgItemText(id,string);
}
#endif

int ListDialogControl::validate(Dialog *dialog)
{int i;
 RCCHAR message[300],string[300];
 ResourceString rs0(NCDIALOG);
  if (value != 0 || shvalue != 0)
     {  dialog->GetDlgItemText(id,string,128);
         for (i = 0 ; i < n ; i++)
            if (strcmp(string,list[i]) == 0) break;
         if (i == n)
            {  sprintf(message,rs0.gets(),string);
               cadwindow->MessageBox(message,"",MB_ICONSTOP);
               return FALSE;
            }
     }
  return TRUE;
}

int ListDialogControl::store(Dialog *dialog)
{int i;
 RCCHAR message[300],string[300];
 ResourceString rs0(NCDIALOG);
  if (value != 0 || shvalue != 0)
       {  dialog->GetDlgItemText(id,string,128);
         for (i = 0 ; i < n ; i++)
            if (strcmp(string,list[i]) == 0) break;
         if (i == n && n > 0)
         {
              sprintf(message,rs0.gets(),string);
               dialog->MessageBox(message,_RCT(""),MB_ICONSTOP);
               return FALSE;
         }
         if (value != 0)
            *value = i;
         else
            *shvalue = short(i);
     }
  else if (svalue != 0)
     {
        memset(svalue,0,length);
        dialog->GetDlgItemText(id,svalue,length);
        svalue[length-1] = 0;
     }
  return TRUE;
}

int ListDialogControl::currentvalue(Dialog *dialog,int *newvalue)
{int i;
 RCCHAR string[256];
  dialog->GetDlgItemText(id,string,256);
  for (i = 0 ; i < n ; i++)
     if (strcmp(string,list[i]) == 0) break;
  if (i == n)
     return 0;
  *newvalue = i;
  return 1;
}



int ListDialogControl::currentvalue(Dialog *dialog,short *newvalue)
{int i;
 RCCHAR string[256];
  dialog->GetDlgItemText(id,string,256);
  for (i = 0 ; i < n ; i++)
     if (strcmp(string,list[i]) == 0) break;
  if (i == n)
     return 0;
  *newvalue = short(i);
  return 1;
}


int ListDialogControl::currentvalue(Dialog *dialog,RCCHAR *newstring,int len)
{ memset(newstring,0,len);
  if (value == 0 && shvalue == 0 && svalue == 0)
    {int index;
       index = SendDlgItemMessage(dialog->gethdlg(),id,LB_GETCURSEL,0,0);
       if (index >= 0)
         SendDlgItemMessage(dialog->gethdlg(),id,LB_GETTEXT,index,(LPARAM)newstring);
    }
  else
    {  dialog->GetDlgItemText(id,newstring,len);
       newstring[len-1] = 0;
    }
  return TRUE;
}
//
#if 1
#ifdef _USING_QTCHAR
RCListDialogControl::RCListDialogControl(int i,int num,RCCHAR **l) : DialogControl(i)
{  n = num;  list = l;  value = 0;  svalue = 0;  shvalue = 0;
}

RCListDialogControl::RCListDialogControl(int i,int num,RCCHAR **l,int *v) : DialogControl(i)
{  n = num;  list = l;  value = v;  svalue = 0;  shvalue = 0;
}

RCListDialogControl::RCListDialogControl(int i,int num,RCCHAR **l,short *v) : DialogControl(i)
{  n = num;  list = l;  shvalue = v;  svalue = 0;  value = 0;
}

RCListDialogControl::RCListDialogControl(int i,int num,RCCHAR **l,RCCHAR *s,int len) : DialogControl(i)
{  n = num;  list = l;  value = 0;  shvalue = 0;  svalue = s;  length = len;
}

void RCListDialogControl::load(Dialog *dialog)
{int i;
  if (value == 0 && shvalue == 0 && svalue == 0)
     {  if (RCWORD(SendDlgItemMessage(dialog->gethdlg(),id,LB_GETCOUNT,0,0)) == 0)
                {  for (i = 0 ; i < n ; i++)
                          SendDlgItemMessage(dialog->gethdlg(),id,LB_ADDSTRING,0,(LPARAM)((LPCSTR)list[i]));
                  }
       }
  else
       {  if (RCWORD(SendDlgItemMessage(dialog->gethdlg(),id,CB_GETCOUNT,0,0)) == 0)
                {  for (i = 0 ; i < n ; i++)
                          SendDlgItemMessageW(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((LPCSTR)list[i]));
                  }
       }
  if (value != 0 && n > 0)
       dialog->SetDlgItemText(id,list[*value]);
  else if (shvalue != 0 && n > 0)
       dialog->SetDlgItemText(id,list[*shvalue]);
  else if (svalue != 0)
       dialog->SetDlgItemText(id,svalue);
}

void RCListDialogControl::additem(Dialog *dialog,RCCHAR *item)
{ if (value == 0 && shvalue == 0 && svalue == 0)
    SendDlgItemMessage(dialog->gethdlg(),id,LB_ADDSTRING,0,(LPARAM)((RCPCHAR)item));
  else
    SendDlgItemMessage(dialog->gethdlg(),id,CB_ADDSTRING,0,(LPARAM)((RCPCHAR)item));
}

int RCListDialogControl::getitem(Dialog *dialog,int index,RCCHAR *text,int)
{ return SendDlgItemMessage(dialog->gethdlg(),id,LB_GETTEXT,index,(LPARAM)text) != LB_ERR;
}


void RCListDialogControl::removeitem(Dialog *dialog,RCCHAR *item)
{long index;
  if (value == 0 && shvalue == 0 && svalue == 0)
    {  index = SendDlgItemMessage(dialog->gethdlg(),id,LB_FINDSTRING, 0, (LPARAM) ((RCPCHAR) item));
       if (index != CB_ERR)
          SendDlgItemMessage(dialog->gethdlg(),id,LB_DELETESTRING,(WPARAM)index,(LPARAM)0);
    }
  else
    {  index = SendDlgItemMessage(dialog->gethdlg(),id,CB_FINDSTRING, 0, (LPARAM) ((RCPCHAR) item));
       if (index != CB_ERR)
          SendDlgItemMessage(dialog->gethdlg(),id,CB_DELETESTRING,(WPARAM)index,(LPARAM)0);
    }
}

void RCListDialogControl::removeallitems(Dialog *dialog)
{ if (value == 0 && shvalue == 0 && svalue == 0)
    SendDlgItemMessage(dialog->gethdlg(),id,LB_RESETCONTENT,0,0);
  else
    SendDlgItemMessage(dialog->gethdlg(),id,CB_RESETCONTENT,0,0);
}

void RCListDialogControl::change(Dialog *dialog,int newvalue)
{  dialog->SetDlgItemText(id,list[newvalue]);
}

void RCListDialogControl::change(Dialog *dialog,RCCHAR *string)
{ dialog->SetDlgItemText(id,string);
}

int RCListDialogControl::validate(Dialog *dialog)
{int i;
 RCCHAR message[300],string[300];
 ResourceString rs0(NCDIALOG);
  if (value != 0 || shvalue != 0)
     {  dialog->GetDlgItemText(id,string,128);
         for (i = 0 ; i < n ; i++)
            if (RCSTRCMP(string,list[i]) == 0) break;
         if (i == n)
            {  sprintf(message,rs0.gets(),string);
               cadwindow->MessageBox(message,_RCT(""),MB_ICONSTOP);
               return FALSE;
            }
     }
  return TRUE;
}

int RCListDialogControl::store(Dialog *dialog)
{int i;
 RCCHAR message[300],string[300];
 ResourceString rs0(NCDIALOG);
  if (value != 0 || shvalue != 0)
       {  dialog->GetDlgItemText(id,string,128);
        printQChar(string);
         for (i = 0 ; i < n ; i++)
             if (RCSTRCMP(string,list[i]) == 0) break;
         if (i == n && n > 0)
            {  sprintf(message,rs0.gets(),string);
               dialog->MessageBox(message,_RCT(""),MB_ICONSTOP);
               return FALSE;
            }
         if (value != 0)
            *value = i;
         else
            *shvalue = short(i);
     }
  else if (svalue != 0)
     {  memset(svalue,0,length);
        dialog->GetDlgItemText(id,svalue,length);
        printQChar(svalue);
        svalue[length-1] = 0;
     }
  return TRUE;
}

int RCListDialogControl::currentvalue(Dialog *dialog,int *newvalue)
{int i;
 RCCHAR string[256];
  dialog->GetDlgItemText(id,string,256);
  for (i = 0 ; i < n ; i++)
     if (RCSTRCMP(string,list[i]) == 0) break;
  if (i == n)
     return 0;
  *newvalue = i;
  return 1;
}

int RCListDialogControl::currentvalue(Dialog *dialog,short *newvalue)
{int i;
 RCCHAR string[256];
  dialog->GetDlgItemText(id,string,256);
  for (i = 0 ; i < n ; i++)
     if (RCSTRCMP(string,list[i]) == 0) break;
  if (i == n)
     return 0;
  *newvalue = short(i);
  return 1;
}

int RCListDialogControl::currentvalue(Dialog *dialog,RCCHAR *newstring,int len)
{ memset(newstring,0,len);
  if (value == 0 && shvalue == 0 && svalue == 0)
    {int index;
       index = SendDlgItemMessage(dialog->gethdlg(),id,LB_GETCURSEL,0,0);
       if (index >= 0)
         SendDlgItemMessage(dialog->gethdlg(),id,LB_GETTEXT,index,(LPARAM)newstring);
    }
  else
    {  dialog->GetDlgItemText(id,newstring,len);
       newstring[len-1] = 0;
    }
  return TRUE;
}
#endif
#endif
//

void StringDialogControl::load(Dialog *dialog)
{
    dialog->SetDlgItemText(id,string);
}

int StringDialogControl::store(Dialog *dialog)
{
  memset(string,0,sizeof(RCCHAR)*length);
  dialog->GetDlgItemText(id,string,length);
  string[length-1] = 0;
  return TRUE;
}

void StringDialogControl::change(Dialog *dialog,char *_string)
{
    change( dialog,CHAR2RCCHR(_string));
}

void StringDialogControl::change(Dialog *dialog,RCCHAR *_string)
{
    dialog->SetDlgItemText(id,_string);
    store(dialog);
}

int StringDialogControl::currentvalue(Dialog *dialog,RCCHAR *newstring,int len)
{ memset(newstring,0,sizeof(RCCHAR)*len);
  dialog->GetDlgItemText(id,newstring,len);
  newstring[len-1] = 0;
  return TRUE;
}

RealDialogControl::RealDialogControl(int i,double *v,ScaleBy sb, double df, QWidget *qw) : DialogControl(i,qw)
{  value = v;  step = 1.0;  rmin = rmax = 0.0;  scaleby = sb;
    defaultValue = df;
    //strcpy(specialValueText,_RCT(""));
    QString str = QString("%1").arg(df, 0, 'f', db.getnumericdisplaytolerance());
    strcpy(specialValueText,str.data());
}

RealDialogControl::RealDialogControl(int i,double *v,double s,ScaleBy sb, double df, QWidget *qw) : DialogControl(i,qw)
{  value = v;  step = s;  rmin = rmax = 0.0;  scaleby = sb;
    strcpy(specialValueText,_RCT(""));
    defaultValue = df;
    QString str = QString("%1").arg(df, 0, 'f', db.getnumericdisplaytolerance());
    strcpy(specialValueText,str.data());
}

RealDialogControl::RealDialogControl(int i,double *v,double s,double rmin1,double rmax1,ScaleBy sb, double df, QWidget *qw) : DialogControl(i,qw)
{  value = v;  step = s;  rmin = rmin1;  rmax = rmax1;  scaleby = sb;
    strcpy(specialValueText,_RCT(""));
    defaultValue = df;
    QString str = QString("%1").arg(df, 0, 'f', db.getnumericdisplaytolerance());
    strcpy(specialValueText,str.data());
}

void RealDialogControl::load(Dialog *dialog)
{double x;
#ifdef USING_WIDGETS
  // KMJ: new
  if(getQW() != 0 && qobject_cast<QDoubleSpinBox*>(getQW()) != 0)
      ((QDoubleSpinBox*)getQW())->setSpecialValueText(QString(specialValueText));
#endif

  if (scaleby == ScaleByLength)
  {
      //db.formatnumber(originaltext,*value,FormatForEditField);
      x = *value / db.getmainunitscale();
      sprintf(originaltext,"%.*lf",db.getnumericdisplaytolerance(),x);
  }
  else
  {     if (scaleby == ScaleByArea)
            x = *value / db.getmainunitscale() / db.getmainunitscale();
        else if (scaleby == ScaleByVolume)
            x = *value / db.getmainunitscale() / db.getmainunitscale() / db.getmainunitscale();
        else
            x = *value;
        sprintf(originaltext,"%.*lf",db.getnumericdisplaytolerance(),x);
  }
  //qDebug() << "originaltext" << QString(originaltext);
  //qDebug() << "strchr(originaltext,'.')" << strchr(originaltext,'.');

  if (strchr(originaltext,'.') != 0)
    while (strlen(originaltext) > 0 && originaltext[strlen(originaltext)-1] == '0') originaltext[strlen(originaltext)-1] = 0;
  if (strlen(originaltext) > 0 && originaltext[strlen(originaltext)-1] == '.') originaltext[strlen(originaltext)-1] = 0;
  // fix for inches display
  // TODO: KMJ make a feet and inches spinbox??
  if (strchr(originaltext,'"') != 0)
    while (strlen(originaltext) > 0 && originaltext[strlen(originaltext)-1] == '"') originaltext[strlen(originaltext)-1] = 0;
  dialog->SetDlgItemText(id,originaltext);
  dialog->GetDlgItemText(id,originaltext,128);
  originalvalue = *value;
}

void RealDialogControl::change(Dialog *dialog,double v)
{ originalvalue = v;
  if (scaleby == ScaleByLength)
  {
      //db.formatnumber(originaltext,v,FormatForEditField);
      v = v / db.getmainunitscale();
      sprintf(originaltext,"%.*lf",db.getnumericdisplaytolerance(),v);
  }
  else
  {  if (scaleby == ScaleByArea)
        v = v / db.getmainunitscale() / db.getmainunitscale();
     else if (scaleby == ScaleByVolume)
        v = v / db.getmainunitscale() / db.getmainunitscale() / db.getmainunitscale();
     //qDebug() << "change before sprintf: " << v;
     sprintf(originaltext,"%.*lf",db.getnumericdisplaytolerance(),v);
     //qDebug() << "change after sprintf: " << QString(originaltext);
  }
  if (strchr(originaltext,'.') != 0)
    while (strlen(originaltext) > 1 && originaltext[strlen(originaltext)-1] == '0') originaltext[strlen(originaltext)-1] = 0;
  if (strlen(originaltext) > 0 && originaltext[strlen(originaltext)-1] == '.') originaltext[strlen(originaltext)-1] = 0;
  dialog->SetDlgItemText(id,originaltext);
  dialog->GetDlgItemText(id,originaltext,128);
}

int RealDialogControl::store(Dialog *dialog)
{RCCHAR string[300],*eptr;
 double x;
 ResourceString rs1(NCDIALOG+1);
 ResourceString rs2(NCDIALOG+2);
  dialog->GetDlgItemText(id,string,128);
  if (strcmp(string,originaltext) != 0)
    {  x = strtod(string,&eptr);
       if (scaleby == ScaleByLength)
         x = x * db.getmainunitscale();
       else if (scaleby == ScaleByArea)
         x = x * db.getmainunitscale() * db.getmainunitscale();
       else if (scaleby == ScaleByVolume)
         x = x * db.getmainunitscale() * db.getmainunitscale() * db.getmainunitscale();

       if (*eptr != 0)
         {  cadwindow->MessageBox(rs1.gets(),"",MB_ICONSTOP);
            return FALSE;
         }
       if ((rmin != 0.0 || rmax != 0.0) && (x < rmin || x > rmax))
         {  cadwindow->MessageBox(rs2.gets(),"",MB_ICONSTOP);
            return FALSE;
         }
       *value = x;
    }
  else
    *value = originalvalue;

  return TRUE;
}

int RealDialogControl::validate(Dialog *dialog)
{RCCHAR string[300],*eptr;
 double x;
 ResourceString rs1(NCDIALOG+1);// check numeric entry
 ResourceString rs2(NCDIALOG+2);// number out of range
  dialog->GetDlgItemText(id,string,128);
  if (strcmp(string,originaltext) != 0)
    {  x = strtod(string,&eptr);
       if (scaleby == ScaleByLength)
         x = x * db.getmainunitscale();
       else if (scaleby == ScaleByArea)
         x = x * db.getmainunitscale() * db.getmainunitscale();
       else if (scaleby == ScaleByVolume)
         x = x * db.getmainunitscale() * db.getmainunitscale() * db.getmainunitscale();
       if (*eptr != 0)
         {  dialog->MessageBox(rs1.getws(),_RCT(""),MB_ICONSTOP);
            return FALSE;
         }
       if ((rmin != 0.0 || rmax != 0.0) && (x < rmin || x > rmax))
           {  dialog->MessageBox(rs2.getws(),_RCT(""),MB_ICONSTOP);
            return FALSE;
         }
    }
  return TRUE;

}

void RealDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int)
{double v;
  if (currentvalue(dialog,&v))
    {  if (scrollcode == SB_LINEDOWN)
         v -= step * db.getmainunitscale();
       else if (scrollcode == SB_LINEUP)
         v += step * db.getmainunitscale();
       if (rmin != 0.0 || rmax != 0.0)
         {  if (v < rmin) v = rmin;
            if (v > rmax) v = rmax;
         }
       change(dialog,v);
    }
}

void RealDialogControl::changefocus(Dialog *dialog,int updatescreen)
{RCCHAR string[300],*eptr;
 double x;

  dialog->GetDlgItemText(id,string,300);
  //QString qstring(string);
  strtod(string,&eptr);
  while (*eptr == ' ') eptr++;
  if (*eptr != 0)
    {Expression expression(string);
       if (expression.evaluate(&x) != 0)
         {  if (scaleby == ScaleByLength)
              x = x * db.getmainunitscale();
            else if (scaleby == ScaleByArea)
              x = x * db.getmainunitscale() * db.getmainunitscale();
            else if (scaleby == ScaleByVolume)
              x = x * db.getmainunitscale() * db.getmainunitscale() * db.getmainunitscale();
            change(dialog,x);
         }
    }
  DialogControl::changefocus(dialog,updatescreen);
}

#ifdef USING_WIDGETS
// KMJ: new
void RealDialogControl::setSpecialValueText(RCCHAR *text)
{
    strcpy(specialValueText,text);
    if(getQW() != 0)
        ((QDoubleSpinBox*)getQW())->setSpecialValueText(RCSTRING(specialValueText));
}
#endif

int RealDialogControl::currentvalue(Dialog *dialog,double *newvalue)
{RCCHAR string[300],*eptr;
 double x;
  dialog->GetDlgItemText(id,string,128);
  if(QString(string) == "*Varies*")
      strcpy(string,"0.0");
  x = strtod(string,&eptr);
  if (scaleby == ScaleByLength)
    x = x * db.getmainunitscale();
  else if (scaleby == ScaleByArea)
    x = x * db.getmainunitscale() * db.getmainunitscale();
  else if (scaleby == ScaleByVolume)
    x = x * db.getmainunitscale() * db.getmainunitscale() * db.getmainunitscale();
  if (*eptr != 0) return 0;

 if (rmin != 0.0 || rmax != 0.0)
    {  if (x < rmin) x = rmin;
       if (x > rmax) x = rmax;
    }

  *newvalue = x;
  return 1;
}

IntegerDialogControl::IntegerDialogControl(int i,int *v) : DialogControl(i)
{  ivalue = v;  lvalue = NULL;  svalue = NULL;  imin = imax = 0;
    strcpy(specialValueText,_RCT(""));
}

IntegerDialogControl::IntegerDialogControl(int i,short *v) : DialogControl(i)
{  svalue = v;  lvalue = NULL;  ivalue = NULL;  imin = imax = 0;
    strcpy(specialValueText,_RCT(""));
}

IntegerDialogControl::IntegerDialogControl(int i,long *v) : DialogControl(i)
{  lvalue = v;  svalue = NULL;  ivalue = NULL;  imin = imax = 0;
    strcpy(specialValueText,_RCT(""));
}

IntegerDialogControl::IntegerDialogControl(int i,int *v,long imin1,long imax1) : DialogControl(i)
{  ivalue = v;  lvalue = NULL;  svalue = NULL;  imin = imin1;  imax = imax1;
   strcpy(specialValueText,_RCT(""));
}

IntegerDialogControl::IntegerDialogControl(int i,short *v,long imin1,long imax1) : DialogControl(i)
{  svalue = v;  lvalue = NULL;  ivalue = NULL;  imin = imin1;  imax = imax1;
    strcpy(specialValueText,_RCT(""));
}

IntegerDialogControl::IntegerDialogControl(int i,long *v,long imin1,long imax1) : DialogControl(i)
{  lvalue = v;  svalue = NULL;  lvalue = NULL;  imin = imin1;  imax = imax1;
    strcpy(specialValueText,_RCT(""));
}

void IntegerDialogControl::load(Dialog *dialog)
{RCCHAR string[300];
#ifdef USING_WIDGETS
  // KMJ: new
  if(getQW() != 0 && qobject_cast<QSpinBox*>(getQW()) != 0)
      ((QSpinBox*)getQW())->setSpecialValueText(QString(specialValueText));
#endif
  if (ivalue != NULL)
     sprintf(string,"%d",*ivalue);
  else if (svalue != NULL)
    sprintf(string,"%d",*svalue);
  else if (lvalue != NULL)
    sprintf(string,"%ld",*lvalue);
  dialog->SetDlgItemText(id,string);
}

void IntegerDialogControl::vscrollevent(Dialog *dialog,int,int scrollcode,int)
{long v;
 RCCHAR string[300];
  if (currentvalue(dialog,&v))
    {  if (scrollcode == SB_LINEDOWN)
         v--;
         else if (scrollcode == SB_LINEUP)
         v++;
       if (imin != 0 || imax != 0)
         {  if (v < imin) v = imin;
            if (v > imax) v = imax;
         }
       sprintf(string,"%ld",v);
       dialog->SetDlgItemText(id,string);
    }
}

void IntegerDialogControl::change(Dialog *dialog,long v)
{RCCHAR string[300];
  sprintf(string,"%ld",v);
  dialog->SetDlgItemText(id,string);
}

int IntegerDialogControl::store(Dialog *dialog)
{RCCHAR string[300],*eptr;
 long i;
 ResourceString rs1(NCDIALOG+1);
 ResourceString rs2(NCDIALOG+2);
  dialog->GetDlgItemText(id,string,128);
  i = strtol(string,&eptr,10);
  if (*eptr != 0)
    {  dialog->MessageBox(rs1.getws(),_RCT(""),MB_ICONSTOP);
       return FALSE;
    }
  if ((imin != 0 || imax != 0) && (i < imin || i > imax))
    {  dialog->MessageBox(rs2.getws(),_RCT(""),MB_ICONSTOP);
       return FALSE;
    }
  if (ivalue != NULL)
   *ivalue = int(i);
  else if (svalue != NULL)
   *svalue = short(i);
  else if (lvalue != NULL)
   *lvalue = i;
  return TRUE;
}

void IntegerDialogControl::changefocus(Dialog *dialog,int updatescreen)
{RCCHAR string[300],*eptr;
 long i;
  dialog->GetDlgItemText(id,string,128);
  strtol(string,&eptr,10);
  while (*eptr == ' ') eptr++;
  if (*eptr != 0)
    {Expression expression(string);
       if (expression.evaluate(&i) != 0)
         {
            sprintf(string,"%ld",i);
            dialog->SetDlgItemText(id,string);
         }
    }
  DialogControl::changefocus(dialog,updatescreen);
}

int IntegerDialogControl::validate(Dialog *dialog)
{RCCHAR string[300],*eptr;
 long i;
 ResourceString rs1(NCDIALOG+1);
 ResourceString rs2(NCDIALOG+2);
  dialog->GetDlgItemText(id,string,128);
  i = strtol(string,&eptr,10);
  if (*eptr != 0)
    {  dialog->MessageBox(rs1.getws(),_RCT(""),MB_ICONSTOP);
       return FALSE;
    }
  if ((imin != 0 || imax != 0) && (i < imin || i > imax))
    {  dialog->MessageBox(rs2.getws(),_RCT(""),MB_ICONSTOP);
       return FALSE;
    }
  return TRUE;
}

int IntegerDialogControl::currentvalue(Dialog *dialog,long *newvalue)
{RCCHAR string[300],*eptr;
 long i;
  dialog->GetDlgItemText(id,string,300);
  i = strtol(string,&eptr,10);
  if (*eptr != 0) return 0;
  *newvalue = i;
  return 1;
}

int IntegerDialogControl::currentvalue(Dialog *dialog,int *newvalue)
{long l;
  if (! currentvalue(dialog,&l)) return 0;
  *newvalue = int(l);
  return 1;
}

int IntegerDialogControl::currentvalue(Dialog *dialog,short *newvalue)
{long l;
  if (! currentvalue(dialog,&l)) return 0;
  *newvalue = short(l);
  return 1;
}

#ifdef USING_WIDGETS
// KMJ: new
void IntegerDialogControl::setSpecialValueText(RCCHAR *text)
{
    strcpy(specialValueText,text);
    if(getQW() != 0)
        ((QSpinBox*)getQW())->setSpecialValueText(RCSTRING(specialValueText));
}
#endif
void CheckBoxDialogControl::load(Dialog *dialog)
{
#if ! defined(_WIN32_WCE)
  CheckDlgButton(dialog->gethdlg(),id,oldvalue);
//WCEFIX
#endif
}

void CheckBoxDialogControl::change(Dialog *dialog,long v)
{ oldvalue = int(v);
#if ! defined(_WIN32_WCE)
  CheckDlgButton(dialog->gethdlg(),id,int(v));
//WCEFIX
#endif
}

int CheckBoxDialogControl::select(Dialog *dialog)
{ oldvalue = ! oldvalue;
#if ! defined(_WIN32_WCE)
  CheckDlgButton(dialog->gethdlg(),id,oldvalue);
//WCEFIX
#endif
  changefocus(dialog);
  return 0;
}

int CheckBoxDialogControl::store(Dialog *)
{ if (ivalue != NULL)
    *ivalue = oldvalue;
  else if (svalue != NULL)
    *svalue = short(oldvalue);
  else if (lvalue != NULL)
    *lvalue = oldvalue;
  else if (bvalue != NULL)
    {  if (oldvalue) bvalue->set(bit); else bvalue->clear(bit);
    }
  return (TRUE);
}

int CheckBoxDialogControl::currentvalue(Dialog *,int *newvalue)
{ *newvalue = oldvalue;
  return 1;
}

int CheckBoxDialogControl::currentvalue(Dialog *,BitMask *newvalue)
{ if (oldvalue) newvalue->set(bit); else newvalue->clear(bit);
  return 1;
}

int CheckBoxDialogControl::currentvalue(Dialog *,short *newvalue)
{ *newvalue = short(oldvalue);
  return 1;
}

int CheckBoxDialogControl::currentvalue(Dialog *,long *newvalue)
{ *newvalue = oldvalue;
  return 1;
}

void TriStateCheckBoxDialogControl::load(Dialog *dialog)
{
    CheckBoxDialogControl::load(dialog);
    if(getQW())
        ((QCheckBox*)getQW())->setTristate(triState);
}

int TriStateCheckBoxDialogControl::select(Dialog *dialog)
{
    if(triState)
    {
        oldvalue++;
        if(oldvalue > 2) oldvalue=0;
    }
    else
        oldvalue = ! oldvalue;
    CheckDlgButton(dialog->gethdlg(),id,oldvalue);
    changefocus(dialog);
    return 0;
}

int TriStateCheckBoxDialogControl::change(Dialog *dialog,long v)
{
    oldvalue = int(v);
    CheckDlgButton(dialog->gethdlg(),id,int(v));
    return 1;
}

void RadioButtonDialogControl::load(Dialog *dialog)
{
#if ! defined(_WIN32_WCE)
  CheckDlgButton(dialog->gethdlg(),id,oldvalue == id - id1);
#endif
}

void RadioButtonDialogControl::selectitem(Dialog *dialog)
{
    SetFocus(GetDlgItem(dialog->gethdlg(),id));
}

int RadioButtonDialogControl::select(Dialog *dialog)
{int i;
  oldvalue = id - id1;
#if ! defined(_WIN32_WCE)
  for (i = id1 ; i <= id2 ; i++)
    CheckDlgButton(dialog->gethdlg(),i,oldvalue == i - id1);
//WCEFIX
#endif
  changefocus(dialog);
  return 0;
}

void RadioButtonDialogControl::change(Dialog *dialog,int v)
{int i;
#if ! defined(_WIN32_WCE)
  for (i = id1 ; i <= id2 ; i++)
    CheckDlgButton(dialog->gethdlg(),i,v == i - id1);
//WCEFIX
#endif
}

int RadioButtonDialogControl::store(Dialog *dialog)
{
    int i;
#if ! defined(_WIN32_WCE)
    *value = 0;
    for (i = id1 ; i <= id2 ; i++)
       if (IsDlgButtonChecked(dialog->gethdlg(),i))
          *value = i - id1;
//WCEFIX
#endif
    return (TRUE);
}

int RadioButtonDialogControl::currentvalue(Dialog *dialog,int *newvalue)
{
    int i;
    *newvalue = 0;
#if ! defined(_WIN32_WCE)
    for (i = id1 ; i <= id2 ; i++)
      if (IsDlgButtonChecked(dialog->gethdlg(),i))
        *newvalue = i - id1;
#endif
    return 1;
}
Dialog::Dialog(const char *n,int m)
{
    strcpy(name,n);  strcpy(dtitle,"");  strcpy(dlabel,"");  modal = m;  hDlg = 0;
    hglobaldialog = 0;
    position = 0;
}

Dialog::Dialog(const RCCHAR *n,int m)
{
  strcpy(name,n);  strcpy(dtitle,"");  strcpy(dlabel,"");  modal = m;  hDlg = 0;
  hglobaldialog = 0;
  position = 0;
  m_qw = 0;
}

Dialog::Dialog(const char *n)
{
  strcpy(name,n);  strcpy(dtitle,"");  strcpy(dlabel,"");  modal = 0;
  hglobaldialog = 0;
  position = 0;
  hDlg = 0;
  m_qw = 0;
}

Dialog::Dialog(const RCCHAR *n)
{
  strcpy(name,n);  strcpy(dtitle,"");  strcpy(dlabel,"");  modal = 0;
  hglobaldialog = 0;
  position = 0;
  hDlg = 0;
  m_qw = 0;

}

Dialog::Dialog(const char *t,const char *l,double *value,ScaleBy sb)
{
    DialogControl *dc;
    strcpy(name,"Standard_Dbl_Dialog");  strcpy(dtitle,t);  strcpy(dlabel,l);  modal = 0;
    add(dc = new RealDialogControl(101,value,sb));
    //add(new ScrollBarDialogControl(1101,dc));
    hglobaldialog = 0;
    position = 0;
    hDlg = 0;
    m_qw = 0;
}

Dialog::Dialog(const RCCHAR *t,const RCCHAR *l,double *value,ScaleBy sb)
{
  DialogControl *dc;
  strcpy(name,"Standard_Dbl_Dialog");  strcpy(dtitle,t);  strcpy(dlabel,l);  modal = 0;
  add(dc = new RealDialogControl(101,value,sb));
  //add(new ScrollBarDialogControl(1101,dc));
  hglobaldialog = 0;
  position = 0;
  hDlg = 0;
  m_qw = 0;

}

Dialog::Dialog(const RCCHAR *t,const RCCHAR *l,double *value,double step,double rmin,double rmax,ScaleBy sb)
{
    DialogControl *dc;
  strcpy(name,_RCT("Standard_Dbl_Dialog"));  strcpy(dtitle,t);  strcpy(dlabel,l);  modal = 0;
  add(dc = new RealDialogControl(101,value,step,rmin,rmax,sb));
  //add(new ScrollBarDialogControl(1101,dc));
  hglobaldialog = 0;
  position = 0;
  hDlg = 0;
  m_qw = 0;

}

Dialog::Dialog(const RCCHAR *t,const RCCHAR *l,int *value)
{
    DialogControl *dc;
  strcpy(name,_RCT("Standard_Int_Dialog"));  strcpy(dtitle,t);  strcpy(dlabel,l);  modal = 0;
  add(dc = new IntegerDialogControl(101,value));
  //add(new ScrollBarDialogControl(1101,dc));
  hglobaldialog = 0;
  position = 0;
  hDlg = 0;
  m_qw = 0;

}

Dialog::Dialog(const RCCHAR *t,const RCCHAR *l,int *value,long imin,long imax)
{
    DialogControl *dc;
  strcpy(name,_RCT("Standard_Int_Dialog"));  strcpy(dtitle,t);  strcpy(dlabel,l);  modal = 0;
  add(dc = new IntegerDialogControl(101,value,imin,imax));
  //add(new ScrollBarDialogControl(1101,dc));
  hglobaldialog = 0;
  position = 0;
  hDlg = 0;
  m_qw = 0;

}

Dialog::Dialog(const RCCHAR *t,const RCCHAR *l,RCCHAR *value,int len)
{
  strcpy(name,_RCT("StandardString_Dialog"));  strcpy(dtitle,t);  strcpy(dlabel,l);  modal = 0;
  add(new StringDialogControl(101,value,len));
  hglobaldialog = 0;
  position = 0;
  hDlg = 0;
  m_qw = 0;

}

#if ! defined(_WIN32_WCE)
void Dialog::SetDlgItemText(int id,char *string )
{
    SetDlgItemText(id,CHAR2RCCHR(string));
}

void Dialog::SetDlgItemText(int id,RCCHAR *string )
{
    ::SetDlgItemText(hDlg,id,string);
}

void Dialog::GetDlgItemText(int id,char *string,int l)
{
    GetDlgItemText(id,CHAR2RCCHR(string),l);
}

void Dialog::GetDlgItemText(int id,RCCHAR *string,int l)
{
    ::GetDlgItemTextW(hDlg,id,string,l);
}

int Dialog::MessageBox(const RCCHAR *message,const RCCHAR *title,int type)
{
    //returnRCMessageBox(hDlg,message,title,type);
    return ::MessageBox(hDlg,message,title,type);
}

int Dialog::MessageBox(const RCCHAR *message,const char *title,int type)
{
    return ::MessageBox(hDlg,message,CHAR2RCCHR(title),type);
}

int Dialog::MessageBox(const char *message,const char *title,int type)
{
    return ::MessageBox(hDlg,CHAR2RCCHR(message),CHAR2RCCHR(title),type);
}

int MainWindow::MessageBox(const RCCHAR *message,const RCCHAR *title,int type)
{
    return ::MessageBox(gethwnd(), (const RCCHAR*)message, (const RCCHAR*)title ,(RCINT)type);
}

int MainWindow::MessageBox(const RCCHAR *message,const char *title,int type)
{
    return ::MessageBox(gethwnd(), message, CHAR2RCCHR(title) ,type);
}

int MainWindow::MessageBox(const char *message,const char *title,int type)
{
    return ::MessageBox(gethwnd(), message, title ,type);
}

#endif


static Dialog *current = NULL;
static int selchange = 0;
static HHOOK hhook;
#if ! defined(_WIN32_WCE)
static HOOKPROC hookproc;
//WCEFIX
#endif

static int lastlbutton = 0;
static int lastrbutton = 0;


void EnterIdle(void)
{
#ifdef USING_WIDGETS
    //qDebug() << "in EnterIdle";
    if (current != 0 && current->gethdlg() != 0)
    {
       POINT p;
       WINDOWPLACEMENT wp;
       GetCursorPos(&p);
       //memset(&wp,0,sizeof(wp));
       //wp.length = sizeof(wp);
       //GetWindowPlacement(current->gethdlg(),&wp);

       QRect dialogRect = ((RCDialog*)current->gethdlg())->frameGeometry();
       dialogRect.setTopLeft(((RCDialog*)current->gethdlg())->mapToGlobal(dialogRect.topLeft()));
       dialogRect.setSize(((RCDialog*)current->gethdlg())->size());
       //dialogRect.setBottomRight(((RCDialog*)current->gethdlg())->mapToGlobal(dialogRect.bottomRight()));
       QPoint screenPoint(p.x,p.y);

       if(app->mouseButtons() == Qt::LeftButton)
           qDebug() << "mousebuts        : left button";
       else if(app->mouseButtons() == Qt::RightButton)
           qDebug() << "mousebuts        : right button";
       else if(app->mouseButtons() == Qt::MiddleButton)
           qDebug() << "mousebuts        : middle button";
       //qDebug() << "dialogRect  " << dialogRect;
       //qDebug() << "screenPoint " << screenPoint;

       //if (! PtInRect(&wp.rcNormalPosition,p))
       //if (dialogRect.contains(screenPoint))
       if ( ! ((RCDialog*)current->gethdlg())->underMouse() && ((RCDialog*)current->gethdlg())->isVisible())
       {
           //qDebug() << "dialog not under mouse  ";
           if ((GetKeyState(VK_LBUTTON)&0x8000) && GetDlgItem(current->gethdlg(),IDOK) != 0)
           {
               //if (lastlbutton == 0 && v.getinteger("wn::dialogboxautook") != 0)
               if (lastlbutton == 0)
                   PostMessage(current->gethdlg(),WM_COMMAND,IDOK,0);
                 lastlbutton = 1;
           }
           else
              lastlbutton = 0;

            if (GetKeyState(VK_RBUTTON)&0x8000)
            {
                //if (lastrbutton == 0 && v.getinteger("wn::dialogboxautook") != 0)
                if (lastrbutton == 0)
                   PostMessage(current->gethdlg(),WM_COMMAND,IDCANCEL,0);
                 lastrbutton = 1;
            }
            else
              lastrbutton = 0;
       }
       else
       {
            //qDebug() << "dialog under mouse  ";
            lastlbutton = (GetKeyState(VK_LBUTTON)&0x8000) != 0;
            lastrbutton = (GetKeyState(VK_RBUTTON)&0x8000) != 0;
       }
    }
#else
    if (current != 0)
    {
       POINT p;
       WINDOWPLACEMENT wp;
       GetCursorPos(&p);
       memset(&wp,0,sizeof(wp));
       wp.length = sizeof(wp);
       GetWindowPlacement(current->gethdlg(),&wp);
       if (! PtInRect(&wp.rcNormalPosition,p))
       {
           if ((GetKeyState(VK_LBUTTON)&0x8000) && GetDlgItem(current->gethdlg(),IDOK) != 0)
           {
               if (lastlbutton == 0 && v.getinteger("wn::dialogboxautook") != 0)
                   PostMessage(current->gethdlg(),WM_COMMAND,IDOK,0);
                 lastlbutton = 1;
           }
           else
              lastlbutton = 0;

            if (GetKeyState(VK_RBUTTON)&0x8000)
            {
                if (lastrbutton == 0 && v.getinteger("wn::dialogboxautook") != 0)
                   PostMessage(current->gethdlg(),WM_COMMAND,IDCANCEL,0);
                 lastrbutton = 1;
            }
            else
              lastrbutton = 0;
       }
       else
       {
            lastlbutton = (GetKeyState(VK_LBUTTON)&0x8000) != 0;
            lastrbutton = (GetKeyState(VK_RBUTTON)&0x8000) != 0;
       }
    }
#endif
}


LRESULT FAR PASCAL customcontrolcb(int code,WPARAM wParam,LPARAM lParam)
{CustomDialogControl *cdc;
 MSG *msg = (MSG *) lParam;
 DialogControl *dc;
 RCCHAR value[256];
 int id;

  if (code < 0)
    {  if (hhook != 0)
#if ! defined(_WIN32_WCE)
         return CallNextHookEx(hhook,code,wParam,lParam);
#else
         ;
#endif
       else
         return 0L;
    }
  if (msg->message == WM_KEYUP && msg->wParam == VK_F2 && GetParent(msg->hwnd) == current->gethdlg())
    {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
         if (dc->getid() == GetDlgCtrlID(msg->hwnd) && (dc->type() == RealControl || dc->type() == IntegerControl))
           {
#if ! defined(_WIN32_WCE)
              GetDlgItemText(GetParent(msg->hwnd),dc->getid(),value,256);
              if (calculator.process(GetParent(msg->hwnd),value) == TRUE)
                {  if (dc->type() == RealControl)
                     SetDlgItemText(GetParent(msg->hwnd),dc->getid(),calculator.getreal());
                   else if (dc->type() == IntegerControl)
                     SetDlgItemText(GetParent(msg->hwnd),dc->getid(),calculator.getinteger());
                   if (hhook != 0)
                     return CallNextHookEx(hhook,code,wParam,lParam);
                   else
                     return 0L;
                }
//WCEFIX
#endif
           }
    }
  if (msg->message == WM_KEYUP && msg->wParam != VK_TAB && msg->wParam != VK_RETURN  && msg->wParam != 229 && GetParent(msg->hwnd) == current->gethdlg())
    {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
         if (dc->getid() == GetDlgCtrlID(msg->hwnd) && (dc->getid() < 200 || dc->getid() > 299))
           {  id = 200 + (dc->getid() % 100);
              for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                if (dc->getid() == id && dc->type() == CheckBoxControl)
                  ((CheckBoxDialogControl *)dc)->change(current,1);
              break;
           }
    }
  for (current->customlist.start() ; (cdc = (CustomDialogControl *)current->customlist.next()) != NULL ; )
    if (cdc->gethwnd() == msg->hwnd || msg->message == WM_TIMER)
      {  switch (msg->message)
           {  case WM_LBUTTONDOWN :
                cdc->lbuttondownevent(current,LOWORD(msg->lParam),HIWORD(msg->lParam),msg->wParam);
                return 1L;
              case WM_LBUTTONUP :
                cdc->lbuttonupevent(current,LOWORD(msg->lParam),HIWORD(msg->lParam),msg->wParam);
                return 1L;
              case WM_MOUSEMOVE :
                cdc->mousemoveevent(current,LOWORD(msg->lParam),HIWORD(msg->lParam),msg->wParam);
                return 1L;
              case WM_TIMER :
                cdc->timerevent(current);
                return 0L;
           }
         if (msg->message == WM_LBUTTONDBLCLK)
           PostMessage(current->gethdlg(),WM_COMMAND,IDOK,0);
         return 0L;
      }

  if (msg->message == WM_LBUTTONDBLCLK)
    {RCCHAR classname[300];
       GetClassName(msg->hwnd,classname,300);
       if (_tcscmp(classname,TEXT("Edit")) == 0)
         {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
              if (dc->getid() == GetDlgCtrlID(msg->hwnd))
                dc->lbuttondblclkevent(current,LOWORD(lParam),HIWORD(lParam),wParam);
            return FALSE;
         }
       else if (_tcscmp(classname,TEXT("#32770")) == 0)
         PostMessage(current->gethdlg(),WM_COMMAND,IDOK,0);
    }
  else if (msg->message == WM_LBUTTONDOWN)
    {  if (cadwindow != 0 && msg->hwnd == cadwindow->subwindow->gethwnd())
         PostMessage(current->gethdlg(),WM_COMMAND,IDOK,0);
       lastlbutton = 1;
    }
  else if (msg->message == WM_LBUTTONUP)
    lastlbutton = 0;
  else if (msg->message == WM_RBUTTONDOWN)
    lastrbutton = 1;
  else if (msg->message == WM_RBUTTONUP)
    lastrbutton = 0;

  if (hhook != 0)
#if ! defined(_WIN32_WCE)
    return CallNextHookEx(hhook,code,wParam,lParam);
//WCEFIX
#else
    return 0L;
#endif
  else
    return 0L;
}

extern "C" long FAR PASCAL dialogcb(HWND,UINT,WPARAM,LPARAM);

#define UDS_ARROWKEYS 0x20

/*
typedef struct _NM_UPDOWN {
NMHDR hdr;
int iPos;
int iDelta;
} NMUPDOWN;
*/

long FAR PASCAL dialogcb(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
 DRAWITEMSTRUCT *dis;
 DialogControl *dc,*lastdc;
 CustomDialogControl *cdc;
 DisplayDialogControl *ddc;
 int id,ncode,scode,spos,left,right,top,bottom;
 static RCCHAR string[256];
 RCCHAR s[256],title[300],*sp,text[300];
 HWND hcontrol;
 GeneralList list;
 WINDOWPLACEMENT dwp,pwp;

  if (selchange && current != 0 && hDlg == current->hDlg)
    {
#if ! defined(_WIN32_WCE)
       GetDlgItemText(hDlg,selchange,s,255);
//WCEFIX
#endif
       if (strcmp(s,string) != 0)
         {  id = selchange;  selchange = 0;
            for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
              if (dc->getid() == id && (dc->getid() < 200 || dc->getid() > 299))
                {  dc->changefocus(current);
                   for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                     if (dc->getid() == 200 + (id % 100) && dc->type() == CheckBoxControl)
                       ((CheckBoxDialogControl *)dc)->change(current,1);
                   break;
               }
        }
    }
  switch (iMessage)
    {
#if defined(WIN32)
//       case WM_CTLCOLOREDIT:
//       case WM_CTLCOLORLISTBOX:
//       case WM_CTLCOLORMSGBOX:
       case WM_CTLCOLORBTN:
//       case WM_CTLCOLORSCROLLBAR:
       case WM_CTLCOLORSTATIC:
       case WM_CTLCOLORDLG:


         SetBkMode((HDC)wParam,TRANSPARENT);

         id = GetDlgCtrlID((HWND)lParam);
         for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
            if ((dc->getoptions() & DC_NOERASEBACKGROUND) != 0 && dc->getid() == id)
              return (long) GetStockObject(NULL_BRUSH);
         for (current->displaylist.start() ; (dc = (DialogControl *)current->displaylist.next()) != NULL ; )
            if ((dc->getoptions() & DC_NOERASEBACKGROUND) != 0 && dc->getid() == id)
              return (long) GetStockObject(NULL_BRUSH);

         return (long) program->getbuttonfacebrush();
#endif
       case WM_INITDIALOG:
         lastlbutton = 1;  //  Disable any button click that may still be happening
         lastrbutton = 1;
         current->hDlg = hDlg;

         //  Find any scroll bars, and if they are small, convert to spin controls
         for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
           if (dc->type() == ScrollBarControl)
             {RECT rect;
              POINT p;
                GetWindowRect(GetDlgItem(hDlg,dc->getid()),&rect);
                p.x = rect.left;  p.y = rect.top;
                ScreenToClient(hDlg,&p);
                rect.left = p.x;  rect.top = p.y;
                p.x = rect.right;  p.y = rect.bottom;
                ScreenToClient(hDlg,&p);
                rect.right = p.x;  rect.bottom = p.y;

                if (rect.right - rect.left < 20 && rect.bottom - rect.top < 25)
                  {//  Convert to spin control
                    RCHWND hWnd = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR,_RCT("msctls_updown32"),_RCT(""),WS_VISIBLE | WS_CHILDWINDOW | UDS_ARROWKEYS,
                               rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,hDlg,(HMENU)dc->getid(),program->gethinst(),0);
                    if (hWnd != 0)
                      {  SendMessage(hWnd,SBM_SETRANGE,-32767,32767);
                         DestroyWindow(GetDlgItem(hDlg,dc->getid()));
                      }
                  }
             }

         for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
           {  ShowWindow(GetDlgItem(hDlg,dc->getid()),SW_SHOWNORMAL);
              EnableWindow(GetDlgItem(hDlg,dc->getid()),TRUE);
           }

         for (current->disablelist.start() ; (id = (LONG) current->disablelist.next()) != 0 ; )
           EnableWindow(GetDlgItem(hDlg,id),FALSE);

         list = current->controllist;
         for (list.start(),lastdc = NULL ; (dc = (DialogControl *)list.next()) != 0 ; lastdc = dc)
           dc->load(current);

#if ! defined(_WIN32_WCE)
         if (strlen(current->dtitle) > 0)
           SetWindowText(hDlg,current->dtitle);
         if (strlen(current->dlabel) > 0)
           SetDlgItemText(hDlg,100,current->dlabel);
//WCEFIX
#endif
         if (current->getposition() == DPTR)
           {RECT rect,wrect;
            POINT p;
#if ! defined(_WIN32_WCE)
              GetCursorPos(&p);
              GetClientRect(hDlg,&rect);

              MONITORINFO mi;
              mi.cbSize = sizeof(mi);
              if (GetMonitorInfo(MonitorFromWindow(hDlg,MONITOR_DEFAULTTOPRIMARY),&mi))
                {  wrect = mi.rcMonitor;
                }
              else
                GetWindowRect(GetDesktopWindow(),&wrect);

              p.x -= rect.right;
              if (p.x < wrect.left) p.x = wrect.left;
              if (p.y < wrect.top) p.y = wrect.top;
              if (p.x > wrect.right - rect.right) p.x = wrect.right - rect.right;
              if (p.y > wrect.bottom - rect.bottom - 70) p.y = wrect.bottom - rect.bottom - 70;
              SetWindowPos(hDlg,NULL,p.x,p.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
//WCEFIX
#endif
           }
         else if (current->getposition() == DPOK)
           {RECT rect1,rect2;
            POINT p;
#if ! defined(_WIN32_WCE)
              GetCursorPos(&p);
              GetClientRect(hDlg,&rect1);
              p.x -= (int)(rect1.right / 2.0);
              p.y -= rect1.bottom;

              MONITORINFO mi;
              mi.cbSize = sizeof(mi);
              if (GetMonitorInfo(MonitorFromWindow(hDlg,MONITOR_DEFAULTTOPRIMARY),&mi))
                {  rect2 = mi.rcMonitor;
                }
              else
                GetWindowRect(GetDesktopWindow(),&rect2);

              if (p.x < rect2.left) p.x = rect2.left;
              if (p.y < rect2.top) p.y = rect2.top;
              if (p.x + rect1.right > rect2.right) p.x = rect2.right - rect1.right;
              if (p.y + rect1.bottom + 100 > rect2.bottom) p.y = rect2.bottom - rect1.bottom - 100;
              SetWindowPos(hDlg,NULL,p.x,p.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
//WCEFIX
#endif
           }
         else
           {int width,height,dx,dy;
            RECT wrect;

                GetWindowPlacement(current->gethdlg(),&dwp);
                GetWindowPlacement(cadwindow->gethwnd(),&pwp);
                GetWindowText(current->gethdlg(),title,300);
                sp = title;
                while (*sp != 0)
                if (*sp >= '0' && *sp <= '9' || *sp >= 'a' && *sp <= 'z' || *sp >= 'A' && *sp <= 'Z')
                    sp++;
                else
                    *sp++ = '_';
                GetPrivateProfileString(_RCT("DialogLocation"),title,_RCT(""),text,300,home.getinifilename());
                if (sscanf(text,_RCT("%d %d %d %d"),&left,&right,&top,&bottom) == 4)
                {   width = dwp.rcNormalPosition.right - dwp.rcNormalPosition.left;
                    height = dwp.rcNormalPosition.bottom - dwp.rcNormalPosition.top;
                    dwp.rcNormalPosition.left = pwp.rcNormalPosition.left + left;
                    dwp.rcNormalPosition.right = dwp.rcNormalPosition.left + width;
                    dwp.rcNormalPosition.top = top + pwp.rcNormalPosition.top;
                    dwp.rcNormalPosition.bottom = dwp.rcNormalPosition.top + height;


                    MONITORINFO mi;
                    mi.cbSize = sizeof(mi);
                    if (GetMonitorInfo(MonitorFromRect(&dwp.rcNormalPosition,0),&mi))
                    {  wrect = mi.rcMonitor;
                    }
                    else
                        GetWindowRect(GetDesktopWindow(),&wrect);

                    dx = 0;
                    dy = 0;
                    if (dwp.rcNormalPosition.left < wrect.left)
                        dx = wrect.left - dwp.rcNormalPosition.left;
                    if (dwp.rcNormalPosition.top < wrect.top)
                        dy = wrect.top - dwp.rcNormalPosition.top;
                    if (dwp.rcNormalPosition.right > wrect.right)
                        dx = wrect.right - dwp.rcNormalPosition.right;
                    if (dwp.rcNormalPosition.bottom > wrect.bottom)
                        dy = wrect.bottom - dwp.rcNormalPosition.bottom;

                    dwp.rcNormalPosition.left += dx;
                    dwp.rcNormalPosition.top += dy;
                    dwp.rcNormalPosition.right += dx;
                    dwp.rcNormalPosition.bottom += dy;

                    SetWindowPlacement(current->gethdlg(),&dwp);
                }

           }

         while (commandfile.getnextdialog(&id,s))
           {  if (id == IDOK)
                {  if (current->validate())
                     {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != 0 ; )
                          dc->store(current);
                        selchange = 0;
                        EndDialog(hDlg,TRUE);
                        SetFocus(cadwindow->gethwnd());
                     }
                   break;
                }
              else if (id == IDCANCEL)
                {  EndDialog(hDlg,FALSE);
                   SetFocus(cadwindow->gethwnd());
                   break;
                }
#if ! defined(_WIN32_WCE)
              else
                SetDlgItemText(hDlg,id,s);
//WCEFIX
#endif
           }
         if (lastdc != NULL && (lastdc->type() == RealControl || lastdc->type() == IntegerControl ||
                                lastdc->type() == ListControl || lastdc->type() == StringControl))
           {  lastdc->selectitem(current);  return FALSE;
           }
         break;
       case WM_COMMAND:
         if (current != 0 && hDlg != current->hDlg)
           return FALSE;

         ncode = HIWORD(wParam);
         id = LOWORD(wParam);

         if (ncode == EN_SETFOCUS || ncode == CBN_SETFOCUS)
           {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                if (dc->getid() == id)
                  {  dc->select(current);
                     return TRUE;
                  }
              return FALSE;
           }
         else if (ncode == LBN_DBLCLK)
           {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                if (dc->getid() == id)
                  dc->lbuttondblclkevent(current,LOWORD(lParam),HIWORD(lParam),wParam);
              return FALSE;
           }
         else if (ncode == BN_CLICKED && id >= 99)
           {  if ((LOWORD(GetWindowLong(GetDlgItem(hDlg,id),GWL_STYLE))&BS_PUSHBUTTON) == BS_PUSHBUTTON ||
                  LOWORD(GetWindowLong(GetDlgItem(hDlg,id),GWL_STYLE)) == BS_DEFPUSHBUTTON)
                {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                     if (dc->getid() == id)
                       {  if (dc->select(current))
                            {  selchange = 0;
                               EndDialog(hDlg,id);
                            }

                          if (dc->type() == RadioButtonControl)
                            id = ((RadioButtonDialogControl *)dc)->getid1();  //  Use the first control id of the set.

                          for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                            {
                              if (dc->getid() == 200 + (id % 100) && dc->type() == CheckBoxControl && id < 200)
                                 ((CheckBoxDialogControl *)dc)->change(current,1);
                            }

                          return TRUE;
                       }
                   return FALSE;
                }
              else
                {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                     if (dc->getid() == id)
                       {  dc->select(current);
                          if (id < 200 || id > 299)
                            for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                              if (dc->getid() == 200 + (id % 100) && dc->type() == CheckBoxControl)
                                ((CheckBoxDialogControl *)dc)->change(current,1);
                          return TRUE;
                       }
                   return FALSE;
                }
           }
         else if (ncode == EN_KILLFOCUS || ncode == CBN_KILLFOCUS || ncode == LBN_SELCHANGE)
           {  selchange = id;
              GetDlgItemText(hDlg,id,string,255);
              for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                if (dc->getid() == id)
                  dc->changefocus(current);
              return FALSE;
           }
         else if (ncode == CBN_SELCHANGE)
           {  selchange = id;
              GetDlgItemText(hDlg,id,string,255);
           }
         else
           switch (id)
             {  case IDOK:

                  //  First evaluate any expressions
                  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                    dc->changefocus(current,0);

                  if (current->validate())
                    {  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
                         dc->store(current);
                       selchange = 0;
                       EndDialog(hDlg,TRUE);
                    }
                  else
                    return FALSE; // don't close the dialog when there is a numeric entry error
                    //return TRUE;
                  break;
                case IDCANCEL:
                case IDYES:
                case IDNO:
                  selchange = 0;
                  if (id == IDCANCEL)
                    EndDialog(hDlg,FALSE);
                  else
                    EndDialog(hDlg,id);
                  break;
              }
          break;
        case WM_DRAWITEM :
          if (current != 0 && hDlg != current->hDlg)
            return FALSE;
          dis = (DRAWITEMSTRUCT *) lParam;
          if (dis->itemAction  == ODA_DRAWENTIRE)
            {  for (current->customlist.start() ; (cdc = (CustomDialogControl *)current->customlist.next()) != NULL ; )
                 if (unsigned(cdc->getid()) == wParam)
                   {  cdc->sethwnd(GetDlgItem(hDlg,cdc->getid()));
                      cdc->draw(current,dis->hDC);
                   }
               for (current->displaylist.start() ; (ddc = (DisplayDialogControl *)current->displaylist.next()) != NULL ; )
                 if (unsigned(ddc->getid()) == wParam)
                   {  ddc->sethwnd(GetDlgItem(hDlg,ddc->getid()));
                      ddc->draw(current,dis->hDC);
                   }
               return FALSE;
            }
          else
            return DefWindowProc(hDlg, iMessage, wParam, lParam);
        case WM_VSCROLL :
          if (current != 0 && hDlg != current->hDlg)
            return FALSE;
#if defined(WIN32) || defined(_MAC)
          hcontrol = (HWND) lParam;
          scode = LOWORD(wParam);
          spos = HIWORD(wParam);
#else
          hcontrol = (HWND) HIWORD(lParam);
          scode = wParam;
          spos = LOWORD(lParam);
#endif
          for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
            if (dc->getid() == GetDlgCtrlID(hcontrol))
              dc->vscrollevent(current,dc->getid(),scode,spos);
          return FALSE;

        case WM_NOTIFY :
          if (current != 0 && hDlg != current->hDlg)
            return FALSE;
#if defined(WIN32) || defined(_MAC)
          hcontrol = (HWND) lParam;
          scode = LOWORD(wParam);
          spos = HIWORD(wParam);
#else
          hcontrol = (HWND) HIWORD(lParam);
          scode = wParam;
          spos = LOWORD(lParam);
#endif

          for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
            if (((NMUPDOWN *)lParam)->hdr.code == 4294966574 && dc->getid() == wParam && dc->type() == ScrollBarControl)
              dc->vscrollevent(current,dc->getid(),((NMUPDOWN *)lParam)->iDelta > 0 ? SB_LINEDOWN : SB_LINEUP,0);
          return FALSE;

        case WM_HSCROLL :
          if (current != 0 && hDlg != current->hDlg)
            return FALSE;
#if defined(WIN32) || defined(_MAC)
          hcontrol = (HWND) lParam;
          scode = LOWORD(wParam);
          spos = HIWORD(wParam);
#else
          hcontrol = (HWND) HIWORD(lParam);
          scode = wParam;
          spos = LOWORD(lParam);
#endif
          for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
            if (dc->getid() == GetDlgCtrlID(hcontrol))
              dc->vscrollevent(current,dc->getid(),scode,spos);
          return FALSE;
        case WM_CLOSE:

          GetWindowPlacement(current->gethdlg(),&dwp);
          GetWindowPlacement(cadwindow->gethwnd(),&pwp);
          GetWindowText(current->gethdlg(),title,300);
          sp = title;
          while (*sp != 0)
            if (*sp >= '0' && *sp <= '9' || *sp >= 'a' && *sp <= 'z' || *sp >= 'A' && *sp <= 'Z')
                sp++;
            else
                *sp++ = '_';
          sprintf(text,"%d %d %d %d",dwp.rcNormalPosition.left - pwp.rcNormalPosition.left,dwp.rcNormalPosition.right - pwp.rcNormalPosition.right,dwp.rcNormalPosition.top - pwp.rcNormalPosition.top,dwp.rcNormalPosition.bottom - pwp.rcNormalPosition.bottom);
          WritePrivateProfileString(_RCT("DialogLocation"),title,text,home.getinifilename());

          if (current->getmodal())
            state.sendevent(new ExitEvent);
          return FALSE;

        case WM_DESTROY :
          GetWindowPlacement(current->gethdlg(),&dwp);
          GetWindowPlacement(cadwindow->gethwnd(),&pwp);
          GetWindowText(current->gethdlg(),title,300);
          sp = title;
          while (*sp != 0)
            if (*sp >= '0' && *sp <= '9' || *sp >= 'a' && *sp <= 'z' || *sp >= 'A' && *sp <= 'Z')
                sp++;
            else
                *sp++ = '_';
          sprintf(text,"%d %d %d %d",dwp.rcNormalPosition.left - pwp.rcNormalPosition.left,dwp.rcNormalPosition.right - pwp.rcNormalPosition.right,dwp.rcNormalPosition.top - pwp.rcNormalPosition.top,dwp.rcNormalPosition.bottom - pwp.rcNormalPosition.bottom);
          WritePrivateProfileString(_RCT("DialogLocation"),title,text,home.getinifilename());
          break;

        case WM_LBUTTONDBLCLK:
          PostMessage(current->gethdlg(),WM_COMMAND,IDOK,0);
          break;
        case WM_KILLFOCUS:
          current->killfocus((HWND)wParam);
          break;
        case WM_SETFOCUS:
          current->setfocus((HWND)wParam);
          break;
        case WM_EXITSIZEMOVE :
          current->exitsizemove();
          break;
        default:
          return FALSE;
    }
  return TRUE;
}

void Dialog::store(void)
{DialogControl *dc;
 GeneralList list;
  list = current->controllist;
  for (list.start() ; (dc = (DialogControl *) list.next()) != NULL ; )
    dc->store(this);
}

void Dialog::load(void)
{DialogControl *dc;
 GeneralList list;
  list = controllist;
  for (list.start() ; (dc = (DialogControl *) list.next()) != NULL ; )
    dc->load(this);
}

#ifdef USING_WIDGETS
DialogControl *Dialog::getdisplaycontrol(int id)
{DialogControl *dc;
 GeneralList list;
  list = displaylist;
  for (list.start() ; (dc = (DialogControl *)list.next()) != NULL ; )
    if (dc->getid() == id) return dc;
  return NULL;
}
#endif
DialogControl *Dialog::getcontrol(int id)
{DialogControl *dc;
 GeneralList list;
  list = controllist;
  for (list.start() ; (dc = (DialogControl *)list.next()) != NULL ; )
    if (dc->getid() == id) return dc;
  list = customlist;
  for (list.start() ; (dc = (DialogControl *)list.next()) != NULL ; )
    if (dc->getid() == id) return dc;
  list = displaylist;
  for (list.start() ; (dc = (DialogControl *)list.next()) != NULL ; )
    if (dc->getid() == id) return dc;
  return NULL;
}

int Dialog::validate(void)
{
 int valid;
 DialogControl *dc;
  valid = 1;
  for (current->controllist.start() ; (dc = (DialogControl *)current->controllist.next()) != NULL ; )
    if (! dc->validate(current))
      {  valid = 0;  dc->selectitem(current);  break;
      }
  return valid;
}

void Dialog::update(int id)
{CustomDialogControl *cdc;
 GeneralList list;
  list = current->customlist;
  for (list.start() ; (cdc = (CustomDialogControl *)list.next()) != NULL ; )
    if (cdc->getid() == id)
      cdc->draw(this,0);
}


void Dialog::updatecustomcontrols(void)
{DisplayDialogControl *ddc;
  for (current->displaylist.start() ; (ddc = (DisplayDialogControl *)current->displaylist.next()) != NULL ; )
    {  ddc->sethwnd(GetDlgItem(current->gethdlg(),ddc->getid()));
       ddc->draw(current,0);
    }
}

int Dialog::currentvalue(int id,short *newvalue)
{DialogControl *dc;
 GeneralList list;
  list = current->controllist;
  for (list.start() ; (dc = (DialogControl *) list.next()) != NULL ; )
    if (dc->getid() == id && dc->type() == IntegerControl)
      return ((IntegerDialogControl *)dc)->currentvalue(this,newvalue);
    else if (dc->getid() == id && dc->type() == ListControl)
      return ((ListDialogControl *)dc)->currentvalue(this,newvalue);
  return 0;
}

int Dialog::currentvalue(int id,int *newvalue)
{DialogControl *dc;
 GeneralList list;
  list = current->controllist;
  for (list.start() ; (dc = (DialogControl *) list.next()) != NULL ; )
    if (dc->getid() == id && dc->type() == IntegerControl)
      return ((IntegerDialogControl *)dc)->currentvalue(this,newvalue);
    else if (dc->getid() == id && dc->type() == CheckBoxControl)
      return ((CheckBoxDialogControl *)dc)->currentvalue(this,newvalue);
    else if (dc->getid() == id && dc->type() == RadioButtonControl)
      return ((RadioButtonDialogControl *)dc)->currentvalue(this,newvalue);
    else if (dc->getid() == id && dc->type() == ListControl)
      return ((ListDialogControl *)dc)->currentvalue(this,newvalue);
#ifdef _USING_QTCHAR
    //else if (dc->getid() == id && dc->type() == RCListControl)
      //return ((RCListDialogControl *)dc)->currentvalue(this,newvalue);
#endif
  return 0;
}

int Dialog::currentvalue(int id,double *newvalue)
{DialogControl *dc;
 GeneralList list;
  list = current->controllist;
  for (list.start() ; (dc = (DialogControl *) list.next()) != NULL ; )
    if (dc->getid() == id && dc->type() == RealControl)
      return ((RealDialogControl *)dc)->currentvalue(this,newvalue);
  return 0;
}

int Dialog::currentvalue(int id,BitMask *newvalue)
{DialogControl *dc;
 GeneralList list;
  list = current->controllist;
  for (list.start() ; (dc = (DialogControl *) list.next()) != NULL ; )
    if (dc->getid() == id && dc->type() == CheckBoxControl)
      return ((CheckBoxDialogControl *)dc)->currentvalue(this,newvalue);
  return 0;
}

int Dialog::currentvalue(int id,RCCHAR *newvalue,int l)
{DialogControl *dc;
 GeneralList list;
  list = current->controllist;
  for (list.start() ; (dc = (DialogControl *) list.next()) != NULL ; )
    if (dc->getid() == id && dc->type() == StringControl)
      return ((StringDialogControl *)dc)->currentvalue(this,newvalue,l);
    else if (dc->getid() == id && dc->type() == ListControl)
      return ((ListDialogControl *)dc)->currentvalue(this,newvalue,l);
  return 0;
}

void Dialog::creatememory(int nitems,int x,int y,int cx,int cy,RCDWORD style,RCDWORD size)
{RCCHAR *s;
  hglobaldialog = GlobalAlloc(GHND,size);
  if (hglobaldialog == 0)
    return;
  globaldialog = (RCCHAR *) GlobalLock(hglobaldialog);
  if (globaldialog == 0)
    {  GlobalFree(hglobaldialog);
       hglobaldialog = 0;
       return;
    }
  globallength = 0;
  *((RCDWORD *)globaldialog) = style | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_SETFONT;
  globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
  *((RCDWORD *)globaldialog) = 0;
  globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
  *((RCCHAR *)globaldialog) = (RCCHAR) nitems;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) x;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) y;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) cx;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) cy;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = 0;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = 0;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  s = dtitle;
  while (*s!=0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) *s++;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  *((RCCHAR *)globaldialog) = (RCCHAR) 0;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) 8;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  s = _RCT("MS Sans Serif");
  while (*s!=0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) *s++;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  *((RCCHAR *)globaldialog) = (RCCHAR) 0;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
}

void Dialog::createcontrol(RCCHAR *c,RCCHAR *t,int id,int x,int y,int cx,int cy,RCDWORD style)
{RCCHAR cl[300],*s;
  if (globaldialog == 0)
    return;
  if (globallength % 4 != 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  strcpy(cl,c);  _strupr(cl);
  if (strcmp(cl,"EDIT") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | ES_LEFT | WS_BORDER | WS_TABSTOP | WS_CHILD | WS_VISIBLE;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else if (strcmp(cl,"STATIC") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | ES_LEFT | WS_CHILD | WS_VISIBLE;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else if (strcmp(cl,"DEFPUSHBUTTON") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | BS_DEFPUSHBUTTON | WS_TABSTOP | WS_CHILD | WS_VISIBLE;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else if (strcmp(cl,"PUSHBUTTON") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | BS_PUSHBUTTON | WS_TABSTOP | WS_CHILD | WS_VISIBLE;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else if (strcmp(cl,"RADIOBUTTON") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | BS_RADIOBUTTON | WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_LEFTTEXT;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else if (strcmp(cl,"CHECKBOX") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | BS_CHECKBOX | WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_LEFTTEXT;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else if (strcmp(cl,"SCROLLBAR") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | SBS_VERT | WS_CHILD | WS_VISIBLE;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else if (strcmp(cl,"GROUPBOX") == 0)
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | BS_GROUPBOX | WS_CHILD | WS_VISIBLE;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  else
    {  *((RCDWORD *)globaldialog) = (RCDWORD) style | WS_CHILD | WS_VISIBLE;
       globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
    }
  *((RCDWORD *)globaldialog) = 0;
  globaldialog += sizeof(RCDWORD);    globallength += sizeof(RCDWORD);
  *((RCCHAR *)globaldialog) = (RCCHAR) x;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) y;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) cx;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) cy;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) id;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  strcpy(cl,c);  _strupr(cl);
  if (strcmp(cl,"EDIT") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x81;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else if (strcmp(cl,"STATIC") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x82;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else if (strcmp(cl,"DEFPUSHBUTTON") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x80;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else if (strcmp(cl,"PUSHBUTTON") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x80;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else if (strcmp(cl,"RADIOBUTTON") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x80;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else if (strcmp(cl,"CHECKBOX") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x80;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else if (strcmp(cl,"SCROLLBAR") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x84;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else if (strcmp(cl,"GROUPBOX") == 0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x80;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  else
    {  *((RCCHAR *)globaldialog) = (RCCHAR) 0xFFFF;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
       *((RCCHAR *)globaldialog) = (RCCHAR) 0x80;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  s = t;
  while (*s!=0)
    {  *((RCCHAR *)globaldialog) = (RCCHAR) *s++;
       globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
    }
  *((RCCHAR *)globaldialog) = (RCCHAR) 0;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
  *((RCCHAR *)globaldialog) = (RCCHAR) 0;
  globaldialog += sizeof(RCCHAR);    globallength += sizeof(RCCHAR);
}

int Dialog::process(RCHWND hwnd,RCHINSTANCE hinstance)
{
#ifdef USING_WIDGETS
    int status=0;

    lcurrent = current;
    lselchange = selchange;
    lhhook = hhook;
    lhookproc = hookproc;

    current = this;
    selchange = 0;

    if (hwnd == 0 && cadwindow != 0)
      hwnd = cadwindow->gethwnd();
    if (hinstance == 0)
      hinstance = program->gethinst();
#ifndef USING_WIDGETS
    modal == 1; // default is modal
    status = DialogBox(this,name,hwnd,addressproc);

    if(modal)
    {
        ((QDialog*)hDlg)->setModal(true);
        current = lcurrent;
        selchange = lselchange;
        hhook = lhhook;
        hookproc = lhookproc;
        /*
        if(v.getinteger("wn::dialogboxautook"))
            status = 0;
        else
            status = 1;
        */
        if (current != 0)
        {
             SetFocus(current->hDlg);
             current->updatecustomcontrols();
        }
    }
    else
    {
        app->mdialogFinishedFlag = RCDIALOG_NOT_FINISHED;
        app->connect((QWidget*)gethdlg(),SIGNAL(finished(int)),app,SLOT(dialogFinished(int)));
        while (app->mdialogFinishedFlag == RCDIALOG_NOT_FINISHED)
        {
            app->processEvents();
        }
        return app->mdialogFinishedFlag;
    }
#else
    if (modal)
    {
         //hDlg = CreateDialog(hinstance,name,hwnd,addressproc);
         hDlg = CreateDialog(this,name,hwnd,addressproc);
         status = hDlg != 0;
    }
    else
    {
         if (hglobaldialog != nullptr)
         {
              globaldialog = (RCCHAR *) GlobalLock(hglobaldialog);
              status = DialogBoxIndirect(program->gethinst(),(DLGTEMPLATE *)globaldialog,hwnd,addressproc);
         }
         else
           status = DialogBox(this,name,hwnd,addressproc);

        current = lcurrent;
        selchange = lselchange;
        hhook = lhhook;
        hookproc = lhookproc;
        if (current != 0)
        {
             SetFocus(current->hDlg);
             current->updatecustomcontrols();
        }
    }
#endif
    return status;
#else // USING_WIDGETS

  int status;

  lcurrent = current;
  lselchange = selchange;
  lhhook = hhook;
#if ! defined(_WIN32_WCE)
  lhookproc = hookproc;
//WCEFIX
#endif

  current = this;
  selchange = 0;

#if ! defined(_WIN32_WCE)
  if (lcurrent == 0)
    {
      //hookproc = (HOOKPROC)MakeProcInstance((FARPROC)customcontrolcb,program->gethinst());
      hookproc = (HOOKPROC)MakeProcInstance(customcontrolcb,program->gethinst());
#if defined(WIN32) || defined(_MAC)
       hhook = SetWindowsHookEx(WH_MSGFILTER,hookproc,program->gethinst(),GetCurrentThreadId());
#else
       hhook = SetWindowsHookEx(WH_MSGFILTER,hookproc,program->gethinst(),GetCurrentTask());
#endif
    }
  //addressproc = (DLGPROC)MakeProcInstance((FARPROC)dialogcb,program->gethinst());
  addressproc = (DLGPROC)MakeProcInstance( dialogcb,program->gethinst());
  if (hwnd == 0 && cadwindow != 0)
    hwnd = cadwindow->gethwnd();
  if (hinstance == 0)
    hinstance = program->gethinst();
  if (modal)
    {  hDlg = CreateDialog(hinstance,name,hwnd,addressproc);
       status = hDlg != 0;
    }
  else
    {
#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD)
       if (hglobaldialog != 0)
         {  GlobalUnlock(hglobaldialog);
            globaldialog = (RCCHAR *) GlobalLock(hglobaldialog);
            status = DialogBoxIndirect(program->gethinst(),(DLGTEMPLATE *)globaldialog,hwnd,addressproc);
         }
       else
#endif
         status = DialogBox(hinstance,name,hwnd,addressproc);

       current = lcurrent;
       selchange = lselchange;
       if (lcurrent == 0)
         {  UnhookWindowsHookEx(hhook);
            //FreeProcInstance((FARPROC)hookproc);
            FreeProcInstance(hookproc);
         }
       hhook = lhhook;
       hookproc = lhookproc;
       if (current != 0)
         {  SetFocus(current->hDlg);
            current->updatecustomcontrols();
         }
       //FreeProcInstance((FARPROC)addressproc);
       FreeProcInstance(addressproc);
    }
//WCEFIX
#endif
  return status;
#endif
}

Dialog::~Dialog()
{
 DialogControl *dc;
 CustomDialogControl *cdc;
 DisplayDialogControl *ddc;
  if (hglobaldialog != 0)
    {  GlobalUnlock(hglobaldialog);
       GlobalFree(hglobaldialog);
    }
  for (controllist.start() ; (dc = (DialogControl * ) controllist.next()) != NULL ; )
    delete dc;
  controllist.destroy();
  for (customlist.start() ; (cdc = (CustomDialogControl * ) customlist.next()) != NULL ; )
    delete cdc;
  customlist.destroy();
  for (displaylist.start() ; (ddc = (DisplayDialogControl * ) displaylist.next()) != NULL ; )
    delete ddc;
  displaylist.destroy();
  disablelist.destroy();
#ifndef USING_WIDGETS
  if(! modal)
  {
      if(hDlg != 0 && !((QDialog*)hDlg)->isModal())
          ((QDialog*)hDlg)->deleteLater();
  }
#else
  if (modal && hDlg != 0)
  {
      DestroyWindow(hDlg);
       if (lcurrent == NULL)
         {  UnhookWindowsHookEx(hhook);
            //FreeProcInstance((FARPROC)hookproc);
            FreeProcInstance(hookproc);
         }
       //FreeProcInstance((FARPROC)addressproc);
       FreeProcInstance(addressproc);
       current = lcurrent;
       selchange = lselchange;
       hhook = lhhook;
       hookproc = lhookproc;
       if (current != 0)
         {  SetFocus(current->hDlg);
            current->updatecustomcontrols();
         }
  }
#endif
}

class CalcButtonDialogControl : public ButtonDialogControl
  {public:
     CalcButtonDialogControl(int i) : ButtonDialogControl(i) {};
     int select(Dialog *);
  };

int CalcButtonDialogControl::select(Dialog *dialog)
{RCCHAR value[256];
  dialog->GetDlgItemText(100,value,256);
  dialog->SetDlgItemText(100,calculator.process(id,value,dialog));
  return 0;
}

class CalcRealDialogControl : public RealDialogControl
  {public:
     CalcRealDialogControl(int i,double *x) : RealDialogControl(i,x) {};
     void load(Dialog *);
  };

void CalcRealDialogControl::load(Dialog *dialog)
{RCCHAR string[300];
  sprintf(string,"%.16lf",*value);
  if (strchr(originaltext,'.') != 0)
    while (strlen(string) > 0 && string[strlen(string)-1] == '0') string[strlen(string)-1] = 0;
  if (strlen(string) > 0 && string[strlen(string)-1] == '.') string[strlen(string)-1] = 0;
  dialog->SetDlgItemText(id,string);
}


int Calculator::process(RCHWND hwnd,RCCHAR *v)
{
 //Dialog dialog(_RCT("Calculator_dialog"));
 Dialog dialog("Calculator_Dialog");
 RCCHAR *endptr;
 int i,status;
 RCCHAR label[10];
  if (active)
    return FALSE;
  active = 1;
  x = strtod(v,&endptr);
  while (*endptr == ' ') endptr++;
  if (*endptr != 0)
    x = 0.0;
  clear = 1;
  lastop = 0;
  strcpy(label,(degrees ? "deg" : "rad"));
  for (i = 101 ; i < 140 ; i++)
    dialog.add(new CalcButtonDialogControl(i));
  dialog.add(new CalcRealDialogControl(100,&x));
  dialog.add(new StringDialogControl(140,label,10));
  status = dialog.process(hwnd);
  active = 0;
  return status;
}

RCCHAR *Calculator::getreal(void)
{
  sprintf(svalue,"%.16g",x);
  return svalue;
}

RCCHAR *Calculator::getinteger(void)
{ if (x > MAXLONG)
    x = MAXLONG;
  else if (x < -MAXLONG)
    x = -MAXLONG;
  sprintf(svalue,"%ld",long(x));
  return svalue;
}

double Calculator::getvalue(RCCHAR *value)
{RCCHAR *endptr;
 double x;
  x = strtod(value,&endptr);
  while (*endptr == ' ') endptr++;
  if (*endptr == 0)
    sprintf(svalue,"%.16g",x);
  else
    {  strcpy(svalue,"Error");  error = 1;
    }
  clear = 1;
  xcleared = 0;
  return x;
}

RCCHAR *Calculator::process(int button,RCCHAR *value,Dialog *dialog)
{RCCHAR newvalue[256],sbutton[2],string[256],*endptr;
 double z;
  if (error && button != 139)
    return svalue;
  if (button >= 101 && button <= 116)
    {  x = getvalue(value);
       if (! error)
         {  switch (button)
              {case 101 : x = sin((degrees ? x * M_PI / 180.0 : x));  break;
               case 102 : if (x < -1.0 || x > 1.0)
                            error = 1;
                          else
                            {  x = asin(x);
                               if (degrees) x *= 180.0 / M_PI;
                               break;
                            }
               case 103 : x = cos((degrees ? x * M_PI / 180.0 : x));  break;
               case 104 : if (x < -1.0 || x > 1.0)
                            error = 1;
                          else
                            {  x = acos(x);
                               if (degrees) x *= 180.0 / M_PI;
                               break;
                            }
               case 105 : if (fabs(fmod(x-M_PI/2.0,M_PI)) < 1.0E-10)
                            error = 1;
                          else
                            {  x = tan((degrees ? x * M_PI / 180.0 : x));
                            }
                          break;
               case 106 : x = atan(x);  break;
               case 107 : lastop = 5;  opused = 0;
                          y = x;
                          break;
               case 108 : lastop = 6;  opused = 0;
                          y = x;
                          break;
               case 109 : if (x > log(MAXDOUBLE))
                            error = 1;
                          else
                            x = exp(x);
                          break;
               case 110 : if (x <= 0.0)
                            error = 1;
                          else
                            x = log(x);
                          break;
               case 111 : if (x > log10(MAXDOUBLE))
                            error = 1;
                          else
                            x = exp(x*log(10.0));
                          break;
               case 112 : if (x <= 0.0)
                            error = 1;
                          else
                            x = log10(x);
                          break;
               case 113 : if (x < 0.0)
                            error = 1;
                          else
                            x = sqrt(x);
                          break;
               case 114 :
#if defined(_MSC_VER)
                          if (fabs(x) < RCDBL_MIN*100.0)
#else
                          if (fabs(x) < MINDOUBLE*100.0)
#endif
                            error = 1;
                          else
                            x = 1.0 / x;
                          break;
               case 115 : x *= x;  break;
               case 116 : x = M_PI;  break;
              }
            if (error)
              strcpy(svalue,"Error");
            else
              sprintf(svalue,"%.16g",x);
         }
    }
  else if (button >= 117 && button <= 129)
    {  if (clear && button != 129)
         strcpy(newvalue,"");
       else
         strcpy(newvalue,value);
       sbutton[1] = 0;
       if (button >= 117 && button <= 126)
         sbutton[0] = RCCHAR('0' + button - 117);
       else if (button == 127)
         sbutton[0] = '.';
       else if (button == 128)
         sbutton[0] = 'E';
       else if (button == 129)
         sbutton[0] = '-';
       if (button <= 128)
         strcat(newvalue,sbutton);
       else
         {  if (strlen(newvalue) > 0)
              {  if (newvalue[strlen(newvalue)-1] == 'E')
                   strcat(newvalue,"-");
                 else if (newvalue[strlen(newvalue)-1] == '-')
                   newvalue[strlen(newvalue)-1] = '+';
                 else if (newvalue[strlen(newvalue)-1] == '+')
                   newvalue[strlen(newvalue)-1] = '-';
                 else if (newvalue[0] == '-')
                   {  strcpy(string,newvalue+1);  strcpy(newvalue,string);
                   }
                 else
                   {  strcpy(string,"-");  strcat(string,newvalue);
                      strcpy(newvalue,string);
                   }
              }
            else
              strcpy(newvalue,"-");
         }
       strcpy(svalue,newvalue);
       strtod(newvalue,&endptr);
       while (*endptr == ' ') endptr++;
       clear = 0;
    }
  else if (button == 130)
    {  degrees = ! degrees;
       ((StringDialogControl *)dialog->getcontrol(140))->change(dialog,(degrees ? _RCT("deg") : _RCT("rad")));
    }
  else if (button >= 131 && button <= 135)
    {  if (lastop == 0 || ! opused)
         {  x = xop = getvalue(value);
         }
       else
         {  x = xop;
         }
       if (! error)
         {  if (lastop == 1)
              {  if (fabs(x) < 1.0E-10) error = 1;
                 else
                   z = y / x;
              }
            else if (lastop == 2)
              z = x * y;
            else if (lastop == 3)
              z = y - x;
            else if (lastop == 4)
              z = x + y;
            else if (lastop == 5)
#if defined(_MSC_VER)
              {  if (y < RCDBL_MIN*100.0  || x * log(y) > log(RCDBL_MAX))
#else
              {  if (y < MINDOUBLE*100.0  || x * log(y) > log(MINDOUBLE))
#endif
                   error = 1;
                 else
                   z = pow(y,x);
              }
            else if (lastop == 6)
#if defined(_MSC_VER)
              {  if (y < RCDBL_MIN*100.0 || fabs(x) < RCDBL_MIN / 100.0)
#else
              {  if (y < MINDOUBLE*100.0 || fabs(x) < MINDOUBLE / 100.0)
#endif
                   error = 1;
                 else
                   z = pow(y,1.0/x);
              }
            else
              z = x;
            if (error)
              strcpy(svalue,"Error");
            else
              sprintf(svalue,"%.16g",z);
            y = z;
            opused = 1;
            if (button >= 132 && button <= 135)
              {  lastop = button - 131;
                 opused = 0;
              }
         }
    }
  else if (button >= 132 && button <= 135)
    {  y = getvalue(value);
       if (! error)
         lastop = button - 131;
       opused = 0;
    }
  else if (button == 136)
    {  x = getvalue(value);
       if (! error)
         m = x;
    }
  else if (button == 137)
    {  x = m;
       sprintf(svalue,"%.16g",x);
       clear = 1;
       xcleared = 0;
    }
  else if (button == 138)
    {  x = getvalue(value);
       if (! error)
         m += x;
    }
  else if (button == 139)
    {  if (error || xcleared)
         lastop = 0;
       x = 0.0;
       strcpy(svalue,"0");
       clear = 1;
       xcleared = 1;
       error = 0;
    }
  return svalue;
}

