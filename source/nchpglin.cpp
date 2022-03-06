
#include "ncwin.h"

#if MENUS != STUDENTMENUS

int HPGLIn::open(RCCHAR *s)
{OPENFILENAME ofn;
 RCCHAR szDirName[256]={0},szFilter[256]={0},name[300]={0},title[300]={0};
 UINT  i;
 ResourceString rs0(NCHPGL);
 ResourceString rs1(NCHPGL+1);
 ResourceString rs2(NCHPGL+2);
  if (s == 0)
    {  _getcwd(szDirName, sizeof(szDirName));
       _tcscpy(name,TEXT(""));
       _tcscpy(title,TEXT(""));
       _tcscpy(szFilter,rs1.getws());
       //for (i = 0; szFilter[i] != '\0'; i++)
       //  if (szFilter[i] == '|') szFilter[i] = '\0';
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
       ofn.lpstrDefExt =  rs2.getws();
       ofn.lpstrTitle =  rs0.getws();
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
       if (GetOpenFileName(&ofn))
         infile = fopen(ofn.lpstrFile,TEXT("r"));
       else
         infile = 0;
    }
  else
    infile = fopen(s,"r");

  down = 0;

  return infile != 0;
}

void HPGLIn::close(void)
{ if (infile != 0)
    fclose(infile);
  infile = 0;
}

HPGLIn::~HPGLIn()
{  close();
}

int HPGLIn::nextcommand(void)
{int i,c,labelcommand;
  labelcommand = 0;
  for (i = 0 ; i < 1024 ; i++)
    {  c = getc(infile);
       if (c == '\r' || c == '\n') c = ' ';
       if (i == 0)
         {  while (c == ' ')
              c = getc(infile);
         }
       if (c == EOF) return 0;
       if (c < ' ' || c == ';')
         {  command[i] = 0;  return 1;
         }
       if (i > 1 && ! labelcommand && c >= 'A' && c <= 'Z')
         {  ungetc(c,infile);
            command[i] = 0;  return 1;
         }
       command[i] = char(c);
       if (i == 1)
         {  labelcommand = command[0] == 'L' && command[1] == 'B';
            if (command[0] == 'P' && command[1] == 'E')
              {  command[2] = 0;  return 1;
              }
         }

    }
  return 0;  //  Command too long
}

int gethpglnumber(unsigned char c,int base,int *terminate)
{int n;
  n = -1;
  *terminate = 0;
  if (base == 64)
    {  if (c >= 191 && c <= 254)
         {  *terminate = 1;
            n = c - 191;
         }
       else if (c >= 63 && c <= 126)
         {  *terminate = 0;
            n = c - 63;
         }
    }
  else 
    {  if (c >= 95 && c <= 126)
         {  *terminate = 1;
            n = c - 95;
         }
       else if (c >= 63 && c <= 94)
         {  *terminate = 0;
            n = c - 63;
         }
    }
  return n; 
}

void HPGLIn::processcommand(void)
{double x,y,cx,cy,sweep,rise,run;
 char *s,*endptr;
 int c,i,pen,type,pestate,perelative,inpecommand,pex,pey,terminate;
  
  if (strlen(command) > 0)
    {  if (command[0] == 'P' && command[1] == 'E')
           {  c = getc(infile);
              down = 1;
              pestate = 0;
              perelative = 1;
              pex = pey = 0;
              inpecommand = 64;
              while (c != ';' && c != EOF)
                {  if (c == ':' || c == '<' || c == '>' ||
                       c == '=' || c == '7')
                     {  if (pestate != 0)
                          {  if ((pex % 2) == 0)
                               pex /= 2;
                             else
                               pex = -pex / 2;
                             if ((pey % 2) == 0)
                               pey /= 2;
                             else
                               pey = -pey / 2;
                             if (perelative)
                               {  x = pex / 40.0 + lastx;
                                  y = pey / 40.0 + lasty;
                               }
                      	     else
                      		       {  x = pex / 40.0;
                                  y = pey / 40.0;
                               }
                             if (down)
                               {Point p1(lastx,lasty,0.0),p2(x,y,0.0);
                                  if (rotate == 90.0)
                                    {  p1.setxyz(lasty,-lastx,0.0);
                                       p2.setxyz(y,-x,0.0);
                                    }
                                  Line *line = new Line(h,p1,p2);
                                  if (line != 0 && line->getdefined())
                                    db.geometry.add(line);
                               }
                             lastx = x;  lasty = y;
                             pestate = 0;
                             pex = 0;  pey = 0;
                             perelative = 1;
                             down = 1;
                          }
                     }
                   if (c == ':')
                     pen = gethpglnumber(getc(infile),inpecommand,&terminate);
                   else if (c == '<')
                     down = 0;
                   else if (c == '>')
                     {  down = 1;
                        getc(infile);
                     }
                   else if (c == '=')
                     perelative = 0;
                   else if (c == '7')
                     inpecommand = 32;
                   else if (pestate < 100)
                     {int t;
                        t = gethpglnumber(c,inpecommand,&terminate);
                        if (t >= 0)
                          {  for (int i = 0 ; i < pestate ; i++)
						                         t *= inpecommand;
                             pex += t;
                             pestate++;
                          }
                        if (terminate) pestate = 100;
      			            }
                   else
                     {int t;
                        t = gethpglnumber(c,inpecommand,&terminate);
                        if (t >= 0)
                          {  for (int i = 0 ; i < pestate - 100 ; i++)
                               t *= inpecommand;
                             pey += t;
                             pestate++;
                          }
                        if (terminate)
                          {  if (pestate != 0)
                               {  if ((pex % 2) == 0)
                                    pex /= 2;
                                  else
                                    pex = -pex / 2;
                                  if ((pey % 2) == 0)
                                    pey /= 2;
                                  else
                                    pey = -pey / 2;
                                  if (perelative)
                                    {  x = pex / 40.0 + lastx;
                                       y = pey / 40.0 + lasty;
                                    }
                                  else
		                                  {  x = pex / 40.0;
                                       y = pey / 40.0;
                                    }
                                  if (down)
                                    {Point p1(lastx,lasty,0.0),p2(x,y,0.0);
                                       if (rotate == 90.0)
                                         {  p1.setxyz(lasty,-lastx,0.0);
                                            p2.setxyz(y,-x,0.0);
                                         }
                                       Line *line = new Line(h,p1,p2);
                                       if (line != 0 && line->getdefined())
                                         db.geometry.add(line);
                                    }
                                  lastx = x;  lasty = y;
                                  pestate = 0;
                                  pex = 0;  pey = 0;
                                  perelative = 1;
                                  down = 1;
                 			       }
                          }
                     }
                   c = getc(infile);
                }
              if (c == ';')
                {  if (pestate != 0)
                     {  if ((pex % 2) == 0)
                          pex /= 2;
                        else
                          pex = -pex / 2;
                        if ((pey % 2) == 0)
                          pey /= 2;
                        else
                          pey = -pey / 2;
                        if (perelative)
                          {  x = pex / 40.0 + lastx;
                             y = pey / 40.0 + lasty;
                          }
                        else
		                        {  x = pex / 40.0;
                             y = pey / 40.0;
                          }
                        if (down)
                          {Point p1(lastx,lasty,0.0),p2(x,y,0.0);
                             if (rotate == 90.0)
                               {  p1.setxyz(lasty,-lastx,0.0);
                                  p2.setxyz(y,-x,0.0);
                               }
                             Line *line = new Line(h,p1,p2);
                             if (line != 0 && line->getdefined())
                               db.geometry.add(line);
                          }
                        lastx = x;  lasty = y;
                        pestate = 0;
                        pex = 0;  pey = 0;
                        perelative = 1;
                        down = 1;
                     }
                   inpecommand = 0;
                }
           }
         else if (command[0] == 'P' && command[1] == 'D' ||
                  command[0] == 'P' && command[1] == 'U' ||
                  command[0] == 'P' && command[1] == 'A' ||
                  command[0] == 'P' && command[1] == 'R')
         {  if (command[0] == 'P' && command[1] == 'D')
              down = 1;
            else if (command[0] == 'P' && command[1] == 'U')
              down = 0;
            if (command[0] == 'P' && command[1] == 'R')
              relative = 1;
            else if (command[0] == 'P' && command[1] == 'A')
              relative = 0;
            s = command + 2;  i = 0;
            while (*s == ' ') s++;
            while (strlen(s) > 0)
              {  if (i == 0)
                   x = strtod(s,&endptr) / 40.0;
                 else
                   y = strtod(s,&endptr) / 40.0;
                 if (s == endptr)
                   break;
                 if (i == 1)
                   {  if (relative)
                        {  x += lastx;  y += lasty;
                        }
                      if (down)
                        {Point p1(lastx,lasty,0.0),p2(x,y,0.0);
                           if (rotate == 90.0)
                             {  p1.setxyz(lasty,-lastx,0.0);
                                p2.setxyz(y,-x,0.0);
                             }
                           Line *line = new Line(h,p1,p2);
                           if (line != 0 && line->getdefined())
                             db.geometry.add(line);
                        }
                      lastx = x;  lasty = y;
                   }
                 s = endptr;
                 if (*s == ',' || *s == ' ')
                   {  s++;
                      while (*s == ' ') s++;
                   }
                 else
                   break;
                 i = ! i;
              }
         }
       else if (command[0] == 'S' && command[1] == 'I')
         sscanf(command,"SI%lf,%lf",&textw,&texth);
       else if (command[0] == 'D' && command[1] == 'I')
         {  sscanf(command,"DI%lf,%lf",&run,&rise);
            texta = atan2(rise,run) * 180.0 / M_PI;
         }
       else if (command[0] == 'L' && command[1] == 'T')
         {  if (strlen(command) == 2)
              type = 0;
            else
              sscanf(command,"LT%d",&type);
            h.change(h.getcolour(),h.getlayer(),type,h.getweight());
         }
       else if (command[0] == 'P' && command[1] == 'P')
         {Point p(lastx,lasty,0.0);
          PointE *point;
            if (rotate == 90.0)
              p.setxyz(lasty,-lastx,0.0);
            point = new PointE(p);
            if (point != 0 && point->getdefined())
              db.geometry.add(point);
         }
       else if (command[0] == 'A' && command[1] == 'A')
         {Circle *circle;
            sscanf(command,"AA%lf,%lf,%lf",&cx,&cy,&sweep);
            Point o(cx/40.0,cy/40.0,0.0),p(lastx,lasty,0.0),xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
            if (rotate == 90.0)
              {  o.setxyz(cy/40.0,-cx/40.0,0.0);
                 p.setxyz(lasty,-lastx,0.0);
              }
            Circle c(o,xa,ya,(p-o).length(),0.0,2.0*M_PI);
            if (sweep < 0.0)
              {  circle = new Circle(h,o,xa,ya,(p-o).length(),c.angle(p)+sweep*M_PI/180.0,-sweep*M_PI/180.0);
                 lastx = circle->start().x;
                 lasty = circle->start().y;
              }
            else
              {  circle = new Circle(h,o,xa,ya,(p-o).length(),c.angle(p),sweep*M_PI/180.0);
                 lastx = circle->end().x;
                 lasty = circle->end().y;
              }
            if (rotate == 90.0)
              {double t;  t = lasty;  lasty = lastx;  lastx = -t;
              }
            if (circle != 0 && circle->getdefined())
              db.geometry.add(circle);
         }
       else if (command[0] == 'L' && command[1] == 'B')
         {Point p(lastx,lasty,0.0),xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
          BitMask options(32);
            if (rotate == 90.0)
              p.setxyz(lasty,-lastx,0.0);
            if (strchr(command,'\003') != 0)
              *strchr(command,'\003') = 0;
            Text *text = new Text(h,CHAR2RCCHR("Arial"),p,xa,ya,0.0,texth*25.4*0.6,0.0,rotate == 90.0 ? texta - 90.0 : texta,0,CHAR2RCCHR(command+2),0,options);
            if (text != 0 && text->getdefined())
              db.geometry.add(text);
         }
    }
}

void HPGLIn::read(void)
{ db.geometry.start();
  if (db.geometry.next() == 0)
    h.change(0,0,0,h.getweight());
  else
    h.change(1,1,0,h.getweight());
  if (cadwindow->MessageBox("Rotate the file by 90 degrees?","HPGL In",MB_YESNO) == IDYES)
    rotate = 90.0;
  else
    rotate = 0.0;
  while (nextcommand())
    processcommand();
}

void hpglin_command(int *cstate,HighLevelEvent *,void **)
{HPGLIn hpglin;
 *cstate = ExitState;
  if (hpglin.open(0))
    hpglin.read();
}

#endif
