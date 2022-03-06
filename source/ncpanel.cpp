
#include "ncwin.h"

class IPRadioButtonDialogControl : public RadioButtonDialogControl
{public:
   IPRadioButtonDialogControl(int i,int i1,int i2,int *v) :  RadioButtonDialogControl(i,i1,i2,v) {}
   void load(Dialog *);
   int select(Dialog *);
};

void IPRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),103),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1103),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1104),!oldvalue);
}

int IPRadioButtonDialogControl::select(Dialog *dialog)
{ RadioButtonDialogControl::select(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),103),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1103),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),!oldvalue);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1104),!oldvalue);
  return 0;
}


struct RBPanel1_data
  {int closed,count;
   double offset,height,thickness;
   Point p1,p2,pi1,pi2,plast,plast1,zaxis;
   Point vlast[2],v1[2],v2[2],v3[2],v4[2];
  };


void CreatePanel(Point p1,Point p2,Point p3,Point p4,Point z,int draw)
{Transform t;
 Entity *l1,*l2,*l3,*l4,*l5,*l6,*l7,*l8,*l9,*l10,*l11,*l12;
 Plane *plane1,*plane2,*plane3,*plane4,*plane5,*plane6;
 BitMask options(32);
 int id;
 Entity *e;
 RCCHAR instance[300];

  Line line1(p1,p2);
  Line line2(p2,p4);
  Line line3(p4,p3);
  Line line4(p3,p1);
  Line line5(p1,p1+z);
  Line line6(p2,p2+z);
  Line line7(p3,p3+z);
  Line line8(p4,p4+z);
  Line line9(p1+z,p2+z);
  Line line10(p2+z,p4+z);
  Line line11(p4+z,p3+z);
  Line line12(p3+z,p1+z);

  if (draw)
    {  //line1.draw(DM_INVERT);
       //line2.draw(DM_INVERT);
       //line3.draw(DM_INVERT);
       //line4.draw(DM_INVERT);
       line5.draw(DM_INVERT);
       line6.draw(DM_INVERT);
       line7.draw(DM_INVERT);
       line8.draw(DM_INVERT);
       line9.draw(DM_INVERT);
       line10.draw(DM_INVERT);
       line11.draw(DM_INVERT);
       line12.draw(DM_INVERT);
   }
  else
    {  
       db.saveundo(UD_STARTBLOCK,NULL);

       db.geometry.add(l1 = line1.clone(t));
       db.geometry.add(l2 = line2.clone(t));
       db.geometry.add(l3 = line3.clone(t));
       db.geometry.add(l4 = line4.clone(t));
       db.geometry.add(l5 = line5.clone(t));
       db.geometry.add(l6 = line6.clone(t));
       db.geometry.add(l7 = line7.clone(t));
       db.geometry.add(l8 = line8.clone(t));
       db.geometry.add(l9 = line9.clone(t));
       db.geometry.add(l10 = line10.clone(t));
       db.geometry.add(l11 = line11.clone(t));
       db.geometry.add(l12 = line12.clone(t));

       //  Add the surfaces as well
       EntityList list,emptylist;
       BitMask options(32);
       options.set(PlaneNoFill);
       list = emptylist;
       list.add(l1);
       list.add(l2);
       list.add(l3);
       list.add(l4);
       plane1 = new Plane(list,10.0,10.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),0,options);
       if (plane1->getdefined()) 
         db.geometry.add(plane1);

       list = emptylist;
       list.add(l9);
       list.add(l10);
       list.add(l11);
       list.add(l12);
       plane2 = new Plane(list,10.0,10.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),0,options);
       if (plane2->getdefined()) 
         db.geometry.add(plane2);

       list = emptylist;
       list.add(l1);
       list.add(l6);
       list.add(l9);
       list.add(l5);
       plane3 = new Plane(list,10.0,10.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),0,options);
       if (plane3->getdefined()) 
         db.geometry.add(plane3);

       list = emptylist;
       list.add(l2);
       list.add(l8);
       list.add(l10);
       list.add(l6);
       plane4 = new Plane(list,10.0,10.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),0,options);
       if (plane4->getdefined()) 
         db.geometry.add(plane4);

       list = emptylist;
       list.add(l3);
       list.add(l7);
       list.add(l11);
       list.add(l8);
       plane5 = new Plane(list,10.0,10.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),0,options);
       if (plane5->getdefined()) 
         db.geometry.add(plane5);

       list = emptylist;
       list.add(l4);
       list.add(l5);
       list.add(l12);
       list.add(l7);
       plane6 = new Plane(list,10.0,10.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),0,options);
       if (plane6->getdefined()) 
         db.geometry.add(plane6);

       list = emptylist;
       list.add(l1);
       list.add(l2);
       list.add(l3);
       list.add(l4);
       list.add(l5);
       list.add(l6);
       list.add(l7);
       list.add(l8);
       list.add(l9);
       list.add(l10);
       list.add(l11);
       list.add(l12);

       list.add(plane1);
       list.add(plane2);
       list.add(plane4);
       list.add(plane6);

       list.add(plane3);
       list.add(plane5);

       Group *group;
       options.set(0);

       id = 0;
       for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
         if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"RC::Panel") == 0)
           {  if (strstr(((Group *)e)->getinstance(),"ID=") != 0 && atoi(strstr(((Group *)e)->getinstance(),"ID=") + 3) > id)
                id = atoi(strstr(((Group *)e)->getinstance(),"ID=") + 3);
           }
       sprintf(instance,"ID=%d",id+1);

       group = new Group(list,_RCT("RC::Panel"),instance,options);
       db.geometry.add(group);

       db.saveundo(UD_ENDBLOCK,NULL);

    }

}

void CreatePanel(Plane *plane,Point zaxis,double thickness)
{Transform t;
 Plane *p1,*p2;
 EntityList list,glist,emptylist,list1,list2;
 Entity *e1,*e2,*e3,*e4;
 RuledSurface *ruledsurface;
 BitMask options(32);
 Group *group;
 int id;
 RCCHAR instance[300];
 Entity *e;

  if (zaxis.dot(plane->getxaxis().cross(plane->getyaxis())) < 0.0)
    thickness = -thickness;

  t.translate(plane->getxaxis().cross(plane->getyaxis()) * thickness);
  db.geometry.clearstatus();
  p1 = (Plane *)plane->clone(t);
  if (p1 == 0)
    return;

  db.saveundo(UD_STARTBLOCK,NULL);

  if (p1->getdefined())
    db.geometry.add(p1);

  list1 = plane->getlist();
  list2 = p1->getlist();

  e3 = 0; e4 = 0;
  for (list1.start(), list2.start() ; ! list1.atend() && ! list2.atend() ; )
    {  e1 = list1.next();
       e2 = list2.next();

       if (e1 != 0 && e2 != 0)
         {  e1->setvisible(*plane->getvisible());
            e2->setvisible(*plane->getvisible());

            db.geometry.add(e3 = new Line(((LinearEntity *)e1)->position(0.0),((LinearEntity *)e2)->position(0.0)));
            db.geometry.add(e4 = new Line(((LinearEntity *)e1)->position(1.0),((LinearEntity *)e2)->position(1.0)));

            glist.add(e1);
            glist.add(e2);
            glist.add(e3);
            glist.add(e4);

            if (e1->isa(line_entity) && e2->isa(line_entity))
              {  list = emptylist;
                 list.add(e1);
                 list.add(e4);
                 list.add(e2);
                 list.add(e3);
                 p2 = new Plane(list,10.0,10.0,0.0,90.0,0.0,0.0,0.0,0.0,0.0,RGB(0,0,0),0,options);
                 if (plane->getdefined()) 
                   {  db.geometry.add(p2);
                      glist.add(p2);
                   }
              }
            else
              {  ruledsurface = new RuledSurface((LinearEntity *)e1,(LinearEntity *)e2,2,2,BitMask(32));
                 if (ruledsurface->getdefined()) 
                   {  db.geometry.add(ruledsurface);
                      glist.add(ruledsurface);
                   }
              }

         }
    }

  glist.add(plane);
  glist.add(p1);
  options.set(0);

  id = 0;
  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"RC::Panel") == 0)
      {  if (strstr(((Group *)e)->getinstance(),"ID=") != 0 && atoi(strstr(((Group *)e)->getinstance(),"ID=") + 3) > id)
           id = atoi(strstr(((Group *)e)->getinstance(),"ID=") + 3);
      }
  sprintf(instance,"ID=%d",id + 1);

  group = new Group(glist,_RCT("RC::Panel"),instance,options);
  db.geometry.add(group);

  db.saveundo(UD_ENDBLOCK,NULL);


}



static int calculate_offset(Point p1,Point zaxis,double offset,double thickness,Point dir1,Point dir2,Point *verts)
{Point n1,n2,n3;
 double cosa;

  n1 = (zaxis.cross(dir1)).normalize();
  if (n1.length() < db.getptoler()) 
    return 0;
  n2 = (zaxis.cross(dir2)).normalize();
  if (n2.length() < db.getptoler()) 
    return 0;

  n3 = (n1 + n2).normalize();
  cosa = n1.dot(n2);
  if (cosa < -1.0) 
    cosa = -1.0; 
  else if (cosa > 1.0) 
    cosa = 1.0;
  cosa = cos(acos(cosa) / 2.0);
  if (fabs(cosa) < db.getatoler())
    return 0;

  n3 /= fabs(cosa);
  verts[0] = p1 + n3 * offset;
  verts[1] = p1 + n3 * (offset + thickness);

  return 1;

}

static int calculate_offsets(RBPanel1_data *rbdata)
{Point dir1,dir2;
 int i;
  if (rbdata->count == 2)
    {  dir1 = (rbdata->p2-rbdata->p1).normalize();
       if (! calculate_offset(rbdata->p1,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir1,rbdata->v1)) 
         return 0;
       if (! calculate_offset(rbdata->p2,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir1,rbdata->v2)) 
         return 0;
    }
  else if (rbdata->count > 2)
    {  if (rbdata->closed)
         {  dir1 = (rbdata->p1-rbdata->plast).normalize();
            dir2 = (rbdata->p2-rbdata->p1).normalize();
            if (! calculate_offset(rbdata->p1,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir2,rbdata->v1)) 
              return 0;
            dir1 = (rbdata->p2-rbdata->p1).normalize();
            dir2 = (rbdata->pi1-rbdata->p2).normalize();
            if (! calculate_offset(rbdata->p2,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir2,rbdata->v2))
              return 0;
            dir1 = (rbdata->pi1-rbdata->p2).normalize();
            dir2 = (rbdata->pi2-rbdata->pi1).normalize();
            if (! calculate_offset(rbdata->pi1,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir2,rbdata->v3))
              return 0;
            if (rbdata->count == 3)
              {  for (i = 0 ; i < 2 ; i++)
                   {  rbdata->v4[i] = rbdata->v1[i];
                      rbdata->vlast[i] = rbdata->v3[i];
                   }
              }
         }
       else
         {  dir1 = (rbdata->p1-rbdata->plast).normalize();
            dir2 = (rbdata->p2-rbdata->p1).normalize();
            if (! calculate_offset(rbdata->p1,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir2,rbdata->v1))
              return 0;
            dir1 = (rbdata->p2-rbdata->p1).normalize();
            if (! calculate_offset(rbdata->p2,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir1,rbdata->v2))
              return 0;
         }
    }
  return 1;
}

void RBPanel1(int,void *data,Point *p2,View3dSurface *)
{RBPanel1_data *rbdata = (RBPanel1_data *) data;

  rbdata->p2 = *p2;
  if (calculate_offsets(rbdata))
    {  CreatePanel(rbdata->v1[0],rbdata->v2[0],rbdata->v1[1],rbdata->v2[1],rbdata->zaxis * rbdata->height,1);
       if (rbdata->count > 2)
         CreatePanel(rbdata->vlast[0],rbdata->v1[0],rbdata->vlast[1],rbdata->v1[1],rbdata->zaxis * rbdata->height,1);
       if (rbdata->count > 3)
         CreatePanel(rbdata->v3[0],rbdata->v4[0],rbdata->v3[1],rbdata->v4[1],rbdata->zaxis * rbdata->height,1);
    }

}



void panel_command_insert(int *cstate,HighLevelEvent *event,void **data)
{Dialog dialog("InsertPanel_Dialog");
 DialogControl *dc;
 Point dir1;
 int i,insertmode;
 RBPanel1_data *rbdata = (RBPanel1_data *) *data;
 EntityList list,planelist,elist;
 Entity *panel,*b;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 Point origin,xaxis,yaxis;
 BitMask options(32);
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;
 Plane *p;

  switch (*cstate)
    {  case InitialState :

         rbdata = new RBPanel1_data;
         *data = rbdata;

         insertmode = v.getinteger("pa::insertmode");
         rbdata->thickness = v.getreal("pa::thickness");
         rbdata->height = v.getreal("pa::height");
         rbdata->offset = v.getreal("pa::offset");
         rbdata->closed = v.getinteger("pa::closed");

         dialog.add(new IPRadioButtonDialogControl(100,100,101,&insertmode));
         dialog.add(new IPRadioButtonDialogControl(101,100,101,&insertmode));
         dialog.add(dc = new RealDialogControl(102,&rbdata->thickness,ScaleByLength));
         dialog.add(new ScrollBarDialogControl(1102,dc));
         dialog.add(dc = new RealDialogControl(103,&rbdata->height,ScaleByLength));
         dialog.add(new ScrollBarDialogControl(1103,dc));
         dialog.add(dc = new RealDialogControl(104,&rbdata->offset,ScaleByLength));
         dialog.add(new ScrollBarDialogControl(1104,dc));
         dialog.add(dc = new CheckBoxDialogControl(105,&rbdata->closed));

         if (dialog.process() == TRUE)
           {  v.setinteger("pa::insertmode",insertmode);
              v.setinteger("pa::closed",rbdata->closed);
              v.setreal("pa::thickness",rbdata->thickness);
              v.setreal("pa::height",rbdata->height);
              v.setreal("pa::offset",rbdata->offset);
              if (insertmode == 0)
                *cstate = 1;
              else
                *cstate = 10;

           } 
         else
           *cstate = ExitState;
        
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->pi1 = rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              rbdata->zaxis = ((Coordinate3dEvent *) event)->getxaxis().cross(((Coordinate3dEvent *) event)->getyaxis());
              rbdata->count = 2;
              *cstate = 2;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->p1 = ((Line *)((EntitySelectedEvent *) event)->getentity())->getp1();
              rbdata->p2 = ((Line *)((EntitySelectedEvent *) event)->getentity())->getp2();
              rbdata->zaxis = ((EntitySelectedEvent *) event)->getxaxis().cross(((EntitySelectedEvent *) event)->getyaxis());

              dir1 = (rbdata->p2-rbdata->p1).normalize();
              if (calculate_offset(rbdata->p1,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir1,rbdata->v1) &&
                  calculate_offset(rbdata->p2,rbdata->zaxis,rbdata->offset,rbdata->thickness,dir1,dir1,rbdata->v2))
                {  CreatePanel(rbdata->v1[0],rbdata->v2[0],rbdata->v1[1],rbdata->v2[1],rbdata->zaxis * rbdata->height,0);
                }
              *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              if (rbdata->count == 2) rbdata->pi2 = rbdata->p2;
              if (calculate_offsets(rbdata))
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   if (rbdata->closed && rbdata->count > 3 ||
                       ! rbdata->closed && rbdata->count > 2)
                     CreatePanel(rbdata->vlast[0],rbdata->v1[0],rbdata->vlast[1],rbdata->v1[1],rbdata->zaxis * rbdata->height,0);
                   db.saveundo(UD_ENDBLOCK,NULL);
                   for (i = 0 ; i < 2 ; i++)
                     {  rbdata->vlast[i] = rbdata->v1[i];
                        rbdata->v1[i] = rbdata->v2[i];
                     }
                   rbdata->count++;
                   if (rbdata->count > 3) rbdata->plast1 = rbdata->plast;
                   rbdata->plast = rbdata->p1;
                   rbdata->p1 = rbdata->p2;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  if (rbdata->closed && rbdata->count > 3 ||
                  ! rbdata->closed && rbdata->count > 2)
                {  rbdata->count--;
                   if (rbdata->closed)
                     {  rbdata->p2 = rbdata->p1;  rbdata->p1 = rbdata->plast;  rbdata->plast = rbdata->plast1;
                     }
                   else
                     rbdata->p2 = rbdata->p1 + (rbdata->p1 - rbdata->plast);
                   if (calculate_offsets(rbdata))
                     {  db.saveundo(UD_STARTBLOCK,NULL);

                        if (rbdata->closed)
                          {  CreatePanel(rbdata->v1[0],rbdata->v2[0],rbdata->v1[1],rbdata->v2[1],rbdata->zaxis * rbdata->height,0);
                             CreatePanel(rbdata->v2[0],rbdata->v3[0],rbdata->v2[1],rbdata->v3[1],rbdata->zaxis * rbdata->height,0);
                             if (rbdata->count > 3)
                               CreatePanel(rbdata->v3[0],rbdata->v4[0],rbdata->v3[1],rbdata->v4[1],rbdata->zaxis * rbdata->height,0);
                          }
                        else
                          {  CreatePanel(rbdata->vlast[0],rbdata->v1[0],rbdata->vlast[1],rbdata->v1[1],rbdata->zaxis * rbdata->height,0);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }
                }
              if (event->isa(Abort))
                *cstate = 1;
              else
                *cstate = ExitState;
           }
         break;
       case 10 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              CreatePanel((Plane *)((EntitySelectedEvent *) event)->getentity(),((EntitySelectedEvent *) event)->getxaxis().cross(((EntitySelectedEvent *) event)->getyaxis()),rbdata->thickness);
              *cstate = 10;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }  

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the start of the panels or locate the line that defines the panel");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the end of the panel");
       rubberband.begin(0);
       rubberband.add(RBPanel1,rbdata);
    }
  else if (*cstate == 10)
    {  list = state.getselection().copy();

       if (list.length() != 0)
         {  db.header.setvisible(state.getlastvisible());

            for (list.start() ; (panel = list.next()) != NULL ; )
              if (panel->isa(plane_entity))
                {  list.del(panel);  
                   planelist.add(panel);
                   elist = ((Plane *)panel)->getlist();
                   for (elist.start() ; ! elist.atend() ; )
                     if ((b = elist.next()) != 0 && ! list.inlist(b))
                       list.add(b);
                }
              else if (! panel->islinear())
                {  list.destroy();
                   break;
                }

            Loops loops(1);
            loops.create(&list);
            if (list.empty() && !loops.getloops().empty())
              {  Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                 Plane *plane = new Plane(loops.getloops(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                 if (plane->getdefined())
                   {EntityList list;
                      state.destroyselection(1);
                      for (planelist.start() ; (p = (Plane *)planelist.next()) != 0 ; )
                        db.geometry.del(p);
                      db.geometry.add(plane);
                      db.geometry.toback(plane);
                      list = plane->getlist();
                      for (list.start() ; ! list.atend() ; )
                        {  if ((panel = list.next()) != 0)
                             panel->draw(DM_NORMAL);
                        }
                      CreatePanel(plane,Point(0.0,0.0,1.0),rbdata->thickness);
                   }
                 else
                   {  planelist.destroy();
                      delete plane;
                  }
              }
         }


       state.selmask.entity.clearandset(plane_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the plane that defines the panel");
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0)
         delete rbdata;
    }

}


Entity *RemoveVertex(Plane *p,LinearEntity *edge1,LinearEntity *edge2)
{Point p1,p2,p3;
 Line *line;

  if ((edge1->start() - edge2->start()).length() < db.getptoler())
    {  p1 = edge1->end();
       p2 = edge2->end();
       p3 = edge1->start();
    }
  else if ((edge1->end() - edge2->start()).length() < db.getptoler())
    {  p1 = edge1->start();
       p2 = edge2->end();
       p3 = edge1->end();
    }
  else if ((edge1->end() - edge2->end()).length() < db.getptoler())
    {  p1 = edge1->start();
       p2 = edge2->start();
       p3 = edge1->end();
    }
  else if ((edge1->start() - edge2->end()).length() < db.getptoler())
    {  p1 = edge1->end();
       p2 = edge2->start();
       p3 = edge1->start();
    }

  db.geometry.add(line = new Line(p1,p2)); 
  p->ReplaceEdges(line,p3);
  p->ReCalculatePlane();

  return line;

}

static void DeleteAssociated(Entity *e,void *group)
{ ((Group *)group)->del(e);
}

static void AddChildren(Entity *e,void *children)
{ ((EntityList *)children)->add(e);
}

void panel_command_delete_vertex(int *cstate,HighLevelEvent *event,void **)
{static Entity *panel;
 Point p1,p2,p3;
 int i,ntop,nbottom,nedgestop,nedgesbottom,n;
 Entity *e1,*e2,*edge[4],*newedge[2];
 EntityList list1,list2,children,edgelist;
 Entity *top,*bottom;
 Point zaxistop,zaxisbottom;

  switch (*cstate)
    {  case InitialState :

         list1 = state.getselection();
         list1.start();
         panel = list1.next();   

         if (panel->isa(group_entity) && strcmp(((Group *)panel)->getclassname(),"RC::Panel") == 0)
           {  *cstate = 1;
           }
         else
           *cstate = ExitState;

         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  p1 = ((Coordinate3dEvent *) event)->getp();
              list1 = ((Group *)panel)->getlist();
              list1.start();
              top = list1.next();
              bottom = list1.next();


              list1 = ((Group *)panel)->getlist();

              //  Search for a line one end of which is the point just located, and the other on the opposite side of the surface
              n = 0;
              for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 ; i++)
                if (e1->islinear() && 
                       ( (((LinearEntity *)e1)->start() - p1).length() < db.getptoler() || 
                         (((LinearEntity *)e1)->end() - p1).length() < db.getptoler() ) &&
                         ( ((Plane *)top)->LiesOn( ((LinearEntity *)e1)->start() ) &&  ((Plane *)bottom)->LiesOn( ((LinearEntity *)e1)->end() ) ||
                           ((Plane *)top)->LiesOn( ((LinearEntity *)e1)->end() ) &&  ((Plane *)bottom)->LiesOn( ((LinearEntity *)e1)->start() )) )
                  {  if ((((LinearEntity *)e1)->start() - p1).length() < db.getptoler())
                       p2 = ((LinearEntity *)e1)->end();
                     else
                       p2 = ((LinearEntity *)e1)->start();
                     n++;
                  }

              if (n == 0)
                {  *cstate = ExitState;
                   break;
                }     

              db.saveundo(UD_STARTBLOCK,NULL);

              ntop = nbottom = 0;
              if (((Plane *)bottom)->LiesOn(p1))
                {  p3 = p1;  p1 = p2;  p2 = p3;
                }

              //  Find the edges connected to p1 and p2
              for (list1.start() ; (e1 = list1.next()) != 0 ; i++)
                if (e1->islinear())
                  if ((p1 - ((LinearEntity *)e1)->start()).length() < db.getptoler() ||  
                      (p1 - ((LinearEntity *)e1)->end()).length() < db.getptoler() ||
                      (p2 - ((LinearEntity *)e1)->start()).length() < db.getptoler() ||  
                      (p2 - ((LinearEntity *)e1)->end()).length() < db.getptoler())
                    edgelist.add(e1);

              //  Remove the planes and ruled surfaces associated with these edges - except if it is a top or bottom face or the group
              db.saveundo(UD_STARTBLOCK,NULL);
              for (edgelist.start() ; (e1 = edgelist.next()) != 0 ; )
                {  list2 = e1->getalist();
                   for (list2.start() ; (e2 = list2.next()) != 0 ; )
                     if (e2 != top && e2 != bottom && e2 != panel)
                       {  ((Group *)panel)->del(e2);
                          db.geometry.del(e2);
                       }   
                }
              db.saveundo(UD_ENDBLOCK,NULL);

              nedgestop = nedgesbottom = 0;
              for (edgelist.start() ; (e1 = edgelist.next()) != 0 ; )
                if (((Plane *)top)->getlist().inlist(e1) && nedgestop < 2)
                  edge[nedgestop++] = e1;
                else if (((Plane *)bottom)->getlist().inlist(e1) && nedgesbottom < 2)
                  edge[2+nedgesbottom++] = e1;

              newedge[0] = newedge[1] = 0;
              if (nedgestop == 2 && nedgesbottom == 2)
                {  db.saveundo(UD_STARTBLOCK,NULL);
                   newedge[0] = RemoveVertex((Plane *)top,(LinearEntity *)edge[0],(LinearEntity *)edge[1]);
                   db.saveundo(UD_ENDBLOCK,NULL);
                   db.saveundo(UD_STARTBLOCK,NULL);
                   newedge[1] = RemoveVertex((Plane *)bottom,(LinearEntity *)edge[2],(LinearEntity *)edge[3]);
                   db.saveundo(UD_ENDBLOCK,NULL);
                }

              db.saveundo(UD_STARTBLOCK,NULL);
              for (edgelist.start() ; (e1 = edgelist.next()) != 0 ; )
                if (e1->getalist().length() == 1)  //  Only a member of the group
                  {  ((Group *)panel)->del(e1);
                     db.geometry.del(e1);
                  }   
              db.saveundo(UD_ENDBLOCK,NULL);

              if (newedge[0] != 0 && newedge[1] != 0)
                {BitMask o(32);
                 RuledSurface *rs;

                   db.saveundo(UD_STARTBLOCK,NULL);
                   if (((LinearEntity *)newedge[0])->direction(0.0).dot(((LinearEntity *)newedge[1])->direction(0.0)) < 0.0)
                     o.set(0); 
                   db.geometry.add(rs = new RuledSurface((LinearEntity *)newedge[0],(LinearEntity *)newedge[1],2,2,o));
                   db.saveundo(UD_ENDBLOCK,NULL);

                   db.saveundo(UD_STARTBLOCK,NULL);
                   ((Group *)panel)->add(0x7fffffff,newedge[0]);
                   ((Group *)panel)->add(0x7fffffff,newedge[1]);
                   ((Group *)panel)->add(0x7fffffff,rs);
                   db.saveundo(UD_ENDBLOCK,NULL);

                }  

              db.saveundo(UD_ENDBLOCK,NULL);

              cadwindow->invalidatedisplaylist(top);
              cadwindow->invalidatedisplaylist(bottom);
              cadwindow->paintall(); 

           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }  

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the point to delete from the panel");
    }
  else if (*cstate == ExitState)
    {  
    }

}

struct RBPanel2_data
  {Point p1,p2;
   Point p3,p4;
   Entity *surface1,*surface2;
  };

void RBPanel2(int dm,void *data,Point *p2,View3dSurface *v)
{RBPanel2_data *rbdata = (RBPanel2_data *) data;

  Line line1(rbdata->p1,*p2);
  Line line2(*p2,rbdata->p2);
  line1.draw(DM_INVERT);
  line2.draw(DM_INVERT);

  Line line3(*p2,*p2 + rbdata->p4 - rbdata->p2);
  line3.draw(DM_INVERT);

}


void panel_command_insert_vertex(int *cstate,HighLevelEvent *event,void **data)
{static RBPanel2_data rbdata;
 static Entity *panel,*epicked;
 static Workplane *lastworkplane;
 Point p1,p2,ppicked;
 int i;
 Entity *e1,*e2,*parent1,*parent2,*newedge[4];
 EntityList list1,list2,children;
 Entity *top,*bottom;
 Point zaxistop,zaxisbottom;
 Line *line1,*line2,*line3,*line4,*line5;
 double t;

  switch (*cstate)
    {  case InitialState :

         *data = &rbdata;

         lastworkplane = db.workplanes.getcurrent();

         list1 = state.getselection();
         list1.start();
         panel = list1.next();   

         epicked = 0;
         ppicked = state.getlastpickpoint();

         //  Search for the boundary entity that was selected
         list1 = ((Group *)panel)->getlist();
         list1.start();
         top = list1.next();
         bottom = list1.next();

         //  Locate the edge that was used to select the panel
         for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 && i < 2 ; i++)
           if (e1->isa(plane_entity))
             {  list2 = ((Plane *)e1)->getlist();
                for (list2.start() ; ! list2.atend() ; )
                  {  e2 = list2.next();
                     if (e2 != 0 && (e2->nearp(ppicked) - ppicked).length() < db.getptoler() * 100.0)
                       {  t = ((LinearEntity *)e2)->gett(e2->nearp(ppicked));
                          if (t >= -0.001 && t <= 1.001)
                            epicked = e2;
                       } 
                  } 
             }

         if (epicked == 0)
           { RCMessageBox(cadwindow->gethwnd(),"Locate the panel on the edge you want to replace","Insert vertex",MB_OK);
              *cstate = ExitState;
              break;
           }

         rbdata.p1 = ((LinearEntity *)epicked)->start();
         rbdata.p2 = ((LinearEntity *)epicked)->end();

         //  Locate the edge on the other face that corresponds to this edge
         for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 ; i++)
           if (e1->islinear())
             {  
                if ( ((((LinearEntity *)e1)->start() - rbdata.p1).length() < db.getptoler() || 
                     (((LinearEntity *)e1)->end() - rbdata.p1).length() < db.getptoler()) &&

                     (((Plane *)top)->LiesOn(((LinearEntity *)e1)->start()) && 
                      ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->end()) || 

                      ((Plane *)top)->LiesOn(((LinearEntity *)e1)->end()) && 
                      ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->start()) ))
                  {  if ((((LinearEntity *)e1)->start() - rbdata.p1).length() < db.getptoler())
                       rbdata.p3 = ((LinearEntity *)e1)->end();
                     else      
                       rbdata.p3 = ((LinearEntity *)e1)->start();
                  }

                if ( ((((LinearEntity *)e1)->start() - rbdata.p2).length() < db.getptoler() || 
                     (((LinearEntity *)e1)->end() - rbdata.p2).length() < db.getptoler()) &&

                     ( ((Plane *)top)->LiesOn(((LinearEntity *)e1)->start()) && 
                       ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->end()) || 

                       ((Plane *)top)->LiesOn(((LinearEntity *)e1)->end()) && 
                       ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->start()) ))
                  {  if ((((LinearEntity *)e1)->start() - rbdata.p2).length() < db.getptoler())
                       rbdata.p4 = ((LinearEntity *)e1)->end();
                     else      
                       rbdata.p4 = ((LinearEntity *)e1)->start();
                  }

             }

         if (panel->isa(group_entity) && strcmp(((Group *)panel)->getclassname(),"RC::Panel") == 0 && epicked != 0 && epicked->isa(line_entity))
           {  db.workplanes.setcurrent(db.workplanes.match("RC::Dynamic"));
              db.workplanes.getcurrent()->change(((Plane *)top)->getorigin(),((Plane *)top)->getxaxis(),((Plane *)top)->getyaxis());
              *cstate = 1;
           }
         else
           *cstate = ExitState;

         break;
       case 1 :
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  p1 = ((Coordinate3dEvent *) event)->getp();

              //  Search for the boundary entity that was selected
              list1 = ((Group *)panel)->getlist();
              list1.start();
              top = list1.next();
              bottom = list1.next();

              db.saveundo(UD_STARTBLOCK,NULL);

              db.saveundo(UD_STARTBLOCK,NULL);

              if (((Plane *)top)->LiesOn(rbdata.p1))
                {  
                   db.geometry.add(line1 = new Line(rbdata.p1,p1));
                   db.geometry.add(line2 = new Line(p1,rbdata.p2));
                   e1 = ((Plane *)top)->AddVertex(line1,line2,newedge);
                   db.geometry.add(line3 = new Line(rbdata.p3,p1 + rbdata.p3 - rbdata.p1));
                   db.geometry.add(line4 = new Line(p1 + rbdata.p3 - rbdata.p1,rbdata.p4));
                   e2 = ((Plane *)bottom)->AddVertex(line3,line4,newedge + 2);

                   db.geometry.add(line5 = new Line(p1 + rbdata.p3 - rbdata.p1,p1));

                }
              else
                {  db.geometry.add(line1 = new Line(rbdata.p1,p1 - rbdata.p3 + rbdata.p1));
                   db.geometry.add(line2 = new Line(p1 - rbdata.p3 + rbdata.p1,rbdata.p2));
                   e1 = ((Plane *)bottom)->AddVertex(line1,line2,newedge);

                   cadwindow->invalidatedisplaylist(bottom);
                   bottom->draw(DM_NORMAL);

                   db.geometry.add(line3 = new Line(rbdata.p3,p1));
                   db.geometry.add(line4 = new Line(p1,rbdata.p4));
                   e2 = ((Plane *)top)->AddVertex(line3,line4,newedge + 2);

                   db.geometry.add(line5 = new Line(p1 - rbdata.p3 + rbdata.p1,p1));

                }
              db.saveundo(UD_ENDBLOCK,NULL);

              list1 = e1->getalist();
              for (list1.start() ; (parent1 = list1.next()) != 0 ; )
                {  list2 = e2->getalist();
                   for (list2.start() ; (parent2 = list2.next()) != 0 ; )
                     if (parent1 == parent2 && ! parent1->isa(group_entity))
                       {  ((Group *)panel)->del(parent1);
                          db.saveundo(UD_STARTBLOCK,NULL);
                          parent1->dbtraverseassociated(DeleteAssociated,panel);
                          parent1->dbtraverseassociated(AddChildren,&children);
                          db.geometry.del(parent1);
                          db.saveundo(UD_ENDBLOCK,NULL);
                          for (children.start() ; (e1 = children.next()) != 0 ; )
                            {//  If the entity is one of the 'connecting' linese between front and back - add it back into the group
                               db.saveundo(UD_STARTBLOCK,NULL);
                               if (e1->islinear() && 
                                    ( (rbdata.p1 - ((LinearEntity *)e1)->start()).length() < db.getptoler() &&
                                      (rbdata.p3 - ((LinearEntity *)e1)->end()).length() < db.getptoler() ||
                                      (rbdata.p1 - ((LinearEntity *)e1)->end()).length() < db.getptoler() &&
                                      (rbdata.p3 - ((LinearEntity *)e1)->start()).length() < db.getptoler() ||
                                      (rbdata.p2 - ((LinearEntity *)e1)->start()).length() < db.getptoler() &&
                                      (rbdata.p4 - ((LinearEntity *)e1)->end()).length() < db.getptoler() ||
                                      (rbdata.p2 - ((LinearEntity *)e1)->end()).length() < db.getptoler() &&
                                      (rbdata.p4 - ((LinearEntity *)e1)->start()).length() < db.getptoler()))
                                 ((Group *)panel)->add(0x7fffffff,e1);
                               else   
                                 db.geometry.del(e1); 
                               db.saveundo(UD_ENDBLOCK,NULL);
                            }
                          children.destroy();
                          break; 
                       }
                   if (parent1 == parent2)
                     break; 
                }

              db.saveundo(UD_STARTBLOCK,NULL);
              RuledSurface *rs1,*rs2;                 
              BitMask o(32);
              if (line1->direction(0.0).dot(line3->direction(0.0)) < 0.0)
                o.set(0); 
              db.geometry.add(rs1 = new RuledSurface(line1,line3,2,2,o));
              if (line2->direction(0.0).dot(line4->direction(0.0)) < 0.0)
                o.set(0); 
              db.geometry.add(rs2 = new RuledSurface(line2,line4,2,2,o));
              db.saveundo(UD_ENDBLOCK,NULL);

              db.saveundo(UD_STARTBLOCK,NULL);
              ((Group *)panel)->add(0x7fffffff,line1);
              ((Group *)panel)->add(0x7fffffff,line2);
              ((Group *)panel)->add(0x7fffffff,line3);
              ((Group *)panel)->add(0x7fffffff,line4);
              ((Group *)panel)->add(0x7fffffff,line5);
              ((Group *)panel)->add(0x7fffffff,rs1);
              ((Group *)panel)->add(0x7fffffff,rs2);
              db.saveundo(UD_ENDBLOCK,NULL);
                          
              cadwindow->invalidatedisplaylist(top);
              cadwindow->invalidatedisplaylist(bottom);

              db.saveundo(UD_ENDBLOCK,NULL);

              cadwindow->paintall(); 

           }

         *cstate = ExitState;
         break;
    }  

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the point to add to the panel");
       rubberband.begin(0);
       rubberband.add(RBPanel2,&rbdata);
    }
  else if (*cstate == ExitState)
    {  db.workplanes.setcurrent(lastworkplane);
    }

}

void project_entities(Plane *plane,Point offset,EntityList *list)
{EntityList newlist;
 int i,nsegments;
 Point *polygon;
 double *knot,*weight;
 BitMask options(32);
 Entity *e;

  for (list->start() ; (e = list->next()) != 0 ; )
    {  if (e->isa(line_entity))
         {  if (plane->LiesOn(((Line *)e)->getp1() + offset) && plane->LiesOn(((Line *)e)->getp2() + offset))
              newlist.add(e);
            else
              newlist.add(new Line(*e,plane->nearp(((Line *)e)->getp1()) + offset,plane->nearp(((Line *)e)->getp2()) + offset));
         }
       else if (e->isa(curve_entity))
         {  for (i = 0 ; i < ((Curve *)e)->getn() ; i++)
              if (! plane->LiesOn(((Curve *)e)->getpolygon()[i] + offset))
                break;
            if (i == ((Curve *)e)->getn())
              newlist.add(e);
            else
              {  polygon = new Point[((Curve *)e)->getn()];
                 for (i = 0 ; i < ((Curve *)e)->getn() ; i++)
                   polygon[i] = plane->nearp(((Curve *)e)->getpolygon()[i]) + offset;
                 newlist.add(new Curve(*e,((Curve *)e)->getn(),((Curve *)e)->getdegree(),polygon,((Curve *)e)->getknot(),((Curve *)e)->getcweight(),((Curve *)e)->getoptions()));
              }
         }
       else if (e->isa(circle_entity))
         {  if (plane->LiesOn(((Circle *)e)->position(0.25) + offset) && plane->LiesOn(((Circle *)e)->position(0.5) + offset) && plane->LiesOn(((Circle *)e)->position(0.75) + offset))
              newlist.add(e);
            else
              {  nsegments = int(((Circle *)e)->getsweepa() / M_PI * 2.0 + 1.0);
                 if (nsegments < 0) 
                   nsegments = 1; 
                 else if (nsegments > 4) 
                   nsegments = 4;
                 polygon = new Point[nsegments*2+1];
                 weight = new double[nsegments*2+1];
                 knot = new double[nsegments*2+1+2+1];
                 if (polygon != 0 && weight != 0 && knot != 0)
                   {  for (i = 0 ; i < nsegments ; i++)
                        {  polygon[i*2] = plane->nearp(((Circle *)e)->position(double(i)/double(nsegments))) + offset;
                           polygon[i*2+2] = plane->nearp(((Circle *)e)->position(double(i+1)/double(nsegments))) + offset;
                           Line line1(polygon[i*2],polygon[i*2]+((Circle *)e)->direction(double(i)/double(nsegments)));
                           Line line2(polygon[i*2+2],polygon[i*2+2]+((Circle *)e)->direction(double(i+1)/double(nsegments)));
                           Intersect i1(&line1,&line2);
                           if (i1.nintersections() == 1)
                             polygon[i*2+1] = plane->nearp(((PointE *)i1.intersection(0))->getp()) + offset;
                           else
                             polygon[i*2+1] = plane->nearp((polygon[i*2] + polygon[i*2+2])/2.0) + offset;
                           weight[i*2] = 1.0;
                           weight[i*2+1] = cos(((Circle *)e)->getsweepa()/nsegments/2.0);
                           weight[i*2+2] = 1.0;
                           knot[i*2+1] = double(i) / double(nsegments);
                           knot[i*2+2] = double(i) / double(nsegments);
                        }
                      knot[0] = 0.0;
                      knot[nsegments*2+1] = 1.0;
                      knot[nsegments*2+1+1] = 1.0;
                      knot[nsegments*2+1+2] = 1.0;
                      newlist.add(new Curve(*e,nsegments*2+1,2,polygon,knot,weight,options));
                   }
                 delete [] polygon;  delete [] weight;  delete [] knot;
             }
      }
  }

  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    if (e->getposition() == 0)
      db.geometry.add(e);

  list->destroy();
  *list = newlist;


}

void panel_command_insert_hole(int *cstate,HighLevelEvent *event,void **data)
{static EntityList holelist;
 static RBPanel2_data rbdata;
 static Entity *panel,*epicked;
 static Workplane *lastworkplane;
 Point p1,p2,ppicked;
 int topcount,bottomcount,count;
 Entity *e1,*e2;
 EntityList list1,list2,children,holelist1,holelist2;
 Entity *top,*bottom;
 Point zaxistop,zaxisbottom;
 Dialog dialog("PanelHole_Dialog");
 DialogControl *dc;
 double holedepth,holedraftangle,thickness;
 double xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange;
 BitMask options(32);
 RCCHAR patternfilename[300];
 RCCOLORREF gradientcolour;
 RuledSurface *rs;
 Plane *plane1,*plane2;
 Line *line;

  switch (*cstate)
    {  case InitialState :

         holedepth = v.getreal("pa::holedepth");
         holedraftangle = v.getreal("pa::holedraftangle");

         dialog.add(dc = new RealDialogControl(100,&holedepth,ScaleByLength));
         dialog.add(new ScrollBarDialogControl(1100,dc));
         dialog.add(dc = new RealDialogControl(101,&holedraftangle,ScaleByLength));
         dialog.add(new ScrollBarDialogControl(1101,dc));

         if (dialog.process() == TRUE)
           {  v.setreal("pa::holedepth",holedepth);
              v.setreal("pa::holedraftangle",holedraftangle);
              list1 = state.getselection();
              list1.start();
              panel = list1.next();   
              ppicked = state.getlastpickpoint();
              *cstate = 1;
           } 
         else
           *cstate = ExitState;

         break;

       case 1 :
         if (event->isa(EntitySelected))
           {  holelist.add(((EntitySelectedEvent *) event)->getentity());
              ((EntitySelectedEvent *) event)->getentity()->highlight();
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  if (holelist.length() > 0)
                {Point origin,xaxis,yaxis,zaxis;
                 double D; 

                   holedepth = v.getreal("pa::holedepth");
                   holedraftangle = v.getreal("pa::holedraftangle");

                   list1 = ((Group *)panel)->getlist();
                   list1.start();
                   top = list1.next();
                   bottom = list1.next();

                   origin = (((Plane *)top)->getorigin() + ((Plane *)bottom)->getorigin()) / 2.0;
                   zaxis = ((Plane *)top)->getxaxis().cross(((Plane *)bottom)->getyaxis());
                   if (zaxis.dot(((Plane *)top)->getorigin()) < 0.0)
                     zaxis = -zaxis;

                   thickness = fabs(zaxis.dot(((Plane *)top)->getorigin() - ((Plane *)bottom)->getorigin()));
                   if (holedepth >= thickness - db.getptoler())
                     holedepth = thickness;

                   topcount = bottomcount = count = 0;

                   for (holelist.start() ; (e1 = holelist.next()) != 0 ; )
                     {  if (zaxis.dot(((LinearEntity *)e1)->start() - origin) > 0.0)
                          topcount++;
                        else
                          bottomcount++;
                       count++;
                     }

                   db.saveundo(UD_STARTBLOCK,NULL);

                   if (topcount > bottomcount)
                     {  holelist1 = holelist.copy();
                        holelist2 = holelist.copy();
                        project_entities(((Plane *)top),Point(0.0,0.0,0.0),&holelist1);
                        project_entities(((Plane *)top),-zaxis * holedepth,&holelist2);
                     }
                   else
                     {  holelist1 = holelist.copy();
                        holelist2 = holelist.copy();
                        project_entities(((Plane *)bottom),-zaxis * holedepth,&holelist1);
                        project_entities(((Plane *)bottom),Point(0.0,0.0,0.0),&holelist2);
                     }

                   holelist.unhighlight();
                   holelist.destroy();

                   Loops loops1(1,holelist1.copy());
                   Loops loops2(1,holelist2.copy());
                   if (loops1.planar(&origin,&xaxis,&yaxis,&zaxis,&D) && loops2.planar(&origin,&xaxis,&yaxis,&zaxis,&D))
                     {  //  Search for the boundary entity that was selected

                        plane1 = 0;
                        db.saveundo(UD_STARTBLOCK,NULL);
                        if (topcount > bottomcount || holedepth == thickness)
                          {  db.saveundo(UD_MOVED,top);
                             ((Plane *)top)->draw(DM_ERASE);
                             ((Plane *)top)->dbdelete();
                             list1 = loops1.getloops();
                             list2 = ((Plane *)top)->getlist().copy();
                             list2.reverse();
                             list2.add(0); 
                             for (list1.start() ; (e1 = list1.next()) != 0 ; )
                               list2.add(e1);
                             list2.reverse();
                             ((Plane *)top)->setlist(list2);
                             ((Plane *)top)->dbinsert(&db);
                             cadwindow->invalidatedisplaylist(top);
                             ((Plane *)top)->draw(DM_NORMAL);
                          }
                        else
                          {  Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                             plane1 = new Plane(loops1.getloops(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                             db.geometry.add(plane1);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);

                        plane2 = 0;
                        db.saveundo(UD_STARTBLOCK,NULL);
                        if (topcount <= bottomcount || holedepth == thickness)
                          {  
                             db.saveundo(UD_MOVED,bottom);
                             ((Plane *)bottom)->draw(DM_ERASE);
                             ((Plane *)bottom)->dbdelete();
                             if (holedraftangle == 0.0)
                               list1 = loops2.getloops();
                             else
                               {  loops2.offset(1,0,Point(0.0,0.0,0.0),holedepth * tan(holedraftangle * acos(-1.0) / 180.0),&list1);
                                  db.saveundo(UD_STARTBLOCK,NULL);
                                  for (holelist2.start() ; (e1 = holelist2.next()) != 0 ; )
                                    db.geometry.del(e1);
                                  db.saveundo(UD_ENDBLOCK,NULL);
                                  holelist2 = list1.copy();
                               }

                             list2 = ((Plane *)bottom)->getlist().copy();
                             list2.reverse();
                             list2.add(0); 
                             for (list1.start() ; (e1 = list1.next()) != 0 ; )
                               list2.add(e1);
                             list2.reverse();
                             ((Plane *)bottom)->setlist(list2);
                             ((Plane *)bottom)->dbinsert(&db);
                             cadwindow->invalidatedisplaylist(bottom);
                             ((Plane *)bottom)->draw(DM_NORMAL);
                          }
                        else
                          {  Plane::getdefaults(&xspacing,&yspacing,&angle,&crossangle,&transparency,&red,&green,&blue,&transparencyrange,&gradientcolour,patternfilename,&options);
                             plane2 = new Plane(loops2.getloops(),xspacing,yspacing,angle,crossangle,transparency,red,green,blue,transparencyrange,gradientcolour,patternfilename,options);
                             db.geometry.add(plane2);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);

                        db.saveundo(UD_STARTBLOCK,NULL);
                        if (plane1 != 0)
                          ((Group *)panel)->add(0x7fffffff,plane1);
                        if (plane2 != 0)
                          ((Group *)panel)->add(0x7fffffff,plane2);
                        db.saveundo(UD_ENDBLOCK,NULL);

                        db.saveundo(UD_STARTBLOCK,NULL);
                        for (holelist1.start(), holelist2.start() ; ! holelist1.atend() ; )
                          {BitMask o(32);
                             e1 = holelist1.next();
                             e2 = holelist2.next();
                             o.set(0,((LinearEntity *)e1)->direction(0.0).dot(((LinearEntity *)e2)->direction(0.0)) < 0.0);
                             db.geometry.add(rs = new RuledSurface(*e1,(LinearEntity *)e1,(LinearEntity *)e2,2,2,o));
                             db.geometry.add(line = new Line(*e1,((LinearEntity *)e1)->start(),o.test(0) ? ((LinearEntity *)e2)->end() : ((LinearEntity *)e2)->start()));
                             db.saveundo(UD_STARTBLOCK,NULL);
                             ((Group *)panel)->add(0x7fffffff,line);
                             ((Group *)panel)->add(0x7fffffff,e1);
                             ((Group *)panel)->add(0x7fffffff,e2);
                             ((Group *)panel)->add(0x7fffffff,rs);
                             db.saveundo(UD_ENDBLOCK,NULL);
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);

                     } 
                   else
                     {//  Delete these new entities
                        db.saveundo(UD_STARTBLOCK,NULL);
                        for (holelist1.start(), holelist2.start() ; ! holelist1.atend() ; )
                          {  db.geometry.del(holelist1.next());
                             db.geometry.del(holelist2.next());
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);

                     }


                   db.saveundo(UD_ENDBLOCK,NULL);

                }
              *cstate = ExitState;
           }
         break;
    }  

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the hole entity to add to the panel");
    }
  else if (*cstate == ExitState)
    { 
    }

}

void find_connected(Group *group,Point p1)
{EntityList list1,list2;
 Entity *e1,*e2;

  list1 = group->getlist();
  for (list1.start() ; (e1 = list1.next()) != 0 ; )
    if (e1->islinear() && e1->getstatus() == 0)
      {  if ((((LinearEntity *)e1)->start() - p1).length() < db.getptoler())
           {  e1->setstatus(1);
              list2 = e1->getalist();
              for (list2.start()  ; (e2 = list2.next()) != 0 ; )
                if (e2 != group) 
                  e2->setstatus(1); 
              find_connected(group,((LinearEntity *)e1)->end());
           }
         else if ((((LinearEntity *)e1)->end() - p1).length() < db.getptoler())
           {  e1->setstatus(1);
              list2 = e1->getalist();
              for (list2.start()  ; (e2 = list2.next()) != 0 ; )
                if (e2 != group) 
                  e2->setstatus(1);
              find_connected(group,((LinearEntity *)e1)->start());
           }
      }

}


void panel_command_delete_hole(int *cstate,HighLevelEvent *event,void **data)
{EntityList list1,toplist,bottomlist;
 Entity *e,*e1,*epicked;
 Plane *top,*bottom;
 int ndeleted;
 Point ppicked;
 double t;

  list1 = state.getselection();
  list1.start();
  e = list1.next(); 

  list1 = ((Group *)e)->getlist();
  list1.start();
  top = (Plane *)list1.next();
  bottom = (Plane *)list1.next();

  epicked = 0;
  ppicked = state.getlastpickpoint();

  //  Locate the edge that was used to select the panel
  for (list1.start() ; (e1 = list1.next()) != 0 ; )
    if (e1->islinear() && (e1->nearp(ppicked) - ppicked).length() < db.getptoler() * 100.0)
      {  t = ((LinearEntity *)e1)->gett(e1->nearp(ppicked));
         if (t >= -0.001 && t <= 1.001)
           epicked = e1;
      } 

  if (epicked == 0)
    { RCMessageBox(cadwindow->gethwnd(),"The hole could not be located","Delete Hole",MB_OK);
       *cstate = ExitState;
       return;
    }

  state.destroyselection(1);

  //  Find all linear entities that are connected to this one.
  db.geometry.clearstatus();
  find_connected((Group *)e,((LinearEntity *)epicked)->start());

  list1 = ((Plane *)top)->getlist();
  for (list1.start() ; ! list1.atend() ; )
    {  e1 = list1.next();
       if (e1 != 0 && e1->getstatus() == 0)
          break;
    }

  if (list1.atend())
    {  list1 = ((Plane *)bottom)->getlist();
       for (list1.start() ; ! list1.atend() ; )
         {  e1 = list1.next();
            if (e1 != 0 && e1->getstatus() == 0)
               break;
         }
    } 
  
  if (list1.atend())
    {  *cstate = ExitState;
       return;  //  Can't allow the top or bottom to be deleted.
    }

  db.saveundo(UD_STARTBLOCK,NULL);

  db.saveundo(UD_STARTBLOCK,NULL);
  db.saveundo(UD_MOVED,top);
  db.saveundo(UD_MOVED,bottom);
  db.saveundo(UD_ENDBLOCK,NULL);

  ((Plane *)top)->draw(DM_ERASE);
  ((Plane *)top)->dbdelete();
  ((Plane *)bottom)->draw(DM_ERASE);
  ((Plane *)bottom)->dbdelete();


  db.saveundo(UD_STARTBLOCK,NULL);
  list1 = ((Group *)e)->getlist().copy();

  toplist = ((Plane *)top)->getlist();
  bottomlist = ((Plane *)bottom)->getlist();

  for (list1.start() ; (e1 = list1.next()) != 0 ; )
    if (e1->getstatus() && e1 != e && e1 != top && e1 != bottom)
      {  ((Group *)e)->del(e1);
         toplist.del(e1);
         bottomlist.del(e1);
      }
  ((Plane *)top)->setlist(toplist);
  ((Plane *)bottom)->setlist(bottomlist);

  db.saveundo(UD_ENDBLOCK,NULL);

  db.saveundo(UD_STARTBLOCK,NULL);
  ndeleted = 1;
  while (ndeleted > 0)
    {  ndeleted = 0;
       for (list1.start() ; (e1 = list1.next()) != 0 ; )
         if (e1->getstatus() && e1 != e && e1 != top && e1 != bottom && e1->getalist().length() == 0)
           {  list1.del(e1);
              db.geometry.del(e1);
              ndeleted++; 
           }
    }
  db.saveundo(UD_ENDBLOCK,NULL);

  db.saveundo(UD_ENDBLOCK,NULL);

  list1.destroy();

  ((Plane *)top)->dbinsert(&db);
  cadwindow->invalidatedisplaylist(top);
  ((Plane *)top)->draw(DM_NORMAL);

  ((Plane *)bottom)->dbinsert(&db);
  cadwindow->invalidatedisplaylist(bottom);
  ((Plane *)bottom)->draw(DM_NORMAL);

  *cstate = ExitState;
       
}


void panel_command_align(int *cstate,HighLevelEvent *event,void **data)
{static RBPanel2_data rbdata;
 static Entity *panel,*epickedtop,*epickedbottom;
 static Workplane *lastworkplane;
 Point p1,p2,pnew[4],pold[4],ppicked;
 int i,intersectmask;
 Entity *e1,*e2;
 EntityList list1,list2,children;
 Entity *top,*bottom;
 Point zaxistop,zaxisbottom;
 Line *line;
 double t;

  switch (*cstate)
    {  case InitialState :

         list1 = state.getselection();
         list1.start();
         panel = list1.next();   

         epickedtop = 0;
         ppicked = state.getlastpickpoint();

         //  Search for the boundary entity that was selected
         list1 = ((Group *)panel)->getlist();
         list1.start();
         top = list1.next();
         bottom = list1.next();

         //  Locate the edge that was used to select the panel
         for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 && i < 2 ; i++)
           if (e1->isa(plane_entity))
             {  list2 = ((Plane *)e1)->getlist();
                for (list2.start() ; ! list2.atend() ; )
                  {  e2 = list2.next();
                     if (e2 != 0 && (e2->nearp(ppicked) - ppicked).length() < db.getptoler() * 100.0)
                       {  t = ((LinearEntity *)e2)->gett(e2->nearp(ppicked));
                          if (t >= -0.001 && t <= 1.001)
                            epickedtop = e2;
                       } 
                  } 
             }

         if (epickedtop == 0)
           { RCMessageBox(cadwindow->gethwnd(),"Locate the panel on the edge you want to align","Align Edge",MB_OK);
              *cstate = ExitState;
              break;
           }

         rbdata.p1 = ((LinearEntity *)epickedtop)->start();
         rbdata.p2 = ((LinearEntity *)epickedtop)->end();

         //  Locate the edge on the other face that corresponds to this edge
         for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 ; i++)
           if (e1->islinear())
             {  
                if ( ((((LinearEntity *)e1)->start() - rbdata.p1).length() < db.getptoler() || 
                     (((LinearEntity *)e1)->end() - rbdata.p1).length() < db.getptoler()) &&

                     (((Plane *)top)->LiesOn(((LinearEntity *)e1)->start()) && 
                      ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->end()) || 

                      ((Plane *)top)->LiesOn(((LinearEntity *)e1)->end()) && 
                      ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->start()) ))
                  {  if ((((LinearEntity *)e1)->start() - rbdata.p1).length() < db.getptoler())
                       rbdata.p3 = ((LinearEntity *)e1)->end();
                     else      
                       rbdata.p3 = ((LinearEntity *)e1)->start();
                  }

                if ( ((((LinearEntity *)e1)->start() - rbdata.p2).length() < db.getptoler() || 
                     (((LinearEntity *)e1)->end() - rbdata.p2).length() < db.getptoler()) &&

                     ( ((Plane *)top)->LiesOn(((LinearEntity *)e1)->start()) && 
                       ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->end()) || 

                       ((Plane *)top)->LiesOn(((LinearEntity *)e1)->end()) && 
                       ((Plane *)bottom)->LiesOn(((LinearEntity *)e1)->start()) ))
                  {  if ((((LinearEntity *)e1)->start() - rbdata.p2).length() < db.getptoler())
                       rbdata.p4 = ((LinearEntity *)e1)->end();
                     else      
                       rbdata.p4 = ((LinearEntity *)e1)->start();
                  }

             }

         epickedbottom = 0;
         for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 ; i++)
           if (e1->islinear() && 
                  ( (((LinearEntity *)e1)->start() - rbdata.p3).length() < db.getptoler() && 
                    (((LinearEntity *)e1)->end() - rbdata.p4).length() < db.getptoler() ||
                    (((LinearEntity *)e1)->start() - rbdata.p4).length() < db.getptoler() && 
                    (((LinearEntity *)e1)->end() - rbdata.p3).length() < db.getptoler()))
             epickedbottom = e1;

         if (panel->isa(group_entity) && strcmp(((Group *)panel)->getclassname(),"RC::Panel") == 0 && epickedtop != 0 && epickedtop->isa(line_entity) && epickedbottom != 0 && epickedbottom->isa(line_entity))
           {  

              if (((Plane *)top)->LiesOn(((LinearEntity *)epickedbottom)->start()))
                {  e1 = epickedbottom;
                   epickedbottom = epickedtop;
                   epickedtop = e1;
                }

              *cstate = 1;
           }
         else
           *cstate = ExitState;

         break;
       case 1 :

         if (event->isa(EntitySelected))
           {  line = (Line *)((EntitySelectedEvent *) event)->getentity();

              list1 = ((Group *)panel)->getlist();
              list1.start();
              top = list1.next();
              bottom = list1.next();

              p1 = ((Plane *)top)->nearp(line->getp1());
              p2 = ((Plane *)top)->nearp(line->getp2()); 
              if ((p2 - p1).dot(((Line *)epickedtop)->getp2() - ((Line *)epickedtop)->getp2()) < 0.0)
                {  p1 = ((Plane *)top)->nearp(line->getp2());
                   p2 = ((Plane *)top)->nearp(line->getp1()); 
                }

              pnew[0] = (((Line *)epickedtop)->getp1() + ((Line *)epickedtop)->getp2()) / 2.0 - (p2 - p1) / 2.0;
              pnew[1] = (((Line *)epickedtop)->getp1() + ((Line *)epickedtop)->getp2()) / 2.0 + (p2 - p1) / 2.0;
              Line line1(pnew[0],pnew[1]);

              pnew[2] = (((Line *)epickedbottom)->getp1() + ((Line *)epickedbottom)->getp2()) / 2.0 - (p2 - p1) / 2.0;
              pnew[3] = (((Line *)epickedbottom)->getp1() + ((Line *)epickedbottom)->getp2()) / 2.0 + (p2 - p1) / 2.0;
              Line line2(pnew[2],pnew[3]);

              pold[0] = ((Line *)epickedtop)->getp1();
              pold[1] = ((Line *)epickedtop)->getp2();
              pold[2] = ((Line *)epickedbottom)->getp1();
              pold[3] = ((Line *)epickedbottom)->getp2();

              intersectmask = 0;

              //  Find the edges on the top and bttom face that connect to these lines and trim them
              list1 = ((Plane *)top)->getlist();
              for (list1.start() ; (e1 = list1.next()) != 0 ; )
                if (e1 != 0 && e1 != epickedtop && e1->isa(line_entity) && ((((LinearEntity *)e1)->start() - ((Line *)epickedtop)->getp1()).length() < db.getptoler() ||
                                (((LinearEntity *)e1)->end() - ((Line *)epickedtop)->getp1()).length() < db.getptoler()))
                  {  Intersect i1(&line1,e1);
                     for (i = 0 ; i1.intersection(i) != 0 ; i++)
                       {  pnew[0] = ((PointE *)i1.intersection(i))->getp();
                          intersectmask |= 1;
                       }  
                  }
                else if (e1 != 0 && e1 != epickedtop && e1->isa(line_entity) && ((((LinearEntity *)e1)->start() - ((Line *)epickedtop)->getp2()).length() < db.getptoler() ||
                                (((LinearEntity *)e1)->end() - ((Line *)epickedtop)->getp2()).length() < db.getptoler()))
                  {  Intersect i1(&line1,e1);
                     for (i = 0 ; i1.intersection(i) != 0 ; i++)
                       {  pnew[1] = ((PointE *)i1.intersection(i))->getp();
                          intersectmask |= 2;
                       }  
                  }
                  
              list1 = ((Plane *)bottom)->getlist();
              for (list1.start() ; (e1 = list1.next()) != 0 ; )
                if (e1 != 0 && e1 != epickedbottom && e1->isa(line_entity) && ((((LinearEntity *)e1)->start() - ((Line *)epickedbottom)->getp1()).length() < db.getptoler() ||
                                (((LinearEntity *)e1)->end() - ((Line *)epickedbottom)->getp1()).length() < db.getptoler()))
                  {  Intersect i1(&line2,e1);
                     for (i = 0 ; i1.intersection(i) != 0 ; i++)
                       {  pnew[2] = ((PointE *)i1.intersection(i))->getp();
                          intersectmask |= 4;
                       }  
                  }
                else if (e1 != 0 && e1 != epickedbottom && e1->isa(line_entity) && ((((LinearEntity *)e1)->start() - ((Line *)epickedbottom)->getp2()).length() < db.getptoler() ||
                                (((LinearEntity *)e1)->end() - ((Line *)epickedbottom)->getp2()).length() < db.getptoler()))
                  {  Intersect i1(&line2,e1);
                     for (i = 0 ; i1.intersection(i) != 0 ; i++)
                       {  pnew[3] = ((PointE *)i1.intersection(i))->getp();
                          intersectmask |= 8;
                       }  
                  }

                if (intersectmask == 15)
                  {  db.saveundo(UD_STARTBLOCK,NULL);

                     list1 = ((Group *)panel)->getlist();

                     db.saveundo(UD_STARTBLOCK,NULL);
                     for (list1.start() ; (e1 = list1.next()) ; )
                       {  if (e1->isa(plane_entity) || e1->isa(ruledsurface_entity))
                            {  db.saveundo(UD_MOVED,e1);
                               cadwindow->invalidatedisplaylist(e1);
                            }
                       }
                     db.saveundo(UD_ENDBLOCK,NULL);


                     db.saveundo(UD_STARTBLOCK,NULL);
                     for (list1.start() ; (e1 = list1.next()) ; )
                       if (e1->isa(line_entity))
                         {  for (i = 0 ; i < 4 ; i++)
                              {  if ((((Line *)e1)->getp1() - pold[i]).length() < db.getptoler())
                                   ((Line *)e1)->moveendpoint(1,3,pnew[i]);
                                 if ((((Line *)e1)->getp2() - pold[i]).length() < db.getptoler())
                                   ((Line *)e1)->moveendpoint(1,4,pnew[i]);
                              }
                         }
                     db.saveundo(UD_ENDBLOCK,NULL);

                     for (list1.start() ; (e1 = list1.next()) ; )
                       {  if (e1->isa(plane_entity))
                            {  ((Plane *)e1)->ReCalculatePlane();
                               db.saveundo(UD_MOVED,e1);
                            }
                          else if (e1->isa(ruledsurface_entity))
                            {  db.saveundo(UD_MOVED,e1);
                            }
                       }

                     db.saveundo(UD_ENDBLOCK,NULL);

                     cadwindow->paintall();

                  } 

           }

         *cstate = ExitState;
         break;
    }  

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the line to align the panel edge to");
    }
  else if (*cstate == ExitState)
    {  
    }

}

void panel_command_thickness(int *cstate,HighLevelEvent *event,void **data)
{double thickness;
 Entity *panel,*e,*top,*bottom;
 Point origin,zaxis;
 Dialog dialog(_RCT("Change thickness"),_RCT("New thickness"),&thickness,ScaleByLength);
 EntityList list1;
 Transform t;

  list1 = state.getselection();
  list1.start();
  panel = list1.next();   

  list1 = ((Group *)panel)->getlist();
  list1.start();
  top = list1.next();
  bottom = list1.next();

  origin = (((Plane *)top)->getorigin() + ((Plane *)bottom)->getorigin()) / 2.0;
  zaxis = ((Plane *)top)->getxaxis().cross(((Plane *)bottom)->getyaxis());
  if (zaxis.dot(((Plane *)top)->getorigin() - origin) < 0.0)
    zaxis = -zaxis;

  thickness = fabs(zaxis.dot(((Plane *)top)->getorigin() - ((Plane *)bottom)->getorigin()));
  
  if (dialog.process() == TRUE && fabs(thickness - fabs(zaxis.dot(((Plane *)top)->getorigin() - ((Plane *)bottom)->getorigin()))) > db.getptoler())
    {  t.translate(zaxis * (thickness - fabs(zaxis.dot(((Plane *)top)->getorigin() - ((Plane *)bottom)->getorigin()))));
       db.saveundo(UD_STARTBLOCK,NULL);

       db.saveundo(UD_STARTBLOCK,NULL);
       for (list1.start() ; (e = list1.next()) != 0 ; )
         if (e->isa(plane_entity))
           db.saveundo(UD_MOVED,e);
       db.saveundo(UD_ENDBLOCK,NULL);

       db.saveundo(UD_STARTBLOCK,NULL);

       for (list1.start() ; (e = list1.next()) != 0 ; )
         if (e->isa(line_entity) && ((Plane *)top)->LiesOn(((LinearEntity *)e)->start()) && ((Plane *)bottom)->LiesOn(((LinearEntity *)e)->end()))
           ((Line *)e)->moveendpoint(1,3,((Line *)e)->getp1() + zaxis * (thickness - fabs(zaxis.dot(((Plane *)top)->getorigin() - ((Plane *)bottom)->getorigin()))));
         else if (e->isa(line_entity) && ((Plane *)bottom)->LiesOn(((LinearEntity *)e)->start()) && ((Plane *)top)->LiesOn(((LinearEntity *)e)->end()))
           ((Line *)e)->moveendpoint(1,4,((Line *)e)->getp2() + zaxis * (thickness - fabs(zaxis.dot(((Plane *)top)->getorigin() - ((Plane *)bottom)->getorigin()))));
         else if (e->islinear() && zaxis.dot(((LinearEntity *)e)->start() - origin) > 0 && zaxis.dot(((LinearEntity *)e)->end() - origin) > 0)
           {  e->transform(t);
              cadwindow->invalidatedisplaylist(e);
           }
         else
           cadwindow->invalidatedisplaylist(e);

       db.saveundo(UD_ENDBLOCK,NULL);

       db.saveundo(UD_STARTBLOCK,NULL);
       for (list1.start() ; (e = list1.next()) != 0 ; )
         if (e->isa(plane_entity))
           {  ((Plane *)e)->ReCalculatePlane();
              db.saveundo(UD_MOVED,e);
           }
       db.saveundo(UD_ENDBLOCK,NULL);

       db.saveundo(UD_ENDBLOCK,NULL);
       cadwindow->paintall();
    }

   *cstate = ExitState;
}

int PlanePlaneIntersection(Plane *plane1,Plane *plane2,Point *normal,Point *origin)
{double d1,d2,disc1,disc2,disc3;
 Point p1,p2,n1,n2;

  n1 = plane1->getxaxis().cross(plane1->getyaxis()).normalize();
  d1 = - n1.dot(plane1->getorigin());
  n2 = plane2->getxaxis().cross(plane2->getyaxis()).normalize();
  d2 = - n2.dot(plane2->getorigin());

  disc1 = n1.y * n2.z - n1.z * n2.y;
  disc2 = n1.x * n2.z - n1.z * n2.x;
  disc3 = n1.x * n2.y - n1.y * n2.x;

  if (fabs(disc1) < 0.0001 && fabs(disc2) < 0.0001 && fabs(disc3) < 0.001)
    return 0;

  *normal = n1.cross(n2).normalize();

  if (fabs(disc1) >= fabs(disc2) && fabs(disc1) >= fabs(disc3))
    {  origin->x = 0.0;
       origin->y = -( n2.z * d1 - n1.z * d2) / disc1;
       origin->z = -(-n2.y * d1 + n1.y * d2) / disc1;
    }
  else if (fabs(disc2) >= fabs(disc1) && fabs(disc2) >= fabs(disc3))
    {  origin->x = -( n2.z * d1 - n1.z * d2) / disc2;
       origin->y = 0.0;
       origin->z = -(-n2.x * d1 + n1.x * d2) / disc2;
    }
  else
    {  origin->x = -( n2.y * d1 - n1.y * d2) / disc3;
       origin->y = -(-n2.x * d1 + n1.x * d2) / disc3;
       origin->z = 0.0;
    }

  return 1;


}


void TrimPanel(Group *panel1,Point ppicked1,Entity *trime,Point ppicked2,int mitre)
{static Point dir1,dir2;
 Entity *epickedtop,*epickedbottom,*epicked2;
 Point p1,p2,p3,p4,p,dir,pnew[4],pold[4],pint[4],zaxis,origin;
 int i,j,topselected,intersectmask,ntop1,nbottom1,ntop2,nbottom2;
 Entity *e1,*e2,*edge[4];
 Group *panel2;
 EntityList list1,list2,children;
 Entity *top1,*bottom1,*top2,*bottom2;
 Point zaxistop,zaxisbottom,za;
 double t;


  //  Search for the boundary entity that was selected
  list1 = panel1->getlist();
  list1.start();
  top1 = list1.next();
  bottom1 = list1.next();
  epickedtop = epickedbottom = 0;

  //  Locate the edge that was used to select the panel
  for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 && i < 2 ; i++)
    if (e1->isa(plane_entity))
      {  list2 = ((Plane *)e1)->getlist();
         for (list2.start() ; ! list2.atend() ; )
           {  e2 = list2.next();
              if (e2 != 0 && (e2->nearp(ppicked1) - ppicked1).length() < db.getptoler() * 100.0)
                {  t = ((LinearEntity *)e2)->gett(e2->nearp(ppicked1));
                   if (t >= -0.001 && t <= 1.001)
                     {  if (i == 0)
                          epickedtop = e2;
                        else
                          epickedbottom = e2;
                     }
                } 
           } 
      }

  topselected = epickedtop != 0;

  if (epickedtop == 0 && epickedbottom == 0)
    { RCMessageBox(cadwindow->gethwnd(),"Locate the panel on the edge you want to move","Align Edge",MB_OK);
       return;
    }

  p1 = ((LinearEntity *)(epickedtop != 0 ? epickedtop : epickedbottom))->start();
  p2 = ((LinearEntity *)(epickedtop != 0 ? epickedtop : epickedbottom))->end();

  //  Locate the edge on the other face that corresponds to this edge
  for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 ; i++)
    if (e1->islinear())
      {  if ( ((((LinearEntity *)e1)->start() - p1).length() < db.getptoler() || 
              (((LinearEntity *)e1)->end() - p1).length() < db.getptoler()) &&

              (((Plane *)top1)->LiesOn(((LinearEntity *)e1)->start()) && 
              ((Plane *)bottom1)->LiesOn(((LinearEntity *)e1)->end()) || 

              ((Plane *)top1)->LiesOn(((LinearEntity *)e1)->end()) && 
              ((Plane *)bottom1)->LiesOn(((LinearEntity *)e1)->start()) ))
           {  if ((((LinearEntity *)e1)->start() - p1).length() < db.getptoler())
                p3 = ((LinearEntity *)e1)->end();
              else      
                p3 = ((LinearEntity *)e1)->start();
           }

         if ( ((((LinearEntity *)e1)->start() - p2).length() < db.getptoler() || 
               (((LinearEntity *)e1)->end() - p2).length() < db.getptoler()) &&

               ( ((Plane *)top1)->LiesOn(((LinearEntity *)e1)->start()) && 
                 ((Plane *)bottom1)->LiesOn(((LinearEntity *)e1)->end()) || 

                 ((Plane *)top1)->LiesOn(((LinearEntity *)e1)->end()) && 
                 ((Plane *)bottom1)->LiesOn(((LinearEntity *)e1)->start()) ))
           {  if ((((LinearEntity *)e1)->start() - p2).length() < db.getptoler())
                p4 = ((LinearEntity *)e1)->end();
              else      
                p4 = ((LinearEntity *)e1)->start();
           }

      }

 for (list1.start() ; (e1 = list1.next()) != 0 ; )
   if (e1->islinear() && 
        ( (((LinearEntity *)e1)->start() - p3).length() < db.getptoler() && 
          (((LinearEntity *)e1)->end() - p4).length() < db.getptoler() ||
          (((LinearEntity *)e1)->start() - p4).length() < db.getptoler() && 
          (((LinearEntity *)e1)->end() - p3).length() < db.getptoler()))
     {  if (epickedbottom == 0)
          epickedbottom = e1;
        else
          epickedtop = e1;
     }

  if (epickedtop == 0 || ! epickedtop->isa(line_entity) || epickedbottom == 0 || ! epickedbottom->isa(line_entity))
    return;

  if (! topselected)
    {  p = p1;  p1 = p3;  p3 = p;
       p = p2;  p2 = p4;  p4 = p;
       e1 = epickedtop;  epickedtop = epickedbottom;  epickedbottom = e1;
    }

  //  Find the connecting edges to the top edge
  edge[0] = edge[1] = 0;
  list2 = ((Plane *)top1)->getlist();
  for (list2.start() ; ! list2.atend() ; )
    {  e2 = list2.next();
       if (e2 != 0 && e2 != epickedtop && 
             ((((LinearEntity *)e2)->start() - p1).length() < db.getptoler() || (((LinearEntity *)e2)->end() - p1).length() < db.getptoler()) &&
             ((((LinearEntity *)e2)->start() - p2).length() > db.getptoler() && (((LinearEntity *)e2)->end() - p2).length() > db.getptoler()))
         edge[0] = e2;

       if (e2 != 0 && e2 != epickedtop && 
             ((((LinearEntity *)e2)->start() - p2).length() < db.getptoler() || (((LinearEntity *)e2)->end() - p2).length() < db.getptoler()) &&
             ((((LinearEntity *)e2)->start() - p1).length() > db.getptoler() && (((LinearEntity *)e2)->end() - p1).length() > db.getptoler()))
         edge[1] = e2;
    }

  //  Find the connecting edges to the bottom edge
  edge[2] = edge[3] = 0;
  list2 = ((Plane *)bottom1)->getlist();
  for (list2.start() ; ! list2.atend() ; )
    {  e2 = list2.next();
       if (e2 != 0 && e2 != epickedbottom && 
             ((((LinearEntity *)e2)->start() - p3).length() < db.getptoler() || (((LinearEntity *)e2)->end() - p3).length() < db.getptoler()) &&
             ((((LinearEntity *)e2)->start() - p4).length() > db.getptoler() && (((LinearEntity *)e2)->end() - p4).length() > db.getptoler()))
         edge[2] = e2;

       if (e2 != 0 && e2 != epickedbottom && 
             ((((LinearEntity *)e2)->start() - p4).length() < db.getptoler() || (((LinearEntity *)e2)->end() - p4).length() < db.getptoler()) &&
             ((((LinearEntity *)e2)->start() - p3).length() > db.getptoler() && (((LinearEntity *)e2)->end() - p3).length() > db.getptoler()))
         edge[3] = e2;
    }

  if (edge[0] == 0 || edge[1] == 0 || edge[2] == 0 || edge[3] == 0 || 
      ! edge[0]->isa(line_entity) || ! edge[1]->isa(line_entity) || ! edge[2]->isa(line_entity) || ! edge[3]->isa(line_entity))
    return;


  if (trime != 0 && trime->isa(group_entity) && strcmp(((Group *)trime)->getclassname(),"RC::Panel") == 0)
    panel2 = (Group *)trime;
  else
    panel2 = 0; 

  if (panel2 != 0)
    {  list1 = panel2->getlist();
       list1.start();
       top2 = list1.next();
       bottom2 = list1.next();

       if (((Plane *)top2)->LiesOn(ppicked2))
         {  zaxis = ((Plane *)top2)->getxaxis().cross(((Plane *)top2)->getyaxis());
            origin = ((Plane *)top2)->getorigin();
         }
       else if (((Plane *)bottom2)->LiesOn(ppicked2))
         {  zaxis = ((Plane *)bottom2)->getxaxis().cross(((Plane *)bottom2)->getyaxis());
            origin = ((Plane *)bottom2)->getorigin();
         }

       //  Only calculuate the directions for the first mitred panel - after that the panel will be changed and this wont work
       if (mitre == 1)
         {  if ((((Line *)edge[0])->getp1() - p1).length() < db.getptoler())
              dir1 = ((Line *)edge[0])->getp1() - ((Line *)edge[0])->getp2();
            else
              dir1 = ((Line *)edge[0])->getp2() - ((Line *)edge[0])->getp1();

            //  Locate the edge that was used to select the panel
            epicked2 = 0;
            for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 && i < 2 ; i++)
              if (e1->isa(plane_entity))
                {  list2 = ((Plane *)e1)->getlist();
                   for (list2.start() ; ! list2.atend() ; )
                     {  e2 = list2.next();
                        if (e2 != 0 && (e2->nearp(ppicked2) - ppicked2).length() < db.getptoler() * 100.0)
                          {  t = ((LinearEntity *)e2)->gett(e2->nearp(ppicked2));
                             if (t >= -0.001 && t <= 1.001)
                               epicked2 = e2;
                          }
                     } 
                } 

            if (epicked2 == 0)
              return; 

            for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 && i < 2 ; i++)
              if (e1->isa(plane_entity))
                {  list2 = ((Plane *)e1)->getlist();
                   for (list2.start() ; ! list2.atend() ; )
                     {  e2 = list2.next();
                        if (e2 != 0 && e2->islinear() && 
                           ( (((LinearEntity *)e2)->start() - ((LinearEntity *)epicked2)->start()).length() < db.getptoler() || 
                             (((LinearEntity *)e2)->start() - ((LinearEntity *)epicked2)->end()).length() < db.getptoler()))
                          dir2 = ((LinearEntity *)e2)->start() - ((LinearEntity *)e2)->end();
                        else if (e2 != 0 && e2->islinear() && 
                          ( (((LinearEntity *)e2)->end() - ((LinearEntity *)epicked2)->start()).length() < db.getptoler() || 
                            (((LinearEntity *)e2)->end() - ((LinearEntity *)epicked2)->end()).length() < db.getptoler()))
                          dir2 = ((LinearEntity *)e2)->end() - ((LinearEntity *)e2)->start();
                     } 
                } 

          }

    }
  else if (trime != 0 && trime->isa(line_entity))
    {  origin = ((Line *)trime)->getp1();
       zaxis = (((Line *)trime)->getp2() - ((Line *)trime)->getp1()).cross(((Plane *)top1)->getxaxis().cross(((Plane *)top1)->getyaxis()));
    }
  else
    {  zaxis = (p2 - p1).cross(((Plane *)top1)->getxaxis().cross(((Plane *)top1)->getyaxis()));
       origin = ppicked2;  
       top2 = 0;
       bottom2 = 0;

    }

  if (mitre != 0 && top1 != 0 && top2 != 0 && bottom1 != 0 && bottom2 != 0 && panel2 != 0)
    { 
       if (! PlanePlaneIntersection((Plane *)top1,(Plane *)top2,&dir,&pint[0]) ||
           ! PlanePlaneIntersection((Plane *)top1,(Plane *)bottom2,&dir,&pint[1]) ||
           ! PlanePlaneIntersection((Plane *)bottom1,(Plane *)top2,&dir,&pint[2]) ||
           ! PlanePlaneIntersection((Plane *)bottom1,(Plane *)bottom2,&dir,&pint[3]))
         return;

       za = (dir1.normalize() + dir2.normalize()).normalize();
       za = (za.cross(dir)).normalize();

       //  Intersections must be lines - as they are plane-plane intersections
       for (i = 0 ; i < 4 ; i++)
         for (j = 0 ; j < 4 ; j++)
           {  ntop1 =    ((Plane *)top1)->LiesOn(pint[i]) + 
                         ((Plane *)top1)->LiesOn(pint[j]);
              nbottom1 = ((Plane *)bottom1)->LiesOn(pint[i]) + 
                         ((Plane *)bottom1)->LiesOn(pint[j]);
              ntop2 =    ((Plane *)top2)->LiesOn(pint[i]) + 
                         ((Plane *)top2)->LiesOn(pint[j]);
              nbottom2 = ((Plane *)bottom2)->LiesOn(pint[i]) + 
                         ((Plane *)bottom2)->LiesOn(pint[j]);
              if (ntop1 == 1 && nbottom1 == 1 && ntop2 == 1 && nbottom2 == 1)
                {  zaxis = (dir.cross(pint[i] - pint[j])).normalize();
                   origin = pint[i];  
                   if (fabs(zaxis.dot(za)) > 0.7)
                     {  i = 4;  j = 4;
                        break;
                     }
                }
           }   

    }

  zaxis.normalize();

  pold[0] = p1;  
  pold[1] = p2;  
  pold[2] = p3;  
  pold[3] = p4;  

  InfinitePlane ip(zaxis,origin);

  intersectmask = 0;

  for (i = 0 ; i < 4 ; i++)
    {  Intersect i1(edge[i],&ip);
       for (j = 0 ; i1.intersection(j) != 0 ; j++)
         {  pnew[i] = ((PointE *)i1.intersection(j))->getp();
            intersectmask |= (1 << i);
         }  
    }

  if (intersectmask == 15)
    {  

       list1 = ((Group *)panel1)->getlist();

       db.saveundo(UD_STARTBLOCK,NULL);
       for (list1.start() ; (e1 = list1.next()) ; )
         db.saveundo(UD_MOVED,e1);
       db.saveundo(UD_ENDBLOCK,NULL);


       db.saveundo(UD_STARTBLOCK,NULL);

       for (list1.start() ; (e1 = list1.next()) ; )
         if (e1->isa(line_entity))
           {  for (i = 0 ; i < 4 ; i++)
                {  if ((((Line *)e1)->getp1() - pold[i]).length() < db.getptoler())
                     ((Line *)e1)->moveendpoint(1,3,pnew[i]);
                   if ((((Line *)e1)->getp2() - pold[i]).length() < db.getptoler())
                     ((Line *)e1)->moveendpoint(1,4,pnew[i]);
                }
           }
       db.saveundo(UD_ENDBLOCK,NULL);

       db.saveundo(UD_STARTBLOCK,NULL);
       for (list1.start() ; (e1 = list1.next()) ; )
         {  db.saveundo(UD_MOVED,e1);
            if (e1->isa(plane_entity))
              ((Plane *)e1)->ReCalculatePlane();
            cadwindow->invalidatedisplaylist(e1);
         }
       db.saveundo(UD_ENDBLOCK,NULL);

       cadwindow->paintall();

    } 

}

void panel_command_renumber(int *cstate,HighLevelEvent *event,void **)
{Entity *e;
 int id;
 BitMask change(32);
 RCCHAR text[32];

  id = 1;
  change.set(5);

  for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
    if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"RC::Panel") == 0)
      {  sprintf(text,"ID=%d",id++);
         ((Group *)e)->change(change,*e,0,text,BitMask(32));
      }

  *cstate = ExitState;
}

void panel_command_layout(int *cstate,HighLevelEvent *event,void **)
{static Point p1;
 Point p,p2,p3;
 Entity *e,*e1,*top;
 EntityList list;
 Transform t,t1,t2;
 Point xaxis,yaxis,zaxis,pmin,pmax;
 double maximumheight,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset;
 int i,precision,tolprecision,count;
 TerminatorInfo term1,term2;
 TextInfo vinfo,tinfo,pinfo,sinfo;
 SortedPointList l,lempty;
 BitMask options(32),visible(MaxViews);
 
  switch (*cstate)
    {  case InitialState :
         *cstate = 1;
         break;

       case 1 :
         if (event->isa(Coordinate3d))
           {  p1 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;

       case 2 :
         if (event->isa(Coordinate3d))
           {  p2 = ((Coordinate3dEvent *) event)->getp();
              p3 = p1;
              maximumheight = 0.0;

              LinearDimension::getdefaults(&extension,&gap,&stackdis,&uppertol,&lowertol,&scale,&textyoffset,&precision,&tolprecision,&term1,&term2,&vinfo,&tinfo,&pinfo,&sinfo,&options);


              db.saveundo(UD_STARTBLOCK,NULL);
              for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
                if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"RC::Panel") == 0)
                  {  db.geometry.clearstatus();
                     list = ((Group *)e)->getlist();
                     list.start();
                     top = list.next();
                     if (top->isa(plane_entity))
                       {  xaxis = ((Plane *)top)->getxaxis(); 
                          yaxis = ((Plane *)top)->getyaxis(); 
                          zaxis = xaxis.cross(yaxis);
                          t1.translate(-((Plane *)top)->getorigin());
                          t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
                          t = t1.apply(t2);  

                          list = ((Plane *)top)->getlist();
                          for (list.start(),count = 0 ; ! list.atend() != 0 ; )
                            if ((e1 = list.next()) != 0)
                              {  for (i = 0 ; i <= 6 ; i++)
                                   {  p = t.transform(((LinearEntity *)e1)->position(i / 6.0));
                                      if (count == 0)
                                        {  pmax = pmin = p;
                                        }
                                      else
                                        {  if (p.x > pmax.x) pmax.x = p.x;
                                           if (p.y > pmax.y) pmax.y = p.y;
                                           if (p.z > pmax.z) pmax.z = p.z;
                                           if (p.x < pmin.x) pmin.x = p.x;
                                           if (p.y < pmin.y) pmin.y = p.y;
                                           if (p.z < pmin.z) pmin.z = p.z;
                                        }
                                      count++;
                                   }
                              } 

                          if (p3.x + pmax.x - pmin.x > p2.x)
                            {  p1.y += maximumheight * 1.5;
                               p3 = p1;
                               maximumheight = 0.0;
                            }                             
                             
                          if (pmax.y - pmin.y > maximumheight)
                            maximumheight = pmax.y - pmin.y;    

                          t1.translate(p3 - pmin);
                          t.apply(t1);  
                          db.geometry.add(e->clone(t));

                          l = lempty;
                          l.add(Point(0.0,-(pmax.y - pmin.y)*0.2,0.0));
                          l.add(Point(pmax.x - pmin.x,-(pmax.y - pmin.y)*0.2,0.0));
                          db.geometry.add(new LinearDimension(p3+Point(0.0,(pmax.y - pmin.y)*1.2,0.0),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options));
                          
                          l = lempty;
                          l.add(Point(0.0,-(pmax.x - pmin.x)*0.2,0.0));
                          l.add(Point(pmax.y - pmin.y,-(pmax.x - pmin.x)*0.2,0.0));
                          db.geometry.add(new LinearDimension(p3+Point((pmax.x - pmin.x) * 1.2,0.0,0.0),Point(0.0,1.0,0.0),Point(1.0,0.0,0.0),l,extension,gap,stackdis,uppertol,lowertol,scale,textyoffset,precision,tolprecision,term1,term2,vinfo,tinfo,pinfo,sinfo,options));

                          db.geometry.add(new Text(CHAR2RCCHR("Arial"),p3,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),0.0,(pmax.y - pmin.y)*0.1,0.0,0.0,0,CHAR2RCCHR(((Group *)e)->getinstance()),0,BitMask(32)));

                          p3.x += (pmax.x - pmin.x) * 1.5;

                       }
                  } 
              db.saveundo(UD_ENDBLOCK,NULL);

           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;

    }  

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the lower left position to start placing the panels");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the lower right position to start placing the panels");
    }
  else if (*cstate == ExitState)
    { 
    }

}

void panel_command_export_csv(int *cstate,HighLevelEvent *event,void **)
{OPENFILENAME ofn;
 Point p,p2,p3;
 Entity *e,*e1,*top;
 EntityList list;
 Transform t,t1,t2;
 Point xaxis,yaxis,zaxis,pmin,pmax;
 RCCHAR  filename[300],filter[300];
 FILE *outfile;
 int i,count;

  strcpy(filename,"");
  strcpy(filter,"CSV Files *.csv|*.csv|");
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
  ofn.lpstrDefExt = _RCT("*.csv");
  ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST;
  if (GetSaveFileName(&ofn))
    {  if ((outfile = fopen(filename,"w")) == 0)
        RCMessageBox(cadwindow->gethwnd(),"The CSV file could not be created.","Export Panels",MB_OK);
       else
         {  
            for (db.geometry.start() ; (e = db.geometry.next()) != 0 ; )
              if (e->isa(group_entity) && strcmp(((Group *)e)->getclassname(),"RC::Panel") == 0)
                {  list = ((Group *)e)->getlist();
                   list.start();
                   top = list.next();
                   if (top->isa(plane_entity))
                     {  xaxis = ((Plane *)top)->getxaxis(); 
                        yaxis = ((Plane *)top)->getyaxis(); 
                        zaxis = xaxis.cross(yaxis);
                        t1.translate(-((Plane *)top)->getorigin());
                        t2.fromcoordinatesystem(xaxis,yaxis,zaxis);
                        t = t1.apply(t2);  

                        list = ((Plane *)top)->getlist();
                        for (list.start(),count = 0 ; ! list.atend() != 0 ; )
                          if ((e1 = list.next()) != 0)
                            {  for (i = 0 ; i <= 6 ; i++)
                                 {  p = t.transform(((LinearEntity *)e1)->position(i / 6.0));
                                    if (count == 0)
                                      {  pmax = pmin = p;
                                      }
                                    else
                                      {  if (p.x > pmax.x) pmax.x = p.x;
                                         if (p.y > pmax.y) pmax.y = p.y;
                                         if (p.z > pmax.z) pmax.z = p.z;
                                         if (p.x < pmin.x) pmin.x = p.x;
                                         if (p.y < pmin.y) pmin.y = p.y;
                                         if (p.z < pmin.z) pmin.z = p.z;
                                      }
                                    count++;
                                 }
                            } 

                         fprintf(outfile,"%s,%.6lf,%.6lf\n",((Group *)e)->getinstance(),pmax.x - pmin.x,pmax.y - pmin.y);

                       }
                  } 


            fclose(outfile);
         }
    }

  *cstate = ExitState;

}

Entity *panel_get_face(Entity *panel)
{Point ppicked;
 EntityList list1,list2;
 Entity *e1,*e2;
 int i;
 double t;

  ppicked = state.getlastpickpoint();

  //  Search for the boundary entity that was selected
  list1 = ((Group *)panel)->getlist();
  
  //  Locate the edge that was used to select the panel
  for (list1.start(),i = 0 ; (e1 = list1.next()) != 0 && i < 2 ; i++)
    if (e1->isa(plane_entity))
      {  list2 = ((Plane *)e1)->getlist();
         for (list2.start() ; ! list2.atend() ; )
           {  e2 = list2.next();
              if (e2 != 0 && (e2->nearp(ppicked) - ppicked).length() < db.getptoler() * 100.0)
                {  t = ((LinearEntity *)e2)->gett(e2->nearp(ppicked));
                   if (t >= -0.001 && t <= 1.001)
                     return e1;
                } 
           } 
      }

  return 0;

}
 
