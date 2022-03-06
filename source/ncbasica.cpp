
#include "ncwin.h"

static RCCHAR *NEAR SymbolName[] =
  {  _RCT("dim"),_RCT("as"),_RCT("if"),_RCT("then"),_RCT("else"),_RCT("endif"),_RCT("do"),_RCT("while"),_RCT("loop"),_RCT("for"),
     _RCT("to"),_RCT("step"),_RCT("next"),_RCT("select"),_RCT("case"),_RCT("end"),_RCT("print"),_RCT("and"),_RCT("or"),_RCT("not"),
     _RCT("open"),_RCT("close"),_RCT("input"),_RCT("output"),_RCT("rem"),_RCT("function"),_RCT("sub"),0
  };

static RCCHAR *NEAR FunctionName[] =
  {  _RCT("abs"),_RCT("acos"),_RCT("asin"),_RCT("atan"),_RCT("atan2"),_RCT("ceil"),_RCT("int"),
     _RCT("cos"),_RCT("exp"),_RCT("log"),_RCT("log10"),_RCT("fmod"),_RCT("pow"),_RCT("pow10"),_RCT("random"),
     _RCT("randomize"),_RCT("tan"),_RCT("sqrt"),_RCT("sin"),_RCT("pi"),_RCT("msgbox"),
     _RCT("mb_ok"),_RCT("mb_okcancel"),_RCT("mb_abortretryignore"),_RCT("mb_yesnocancel"),
     _RCT("mb_yesno"),_RCT("mb_retrycancel"),_RCT("idok"),_RCT("idcancel"),_RCT("idabort"),_RCT("idretry"),
     _RCT("idignore"),_RCT("idyes"),_RCT("idno"),_RCT("mb_iconstop"),_RCT("mb_iconquestion"),
     _RCT("mb_iconexclamation"),_RCT("mb_iconexclamation"), _RCT("str$"),_RCT("mid$"),_RCT("len"),_RCT("val"),
     _RCT("asc"),_RCT("chr$"),
     0
  };

#define MAX_BASICFILES 32

class BasicOpenFiles
  {private:
     FILE *file[MAX_BASICFILES];
   public:
     BasicOpenFiles();
     int open(RCCHAR *filename,RCCHAR *mode,int index);
     int close(int index);
     void closeall(void);
     int readline(int index,RCCHAR *line,int len);
     int writeline(int index,RCCHAR *line);
  };

BasicOpenFiles::BasicOpenFiles()
{int i;
  for (i = 0 ; i < MAX_BASICFILES ; i++)
    file[i] = 0;
}

void BasicOpenFiles::closeall(void)
{int i;
  for (i = 0 ; i < MAX_BASICFILES ; i++)
    if (file[i] != 0)
      {  fclose(file[i]);
         file[i] = 0;
      }
}

int BasicOpenFiles::open(RCCHAR *filename,RCCHAR *mode,int index)
{ if (file[index] != 0)
    return 0;
  else
    {  file[index] = fopen(filename,mode);
       return file[index] != 0;
    }
}

int BasicOpenFiles::close(int index)
{ if (file[index] != 0)
    {  fclose(file[index]);
       file[index] = 0;
       return 1;
    }
  else
    return 0;
}

int BasicOpenFiles::readline(int index,RCCHAR *line,int len)
{RCCHAR *s;
  if (file[index] != 0)
    {  if ((s = fgets(line,len,file[index])) != 0)
         {  while (*s!=0)
              {  if (*s == '\n') *s = 0;
                 if (*s == '\r') *s = 0;
                 s++;
              }
            return 1;
         }
       else
         return 0;
    }
  else
    return 0;
}

int BasicOpenFiles::writeline(int index,RCCHAR *line)
{ if (file[index] != 0)
    return fprintf(file[index],"%s\n",line) != EOF;
  else
    return 0;
}

BasicOpenFiles basicopenfiles;

void CadBasicProgram::nextsymbol(void)
{RCCHAR *eptr1,*eptr2;
 long i;
 double x;
  if (currentlineno >= nlines)
    {  stype = EolSym;
       return;
    }
  stype = BadSym;
  while (line[currentlineno][pos] == ' ') pos++;
  if (line[currentlineno][pos] == 0)
    stype = EolSym;
  else if (line[currentlineno][pos] == '+')
    {  stype = PlusSym;  pos++;
    }
  else if (line[currentlineno][pos] == '-')
    {  stype = MinusSym;  pos++;
    }
  else if (line[currentlineno][pos] == '=')
    {  stype = EqualsSym;  pos++;
    }
  else if (line[currentlineno][pos] == '<')
    {  pos++;
       while (line[currentlineno][pos] == ' ') pos++;
       if (line[currentlineno][pos] == '>')
         {  stype = NotEqualSym;
            pos++;
         }
       else if (line[currentlineno][pos] == '=')
         {  stype = LessEqualSym;
            pos++;
         }
       else
         stype = LessSym;
    }
  else if (line[currentlineno][pos] == '>')
    {  pos++;
       while (line[currentlineno][pos] == ' ') pos++;
       if (line[currentlineno][pos] == '=')
         {  stype = GreaterEqualSym;
            pos++;
         }
       else
         stype = GreaterSym;
    }
  else if (line[currentlineno][pos] == ':')
    {  stype = ColonSym;  pos++;
    }
  else if (line[currentlineno][pos] == '/')
    {  stype = DivideSym;  pos++;
    }
  else if (line[currentlineno][pos] == '*')
    {  stype = MultiplySym;  pos++;
    }
  else if (line[currentlineno][pos] == '(')
    {  stype = LBracketSym;  pos++;
    }
  else if (line[currentlineno][pos] == ')')
    {  stype = RBracketSym;   pos++;
    }
  else if (line[currentlineno][pos] == ',')
    {  stype = CommaSym;   pos++;
    }
  else if (line[currentlineno][pos] == ';')
    {  stype = SemiColonSym;   pos++;
    }
  else if (line[currentlineno][pos] == '.')
    {  stype = DotSym;   pos++;
    }
  else if (line[currentlineno][pos] == '#')
    {  stype = HashSym;   pos++;
    }
  else if (line[currentlineno][pos] == '"')
    {  pos++;
       i = 0;
       while (line[currentlineno][pos] != 0 && line[currentlineno][pos] != '"')
         svalue[i++] = line[currentlineno][pos++];
       svalue[i] = 0;
       if (line[currentlineno][pos] == 0)
         stype = BadSym;
       else
         {  pos++;
            stype = StringConstantSym;
         }
    }
  else if (tolower(line[currentlineno][pos]) >= 'a' && tolower(line[currentlineno][pos]) <= 'z')
    {  i = 0;
       sname[i++] = (RCCHAR) tolower(line[currentlineno][pos++]);
       while (tolower(line[currentlineno][pos]) >= 'a' && tolower(line[currentlineno][pos]) <= 'z' ||
              line[currentlineno][pos] >= '0' && line[currentlineno][pos] <= '9' ||
              line[currentlineno][pos] == '_' || line[currentlineno][pos] == '$')
         {  if (i < 255)
              sname[i++] = (RCCHAR) tolower(line[currentlineno][pos++]);
            else
              pos++;
         }
       sname[i] = 0;
       for (i = 0 ; FunctionName[i] != 0 ; i++)
         if (strcmp(sname,FunctionName[i]) == 0)
           break;
       if (FunctionName[i] == 0)
         {  for (i = 0 ; SymbolName[i] != 0 ; i++)
              if (strcmp(sname,SymbolName[i]) == 0)
                break;
            if (SymbolName[i] == 0)
              stype = VariableNameSym;
            else
              stype = (SymbolType) i;
         }
       else
         stype = FunctionNameSym;
    }
  else
    {  x = strtod(line[currentlineno]+pos,&eptr1);
       i = strtol(line[currentlineno]+pos,&eptr2,10);
       if (eptr1 > eptr2)
         {  stype = RealConstantSym;
            xvalue = x;
            pos = int(eptr1 - line[currentlineno]);
         }
       else
         {  stype = IntegerConstantSym;
            ivalue = i;
            pos = int(eptr2 - line[currentlineno]);
         }
    }
}

int CadBasicProgram::lexpression(Variant *v)
{Variant t1,t2;
 double x1,x2;
 long i1,i2;
 int type;
 RCCHAR *s;
  if (! lterm(&t1))
    return 0;
  if (t1.gettype() != IntegerValue && t1.gettype() != RealValue && t1.gettype() != StringValue)
    return 0;
  switch (type = t1.gettype())
    {case IntegerValue:  t1.getvalue(&i1);  break;
     case RealValue:  t1.getvalue(&x1);  break;
     case StringValue:  t1.getvalue(&s);  break;
    }
  while (stype == OrSym)
    {  nextsymbol();
       if (! lterm(&t2))
         return 0;
       if (t2.gettype() != IntegerValue && t2.gettype() != RealValue)
         return 0;
       if (type == IntegerValue && t2.gettype() == IntegerValue)
         {  t2.getvalue(&i2);
            i1 = i1 || i2;
         }
       else
         {  if (type == IntegerValue)
              type = RealValue;
            t2.getvalue(&x2);
            x1 = x1 || x2;
         }
    }
  switch (type)
    {case IntegerValue:  v->setvalue(i1);  break;
     case RealValue:  v->setvalue(x1);  break;
     case StringValue:  v->setvalue(s);  break;
    }
  return 1;
}

int CadBasicProgram::lterm(Variant *v)
{Variant f1,f2;
 double x1,x2;
 long i1,i2;
 int type;
 RCCHAR *s;
  if (! lfactor(&f1))
    return 0;
  if (f1.gettype() != IntegerValue && f1.gettype() != RealValue && f1.gettype() != StringValue)
    return 0;
  switch (type = f1.gettype())
    {case IntegerValue:  f1.getvalue(&i1);  break;
     case RealValue:  f1.getvalue(&x1);  break;
     case StringValue:  f1.getvalue(&s);  break;
    }
  while (stype == AndSym)
    {  nextsymbol();
       if (! lfactor(&f2))
         return 0;
       if (f2.gettype() != IntegerValue && f2.gettype() != RealValue)
         return 0;
       if (type == IntegerValue && f2.gettype() == IntegerValue)
         {  f2.getvalue(&i2);
            i1 = i1 && i2;
         }
       else
         {  if (type == IntegerValue)
              type = RealValue;
            f2.getvalue(&x2);
            x1 = x1 && x2;
         }
    }
  switch (type)
    {case IntegerValue:  v->setvalue(i1);  break;
     case RealValue:  v->setvalue(x1);  break;
     case StringValue:  v->setvalue(s);  break;
    }
  return 1;
}

int CadBasicProgram::lfactor(Variant *v)
{Variant e1,e2;
 double x1,x2;
 long i1,i2;
 SymbolType op;
 int type;
 RCCHAR *s1,*s2;
  if (! expression(&e1))
    return 0;
  if (e1.gettype() != IntegerValue && e1.gettype() != RealValue && e1.gettype() != StringValue)
    return 0;
  switch (type = e1.gettype())
    {case IntegerValue:  e1.getvalue(&i1);  break;
     case RealValue:  e1.getvalue(&x1);  break;
     case StringValue:  e1.getvalue(&s1);  break;
    }
  if (stype == LessSym || stype == LessEqualSym ||
      stype == GreaterSym || stype == GreaterEqualSym ||
      stype == EqualsSym || stype == NotEqualSym)
    {  op = stype;
       nextsymbol();
       if (! expression(&e2))
         return 0;
       if (type == StringValue || e2.gettype() == StringValue)
         {  if (type != StringValue || e2.gettype() != StringValue)
              return 0;
            e2.getvalue(&s2);
            switch (op)
              {case LessSym:  i1 = strcmp(s1,s2) < 0;  break;
               case LessEqualSym:  i1 = strcmp(s1,s2) <= 0;  break;
               case GreaterSym:  i1 = strcmp(s1,s2) > 0;  break;
               case GreaterEqualSym:  i1 = strcmp(s1,s2) >= 0;  break;
               case EqualsSym:  i1 = strcmp(s1,s2) == 0;  break;
               case NotEqualSym:  i1 = strcmp(s1,s2) != 0;  break;
              }
            type = IntegerValue;
         }
       else if (type == IntegerValue && e2.gettype() == IntegerValue)
         {  e2.getvalue(&i2);
            switch (op)
              {case LessSym:  i1 = i1 < i2;  break;
               case LessEqualSym:  i1 = i1 <= i2;  break;
               case GreaterSym:  i1 = i1 > i2;  break;
               case GreaterEqualSym:  i1 = i1 >= i2;  break;
               case EqualsSym:  i1 = i1 == i2;  break;
               case NotEqualSym:  i1 = i1 != i2;  break;
              }
         }
       else
         {  if (type == IntegerValue)
              x1 = i1;
            e2.getvalue(&x2);
            switch (op)
              {case LessSym:  x1 = x1 < x2;  break;
               case LessEqualSym:  x1 = x1 <= x2;  break;
               case GreaterSym:  x1 = x1 > x2;  break;
               case GreaterEqualSym:  x1 = x1 >= x2;  break;
               case EqualsSym:  x1 = x1 == x2;  break;
               case NotEqualSym:  x1 = x1 != x2;  break;
              }
         }
    }
  switch (type)
    {case IntegerValue:  v->setvalue(i1);  break;
     case RealValue:  v->setvalue(x1);  break;
     case StringValue:  v->setvalue(s1);  break;
    }
  return 1;
}

int CadBasicProgram::expression(Variant *v)
{Variant t1,t2;
 double sign,x1,x2;
 long i1,i2;
 SymbolType op;
 int type;
 RCCHAR *s1,*s2;
  sign = 1.0;
  if (stype == PlusSym)
    {  sign = 1.0;  nextsymbol();
    }
  else if (stype == MinusSym)
    {  sign = -1.0;  nextsymbol();
    }
  if (! term(&t1))
    return 0;
  if (t1.gettype() != IntegerValue && t1.gettype() != RealValue && t1.gettype() != StringValue)
    return 0;
  switch (type = t1.gettype())
    {case IntegerValue:  t1.getvalue(&i1);  if (sign < 0.0) i1 = -i1;  break;
     case RealValue:  t1.getvalue(&x1);  if (sign < 0.0) x1 = -x1;  break;
     case StringValue:  t1.getvalue(&s1);  break;
    }
  while (stype == PlusSym || stype == MinusSym)
    {  op = stype;
       nextsymbol();
       if (! term(&t2))
         return 0;
       if (t2.gettype() != IntegerValue && t2.gettype() != RealValue && t2.gettype() != StringValue)
         return 0;
       if (type == StringValue || t2.gettype() == StringValue)
         {  if (type != StringValue || t2.gettype() != StringValue)
              return 0;
            t2.getvalue(&s2);
            if (op == PlusSym)
              {RCCHAR *stemp = new RCCHAR[strlen(s1) + strlen(s2) + 1];
                 strcpy(stemp,s1);  strcat(stemp,s2);
                 t1.setvalue(stemp);
                 delete [] stemp;
                 t1.getvalue(&s1);
              }
            else
              return 0;
         }
       else if (type == IntegerValue && t2.gettype() == IntegerValue)
         {  t2.getvalue(&i2);
            if (op == PlusSym)
              i1 += i2;
            else
              i1 -= i2;
         }
       else
         {  if (type == IntegerValue)
              type = RealValue;
            t2.getvalue(&x2);
            if (op == PlusSym)
              x1 += x2;
            else
              x1 -= x2;
         }
    }
  switch (type)
    {case IntegerValue:  v->setvalue(i1);  break;
     case RealValue:  v->setvalue(x1);  break;
     case StringValue:  v->setvalue(s1);  break;
    }
  return 1;
}

int CadBasicProgram::term(Variant *v)
{Variant f1,f2;
 double x1,x2;
 long i1,i2;
 SymbolType op;
 int type;
 RCCHAR *s;
  if (! factor(&f1))
    return 0;
  if (f1.gettype() != IntegerValue && f1.gettype() != RealValue && f1.gettype() != StringValue)
    return 0;
  switch (type = f1.gettype())
    {case IntegerValue:  f1.getvalue(&i1);  break;
     case RealValue:  f1.getvalue(&x1);  break;
     case StringValue:  f1.getvalue(&s);  break;
    }
  while (stype == DivideSym || stype == MultiplySym)
    {  op = stype;
       nextsymbol();
       if (! factor(&f2))
         return 0;
       if (f2.gettype() != IntegerValue && f2.gettype() != RealValue)
         return 0;
       if (op == DivideSym)
         {  if (type == IntegerValue && f2.gettype() == IntegerValue)
              {  f2.getvalue(&i2);
                 if (i2 == 0)
                   return 0;
                 i1 /= i2;
              }
            else
              {  if (type == IntegerValue)
                   {  x1 = i1;
                      type = RealValue;
                   }
                 f2.getvalue(&x2);
                 if (fabs(x2) < 1.0E-10)
                   return 0;
                 x1 /= x2;
              }
         }
       else
         {  if (type == IntegerValue && f2.gettype() == IntegerValue)
              {  f2.getvalue(&i2);
                 i1 *= i2;
              }
            else
              {  if (type == IntegerValue)
                   {  x1 = i1;
                      type = RealValue;
                   }
                 f2.getvalue(&x2);
                 x1 *= x2;
              }
         }
    }
  switch (type)
    {case IntegerValue:  v->setvalue(i1);  break;
     case RealValue:  v->setvalue(x1);  break;
     case StringValue:  v->setvalue(s);  break;
    }
  return 1;
}

int CadBasicProgram::factor(Variant *v)
{Variant v1,v2,v3,*v4;
 double x1,p1,p2,p3;
 long i1;
 int _not;
 RCCHAR fname[256],*s1,*s2;
 Point wp1,wp2,wpo,wpxa,wpya;

  if (stype == NotSym)
    {  nextsymbol();
       _not = 1;
    }
  else
    _not = 0;
  if (stype == RealConstantSym)
    {  v->setvalue(xvalue);  nextsymbol();
    }
  else if (stype == IntegerConstantSym)
    {  v->setvalue(ivalue);  nextsymbol();
    }
  else if (stype == StringConstantSym)
    {  v->setvalue(svalue);  nextsymbol();
    }
  else if (stype == VariableNameSym)
    {  if ((v4 = rtsymbols.match(sname)) != 0)
         {  nextsymbol();
            if (stype == DotSym)
              {  if (v4->gettype() == LineValue)
                   {  nextsymbol();
                      if (stype == VariableNameSym || stype == SelectSym)
                        {Line *line;
                           v4->getvalue(LineValue,(void **)&line);

                           wp1 = line->p1;
                           wp2 = line->p2;
                           if (rtsymbols.getworkplanecoordinates())
                             {  wp1 = db.workplanes.getcurrent()->modeltoworkplane(wp1);
                                wp2 = db.workplanes.getcurrent()->modeltoworkplane(wp2);
                             } 

 
#if ! defined(_WIN32_WCE)
                           strcpy(fname,sname);  _strlwr(fname);
//WCEFIX
#endif
                           nextsymbol();
                           if (strcmp(fname,"x1") == 0)
                             v->setvalue(wp1.x);
                           else if (strcmp(fname,"y1") == 0)
                             v->setvalue(wp1.y);
                           else if (strcmp(fname,"z1") == 0)
                             v->setvalue(wp1.z);
                           else if (strcmp(fname,"x2") == 0)
                             v->setvalue(wp2.x);
                           else if (strcmp(fname,"y2") == 0)
                             v->setvalue(wp2.y);
                           else if (strcmp(fname,"z2") == 0)
                             v->setvalue(wp2.z);
                           else if (strcmp(fname,"colour") == 0)
                             v->setvalue((long)line->colour);
                           else if (strcmp(fname,"layer") == 0)
                             v->setvalue((long)line->layer);
                           else if (strcmp(fname,"style") == 0)
                             v->setvalue((long)line->style);
                           else if (strcmp(fname,"weight") == 0)
                             v->setvalue((long)line->weight);
                           else if (strcmp(fname,"select") == 0)
                             {  if (stype == LBracketSym)
                                  {  nextsymbol();
                                     lexpression(&v1);
                                     if (v1.gettype() == StringValue)
                                       {RCCHAR *s;
                                          v1.getvalue(&s);
                                          if (stype == RBracketSym)
                                            {  nextsymbol();
                                               cadwindow->prompt->normalprompt(s);
                                               for (;;)
                                                 {  program->processmessages(3);
                                                    if (state.getevent() != 0 && state.getevent()->isa(Abort))
                                                      {  v->setvalue(0L);
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                    else if (state.getevent() && state.getevent()->isa(EntitySelected) && ((EntitySelectedEvent *)state.getevent())->getentity()->isa(line_entity))
                                                      {Transform ident;
                                                       Line *line;
                                                         line = (Line *) ((EntitySelectedEvent *)state.getevent())->getentity();
                                                         line->setcopye(0L);
                                                         v->setvalue(1L);
                                                         v4->setvalue(LineValue,line->clone(ident));
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                 }
                                               cadwindow->prompt->normalprompt(NCWIN+1);
                                            }
                                          else
                                            {  stype = BadSym;
                                               cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                               return 0;
                                            }
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("String value expected.","Run CAD Basic",MB_ICONSTOP);
                                          return 0;
                                       }
                                  }
                                else
                                  {  stype = BadSym;
                                     cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                     return 0;
                                  }
                             }
                           else
                             {  stype = BadSym;
                                cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                                return 0;
                             }
                        }
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                           return 0;
                        }
                   }
                 else if (v4->gettype() == PointEValue)
                   {  nextsymbol();
                      if (stype == VariableNameSym || stype == SelectSym)
                        {PointE *point;
                           v4->getvalue(PointEValue,(void **)&point);
                           wp1 = point->p;
                           if (rtsymbols.getworkplanecoordinates())
                             wp1 = db.workplanes.getcurrent()->modeltoworkplane(wp1);
                            
#if ! defined(_WIN32_WCE)
                           strcpy(fname,sname);  _strlwr(fname);
//WCEFIX
#endif
                           nextsymbol();
                           if (strcmp(fname,"x") == 0)
                             v->setvalue(wp1.x);
                           else if (strcmp(fname,"y") == 0)
                             v->setvalue(wp1.y);
                           else if (strcmp(fname,"z") == 0)
                             v->setvalue(wp1.z);
                           else if (strcmp(fname,"colour") == 0)
                             v->setvalue((long)point->colour);
                           else if (strcmp(fname,"layer") == 0)
                             v->setvalue((long)point->layer);
                           else if (strcmp(fname,"style") == 0)
                             v->setvalue((long)point->style);
                           else if (strcmp(fname,"weight") == 0)
                             v->setvalue((long)point->weight);
                           else if (strcmp(fname,"select") == 0)
                             {  if (stype == LBracketSym)
                                  {  nextsymbol();
                                     lexpression(&v1);
                                     if (v1.gettype() == StringValue)
                                       {RCCHAR *s;
                                          v1.getvalue(&s);
                                          if (stype == RBracketSym)
                                            {  nextsymbol();
                                               cadwindow->prompt->normalprompt(s);
                                               for (;;)
                                                 {  program->processmessages(3);
                                                    if (state.getevent() != 0 && state.getevent()->isa(Abort))
                                                      {  v->setvalue(0L);
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                    else if (state.getevent() && state.getevent()->isa(EntitySelected) && ((EntitySelectedEvent *)state.getevent())->getentity()->isa(point_entity))
                                                      {Transform ident;
                                                       PointE *point;
                                                         point = (PointE *) ((EntitySelectedEvent *)state.getevent())->getentity();
                                                         point->setcopye(0L);
                                                         v->setvalue(1L);
                                                         v4->setvalue(PointEValue,point->clone(ident));
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                 }
                                               cadwindow->prompt->normalprompt(NCWIN+1);
                                            }
                                          else
                                            {  stype = BadSym;
                                               cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                               return 0;
                                            }
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("String value expected.","Run CAD Basic",MB_ICONSTOP);
                                          return 0;
                                       }
                                  }
                                else
                                  {  stype = BadSym;
                                     cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                     return 0;
                                  }
                             }
                           else
                             {  stype = BadSym;
                                cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                                return 0;
                             }
                        }
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                           return 0;
                        }
                   }
                 else if (v4->gettype() == CircleValue)
                   {  nextsymbol();
                      if (stype == VariableNameSym || stype == SelectSym)
                        {Circle *circle;
                           v4->getvalue(CircleValue,(void **)&circle);

                           wpo = circle->origin;
                           wpxa = circle->xaxis;
                           wpya = circle->yaxis;

                           if (rtsymbols.getworkplanecoordinates())
                             {  wpo = db.workplanes.getcurrent()->modeltoworkplane(wpo);
                                wpxa = db.workplanes.getcurrent()->modeltoworkplane(circle->origin + wpxa) - wpo;
                                wpya = db.workplanes.getcurrent()->modeltoworkplane(circle->origin + wpya) - wpo;
                             }

                           strcpy(fname,sname);  _strlwr(fname);
                           nextsymbol();
                           if (strcmp(fname,"xorigin") == 0)
                             v->setvalue(wpo.x);
                           else if (strcmp(fname,"yorigin") == 0)
                             v->setvalue(wpo.y);
                           else if (strcmp(fname,"zorigin") == 0)
                             v->setvalue(wpo.z);
                           else if (strcmp(fname,"xxaxis") == 0)
                             v->setvalue(wpxa.x);
                           else if (strcmp(fname,"yxaxis") == 0)
                             v->setvalue(wpxa.y);
                           else if (strcmp(fname,"zxaxis") == 0)
                             v->setvalue(wpxa.z);
                           else if (strcmp(fname,"xyaxis") == 0)
                             v->setvalue(wpya.x);
                           else if (strcmp(fname,"yyaxis") == 0)
                             v->setvalue(wpya.y);
                           else if (strcmp(fname,"zyaxis") == 0)
                             v->setvalue(wpya.z);
                           else if (strcmp(fname,"radius") == 0)
                             v->setvalue(circle->radius);
                           else if (strcmp(fname,"sweepa") == 0)
                             v->setvalue(circle->sweepa * 180.0 / M_PI);
                           else if (strcmp(fname,"starta") == 0)
                             v->setvalue(circle->starta * 180.0 / M_PI);
                           else if (strcmp(fname,"colour") == 0)
                             v->setvalue((long)circle->colour);
                           else if (strcmp(fname,"layer") == 0)
                             v->setvalue((long)circle->layer);
                           else if (strcmp(fname,"style") == 0)
                             v->setvalue((long)circle->style);
                           else if (strcmp(fname,"weight") == 0)
                             v->setvalue((long)circle->weight);
                           else if (strcmp(fname,"select") == 0)
                             {  if (stype == LBracketSym)
                                  {  nextsymbol();
                                     lexpression(&v1);
                                     if (v1.gettype() == StringValue)
                                       {RCCHAR *s;
                                          v1.getvalue(&s);
                                          if (stype == RBracketSym)
                                            {  nextsymbol();
                                               cadwindow->prompt->normalprompt(s);
                                               for (;;)
                                                 {  program->processmessages(3);
                                                    if (state.getevent() != 0 && state.getevent()->isa(Abort))
                                                      {  v->setvalue(0L);
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                    else if (state.getevent() && state.getevent()->isa(EntitySelected) && ((EntitySelectedEvent *)state.getevent())->getentity()->isa(circle_entity))
                                                      {Transform ident;
                                                       Circle *circle;
                                                         circle = (Circle *) ((EntitySelectedEvent *)state.getevent())->getentity();
                                                         circle->setcopye(0L);
                                                         v->setvalue(1L);
                                                         v4->setvalue(CircleValue,circle->clone(ident));
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                 }
                                               cadwindow->prompt->normalprompt(NCWIN+1);
                                            }
                                          else
                                            {  stype = BadSym;
                                               cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                               return 0;
                                            }
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("String value expected.","Run CAD Basic",MB_ICONSTOP);
                                          return 0;
                                       }
                                  }
                                else
                                  {  stype = BadSym;
                                     cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                     return 0;
                                  }
                             }
                           else
                             {  stype = BadSym;
                                cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                                return 0;
                             }
                        }
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                           return 0;
                        }
                   }
                 else if (v4->gettype() == PointValue)
                   {  nextsymbol();
                      if (stype == VariableNameSym || stype == SelectSym)
                        {Point *p;
                           v4->getvalue(PointValue,(void **)&p);
                           strcpy(fname,sname);  _strlwr(fname);
                           nextsymbol();
                           if (strcmp(fname,"x") == 0)
                             v->setvalue(p->x);
                           else if (strcmp(fname,"y") == 0)
                             v->setvalue(p->y);
                           else if (strcmp(fname,"z") == 0)
                             v->setvalue(p->z);
                           else if (strcmp(fname,"select") == 0)
                             {  if (stype == LBracketSym)
                                  {  nextsymbol();
                                     lexpression(&v1);
                                     if (v1.gettype() == StringValue)
                                       {RCCHAR *s;
                                          v1.getvalue(&s);
                                          if (stype == RBracketSym)
                                            {  nextsymbol();
                                               cadwindow->prompt->normalprompt(s);
                                               state.selmask.entity.set(xyz_mask);
                                               state.selmask.entity.clear(many_mask);
                                               state.tempentitymask.set(xyz_mask);
                                               for (;;)
                                                 {  program->processmessages(3);
                                                    if (state.getevent() != 0 && state.getevent()->isa(Abort))
                                                      {  v->setvalue(0L);
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                    else if (state.getevent() && state.getevent()->isa(Coordinate3d))
                                                      {Point *p1;
                                                         p1 = new Point(((Coordinate3dEvent *)state.getevent())->getp());
                                                         if (rtsymbols.getworkplanecoordinates())
                                                           *p1 = db.workplanes.getcurrent()->modeltoworkplane(*p1);
                                                         v->setvalue(1L);
                                                         v4->setvalue(PointValue,p1);
                                                         state.sendevent(0);
                                                         break;
                                                      }
                                                 }
                                               state.selmask.entity.clear(xyz_mask);
                                               state.selmask.entity.set(many_mask);
                                               state.tempentitymask.clear(xyz_mask);
                                               cadwindow->prompt->normalprompt(NCWIN+1);
                                            }
                                          else
                                            {  stype = BadSym;
                                               cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                               return 0;
                                            }
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("String value expected.","Run CAD Basic",MB_ICONSTOP);
                                          return 0;
                                       }
                                  }
                                else
                                  {  stype = BadSym;
                                     cadwindow->MessageBox("( expected.","Run CAD Basic",MB_ICONSTOP);
                                     return 0;
                                  }
                             }
                           else
                             {  stype = BadSym;
                                cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                                return 0;
                             }
                        }
                   }
                 else if (v4->gettype() == DialogValue)
                   {  nextsymbol();
                      if (stype == VariableNameSym)
                        {Dialog *d;
                           v4->getvalue(DialogValue,(void **)&d);
                           strcpy(fname,sname);  _strlwr(fname);
                           nextsymbol();
                           if (strcmp(fname,"process") == 0)
                             v->setvalue((long)d->process());
                           else
                             {  stype = BadSym;
                                cadwindow->MessageBox("Function name expected.","Run CAD Basic",MB_ICONSTOP);
                                return 0;
                             }
                        }
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Structed variable field name required.","Run CAD Basic",MB_ICONSTOP);
                           return 0;
                        }
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Structured variable required.","Run CAD Basic",MB_ICONSTOP);
                      return 0;
                   }
              }
            else if (v4->gettype() == IntegerValue)
              {  v4->getvalue(&i1);
                 v->setvalue(i1);
              }
            else if (v4->gettype() == RealValue)
              {  v4->getvalue(&x1);
                 v->setvalue(x1);
              }
            else if (v4->gettype() == StringValue)
              {RCCHAR *s;
                 v4->getvalue(&s);
                 v->setvalue(s);
              }
            else
              return 0;
         }
       else
         {  nextsymbol();
            return 0;
         }
    }
  else if (stype == FunctionNameSym)
    {  s1 = s2 = _RCT("");
       p1 = p2 = p3 = 0.0;
       strcpy(fname,sname);
       nextsymbol();
       if (strcmp(fname,"pi") != 0 && strcmp(fname,"random") != 0 &&
           strcmp(fname,"mb_ok") != 0 &&
           strcmp(fname,"mb_okcancel") != 0 &&
           strcmp(fname,"mb_abortretrycancel") != 0 &&
           strcmp(fname,"mb_yesnocancel") != 0 &&
           strcmp(fname,"mb_yesno") != 0 &&
           strcmp(fname,"mb_retrycancel") != 0 &&
           strcmp(fname,"mb_iconstop") != 0 &&
           strcmp(fname,"mb_iconquestion") != 0 &&
           strcmp(fname,"mb_iconexclamation") != 0 &&
           strcmp(fname,"mb_iconinformation") != 0 &&
           strcmp(fname,"idok") != 0 &&
           strcmp(fname,"idcancel") != 0 &&
           strcmp(fname,"idabort") != 0 &&
           strcmp(fname,"idretry") != 0 &&
           strcmp(fname,"idignore") != 0 &&
           strcmp(fname,"idyes") != 0 &&
           strcmp(fname,"idno") != 0)
         {  if (stype != LBracketSym)
              return 0;
            nextsymbol();
            if (! expression(&v1))
              return 0;
            if (strcmp(fname,"fmod") == 0 || strcmp(fname,"pow") == 0 || strcmp(fname,"atan2") == 0 || strcmp(fname,"msgbox") == 0 || strcmp(fname,"mid$") == 0)
              {  if (stype != CommaSym)
                   return 0;
                 nextsymbol();
                 if (! expression(&v2))
                   return 0;
                 if (stype == CommaSym && (strcmp(fname,"mid$") == 0 || strcmp(fname,"msgbox") == 0))
                   {  nextsymbol();
                      if (! expression(&v3))
                        return 0;
                   }
                 else
                   v3.setvalue(strcmp(fname,"msgbox") == 0 ? (long)MB_ICONINFORMATION : 0L);
              }
            if (stype != RBracketSym)
              return 0;
            nextsymbol();
            if (v1.gettype() == StringValue)
              v1.getvalue(&s1);
            else
              v1.getvalue(&p1);
            if (v2.gettype() == StringValue)
              v2.getvalue(&s2);
            else
              v2.getvalue(&p2);
            v3.getvalue(&p3);
         }
       if (strcmp(fname,"abs") == 0)
         v->setvalue(fabs(p1));
       else if (strcmp(fname,"val") == 0)
         v->setvalue(atof(s1));
       else if (strcmp(fname,"asc") == 0)
         v->setvalue(&s1[0]);
       else if (strcmp(fname,"chr$") == 0)
         {RCCHAR s[2];
            //s[0] = (char)p1;
            itoa((int)p1,&s[0],10);
            s[1] = 0;
            v->setvalue(s);
         }
       else if (strcmp(fname,"len") == 0)
         v->setvalue((long)strlen(s1));
       else if (strcmp(fname,"mid$") == 0)
         {RCCHAR string[1000];
            if (p2 - 1 < strlen(s1))
              {  strcpy(string,s1+(int)p2-1);
                 string[p3 >= 0 ? (int)p3 : 0] = 0;
                 v->setvalue(string);
              }
            else
              v->setvalue(_RCT(""));
         }
       else if (strcmp(fname,"acos") == 0)
         {  if (p1 < -1.0 || p1 > 1.0)
              return 0;
            v->setvalue(acos(p1) * 180.0 / M_PI);
         }
       else if (strcmp(fname,"asin") == 0)
         {  if (p1 < -1.0 || p1 > 1.0)
              return 0;
            v->setvalue(asin(p1) * 180.0 / M_PI);
         }
       else if (strcmp(fname,"atan") == 0)
         v->setvalue(atan(p1) * 180.0 / M_PI);
       else if (strcmp(fname,"atan2") == 0)
         v->setvalue(atan2(p1,p2) * 180.0 / M_PI);
       else if (strcmp(fname,"ceil") == 0)
         v->setvalue(ceil(p1));
       else if (strcmp(fname,"int") == 0)
         {  if (p1 < -MAXLONG || p1 > MAXLONG)
              return 0;
            v->setvalue(long(p1));
         }
       else if (strcmp(fname,"cos") == 0)
         v->setvalue(cos(p1 / 180.0 * M_PI));
       else if (strcmp(fname,"exp") == 0)
         {  if (p1 > log(MAXDOUBLE))
              return 0;
            v->setvalue(exp(p1));
         }
       else if (strcmp(fname,"log") == 0)
         {  if (p1 <= 0.0)
              return 0;
            v->setvalue(log(p1));
         }
       else if (strcmp(fname,"log10") == 0)
         {  if (p1 <= 0.0)
              return 0;
            v->setvalue(log10(p1));
         }
       else if (strcmp(fname,"fmod") == 0)
         {  if (fabs(p2) < 1.0E-10)
              return 0;
            v->setvalue(fmod(p1,p2));
         }
       else if (strcmp(fname,"pow") == 0)
         {  if (p1 <= 0.0 || p2 * log(p1) > log(MAXDOUBLE))
              return 0;
            v->setvalue(pow(p1,p2));
         }
       else if (strcmp(fname,"pow10") == 0)
         {  if (p1 > log10(MAXDOUBLE))
              return 0;
            v->setvalue(exp(log(10.0)*p1));
         }
       else if (strcmp(fname,"random") == 0)
         v->setvalue(double(rand()) / double(RAND_MAX));
       else if (strcmp(fname,"randomize") == 0)
         {  v->setvalue(0.0); /* randomize(12); */
         }
       else if (strcmp(fname,"tan") == 0)
         {  p1 *= M_PI / 180.0;
            if (fabs(fmod(p1 - M_PI / 2.0,M_PI)) < 1.0E-10)
              return 0;
            v->setvalue(tan(p1));
         }
       else if (strcmp(fname,"sqrt") == 0)
         {  if (p1 < 0.0)
              return 0;
            v->setvalue(sqrt(p1));
         }
       else if (strcmp(fname,"sin") == 0)
         v->setvalue(sin(p1 / 180.0 * M_PI));
       else if (strcmp(fname,"pi") == 0)
         v->setvalue(M_PI);
       else if (strcmp(fname,"mb_ok") == 0)
         v->setvalue(0L);
       else if (strcmp(fname,"mb_okcancel") == 0)
         v->setvalue(1L);
       else if (strcmp(fname,"mb_abortretrycancel") == 0)
         v->setvalue(2L);
       else if (strcmp(fname,"mb_yesnocancel") == 0)
         v->setvalue(3L);
       else if (strcmp(fname,"mb_yesno") == 0)
         v->setvalue(4L);
       else if (strcmp(fname,"mb_retrycancel") == 0)
         v->setvalue(5L);
       else if (strcmp(fname,"mb_iconstop") == 0)
         v->setvalue(0x10L);
       else if (strcmp(fname,"mb_iconquestion") == 0)
         v->setvalue(0x20L);
       else if (strcmp(fname,"mb_iconexclamation") == 0)
         v->setvalue(0x30L);
       else if (strcmp(fname,"mb_iconinformation") == 0)
         v->setvalue(0x40L);
       else if (strcmp(fname,"idok") == 0)
         v->setvalue(1L);
       else if (strcmp(fname,"idcancel") == 0)
         v->setvalue(2L);
       else if (strcmp(fname,"idabort") == 0)
         v->setvalue(3L);
       else if (strcmp(fname,"idretry") == 0)
         v->setvalue(4L);
       else if (strcmp(fname,"idignore") == 0)
         v->setvalue(5L);
       else if (strcmp(fname,"idyes") == 0)
         v->setvalue(6L);
       else if (strcmp(fname,"idno") == 0)
         v->setvalue(7L);
       else if (strcmp(fname,"msgbox") == 0)
         v->setvalue((long)cadwindow->MessageBox(s1,s2,(unsigned int)p3));
       else if (strcmp(fname,"str$") == 0)
         {RCCHAR buffer[2000];
            v->setvalue(v1.format(buffer,0,0));
         }
    }
  else if (stype == LBracketSym)
    {  nextsymbol();
       if (! lexpression(v))
         return 0;
       if (stype != RBracketSym)
         return 0;
       nextsymbol();
    }
  else
    return 0;
  if (_not)
    {  if (v->gettype() == IntegerValue)
         {  v->getvalue(&i1);  v->setvalue((long)(! i1));
         }
       else if (v->gettype() == RealValue)
         {  v->getvalue(&x1);  v->setvalue((long)(x1 != 0.0));
         }
    }
  return 1;
}

CadBasicProgram::CadBasicProgram()
{ResourceString rs0(NCBASIC);
  nlines = 0;
  inclines = 2000;
  maxlines = 2000;
  line = new RCCHAR *[maxlines];
  breakpoint = new RCCHAR [maxlines];
  changed = 0;
  currentlineno = 0;
  iflevel = whilelevel = forlevel = selectlevel = calllevel = 0;
  strcpy(filename,rs0.gets());
  strcpy(title,rs0.gets());
}

int CadBasicProgram::filesave(RCHWND hwnd)
{ResourceString rs0(NCBASIC),rs1(NCBASIC+1),rs2(NCBASIC+2),rs3(NCBASIC+3);
 FILE *outfile;
 long i;
  if (strcmp(filename,rs0.gets()) == 0)
    {  if (! filesaveas(hwnd))
         return 0;
    }
  if ((outfile = fopen(filename,"w")) == 0)
    {  cadwindow->MessageBox(rs1.gets(),rs2.gets(),MB_ICONINFORMATION);
       return 0;
    }
  else
    {  for (i = 0 ; i < nlines ; i++)
         if (fprintf(outfile,"%s\n",line[i]) == EOF)
           {  cadwindow->MessageBox(rs3.gets(),rs2.gets(),MB_ICONINFORMATION);
              fclose(outfile);
              return 0;
           }
       fclose(outfile);
       return 1;
    }
}

int CadBasicProgram::filesaveas(RCHWND hwnd)
{RCOPENFILENAME ofn;
 RCCHAR dirname[300],nfilename[300],nfiletitle[300], filter[300],oldname[300];
 RCUINT  i;
 ResourceString rs4(NCBASIC+4),rs5(NCBASIC+5),rs6(NCBASIC+6);
#if ! defined(_WIN32_WCE)
  _getcwd(dirname,sizeof(dirname));
//WCEFIX
#endif
  nfilename[0] = nfiletitle[0] = '\0';
  _tcscpy(filter,rs4.getws());
  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd == 0 ? cadwindow->gethwnd() : hwnd;
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= nfilename;
  ofn.nMaxFile = sizeof(nfilename);
  ofn.lpstrFileTitle = nfiletitle;
  ofn.nMaxFileTitle = sizeof(nfiletitle);
  ofn.lpstrInitialDir = dirname;
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
  ofn.lpstrDefExt =  rs5.getws();
  ofn.lpstrTitle =  rs6.getws();
  if (GetSaveFileName(&ofn))
    {  
#if ! defined(_WIN32_WCE)
       strcpy(oldname,filename);
       strcpy(filename,nfilename);
       if (filesave(hwnd))
         {  strcpy(title,nfiletitle);
            if (hwnd != 0)
              SetWindowText(hwnd,title);
            return 1;
         }
       strcpy(filename,oldname);
//WCEFIX
#endif
       return 0;
   }
  return 0;
}

int CadBasicProgram::filechanged(RCHWND hwnd)
{int status;
 ResourceString nc0(NCBASIC),nc7(NCBASIC+7);
  if (! changed) return 1;
  status = cadwindow->MessageBox(nc7.gets(),filename,MB_YESNOCANCEL);
  if (status == IDCANCEL)
    return 0;
  if (status == IDYES)
    filesave(hwnd);
  return 1;
}

int CadBasicProgram::filenew(RCHWND hwnd)
{long i;
  if (filechanged(hwnd))
    {  for (i = 0 ; i < nlines ; i++)
         delete [] line[i];
       nlines = 0;
       return 1;
    }
  else
    return 0;
}

int CadBasicProgram::fileopen(RCHWND hwnd)
{RCOPENFILENAME ofn;
 RCCHAR dirname[300],nfilename[300],nfiletitle[300],filter[300],l[300];
 RCUINT  i;
 ResourceString rs0(NCBASIC),rs4(NCBASIC+4),rs5(NCBASIC+5),rs8(NCBASIC+8);
 FILE *infile;

  if (! filechanged(hwnd))
    return 0;
#if ! defined(_WIN32_WCE)
  _getcwd(dirname, sizeof(dirname));
#endif
  nfilename[0] = nfiletitle[0] = title[0] = '\0';
  _tcscpy(filter,rs4.getws());
  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hwnd == 0 ? cadwindow->gethwnd() : hwnd;
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= nfilename;
  ofn.nMaxFile = sizeof(nfilename);
  ofn.lpstrFileTitle = nfiletitle;
  ofn.nMaxFileTitle = sizeof(nfiletitle);
  ofn.lpstrInitialDir = dirname;
  ofn.lpstrDefExt =  rs5.getws();
  ofn.lpstrTitle =  rs8.getws();
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (GetOpenFileName(&ofn))
    {  nlines = 0;
#if ! defined(_WIN32_WCE)
       if ((infile = fopen(nfilename,"r")) != 0)
         {  while (fgets(l,300,infile) != 0)
              {  line[nlines] = new RCCHAR[300];
                 if (strlen(l) > 0 && l[strlen(l)-1] == '\n')
                   l[strlen(l)-1] = 0;
                 strcpy(line[nlines],l);
                 breakpoint[nlines] = 0;
                 nlines++;
              }
            fclose(infile);
            strcpy(filename,nfilename);
            strcpy(title,nfiletitle);
            if (hwnd != 0)
              SetWindowText(hwnd,title);
            return 1;
         }
       else
//WCEFIX
#endif
         return 0;
    }
  else
    return 0;
}

int CadBasicProgram::getbreakpoint(long lineno)
{  if (lineno < nlines)
     return (int)breakpoint[lineno].toLatin1();
   else
     return 0;
}

void CadBasicProgram::setbreakpoint(long lineno,int set)
{ breakpoint[lineno] = (RCCHAR) set;
}

int CadBasicProgram::run(int reset)
{RCCHAR vname[300],fname[300];
 Variant v,v1,v2,v3,*lvalue;
 double x;
 long i;
 int level,fid;

  if (reset)
    {  currentlineno = 0;
       rtsymbols.clear();
       iflevel = whilelevel = forlevel = selectlevel = calllevel = 0;
       basicopenfiles.closeall();
       return 1;
    }
  if (currentlineno > nlines)
    return 0;

  pos = 0;
  nextsymbol();
  if (stype == DimSym)
    {  nextsymbol();
       while (stype == VariableNameSym)
         {  strcpy(vname,sname);  _strlwr(vname);
            nextsymbol();
            if (stype == AsSym)
              {  nextsymbol();
                 if (stype == VariableNameSym)
                   {  _strlwr(sname);
                      if (strcmp(sname,"integer") == 0)
                        rtsymbols.setvalue(vname,0L);
                      else if (strcmp(sname,"string") == 0)
                        rtsymbols.setvalue(vname,_RCT(""));
                      else if (strcmp(sname,"real") == 0)
                        rtsymbols.setvalue(vname,0.0);
                      else if (strcmp(sname,"point") == 0)
                        rtsymbols.setvalue(vname,PointValue,new Point);
                      else if (strcmp(sname,"pointe") == 0)
                        {PointE *point;
                           rtsymbols.setvalue(vname,PointEValue,point = new PointE);
                           point->defined = 1;
                           point->p.setxyz(0.0,0.0,0.0);
                        }
                      else if (strcmp(sname,"line") == 0)
                        {Line *line;
                           rtsymbols.setvalue(vname,LineValue,line = new Line);
                           line->defined = 1;
                           line->p1.setxyz(0.0,0.0,0.0);  
                           line->p2.setxyz(0.0,0.0,0.0);  
                        }
                      else if (strcmp(sname,"circle") == 0)
                        {Circle *circle;
                           rtsymbols.setvalue(vname,CircleValue,circle = new Circle);
                           circle->defined = 0;
                           circle->origin.setxyz(0.0,0.0,0.0);
                           circle->xaxis.setxyz(1.0,0.0,0.0);
                           circle->yaxis.setxyz(0.0,1.0,0.0);
                        }
                      else if (strcmp(sname,"position") == 0)
                        rtsymbols.setvalue(vname,PointValue,new Point);
                      else if (strcmp(sname,"dialog") == 0)
                        rtsymbols.setvalue(vname,DialogValue,new Dialog(""));
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Unknown type name in dimension statement.","Run",MB_ICONSTOP);
                        }
                      if (stype != BadSym)
                        nextsymbol();
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Syntax error in dimension statement.","Run",MB_ICONSTOP);
                   }
              }
            else
              {  stype = BadSym;
                 cadwindow->MessageBox("Syntax error in dimension statement.","Run",MB_ICONSTOP);
              }
            if (stype == CommaSym)
              nextsymbol();
            else if (stype == EolSym)
              currentlineno++;
            else
              {  stype = BadSym;
                 cadwindow->MessageBox("Syntax error in dimension statement.","Run",MB_ICONSTOP);
                 break;
              }
         }
    }
  else if (stype == IfSym)
    {  nextsymbol();
       lexpression(&v);
       if (v.gettype() != IntegerValue && v.gettype() != RealValue)
         {  stype = BadSym;
            cadwindow->MessageBox("Integer or Real expression expected.","Run",MB_ICONSTOP);
         }
       else if (stype == ThenSym)
         {  nextsymbol();
            if (stype != EolSym)
              cadwindow->MessageBox("Error in if..then statement","Run",MB_ICONSTOP);
            else
              {  v.getvalue(&x);
                 ifstack[iflevel].result = x != 0.0;
                 iflevel++;
                 if (ifstack[iflevel-1].result != 0)
                   currentlineno++;
                 else
                   {  level = 1;
                      for (;;)
                        {  currentlineno++;
                           if (currentlineno == nlines)
                             {  stype = BadSym;
                                cadwindow->MessageBox("Missing endif statement.","Run",MB_ICONSTOP);
                                break;
                             }
                           pos = 0;
                           nextsymbol();
                           if (stype == IfSym)
                             level++;
                           else if (stype == EndIfSym)
                             {  level--;
                                if (level == 0)
                                  {  nextsymbol();
                                     if (stype != EolSym)
                                       cadwindow->MessageBox("Else statement syntax error.","Run",MB_ICONSTOP);
                                     else
                                       {  currentlineno++;
                                          iflevel--;
                                       }
                                     break;
                                  }
                             }
                           else if (level == 1 && stype == ElseSym)
                             {  nextsymbol();
                                if (stype != EolSym)
                                  cadwindow->MessageBox("Else statement syntax error.","Run",MB_ICONSTOP);
                                else
                                  currentlineno++;
                                break;
                             }
                        }
                   }
              }
         }
       else
         {  stype = BadSym;
            cadwindow->MessageBox("Then expected.","Run",MB_ICONSTOP);
         }
    }
  else if (stype == ElseSym)
    {  if (iflevel == 0 || ifstack[iflevel-1].result == 0)
         {  stype = BadSym;
            cadwindow->MessageBox("Misplaced else.","Run",MB_ICONSTOP);
         }
       else
         {  nextsymbol();
            if (stype != EolSym)
              cadwindow->MessageBox("Else statement syntax error.","Run",MB_ICONSTOP);
            else
              {  level = 1;
                 for (;;)
                   {  currentlineno++;
                      if (currentlineno == nlines)
                        {  stype = BadSym;
                           cadwindow->MessageBox("Missing endif statement.","Run",MB_ICONSTOP);
                           break;
                        }
                      pos = 0;
                      nextsymbol();
                      if (stype == IfSym)
                        level++;
                      else if (stype == EndIfSym)
                        {  level--;
                           if (level == 0)
                             {  nextsymbol();
                                if (stype != EolSym)
                                  cadwindow->MessageBox("Else statement syntax error.","Run",MB_ICONSTOP);
                                else
                                  {  currentlineno++;
                                     iflevel--;
                                  }
                                break;
                             }
                        }
                   }
              }
         }
    }
  else if (stype == EndIfSym)
    {  if (iflevel == 0)
         {  stype = BadSym;
            cadwindow->MessageBox("Misplaced endif.","Run",MB_ICONSTOP);
         }
       else
         {  nextsymbol();
            if (stype != EolSym)
              cadwindow->MessageBox("Endif statement syntax error.","Run",MB_ICONSTOP);
            else
              {  iflevel--;
                 currentlineno++;
              }
         }
    }
  else if (stype == FunctionSym || stype == SubSym)
    {  for (;;)
         {  currentlineno++;
            if (currentlineno == nlines)
              {  stype = BadSym;
                 cadwindow->MessageBox("Missing end statement.","Run",MB_ICONSTOP);
                 break;
              }
            pos = 0;
            nextsymbol();
            if (stype == EndSym)
              {  stype = EolSym;
                 currentlineno++;
                 break;
              }
         }
    }
  else if (stype == OpenSym)
    {RCCHAR filename[300],mode[10],*fn;
     long filenumber;
       nextsymbol();
       lexpression(&v);
       if (v.gettype() != StringValue)
         {  stype = BadSym;
            cadwindow->MessageBox("String expression expected.","Run",MB_ICONSTOP);
         }
       else
         {  v.getvalue(&fn);
            strcpy(filename,fn);
            if (stype != ForSym)
              {  stype = BadSym;
                 cadwindow->MessageBox("For expected in open statement","Run",MB_ICONSTOP);
              }
            else
              {  nextsymbol();
                 if (stype != InputSym && stype != OutputSym)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Error in open statement","Run",MB_ICONSTOP);
                   }
                 else
                    {  strcpy(mode,stype == InputSym ? "r" : "w");
                       nextsymbol();
                       if (stype != AsSym)
                         {  stype = BadSym;
                            cadwindow->MessageBox("As expected in open statement","Run",MB_ICONSTOP);
                         }
                       else
                         {  nextsymbol();
                            if (stype != HashSym)
                              {  stype = BadSym;
                                 cadwindow->MessageBox("# expected in open statement","Run",MB_ICONSTOP);
                              }
                            else
                              {  nextsymbol();
                                 lexpression(&v);
                                 if (v.gettype() != IntegerValue)
                                   {  stype = BadSym;
                                      cadwindow->MessageBox("Integer expected in open statement","Run",MB_ICONSTOP);
                                   }
                                 else
                                   {  if (stype != EolSym)
                                        {  stype = BadSym;
                                           cadwindow->MessageBox("Integer expected in open statement","Run",MB_ICONSTOP);
                                        }
                                      else
                                        {  v.getvalue(&filenumber);
                                           if (! basicopenfiles.open(filename,mode,filenumber))
                                             {  stype = BadSym;
                                                cadwindow->MessageBox("File could not be opened","Run",MB_ICONSTOP);
                                             }
                                           else
                                             currentlineno++;
                                        }
                                   }
                              }
                         }
                    }
              }
         }
    }
  else if (stype == CloseSym)
    {long filenumber;
       nextsymbol();
       if (stype != HashSym)
         {  stype = BadSym;
            cadwindow->MessageBox("# expected.","Run",MB_ICONSTOP);
         }
       else
         {  nextsymbol();
            lexpression(&v);
            if (v.gettype() != IntegerValue)
              {  stype = BadSym;
                 cadwindow->MessageBox("Integer expected in open statement","Run",MB_ICONSTOP);
              }
            else
              {  v.getvalue(&filenumber);
                 if (! basicopenfiles.close(filenumber))
                   {  stype = BadSym;
                      cadwindow->MessageBox("File could not be closed","Run",MB_ICONSTOP);
                   }
                 else
                   currentlineno++;
              }
         }
    }
  else if (stype == PrintSym)
    {long filenumber;
     RCCHAR line[2000];
       nextsymbol();
       if (stype != HashSym)
         {  stype = BadSym;
            cadwindow->MessageBox("# expected.","Run",MB_ICONSTOP);
         }
       else
         {  nextsymbol();
            lexpression(&v);
            if (v.gettype() != IntegerValue)
              {  stype = BadSym;
                 cadwindow->MessageBox("Integer expected in print statement","Run",MB_ICONSTOP);
              }
            else
              {  v.getvalue(&filenumber);
                 if (stype != CommaSym)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Comma expected in print statement","Run",MB_ICONSTOP);
                   }
                 else
                   {  nextsymbol();
                      line[0] = 0;
                      while (stype != BadSym && stype != EolSym)
                        {  lexpression(&v);
                           v.format(line+strlen(line),0,0);
                           if (stype == CommaSym)
                             {  strcat(line," ");
                                while ((strlen(line) % 4) != 0)
                                  strcat(line," ");
                                nextsymbol();
                             }
                           else if (stype == SemiColonSym)
                             nextsymbol();
                           else if (stype != EolSym)
                             stype = BadSym;
                        }
                      if (stype != EolSym)
                        {  stype = BadSym;
                           cadwindow->MessageBox("Bad symbol in print statement","Run",MB_ICONSTOP);
                        }
                      else
                        {  if (! basicopenfiles.writeline(filenumber,line))
                             {  stype = BadSym;
                                cadwindow->MessageBox("Error printing to file.","Run",MB_ICONSTOP);
                             }
                           else
                             currentlineno++;
                        }
                   }
              }
         }
    }
  else if (stype == InputSym)
    {long filenumber;
     RCCHAR line[2000],*s;
       nextsymbol();
       if (stype != HashSym)
         {  stype = BadSym;
            cadwindow->MessageBox("# expected.","Run",MB_ICONSTOP);
         }
       else
         {  nextsymbol();
            lexpression(&v);
            if (v.gettype() != IntegerValue)
              {  stype = BadSym;
                 cadwindow->MessageBox("Integer expected in input statement","Run",MB_ICONSTOP);
              }
            else
              {  v.getvalue(&filenumber);
                 if (stype != CommaSym)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Comma expected in input statement","Run",MB_ICONSTOP);
                   }
                 else
                   {  nextsymbol();
                      if (! basicopenfiles.readline(filenumber,line,sizeof(line)))
                        {  stype = BadSym;
                           cadwindow->MessageBox("Error reading from file.","Run",MB_ICONSTOP);
                        }
                      else
                        {  s = line;
                           while (stype != BadSym && stype != EolSym)
                             {  if (stype != VariableNameSym)
                                  {  stype = BadSym;
                                     cadwindow->MessageBox("Variable name expected in input statement","Run",MB_ICONSTOP);
                                  }
                                else
                                  {Variant *v;
                                     if ((v = rtsymbols.match(sname)) == 0)
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("Bad variable name in input statement","Run",MB_ICONSTOP);
                                       }
                                     else
                                       {  v->read(s,&s);
                                          nextsymbol();
                                       }
                                  }
                                if (stype == CommaSym)
                                  nextsymbol();
                                else if (stype != EolSym)
                                  stype = BadSym;
                             }
                           if (stype != EolSym)
                             {  stype = BadSym;
                                cadwindow->MessageBox("Bad symbol in input statement","Run",MB_ICONSTOP);
                             }
                           else
                             {  currentlineno++;
                             }
                        }
                   }
              }
         }
    }
  else if (stype == DoSym)
    {  nextsymbol();
       if (stype == WhileSym)
         {  nextsymbol();
            lexpression(&v);
            if (stype != EolSym)
              cadwindow->MessageBox("Error in do while statement","Run",MB_ICONSTOP);
            else
              {  if (v.gettype() == IntegerValue || v.gettype() == RealValue)
                   {  v.getvalue(&x);
                      if (x != 0.0)
                        {  whilestack[whilelevel].lineno = currentlineno;
                           whilelevel++;
                           currentlineno++;
                        }
                      else
                        {  level = 1;
                           for (;;)
                             {  currentlineno++;
                                if (currentlineno == nlines)
                                  {  stype = BadSym;
                                     cadwindow->MessageBox("Missing loop statement.","Run",MB_ICONSTOP);
                                     break;
                                  }
                                pos = 0;
                                nextsymbol();
                                if (stype == DoSym)
                                  level++;
                                else if (stype == LoopSym)
                                  level--;
                                if (level == 0)
                                  {  nextsymbol();
                                     if (stype != EolSym)
                                       cadwindow->MessageBox("Loop statement syntax error.","Run",MB_ICONSTOP);
                                     else
                                       currentlineno++;
                                     break;
                                  }
                             }
                        }
                   }
              }
         }
       else
         {  stype = BadSym;
            cadwindow->MessageBox("WHILE expected.","Run",MB_ICONSTOP);
         }
    }
  else if (stype == LoopSym)
    {  nextsymbol();
       if (stype != EolSym)
         {  stype = BadSym;
            cadwindow->MessageBox("Loop statement syntax error.","Run",MB_ICONSTOP);
         }
       else if (whilelevel == 0)
         {  stype = BadSym;
            cadwindow->MessageBox("Misplaced Loop statement.","Run",MB_ICONSTOP);
         }
       else
         {  whilelevel--;
            currentlineno = whilestack[whilelevel].lineno;
         }
    }
  else if (stype == ForSym)
    {  nextsymbol();
       if (stype == VariableNameSym)
         {  _strlwr(sname);
            strcpy(forstack[forlevel].name,sname);
            nextsymbol();
            if (stype == EqualsSym)
              {  nextsymbol();
                 if (! lexpression(&v1))
                   {  stype = BadSym;
                      cadwindow->MessageBox("Equal expected.","Run",MB_ICONSTOP);
                   }
                 else if (stype == ToSym)
                   {  nextsymbol();
                      if (! lexpression(&v2))
                        {  stype = BadSym;
                           cadwindow->MessageBox("Expression expected.","Run",MB_ICONSTOP);
                        }
                      else
                        {  v3.setvalue(1L);
                           if (stype == StepSym)
                             {  nextsymbol();
                                if (! lexpression(&v3))
                                  {  stype = BadSym;
                                     cadwindow->MessageBox("Expression expected.","Run",MB_ICONSTOP);
                                  }
                             }
                           if (stype == EolSym)
                             {  if (v1.gettype() == IntegerValue && v2.gettype() == IntegerValue && v3.gettype() == IntegerValue)
                                  {  forstack[forlevel].type = IntegerValue;
                                     forstack[forlevel].lineno = currentlineno + 1;
                                     v1.getvalue(&i);  rtsymbols.setvalue(forstack[forlevel].name,i);
                                     v2.getvalue(&forstack[forlevel].ilast);
                                     v3.getvalue(&forstack[forlevel].istep);
                                  }
                                else
                                  {  forstack[forlevel].type = RealValue;
                                     forstack[forlevel].lineno = currentlineno + 1;
                                     v1.getvalue(&x);  rtsymbols.setvalue(forstack[forlevel].name,x);
                                     v2.getvalue(&forstack[forlevel].xlast);
                                     v3.getvalue(&forstack[forlevel].xstep);
                                  }
                                currentlineno++;
                                forlevel++;
                             }
                        }
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Equal expected.","Run",MB_ICONSTOP);
                   }


              }
            else
              {  stype = BadSym;
                 cadwindow->MessageBox("Equal expected.","Run",MB_ICONSTOP);
              }
         }
       else
         {  stype = BadSym;
            cadwindow->MessageBox("Variable name expected.","Run",MB_ICONSTOP);
         }
    }
  else if (stype == NextSym)
    {  nextsymbol();
       if (forlevel == 0)
         {  stype = BadSym;
            cadwindow->MessageBox("Next expected.","Run",MB_ICONSTOP);
         }
       else
         {  if (stype == VariableNameSym)
              {  _strlwr(sname);
                 if (strcmp(forstack[forlevel-1].name,sname) != 0)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Incorrectly nested for loop.","Run",MB_ICONSTOP);
                   }
                 else
                   nextsymbol();
              }
            if (stype != EolSym && stype != BadSym)
              {  stype = BadSym;
                 cadwindow->MessageBox("Syntax error in next statement.","Run",MB_ICONSTOP);
              }
            if (stype == EolSym)
              {  if (forstack[forlevel-1].type == IntegerValue)
                   {  rtsymbols.getvalue(forstack[forlevel-1].name,&i);
                      i += forstack[forlevel-1].istep;
                      rtsymbols.setvalue(forstack[forlevel-1].name,i);
                      if (i <= forstack[forlevel-1].ilast)
                        currentlineno = forstack[forlevel-1].lineno;
                      else
                        {  currentlineno++;
                           forlevel--;
                        }
                   }
                 else
                   {  rtsymbols.getvalue(forstack[forlevel-1].name,&x);
                      x += forstack[forlevel-1].xstep;
                      rtsymbols.setvalue(forstack[forlevel-1].name,x);
                      if (x <= forstack[forlevel-1].xlast)
                        currentlineno = forstack[forlevel-1].lineno;
                      else
                        {  currentlineno++;
                           forlevel--;
                        }
                   }
              }
         }
    }
  else if (stype == EndSym)
    {  nextsymbol();
       if (calllevel == 0)
         {  stype = BadSym;
            cadwindow->MessageBox("End not expected.","Run",MB_ICONSTOP);
         }
       else
         {  if (stype != FunctionSym && stype != SubSym)
              {  stype = BadSym;
                 cadwindow->MessageBox("Sub or Function expected.","Run",MB_ICONSTOP);
              }
            else
              nextsymbol();
            if (stype != EolSym && stype != BadSym)
              {  stype = BadSym;
                 cadwindow->MessageBox("Symbols after end statement.","Run",MB_ICONSTOP);
              }
            if (stype == EolSym)
              {  currentlineno = callstack[calllevel-1].lineno;
                 currentlineno++;
                 calllevel--;
              }
         }
    }
  else if (stype == SelectSym)
    {  cadwindow->MessageBox("Unknown statement type","Run",MB_ICONSTOP);
       return 0;
    }
  else if (stype == CaseSym)
    {  cadwindow->MessageBox("Unknown statement type","Run",MB_ICONSTOP);
       return 0;
    }
  else if (stype == EndSym)
    {  nextsymbol();
       if (stype != EolSym)
         cadwindow->MessageBox("Syntax error in end statement.","Run",MB_ICONSTOP);
       stype = BadSym;
    }
  else if (stype == FunctionNameSym)
    {  stype = BadSym;
       cadwindow->MessageBox("Unknown statement type","Run",MB_ICONSTOP);
       return 0;
    }
  else if (stype == VariableNameSym)
    {long lineno;
       strcpy(vname,sname);
       nextsymbol();
       if (stype == LBracketSym)
         {  nextsymbol();
            if (stype != RBracketSym)
              {  stype = BadSym;
                 cadwindow->MessageBox(") expected","Run",MB_ICONSTOP);
                 return 0;
              }
            lineno = currentlineno;
            for (currentlineno = 0 ; ; currentlineno++)
              {  if (currentlineno == nlines)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Missing sub.","Run",MB_ICONSTOP);
                      break;
                   }
                  pos = 0;
                  nextsymbol();
                  if (stype == SubSym)
                    {  nextsymbol();
#if ! defined(_WIN32_WCE)
                       if (stype == VariableNameSym && _stricmp(vname,sname) == 0)
//WCEFIX
#endif
                         {  nextsymbol();
                            if (stype != EolSym)
                              {  stype = BadSym;
                                 cadwindow->MessageBox("Else statement syntax error.","Run",MB_ICONSTOP);
                              }
                            else
                              {  callstack[calllevel].lineno = lineno;
                                 calllevel++;
                                 currentlineno++;
                                 return 1;
                              }
                            break;
                        }
                    }
              }
         }
       else if (stype == DotSym)
         {  lvalue = rtsymbols.match(vname);
            if (lvalue != 0 && lvalue->gettype() == LineValue)
              {  nextsymbol();
                 if (stype == VariableNameSym)
                   {  strcpy(fname,sname);  _strlwr(fname);
                      nextsymbol();
                      if (strcmp(fname,"x1") == 0)
                        fid = 0;
                      else if (strcmp(fname,"y1") == 0)
                        fid = 1;
                      else if (strcmp(fname,"z1") == 0)
                        fid = 2;
                      else if (strcmp(fname,"x2") == 0)
                        fid = 3;
                      else if (strcmp(fname,"y2") == 0)
                        fid = 4;
                      else if (strcmp(fname,"z2") == 0)
                        fid = 5;
                      else if (strcmp(fname,"colour") == 0)
                        fid = 6;
                      else if (strcmp(fname,"layer") == 0)
                        fid = 7;
                      else if (strcmp(fname,"style") == 0)
                        fid = 8;
                      else if (strcmp(fname,"weight") == 0)
                        fid = 9;
                      else if (strcmp(fname,"insert") == 0)
                        fid = 10;
                      else if (strcmp(fname,"default") == 0)
                        fid = 11;
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Field name is not unknown.","Run CAD Basic",MB_ICONSTOP);
                        }
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Field name expected.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else if (lvalue != 0 && lvalue->gettype() == PointEValue)
              {  nextsymbol();
                 if (stype == VariableNameSym)
                   {  strcpy(fname,sname);  _strlwr(fname);
                      nextsymbol();
                      if (strcmp(fname,"x") == 0)
                        fid = 0;
                      else if (strcmp(fname,"y") == 0)
                        fid = 1;
                      else if (strcmp(fname,"z") == 0)
                        fid = 2;
                      else if (strcmp(fname,"colour") == 0)
                        fid = 6;
                      else if (strcmp(fname,"layer") == 0)
                        fid = 7;
                      else if (strcmp(fname,"style") == 0)
                        fid = 8;
                      else if (strcmp(fname,"weight") == 0)
                        fid = 9;
                      else if (strcmp(fname,"insert") == 0)
                        fid = 10;
                      else if (strcmp(fname,"default") == 0)
                        fid = 11;
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Field name is not unknown.","Run CAD Basic",MB_ICONSTOP);
                        }
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Field name expected.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else if (lvalue != 0 && lvalue->gettype() == CircleValue)
              {  nextsymbol();
                 if (stype == VariableNameSym)
                   {  strcpy(fname,sname);  _strlwr(fname);
                      nextsymbol();
                      if (strcmp(fname,"xorigin") == 0)
                        fid = 0;
                      else if (strcmp(fname,"yorigin") == 0)
                        fid = 1;
                      else if (strcmp(fname,"zorigin") == 0)
                        fid = 2;
                      else if (strcmp(fname,"xxaxis") == 0)
                        fid = 3;
                      else if (strcmp(fname,"yxaxis") == 0)
                        fid = 4;
                      else if (strcmp(fname,"zxaxis") == 0)
                        fid = 5;
                      else if (strcmp(fname,"xyaxis") == 0)
                        fid = 6;
                      else if (strcmp(fname,"yyaxis") == 0)
                        fid = 7;
                      else if (strcmp(fname,"zyaxis") == 0)
                        fid = 8;
                      else if (strcmp(fname,"radius") == 0)
                        fid = 9;
                      else if (strcmp(fname,"sweepa") == 0)
                        fid = 10;
                      else if (strcmp(fname,"starta") == 0)
                        fid = 11;
                      else if (strcmp(fname,"colour") == 0)
                        fid = 12;
                      else if (strcmp(fname,"layer") == 0)
                        fid = 13;
                      else if (strcmp(fname,"style") == 0)
                        fid = 14;
                      else if (strcmp(fname,"weight") == 0)
                        fid = 15;
                      else if (strcmp(fname,"insert") == 0)
                        fid = 16;
                      else if (strcmp(fname,"default") == 0)
                        fid = 17;
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Field name is not unknown.","Run CAD Basic",MB_ICONSTOP);
                        }
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Field name expected.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else if (lvalue != 0 && lvalue->gettype() == PointValue)
              {  nextsymbol();
                 if (stype == VariableNameSym)
                   {  strcpy(fname,sname);  _strlwr(fname);
                      nextsymbol();
                      if (strcmp(fname,"x") == 0)
                        fid = 0;
                      else if (strcmp(fname,"y") == 0)
                        fid = 1;
                      else if (strcmp(fname,"z") == 0)
                        fid = 2;
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Field name is not unknown.","Run CAD Basic",MB_ICONSTOP);
                        }
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Field name expected.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else if (lvalue != 0 && lvalue->gettype() == DialogValue)
              {  nextsymbol();
                 if (stype == VariableNameSym)
                   {  strcpy(fname,sname);  _strlwr(fname);
                      nextsymbol();
                      if (strcmp(fname,"define") == 0)
                        fid = 0;
                      else if (strcmp(fname,"control") == 0)
                        fid = 1;
                      else if (strcmp(fname,"process") == 0)
                        fid = 2;
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Field name is not unknown.","Run CAD Basic",MB_ICONSTOP);
                        }
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Field name expected.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else
              {  stype = BadSym;
                 cadwindow->MessageBox("Variable is not structured.","Run CAD Basic",MB_ICONSTOP);
              }
         }
       else
         lvalue = 0;
       if (stype == EqualsSym)
         {  nextsymbol();
            lexpression(&v);
            if (stype == EolSym)
              {  if (v.gettype() == RealValue)
                   {  v.getvalue(&x);
                      if (lvalue != 0 && lvalue->gettype() == LineValue)
                        {Line *line;
                         Point wp1,wp2;
                             
                           lvalue->getvalue(LineValue,(void **) &line);

                           wp1 = line->p1;
                           wp2 = line->p2;

                           if (rtsymbols.getworkplanecoordinates())
                             {  wp1 = db.workplanes.getcurrent()->modeltoworkplane(wp1);
                                wp2 = db.workplanes.getcurrent()->modeltoworkplane(wp2);
                             }

                           switch (fid)
                             {case 0 :  wp1.x = x;  break;
                              case 1 :  wp1.y = x;  break;
                              case 2 :  wp1.z = x;  break;
                              case 3 :  wp2.x = x;  break;
                              case 4 :  wp2.y = x;  break;
                              case 5 :  wp2.z = x;  break;
                              case 6 :  line->colour = (Byte)x;  break;
                              case 7 :  line->layer = (short)x;  break;
                              case 8 :  line->style = (Byte)x;  break;
                              case 9 :  line->weight = (Byte)x;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }
                           if (rtsymbols.getworkplanecoordinates())
                             {  line->p1 = db.workplanes.getcurrent()->workplanetomodel(wp1);
                                line->p2 = db.workplanes.getcurrent()->workplanetomodel(wp2);
                             }
                           else
                             {  line->p1 = wp1;
                                line->p2 = wp2;
                             }

                        }
                      else if (lvalue != 0 && lvalue->gettype() == PointEValue)
                        {PointE *point;
                         Point wp;

                           lvalue->getvalue(PointEValue,(void **) &point);
                             
                           wp = point->p;

                           if (rtsymbols.getworkplanecoordinates())
                             wp = db.workplanes.getcurrent()->modeltoworkplane(wp);

                           switch (fid)
                             {case 0 :  wp.x = x;  break;
                              case 1 :  wp.y = x;  break;
                              case 2 :  wp.z = x;  break;
                              case 6 :  point->colour = (Byte)x;  break;
                              case 7 :  point->layer = (short)x;  break;
                              case 8 :  point->style = (Byte)x;  break;
                              case 9 :  point->weight = (Byte)x;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }
                           if (rtsymbols.getworkplanecoordinates())
                             point->p = db.workplanes.getcurrent()->workplanetomodel(wp);
                           else
                             point->p = wp;

                        }
                      else if (lvalue != 0 && lvalue->gettype() == CircleValue)
                        {Circle *circle;
                         Point wpo,wpxa,wpya;
                             
                           lvalue->getvalue(CircleValue,(void **) &circle);

                           wpo = circle->origin;
                           wpxa = circle->xaxis;
                           wpya = circle->yaxis;

                           if (rtsymbols.getworkplanecoordinates())
                             {  wpo = db.workplanes.getcurrent()->modeltoworkplane(wpo);
                                wpxa = db.workplanes.getcurrent()->modeltoworkplane(wpxa + circle->origin) - wpo;
                                wpya = db.workplanes.getcurrent()->modeltoworkplane(wpya + circle->origin) - wpo;
                             }

                           switch (fid)
                             {case 0 :  wpo.x = x;  break;
                              case 1 :  wpo.y = x;  break;
                              case 2 :  wpo.z = x;  break;
                              case 3 :  wpxa.x = x;  break;
                              case 4 :  wpxa.y = x;  break;
                              case 5 :  wpxa.z = x;  break;
                              case 6 :  wpya.x = x;  break;
                              case 7 :  wpya.y = x;  break;
                              case 8 :  wpya.z = x;  break;
                              case 9 :  circle->radius = x;  break;
                              case 10 :  circle->sweepa = x * M_PI / 180.0;  break;
                              case 11 :  circle->starta = x * M_PI / 180.0;  break;
                              case 12 :  circle->colour = (Byte)x;  break;
                              case 13 :  circle->layer = (short)x;  break;
                              case 14 :  circle->style = (Byte)x;  break;
                              case 15 :  circle->weight = (Byte)x;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }

                           if (rtsymbols.getworkplanecoordinates())
                             {  circle->xaxis = db.workplanes.getcurrent()->workplanetomodel(wpxa + wpo);
                                circle->yaxis = db.workplanes.getcurrent()->workplanetomodel(wpya + wpo);
                                circle->origin = db.workplanes.getcurrent()->workplanetomodel(wpo);
                                circle->xaxis -= circle->origin;  
                                circle->yaxis -= circle->origin;  
                             }
                           else
                             {  circle->origin = wpo;
                                circle->xaxis = wpxa;
                                circle->yaxis = wpya;
                             }

                        }
                      else if (lvalue != 0 && lvalue->gettype() == PointValue)
                        {Point *p;
                           lvalue->getvalue(PointValue,(void **) &p);
                           switch (fid)
                             {case 0 :  p->x = x;  break;
                              case 1 :  p->y = x;  break;
                              case 2 :  p->z = x;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }
                        }
                      else
                        rtsymbols.setvalue(vname,x);
                      if (stype != BadSym)
                        currentlineno++;
                   }
                 else if (v.gettype() == IntegerValue)
                   {  v.getvalue(&i);
                      if (lvalue != 0 && lvalue->gettype() == LineValue)
                        {Line *line;
                         Point wp1,wp2;

                           lvalue->getvalue(LineValue,(void **) &line);
                             
                           wp1 = line->p1;
                           wp2 = line->p2;

                           if (rtsymbols.getworkplanecoordinates())
                             {  wp1 = db.workplanes.getcurrent()->modeltoworkplane(wp1);
                                wp2 = db.workplanes.getcurrent()->modeltoworkplane(wp2);
                             }

                           switch (fid)
                             {case 0 :  wp1.x = i;  break;
                              case 1 :  wp1.y = i;  break;
                              case 2 :  wp1.z = i;  break;
                              case 3 :  wp2.x = i;  break;
                              case 4 :  wp2.y = i;  break;
                              case 5 :  wp2.z = i;  break;
                              case 6 :  line->colour = (Byte)i;  break;
                              case 7 :  line->layer = (short)i;  break;
                              case 8 :  line->style = (Byte)i;  break;
                              case 9 :  line->weight = (Byte)i;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }
                           if (rtsymbols.getworkplanecoordinates())
                             {  line->p1 = db.workplanes.getcurrent()->workplanetomodel(wp1);
                                line->p2 = db.workplanes.getcurrent()->workplanetomodel(wp2);
                             }
                           else
                             {  line->p1 = wp1;
                                line->p2 = wp2;
                             }
                        }
                      else if (lvalue != 0 && lvalue->gettype() == PointEValue)
                        {PointE *point;
                         Point wp;

                           lvalue->getvalue(PointEValue,(void **) &point);
                             
                           wp = point->p;

                           if (rtsymbols.getworkplanecoordinates())
                             wp = db.workplanes.getcurrent()->modeltoworkplane(wp);

                           switch (fid)
                             {case 0 :  wp.x = i;  break;
                              case 1 :  wp.y = i;  break;
                              case 2 :  wp.z = i;  break;
                              case 6 :  point->colour = (Byte)i;  break;
                              case 7 :  point->layer = (short)i;  break;
                              case 8 :  point->style = (Byte)i;  break;
                              case 9 :  point->weight = (Byte)i;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }
                           if (rtsymbols.getworkplanecoordinates())
                             point->p = db.workplanes.getcurrent()->workplanetomodel(wp);
                           else
                             point->p = wp;

                        }
                      else if (lvalue != 0 && lvalue->gettype() == CircleValue)
                        {Circle *circle;
                         Point wpo,wpxa,wpya;
                             
                           lvalue->getvalue(CircleValue,(void **) &circle);

                           wpo = circle->origin;
                           wpxa = circle->xaxis;
                           wpya = circle->yaxis;

                           if (rtsymbols.getworkplanecoordinates())
                             {  wpo = db.workplanes.getcurrent()->modeltoworkplane(wpo);
                                wpxa = db.workplanes.getcurrent()->modeltoworkplane(wpxa + circle->origin) - wpo;
                                wpya = db.workplanes.getcurrent()->modeltoworkplane(wpya + circle->origin) - wpo;
                             }

                           switch (fid)
                             {case 0 :  wpo.x = i;  break;
                              case 1 :  wpo.y = i;  break;
                              case 2 :  wpo.z = i;  break;
                              case 3 :  wpxa.x = i;  break;
                              case 4 :  wpxa.y = i;  break;
                              case 5 :  wpxa.z = i;  break;
                              case 6 :  wpya.x = i;  break;
                              case 7 :  wpya.y = i;  break;
                              case 8 :  wpya.z = i;  break;
                              case 9 :  circle->radius = i;  break;
                              case 10 :  circle->sweepa = i * M_PI / 180.0;  break;
                              case 11 :  circle->starta = i * M_PI / 180.0;  break;
                              case 12 :  circle->colour = (BYTE)i;  break;
                              case 13 :  circle->layer = (short)i;  break;
                              case 14 :  circle->style = (BYTE)i;  break;
                              case 15 :  circle->weight = (BYTE)i;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }
                           if (rtsymbols.getworkplanecoordinates())
                             {  circle->xaxis = db.workplanes.getcurrent()->workplanetomodel(wpxa + wpo);
                                circle->yaxis = db.workplanes.getcurrent()->workplanetomodel(wpya + wpo);
                                circle->origin = db.workplanes.getcurrent()->workplanetomodel(wpo);
                                circle->xaxis -= circle->origin;  
                                circle->yaxis -= circle->origin;  
                             }
                           else
                             {  circle->origin = wpo;
                                circle->xaxis = wpxa;
                                circle->yaxis = wpya;
                             }
                        }
                      else if (lvalue != 0 && lvalue->gettype() == PointValue)
                        {Point *p;
                           lvalue->getvalue(PointValue,(void **) &p);
                           switch (fid)
                             {case 0 :  p->x = i;  break;
                              case 1 :  p->y = i;  break;
                              case 2 :  p->z = i;  break;
                              default:
                                stype = BadSym;
                                cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                             }
                        }
                      else
                        rtsymbols.setvalue(vname,i);
                      if (stype != BadSym)
                        currentlineno++;
                   }
                 else if (v.gettype() == StringValue)
                   {RCCHAR *s;
                      v.getvalue(&s);
                      rtsymbols.setvalue(vname,s);
                      if (stype != BadSym)
                        currentlineno++;
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Incorrect assignment","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else
              {  stype = BadSym;
                 cadwindow->MessageBox("Bad expression","Run CAD Basic",MB_ICONSTOP);
              }
         }
       else
         {  if (lvalue != 0 && lvalue->gettype() == LineValue)
              {  if (stype != EolSym)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                   }
                 else if (fid == 10)
                   {Transform ident;
                    Line *line;
                      lvalue->getvalue(LineValue,(void **)&line);
                      line->setcopye(0);
                      db.geometry.add(line->clone(ident));
                      currentlineno++;
                   }
                 else if (fid == 11)
                   {Line *line;
                      lvalue->getvalue(LineValue,(void **)&line);
                      line->EntityHeader::change(state.defheader.getcolour(),state.defheader.getlayer(),state.defheader.getstyle(),state.defheader.getweight());
                      currentlineno++;
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else if (lvalue != 0 && lvalue->gettype() == PointEValue)
              {  if (stype != EolSym)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                   }
                 else if (fid == 10)
                   {Transform ident;
                    PointE *point;
                      lvalue->getvalue(PointEValue,(void **)&point);
                      point->setcopye(0);
                      db.geometry.add(point->clone(ident));
                      currentlineno++;
                   }
                 else if (fid == 11)
                   {PointE *point;
                      lvalue->getvalue(PointEValue,(void **)&point);
                      point->EntityHeader::change(state.defheader.getcolour(),state.defheader.getlayer(),state.defheader.getstyle(),state.defheader.getweight());
                      currentlineno++;
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else if (lvalue != 0 && lvalue->gettype() == CircleValue)
              {  if (stype != EolSym)
                   {  stype = BadSym;
                      cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                   }
                 else if (fid == 16)
                   {Transform ident;
                    Circle *circle;
                      lvalue->getvalue(CircleValue,(void **)&circle);
                      circle->setcopye(0);
                      db.geometry.add(circle->clone(ident));
                      currentlineno++;
                   }
                 else if (fid == 17)
                   {Circle *circle;
                      lvalue->getvalue(CircleValue,(void **)&circle);
                      circle->EntityHeader::change(state.defheader.getcolour(),state.defheader.getlayer(),state.defheader.getstyle(),state.defheader.getweight());
                      currentlineno++;
                   }
                 else
                   {  stype = BadSym;
                      cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                   }
              }
            else if (lvalue != 0 && lvalue->gettype() == DialogValue)
              {  if (fid == 0)
                   {int nparameters;
                    long parameter[10];
                    RCCHAR *s,text[300];
                    Dialog *d;
                      if (stype == LBracketSym)
                        {  nextsymbol();
                           nparameters = 0;
                           while (lexpression(&v))
                             {  if (nparameters == 0)
                                  {  if (v.gettype() == StringValue)
                                       {  v.getvalue(&s);  strcpy(text,s);
                                          nparameters++;
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("String value expected.","Run CAD Basic",MB_ICONSTOP);
                                          break;
                                       }
                                  }
                                else
                                  {  if (v.gettype() == IntegerValue)
                                       {  v.getvalue(&parameter[nparameters]);
                                          nparameters++;
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("Integer value expected.","Run CAD Basic",MB_ICONSTOP);
                                          break;
                                       }
                                  }

                                if (stype == RBracketSym)
                                  {  nextsymbol();
                                     break;
                                  }
                                else if (stype == CommaSym)
                                  {  nextsymbol();
                                     if (nparameters == 8)
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("Too many parameters.","Run CAD Basic",MB_ICONSTOP);
                                          break;
                                       }
                                  }
                             }
                           if (stype == EolSym && nparameters == 8)
                             {  lvalue->getvalue(DialogValue,(void **) &d);
                                d->title(text);
                                d->creatememory(parameter[1],parameter[2],parameter[3],parameter[4],parameter[5],parameter[6],parameter[7]);
                                currentlineno++;
                             }
                           else if (stype != BadSym)
                             {  stype = BadSym;
                                cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                             }
                        }
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                        }
                   }
                 else if (fid == 1)
                   {int nparameters;
                    long parameter[10],*i1;
                    double *x1;
                    RCCHAR *s,cl[300],text[300];
                    Dialog *d;
                    Variant *vparameter;
                      if (stype == LBracketSym)
                        {  nextsymbol();
                           nparameters = 0;
                           vparameter = 0;
                           while (lexpression(&v))
                             {  if (nparameters == 0 || nparameters == 1)
                                  {  if (v.gettype() == StringValue)
                                       {  v.getvalue(&s);
                                          if (nparameters == 0)
                                            strcpy(cl,s);
                                          else
                                            strcpy(text,s);
                                          nparameters++;
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("String value expected.","Run CAD Basic",MB_ICONSTOP);
                                          break;
                                       }
                                  }
                                else
                                  {  if (v.gettype() == IntegerValue)
                                       {  v.getvalue(&parameter[nparameters]);
                                          nparameters++;
                                       }
                                     else
                                       {  stype = BadSym;
                                          cadwindow->MessageBox("Integer value expected.","Run CAD Basic",MB_ICONSTOP);
                                          break;
                                       }
                                  }
                                if (stype == RBracketSym)
                                  {  nextsymbol();
                                     break;
                                  }
                                else if (stype == CommaSym)
                                  {  nextsymbol();
                                     if (nparameters == 8)
                                       {  if (stype == VariableNameSym)
                                            {  if ((vparameter = rtsymbols.match(sname)) != 0)
                                                 {  nextsymbol();
                                                    if (stype == RBracketSym)
                                                      nextsymbol();
                                                    else
                                                      {  stype = BadSym;
                                                         cadwindow->MessageBox(") expected.","Run CAD Basic",MB_ICONSTOP);
                                                         break;
                                                      }
                                                 }
                                            }
                                          else
                                            {  stype = BadSym;
                                               cadwindow->MessageBox("Variable name expected.","Run CAD Basic",MB_ICONSTOP);
                                            }
                                       }
                                  }
                             }
                           if (stype == EolSym && nparameters == 8 && vparameter != 0)
                             {  lvalue->getvalue(DialogValue,(void **) &d);
                                d->createcontrol(cl,text,parameter[2],parameter[3],parameter[4],parameter[5],parameter[6],parameter[7]);
                                _strupr(cl);
                                if (strcmp(cl,"STATIC") == 0 || strcmp(cl,"GROUPBOX") == 0)
                                  {/*  Nothing  */
                                  }
                                else if (strcmp(cl,"DEFPUSHBUTTON") == 0 || strcmp(cl,"PUSHBUTTON") == 0)
                                  {  if (parameter[2] > 1)
                                       d->add(new ButtonDialogControl(parameter[2]));
                                  }
                                else if (strcmp(cl,"SCROLLBAR") == 0)
                                  {  if (d->getcontrol(parameter[2] - 1000) != 0)
                                       d->add(new ScrollBarDialogControl(parameter[2],d->getcontrol(parameter[2] - 1000)));
                                  }
                                else if (strcmp(cl,"CHECKBOX") == 0)
                                  {  vparameter->getaddress(&i1);
                                     d->add(new CheckBoxDialogControl(parameter[2],i1));
                                  }
                                else if (strcmp(cl,"EDIT") == 0 && vparameter->gettype() == RealValue)
                                  {  vparameter->getaddress(&x1);
                                     d->add(new RealDialogControl(parameter[2],x1));
                                  }
                                else if (strcmp(cl,"EDIT") == 0 && vparameter->gettype() == IntegerValue)
                                  {  vparameter->getaddress(&i1);
                                     d->add(new IntegerDialogControl(parameter[2],i1));
                                  }
                                else if (strcmp(cl,"EDIT") == 0 && vparameter->gettype() == StringValue)
                                  {RCCHAR *s,oldtext[300],newtext[300];
                                     vparameter->getvalue(&s);
                                     strcpy(oldtext,s);
                                     memset(newtext,' ',300);  newtext[299] = 0;
                                     vparameter->setvalue(newtext);
                                     vparameter->getvalue(&s);
                                     strcpy(s,oldtext);
                                     d->add(new StringDialogControl(parameter[2],s,300));
                                  }
                                currentlineno++;
                             }
                           else if (stype != BadSym)
                             {  stype = BadSym;
                                cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                             }
                        }
                      else
                        {  stype = BadSym;
                           cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
                        }
                   }
              }
            else
              {  stype = BadSym;
                 cadwindow->MessageBox("Syntax error.","Run CAD Basic",MB_ICONSTOP);
              }
         }
    }
  else if (stype == RemSym)
    {  currentlineno++;
       stype = EolSym;
    }
  else
    {  if (stype == EolSym)
         {  currentlineno++;
            return 1;
         }
       else
         {  cadwindow->MessageBox("Unknown statement type","Run",MB_ICONSTOP);
            return 0;
         }
    }
  return stype == EolSym;
}

void CadBasicProgram::add(long lineno,RCCHAR *l)
{RCCHAR **table;
 RCCHAR *bptable;
 long i;
  if (nlines == maxlines-1)
    {  table = new RCCHAR * [maxlines + inclines];
       bptable = new RCCHAR[maxlines + inclines];
       for (i = 0 ; i < nlines ; i++)
         {  table[i] = line[i];
            bptable[i] = breakpoint[i];
         }
       delete [] line;
       delete [] breakpoint;
       line = table;
       breakpoint = bptable;
       line = table;
       maxlines += inclines;
    }
  if (lineno > nlines)
    lineno = nlines;
  if (nlines == 0)
    lineno = 0;
  for (i = nlines-1 ; i >= lineno ; i--)
    {  line[i+1] = line[i];
       breakpoint[i+1] = breakpoint[i];
    }
  line[lineno] = new RCCHAR [300];
  strcpy(line[lineno],l);
  breakpoint[lineno] = 0;
  nlines++;
  changed = 1;
}

void CadBasicProgram::del(RCCHAR *l)
{long i;
  for (i = 0 ; i < nlines && line[i] != l ; i++) ;
  if (i == nlines)
    return;
  delete [] line[i];
  for ( ; i < nlines-1 ; i++)
    {  line[i] = line[i+1];
       breakpoint[i] = breakpoint[i+1];
    }
  line[nlines-1] = 0;
  nlines--;
  changed = 1;
}

class CBEButtonDialogControl : public ButtonDialogControl
  {private:
     CadBasicProgram *cbp;
   public:
     CBEButtonDialogControl(int id,CadBasicProgram *c) : ButtonDialogControl(id) {  cbp = c;  }
     int select(Dialog *);
  };

int CBEButtonDialogControl::select(Dialog *dialog)
{RCCHAR exline[300],result[1200];
 Variant v;
  if (dialog->currentvalue(100,exline,300) != 0)
    {  cbp->evaluate(exline,result);
       ((StringDialogControl *)dialog->getcontrol(101))->change(dialog,result);
    }
  return 0;
}

void CadBasicProgram::evaluate(RCCHAR *exline,RCCHAR *result)
{int simple;
 long lineno,nl;
 Variant *v1,v2;
  lineno = currentlineno;
  nl = nlines;
  currentlineno = maxlines - 1;
  nlines = currentlineno+1;
  line[currentlineno] = exline;
  pos = 0;
  nextsymbol();
  simple = 0;
  if (stype == VariableNameSym)
    {  if ((v1 = rtsymbols.match(sname)) != 0)
         {  nextsymbol();
            if (stype == EolSym)
              {  v1->format(result,0,0);
                 simple = 1;
              }
         }
    }
  if (simple == 0)
    {  pos = 0;
       nextsymbol();
       lexpression(&v2);
       v2.format(result,0,0);
    }
  line[currentlineno] = 0;
  currentlineno = lineno;
  nlines = nl;
}

void CadBasicProgram::evaluate(Window *window)
{Dialog dialog("EvaluateBasic_Dialog");
 RCCHAR name[300],value[300];
  strcpy(name,"");
  strcpy(value,"");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(new StringDialogControl(101,value,300));
  dialog.add(new CBEButtonDialogControl(102,this));
  dialog.process(window->gethwnd());
}

Variant::~Variant()
{ deletedata();
}

void Variant::setname(RCCHAR *n)
{ strcpy(name,n);  _strlwr(name);
}

int Variant::match(RCCHAR *n)
{RCCHAR n1[32];
  strcpy(n1,n);  _strlwr(n1);
  return strcmp(n1,name) == 0;
}

void Variant::deletedata(void)
{ if (type == StringValue)
    delete s;
  else if (type == LineValue)
    delete (Line *) data;
  else if (type == PointValue)
    delete (Point *) data;
  else if (type == DialogValue)
    delete (Dialog *) data;
}

void Variant::setvalue(RCCHAR *s1)
{ deletedata();
  type = StringValue;
  s = new RCCHAR [strlen(s1) + 1];
  strcpy(s,s1);
}

void Variant::setvalue(double x1)
{ deletedata();
  type = RealValue;
  x = x1;
}

void Variant::setvalue(double x1,double y1,double z1)
{ deletedata();
  type = PointValue;
  x = x1;  y = y1;  z = z1;
}

void Variant::setvalue(long i1)
{ deletedata();
  type = IntegerValue;
  i = i1;
}

void Variant::setvalue(int t,void *d)
{ deletedata();
  type = t;
  data = d;
}

int Variant::getvalue(RCCHAR **s1)
{ if (type == StringValue)
    {  *s1 = s;
       return 1;
    }
  else
    return 0;
}

int Variant::getvalue(double *x1)
{ if (type == RealValue)
    {  *x1 = x;
       return 1;
    }
  else if (type == IntegerValue)
    {  *x1 = i;
       return 1;
    }
  else
    return 0;
}

int Variant::getvalue(double *x1,double *y1,double *z1)
{ if (type == PointValue)
    {  *x1 = x;  *y1 = y;  *z1 = z;
       return 1;
    }
  else
    return 0;
}

int Variant::getvalue(long *i1)
{ if (type == IntegerValue)
    {  *i1 = i;
       return 1;
    }
  else
    return 0;
}

int Variant::getvalue(int t,void **d)
{ if (type == t)
    {  *d = data;
       return 1;
    }
  else
    return 0;
}

int Variant::getaddress(RCCHAR **s1)
{ if (type == StringValue)
    {  *s1 = s;
       return 1;
    }
  else
    return 0;
}

int Variant::getaddress(double **x1)
{ if (type == RealValue)
    {  *x1 = & x;
       return 1;
    }
  else
    return 0;
}

int Variant::getaddress(long **i1)
{ if (type == IntegerValue)
    {  *i1 = & i;
       return 1;
    }
  else
    return 0;
}

RCCHAR *Variant::format(RCCHAR *value,int,int)
{PointE *point;
 Line *line;
 Circle *circle;
 Point *p;
  if (type == IntegerValue)
    sprintf(value,"%ld",i);
  else if (type == RealValue)
    sprintf(value,"%.6lf",x);
  else if (type == StringValue)
    strcpy(value,s);
  else if (type == PointEValue)
    {  point = (PointE *)data;
       sprintf(value,"(%.6lf,%.6lf,%.6lf)",point->getp().x,point->getp().y,point->getp().z);
    }
  else if (type == LineValue)
    {  line = (Line *) data;
       sprintf(value,"(%.6lf,%.6lf,%.6lf),(%.6lf,%.6lf,%.6lf)",line->getp1().x,line->getp1().y,line->getp1().z,line->getp2().x,line->getp2().y,line->getp2().z);
    }
  else if (type == CircleValue)
    {  circle = (Circle *) data;
       sprintf(value,"(%.6lf,%.6lf,%.6lf),(%.6lf,%.6lf,%.6lf),(%.6lf,%.6lf,%.6lf),%.6lf,%.6lf,%.6lf",circle->getorigin().x,circle->getorigin().y,circle->getorigin().z,circle->getxaxis().x,circle->getxaxis().y,circle->getxaxis().z,circle->getyaxis().x,circle->getyaxis().y,circle->getyaxis().z,circle->getradius(),circle->getstarta()*180.0/M_PI,circle->getsweepa()*180.0/M_PI);
    }
  else if (type == PointValue)
    {  p = (Point *) data;
       sprintf(value,"(%.6lf,%.6lf,%.6lf)",p->x,p->y,p->z);
    }
  else
    strcpy(value,"Cannot display type");
  return value;  
}

int Variant::read(RCCHAR *buffer,RCCHAR **endptr)
{ if (type == IntegerValue)
    i = strtol(buffer,endptr,10);
  else if (type == RealValue)
    x = strtod(buffer,endptr);
  else if (type == StringValue)
    {  setvalue(buffer);
       *endptr = buffer + strlen(buffer);
    }
  return 1;
}

void RTSymbols::clear(void)
{Variant *v;
  for (list.start() ; (v = (Variant *) list.next()) != 0 ; )
    delete v;
  list.destroy();
}

Variant *RTSymbols::match(RCCHAR *name)
{Variant *v;
  for (list.start() ; (v = (Variant *) list.next()) != 0 ; )
    if (v->match(name))
      return v;
  return 0;
}

void RTSymbols::setvalue(RCCHAR *name,RCCHAR *s)
{Variant *v;
  if ((v = match(name)) == 0)
    {  v = new Variant();
       v->setname(name);
       list.add(v);
    }
  v->setvalue(s);
}

void RTSymbols::setvalue(RCCHAR *name,double x)
{Variant *v;
  if ((v = match(name)) == 0)
    {  v = new Variant();
       v->setname(name);
       list.add(v);
    }
  v->setvalue(x);
}

void RTSymbols::setvalue(RCCHAR *name,double x,double y,double z)
{Variant *v;
  if ((v = match(name)) == 0)
    {  v = new Variant();
       v->setname(name);
       list.add(v);
    }
  v->setvalue(x,y,z);
}

void RTSymbols::setvalue(RCCHAR *name,long i)
{Variant *v;
  if (_stricmp(name,"workplanecoordinates") == 0)
    workplanecoordinates = i;
  if ((v = match(name)) == 0)
    {  v = new Variant();
       v->setname(name);
       list.add(v);
    }
  v->setvalue(i);
}

void RTSymbols::setvalue(RCCHAR *name,int t,void *d)
{Variant *v;
  if ((v = match(name)) == 0)
    {  v = new Variant();
       v->setname(name);
       list.add(v);
    }
  v->setvalue(t,d);
}

int RTSymbols::getvalue(RCCHAR *name,RCCHAR **s)
{Variant *v;
  if ((v = match(name)) == 0)
    return 0;
  return v->getvalue(s);
}

int RTSymbols::getvalue(RCCHAR *name,double *x)
{Variant *v;
  if ((v = match(name)) == 0)
    return 0;
  return v->getvalue(x);
}

int RTSymbols::getvalue(RCCHAR *name,double *x,double *y,double *z)
{Variant *v;
  if ((v = match(name)) == 0)
    return 0;
  return v->getvalue(x,y,z);
}

int RTSymbols::getvalue(RCCHAR *name,long *i)
{Variant *v;
  if ((v = match(name)) == 0)
    return 0;
  return v->getvalue(i);
}

