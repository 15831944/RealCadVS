
#include "ncwin.h"

Point Wall::nearp(Point p)
{double t,len;
 Point  dir;
 len = (p2-p1).length();
  if (len < db.getptoler())
    return (p1 + p2) / 2.0;
 dir = (p2 - p1) / len;
 t = dir.dot(p-p1);
 return p1 + dir * t;
}

int Wall::stretchent(Database *db,Transform &t,SortedPointTable *sp,int erase)
{int match1,match2;
 Entity *e;
 Point p3,p4,xaxis;
 double dx;
 SortedDoubleList sd1,sd4;
 EntityList ol;

  if (! isvisible(0)) return 1;
  if (status & 8) return 1;
  status |= 8;
  if (copye != NULL) return 1;
  if ((status & 1) == 1)
    {  if ((status & 16) == 0) draw(DM_ERASE);
       status |= 16;
       return move(db,t,erase);
    }

  //  Stretch any figures that are on the wall
  ol = openinglist;
  for (ol.start() ; (e = ol.next()) != NULL ; )
    {  e->stretchent(db,t,sp,erase);
       cadwindow->invalidatedisplaylist(this);
    } 

  //  Stretch the wall itself
  match1 = sp->match(p1);
  match2 = sp->match(p2);
  if (! match1 && ! match2) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 16) == 0)
      {  e->draw(DM_ERASE);  e->setstatus(e->getstatus() | 16);
      }
  p3 = match1 ? t.transform(p1) : p1;
  p4 = match2 ? t.transform(p2) : p2;
  if ((p3 - nearp(p3)).length() > ::db.getptoler() ||
      (p4 - nearp(p4)).length() > ::db.getptoler())
    return 1;
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  if (erase && (status & 16) == 0) draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  status |= 2 | 16;
  xaxis = (p2 - p1).normalize();
  dx = xaxis.dot(p1 - p3);
  for (break2dlist1.start() ; ! break2dlist1.atend() ; )
    sd1.add(break2dlist1.next()+dx);
  for (break2dlist4.start() ; ! break2dlist4.atend() ; )
    sd4.add(break2dlist4.next()+dx);
  break2dlist1.destroy();
  break2dlist4.destroy();
  break2dlist1 = sd1;
  break2dlist4 = sd4;
  p1 = p3;
  p2 = p4;
  copye = this;
  return 1;
}

void Wall::breakwall(Wall *wall)
{Point xaxis,yaxis,p3,p4;
 int on1,on2,on3;
 double totalwidth;

  db.saveundo(UD_MOVED,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);

  totalwidth = gettotalwidth();

  xaxis = (p2 - p1).normalize();
  yaxis = xaxis.cross(zaxis).normalize() * totalwidth / 2.0;
  Line line1(p1+yaxis-xaxis*(totalwidth/2.0*tan(angle1)),p2+yaxis-xaxis*(totalwidth/2.0*tan(-angle2)));
  Line line2(p1-yaxis+xaxis*(totalwidth/2.0*tan(angle1)),p2-yaxis+xaxis*(totalwidth/2.0*tan(-angle2)));
  on1 = (line1.nearp(wall->p1) - wall->p1).length() < db.getptoler();
  on2 = (line1.nearp(wall->p2) - wall->p2).length() < db.getptoler();
  on3 = (line2.nearp(wall->p1) - wall->p1).length() < db.getptoler();

  xaxis = (wall->p2 - wall->p1).normalize();
  yaxis = xaxis.cross(wall->zaxis).normalize() * wall->gettotalwidth() / 2.0;
  if (on1 || on3)
    {  p3 = wall->p1+yaxis+xaxis*(wall->gettotalwidth()/2.0*tan(wall->angle1));
       p4 = wall->p1-yaxis-xaxis*(wall->gettotalwidth()/2.0*tan(wall->angle1));
    }
  else
    {  p3 = wall->p2+yaxis+xaxis*(wall->gettotalwidth()/2.0*tan(-wall->angle2));
       p4 = wall->p2-yaxis-xaxis*(wall->gettotalwidth()/2.0*tan(-wall->angle2));
    }
  xaxis = (p2 - p1).normalize();
  yaxis = xaxis.cross(zaxis).normalize() * totalwidth / 2.0;
  if (on1 || on2)
    {  break2dlist1.add((p1+yaxis+xaxis*(totalwidth/2.0*tan(angle1))-p3).length());
       break2dlist1.add((p1+yaxis+xaxis*(totalwidth/2.0*tan(angle1))-p4).length());
    }
  else
    {  break2dlist4.add((p1-yaxis-xaxis*(totalwidth/2.0*tan(angle1))-p3).length());
       break2dlist4.add((p1-yaxis-xaxis*(totalwidth/2.0*tan(angle1))-p4).length());
    }
  draw(DM_NORMAL);
}

int Wall::unbreakwall(Wall *wall)
{Point xaxis,yaxis,p3,p4;
 int on1,on2,on3,on4,unbroken;
 double t,t1,t2,t3,t4;
 SortedDoubleList newbreak;

  unbroken = 0;
  xaxis = (p2 - p1).normalize();
  yaxis = xaxis.cross(zaxis).normalize() * width / 2.0;
  Line line1(p1+yaxis-xaxis*(width/2.0*tan(angle1)),p2+yaxis-xaxis*(width/2.0*tan(-angle2)));
  Line line2(p1-yaxis+xaxis*(width/2.0*tan(angle1)),p2-yaxis+xaxis*(width/2.0*tan(-angle2)));
  on1 = (line1.nearp(wall->p1) - wall->p1).length() < db.getptoler();
  on2 = (line1.nearp(wall->p2) - wall->p2).length() < db.getptoler();
  on3 = (line2.nearp(wall->p1) - wall->p1).length() < db.getptoler();
  on4 = (line2.nearp(wall->p2) - wall->p2).length() < db.getptoler();
  if (! on1 && ! on2 && ! on3 && !on4)
    return 0;
  db.saveundo(UD_MOVED,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  xaxis = (wall->p2 - wall->p1).normalize();
  yaxis = xaxis.cross(wall->zaxis).normalize() * wall->width / 2.0;
  if (on1 || on3)
    {  p3 = wall->p1+yaxis+xaxis*(wall->width/2.0*tan(wall->angle1));
       p4 = wall->p1-yaxis-xaxis*(wall->width/2.0*tan(wall->angle1));
    }
  else
    {  p3 = wall->p2+yaxis+xaxis*(wall->width/2.0*tan(-wall->angle2));
       p4 = wall->p2-yaxis-xaxis*(wall->width/2.0*tan(-wall->angle2));
    }
  xaxis = (p2 - p1).normalize();
  yaxis = xaxis.cross(zaxis).normalize() * width / 2.0;
  if (on1 || on2)
    {  t1 = (p1+yaxis+xaxis*(width/2.0*tan(angle1))-p3).length();
       t2 = (p1+yaxis+xaxis*(width/2.0*tan(angle1))-p4).length();
       if (t1 > t2)
         {  t = t1;  t1 = t2;  t2 = t;
         }
       for (break2dlist1.start() ; ! break2dlist1.atend() ; )
         {  t3 = break2dlist1.next();
            if (break2dlist1.atend())
              break;
            t4 = break2dlist1.next();
            if (fabs(t3-t1) > 1.0E-5 || fabs(t4 - t2) > 1.0E-5)
              {  newbreak.add(t3);  newbreak.add(t4);
              }
            else
              unbroken = 1;  
         }
       break2dlist1.destroy();
       break2dlist1 = newbreak;
    }
  else
    {  t1 = (p1-yaxis-xaxis*(width/2.0*tan(angle1))-p3).length();
       t2 = (p1-yaxis-xaxis*(width/2.0*tan(angle1))-p4).length();
       if (t1 > t2)
         {  t = t1;  t1 = t2;  t2 = t;
         }
       for (break2dlist4.start() ; ! break2dlist4.atend() ; )
         {  t3 = break2dlist4.next();
            if (break2dlist4.atend())
              break;
            t4 = break2dlist4.next();
            if (fabs(t3-t1) > 1.0E-5 || fabs(t4 - t2) > 1.0E-5)
              {  newbreak.add(t3);  newbreak.add(t4);
              }
            else
              unbroken = 1;
         }
       break2dlist4.destroy();
       break2dlist4 = newbreak;
    }
  draw(DM_NORMAL);

  return unbroken;

}

int Wall::move(Database *db,Transform &t,int erase)
{Entity *e;
 EntityList newlist;
  if (copye != NULL) return 1;
  for (alist.start() ; (e = alist.next()) != NULL ; )
    if ((e->getstatus() & 1) == 0)
      return clone(t) != NULL;
  for (openinglist.start() ; (e = openinglist.next()) != 0 ; )
    {  if (e->getcopye() == 0) e->move(db,t,erase);
       if (e->getcopye() == 0)
         {  newlist.destroy();  draw(DM_NORMAL);  return 0;
         }
       newlist.add(e->getcopye());
    }
  newlist.reverse();
  status |= 1;
  if (erase) draw(DM_ERASE);
  if (db != 0)
    {  db->saveundo(UD_MOVED,this);
       status |= 32;  // Moved
    } 
  cadwindow->invalidatedisplaylist(this);
  dbdelete();
  openinglist.destroy();
  openinglist = newlist;
  dbinsert(db);
  p1 = t.transform(p1);
  p2 = t.transform(p2);
  copye = this;
  return 1;
}

Entity *Wall::clone(Transform &t)
{Point p1new,p2new,zaxisnew;
 Entity *e;
  if (copye != NULL) return copye;
  p1new = t.transform(p1);
  p2new = t.transform(p2);
  zaxisnew = (t.transform(p1+zaxis) - p1new).normalize();
  copye = new Wall(*this,p1new,p2new,zaxisnew,name,_RCT(""),width,height,eaves,pitch,peakdistance,peakheight,middlecolour,rightcolour,lhatchspacing,rhatchspacing,lhatchangle,rhatchangle,angle1,angle2,nattributes,attributes,options,options);
  if (copye == NULL || ! copye->getdefined())
    {  delete copye;
       copye = NULL;
    }
  for (openinglist.start() ; (e = openinglist.next()) != 0 ; )
    {  if (e->getcopye() == 0)
         {  if (e->clone(t) == 0) return 0;
         }
       ((Wall *)copye)->openinglist.add(e->getcopye());
    }
  copye->cloneattributes(this);
  return copye;
}

int Wall::project(Database *db,Transform &t,int)
{  return db->geometry.add(clone(t));
}

void Wall::change(const BitMask &change,const EntityHeader &eh,RCCHAR *n,RCCHAR *,double w,double h,double e,double p,double pd,double ph,int mc,int rc,double lhs,double rhs,double lha,double rha,short nattributes_,double *attributes_,const BitMask &op,const BitMask &)
{Transform t1,t2;
 Point org,dir;
  db.saveundo(UD_CHANGEATTRIBUTES,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);
  if (change.test(0)) colour = eh.getcolour();
  if (change.test(1)) layer = eh.getlayer();
  if (change.test(2)) style = eh.getstyle();
  if (change.test(3)) weight = eh.getweight();
  if (change.test(4))
    {RCCHAR *s;
       if ((s = new RCCHAR[strlen(n) + 1]) != 0)
         {  delete [] name;
            name = s;
            strcpy(name,n);
         }
    }
  if (change.test(5))
    {  width = w;
       if (options.test(1))
         {  //  Change column length too.
            org = (p1 + p2) / 2.0;
            dir = (p2 - p1).normalize();
            p1 = org - dir * w / 2.0;
            p2 = org + dir * w / 2.0;
         }
    }
  if (change.test(6)) height = h;
  if (change.test(7)) eaves = e;
  if (change.test(8)) options.set(0,op.test(0));
  if (change.test(9)) pitch = p;
  if (change.test(10)) peakdistance = pd;
  if (change.test(11)) peakheight = ph;
  if (change.test(12)) middlecolour = short(mc);
  if (change.test(13)) rightcolour = short(rc);
  if (change.test(14)) lhatchspacing = lhs;
  if (change.test(15)) rhatchspacing = rhs;
  if (change.test(16)) lhatchangle = lha;
  if (change.test(17)) rhatchangle = rha;
  if (change.test(18))
    options.set(ArWallCloseStartBit,op.test(ArWallCloseStartBit));
  if (change.test(19))
    options.set(ArWallCloseEndBit,op.test(ArWallCloseEndBit));
  if (change.test(20))
    attributes[ArWallWidth2] = attributes_[ArWallWidth2];
  if (change.test(21))
    attributes[ArWallHeight2] = attributes_[ArWallHeight2];
  if (change.test(22))
    attributes[ArWallGap] = attributes_[ArWallGap];
  if (change.test(23))
    attributes[ArWallCavity] = attributes_[ArWallCavity];
  if (change.test(26)) 
    options.set(ArWallFrame,op.test(ArWallFrame));
  if (change.test(27))
    attributes[ArStudSpacing] = attributes_[ArStudSpacing];
  if (change.test(28))
    options.set(ArStudsUnderTrusses,op.test(ArStudsUnderTrusses));
  if (change.test(30)) 
    options.set(ArWallFrameExtendStart,op.test(ArWallFrameExtendStart));
  if (change.test(31)) 
    options.set(ArWallFrameExtendEnd,op.test(ArWallFrameExtendEnd));
  draw(DM_NORMAL);
}

EntityList Wall::changegeometry(Point pn1,Point pn2,double a1,double a2)
{Transform t1,t2;
 Point p3,p4,p5,p6,xaxis,yaxis,dir;
 double d1,d4;
 SortedDoubleList blist1,blist4;
  db.saveundo(UD_MOVED,this);
  draw(DM_ERASE);
  cadwindow->invalidatedisplaylist(this);

  dir = p2 - p1;
  xaxis = dir.normalize();
  yaxis = xaxis.cross(zaxis).normalize() * width / 2.0;
  p3 = p1-yaxis-xaxis*(width/2.0*tan(angle1));
  p4 = p1+yaxis+xaxis*(width/2.0*tan(angle1));
  angle1 = a1;
  angle2 = a2;
  p1 = pn1;
  p2 = pn2;
  xaxis = (p2 - p1).normalize();
  yaxis = xaxis.cross(zaxis).normalize() * width / 2.0;
  p5 = p1-yaxis-xaxis*(width/2.0*tan(angle1));
  p6 = p1+yaxis+xaxis*(width/2.0*tan(angle1));
  d1 = (p5 - p3).length();  if ((p5 - p3).dot(dir) > 0.0) d1 = -d1;
  d4 = (p6 - p4).length();  if ((p6 - p4).dot(dir) > 0.0) d4 = -d4;
  for (break2dlist1.start() ; ! break2dlist1.atend() ; )
    blist1.add(break2dlist1.next() + d1);
  for (break2dlist4.start() ; ! break2dlist4.atend() ; )
    blist4.add(break2dlist4.next() + d4);
  break2dlist1.destroy();
  break2dlist4.destroy();
  break2dlist1 = blist1;
  break2dlist4 = blist4;
  draw(DM_NORMAL);

  return EntityList();
}

class WSCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
     WSCheckBoxDialogControl(int i,BitMask *v,int b) : CheckBoxDialogControl(i,v,b) {};
     void load(Dialog *dialog);
     int select(Dialog *dialog);
  };

void WSCheckBoxDialogControl::load(Dialog *dialog)
{int i;
  CheckBoxDialogControl::load(dialog);
  for (i = 0 ; i <= 200 ; i += 100)
    EnableWindow(GetDlgItem(dialog->gethdlg(),105+i),! oldvalue);
}

int WSCheckBoxDialogControl::select(Dialog *dialog)
{int i,status;
  status = CheckBoxDialogControl::select(dialog);
  for (i = 0 ; i <= 200 ; i += 100)
    EnableWindow(GetDlgItem(dialog->gethdlg(),105+i),! oldvalue);
  return status;
}

class WNListDialogControl : public ListDialogControl
  {public:
     WNListDialogControl(int id,RCCHAR *n,int l) : ListDialogControl(id,0,0,n,l) {}
     void load(Dialog *);
  };

void WNListDialogControl::load(Dialog *dialog)
{RCCHAR walltype[300],key[300];
 int i;

  for (i = 0 ; ; i++)
    {  sprintf(key,"WallType%d",i);
       GetPrivateProfileString(_RCT("SBSWallTypes"),key,_RCT(""),walltype,300,home.getinifilename());
       if (strlen(walltype) == 0)
         break;
       ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,walltype);
    }

  dialog->SetDlgItemText(100,svalue);

}

class ArWallButtonDialogControl : public ButtonDialogControl
  {public:
     ArWallButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };


int ArWallButtonDialogControl::select(Dialog *dialog)
{RCCHAR walltype[300],wt[300],key[300],section[300],text[300];
 int i;

  dialog->currentvalue(100,walltype,300);
  for (i = 0 ; ; i++)
    {  sprintf(key,"WallType%d",i);
       GetPrivateProfileString(_RCT("SBSWallTypes"),key,_RCT(""),wt,300,home.getinifilename());
       if (strcmp(walltype,wt) == 0 || strlen(wt) == 0)
         break;
    }

  if (strlen(wt) == 0)
    {  ((ListDialogControl *)dialog->getcontrol(100))->additem(dialog,walltype);
       WritePrivateProfileString(_RCT("SBSWallTypes"),key,walltype,home.getinifilename());
    }

  sprintf(section,_RCT("WallType-%s"),walltype);
  dialog->GetDlgItemText(101,text,300);  WritePrivateProfileString(section,_RCT("ar::wallwidth"),text,home.getinifilename());
  dialog->GetDlgItemText(102,text,300);  WritePrivateProfileString(section,_RCT("ar::wallheight"),text,home.getinifilename());
  dialog->GetDlgItemText(103,text,300);  WritePrivateProfileString(section,_RCT("ar::walleaves"),text,home.getinifilename());
  dialog->GetDlgItemText(105,text,300);  WritePrivateProfileString(section,_RCT("ar::wallpitch"),text,home.getinifilename());
  dialog->GetDlgItemText(106,text,300);  WritePrivateProfileString(section,_RCT("ar::wallpeakdistance"),text,home.getinifilename());
  dialog->GetDlgItemText(107,text,300);  WritePrivateProfileString(section,_RCT("ar::wallpeakheight"),text,home.getinifilename());
  dialog->GetDlgItemText(108,text,300);  WritePrivateProfileString(section,_RCT("ar::wallmiddlecolour"),text,home.getinifilename());
  dialog->GetDlgItemText(109,text,300);  WritePrivateProfileString(section,_RCT("ar::wallrightcolour"),text,home.getinifilename());
  dialog->GetDlgItemText(110,text,300);  WritePrivateProfileString(section,_RCT("ar::walllhatchspacing"),text,home.getinifilename());
  dialog->GetDlgItemText(111,text,300);  WritePrivateProfileString(section,_RCT("ar::wallrhatchspacing"),text,home.getinifilename());
  dialog->GetDlgItemText(112,text,300);  WritePrivateProfileString(section,_RCT("ar::walllhatchangle"),text,home.getinifilename());
  dialog->GetDlgItemText(113,text,300);  WritePrivateProfileString(section,_RCT("ar::wallrhatchangle"),text,home.getinifilename());
  dialog->GetDlgItemText(116,text,300);  WritePrivateProfileString(section,_RCT("ar::wallwidth2"),text,home.getinifilename());
  dialog->GetDlgItemText(117,text,300);  WritePrivateProfileString(section,_RCT("ar::wallheight2"),text,home.getinifilename());
  dialog->GetDlgItemText(118,text,300);  WritePrivateProfileString(section,_RCT("ar::wallgap"),text,home.getinifilename());
  dialog->GetDlgItemText(123,text,300);  WritePrivateProfileString(section,_RCT("ar::studspacing"),text,home.getinifilename());
  strcpy(text,SendDlgItemMessage(dialog->gethdlg(),119,BM_GETCHECK,0,0) == BST_CHECKED ? _RCT("1") : _RCT("0"));  WritePrivateProfileString(section,_RCT("ar::wallcavity"),text,home.getinifilename());
  strcpy(text,SendDlgItemMessage(dialog->gethdlg(),122,BM_GETCHECK,0,0) == BST_CHECKED ? _RCT("1") : _RCT("0"));  WritePrivateProfileString(section,_RCT("ar::frame"),text,home.getinifilename());
  strcpy(text,SendDlgItemMessage(dialog->gethdlg(),124,BM_GETCHECK,0,0) == BST_CHECKED ? _RCT("1") : _RCT("0"));  WritePrivateProfileString(section,_RCT("ar::studsundertrusses"),text,home.getinifilename());
 
  return 0;

}

class ArWallDisplayDialogControl : public DisplayDialogControl
  {private:
     RCCHAR oldwalltype[300];
   public:
     ArWallDisplayDialogControl(int id) : DisplayDialogControl(id) {  strcpy(oldwalltype,"");  }
     void draw(Dialog *dialog,RCHDC hDC);
  };

void ArWallDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{RCCHAR walltype[300],section[300],text[300];

  dialog->currentvalue(100,walltype,300);
  if (strcmp(walltype,oldwalltype) != 0)
    {  sprintf(section,_RCT("WallType-%s"),walltype);
       GetPrivateProfileString(section,_RCT("ar::wallwidth"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(101,text);
       GetPrivateProfileString(section,_RCT("ar::wallheight"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(102,text);
       GetPrivateProfileString(section,_RCT("ar::walleaves"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(103,text);
       GetPrivateProfileString(section,_RCT("ar::wallpitch"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(105,text);
       GetPrivateProfileString(section,_RCT("ar::wallpeakdistance"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(106,text);
       GetPrivateProfileString(section,_RCT("ar::wallpeakheight"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(107,text);
       GetPrivateProfileString(section,_RCT("ar::wallmiddlecolour"),_RCT("0"),text,300,home.getinifilename());  dialog->SetDlgItemText(108,text);
       GetPrivateProfileString(section,_RCT("ar::wallrightcolour"),_RCT("0"),text,300,home.getinifilename());  dialog->SetDlgItemText(109,text);
       GetPrivateProfileString(section,_RCT("ar::walllhatchspacing"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(110,text);
       GetPrivateProfileString(section,_RCT("ar::wallrhatchspacing"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(111,text);
       GetPrivateProfileString(section,_RCT("ar::walllhatchangle"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(112,text);
       GetPrivateProfileString(section,_RCT("ar::wallrhatchangle"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(113,text);
       GetPrivateProfileString(section,_RCT("ar::wallwidth2"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(116,text);
       GetPrivateProfileString(section,_RCT("ar::wallheight2"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(117,text);
       GetPrivateProfileString(section,_RCT("ar::wallgap"),_RCT("0.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(118,text);
       GetPrivateProfileString(section,_RCT("ar::wallcavity"),_RCT("0.0"),text,300,home.getinifilename());  ((CheckBoxDialogControl *)dialog->getcontrol(119))->change(dialog,text[0] == '1');
       GetPrivateProfileString(section,_RCT("ar::frame"),_RCT("1.0"),text,300,home.getinifilename());  ((CheckBoxDialogControl *)dialog->getcontrol(122))->change(dialog,text[0] == '1');
       GetPrivateProfileString(section,_RCT("ar::studsundertrusses"),_RCT("0.0"),text,300,home.getinifilename());  ((CheckBoxDialogControl *)dialog->getcontrol(124))->change(dialog,text[0] == '1');
       GetPrivateProfileString(section,_RCT("ar::studspacing"),_RCT("600.0"),text,300,home.getinifilename());  dialog->SetDlgItemText(123,text);
       strcpy(oldwalltype,walltype);
    }
}

int Wall::setup(BitMask *change,RCCHAR *name,RCCHAR *,double *w,double *h,double *e,double *p,double *pd,double *ph,int *mc,int *rc,double *lhs,double *rhs,double *lha,double *rha,short nattributes,double *attributes,BitMask *o,BitMask *,Wall *)
{Dialog dialog("ArWall_dialog");
 DialogControl *dc;
 ResourceString rs15(NCARCHITECT+15);
 int i,cavity,result;
  dialog.add(new WNListDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,w,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,h,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,e,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new WSCheckBoxDialogControl(104,o,0));
  dialog.add(dc = new RealDialogControl(105,p));
  dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(dc = new RealDialogControl(106,pd,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(107,ph,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(dc = new IntegerDialogControl(108,mc,0,256));
  dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(dc = new IntegerDialogControl(109,rc,0,256));
  dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(dc = new RealDialogControl(110,lhs));
  dialog.add(new ScrollBarDialogControl(1110,dc));
  dialog.add(dc = new RealDialogControl(111,rhs));
  dialog.add(new ScrollBarDialogControl(1111,dc));
  dialog.add(dc = new RealDialogControl(112,lha));
  dialog.add(new ScrollBarDialogControl(1112,dc));
  dialog.add(dc = new RealDialogControl(113,rha));
  dialog.add(new ScrollBarDialogControl(1113,dc));
  dialog.add(dc = new RealDialogControl(116,&attributes[ArWallWidth2],ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1116,dc));
  dialog.add(dc = new RealDialogControl(117,&attributes[ArWallHeight2],ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1117,dc));
  dialog.add(dc = new RealDialogControl(118,&attributes[ArWallGap],ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1118,dc));
  cavity = int(attributes[ArWallCavity]);
  dialog.add(new CheckBoxDialogControl(119,&cavity));
  dialog.add(new CheckBoxDialogControl(122,o,ArWallFrame));
  dialog.add(new CheckBoxDialogControl(124,o,ArStudsUnderTrusses));
  dialog.add(new ButtonDialogControl(300));
  dialog.add(new ArWallButtonDialogControl(120));
  dialog.add((DisplayDialogControl *) new ArWallDisplayDialogControl(121));
  dialog.add(dc = new RealDialogControl(123,&attributes[ArStudSpacing]));
  dialog.add(new ScrollBarDialogControl(1123,dc));

  if (change != 0)
    {  dialog.title(rs15.gets());
       change->clearall();
       for (i = 0 ; i < 25 ; i++)
         if (i <= 19 || i >= 22)
           dialog.add(new CheckBoxDialogControl(200+i,change,4+i));
       dialog.add(new CheckBoxDialogControl(114,o,2));
       dialog.add(new CheckBoxDialogControl(115,o,3));
    }
  else
    {  dialog.adddisable(114);
       dialog.adddisable(115);
       dialog.adddisable(314);
    }
  result = dialog.process();
  if (result == TRUE)
    {  attributes[ArWallCavity] = cavity;
    }
  return result;
}

void Wall::getdefaults(BOOL,RCCHAR *name,RCCHAR *,double *w,double *h,double *eaves,double *pitch,double *peakdistance,double *peakheight,int *middlecolour,int *rightcolour,double *lhatchspacing,double *rhatchspacing,double *lhatchangle,double *rhatchangle,short *nattributes,double *attributes,BitMask *o,BitMask *)
{ strcpy(name,v.getstring("ar::wallname"));
  *w = v.getreal("ar::wallwidth");
  *h = v.getreal("ar::wallheight");
  *eaves = v.getreal("ar::walleaves");
  *pitch = v.getreal("ar::wallpitch");
  *peakdistance = v.getreal("ar::wallpeakdistance");
  *peakheight = v.getreal("ar::wallpeakheight");
  *middlecolour = v.getinteger("ar::wallmiddlecolour");
  *rightcolour = v.getinteger("ar::wallrightcolour");
  *lhatchspacing = v.getreal("ar::walllhatchspacing");
  *rhatchspacing = v.getreal("ar::wallrhatchspacing");
  *lhatchangle = v.getreal("ar::walllhatchangle");
  *rhatchangle = v.getreal("ar::wallrhatchangle");
  *nattributes = ArMaxWallAttributes;
  attributes[ArWallWidth2] = v.getreal("ar::wallwidth2");
  attributes[ArWallHeight2] = v.getreal("ar::wallheight2");
  attributes[ArWallGap] = v.getreal("ar::wallgap");
  attributes[ArWallCavity] = v.getreal("ar::wallcavity");
  attributes[ArStudSpacing] = v.getreal("ar::studspacing");
  *o = *v.getbm("ar::walloptions");
}

void Wall::setdefaults(RCCHAR *name,RCCHAR *,double w,double h,double eaves,double pitch,double peakdistance,double peakheight,int middlecolour,int rightcolour,double lhatchspacing,double rhatchspacing,double lhatchangle,double rhatchangle,short nattributes,double *attributes,const BitMask &o,const BitMask &)
{ v.setstring("ar::wallname",name);
  v.setreal("ar::wallwidth",w);
  v.setreal("ar::wallheight",h);
  v.setreal("ar::walleaves",eaves);
  v.setreal("ar::wallpitch",pitch);
  v.setreal("ar::wallpeakdistance",peakdistance);
  v.setreal("ar::wallpeakheight",peakheight);
  v.setinteger("ar::wallmiddlecolour",middlecolour);
  v.setinteger("ar::wallrightcolour",rightcolour);
  v.setreal("ar::walllhatchspacing",lhatchspacing);
  v.setreal("ar::wallrhatchspacing",rhatchspacing);
  v.setreal("ar::walllhatchangle",lhatchangle);
  v.setreal("ar::wallrhatchangle",rhatchangle);
  v.setreal("ar::wallwidth2",attributes[ArWallWidth2]);
  v.setreal("ar::wallheight2",attributes[ArWallHeight2]);
  v.setreal("ar::wallgap",attributes[ArWallGap]);
  v.setreal("ar::wallcavity",attributes[ArWallCavity]);
  v.setreal("ar::studspacing",attributes[ArStudSpacing]);
  v.setbm("ar::walloptions",o);
}

void Wall::setdefaults(void)
{ v.setstring("ar::wallname",name);
  v.setreal("ar::wallwidth",width);
  v.setreal("ar::wallheight",height);
  v.setreal("ar::walleaves",eaves);
  v.setreal("ar::wallpitch",pitch);
  v.setreal("ar::wallpeakdistance",peakdistance);
  v.setreal("ar::wallpeakheight",peakheight);
  v.setinteger("ar::wallmiddlecolour",middlecolour);
  v.setinteger("ar::wallrightcolour",rightcolour);
  v.setreal("ar::walllhatchspacing",lhatchspacing);
  v.setreal("ar::wallrhatchspacing",rhatchspacing);
  v.setreal("ar::walllhatchangle",lhatchangle);
  v.setreal("ar::wallrhatchangle",rhatchangle);
  v.setreal("ar::wallwidth2",attributes[ArWallWidth2]);
  v.setreal("ar::wallheight2",attributes[ArWallHeight2]);
  v.setreal("ar::wallgap",attributes[ArWallGap]);
  v.setreal("ar::wallcavity",attributes[ArWallCavity]);
  v.setreal("ar::studspacing",attributes[ArStudSpacing]);
  v.setbm("ar::walloptions",options);
}

int Wall::save(CadFile *outfile)
{short type;
 int i;
  type = wall_entity;
  if (! outfile->write(&type)) return 0;
  if (! saveheader(outfile)) return 0;
  if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  if (! zaxis.save(outfile)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&height)) return 0;
  if (! outfile->write(&eaves)) return 0;
  if (! outfile->write(&pitch)) return 0;
  if (! outfile->write(&peakdistance)) return 0;
  if (! outfile->write(&peakheight)) return 0;
  if (! outfile->write(&middlecolour)) return 0;
  if (! outfile->write(&rightcolour)) return 0;
  if (! outfile->write(&lhatchspacing)) return 0;
  if (! outfile->write(&rhatchspacing)) return 0;
  if (! outfile->write(&lhatchangle)) return 0;
  if (! outfile->write(&rhatchangle)) return 0;
  if (! outfile->write(&angle1)) return 0;
  if (! outfile->write(&angle2)) return 0;
  if (! outfile->write(name)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&nattributes)) return 0;
  for (i = 0 ; i < nattributes ; i++)
    if (! outfile->write(&attributes[i])) return 0;
  if (! openinglist.save(outfile)) return 0;
  if (! break2dlist1.save(outfile)) return 0;
  if (! break2dlist4.save(outfile)) return 0;
//  if (! openinglist.savegeometry(outfile)) return 0;  For figures...
  return 1;
}

Entity *Wall::load(int references,CadFile *infile)
{Wall *wall;
 EntityHeader *header;
 Point p1,p2,zaxis;
 BitMask options(32);
 RCCHAR *n;
 double width,height,angle1,angle2,eaves,pitch,peakdistance,peakheight,lhatchspacing,rhatchspacing,lhatchangle,rhatchangle,attribute[ArMaxWallAttributes*10];  //  Allow for future expansion in attributes
 short middlecolour,rightcolour,nattributes;
 EntityList openinglist,junklist;
 SortedDoubleList blist1,blist4;
  if ((header = loadheader(infile)) == NULL) return 0;
  if (! p1.load(infile)) return 0;
  if (! p2.load(infile)) return 0;
  if (! zaxis.load(infile)) return 0;
  if (! infile->read(&width)) return 0;
  if (! infile->read(&height)) return 0;
  if (! infile->read(&eaves)) return 0;
  if (! infile->read(&pitch)) return 0;
  if (! infile->read(&peakdistance)) return 0;
  if (! infile->read(&peakheight)) return 0;
  if (! infile->read(&middlecolour)) return 0;
  if (! infile->read(&rightcolour)) return 0;
  if (! infile->read(&lhatchspacing)) return 0;
  if (! infile->read(&rhatchspacing)) return 0;
  if (! infile->read(&lhatchangle)) return 0;
  if (! infile->read(&rhatchangle)) return 0;
  if (! infile->read(&angle1)) return 0;
  if (! infile->read(&angle2)) return 0;
  if (! infile->read(&n)) return 0;
  if (! infile->read(&options)) return 0;
  if (options.test(ArWallAttributes))
    {  if (! infile->read(&nattributes)) return 0;
       for (int i = 0 ; i < nattributes ; i++)
         if (! infile->read(&attribute[i])) return 0;
    }
  else
    nattributes = 0;
  if (references)
    {  if (! openinglist.load(infile)) return NULL;
    }
  else
    {  if (! junklist.load(infile)) return NULL;  junklist.destroy();
    }
  if (! blist1.load(infile)) return 0;
  if (! blist4.load(infile)) return 0;
  wall = new Wall(*header,p1,p2,zaxis,n,_RCT(""),width,height,eaves,pitch,peakdistance,peakheight,middlecolour,rightcolour,lhatchspacing,rhatchspacing,lhatchangle,rhatchangle,angle1,angle2,nattributes,attribute,options,options);
  wall->atlist = header->getatlist();
  wall->openinglist = openinglist;
  wall->break2dlist1 = blist1;
  wall->break2dlist4 = blist4;
  delete n;
  return wall;
}

int Wall::save(UndoFile *outfile)
{short type;
  type = wall_entity;
  if (! outfile->write(&type)) return 0;
  if (! outfile->blockwrite(this,sizeof(*this))) return 0;
  if (! outfile->write(&visible)) return 0;
  if (! atlist.save(outfile)) return 0;
  if (! outfile->write(name)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! openinglist.save(outfile)) return 0;
  if (! break2dlist1.save(outfile)) return 0;
  if (! break2dlist4.save(outfile)) return 0;
  if (! outfile->write(&nattributes)) return 0;
  for (int i = 0 ; i < nattributes ; i++)
    if (! outfile->write(&attributes[i])) return 0;
  return 1;
}

Entity *Wall::load(UndoFile *infile)
{Wall *wall;
 EntityList list;
 SortedDoubleList blist1,blist4;
 char bm[sizeof(BitMask)];
  if ((wall = new Wall) != 0)
    {AttributeList al;
       memcpy(bm,&wall->visible,sizeof(bm));
       if (! infile->blockread(wall,sizeof(*wall))) return 0;
       memcpy(&wall->visible,bm,sizeof(bm));
       if (! infile->read(&wall->visible)) return 0;
       if (! al.load(infile)) return 0;   wall->atlist = al;
       if (! infile->read(&wall->name)) return NULL;
       if (! infile->read(&wall->options)) return 0;
       if (! list.load(infile)) return 0;
       if (! blist1.load(infile)) return 0;
       if (! blist4.load(infile)) return 0;
       if (! infile->read(&wall->nattributes)) return 0;
       for (int i = 0 ; i < wall->nattributes ; i++)
         if (! infile->read(&wall->attributes[i])) return 0;
       wall->openinglist = list;
       wall->break2dlist1 = blist1;
       wall->break2dlist4 = blist4;
    }
  return wall;
}

int Wall::saveattributes(CadFile *outfile)
{ if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  if (! outfile->write(&width)) return 0;
  if (! outfile->write(&height)) return 0;
  if (! outfile->write(&eaves)) return 0;
  if (! outfile->write(&pitch)) return 0;
  if (! outfile->write(&peakdistance)) return 0;
  if (! outfile->write(&peakheight)) return 0;
  if (! outfile->write(&middlecolour)) return 0;
  if (! outfile->write(&rightcolour)) return 0;
  if (! outfile->write(&lhatchspacing)) return 0;
  if (! outfile->write(&rhatchspacing)) return 0;
  if (! outfile->write(&lhatchangle)) return 0;
  if (! outfile->write(&rhatchangle)) return 0;
  if (! outfile->write(&angle1)) return 0;
  if (! outfile->write(&angle2)) return 0;
  if (! outfile->write(name)) return 0;
  if (! outfile->write(&options)) return 0;
  if (! outfile->write(&nattributes)) return 0;
  for (int i = 0 ; i < nattributes ; i++)
    if (! outfile->write(&attributes[i])) return 0;
  return 1;
}

int Wall::loadattributes(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! p1.load(infile)) return 0;
  if (! p2.load(infile)) return 0;
  if (! infile->read(&width)) return 0;
  if (! infile->read(&height)) return 0;
  if (! infile->read(&eaves)) return 0;
  if (! infile->read(&pitch)) return 0;
  if (! infile->read(&peakdistance)) return 0;
  if (! infile->read(&peakheight)) return 0;
  if (! infile->read(&middlecolour)) return 0;
  if (! infile->read(&rightcolour)) return 0;
  if (! infile->read(&lhatchspacing)) return 0;
  if (! infile->read(&rhatchspacing)) return 0;
  if (! infile->read(&lhatchangle)) return 0;
  if (! infile->read(&rhatchangle)) return 0;
  if (! infile->read(&angle1)) return 0;
  if (! infile->read(&angle2)) return 0;
  if (! infile->read(&name)) return 0;
  if (! infile->read(&options)) return 0;
  if (! infile->read(&nattributes)) return 0;
  for (int i = 0 ; i < nattributes ; i++)
    if (! infile->read(&attributes[i])) return 0;
  return 1;
}

int Wall::savedxf(int blocksection,DXFOut *dxffile)
{Transform tview;
 Point xax1,xax2,xaxis,yaxis,za,p3,p4,p5,p6,dir;
 int j;
 EntityList list,emptylist,clist;
 BitMask o(32);
 SortedDoubleList sdl,esdl;
 Figure *figure;
 Entity *e;
 RCCHAR *leaf;
 double totalwidth,walllength,t1;

  if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0)
    totalwidth = width + attributes[ArWallWidth2] + attributes[ArWallGap];
  else    
    totalwidth = width;
  walllength = (p2 - p1).length();
  if (1)  //  Only export TOP View
    {  xaxis = (p2 - p1).normalize();
       yaxis = (p2 - p1).cross(zaxis).normalize();
       for (j = 0 ; j < 4 ; j++)
         if (ArWallCavity < nattributes && attributes[ArWallCavity] != 0.0 || j == 0 || j == 3)   
           {  if (j == 0)
                {  p3 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
                   p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist1.start() ; ! break2dlist1.atend() ; )
                     sdl.add(break2dlist1.next());
                   leaf = _RCT("1");
                }
              else if (j == 1)
                {  p3 = p1 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(angle1));
                   p4 = p2 + yaxis*(totalwidth/2.0-width) + xaxis*((totalwidth/2.0-width)*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist2.start() ; ! break2dlist2.atend() ; )
                     sdl.add(break2dlist2.next());
                   leaf = _RCT("2");
                }
              else if (j == 2)
                {  p3 = p1 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(angle1));
                   p4 = p2 - yaxis*(totalwidth/2.0-attributes[ArWallWidth2]) - xaxis*((totalwidth/2.0-attributes[ArWallWidth2])*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist3.start() ; ! break2dlist3.atend() ; )
                     sdl.add(break2dlist3.next());
                   leaf = _RCT("3");
                }
              else
                {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
                   p4 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
                   sdl = esdl;
                   for (break2dlist4.start() ; ! break2dlist4.atend() ; )
                     sdl.add(break2dlist4.next());
                   leaf = _RCT("4");
                }

            dir = (p4 - p3).normalize();
            for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
              if (figure->cutout(0,leaf,&clist))
                {  for (clist.start() ; (e = clist.next()) != 0 ; )
                     {  if (e->isa(line_entity))
                          {  p5 = ((Line *)e)->getp1();  p5.z = p3.z;
                             p6 = ((Line *)e)->getp2();  p6.z = p3.z;
                             sdl.add(fabs((p5 - p3).dot(dir)));
                             sdl.add(fabs((p6 - p3).dot(dir)));
                          }
                        delete e;
                     }
                   clist.destroy();
                }
            p5 = p3;
            for (sdl.start() ; ! sdl.atend() ; )
              {  t1 = sdl.next();
                 if (t1 < 1.0 || t1 >= walllength - 1.0)
                   {  sdl.next();  continue;  //  Discard breaks not actually on the wall
                   } 
                 p6 = p3 + dir * t1;
                 Line line1(*this,p5,p6);
                 if (j == 1 && rightcolour < 256)
                   line1.EntityHeader::change(rightcolour,getlayer(),getstyle(),getweight());
                 line1.savedxf(blocksection,dxffile);
                 p5 = p3 + dir * sdl.next();
              }
            Line line1(*this,p5,p4);
            if (j == 1 && rightcolour < 256)
              line1.EntityHeader::change(rightcolour,getlayer(),getstyle(),getweight());
            line1.savedxf(blocksection,dxffile);
            sdl.destroy();
         }
       for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
         if (figure->cutout(0,_RCT("5"),&clist))
           {  for (clist.start() ; (e = clist.next()) != 0 ; )
                {  e->EntityHeader::change(middlecolour < 256 ? middlecolour : getcolour(),getlayer(),getstyle(),getweight());
                   e->savedxf(blocksection,dxffile);
                   delete e;
                }
              clist.destroy();
           }

       if (options.test(1) || options.test(2))
         {  p3 = p1 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(angle1));
            p4 = p1 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(angle1));
            Line line1(*this,p3,p4);
            line1.savedxf(blocksection,dxffile);
         }

       if (options.test(1) || options.test(3))
         {  p3 = p2 - yaxis*totalwidth/2.0 - xaxis*(totalwidth/2.0*tan(-angle2));
            p4 = p2 + yaxis*totalwidth/2.0 + xaxis*(totalwidth/2.0*tan(-angle2));
            Line line2(*this,p3,p4);
            line2.savedxf(blocksection,dxffile);
         }

    }

  return 1;

}

int Wall::exportiges(FILE *,int,int,int *,int *)
{ return 1;
}

int Wall::savegeometry(CadFile *outfile)
{ if (! p1.save(outfile)) return 0;
  if (! p2.save(outfile)) return 0;
  if (! zaxis.save(outfile)) return 0;
  if (! outfile->write(&angle1)) return 0;
  if (! outfile->write(&angle2)) return 0;
  if (! openinglist.save(outfile)) return 0;
  if (! break2dlist1.save(outfile)) return 0;
  if (! break2dlist4.save(outfile)) return 0;
  return 1;
}

int Wall::loadgeometry(CadFile *infile)
{ cadwindow->invalidatedisplaylist(this);
  if (! p1.load(infile)) return 0;
  if (! p2.load(infile)) return 0;
  if (! zaxis.load(infile)) return 0;
  if (! infile->read(&angle1)) return 0;
  if (! infile->read(&angle2)) return 0;
  openinglist.destroy();
  break2dlist1.destroy();
  break2dlist4.destroy();
  if (! openinglist.load(infile)) return 0;
  if (! break2dlist1.load(infile)) return 0;
  if (! break2dlist4.load(infile)) return 0;
  return 1;
}

void Wall::fixupreferences(Entity ** index)
{ openinglist.fixupreferences(index);
  Entity::fixupreferences(index);
}

extern void architect_command2(int *cstate,HighLevelEvent *event,void **);
extern void architect_command24(int *cstate,HighLevelEvent *event,void **);

void Wall::verify(void)
{Dialog dialog1("ArWallVerify_dialog");
 Dialog dialog2("ArColumnVerify_dialog");
 int c,l,s,cavity;
 double length,w;
 RCCHAR direction[128],start[128],end[128];
 Point p3,p4;
 Workplane *wp;
 void *data;
 int state;
  if ((wp = db.workplanes.getcurrent()) != NULL)
    {  p3 = wp->modeltoworkplane(p1);
       p4 = wp->modeltoworkplane(p2);
    }
  else
    {  p3 = p1;  p4 = p2;
    }
  if (isacolumn())
    {  EntityHeader::verify(&dialog2,&c,&l,&s,&w);
       dialog2.add(new StringDialogControl(121,name,strlen(name)+1));
       dialog2.add(new StringDialogControl(104,((p3+p4)/2.0).string(start),128));
       dialog2.add(new StringDialogControl(106,(p3-p4).string(direction),128));
       dialog2.add(new RealDialogControl(108,&width,ScaleByLength));
       dialog2.add(new RealDialogControl(109,&height,ScaleByLength));
       dialog2.add(new ButtonDialogControl(122));
       if (dialog2.process() == 122)
         architect_command24(&state,0,&data);
    }
  else
    {  EntityHeader::verify(&dialog1,&c,&l,&s,&w);
       dialog1.add(new StringDialogControl(121,name,strlen(name)+1));
       dialog1.add(new StringDialogControl(104,p3.string(start),128));
       dialog1.add(new StringDialogControl(105,p4.string(end),128));
       dialog1.add(new StringDialogControl(106,(p4-p3).string(direction),128));
       length = (p2 - p1).length();
       dialog1.add(new RealDialogControl(107,&length,ScaleByLength));
       dialog1.add(new RealDialogControl(108,&width,ScaleByLength));
       dialog1.add(new RealDialogControl(109,&height,ScaleByLength));
       dialog1.add(new RealDialogControl(110,&eaves,ScaleByLength));
       dialog1.add(new CheckBoxDialogControl(111,&options,0));
       dialog1.add(new RealDialogControl(112,&pitch));
       dialog1.add(new RealDialogControl(113,&peakdistance,ScaleByLength));
       dialog1.add(new RealDialogControl(114,&peakheight,ScaleByLength));
       dialog1.add(new IntegerDialogControl(115,&middlecolour));
       dialog1.add(new IntegerDialogControl(116,&rightcolour));
       dialog1.add(new RealDialogControl(117,&lhatchspacing));
       dialog1.add(new RealDialogControl(118,&rhatchspacing));
       dialog1.add(new RealDialogControl(119,&lhatchangle));
       dialog1.add(new RealDialogControl(120,&rhatchangle));
       dialog1.add(new RealDialogControl(125,&attributes[ArWallWidth2],ScaleByLength));
       dialog1.add(new RealDialogControl(126,&attributes[ArWallHeight2],ScaleByLength));
       dialog1.add(new RealDialogControl(127,&attributes[ArWallGap],ScaleByLength));
       cavity = int(attributes[ArWallCavity]);
       dialog1.add(new CheckBoxDialogControl(128,&cavity));
       dialog1.add(new CheckBoxDialogControl(130,&options,ArWallFrame));
       dialog1.add(new CheckBoxDialogControl(132,&options,ArStudsUnderTrusses));
       dialog1.add(new ButtonDialogControl(122));
       dialog1.add(new ButtonDialogControl(123));
       dialog1.add(new ButtonDialogControl(124));
       dialog1.add(new RealDialogControl(131,&attributes[ArStudSpacing]));
       if ((status = dialog1.process()) == 122)
         architect_command2(&state,0,&data);
       else if (status == 123)
         {SurfaceProperties sp;
            sp.verify(this,0);
         }
       else if (status == 124)
         {SurfaceProperties sp;
            sp.verify(this,1);
         }
    }
}

void Wall::extents(View3dSurface *surface,Point *pmin,Point *pmax)
{Point p3;
  if (surface == NULL)
    {  *pmin = *pmax = p1;
       p3 = p2;
    }
  else
    {  *pmin = *pmax = surface->modeltouv(p1);
       p3 = surface->modeltouv(p2);
    }
  if (p3.x < pmin->x) pmin->x = p3.x;
  if (p3.y < pmin->y) pmin->y = p3.y;
  if (p3.z < pmin->z) pmin->z = p3.z;
  if (p3.x > pmax->x) pmax->x = p3.x;
  if (p3.y > pmax->y) pmax->y = p3.y;
  if (p3.z > pmax->z) pmax->z = p3.z;
}

int Wall::issame(int geometry,const Entity &entity) const
{ if (! entity.isa(wall_entity)) return 0;
  switch (geometry)
    {  case Geometry :
         return 0;
       case Attributes :
         return ((EntityHeader *)this)->issame(entity);
    }
  return 0;
}

int Wall::explode(void)
{ return 1;
}

int ClipToOpenings(PointList *pl,Point p1,Point p2,double sillwidth,double platewidth,Point *p3,Point *p4,Point *p5,Point *p6)
{double y1,y2,y3;
 Point p7,p8;
  y1 = y2 = -1.0;
  pl->start();
  while (! pl->atend())
    {  p8 = pl->next();  p7 = pl->next();
       if (p1.x > p7.x+0.5 && p1.x < p8.x-0.5)
         {  if (y1 == -1.0)
              y1 = p7.y;
            else
              y2 = p7.y;
         }
    }

  if (y1 > y2)
    {  y3 = y1;  y1 = y2;  y2 = y3;  
    }

  if (y2 == -1.0)
    {  *p3 = Point(p1.x,p1.y+platewidth,0.0);  *p4 = Point(p2.x,p2.y-platewidth,0.0);
       return 1;
    }
  else if (y1 < 1.0)
    {  *p3 = Point(p1.x,y2+sillwidth,0.0);  *p4 = Point(p2.x,p2.y-platewidth,0.0);
       return 2;
    }
  else if (y2 > p2.y - 1.0)
    {  *p3 = Point(p1.x,p1.y+platewidth,0.0);  *p4 = Point(p1.x,y1-sillwidth,0.0);
       return 3;
    }
  else
    {  *p3 = Point(p1.x,p1.y+platewidth,0.0);  *p4 = Point(p1.x,y1-sillwidth,0.0);
       *p5 = Point(p1.x,y2+sillwidth,0.0);  *p6 = Point(p2.x,p2.y-platewidth,0.0);
       return 4;
    }

}

void InsertWallFrameMember(int layer,double walllength,Point p1,Point p2,double width,EntityList *list)
{Point xa,ya,p[5];
 BitMask options(32);
 Curve *c;
  xa = (p2 - p1).normalize();
  if ((p1 - p2).length() < 0.00001)
    xa.setxyz(1.0,0.0,0.0);
  ya = Point(0.0,0.0,1.0).cross(xa).normalize() * width;
  p[0] = p2;  
  p[1] = p1;
  p[2] = p1 + ya;
  p[3] = p2 + ya;
  p[4] = p2;
  for (int i = 0 ; i < 5 ; i++)
    p[i].x = walllength - p[i].x;
  c = new Curve(5,0,p,0,0,options);
  c->EntityHeader::change(c->getcolour(),layer,c->getstyle(),c->getweight());
  db.geometry.add(c);
  list->add(c);
}

RCCHAR *GetWallLabel(Wall *wall)
{static RCCHAR buffer[300];
 AttributeList atlist;
 RCCHAR *at;
 int wallnumber;

  atlist = wall->getatlist().copy();
  wallnumber = 0;
  for (atlist.start() ; (at = atlist.next()) != NULL ; )
    if (strncmp(at,"ArWallReference=",16) == 0)
      {  if (strchr(at,'_') != 0)
           wallnumber = atoi(strchr(at,'_')+1);
      }
  atlist.destroy();

  if (wallnumber != 0)
    {  sprintf(buffer,"WF1_%d",wallnumber);
       return buffer;
    }
  else
    return _RCT("");
 
}

#define MAX_OPENINGS 200

struct OpeningData
  {double xmin,ymin,xmax,ymax,side,top,bottom;
  };

int Wall::frame(Point origin,Point *size,short viewno,int frameno)
{EntityList clist,list;
 Figure *figure;
 Entity *e;
 Point p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,pf1,pf2,xa,ya,yaxis;
 SortedPointList spl;
 SortedDoubleList sdl;
 PointList pl,pl1;
 double spacing,studwidth,sillwidth,platewidth,noggingheight,l1,l2,offset,totalstudlength,height1,height2;
 int i,j,n,n1,n2,lintel,above,below,matched,closematch,nopenings;
 OpeningData openingdata[MAX_OPENINGS];
 AttributeList atlist;
 RCCHAR *at,text[300],name[300],leftlabel[300],rightlabel[300];
 BitMask o(32);
 double extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,walllength,totalwidth,noggingwidth;
 int precision,tolprecision;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 BitMask visible(MaxViews);
 SortedPointList list1,list2,list3,list4,list5;
 Line *line;
 Text *txt;
 EntityList glist; 
 Circle *cr;
 LinearDimension *ld;
 Group *group;
   
  View *vw;
  db.header.getvisible()->clearall();
  for (i = 0 ; i < 32 ; i++)
    if ((vw = db.views3d.match(i)) != 0 && ! vw->getvisibility())
      db.header.getvisible()->set(i);

  spacing = 600.0;
  if (nattributes > ArStudSpacing )
    spacing = attributes[ArStudSpacing];

  studwidth = 35.0;
  sillwidth = 35.0;
  platewidth = 35.0;
  noggingwidth = 35.0;
  if (height > 2490)
    noggingheight = 1360;
  else
    noggingheight = 1210;
  noggingheight -= noggingwidth / 2.0;

  height1 = height2 = height;
  if (fabs(peakheight) > 1.0E-5)
    {  if (fabs(peakdistance - 0.0) > 0.01 && fabs(peakdistance-100.0) > 0.01)
         {char message[300];
            sprintf(message,"Wall frame %d is not rectangular or triangular.  It has not been framed correctly.",frameno);
            cadwindow->MessageBox(message,"Frame",MB_OK);
         }
       if (fabs(peakdistance - 0.0) < 0.01)
         height1 += peakheight;
       if (fabs(peakdistance - 100.0) < 0.01)
         height2 += peakheight;
    }

  totalwidth = gettotalwidth();

  l1 = -totalwidth / 2.0 * tan(angle1);
  l2 = (-totalwidth / 2.0 + width) * tan(angle1);
  if (options.test(ArWallFrameExtendStart) && l1 < l2) 
    l1 = l2;
  if (! options.test(ArWallFrameExtendStart) && l1 > l2) 
    l1 = l2;
  p3 = p1 - (p2-p1).normalize()*l1;

  l1 = totalwidth / 2.0 * tan(-angle2);
  l2 = (totalwidth / 2.0 - width) * tan(-angle2);
  if (options.test(ArWallFrameExtendEnd) && l1 < l2) 
    l1 = l2;
  if (! options.test(ArWallFrameExtendEnd) && l1 > l2) 
    l1 = l2;
  p4 = p2 + (p2-p1).normalize()*l1;

  yaxis = (p2 - p1).cross(zaxis).normalize();

  pf1 = p3;
  pf2 = p4;

  //  Insert the wall frame lines
  p5 = pf1 + yaxis*(totalwidth/2.0);
  p6 = pf2 + yaxis*(totalwidth/2.0);
  line = new Line(p5,p6);
  line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
  db.geometry.add(line);

  p5 = pf1 + yaxis*(totalwidth/2.0 - width);
  p6 = pf2 + yaxis*(totalwidth/2.0 - width);
  line = new Line(p5,p6);
  line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
  db.geometry.add(line);

  walllength = (p4 - p3).length();

  if (size != 0)
    {  size->x = walllength + 1200.0;
       size->y = height + 2000.0;
       return 1;
    }

  db.header.getvisible()->clearall();
  View *view;
  for (i = 0 ; i < 32 ; i++)
    if ((view = db.views3d.match(i)) != 0 && ! view->getvisibility())
      db.header.getvisible()->set(i);

  visible = *db.header.getvisible();
  db.header.getvisible()->clearall();
  db.header.getvisible()->set(viewno);

  InsertWallFrameMember(BOTTOMPLATE_LAYER,walllength+2.0*origin.x,Point(origin.x,origin.y,0.0),Point(origin.x + walllength,origin.y,0.0),platewidth,&glist);
  InsertWallFrameMember(TOPPLATE_LAYER,walllength+2.0*origin.x,Point(origin.x,origin.y+height1-platewidth,0.0),Point(origin.x + walllength,origin.y+height2-platewidth,0.0),platewidth,&glist);

  spl.add(origin);
  spl.add(origin+Point(1.0,0.0,0.0)*walllength);

  nopenings = 0;
  for (openinglist.start() ; (figure = (Figure *) openinglist.next()) != 0 ; )
    if (figure->cutout(1,_RCT("1"),&clist))
      {  strcpy(name,figure->getlibrary());
         _strlwr(name);
         openingdata[nopenings].top = openingdata[nopenings].bottom = openingdata[nopenings].side = 0.0;
         atlist = figure->getatlist().copy();
         for (atlist.start() ; (at = atlist.next()) != NULL ; )
           if (strncmp(at,"AR::FrameAllowance::",20) == 0)
             sscanf(at+20,"%lf,%lf,%lf",&openingdata[nopenings].top,&openingdata[nopenings].bottom,&openingdata[nopenings].side);
 
         openingdata[nopenings].xmax = RCDBL_MAX;
         openingdata[nopenings].xmin = RCDBL_MAX;
         openingdata[nopenings].ymax = RCDBL_MAX;
         openingdata[nopenings].ymin = RCDBL_MAX;
         for (clist.start() ; ! clist.atend() ; )
           {  e = clist.next();
              if (e != 0 && e->isa(line_entity))
                {  p3 = ((Line *)e)->getp1();
                   p4 = ((Line *)e)->getp2();
                   xa = (p2 - p1).normalize();
                   ya = Point(0.0,0.0,1.0);
                   p5.x = (p3 - pf1).dot(xa);
                   p5.y = (p3 - pf1).dot(ya);
                   p5.z = 0.0;
                   p6.x = (p4 - pf1).dot(xa);
                   p6.y = (p4 - pf1).dot(ya);
                   p6.z = 0.0;
                   if (openingdata[nopenings].xmax == RCDBL_MAX || p5.x > openingdata[nopenings].xmax) openingdata[nopenings].xmax = p5.x;
                   if (openingdata[nopenings].xmax == RCDBL_MAX || p6.x > openingdata[nopenings].xmax) openingdata[nopenings].xmax = p6.x;
                   if (openingdata[nopenings].xmin == RCDBL_MAX || p5.x < openingdata[nopenings].xmin) openingdata[nopenings].xmin = p5.x;
                   if (openingdata[nopenings].xmin == RCDBL_MAX || p6.x < openingdata[nopenings].xmin) openingdata[nopenings].xmin = p6.x;
                   if (openingdata[nopenings].ymax == RCDBL_MAX || p5.y > openingdata[nopenings].ymax) openingdata[nopenings].ymax = p5.y;
                   if (openingdata[nopenings].ymax == RCDBL_MAX || p6.y > openingdata[nopenings].ymax) openingdata[nopenings].ymax = p6.y;
                   if (openingdata[nopenings].ymin == RCDBL_MAX || p5.y < openingdata[nopenings].ymin) openingdata[nopenings].ymin = p5.y;
                   if (openingdata[nopenings].ymin == RCDBL_MAX || p6.y < openingdata[nopenings].ymin) openingdata[nopenings].ymin = p6.y;
                }
           }
         clist.destroy();
         if (openingdata[nopenings].xmin != RCDBL_MAX && openingdata[nopenings].xmax != RCDBL_MAX && openingdata[nopenings].ymin != RCDBL_MAX && openingdata[nopenings].ymax != RCDBL_MAX && openingdata[nopenings].ymin <= height)
           nopenings++;
      } 

  //  Search for any openings that are adjacent, and adjust the opening position of the window to make the door and window stud at the same location.
  for (i = 0 ; i < nopenings ; i++)
    {  for (j = i+1 ; j < nopenings ; j++)
         if (fabs(openingdata[i].xmax - openingdata[j].xmin) < 1.0)
           {  if (openingdata[i].ymin < openingdata[j].ymin)
                openingdata[j].xmin += openingdata[i].side + openingdata[j].side;
              else
                openingdata[i].xmax -= openingdata[i].side + openingdata[j].side;
           } 
         else if (fabs(openingdata[i].xmin - openingdata[j].xmax) < 1.0)
           {  if (openingdata[j].ymin < openingdata[i].ymin)
                openingdata[i].xmin += openingdata[i].side + openingdata[j].side;
              else
                openingdata[j].xmax -= openingdata[i].side + openingdata[j].side;
           } 
    }

  for (i = 0 ; i < nopenings ; i++)
    {  p5 = pf1 + yaxis*(totalwidth/2.0 - width/2.0) + (pf2 - pf1).normalize() * openingdata[i].xmax;
       p6 = pf1 + yaxis*(totalwidth/2.0 - width/2.0) + (pf2 - pf1).normalize() * openingdata[i].xmin;
       line = new Line(p5,p6);
       line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
       line->setvisible(visible);
       db.geometry.add(line);

       if (strstr(name,"notch.cad") != 0)
         {//  Allow notches to be near the sides

            if (openingdata[i].xmin - openingdata[i].side > 1.0)
              spl.add(Point(openingdata[i].xmin-openingdata[i].side,0.0,0.0)+origin);
            else
              openingdata[i].xmin = studwidth + openingdata[i].side;  
            if (openingdata[i].xmax + openingdata[i].side < walllength - 1.0)
              spl.add(Point(openingdata[i].xmax+openingdata[i].side,0.0,0.0)+origin);
            else
              openingdata[i].xmax = walllength - studwidth - openingdata[i].side;

            if (openingdata[i].ymin - openingdata[i].bottom > 1.0)
              {  pl.add(Point(openingdata[i].xmin-openingdata[i].side,openingdata[i].ymin-openingdata[i].bottom,0.0)+origin);  
                 pl.add(Point(openingdata[i].xmax+openingdata[i].side,openingdata[i].ymin-openingdata[i].bottom,0.0)+origin);  
              }

            if (openingdata[i].ymax + openingdata[i].top < height - 1.0)
              {  pl.add(Point(openingdata[i].xmin-openingdata[i].side,openingdata[i].ymax+openingdata[i].top,0.0)+origin);  
                 pl.add(Point(openingdata[i].xmax+openingdata[i].side,openingdata[i].ymax+openingdata[i].top,0.0)+origin);  
              }
            else
              {  pl.add(Point(openingdata[i].xmin-openingdata[i].side,height,0.0)+origin);  
                 pl.add(Point(openingdata[i].xmax+openingdata[i].side,height,0.0)+origin);  
              }
         }
       else
         {  spl.add(Point(openingdata[i].xmin-openingdata[i].side,0.0,0.0)+origin);
            spl.add(Point(openingdata[i].xmax+openingdata[i].side,0.0,0.0)+origin);
            if (openingdata[i].xmin - openingdata[i].side < studwidth - 1.0 || openingdata[i].xmin - openingdata[i].side > studwidth + 1.0 && openingdata[i].xmin - openingdata[i].side < studwidth * 2.0 - 1.0 ||
                openingdata[i].xmax + openingdata[i].side > walllength - (studwidth - 1.0) || openingdata[i].xmax + openingdata[i].side > walllength - (studwidth + 1.0) && openingdata[i].xmax + openingdata[i].side < walllength - (studwidth * 2.0 - 1.0))
              {char message[300];
                 sprintf(message,"A stud may be too close to the end of wall frame %d",frameno); 
                 cadwindow->MessageBox(message,"Frame",MB_OK);
              }
            pl.add(Point(openingdata[i].xmin-openingdata[i].side,openingdata[i].ymin-openingdata[i].bottom,0.0)+origin);
            pl.add(Point(openingdata[i].xmax+openingdata[i].side,openingdata[i].ymin-openingdata[i].bottom,0.0)+origin);
            pl.add(Point(openingdata[i].xmin-openingdata[i].side,openingdata[i].ymax+openingdata[i].top,0.0)+origin);
            pl.add(Point(openingdata[i].xmax+openingdata[i].side,openingdata[i].ymax+openingdata[i].top,0.0)+origin);
         }
    }

  //  Add extra studs
  Line line1(p1,p2);
  list = *db.geometry.getlist();
  list = list.copy();
  xa = (p2 - p1).normalize();
  ya = xa.cross(zaxis).normalize() * gettotalwidth() / 2.0;

  //  Add the studs required due to load points
  for (list.start() ; (e = list.next()) != 0 ; )
    if (e->isa(figure_entity))
      {Point p3;
       char name[300];
        strcpy(name,((Figure *)e)->getlibrary());
        _strlwr(name);
        if (strstr(name,"loadpoint") != 0)
          {  p5 = Point((nearp(((Figure *)e)->getorigin()) - pf1).dot(xa) - studwidth / 2.0,0.0,0.0) + origin;
             if ((nearp(((Figure *)e)->getorigin()) - ((Figure *)e)->getorigin()).length() < totalwidth / 2.0 + 5.0 && p5.x - origin.x >= 0.0 && p5.x - origin.x <= walllength)
               spl.add(p5);
          } 
      }

  //  Add the studs required due to touching wall frames.
  Line line2(p1+ya-xa*(gettotalwidth()/2.0*tan(angle1)),p2+ya-xa*(gettotalwidth()/2.0*tan(-angle2)));
  Line line3(p1-ya+xa*(gettotalwidth()/2.0*tan(angle1)),p2-ya+xa*(gettotalwidth()/2.0*tan(-angle2)));
  for (list.start() ; (e = list.next()) != 0 ; )
    if (e->isa(wall_entity) && e != this)
      {int on1,on2,on3,on4,matched1,matched2;
       Point p3,p5,p6;
         on1 = line2.ison(((Wall *)e)->p1) && (line2.nearp(((Wall *)e)->p1) - ((Wall *)e)->p1).length() < db.getptoler();
         on2 = line2.ison(((Wall *)e)->p2) && (line2.nearp(((Wall *)e)->p2) - ((Wall *)e)->p2).length() < db.getptoler();
         on3 = line3.ison(((Wall *)e)->p1) && (line3.nearp(((Wall *)e)->p1) - ((Wall *)e)->p1).length() < db.getptoler();
         on4 = line3.ison(((Wall *)e)->p2) && (line3.nearp(((Wall *)e)->p2) - ((Wall *)e)->p2).length() < db.getptoler();
         if (on1 || on2 || on3 || on4)
           {  if (on1)
                {  p5 = Point((((Wall *)e)->p1 - pf1).dot(xa) - ((Wall *)e)->gettotalwidth() / 2.0 - studwidth,0.0,0.0) + origin;
                   p6 = Point((((Wall *)e)->p1 - pf1).dot(xa) - (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()),0.0,0.0) + origin;
                }
              else if (on2)
                {  p5 = Point((((Wall *)e)->p2 - pf1).dot(xa) + ((Wall *)e)->gettotalwidth() / 2.0,0.0,0.0) + origin;
                   p6 = Point((((Wall *)e)->p2 - pf1).dot(xa) + (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()) - studwidth,0.0,0.0) + origin;
                } 
              else if (on3)
                {  p5 = Point((((Wall *)e)->p1 - pf1).dot(xa) - ((Wall *)e)->gettotalwidth() / 2.0 - studwidth,0.0,0.0) + origin;
                   p6 = Point((((Wall *)e)->p1 - pf1).dot(xa) - (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()),0.0,0.0) + origin;
                }
              else
                {  p5 = Point((((Wall *)e)->p2 - pf1).dot(xa) + ((Wall *)e)->gettotalwidth() / 2.0,0.0,0.0) + origin;
                   p6 = Point((((Wall *)e)->p2 - pf1).dot(xa) + (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()) - studwidth,0.0,0.0) + origin;
                }
           }
         else
           {  if ((((Wall *)e)->p1 - p1).length() < db.getptoler() && ! ((Wall *)e)->getoptions()->test(ArWallFrameExtendStart) && getoptions()->test(ArWallFrameExtendStart))
                {  if ((((((Wall *)e)->p2 - ((Wall *)e)->p1)).cross(p2 - p1)).z < 0.0)
                     {  p5 = Point((((Wall *)e)->p1 - pf1).dot(xa) + ((Wall *)e)->gettotalwidth() / 2.0,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p1 - pf1).dot(xa) + (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()) - studwidth,0.0,0.0) + origin;
                     }
                   else
                     {  p5 = Point((((Wall *)e)->p1 - pf1).dot(xa) - ((Wall *)e)->gettotalwidth() / 2.0 - studwidth,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p1 - pf1).dot(xa) - (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()),0.0,0.0) + origin;
                     }
                   on1 = 1;
                }
              else if ((((Wall *)e)->p1 - p2).length() < db.getptoler() && ! ((Wall *)e)->getoptions()->test(ArWallFrameExtendStart) && getoptions()->test(ArWallFrameExtendEnd))
                {  if ((((((Wall *)e)->p2 - ((Wall *)e)->p1)).cross(p2 - p1)).z < 0.0)
                     {  p5 = Point((((Wall *)e)->p1 - pf1).dot(xa) + ((Wall *)e)->gettotalwidth() / 2.0,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p1 - pf1).dot(xa) + (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()) - studwidth,0.0,0.0) + origin;
                     }
                   else
                     {  p5 = Point((((Wall *)e)->p1 - pf1).dot(xa) - ((Wall *)e)->gettotalwidth() / 2.0 - studwidth,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p1 - pf1).dot(xa) - (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()),0.0,0.0) + origin;
                     }
                   on2 = 1;
                }
              else if ((((Wall *)e)->p2 - p1).length() < db.getptoler() && ! ((Wall *)e)->getoptions()->test(ArWallFrameExtendEnd) && getoptions()->test(ArWallFrameExtendStart))
                {  if ((((((Wall *)e)->p2 - ((Wall *)e)->p1)).cross(p2 - p1)).z < 0.0)
                     {  p5 = Point((((Wall *)e)->p2 - pf1).dot(xa) + ((Wall *)e)->gettotalwidth() / 2.0,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p2 - pf1).dot(xa) + (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()) - studwidth,0.0,0.0) + origin;
                     }
                   else
                     {  p5 = Point((((Wall *)e)->p2 - pf1).dot(xa) - ((Wall *)e)->gettotalwidth() / 2.0 - studwidth,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p2 - pf1).dot(xa) - (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()),0.0,0.0) + origin;
                     }
                   on3 = 1;
                }
              else if ((((Wall *)e)->p2 - p2).length() < db.getptoler() && ! ((Wall *)e)->getoptions()->test(ArWallFrameExtendEnd) && getoptions()->test(ArWallFrameExtendEnd))
                {  if ((((((Wall *)e)->p2 - ((Wall *)e)->p1)).cross(p2 - p1)).z < 0.0)
                     {  p5 = Point((((Wall *)e)->p2 - pf1).dot(xa) + ((Wall *)e)->gettotalwidth() / 2.0,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p2 - pf1).dot(xa) + (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()) - studwidth,0.0,0.0) + origin;
                     }
                   else
                     {  p5 = Point((((Wall *)e)->p2 - pf1).dot(xa) - ((Wall *)e)->gettotalwidth() / 2.0 - studwidth,0.0,0.0) + origin;
                        p6 = Point((((Wall *)e)->p2 - pf1).dot(xa) - (((Wall *)e)->gettotalwidth() / 2.0 - ((Wall *)e)->getwidth()),0.0,0.0) + origin;
                     }
                   on4 = 1;
                }
           }

         if (on1 || on2 || on3 || on4)
           {  spl.start();
              matched1 = matched2 = 0;
              while (! spl.atend())
                {  p3 = spl.next();
                   //  Check to see if this stud is reversed 
                   n1 = ClipToOpenings(&pl,p3,p3+Point(0.0,height,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
                   n2 = ClipToOpenings(&pl,p3+Point(studwidth,0.0,0.0),p3+Point(studwidth,height,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
                   if (n2 != n1 || p3.x + studwidth - origin.x > walllength)
                     p3.x -= studwidth;
                   //  Match the existing stud with the new ones
                   if (! matched1)
                     matched1 = fabs(p3.x - p5.x) <= studwidth * 0.9999;
                   if (! matched2)
                     matched2 = fabs(p3.x - p6.x) <= studwidth * 0.9999;
                }
              if (! matched1 && p5.x - origin.x >= 0.0 && p5.x - origin.x <= walllength - studwidth + 0.01)
                spl.add(p5);
              if (! matched2 && p6.x - origin.x >= 0.0 && p6.x - origin.x <= walllength - studwidth + 0.01)
                spl.add(p6);
 
           }

      }

  //  Get the labels for the touching wall frames at each end of the wall.
  //  Add the studs required due to touching wall frames.
  strcpy(leftlabel,"");
  strcpy(rightlabel,"");
  for (list.start() ; (e = list.next()) != 0 ; )
    if (e->isa(wall_entity) && e != this)
      {  if ((((Wall *)e)->p1 - p1).length() < gettotalwidth() / 2.0 + 5.0 || (((Wall *)e)->p2 - p1).length() < gettotalwidth() / 2.0 + 5.0)
           strcpy(rightlabel,GetWallLabel((Wall *)e));
         if (strlen(rightlabel) == 0 && (p1 - ((Wall *)e)->nearp(p1)).length() <= ((Wall *)e)->gettotalwidth() / 2.0 && Line(((Wall *)e)->p1,((Wall *)e)->p2).ison(nearp(p1)))
           strcpy(rightlabel,GetWallLabel((Wall *)e));

         if ((((Wall *)e)->p1 - p2).length() < gettotalwidth() / 2.0 + 5.0 || (((Wall *)e)->p2 - p2).length() < gettotalwidth() / 2.0 + 5.0)
           strcpy(leftlabel,GetWallLabel((Wall *)e));
         if (strlen(leftlabel) == 0 && (p2 - ((Wall *)e)->nearp(p2)).length() <= ((Wall *)e)->gettotalwidth() / 2.0 && Line(((Wall *)e)->p1,((Wall *)e)->p2).ison(nearp(p2)))
           strcpy(leftlabel,GetWallLabel((Wall *)e));

      }

  for (list.start() ; (e = list.next()) != 0 ; )
    if (e->isa(line_entity) && e->getlayer() == TRUSSPLAN_LAYER && options.test(ArStudsUnderTrusses))
      {Point p3,p4,p5;
         p3 = ((Line *)e)->getp1();  p3.z = p1.z;
         p4 = ((Line *)e)->getp2();  p4.z = p1.z;
         Line line2(p3+(p3-p4).normalize()*50.0,p4+(p4-p3).normalize()*50.0);
         Intersect i(&line1,&line2);
         if (i.nintersections() > 0 && line1.ison(((PointE *)i.intersection(0))->getp()) && line2.ison(((PointE *)i.intersection(0))->getp()))
           {  p5 = Point((((PointE *)i.intersection(0))->getp() - pf1).dot(xa),0.0,0.0) + origin;
              spl.start();
              matched = 0;
              closematch = 0;
              while (! matched && ! spl.atend())
                {  //  Check to see if this stud is reversed 
                   p3 = spl.next();
                   n1 = ClipToOpenings(&pl,p3,p3+Point(0.0,height,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
                   n2 = ClipToOpenings(&pl,p3+Point(studwidth,0.0,0.0),p3+Point(studwidth,height,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
                   if (n2 != n1 || p3.x + studwidth - origin.x > walllength)
                     p3.x -= studwidth;
                   matched = fabs(p3.x - p5.x) < studwidth * 0.9999;
                   closematch = fabs(p3.x - p5.x) <  10.0;
                }

              if (matched && ! closematch)
                {//  Try a stud on the other side of the truss
                   p5.x -= studwidth;
                   spl.start();
                   matched = 0;
                   while (! matched && ! spl.atend())
                     {  //  Check to see if this stud is reversed 
                        p3 = spl.next();
                        n1 = ClipToOpenings(&pl,p3,p3+Point(0.0,height,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
                        n2 = ClipToOpenings(&pl,p3+Point(studwidth,0.0,0.0),p3+Point(studwidth,height,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
                        if (n2 != n1 || p3.x + studwidth - origin.x > walllength)
                          p3.x -= studwidth;
                        matched = fabs(p3.x - p5.x) <  studwidth * 0.9999;
                     }
                }

              if (! matched && p5.x - origin.x >= 0.0 && p5.x - origin.x <= walllength - studwidth - 0.01)
                {  spl.add(p5);
                   cr = new Circle(Point(walllength+2.0*origin.x-p5.x,height+p5.y,p5.z),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),20.0,0.0,acos(-1.0));
                   cr->EntityHeader::change(cr->getcolour(),CLIP_LAYER,cr->getstyle(),cr->getweight());
                   db.geometry.add(cr);
                   glist.add(cr);
                   sdl.add(p5.x - origin.x); 
                }
           }
      }

  ya.setxyz(0.0,1.0,0.0);
  list.destroy();


  //  Insert additional studs to avoid a stud spacing greater than spacing
  spl.start();
  p3 = spl.next();  
  while (! spl.atend())
    {  p4 = spl.next();
       n = int(floor((p4.x - p3.x) / (spacing + 1.0)) + 1);
       if (n > 1)
         {  for (i = 1 ; i < n-1 ; i++)
              spl.add(p3 + (p4 - p3).normalize() * double(i) * spacing);
            spl.add((p3 + (p4 - p3).normalize() * double(n-2) * spacing + p4)/2.0);
            spl.start();
            p4 = spl.next();
         }
       p3 = p4;
    }

  //  Insert the studs
  for (spl.start() ; ! spl.atend() ; )
    {  p3 = spl.next();
       n1 = ClipToOpenings(&pl,p3,p3+Point(0.0,height1 + (height2 - height1) * (p3.x - origin.x) / walllength,0.0),sillwidth,platewidth,&p5,&p6,&p7,&p8);
       n2 = ClipToOpenings(&pl,p3+Point(studwidth,0.0,0.0),p3+Point(studwidth,height1 + (height2 - height1) * (p3.x - origin.x) / walllength,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
       if (n2 != n1 || p3.x + studwidth - origin.x > walllength)
         {  if (n1 == 4)
              totalstudlength = (p6 - p5).length() + (p8 - p7).length();
            else  
              totalstudlength = (p9 - p10).length() + (p11 - p12).length();
            n1 = ClipToOpenings(&pl,p3+Point(-studwidth,0.0,0.0),p3+Point(-studwidth,height1 + (height2 - height1) * (p3.x - origin.x) / walllength,0.0),sillwidth,platewidth,&p5,&p6,&p7,&p8);
            n2 = ClipToOpenings(&pl,p3,p3+Point(0.0,height1 + (height2 - height1) * (p3.x - origin.x) / walllength,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
            if (n1 == 4 && n2 == 1 && totalstudlength > (p6 - p5).length() + (p8 - p7).length())
              {//  Laundry door - two openings adjacent, first position used most studs.
                 n2 = ClipToOpenings(&pl,p3,p3+Point(0.0,height1 + (height2 - height1) * (p3.x - origin.x) / walllength,0.0),sillwidth,platewidth,&p9,&p10,&p11,&p12);
                 n1 = ClipToOpenings(&pl,p3+Point(studwidth,0.0,0.0),p3+Point(studwidth,height1 + (height2 - height1) * (p3.x - origin.x) / walllength,0.0),sillwidth,platewidth,&p5,&p6,&p7,&p8);
                 p9.x = p5.x;
                 p10.x = p6.x;
              } 
            else
              offset = -studwidth;
         }
       else
         offset = 0.0; 

       if (n1 == 4 && n2 == 1)
         {//  There is an opening on both sides of the stud edge.  Laundary window/door combination
          //  Cut the middle out of the stud.
            n2 = n1;
            p11 = p9;
            p12 = p10;
            p9.y = p5.y;
            p10.y = p6.y;
            p11.y = p7.y;
            p12.y = p8.y;
         }
         
        
       if (n1 != 0 && p5.y >= origin.y && p6.y >= origin.y)
         InsertWallFrameMember(STUD_LAYER,walllength+2.0*origin.x,p10,p9,-studwidth,&glist);

       if (n1 == 4 && p7.y >= origin.y && p8.y >= origin.y)
         InsertWallFrameMember(STUD_LAYER,walllength+2.0*origin.x,p12,p11,-studwidth,&glist);
         
       line = new Line(Point(walllength+2.0*origin.x-p3.x,origin.y - 200.0,0.0),Point(walllength+2.0*origin.x-p3.x,origin.y - 300.0,0.0));
       line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
       db.geometry.add(line);
       line = new Line(Point(walllength+2.0*origin.x-p3.x,origin.y - 200.0,0.0),Point(walllength+2.0*origin.x-(p9.x+offset),origin.y - 200.0,0.0));
       line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
       db.geometry.add(line);
       line = new Line(Point(walllength+2.0*origin.x-p3.x,origin.y - 300.0,0.0),Point(walllength+2.0*origin.x-(p9.x+offset),origin.y - 300.0,0.0));
       line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
       db.geometry.add(line);
       
       p5 = pf1 + yaxis*(totalwidth/2.0) + (pf2 - pf1).normalize() * (p3.x - origin.x);
       p6 = pf1 + yaxis*(totalwidth/2.0 - width) + (pf2 - pf1).normalize() * (p3.x - origin.x);
       line = new Line(p5,p6);
       line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
       line->setvisible(visible);
       db.geometry.add(line);

       p5 = pf1 + yaxis*(totalwidth/2.0) + (pf2 - pf1).normalize() * (p9.x + offset - origin.x);
       p6 = pf1 + yaxis*(totalwidth/2.0 - width) + (pf2 - pf1).normalize() * (p9.x + offset - origin.x);
       line = new Line(p5,p6);
       line->setvisible(visible);
       line->EntityHeader::change(line->getcolour(),WALLFRAMING_LAYER,line->getstyle(),line->getweight());
       db.geometry.add(line);

    }     

  for ( ; noggingheight + studwidth*3 < height ; noggingheight += 1350)
    {//  Insert the noggings
       spl.start();
       p3 = spl.next();
       n1 = ClipToOpenings(&pl,p3,p3+Point(0.0,height,0.0),sillwidth,platewidth,&p5,&p6,&p7,&p8);
       n2 = ClipToOpenings(&pl,p3+Point(studwidth,0.0,0.0),p3+Point(studwidth,height,0.0),sillwidth,platewidth,&p5,&p6,&p7,&p8);
       if (n2 == n1 && p3.x + studwidth - origin.x < walllength)
         p3 += Point(studwidth,0.0,0.0);
       while (! spl.atend())
         {  p4 = spl.next();
            n1 = ClipToOpenings(&pl,p4,p4+Point(0.0,height,0.0),noggingwidth,platewidth,&p5,&p6,&p7,&p8);
            n2 = ClipToOpenings(&pl,p4+Point(studwidth,0.0,0.0),p4+Point(studwidth,height,0.0),noggingwidth,platewidth,&p5,&p6,&p7,&p8);
            if (n2 != n1 || p4.x + studwidth - origin.x > walllength)
              p4 -= Point(studwidth,0.0,0.0);

            p7 = (p3 + p4) / 2.0+Point(0.0,noggingheight,0.0);
            above = below = 0;
            for (pl.start() ; ! pl.atend() ; )
              {  p6 = pl.next();  p5 = pl.next();
                 if (p7.x > p5.x && p7.x < p6.x && p7.y < p5.y + studwidth * 2.0)
                   below = 1;
                 if (p7.x > p5.x && p7.x < p6.x && p7.y > p5.y - studwidth * 2.0)
                   above = 1;
              }
 
            if (above == 0 || below == 0)
              {  InsertWallFrameMember(NOGGING_LAYER,walllength+2.0*origin.x,p3+Point(0.0,noggingheight,0.0),p4+Point(0.0,noggingheight,0.0),noggingwidth,&glist);
              }

            p3 = p4 + Point(studwidth,0.0,0.0);

         }
    }

  //  Insert the sills and lintels
  for (pl.start() ; ! pl.atend() ; )
    {  p3 = pl.next();  p4 = pl.next();
       pl1 = pl;
       lintel = 1;
       for (pl1.start() ; ! pl1.atend() ; )
         {  p5 = pl1.next();  p6 = pl1.next();
            if (fabs(p5.x - p3.x) < 1.0 && fabs(p6.x - p4.x) < 1.0 && p5.y < p3.y)
              lintel = 0;
         }
       if (lintel && (p3.y - sillwidth < origin.y || p4.y - sillwidth < origin.y))
         continue;

       if (lintel)
         {  if (p3.y > sillwidth)
              {  InsertWallFrameMember(SILL_LAYER,walllength+2.0*origin.x,p4-Point(0.0,sillwidth,0.0),p3-Point(0.0,sillwidth,0.0),sillwidth,&glist);
                 
                 //  Insert the height of the opening
                 BitMask o(32);
                 o.set(1);
                 sprintf(text,"%.0lf",p3.y - origin.y);
                 txt = new Text(CHAR2RCCHR("Arial"),Point(walllength+2.0*origin.x - (p3+p4).x/2.0,(p3+p4).y/2.0+25.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,200.0,0,0,0,CHAR2RCCHR(text),0,o);
                 txt->EntityHeader::change(txt->getcolour(),WALLFRAMING_LAYER,txt->getstyle(),txt->getweight());
                 db.geometry.add(txt);
       
              }
         }
       else
         {  if (p3.y < origin.y + height - sillwidth)
              {  InsertWallFrameMember(LINTEL_LAYER,walllength+2.0*origin.x,p4,p3,sillwidth,&glist);

                 //  Insert the height of the opening
                 sprintf(text,"%.0lf",p3.y - origin.y);
                 BitMask o(32);
                 o.set(1);
                 o.set(4);
                 txt = new Text(CHAR2RCCHR("Arial"),Point(walllength+2.0*origin.x - (p3+p4).x/2.0,(p3+p4).y/2.0-25.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,200.0,0,0,0,CHAR2RCCHR(text),0,o);
                 txt->EntityHeader::change(txt->getcolour(),WALLFRAMING_LAYER,txt->getstyle(),txt->getweight());
                 db.geometry.add(txt);
              }
         }
    }     

  //  Insert the wall frame label
  atlist = getatlist().copy();
  for (atlist.start() ; (at = atlist.next()) != NULL ; )
    if (strncmp(at,"ArWallReference=",16) == 0)
      db.geometry.add(new Text(CHAR2RCCHR("Arial"),origin+Point(0.0,-600.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,200.0,0,0,0,CHAR2RCCHR(at+16),0,o));

  BitMask to(32);
  to.set(1);
  db.geometry.add(new Text(CHAR2RCCHR("Arial"),origin+Point(-80.0,height/2.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,140.0,0,90.0,0,CHAR2RCCHR(leftlabel),0,to));
  db.geometry.add(new Text(CHAR2RCCHR("Arial"),origin+Point(walllength+140.0,height/2.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,140.0,0,90.0,0,CHAR2RCCHR(rightlabel),0,to));

  LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&o);
  gap = 0.0;
  extension = 1.5;
  vinfo.height = 1.5;
  term1.length = 1.5;
  term1.height = 0.5;
 
  if (height2 > height1)
    height1 = height2; 

  CoordinateSystem cs1(origin+Point(0.0,height+900.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0));
  list1.add(cs1.frommodel(Point(origin)+Point(0.0,height+100.0,0.0)));
  list1.add(cs1.frommodel(Point(origin)+Point(walllength,height+100.0,0.0)));
  vinfo.height = 2.0;
  ld = new LinearDimension(origin+Point(0.0,height1+900.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),list1,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,o);
  ld->EntityHeader::change(ld->getcolour(),WALLFRAMING_LAYER,ld->getstyle(),ld->getweight());
  db.geometry.add(ld);

  CoordinateSystem cs2(origin+Point(-600.0,0.0,0.0),Point(0.0,1.0,0.0),Point(-1.0,0.0,0.0));
  list2.add(cs2.frommodel(origin+Point(-100.0,0.0,0.0)));
  list2.add(cs2.frommodel(origin+Point(-100.0,height,0.0)));
  vinfo.height = 2.0;
  ld = new LinearDimension(origin+Point(-600.0,0.0,0.0),Point(0.0,1.0,0.0),Point(-1.0,0.0,0.0),list2,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,o);
  ld->EntityHeader::change(ld->getcolour(),WALLFRAMING_LAYER,ld->getstyle(),ld->getweight());
  db.geometry.add(ld);

  CoordinateSystem cs3(origin+Point(0.0,height+600.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0));
  list3.add(cs3.frommodel(Point(origin)+Point(0.0,height+100.0,0.0)));
  list3.add(cs3.frommodel(Point(origin)+Point(walllength,height+100.0,0.0)));
  for (pl.start() ; ! pl.atend() ; )
    {  p3 = pl.next();  p4 = pl.next();
       pl1 = pl;
       lintel = 1;
       for (pl1.start() ; ! pl1.atend() ; )
         {  p5 = pl1.next();  p6 = pl1.next();
            if (fabs(p5.x - p3.x) < 1.0 && fabs(p6.x - p4.x) < 1.0 && p5.y < p3.y)
              lintel = 0;
         }
       if (lintel)
         {  list3.add(cs3.frommodel(Point(walllength+2.0*origin.x - p3.x,origin.y+height+100.0,0.0)));
            list3.add(cs3.frommodel(Point(walllength+2.0*origin.x - p4.x,origin.y+height+100.0,0.0)));
         } 
    }
  vinfo.height = 2.0;
  ld = new LinearDimension(origin+Point(0.0,height1+600.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),list3,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,o);
  ld->EntityHeader::change(ld->getcolour(),WALLFRAMING_LAYER,ld->getstyle(),ld->getweight());
  db.geometry.add(ld);

  CoordinateSystem cs4(origin+Point(0.0,height+300.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0));
  for (spl.start() ; ! spl.atend() ; )
    {  p3 = spl.next();
       n1 = ClipToOpenings(&pl,p3,p3+Point(0.0,height,0.0),sillwidth,platewidth,&p5,&p6,&p7,&p8);
       n2 = ClipToOpenings(&pl,p3+Point(studwidth,0.0,0.0),p3+Point(studwidth,height,0.0),sillwidth,platewidth,&p5,&p6,&p7,&p8);
       if (n2 != n1 || p3.x + studwidth - origin.x > walllength)
         list4.add(cs4.frommodel(Point(walllength+2.0*origin.x - (p3.x - studwidth / 2.0),origin.y+height+100.0,0.0)));
       else
         list4.add(cs4.frommodel(Point(walllength+2.0*origin.x - (p3.x + studwidth / 2.0),origin.y+height+100.0,0.0)));
    }
  vinfo.height = 1.5;
  ld = new LinearDimension(origin+Point(0.0,height1+300.0,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),list4,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,o);
  ld->EntityHeader::change(ld->getcolour(),WALLFRAMING_LAYER,ld->getstyle(),ld->getweight());
  db.geometry.add(ld);

  if (height > 2490)
    noggingheight = 1360;
  else
    noggingheight = 1210;
  if (height > noggingheight + studwidth*2)
    {  CoordinateSystem cs5(origin+Point(-300.0,0.0,0.0),Point(0.0,1.0,0.0),Point(-1.0,0.0,0.0));
       list5.add(cs5.frommodel(origin+Point(-100.0,0.0,0.0)));
       for ( ; noggingheight + studwidth*3 < height ; noggingheight += 1350)
         list5.add(cs5.frommodel(origin+Point(-100.0,noggingheight,0.0)));
       vinfo.height = 2.0;
       ld = new LinearDimension(origin+Point(-300.0,0.0,0.0),Point(0.0,1.0,0.0),Point(-1.0,0.0,0.0),list5,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,o);
       ld->EntityHeader::change(ld->getcolour(),WALLFRAMING_LAYER,ld->getstyle(),ld->getweight());
       db.geometry.add(ld);
    }

  sprintf(text,"WF_%d",frameno);
  group = new Group(glist,_RCT("Wall Frame"),text,BitMask(32));
  group->EntityHeader::change(group->getcolour(),WALLFRAMING_LAYER,group->getstyle(),group->getweight());
  db.geometry.add(group);

  db.header.setvisible(visible);

  return 1;

}

