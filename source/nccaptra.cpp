#include "ncwin.h"

// capture scene and image capture tools
void capture_command0(int *cstate,HighLevelEvent *,void **)
{
    cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Capture"));
    *cstate = ExitState;
}

// capture view
void capture_command1(int *cstate,HighLevelEvent *,void **)
{
    // calls - copybitmap_toclipboard_command()
    int commandid = 222;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
    }
    *cstate = ExitState;
}

// capture area
void capture_command2(int *cstate,HighLevelEvent *,void **)
{
   // call - copybitmapwindow_toclipboard_command()
    int commandid = 223;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
    }
   *cstate = ExitState;
}
// capture bitmap
void capture_command3(int *cstate,HighLevelEvent *,void **)
{
    // calls - copybitmap_command()
    int commandid = 214;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
    }
    *cstate = ExitState;
}
// capture raytrace render
void capture_command4(int *cstate,HighLevelEvent *,void **)
{
    // calls - raytrace_command4()
    int commandid = 540;
    if ((cadwindow != 0 && cadwindow->getcadbasicwindow() == 0)&& commands.command(commandid) != NULL)
    {
        NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
        state.sendevent(nevent);
    }
    *cstate = ExitState;
}
// camera path
void capture_command5(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}
// reference path
void capture_command6(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}
// object path
void capture_command7(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}
// patch path
void capture_command8(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}
// record animation
void capture_command9(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}
// replay animation
void capture_command10(int *cstate,HighLevelEvent *,void **)
{
    *cstate = ExitState;
}

