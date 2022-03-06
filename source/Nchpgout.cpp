
#include "ncwin.h"

#if MENUS != STUDENTMENUS

void hpglout_command(int *cstate,HighLevelEvent *,void **)
{ if (cadwindow->getcurrentwindow() == 0)
    cadwindow->MessageBox("Please select a window","Export HPGL",MB_ICONINFORMATION);
  else
    if (cadwindow->getcurrentwindow()->openhpgl(0))
      {  db.setdtoler(db.getdtoler()/200.0);
         if (cadwindow->getcurrentwindow()->getoffscreenbitmap() != 0)
           {  cadwindow->getcurrentwindow()->getoffscreenbitmap()->displaylist.clear();
              cadwindow->getcurrentwindow()->getoffscreenbitmap()->displaylist.seterrorstatus(1);  //  Disable the display list for while generating the HPGL code
           }
         cadwindow->getcurrentwindow()->paintevent();
         if (cadwindow->getcurrentwindow()->getoffscreenbitmap() != 0)
           {  cadwindow->getcurrentwindow()->getoffscreenbitmap()->displaylist.seterrorstatus(0);
           }
         cadwindow->getcurrentwindow()->closehpgl();
         db.setdtoler(db.getdtoler()*200.0);
      }
  *cstate = ExitState;
}

#endif
