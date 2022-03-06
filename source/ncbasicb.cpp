
#include "ncwin.h"

#if ! defined(_WIN32_WCE)
void custom_command2(int *cstate,HighLevelEvent *,void **)
{CadBasicProgram *cbp = new CadBasicProgram;
  cbp->fileopen(0);
  for (;;)
    {  if (! cbp->run(0))
         break;
       program->processmessages(0);
       if (state.getevent())
         break;
    }
  *cstate = ExitState;
}

void custom_command3(int *cstate,HighLevelEvent *,void **)
{CadBasicProgram *cbp = new CadBasicProgram;
  cbp->fileopen(0);
  new CadBasicWindow(cadwindow->getsubwindow(),cbp,0);
  *cstate = ExitState;
}

void custom_command4(int *cstate,HighLevelEvent *,void **)
{CadBasicProgram *cbp = new CadBasicProgram;
  new CadBasicWindow(cadwindow->getsubwindow(),cbp,0);
  *cstate = ExitState;
}

void custom_command5(int *cstate,HighLevelEvent *event,void **)
{ state.sendevent(0L);
  ((CadBasicWindow *)((NewCommandEvent *)event)->getwindow())->stop();
  *cstate = ExitState;
}

void custom_command6(int *cstate,HighLevelEvent *event,void **)
{ state.sendevent(0L);
  ((CadBasicWindow *)((NewCommandEvent *)event)->getwindow())->pause();
  *cstate = ExitState;
}

void custom_command7(int *cstate,HighLevelEvent *event,void **)
{ state.sendevent(0L);
  ((CadBasicWindow *)((NewCommandEvent *)event)->getwindow())->step();
  *cstate = ExitState;
}

void custom_command8(int *cstate,HighLevelEvent *event,void **)
{ state.sendevent(0L);
  ((CadBasicWindow *)((NewCommandEvent *)event)->getwindow())->run();
  *cstate = ExitState;
}

void custom_command9(int *cstate,HighLevelEvent *event,void **)
{ ((CadBasicWindow *)((NewCommandEvent *)event)->getwindow())->getcbp()->filesave(((NewCommandEvent *)event)->getwindow()->gethwnd());
  *cstate = ExitState;
}

void custom_command10(int *cstate,HighLevelEvent *event,void **)
{ ((CadBasicWindow *)((NewCommandEvent *)event)->getwindow())->getcbp()->filesaveas(((NewCommandEvent *)event)->getwindow()->gethwnd());
  *cstate = ExitState;
}

void custom_command11(int *cstate,HighLevelEvent *event,void **)
{ ((CadBasicWindow *)((NewCommandEvent *)event)->getwindow())->getcbp()->evaluate(((CadBasicWindow *)((NewCommandEvent *)event)->getwindow()));
  *cstate = ExitState;
}

#endif

