
#include "ncwin.h"
void arrange_command0(int *cstate,HighLevelEvent *,void **)
{
  //cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Arrange"),buttonmenus.match("Interrupt"));
  cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Arrange"),buttonmenus.match("Assistant"));
  *cstate = ExitState;
}

// bring to front
void arrange_command1(int *cstate,HighLevelEvent *,void **)
{EntityList list,fulllist;
 Entity *e;
 ResourceString rs0(NCARRANG+0);
 ResourceString rs1(NCARRANG+1);
  list = state.getselection();
  if (list.empty())
    cadwindow->MessageBox(rs0.gets(),rs1.gets(),MB_ICONINFORMATION);
  else
    {
       db.geometry.clearstatus();

       for (list.start() ; (e = list.next()) != NULL ; )
         {  e->setstatus(1);
            e->dbsetassociated();
         } 

       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->getstatus())
           fulllist.add(e);       

       for (fulllist.start() ; (e = fulllist.next()) != NULL ; )
           db.geometry.tofront(e);

       fulllist.destroy();

       cadwindow->paintall();
    }
  *cstate = ExitState;
}

// send to back
void arrange_command2(int *cstate,HighLevelEvent *,void **)
{EntityList list,fulllist;
 Entity *e;
 ResourceString rs2(NCARRANG+2);
 ResourceString rs3(NCARRANG+3);
  list = state.getselection();
  if (list.empty())
    cadwindow->MessageBox(rs2.gets(),rs3.gets(),MB_ICONINFORMATION);
  else
    {  db.geometry.clearstatus();

       for (list.start() ; (e = list.next()) != NULL ; )
         {  e->setstatus(1);
            e->dbsetassociated();
         } 

       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->getstatus())
           fulllist.add(e);       

       for (fulllist.start() ; (e = fulllist.next()) != NULL ; )
         db.geometry.toback(e);

       fulllist.destroy();

       cadwindow->paintall();
    }
  *cstate = ExitState;
}

/// forward by one
void arrange_command3(int *cstate,HighLevelEvent *,void **)
{EntityList list,fulllist;
 Entity *e;
 ResourceString rs4(NCARRANG+4);
 ResourceString rs5(NCARRANG+5);
  list = state.getselection();
  if (list.empty())
    cadwindow->MessageBox(rs4.gets(),rs5.gets(),MB_ICONINFORMATION);
  else
    {  db.geometry.clearstatus();

       for (list.start() ; (e = list.next()) != NULL ; )
         {  e->setstatus(1);
            e->dbsetassociated();
         } 

       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->getstatus())
           fulllist.add(e);       

       // debug
       EntityList *dblist = db.geometry.getlist();
       qDebug() << "db.geometry before change";
       for (dblist->start() ; (e = dblist->next()) != NULL ; )
       {
            qDebug() << "entity ptr  : " << e;
            qDebug() << "entity pos  : " << e->getposition();
            qDebug() << "entity type : " << e->type();
            AttributeList atlist;
            RCCHAR *at;
            atlist = e->getatlist();
            for (atlist.start() ; (at = atlist.next()) != NULL ;)
                qDebug() << "attribute   : " << QString(at);
       }

       for (fulllist.start() ; (e = fulllist.next()) != NULL ; )
         db.geometry.forward(e);

       // debug
       qDebug() << "db.geometry after change";
       dblist = db.geometry.getlist();
       for (dblist->start() ; (e = dblist->next()) != NULL ; )
       {
            qDebug() << "entity ptr  : " << e;
            qDebug() << "entity pos  : " << e->getposition();
            qDebug() << "entity type : " << e->type();
            AttributeList atlist;
            RCCHAR *at;
            atlist = e->getatlist();
            for (atlist.start() ; (at = atlist.next()) != NULL ;)
                qDebug() << "attribute   : " << QString(at);
       }

       fulllist.destroy();

       cadwindow->paintall();
    }
  *cstate = ExitState;
}

void arrange_command4(int *cstate,HighLevelEvent *,void **)
{EntityList list,fulllist;
 Entity *e;
 ResourceString rs6(NCARRANG+6);
 ResourceString rs7(NCARRANG+7);
  list = state.getselection();
  if (list.empty())
    cadwindow->MessageBox(rs6.gets(),rs7.gets(),MB_ICONINFORMATION);
  else
    {  db.geometry.clearstatus();

       for (list.start() ; (e = list.next()) != NULL ; )
         {  e->setstatus(1);
            e->dbsetassociated();
         } 

       for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
         if (e->getstatus())
           fulllist.add(e);       

       // debug
       EntityList *dblist = db.geometry.getlist();
       qDebug() << "db.geometry before change";
       for (dblist->start() ; (e = dblist->next()) != NULL ; )
       {
            qDebug() << "entity ptr  : " << e;
            qDebug() << "entity pos  : " << e->getposition();
            qDebug() << "entity type : " << e->type();
            AttributeList atlist;
            RCCHAR *at;
            atlist = e->getatlist();
            for (atlist.start() ; (at = atlist.next()) != NULL ;)
                qDebug() << "attribute   : " << QString(at);
       }

       for (fulllist.start() ; (e = fulllist.next()) != NULL ; )
         db.geometry.backward(e);

       qDebug() << "db.geometry after change";
       dblist = db.geometry.getlist();
       for (dblist->start() ; (e = dblist->next()) != NULL ; )
       {
            qDebug() << "entity ptr  : " << e;
            qDebug() << "entity pos  : " << e->getposition();
            qDebug() << "entity type : " << e->type();
            AttributeList atlist;
            RCCHAR *at;
            atlist = e->getatlist();
            for (atlist.start() ; (at = atlist.next()) != NULL ;)
                qDebug() << "attribute   : " << QString(at);
       }

       fulllist.destroy();

       cadwindow->paintall();
    }
  *cstate = ExitState;
}


