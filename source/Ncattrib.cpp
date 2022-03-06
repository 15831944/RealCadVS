
#include "ncwin.h"

/**
 * @brief attribute_command1
 * @param cstate
 *
 * Entry point for New Attribute command
 *
 * KMJ : Working in Qt Version
 *
 */
void attribute_command1(int *cstate,HighLevelEvent *,void **)
{
 RCCHAR attribute[1024];
 Dialog dialog("AttributeNew_Dialog");
 ResourceString rs0(NCATTRIB),rs1(NCATTRIB+1);
  *cstate = ExitState;
  if (state.getselection().length() == 0)
    cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
  else
    {  attribute[0] = 0;
       dialog.add(new StringDialogControl(100,attribute,1024));
       if (dialog.process() == TRUE)
         {EntityList list;
          RCCHAR *at;
          Entity *e;
            list = state.getselection();
            db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              if ((at = new RCCHAR[strlen(attribute) + 1]) != NULL)
                {  strcpy(at,attribute);
                   e->addat(at);
                   e->setattribute(at);
                   db.saveundo(UD_INSERTATT,e);
                }
            db.saveundo(UD_ENDBLOCK,NULL);
         }
    }
}


/**
 * @brief attribute_command2
 * @param cstate
 *
 * Entry point for the delete attribute command
 *
 * KMJ : Working in Qt version
 *
 */
void attribute_command2(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
 ResourceString rs2(NCATTRIB+2),rs3(NCATTRIB+3),rs4(NCATTRIB+4);
 ResourceString rs5(NCATTRIB+5),rs6(NCATTRIB+6);
  if (state.getselection().length() == 0)
    cadwindow->MessageBox(rs2.gets(),rs3.gets(),MB_ICONINFORMATION);
  else if (state.getselection().length() > 1)
    {  if (cadwindow->MessageBox(rs4.gets(),rs3.gets(),MB_YESNO) == IDYES)
         {EntityList list;
          AttributeList atlist;
          RCCHAR *at;
          Entity *e;
            db.saveundo(UD_STARTBLOCK,NULL);
            list = state.getselection();
            for (list.start() ; (e = list.next()) != NULL ; )
              {  atlist = e->getatlist().copy();
                 for (atlist.start() ; (at = atlist.next()) != NULL ; )
                   {  e->delat(at);  e->setattribute(at);
                      db.saveundo(UD_DELETEATT,e);
                      delete at;
                   }
                 atlist.destroy();
              }
            db.saveundo(UD_ENDBLOCK,NULL);
         }
    }
  else
    {int i,j,status;
     Entity *e;
     EntityList list;
     AttributeList al;
     RCCHAR *at=0,label[300];
       i = 0;
       list = state.getselection();  list.start();  e = list.next();
       do {Dialog dialog("AttributeDelete_Dialog");
            if (e->getatlist().length() == 0)
               {  cadwindow->MessageBox(rs5.gets(),rs3.gets(),MB_ICONINFORMATION);
                  status = FALSE;
               }
             else
               {  if (i < 0) i = 0;
                  al = e->getatlist();
                  if (i >= al.length()) i = al.length() - 1;
                  for (j = 0,al.start() ; j <= i ; j++) at = al.next();
                  sprintf(label,rs6.gets(),i+1,al.length());
                  dialog.label(label);
                  dialog.add(new StringDialogControl(101,at,strlen(at)+1));
                  dialog.add(new ButtonDialogControl(102));
                  dialog.add(new ButtonDialogControl(103));
                  dialog.add(new ButtonDialogControl(104));
                  status = dialog.process();
                  switch (status)
                    {  case 102 : i++;  break;
                       case 103 : i--;  break;
                       case 104 :
                         e->delat(at);  e->setattribute(at);
                         db.saveundo(UD_DELETEATT,e);
                         delete at;
                         at=0;
                         break;
                    }
               }
          } while (status != FALSE);
    }
}

/**
 * @brief attribute_command3
 * @param cstate
 *
 * Entry point for the edit attribute command
 *
 * KMJ : Working in Qt version
 *
 */
void attribute_command3(int *cstate,HighLevelEvent *,void **)
{ResourceString rs7(NCATTRIB+7),rs8(NCATTRIB+8),rs9(NCATTRIB+9);
 ResourceString rs10(NCATTRIB+10),rs11(NCATTRIB+11),rs12(NCATTRIB+12);
  *cstate = ExitState;
  if (state.getselection().length() == 0)
    cadwindow->MessageBox(rs7.gets(),rs8.gets(),MB_ICONINFORMATION);
  else if (state.getselection().length() > 1)
    cadwindow->MessageBox(rs9.gets(),rs10.gets(),MB_ICONINFORMATION);
  else
    {int i,j,status;
     Entity *e;
     EntityList list;
     AttributeList al;
     RCCHAR *at,*newat,attribute[1024],label[300];
       i = 0;
       list = state.getselection();  list.start();  e = list.next();
       do {Dialog dialog("AttributeEdit_Dialog");
            if (e->getatlist().length() == 0)
               {  cadwindow->MessageBox(rs11.gets(),rs8.gets(),MB_ICONINFORMATION);
                  status = FALSE;
               }
             else
               {  if (i < 0) i = 0;
                  al = e->getatlist();
                  if (i >= al.length()) i = al.length() - 1;
                  for (j = 0,al.start() ; j <= i ; j++) at = al.next();
                  sprintf(label,rs12.gets(),i+1,al.length());
                  dialog.label(label);
                  strcpy(attribute,at);
                  dialog.add(new StringDialogControl(101,attribute,1023));
                  dialog.add(new ButtonDialogControl(102));
                  dialog.add(new ButtonDialogControl(103));
                  dialog.add(new ButtonDialogControl(104));
                  status = dialog.process();
                  switch (status)
                    {  case TRUE :
                         if ((newat = new RCCHAR[strlen(attribute)+1]) != NULL)
                           {  db.saveundo(UD_STARTBLOCK,NULL);
                              e->delat(at);  e->setattribute(at);
                              db.saveundo(UD_DELETEATT,e);
                              delete at;
                              at=0;
                              strcpy(newat,attribute);
                              e->addat(newat);  e->setattribute(newat);
                              db.saveundo(UD_INSERTATT,e);
                              db.saveundo(UD_ENDBLOCK,NULL);
                           }
                         i = 0;
                         break;
                       case 102 : i++;  break;
                       case 103 : i--;  break;
                    }
               }
          } while (status != FALSE);
    }
}
/**
 * @brief attribute_command4
 * @param cstate
 *
 * Entry point for the list Attribute command
 *
 * KMJ : working in Qt version
 *
 */
void attribute_command4(int *cstate,HighLevelEvent *,void **)
{ResourceString rs13(NCATTRIB+13),rs14(NCATTRIB+14),rs15(NCATTRIB+15);
 ResourceString rs16(NCATTRIB+16),rs17(NCATTRIB+17);
  *cstate = ExitState;
  if (state.getselection().length() == 0)
    cadwindow->MessageBox(rs13.gets(),rs14.gets(),MB_ICONINFORMATION);
  else if (state.getselection().length() > 1)
    cadwindow->MessageBox(rs15.gets(),rs14.gets(),MB_ICONINFORMATION);
  else
    {int i,j,status;
     Entity *e=0;
     EntityList list;
     AttributeList al;
     RCCHAR *at=0,label[300];
       i = 0;
       list = state.getselection();  list.start();  e = list.next();
       do {Dialog dialog("AttributeList_Dialog");
            if (e->getatlist().length() == 0)
               {  cadwindow->MessageBox(rs16.gets(),rs14.gets(),MB_ICONINFORMATION);
                  status = FALSE;
               }
             else
               {  if (i < 0) i = 0;
                  al = e->getatlist();
                  if (i >= al.length()) i = al.length() - 1;
                  for (j = 0,al.start() ; j <= i ; j++) at = al.next();
                  sprintf(label,rs17.gets(),i+1,al.length());
                  dialog.label(label);
                  dialog.add(new StringDialogControl(101,at,strlen(at)+1));
                  dialog.add(new ButtonDialogControl(102));
                  dialog.add(new ButtonDialogControl(103));
                  status = dialog.process();
                  switch (status)
                    {  case 102 : i++;  break;
                       case 103 : i--;  break;
                    }
               }
          } while (status != FALSE);
    }
}

