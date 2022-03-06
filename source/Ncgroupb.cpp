
#include "ncwin.h"

#if MENUS != STUDENTMENUS
void group_command0(int *cstate,HighLevelEvent *,void **)
{
    cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Group"));
   *cstate = ExitState;
}

/**
 * @brief group_command1
 * @param cstate
 *
 * Group entities command
 *
 * KMJ : Working in Qt version
 *
 */
void group_command1(int *cstate,HighLevelEvent *,void **)
{EntityList list,glist;
 BitMask options(32);
 RCCHAR *s1,*s2,classname[300],instance[300];
 ResourceString rs0(NCGROUPB),rs1(NCGROUPB+1);
  list = state.getselection().copy();
  if (list.length() == 0)
    cadwindow->MessageBox(rs1.gets(),rs0.gets(),MB_ICONINFORMATION);
  else
    {  Group::getdefaults(&s1,&s2,&options);
       strcpy(classname,s1);  strcpy(instance,s2);
       if (Group::setup(NULL,classname,instance,&options))
         {
            state.destroyselection(1);
            Group::setdefaults(classname,instance,options);
            Group *group = new Group(list,classname,instance,options);
            if (group != NULL && group->getdefined())
              {  db.geometry.add(group);
                 glist.add(group);
                 state.setselection(glist);
              }
            else
              delete group;
         }
       else
         list.destroy();
    }
  *cstate = ExitState;
}

/**
 * @brief group_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Add to group command
 *
 * KMJ : working in Qt version
 *
 */
void group_command2(int *cstate,HighLevelEvent *event,void **data)
{Entity *e;
 Group **group = (Group **) *data;
 EntityList list,glist;
 BitMask options(32);
 ResourceString rs2(NCGROUPB+2),rs3(NCGROUPB+3),rs5(NCGROUPB+5),rs7(NCGROUPB+7),rs8(NCGROUPB+8);
  switch (*cstate)
    {  case InitialState :
         list = state.getselection().copy();
         if (list.length() == 0)
           {  *cstate = ExitState;
              cadwindow->MessageBox(rs3.gets(),rs2.gets(),MB_ICONINFORMATION);
           }
         else if (list.length() > 1)
           {  state.destroyselection(1);
              *cstate = ExitState;
              cadwindow->MessageBox(rs5.gets(),rs2.gets(),MB_ICONINFORMATION);
           }
         else
           {  state.destroyselection(0);
              list.start();
              if ((e = list.next()) != NULL && e->isa(group_entity))
                {  group = new Group *;
                   *data = group;
                   *group = (Group *) e;
                   *cstate = 1;
                   state.destroyselection(0);
                   state.excludelist.add(*group);
                   db.saveundo(UD_STARTBLOCK,NULL);
                }
              else
                {  *cstate = ExitState;
                   cadwindow->MessageBox(rs7.gets(),rs2.gets(),MB_ICONINFORMATION);
                }
           }
         break;
       case 1 :
         list = state.getselection().copy();
         state.destroyselection(0);
         if (event->isa(EntitySelected))
           {  for (list.start() ; (e = list.next()) != 0 ; )
                {  (*group)->add(e);
                   e->setselected(e->getselected()-1);
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  glist.add(*group);
              state.setselection(glist,1);
              *cstate = ExitState;
              db.saveundo(UD_ENDBLOCK,NULL);
           }
         break;
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(rs8.gets());
  else
    delete group;
}

/**
 * @brief group_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Remove from group command
 *
 * KMJ : Working in Qt version
 *
 */
void group_command3(int *cstate,HighLevelEvent *event,void **data)
{Entity *e;
 Group **group = (Group **) *data;
 EntityList list,glist;
 BitMask options(32);
 ResourceString rs9(NCGROUPB+9),rs10(NCGROUPB+10),rs11(NCGROUPB+11),rs13(NCGROUPB+13),rs15(NCGROUPB+15);
  switch (*cstate)
    {  case InitialState :
         list = state.getselection().copy();
         if (list.length() == 0)
           {  *cstate = ExitState;
              cadwindow->MessageBox(rs9.gets(),rs10.gets(),MB_ICONINFORMATION);
           }
         else if (list.length() > 1)
           {  state.destroyselection(1);
              *cstate = ExitState;
              cadwindow->MessageBox(rs11.gets(),rs10.gets(),MB_ICONINFORMATION);
           }
         else
           {  state.destroyselection(0);
              list.start();
              if ((e = list.next()) != NULL && e->isa(group_entity))
                {  group = new Group *;
                   *data = group;
                   *group = (Group *) e;
                   *cstate = 1;
                   state.destroyselection(0);
                   state.excludelist.add(*group);
                   db.saveundo(UD_STARTBLOCK,NULL);
                }
              else
                {  *cstate = ExitState;
                   cadwindow->MessageBox(rs13.gets(),rs10.gets(),MB_ICONINFORMATION);
                }
           }
         break;
       case 1 :
         list = state.getselection().copy();
         state.destroyselection(1);
         if (event->isa(EntitySelected))
           {  for (list.start() ; (e = list.next()) != 0 ; )
                (*group)->del(e);
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  glist.add(*group);
              state.setselection(glist,1);
              *cstate = ExitState;
              db.saveundo(UD_ENDBLOCK,NULL);
           }
         break;
    }
  if (*cstate == 1)
    cadwindow->prompt->normalprompt(rs15.gets());
  else
    delete group;
}

/**
 * @brief group_command4
 * @param cstate
 *
 * Ungroup entities
 *
 * KMJ : Working in Qt version
 *
 */
void group_command4(int *cstate,HighLevelEvent *,void **)
{EntityList list;
 Entity *e;
 ResourceString rs16(NCGROUPB+16),rs17(NCGROUPB+17);
 BitMask options(32);
 RCCHAR *s1,*s2;

  list = state.getselection().copy();
  state.destroyselection(1);
  if (list.length() == 0)
    cadwindow->MessageBox(rs16.gets(),rs17.gets(),MB_ICONINFORMATION);
  else
    {  db.saveundo(UD_STARTBLOCK,NULL);
       for (list.start() ; (e = list.next()) != NULL ; )
         if (e->isa(group_entity))
           db.geometry.del(e,0);
         else if (e->isa(figure_entity))
           {Entity *firstentity,*e1;
            EntityList list;
              db.geometry.start();
              firstentity = db.geometry.next();
              if (e->explode())
                {  for (db.geometry.start() ; (e1 = db.geometry.next()) != 0 ; )
                     if (e1 == firstentity)
                       break;
                     else
                       list.add(e1);
                   if (list.length() > 0)
                     {  Group::getdefaults(&s1,&s2,&options);
                        Group *group = new Group(list,_RCT("Exploded Figure"),((Figure *)e)->getlibrary(),options);
                        if (group != NULL && group->getdefined())
                          db.geometry.add(group);
                        else
                          delete group;
                     }
                   if (e->getalist().empty())
                     db.geometry.del(e,0);
                }
           }
       list.destroy();
       db.saveundo(UD_ENDBLOCK,NULL);
    }
  *cstate = ExitState;
}

extern void RemoveLockedEntities(EntityList *list,int unhighlight);

/**
 * @brief group_command5
 * @param cstate
 *
 * Group options command
 *
 * KMJ : Working in Qt version
 *
 */
void group_command5(int *cstate,HighLevelEvent *,void **)
{BitMask options(32),change(32);
 EntityList list;
 EntityHeader header;
 Entity *e;
 RCCHAR *s1,*s2,c[300],m[300];
 ResourceString rs18(NCGROUPB+18),rs19(NCGROUPB+19);
 int length;

  *cstate = ExitState;
  list = state.getselection().copy();
  if (list.empty())
    {  Group::getdefaults(&s1,&s2,&options);
       strcpy(c,s1);  strcpy(m,s2);
       if (Group::setup(NULL,c,m,&options))
         Group::setdefaults(c,m,options);
    }
  else
    {
       Group::getdefaults(&s1,&s2,&options);
       strcpy(c,s1);  strcpy(m,s2);
       if (Group::setup(&change,c,m,&options))
         {  
            length = list.length();
            RemoveLockedEntities(&list,0);

            db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              if (e->isa(group_entity))
                ((Group *)e)->change(change,header,c,m,options);
            db.saveundo(UD_ENDBLOCK,NULL);

            if (list.length() != length)
              cadwindow->MessageBox("Entities on locked layers were not changed","Change",MB_OK);

            list.destroy();

         }
    }
}

#endif
