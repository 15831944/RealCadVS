
#include "ncwin.h"

CommandFile::CommandFile()
{ infile = 0;
  xpos = 0;
  ypos = 1;
  zpos = 2;
  xscale = 1.0;
  yscale = 1.0;
  zscale = 1.0;
  ncolumns = 3;
  lineavailable = 0;
}

CommandFile::~CommandFile()
{ if (infile != 0)
    fclose(infile);
}

void CommandFile::close(void)
{ if (infile != 0)
    {  fclose(infile);
       infile = 0;
    }
}

void CommandFile::send(RCCHAR *line)
{ strcpy(commandline,line);
  lineavailable = 1;
}

int CommandFile::getnextstring(RCCHAR *line)
{ if (lineavailable)
    {  if (strchr(commandline,'/') == 0)
         {  strcpy(line,commandline);
            lineavailable = 0;
         }
       else
         {  strcpy(line,commandline);
            strcpy(commandline,strchr(line,'/')+1);
            *strchr(line,'/') = 0;
         }
       return 1;
    }
  else if (infile != 0)
    return fgets(line,1024,infile) != 0;
  else
    return 0;
}

extern int filesave(void);
extern void open_dwg(RCCHAR *filename);

HighLevelEvent *CommandFile::gne(void)
{RCCHAR line[1024],copy[1024],*s,*endptr;
 int i,l,error,xread,yread,zread;
 Point p,xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
 ResourceString rs1(NCCOMM+1);
 ResourceString rs2(NCCOMM+2);
 ResourceString rs3(NCCOMM+3);
 ResourceString rs4(NCCOMM+4);
 ResourceString rs5(NCCOMM+5);
 ResourceString rs6(NCCOMM+6);
 double v;
  while (getnextstring(line) != 0)
    {  lineno++;
       l = strlen(line);
       while (l > 0 && (line[l-1] == '\n' || line[l-1] == ' '))
         l--;
       line[l] = 0;
       strcpy(copy,line);
       s = strtok(line,_RCT(" \t"));
       if (_stricmp(copy,"select file import dxf") == 0)
         {  if (getnextstring(line) != 0)
              {DXFInConvert dxffile;
                 l = strlen(line);
                 while (l > 0 && (line[l-1] == '\n' || line[l-1] == ' '))
                   l--;
                 line[l] = 0;
                 if (dxffile.open(line))
                   {  dxffile.loadcolourtable();
                      dxffile.read();
                      dxffile.close();
                   }
              }
         }
       else if (_stricmp(copy,"select file import dwg") == 0)
         {  if (getnextstring(line) != 0)
              {  l = strlen(line);
                 while (l > 0 && (line[l-1] == '\n' || line[l-1] == ' '))
                   l--;
                 line[l] = 0;
                 open_dwg(line);
              }
         }
       else if (_stricmp(copy,"select file new") == 0)
         db.initialize();
       else if (_stricmp(copy,"select file saveas") == 0)
         {  if (getnextstring(line) != 0)
              {RCCHAR oldname[300],title[300];
                 l = strlen(line);
                 while (l > 0 && (line[l-1] == '\n' || line[l-1] == ' '))
                   l--;
                 line[l] = 0;
                 if (strrchr(line,'\\') != 0)
                   strcpy(title,strrchr(line,'\\')+1);
                 else
                   strcpy(title,line);
                 strcpy(oldname,db.getname());
                 db.setname(line);
                 if (filesave())
                   {  db.settitle(title);
                      cadwindow->updatetitle();
                   }
                 else
                   db.setname(oldname);
              }
         }
       else if (_stricmp(copy,"select Execute commands from a file") == 0)
         {  if (getnextstring(line) != 0)
              {  l = strlen(line);
                 while (l > 0 && (line[l-1] == '\n' || line[l-1] == ' '))
                   l--;
                 line[l] = 0;

                 //  Open the command file
                 if ((infile = fopen(line,TEXT("r"))) == 0)
                   {ResourceString rs0(NCCOMM);
                      ResourceString rs1(NCCOMM);
                      cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONSTOP);
                      return 0;
                   }
                 xpos = 0;  ypos = 1;  zpos = 2;
                 ncolumns = 3;
                 lineno = 0;
                 xscale = yscale = zscale = 1.0;
                 return 0;

              }
         }
       else if (s != 0 && _stricmp(s,"select") == 0)
         {  s = strtok(0,_RCT(" \t"));
            if (s != 0)
              {int commandid;
                 if ((commandid = commands.command(copy + int(s-line))) >= 0)
                   return new NewCommandEvent(commandid,0);
              }
            sprintf(line,rs2.gets(),lineno,copy);
            cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
            close();
            return 0;
         }
       else if (s != 0 && _stricmp(s,"abort") == 0)
         {  s = strtok(0,_RCT(" \t"));
            if (s == 0)
              return new AbortEvent;
            sprintf(line,rs3.gets(),lineno,copy);
            cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
            close();
            return 0;
         }
       else if (s != 0 && _stricmp(s,"format") == 0)
         {  xpos = ypos = zpos = -1;
            error = 0;
            for (ncolumns = 0 ;  ; ncolumns++)
              {  if ((s = strtok(0,_RCT(" \t"))) == 0)
                   break;
                 if (_stricmp(s,"x") == 0 && xpos == -1)
                   xpos = ncolumns;
                 else if (_stricmp(s,"y") == 0 && ypos == -1)
                   ypos = ncolumns;
                 else if (_stricmp(s,"z") == 0 && zpos == -1)
                   zpos = ncolumns;
                 else if (_stricmp(s,"i") != 0)
                   {  error = 1;  break;
                   }
              }
            if (error || xpos == -1 && ypos == -1 && zpos == -1)
              {  sprintf(line,rs4.gets(),lineno,copy);
                 cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
                 close();
                 return 0;
              }
         }
       else if (s != 0 && _stricmp(s,"scale") == 0)
         {  xread = yread = zread = 0;
            error = 0;
            for (i = 0 ; i < 3 ; i++)
              {  if ((s = strtok(0,_RCT(" \t"))) == 0)
                   break;
                 v = strtod(s,&endptr);
                 if (*endptr != 0)
                   {  error = 1;
                      break;
                   }
                 if (i == 0)
                   {  xread = 1;  xscale = v;
                   }
                 else if (i == 1)
                   {  yread = 1;  yscale = v;
                   }
                 else if (i == 2)
                   {  zread = 1;  zscale = v;
                   }
              }
            if (error || strtok(0," \t") != 0 ||
                ! xread || ! yread || ! zread)
              {  sprintf(line,rs6.gets(),lineno,copy);
                 cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
                 close();
                 return 0;
              }
         }
       else if (s != 0)
         {  p.setxyz(0.0,0.0,0.0);
            xread = xpos == -1;
            yread = ypos == -1;
            zread = zpos == -1;
            error = 0;
            for (i = 0 ; i < ncolumns ; i++)
              {  if (s == 0)
                   break;
                 v = strtod(s,&endptr);
                 if (*endptr != 0)
                   {  error = 1;
                      break;
                   }
                 if (xpos == i)
                   {  xread = 1;  p.x = v * xscale;
                   }
                 else if (ypos == i)
                   {  yread = 1;  p.y = v * yscale;
                   }
                 else if (zpos == i)
                   {  zread = 1;  p.z = v * zscale;
                   }
                 s = strtok(0,_RCT(" \t"));
              }
            if (error || strtok(0," \t") != 0 ||
                ! xread || ! yread || ! zread)
              {  sprintf(line,rs5.gets(),lineno,copy);
                 cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
                 close();
                 return 0;
              }
            else
              {Point xaxis,yaxis;
               View3dWindow *window;
                 if (db.workplanes.getcurrent() != NULL)
                   p = db.workplanes.getcurrent()->workplanetomodel(p);
                 if (cadwindow->getcurrentwindow() == NULL)
                   {  if (db.workplanes.getcurrent() == NULL)
                        {  xaxis.setxyz(1.0,0.0,0.0);  yaxis.setxyz(0.0,1.0,0.0);
                        }
                      else
                        {  xaxis = db.workplanes.getcurrent()->getxaxis();
                           yaxis = db.workplanes.getcurrent()->getyaxis();
                        }
                      window = 0;
                   }
                 else
                   {  xaxis = cadwindow->getcurrentwindow()->getxaxis();
                      yaxis = cadwindow->getcurrentwindow()->getyaxis();
                      window = cadwindow->getcurrentwindow();
                   }
                 return new Coordinate3dEvent(p,xaxis,yaxis,window,0);
              }
         }
    }
  close();
  return 0;
}

int CommandFile::gnd(int *id,RCCHAR *value)
{RCCHAR line[1024],copy[1024],*s,*endptr;
 int l;
 Point p,xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
 ResourceString rs1(NCCOMM+1);
 ResourceString rs7(NCCOMM+7);
 ResourceString rs8(NCCOMM+8);
  while (getnextstring(line) != 0)
    {  lineno++;
       l = strlen(line);
       while (l > 0 && (line[l-1] == '\n' || line[l-1] == ' '))
         l--;
       line[l] = 0;
       strcpy(copy,line);
       s = strtok(line,_RCT(" \t"));
       if (s != 0 && _stricmp(s,"dialog") == 0)
         {  s = strtok(0,_RCT(" \t"));
            if (s == 0)
              {  sprintf(line,rs8.gets(),lineno,copy);
                 cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
                 close();
                 return 0;
              }
            if (_stricmp(s,"OK") == 0)
              *id = IDOK;
            else if (_stricmp(s,"CANCEL") == 0)
              *id = IDCANCEL;
            else
              {  *id = int(strtol(s,&endptr,10));
                 if (*endptr != 0)
                   {  sprintf(line,rs8.gets(),lineno,copy);
                      cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
                      close();
                      return 0;
                   }
              }
            s = strtok(0,_RCT(" \t"));
            if (s == 0)
              strcpy(value,"");
            else
              strcpy(value,copy+int(s - line));
            return 1;
         }
       else if (s != 0 && (_stricmp(s,"select") == 0 ||
                           _stricmp(s,"abort") == 0  ||
                           _stricmp(s,"format") == 0 ||
                           _stricmp(s,"scale") == 0))
         {  sprintf(line,rs7.gets(),lineno,copy);
            cadwindow->MessageBox(line,rs1.gets(),MB_ICONSTOP);
            close();
            return 0;
         }
    }
  close();
  return 0;
}

int CommandFile::select(void)
{OPENFILENAME ofn;
 RCCHAR dirname[300],filter[300],name[300];
 UINT  i;
  infile = 0;
  _getcwd(dirname,sizeof(dirname));
  _tcscpy(name,TEXT(""));
  _tcscpy(filter,TEXT("CAD Command files (*.cmd)|*.CMD|"));
  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = cadwindow->gethwnd();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = name;
  ofn.nMaxFile = sizeof(name);
  ofn.lpstrInitialDir = dirname;
  ofn.lpstrDefExt = _RCT("CMD");
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (! GetOpenFileName(&ofn))
    return 0;
  if ((infile = fopen(ofn.lpstrFile,TEXT("r"))) == 0)
    {ResourceString rs0(NCCOMM);
     ResourceString rs1(NCCOMM);
       cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONSTOP);
       return 0;
    }
  xpos = 0;  ypos = 1;  zpos = 2;
  ncolumns = 3;
  lineno = 0;
  xscale = yscale = zscale = 1.0;
  return 1;
}

int find2_maximum(int n,int *y,double *xmax,double *ymax)
{int i,y4,sum,imax,max,imax1,nfitpoints,*y1,*y2,*y3;
 long b1,b2,b3,r;
 double a,b,c;

 //  Discard invalid values
  y1 = y;  y2 = y + 1;  y3 = y + 2;
  for (i = 1 ; i < n-2 ; i++)
    {  if (*y2 > (((*y1 + *y3) * 9) >> 4))
         *y2 = (*y1 + *y3) >> 1;
       y1 = y2;  y2 = y3;  y3++;
    }

//  Find the maximum value
  imax = 0;
  max = y[0] + y[1] + y[2] + y[3] + y[4] + y[5] + y[6] + y[7];
  for (i = 8,y1 = y ; i < n-7 ; i += 8,y1 += 8)
    {  sum = y1[0] + y1[1] + y1[2] + y1[3] + y1[4] + y1[5] + y1[6] + y1[7];
       if (sum > max) {  imax = i;  max = sum;  }
    }

  imax1 = imax;
  for (i = imax - 15 ; i < imax + 15 ; i++)
    if (y[i] > y[imax1])
      imax1 = i;

  if (y[imax1] > 200)
    {  imax = imax1;
       y4 = (y[imax] * 7) >> 3;
       for (i = imax ; i < n && y[i] > y4 ; i++);
       if (i - imax1 <= 5)
         nfitpoints = 6;
       else if (i - imax1 <= 9)
         nfitpoints = 12;
       else if (i - imax1 <= 20)
         nfitpoints = 20;
       else
         nfitpoints = 30;
    }
  else if (y[imax1] > 40)
    nfitpoints = 60;
  else
    nfitpoints = 100;

  if (imax < nfitpoints / 2)
    imax = nfitpoints / 2;
  else if (imax > n - nfitpoints / 2)
    imax = n - nfitpoints / 2;

  imax1 = imax - nfitpoints / 2;
  b1 = b2 = b3 = 0;
  y1 = y + imax1;
  for (i = 0 ; i < nfitpoints ; i++)
    {  b3 += *y1;
       b2 += (r = long(i) * long(*y1));
       b1 += long(i) * r;
       y1++;
    }

  if (nfitpoints == 100)
    {  a =  1.8009003781530589e-08 * b1 - 1.7828913743715279e-06 * b2 + 2.9120559114734937e-05 * b3;
       b = -1.7828913743715281e-06 * b1 + 1.8850744618279324e-04 * b2 - 3.4769947582993527e-03 * b3;
       c =  2.9120559114734961e-05 * b1 - 3.4769947582993545e-03 * b2 + 8.6493884682585800e-02 * b3;
    }
  else if (nfitpoints == 60)
    {  a =  2.3180335904702312e-07 * b1 - 1.3676398183774368e-05 * b2 + 1.3220518244315229e-04 * b3;
       b = -1.3676398183774365e-05 * b1 + 8.6247848478489383e-04 * b2 - 9.4394500264410666e-03 * b3;
       c =  1.3220518244315218e-04 * b1 - 9.4394500264410614e-03 * b2 + 1.4042834479111610e-01 * b3;
    }
  else if (nfitpoints == 30)
    {  a =  7.4487525822342418e-06 * b1 - 2.1601382488479302e-04 * b2 + 1.0080645161290342e-03 * b3;
       b = -2.1601382488479302e-04 * b1 + 6.7093397425711217e-03 * b2 - 3.5685483870967795e-02 * b3;
       c =  1.0080645161290342e-03 * b1 - 3.5685483870967795e-02 * b2 + 2.6330645161290350e-01 * b3;
    }
  else if (nfitpoints == 20)
    {  a =  5.6960583276372517e-05 * b1 - 1.0822510822510777e-03 * b2 + 3.2467532467532305e-03 * b3;
       b = -1.0822510822510781e-03 * b1 + 2.2066529961266717e-02 * b2 - 7.5974025974025666e-02 * b3;
       c =  3.2467532467532335e-03 * b1 - 7.5974025974025708e-02 * b2 + 3.7077922077921988e-01 * b3;
    }
  else if (nfitpoints == 12)
    {  a =  7.4925074925074979e-04 * b1 - 8.2417582417582489e-03 * b2 + 1.3736263736263753e-02 * b3;
       b = -8.2417582417582489e-03 * b1 + 9.7652347652347746e-02 * b2 - 1.8956043956043975e-01 * b3;
       c =  1.3736263736263748e-02 * b1 - 1.8956043956043969e-01 * b2 + 5.4670329670329698e-01 * b3;
    }
  else if (nfitpoints == 6)
    {  a =  2.6785714285714256e-02 * b1 - 1.3392857142857126e-01 * b2 + 8.9285714285714163e-02 * b3;
       b = -1.3392857142857126e-01 * b1 + 7.2678571428571348e-01 * b2 - 5.8928571428571364e-01 * b3;
       c =  8.9285714285714177e-02 * b1 - 5.8928571428571375e-01 * b2 + 8.2142857142857095e-01 * b3;
    }

  *xmax = imax1 - b / 2.0 / a;
  *ymax = -b * b / 4.0 / a + c;

//  Plot the values for debugging
  Point p1,p2;
  for (i = 0 ; i < nfitpoints ; i++)
    {  p1.setxyz(imax+(i-nfitpoints/2),(a*double(i)+b)*double(i)+c,0.0);
       p2.setxyz(imax+(i-nfitpoints/2+1),(a*double(i+1)+b)*double(i+1)+c,0.0);
       db.header.change(14,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
       db.geometry.add(new Line(p1,p2));
    }

  return 1;

}

double matrices[] =
  { // Matrix for nfitpoints = 6
      1.5432098765431801e-02f, -1.1574074074073847e-01f,  2.1141975308641550e-01f, -4.6296296296295260e-02f,
     -1.1574074074073856e-01f,  8.9484126984125312e-01f, -1.7195767195766880e+00f,  4.3650793650792880e-01f,
      2.1141975308641572e-01f, -1.7195767195766887e+00f,  3.6232363315696072e+00f, -1.2235449735449595e+00f,
     -4.6296296296295426e-02f,  4.3650793650792996e-01f, -1.2235449735449615e+00f,  9.6031746031745735e-01f,
    // Matrix for nfitpoints = 10
      3.2375032375031652e-04f, -4.3706293706292695e-03f,  1.4924889924889559e-02f, -8.1585081585079343e-03f,
     -4.3706293706292747e-03f,  6.0897435897434543e-02f, -2.1853146853146360e-01f,  1.3286713286712984e-01f,
      1.4924889924889590e-02f, -2.1853146853146385e-01f,  8.5356772856771168e-01f, -6.3519813519812485e-01f,
     -8.1585081585079777e-03f,  1.3286713286713034e-01f, -6.3519813519812607e-01f,  8.2377622377621817e-01f,
    // Matrix for nfitpoints = 14
      2.8566205036792761e-05f, -5.5704099821745842e-04f,  2.7909182320946463e-03f, -2.4509803921568033e-03f,
     -5.5704099821745896e-04f,  1.1205706058647031e-02f, -5.8887191240131302e-02f,  5.6722689075629072e-02f,
      2.7909182320946502e-03f, -5.8887191240131344e-02f,  3.3510402995696531e-01f, -3.8410364145657649e-01f,
     -2.4509803921568141e-03f,  5.6722689075629239e-02f, -3.8410364145657705e-01f,  6.9957983193276707e-01f,
    // Matrix for nfitpoints = 18
      4.7777395558611306e-06f, -1.2183235867445875e-04f,  8.0504911516259946e-04f, -9.7465886939566808e-04f,
     -1.2183235867445892e-04f,  3.2034743722048923e-03f, -2.2173489278751561e-02f,  2.9239766081870261e-02f,
      8.0504911516260152e-04f, -2.2173489278751592e-02f,  1.6567528570881979e-01f, -2.5633528265106520e-01f,
     -9.7465886939567458e-04f,  2.9239766081870396e-02f, -2.5633528265106581e-01f,  6.0233918128654218e-01f,
    // Matrix for nfitpoints = 22
      1.1557219795206246e-06f, -3.6405242354899673e-05f,  2.9875413170608151e-04f, -4.6113306982872935e-04f,
     -3.6405242354899673e-05f,  1.1820559303397013e-03f, -1.0151861868109157e-02f,  1.6996047430830271e-02f,
      2.9875413170608157e-04f, -1.0151861868109159e-02f,  9.3920489629873025e-02f, -1.8293807641633922e-01f,
     -4.6113306982872930e-04f,  1.6996047430830274e-02f, -1.8293807641633925e-01f,  5.2687747035573429e-01f,
    // Matrix for nfitpoints = 26
      3.5594738243415149e-07f, -1.3348026841280693e-05f,  1.3088185252103780e-04f, -2.4560369387956573e-04f,
     -1.3348026841280687e-05f,  5.1581352181054145e-04f, -5.2896323511018011e-03f,  1.0736390046735244e-02f,
      1.3088185252103761e-04f, -5.2896323511017959e-03f,  5.8348089894818357e-02f, -1.3701177494281308e-01f,
     -2.4560369387956486e-04f,  1.0736390046735218e-02f, -1.3701177494281294e-01f,  4.6739084670119857e-01f,
    // Matrix for nfitpoints = 30
      1.3004431494111401e-07f, -5.6569276999384623e-06f,  6.4514984642286731e-05f, -1.4255457803844955e-04f,
     -5.6569276999384615e-06f,  2.5352510752955741e-04f, -3.0224156568242665e-03f,  7.2091886608015888e-03f,
      6.4514984642286718e-05f, -3.0224156568242665e-03f,  3.8715223623609595e-02f, -1.0640681003584264e-01f,
     -1.4255457803844955e-04f,  7.2091886608015923e-03f, -1.0640681003584268e-01f,  4.1957478005865212e-01f,
    // Matrix for nfitpoints = 34
      5.3961154530416060e-08f, -2.6710771492555953e-06f,  3.4734795171228833e-05f, -8.8323617735385117e-05f,
     -2.6710771492555966e-06f,  1.3619718316113398e-04f, -1.8506748819842316e-03f,  5.0722991899463830e-03f,
      3.4734795171228853e-05f, -1.8506748819842320e-03f,  2.6997347621162345e-02f, -8.5005173240469148e-02f,
     -8.8323617735385198e-05f,  5.0722991899463865e-03f, -8.5005173240469176e-02f,  3.8042243924597341e-01f,
    // Matrix for nfitpoints = 38
      2.4711192875506490e-08f, -1.3714712045906124e-06f,  2.0028421825598098e-05f, -5.7601790592806207e-05f,
     -1.3714712045906105e-06f,  7.8396259397544446e-05f, -1.1959228904030159e-03f,  3.7029722523946734e-03f,
      2.0028421825598030e-05f, -1.1959228904030138e-03f,  1.9572660939798644e-02f, -6.9459530627696206e-02f,
     -5.7601790592805780e-05f,  3.7029722523946530e-03f, -6.9459530627696012e-02f,  3.4783252690826894e-01f,
    // Matrix for nfitpoints = 42
      1.2242402108161223e-08f, -7.5290772965191574e-07f,  1.2199553700782687e-05f, -3.9151201941899853e-05f,
     -7.5290772965191489e-07f,  4.7685030668581432e-05f, -8.0690196969266867e-04f,  2.7853283667237301e-03f,
      1.2199553700782659e-05f, -8.0690196969266748e-04f,  1.4640722784984484e-02f, -5.7815139210488295e-02f,
     -3.9151201941899568e-05f,  2.7853283667237157e-03f, -5.7815139210488128e-02f,  3.2031276217322813e-01f,
    // Matrix for nfitpoints = 46
      6.4674246633239522e-09f, -4.3655116477436585e-07f,  7.7719042179163478e-06f, -2.7531826791769689e-05f,
     -4.3655116477436701e-07f,  3.0343216292916971e-05f, -5.6402410488848060e-04f,  2.1474824897580527e-03f,
      7.7719042179163952e-06f, -5.6402410488848255e-04f,  1.1236765540757927e-02f, -4.8868992555392107e-02f,
     -2.7531826791770042e-05f,  2.1474824897580722e-03f, -4.8868992555392343e-02f,  2.9678208008457085e-01f,
    // Matrix for nfitpoints = 50
      3.6041550198397104e-09f, -2.6490539395821849e-07f,  5.1398854737933995e-06f, -1.9920885625657946e-05f,
     -2.6490539395821859e-07f,  2.0047700394427530e-05f, -4.0606212531024016e-04f,  1.6904294373772612e-03f,
      5.1398854737934046e-06f, -4.0606212531024032e-04f,  8.8117756958777486e-03f, -4.1848089017900089e-02f,
     -1.9920885625658020e-05f,  1.6904294373772657e-03f, -4.1848089017900152e-02f,  2.7644156065909564e-01f,
    // Matrix for nfitpoints = 54
      2.1013060455914318e-09f, -1.6705383062451848e-07f,  3.5085507043239931e-06f, -1.4767558627207244e-05f,
     -1.6705383062451887e-07f,  1.3673468606583578e-05f, -2.9974230180627830e-04f,  1.3543960912381565e-03f,
      3.5085507043240177e-06f, -2.9974230180627960e-04f,  7.0375247525715683e-03f, -3.6237479219934539e-02f,
     -1.4767558627207500e-05f,  1.3543960912381740e-03f, -3.6237479219934796e-02f,  2.5868965342649225e-01f,
    // Matrix for nfitpoints = 58
      1.2734191264323307e-09f, -1.0887733530996418e-07f,  2.4608824618304733e-06f, -1.1178073091822930e-05f,
     -1.0887733530996422e-07f,  9.5836613870206795e-06f, -2.2606045591357392e-04f,  1.1018386333368330e-03f,
      2.4608824618304775e-06f, -2.2606045591357416e-04f,  5.7095120916664920e-03f, -3.1683449744979934e-02f,
     -1.1178073091822974e-05f,  1.1018386333368367e-03f, -3.1683449744979990e-02f,  2.4306560251410672e-01f,
    // Matrix for nfitpoints = 62
      7.9799351998616377e-10f, -7.3016407078733914e-08f,  1.7671566500093551e-06f, -8.6159360352905657e-06f,
     -7.3016407078733980e-08f,  6.8777351501493193e-06f, -1.7369560152501117e-04f,  9.0836582772063840e-04f,
      1.7671566500093615e-06f, -1.7369560152501155e-04f,  4.6957791314701470e-03f, -2.7936557170427475e-02f,
     -8.6159360352906233e-06f,  9.0836582772064285e-04f, -2.7936557170427544e-02f,  2.2921097719484507e-01f,
    // Matrix for nfitpoints = 66
      5.1492906908919886e-10f, -5.0205584236196884e-08f,  1.2954070591076977e-06f, -6.7476305213448750e-06f,
     -5.0205584236196884e-08f,  5.0389413913861768e-06f, -1.3565548860620429e-04f,  7.5766251282529881e-04f,
      1.2954070591076975e-06f, -1.3565548860620423e-04f,  3.9085693563861174e-03f, -2.4816821110289333e-02f,
     -6.7476305213448572e-06f,  7.5766251282529675e-04f, -2.4816821110289284e-02f,  2.1684301117060323e-01f,
    // Matrix for nfitpoints = 70
      3.4096788831068768e-10f, -3.5290176440156254e-08f,  9.6701902803794687e-07f, -5.3594014620451167e-06f,
     -3.5290176440156148e-08f,  3.7597408720957929e-06f, -1.0748379452916143e-04f,  6.3853440276365452e-04f,
      9.6701902803794010e-07f, -1.0748379452916101e-04f,  3.2879706632776961e-03f, -2.2191750196767706e-02f,
     -5.3594014620450388e-06f,  6.3853440276364791e-04f, -2.2191750196767585e-02f,  2.0573578457044234e-01f,
    // Matrix for nfitpoints = 74
      2.3101864140888877e-10f, -2.5296541234273310e-08f,  7.3364589952220715e-07f, -4.3105306263201568e-06f,
     -2.5296541234273313e-08f,  2.8511627666760794e-06f, -8.6261205608871813e-05f,  5.4312685891633877e-04f,
      7.3364589952220778e-07f, -8.6261205608871867e-05f,  2.7921274544851983e-03f, -1.9962067330488557e-02f,
     -4.3105306263201695e-06f,  5.4312685891634007e-04f, -1.9962067330488588e-02f,  1.9570671149618615e-01f,
    // Matrix for nfitpoints = 78
      1.5977018962682132e-10f, -1.8453456901897829e-08f,  5.6470773523599732e-07f, -3.5061568113605596e-06f,
     -1.8453456901897868e-08f,  2.1937702372966775e-06f, -7.0028232734573490e-05f,  4.6581797636647784e-04f,
      5.6470773523600039e-07f, -7.0028232734573693e-05f,  2.3911963319641193e-03f, -1.8052199662606884e-02f,
     -3.5061568113606011e-06f,  4.6581797636648190e-04f, -1.8052199662606967e-02f,  1.8660668133241581e-01f,
    // Matrix for nfitpoints = 82
      1.1255525536708813e-10f, -1.3675463527101210e-08f,  4.4037243662373263e-07f, -2.8809643163759922e-06f,
     -1.3675463527101204e-08f,  1.7101565103864397e-06f, -5.7440854089118561e-05f,  4.0251187163081693e-04f,
      4.4037243662373231e-07f, -5.7440854089118540e-05f,  2.0635082904224443e-03f, -1.6403799251113963e-02f,
     -2.8809643163759879e-06f,  4.0251187163081655e-04f, -1.6403799251113956e-02f,  1.7831275913245151e-01f,
    // Matrix for nfitpoints = 86
      8.0629013187260859e-11f, -1.0280199181375753e-08f,  3.4748685813313736e-07f, -2.3891182897517128e-06f,
     -1.0280199181375761e-08f,  1.3490143862234702e-06f, -4.7559138612810252e-05f,  3.5017648075503680e-04f,
      3.4748685813313805e-07f, -4.7559138612810313e-05f,  1.7930708841341024e-03f, -1.4971239111422703e-02f,
     -2.3891182897517238e-06f,  3.5017648075503805e-04f, -1.4971239111422735e-02f,  1.7072270691743913e-01f,
    // Matrix for nfitpoints = 90
      5.8642357645085474e-11f, -7.8287547456189096e-09f,  2.7714964646643823e-07f, -1.9978982110819402e-06f,
     -7.8287547456189129e-09f,  1.0756407431212494e-06f, -3.9714154430989616e-05f,  3.0653466838600059e-04f,
      2.7714964646643844e-07f, -3.9714154430989636e-05f,  1.5679061018433757e-03f, -1.3718425359379069e-02f,
     -1.9978982110819483e-06f,  3.0653466838600156e-04f, -1.3718425359379095e-02f,  1.6375081985180159e-01f,
    // Matrix for nfitpoints = 94
      4.3246546977970296e-11f, -6.0328933034268638e-09f,  2.2322570153619003e-07f, -1.6835794245430017e-06f,
     -6.0328933034268588e-09f,  8.6612886891069622e-07f, -3.3422228900984823e-05f,  2.6985373061960547e-04f,
      2.2322570153618924e-07f, -3.3422228900984748e-05f,  1.3789220535462420e-03f, -1.2616503696178732e-02f,
     -1.6835794245429885e-06f,  2.6985373061960385e-04f, -1.2616503696178690e-02f,  1.5732472495122704e-01f,
    // Matrix for nfitpoints = 98
      3.2300467987010065e-11f, -4.6997180921099718e-09f,  1.8141557844904281e-07f, -1.4287143000014446e-06f,
     -4.6997180921099677e-09f,  7.0373260320927445e-07f, -2.8328557882641244e-05f,  2.3879939014309837e-04f,
      1.8141557844904225e-07f, -2.8328557882641187e-05f,  1.2191330618513388e-03f, -1.1642184626626027e-02f,
     -1.4287143000014321e-06f,  2.3879939014309672e-04f, -1.1642184626625980e-02f,  1.5138289339138081e-01f
  };

int find3_maximum(int n,int *y,double *xmax,double *ymax)
{int i,half,left,right,sum,imax,max,imax1,nfitpoints,*y1,*y2,*y3;
 long b2,b3,b4,r;
 double a,b,c,d,disc,x1,x2,y5,y6,x,*m,b1;

 //  Discard invalid values
  y1 = y;  y2 = y + 1;  y3 = y + 2;
  for (i = 1 ; i < n-2 ; i++)
    {  if (*y2 > (((*y1 + *y3) * 9) >> 4))
         *y2 = (*y1 + *y3) >> 1;
       y1 = y2;  y2 = y3;  y3++;
    }

//  Find the maximum value
  imax = 0;
  max = y[0] + y[1] + y[2] + y[3] + y[4] + y[5] + y[6] + y[7];
  for (i = 8,y1 = y ; i < n-7 ; i += 8,y1 += 8)
    {  sum = y1[0] + y1[1] + y1[2] + y1[3] + y1[4] + y1[5] + y1[6] + y1[7];
       if (sum > max) {  imax = i;  max = sum;  }
    }

  imax1 = imax;
  for (i = imax - 25 ; i < imax + 25 ; i++)
    if (i >= 0 && i <= n-1 && y[i] > y[imax1])
      imax1 = i;

  half = y[imax1] / 2;
  left = imax1;
  while (left > 0 && y[left] >= half)
    left--;
  right = imax1;
  while (right < n - 1 && y[right] >= half)
    right++;
  nfitpoints = ((right - left) / 4) * 4 + 2;
  imax = (left + right) / 2;
  if (nfitpoints < 6)
    nfitpoints = 6;
  else if (nfitpoints > 98)
    nfitpoints = 98;

  if (imax < nfitpoints / 2)
    imax = nfitpoints / 2;
  else if (imax > n - nfitpoints / 2)
    imax = n - nfitpoints / 2;

  imax1 = imax - nfitpoints / 2;

  b1 = b2 = b3 = b4 = 0;
  y1 = y + imax1;
  for (i = 0 ; i < nfitpoints ; i++)
    {  b4 += *y1;
       b3 += (r = ((long)i) * ((long)*y1));
       b2 += (r = ((long)i) * r);
       b1 += (((long)i) * r);
       y1++;
    }

  m = matrices + (nfitpoints - 6) * 4;

  a =  m[ 0] * b1 + m[ 1] * b2 + m[ 2] * b3 + m[ 3] * b4;
  b =  m[ 4] * b1 + m[ 5] * b2 + m[ 6] * b3 + m[ 7] * b4;
  c =  m[ 8] * b1 + m[ 9] * b2 + m[10] * b3 + m[11] * b4;
  d =  m[12] * b1 + m[13] * b2 + m[14] * b3 + m[15] * b4;

  disc = b * b - 3.0 * a * c;
  if (fabs(a) > 1.0E-10 && disc >= 0.0)
    {  disc = sqrt(disc);
       x1 = (-b - disc) / 3.0 / a;
       x2 = (-b + disc) / 3.0 / a;
       y5 = ((a * x1 + b) * x1 + c) * x1 + d;
       y6 = ((a * x2 + b) * x2 + c) * x2 + d;
       if (y5 > y6)
         {  *xmax = x1 + imax1;
            *ymax = y5;
         }
       else
         {  *xmax = x2 + imax1;
            *ymax = y6;
         }
    }


  sum = y[(int)*xmax-3] + y[(int)*xmax-2] + y[(int)*xmax-1] + y[(int)*xmax];
  if ((sum>>2)-1 > *ymax)  /*  Average of four channels to left of peak is greater than the peak power  */
    *ymax = -1;
  sum = y[(int)*xmax+3] + y[(int)*xmax+2] + y[(int)*xmax+1] + y[(int)*xmax];
  if ((sum>>2)-1 > *ymax)  /*  Average of four channels to right of peak is greater than the peak power  */
    *ymax = -1;


    //  Plot the values for debugging
  Point p1,p2;
  for (x = 0.0 ; x < nfitpoints ; x += 0.25)
    {  p1.setxyz(imax+(x-nfitpoints/2),((a*x+b)*x+c)*x+d,0.0);
       p2.setxyz(imax+(x-nfitpoints/2+0.25),((a*(x+0.25)+b)*(x+0.25)+c)*(x+0.25)+d,0.0);
       db.header.change(14,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
       db.geometry.add(new Line(p1,p2));
    }

  char message[200];
  sprintf(message,"Degree 3 fit, number of fit points %d.",nfitpoints);
  Point org(200.0,100.0,0.0),xa(1.0,0.0,0.0),ya(0.0,1.0,0.0);
  BitMask o(32);

  db.geometry.add(new Text(CHAR2RCCHR("Arial"),org,xa,ya,0.0,40.0,0.0,0.0,0,CHAR2RCCHR(message),0,o));

  return 1;

}

int GetDataRange(unsigned short *TheStart,unsigned short *TheCount,unsigned short *Offset,int Data[])
{int *y,*y1,*y2,*y3,max,sum;
 int i,n,imax,imax1;
 unsigned short Index,Peak,Start,End,RefPower,MaxIndex;

  y = Data + *TheStart;
  n = *TheCount;

//  Discard invalid values
  y1 = y;  y2 = y + 1;  y3 = y + 2;
  for (i = 1 ; i < n-2 ; i++)
    {  if (*y2 > (((*y1 + *y3) * 9) >> 4))
	 *y2 = (*y1 + *y3) >> 1;
       y1 = y2;  y2 = y3;  y3++;
    }

//  Find the maximum value
  imax = 0;
  max = y[0] + y[1] + y[2] + y[3] + y[4] + y[5] + y[6] + y[7];
  for (i = 8,y1 = y ; i < n-7 ; i += 8,y1 += 8)
    {  sum = y1[0] + y1[1] + y1[2] + y1[3] + y1[4] + y1[5] + y1[6] + y1[7];
       if (sum > max) {  imax = i;  max = sum;  }
    }

//  Find the single channel maximum near imax.

  if (y[imax] > 200)  // For tall sweeps, use the actual maximum
    {  imax1 = imax;
       for (i = imax - 15 ; i < imax + 15 ; i++)
         if (y[i] > y[imax1])
           imax1 = i;
       imax = imax1;
    }

  MaxIndex = (short)imax;
  Peak = (short)Data[MaxIndex];
  Start = 0;
  End = *TheCount;

  RefPower = (unsigned short)(Peak / 2);          /* half of the actual data */

  for (Index = (unsigned short)(MaxIndex + 1); Index > Start; Index--)
    if (Data[Index - 1] < RefPower)
      {  Start = Index;
         break;
      }

  for (Index = MaxIndex; Index < End; Index++)
    if (Data[Index] < RefPower)
      {  End = (unsigned short) (Index - 1);
         break;
      }

/* now make sure we don't have too many points for the regression analysis */

   while (End - Start > 100 - 1)
     if (Data[Start] < Data[End])
       Start++;
     else
       End--;

   *TheCount       = (unsigned short) (End - Start + 1);
   *Offset         = (unsigned short) (Start - *TheStart);
   *TheStart       += Start;

   return 1;
}

void calculate2_matrix(int n)
{Matrix A(3);
 Vector X(3),B(3);
 int i,j,col;

  FILE *dfile = fopen("debug.log","a");
  fprintf(dfile,"Matrix for n = %d\n",n);
  fclose(dfile);

  for (col = 0 ; col < 3 ; col++)
    {  for (i = 0 ; i < 3 ; i++)
         for (j = 0 ; j < 3 ; j++)
           A(i,j) = 0;
       for (i = 0 ; i < n ; i++)
         {  A(0,0) += double(i) * double(i) * double(i) * double(i);
            A(1,0) += double(i) * double(i) * double(i);
            A(2,0) += double(i) * double(i);
            A(2,1) += double(i);
         }
       A(0,1) = A(1,0);
       A(1,1) = A(2,0);
       A(0,2) = A(2,0);
       A(1,2) = A(2,1);
       A(2,2) = n;
       B(0) = col == 0;  B(1) = col == 1;  B(2) = col == 2;
       A.solve(&X,&B);
       FILE *dfile = fopen("debug.log","a");
       fprintf(dfile,"        %c = %24.16e * b1 + %24.16e * b2 + %24.16e * b3;\n",'a'+col,X(0),X(1),X(2));
       fclose(dfile);
    }
}

void calculate3_matrix(int n)
{Matrix A(4);
 Vector X(4),B(4);
 int i,j,col;

  FILE *dfile = fopen("debug.log","a");
  fprintf(dfile,"    // Matrix for nfitpoints = %d\n",n);
  fclose(dfile);

  for (col = 0 ; col < 4 ; col++)
    {  for (i = 0 ; i < 4 ; i++)
         for (j = 0 ; j < 4 ; j++)
           A(i,j) = 0;
       for (i = 0 ; i < n ; i++)
         {  A(0,0) += double(i) * double(i) * double(i) * double(i) * double(i) * double(i);
            A(1,0) += double(i) * double(i) * double(i) * double(i) * double(i);
            A(2,0) += double(i) * double(i) * double(i) * double(i);
            A(3,0) += double(i) * double(i) * double(i);
            A(3,1) += double(i) * double(i);
            A(3,2) += double(i);
         }
       A(0,1) = A(1,0);
       A(0,2) = A(2,0);
       A(0,3) = A(3,0);
       A(1,1) = A(2,0);
       A(1,2) = A(3,0);
       A(1,3) = A(3,1);
       A(2,1) = A(3,0);
       A(2,2) = A(3,1);
       A(2,3) = A(3,2);
       A(3,3) = n;
       B(0) = col == 0;  B(1) = col == 1;  B(2) = col == 2;  B(3) = col == 3;

       A.solve(&X,&B);
       FILE *dfile = fopen("debug.log","a");
       fprintf(dfile,"    %24.16ef,%24.16ef,%24.16ef,%24.16ef,\n",X(0),X(1),X(2),X(3));
       fclose(dfile);

       A.solve(&X,&B);
    }
}

void test_data(void)
{FILE *infile;
 OPENFILENAME ofn;
 RCCHAR dirname[300],filter[300],name[300];
 int x[600],y1[600],y2[600];
 int i,n;
 double xmax1,ymax1,xmax2,ymax2;
 Point p1,p2;

  for (i = 6 ; i <= 98 ; i += 4)
    calculate3_matrix(i);

  _getcwd(dirname,sizeof(dirname));
  _tcscpy(name,TEXT(""));
  _tcscpy(filter,TEXT("Sweep files (*.dat)|*.DAT|"));
  for (i = 0; filter[i] != '\0'; i++)
    if (filter[i] == '|') filter[i] = '\0';
  memset(&ofn, 0, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = cadwindow->gethwnd();
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = name;
  ofn.nMaxFile = sizeof(name);
  ofn.lpstrInitialDir = dirname;
  ofn.lpstrDefExt = _RCT("DAT");
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (! GetOpenFileName(&ofn))
    return;

  if ((infile = fopen(ofn.lpstrFile,TEXT("r"))) == 0)
    return;

   n = 0;
   while (fscanf(infile,"%d, %d %d %d\n",&x[n],&y1[n],&y2[n],&i) == 4)
     n++;
   fclose(infile);

   unsigned short TheStart = 0,TheCount = (unsigned short)n,Offset = 0;
   GetDataRange(&TheStart,&TheCount,&Offset,y1);
   TheStart = 0;  TheCount = (unsigned short)n;  Offset = 0;
   GetDataRange(&TheStart,&TheCount,&Offset,y2);

   find3_maximum(n,y1,&xmax1,&ymax1);
   find3_maximum(n,y2,&xmax2,&ymax2);


//  Plot the raw values

   for (i = 0 ; i < n-1 ; i++)
     {  p1.setxyz(i,y1[i],0.0);
        p2.setxyz((i+1),y1[i+1],0.0);
        db.header.change(0,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
        db.geometry.add(new Line(p1,p2));
        p1.setxyz(i,y2[i],0.0);
        p2.setxyz((i+1),y2[i+1],0.0);
        db.header.change(1,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
        db.geometry.add(new Line(p1,p2));
     }


  db.header.change(2,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
  p1.setxyz(xmax1-5.0,ymax1-5.0,0.0);
  p2.setxyz(xmax1+5.0,ymax1+5.0,0.0);
  db.geometry.add(new Line(p1,p2));
  p1.setxyz(xmax1+5.0,ymax1-5.0,0.0);
  p2.setxyz(xmax1-5.0,ymax1+5.0,0.0);
  db.geometry.add(new Line(p1,p2));

  db.header.change(2,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
  p1.setxyz(xmax2-5.0,ymax2-5.0,0.0);
  p2.setxyz(xmax2+5.0,ymax2+5.0,0.0);
  db.geometry.add(new Line(p1,p2));
  p1.setxyz(xmax2+5.0,ymax2-5.0,0.0);
  p2.setxyz(xmax2-5.0,ymax2+5.0,0.0);
  db.geometry.add(new Line(p1,p2));

}

void command_command(int *cstate,HighLevelEvent *,void **)
{ //test_data();
  commandfile.select();
  *cstate = ExitState;
}

