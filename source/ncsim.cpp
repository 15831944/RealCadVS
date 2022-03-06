
#include "ncwin.h"

#if (MENUS != STUDENTMENUS) && (MENUS != FLEXICADMENUS) && ! defined(_WIN32_WCE)

#define TIME_TOL 0.001

SimulationMenuWindow::SimulationMenuWindow(Window *win,int x,int y,int w,int h,SimulationActive *aa) :
  MenuWindow(win,x,y,w,h)
{ simulationactive = aa;
  buttonmenu = simulationactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  simulationactive->setactive(1);
}

SimulationMenuWindow::~SimulationMenuWindow()
{ simulationactive->setactive(0);
}

SimulationActive simulationactive;

extern void simulation_command1(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command2(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command3(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command4(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command5(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command6(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command7(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command8(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command9(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command10(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command11(int *cstate,HighLevelEvent *event,void **);
extern void simulation_command12(int *cstate,HighLevelEvent *event,void **);

void GetItem(RCCHAR *string,int index,RCCHAR *buffer,RCCHAR sep = '\377')
{int i;
 RCCHAR *s;

 s = string;
 for (i = 0 ; i < index ; i++)
   {  if (s != 0)
        {  s = strchr(s,sep);
           if (s != 0)
             s++;
        }
   }
 if (s == 0)
   strcpy(buffer,_RCT(""));
 else
   strcpy(buffer,s);
 if (strchr(buffer,sep) != 0)
   *strchr(buffer,sep) = 0;
}

int nextaction(RCCHAR *section,RCCHAR *command,RCCHAR *action,RCCHAR *parameters,int *pos)
{ if (*pos == 0)
    {  if (strstr(command,section) == 0)
         return 0;
       *pos = strstr(command,section) - command;
       if (strpbrk(command+*pos,";\r") != 0)
         {  *pos = strpbrk(command+*pos,";\r") - command + 1;
            if (command[*pos] == '\n') (*pos)++;
         }
       else
         *pos = strlen(command);
    }

  while (command[*pos] == ' ') (*pos)++;

  strcpy(action,command+*pos);
  if (strpbrk(action," ") != 0)
    {  *pos += strpbrk(action," ") - action + 1;
       //*strpbrk(action," ") = 0;
      strpbrk(action," ",0);
    }
  else
    *pos += strlen(action);
  _strlwr(action);
  if (command[*pos] == '\n') (*pos)++;

  strcpy(parameters,command+*pos);
  if (strpbrk(parameters,";\r") != 0)
    {  *pos += strpbrk(parameters,";\r") - parameters + 1;
       //*strpbrk(parameters,";\r") = 0;
      strpbrk(parameters,";\r",0);
    }
  else
    *pos += strlen(parameters);
  if (command[*pos] == '\n') (*pos)++;
  return strlen(action) > 0 && strcmp(action,"done") != 0;
}

void simulation_command(int *cstate,HighLevelEvent *,void **)
{ *cstate = ExitState;
  if (simulationactive.getactive()) return;
  if (simulationactive.getbuttonmenu() == NULL)
    {ButtonMenu *menu = new ButtonMenu(0,0,12,12,15);
       if (menu != NULL)
         {  commands.add(1,22001,0,simulation_command1, (char*)"Insert/Change Producer");
            commands.add(1,22002,0,simulation_command2, (char*)"Insert/Change Consumer");
            commands.add(1,22003,0,simulation_command3, (char*)"Insert/Change Machine");
            commands.add(1,22004,0,simulation_command4, (char*)"Insert/Change Conveyor");
            commands.add(1,22005,0,simulation_command5, (char*)"Insert/Change Product");
            commands.add(1,22006,0,simulation_command6, (char*)"Insert/Change Vehicle");
            commands.add(1,22007,0,simulation_command7, (char*)"Insert/Change Track or Decision Point");
            commands.add(1,22008,0,simulation_command8, (char*)"Insert/Change Buffer");
            commands.add(1,22009,0,simulation_command9, (char*)"Insert/Change Chart");
            commands.add(1,22010,0,simulation_command10, (char*)"Insert/Change Camera Path");
            commands.add(1,22011,0,simulation_command11, (char*)"Insert/Change Reference Path");
            commands.add(1,22012,0,simulation_command12, (char*)"Run Simulation");
            menu->add(new Button(-1,-1, (char*)"Simulation1",22001));
            menu->add(new Button(-1,-1, (char*)"Simulation2",22002));
            menu->add(new Button(-1,-1, (char*)"Simulation3",22003));
            menu->add(new Button(-1,-1, (char*)"Simulation4",22004));
            menu->add(new Button(-1,-1, (char*)"Simulation5",22005));
            menu->add(new Button(-1,-1, (char*)"Simulation6",22006));
            menu->add(new Button(-1,-1, (char*)"Simulation7",22007));
            menu->add(new Button(-1,-1, (char*)"Simulation8",22008));
            menu->add(new Button(-1,-1, (char*)"Simulation9",22009));
            menu->add(new Button(-1,-1, (char*)"Simulation10",22010));
            menu->add(new Button(-1,-1, (char*)"Simulation11",22011));
            menu->add(new Button(-1,-1, (char*)"Simulation12",22012));
            simulationactive.setbuttonmenu(menu);
         }
    }
  if (simulationactive.getbuttonmenu() == NULL)
    { RCMessageBox(cadwindow->gethwnd(),_RCT("Failed to create the simulation button menu"),_RCT("Simulation"),MB_ICONSTOP);
       return;
    }
  new SimulationMenuWindow(cadwindow->getsubwindow(),
                          int(cadwindow->getsubwindow()->getwidth())-120,0,
                          48,144,&simulationactive);
  *cstate = ExitState;
}

class RBSimulation12_data
  {public:
     View3dWindow *window;
     View3dOffScreenBitmap *bitmap;
     double starttime,finishtime,t,dt;
     int frame,format,wireframe,hide,shadebefore,shadeafter,shading;
     RCCHAR scene[256],prefix[128];
     GeneralList conveyors,cameras,camerareferences,tracks,buffers,consumers,products,realproducts,realvehicles,vehicles,producers,events,machines,charts,decisionpoints;
     PAVIFILE avifile;
     PAVISTREAM avistream,avicompressedstream;
     AVICOMPRESSOPTIONS opts;
     RBSimulation12_data();
     ~RBSimulation12_data();
  } *simulationdata;

enum SimEventType {  MoveTo,Call  };

class RealProduct;
class RealVehicle;

class SimulationPath
  {private:
     EntityList list;
     double length;
   public:
     double starttime,finishtime,startheight,finishheight;
     SimulationPath(EntityList l,double,double,double,double);
     ~SimulationPath() {  list.destroy();  }
     Point getposition(double t);
     Point getdirection(double t);
     double getlength(void)  {  return length;  }
  };

class Camera
  {private:
     int fixreference;
     Point reference;
     double starttime,finishtime,startheight,finishheight;
     double startfocaldistance,finishfocaldistance;
     SimulationPath *path;
   public:
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     int getfixreference(void)  {  return fixreference;  }
     void update(double t,int updatereference,Point up,Point *eye,Point *ref);
     static int isacamera(Entity *);
  };


class CameraReference
  {private:
     double starttime,finishtime,startheight,finishheight;
     SimulationPath *path;
   public:
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     void update(double t,Point *,int *);
     static int isacamerareference(Entity *);
  };

#define MAX_ACTIONLENGTH 1000

class Producer
  {private:
     Group *group;
     Point origin,xaxis,yaxis;
     double size,rate,height;
     RCCHAR name[300],actions[MAX_ACTIONLENGTH];

     double lasttime;

   public:
     Producer(void)  {  lasttime = 0;  group = 0;  }
     void setcoordinatesystem(Point,Point,Point);
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     void update(double t);
     EntityList graphics(void);
     static int isaproducer(Entity *);
  };

class Consumer
  {private:
     Point origin,xaxis,yaxis;
     double size,height;
     RCCHAR name[300],actions[MAX_ACTIONLENGTH];
   public:
     void setcoordinatesystem(Point,Point,Point);
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     EntityList graphics(void);
     static int isaconsumer(Entity *);
     void update(double t);
  };

class Conveyor
  {private:
     double size,speed,height;
     RCCHAR name[300],actions[MAX_ACTIONLENGTH];
     SimulationPath *path;
   public:
     void getdefaults(void);
     void setdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     void update(double t);
     void move(RealProduct *,double dt,double *t);
     static int isaconveyor(Entity *);
     int match(RCCHAR *n)  {  return strcmp(n,name) == 0;  }
  };

class Buffer
  {private:
     Point origin,xaxis,yaxis;
     double width,height,productheight;
     RCCHAR name[300],actions[MAX_ACTIONLENGTH];

     double lasttime;
     int nitems;

   public:
     Buffer(void)  {  lasttime = 0;  nitems = 0;  }
     void setcoordinatesystem(Point,Point,Point);
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     void update(double t);
     EntityList graphics(void);
     static int isabuffer(Entity *);
     int match(RCCHAR *n)  {  return strcmp(n,name) == 0;  }
  };

class Track
  {private:
     double size,speed,height;
     RCCHAR name[300],actions[MAX_ACTIONLENGTH];
     GeneralList paths;
   public:
     void getdefaults(void);
     void setdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     void update(double t);
     void move(RealVehicle *,double dt,double *t);
     SimulationPath *getpath(int);
     double gett(RCCHAR *decisionpoint);
     static int isatrack(Entity *);
  };

class DecisionPoint
  {private:
     Point origin,xaxis,yaxis;
     double size,xscale,yscale,zscale,length,width,height;
     RCCHAR name[300],trackname[300],actions[MAX_ACTIONLENGTH];
   public:
     void setcoordinatesystem(Point,Point,Point);
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     EntityList graphics(void);
     static int isadecisionpoint(Entity *);
     int match(RCCHAR *n)  {  return strcmp(n,name) == 0;  }
     Point getorigin(void)  {  return origin;  }
     RCCHAR *getactions(void)  {  return actions;  }
  };

class Machine
  {private:
     Point origin,xaxis,yaxis;
     double size,height,ninputsrequired;
     RCCHAR name[300],actions[MAX_ACTIONLENGTH];
     Group *group;

     int ninputs,full;
     double currentheight;
     GeneralList realproducts;

   public:
     Machine()  {  ninputs = 0;  currentheight = 0;  group = 0;  full = 0;  }
     void setcoordinatesystem(Point,Point,Point);
     void getdefaults(void);
     void setdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     void update(double t);
     void move(RealProduct *,double dt,double *t);
     EntityList graphics(void);
     static int isamachine(Entity *);
     int match(RCCHAR *n)  {  return strcmp(n,name) == 0;  }
     void setfull(int f)  {  full = f;  }
  };

class Vehicle
  {private:
     Point origin,xaxis,yaxis;
     double size,xscale,yscale,zscale,length,width,height,lx,ly,lz;
     RCCHAR name[300],figure[300],actions[MAX_ACTIONLENGTH];
   public:
     void setcoordinatesystem(Point,Point,Point);
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     EntityList graphics(void);
     static int isavehicle(Entity *);
     int match(RCCHAR *n)  {  return strcmp(n,name) == 0;  }
     Figure *create(Group *,Point,Point,Point);
  };

class Product
  {private:
     Point origin,xaxis,yaxis;
     double size,xscale,yscale,zscale,length,width,height;
     RCCHAR name[300],figure[300],actions[MAX_ACTIONLENGTH];
   public:
     void setcoordinatesystem(Point,Point,Point);
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     EntityList graphics(void);
     static int isaproduct(Entity *);
     int match(RCCHAR *n)  {  return strcmp(n,name) == 0;  }
     Figure *create(Group *,Point,Point,Point);
     double getheight(void)  {  return height;  }
     double getwidth(void)  {  return width;  }
     double getlength(void)  {  return length;  }
  };

class RealProduct
  {private:
     Product *product;
     Figure *figure;
     Producer *producer;
     Conveyor *conveyor;
     Machine *machine;
     RealVehicle *realvehicle;
     Buffer *buffer;
     double t,time;

   public:
     RealProduct(Producer *p,Product *,Figure *);
     RealProduct(Machine *m,Product *,Figure *);
     ~RealProduct();
     void moveonto(Conveyor *c);
     void moveonto(Machine *m);
     void moveonto(RealVehicle *rv);
     void moveonto(Buffer *b);
     void newtransform(Point,Point,Point,Point,Point);
     void update(double t);
     double getlength(void)  {  return product->getlength();  }
     double getwidth(void)  {  return product->getwidth();  }
     double getheight(void)  {  return product->getheight();  }
     double gett(void)  {  return t;  }
     int match(Conveyor *c)  {  return conveyor == c;  }
     int match(RealVehicle *rv)  {  return realvehicle == rv;  }
     int match(Buffer *b)  {  return buffer == b;  }
     int match(Machine *m)  {  return machine == m;  }
  };

class Chart
  {private:
     Point origin,xaxis,yaxis;
     double width,height,xmaximum,ymaximum,xminimum,yminimum,xstep,ystep,xstepsize,ystepsize,textheight;
     RCCHAR name[300],label[300],xlabel[300],ylabel[300];
     Group *g;

     double time;
     double count;

   public:
     Chart()  {  time = count = 0.0;  g = 0;  }
     void setcoordinatesystem(Point,Point,Point);
     void setdefaults(void);
     void getdefaults(void);
     void load(Group *);
     void save(Group *);
     int setup(int);
     EntityList graphics(void);
     static int isachart(Entity *);
     int match(RCCHAR *n)  {  return strcmp(n,name) == 0;  }
     int match(char *n)  {  return strcmp(CHAR2RCCHR(n),name) == 0;  }

     void clear(void);
     void plotcount(int colour,int layer,double period,double c);

  };

class RealVehicle
  {private:
     Figure *figure;
     double t,time;
     RCCHAR destination[300];

     Producer *producer;
     Track *track;
     Vehicle *vehicle;

   public:
     RealVehicle(Producer *p,Vehicle *,Figure *,RCCHAR *);
     ~RealVehicle();
     void moveonto(Track *t,RCCHAR *decisionpoint);
     void newtransform(Point,Point,Point,Point,Point);
     void update(double t);
     RCCHAR *getdestination(void)  {  return destination;  }
     void setdestination(RCCHAR *d)  {  strcpy(destination,d);  time = simulationdata->t;  }
     void move(RealProduct *,double dt,double *t);
  };

class SimulationEvent
  {public:
     SimEventType type;
     RealProduct *realproduct;
     RealVehicle *realvehicle;
     RCCHAR destination[300];
     SimulationEvent(SimEventType t,RealProduct *r,RCCHAR *d) {  type = t;  realproduct = r;  realvehicle = 0;  strcpy(destination,d);  }
     SimulationEvent(SimEventType t,RealVehicle *v,RCCHAR *d) {  type = t;  realvehicle = v;  realproduct = 0;  strcpy(destination,d);  }
  };

RealProduct::RealProduct(Producer *pr,Product *p,Figure *f)
{ producer = pr;
  conveyor = 0;
  machine = 0;
  buffer = 0;
  realvehicle = 0;
  product = p;
  figure = f;
  time = simulationdata->t;
  t = 0;
}

RealProduct::RealProduct(Machine *m,Product *p,Figure *f)
{ producer = 0;
  conveyor = 0;
  realvehicle = 0;
  buffer = 0;
  machine = m;
  product = p;
  figure = f;
  time = simulationdata->t;
  t = 0;
}

RealProduct::~RealProduct()
{ db.geometry.del(figure);
}

void RealProduct::moveonto(Conveyor *c)
{ if (machine != 0)
    machine->setfull(0);
  machine = 0;
  producer = 0;
  realvehicle = 0;
  buffer = 0;
  conveyor = c;
  time = simulationdata->t;
  t = 0;
}

void RealProduct::moveonto(Machine *m)
{ if (machine != 0)
    machine->setfull(0);
  machine = m;
  producer = 0;
  conveyor = 0;
  realvehicle = 0;
  buffer = 0;
  time = simulationdata->t;
  t = 0;
}

void RealProduct::moveonto(RealVehicle *rv)
{ if (machine != 0)
    machine->setfull(0);
  machine = 0;
  producer = 0;
  conveyor = 0;
  buffer = 0;
  realvehicle = rv;
  time = simulationdata->t;
  t = 0;
}

void RealProduct::moveonto(Buffer *b)
{ if (machine != 0)
    machine->setfull(0);
  machine = 0;
  producer = 0;
  conveyor = 0;
  realvehicle = 0;
  buffer = b;
  time = simulationdata->t;
  t = 0;
}

void RealProduct::update(double ct)
{ if (conveyor != 0)
    conveyor->move(this,ct - time,&t);
  if (realvehicle != 0)
    realvehicle->move(this,ct - time,&t);
  time = ct;
}

void RealProduct::newtransform(Point origin,Point xaxis,Point yaxis,Point scale,Point rotation)
{ figure->newtransform(origin,xaxis,yaxis,scale,rotation);
}

RealVehicle::RealVehicle(Producer *pr,Vehicle *v,Figure *f,RCCHAR *d)
{ producer = pr;
  track = 0;
  vehicle = v;
  figure = f;
  time = simulationdata->t;
  t = 0;
  strcpy(destination,d);
}

RealVehicle::~RealVehicle()
{ db.geometry.del(figure);
}

void RealVehicle::move(RealProduct *rp,double,double *)
{int j1;
 SimulationPath *path;
  j1 = (int)t;
  path = track->getpath(j1);
  if (path != 0)
    rp->newtransform(path->getposition(t-j1),path->getdirection(t-j1),Point(0.0,0.0,1.0).cross(path->getdirection(t-j1)),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0));
}

void RealVehicle::moveonto(Track *tr,RCCHAR *decisionpoint)
{ producer = 0;
  track = tr;
  time = simulationdata->t;
  t = track->gett(decisionpoint);
  if (track->getpath((int)t) != 0)
    newtransform(track->getpath((int)t)->getposition(0.0),track->getpath((int)t)->getdirection(0.0),Point(0.0,0.0,1.0).cross(track->getpath((int)t)->getdirection(0.0)),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0));
}

void RealVehicle::update(double ct)
{SimulationEvent *e;
 RCCHAR vehiclename[300],newdestination[300];
  if (track != 0)
    track->move(this,ct - time,&t);
  time = ct;
  if (strlen(destination) == 0)
    {  for (simulationdata->events.start() ; (e = (SimulationEvent *) simulationdata->events.next()) != 0 ; )
         if (e->type == Call)
           {  GetItem(e->destination,0,vehiclename,',');
              GetItem(e->destination,1,newdestination,',');
              if (vehicle->match(vehiclename))
                {  strcpy(destination,newdestination);
                   simulationdata->events.del(e);
                   simulationdata->events.start();
                   break;
                }
         }
    }
}

void RealVehicle::newtransform(Point origin,Point xaxis,Point yaxis,Point scale,Point rotation)
{ figure->newtransform(origin,xaxis,yaxis,scale,rotation);
}

void Camera::setdefaults(void)
{ v.setreal("sm::starttime",starttime);
  v.setreal("sm::finishtime",finishtime);
  v.setinteger("sm::fixref",fixreference);
  v.setreal("sm::startfocaldistance",startfocaldistance);
  v.setreal("sm::finishfocaldistance",finishfocaldistance);
  v.setreal("sm::startheight",startheight);
  v.setreal("sm::finishheight",finishheight);
}

void Camera::getdefaults(void)
{ starttime = v.getreal("sm::starttime");
  finishtime = v.getreal("sm::finishtime");
  fixreference = v.getinteger("sm::fixref");
  startfocaldistance = v.getreal("sm::startfocaldistance");
  finishfocaldistance = v.getreal("sm::startfocaldistance");
  startheight = v.getreal("sm::startheight");
  finishheight = v.getreal("sm::startheight");
}

void Camera::load(Group *group)
{EntityList list;
 PointE *point;
  sscanf(group->getinstance(),"SM::Camera:%lf %lf %d %lf %lf %lf %lf",
        &starttime,&finishtime,&fixreference,&startfocaldistance,&finishfocaldistance,&startheight,&finishheight);
  if (finishfocaldistance == 0.0)
    finishfocaldistance = startfocaldistance;

  list = group->getlist().copy();
  list.start();
  if ((point = (PointE *)list.next()) != 0)
    {  list.del(point);
       reference = point->getp();
    }
  path = new SimulationPath(list,starttime,finishtime,startheight,finishheight);
}

void Camera::save(Group *group)
{RCCHAR info[300];
 BitMask options(32),change(32);
  sprintf(info,"SM::Camera:%.4lf %.4lf %d %.4lf %.4lf %.4lf %.4lf",starttime,finishtime,fixreference,startfocaldistance,finishfocaldistance,startheight,finishheight);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);
}

int Camera::setup(int change)
{Dialog dialog("Simulationcamera_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting camera path");
  dialog.add(dc = new RealDialogControl(100,&starttime,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new RealDialogControl(101,&finishtime,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(new CheckBoxDialogControl(102,&fixreference));
  dialog.add(dc = new RealDialogControl(103,&startfocaldistance,0.1,0.000001,100000.0));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&finishfocaldistance,0.1,0.000001,100000.0));
  dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&startheight,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(dc = new RealDialogControl(106,&finishheight,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1106,dc));
  return dialog.process() == TRUE;
}

void Camera::update(double t,int updatereference,Point up,Point *eye,Point *ref)
{double focaldistance;
  if (t >= path->starttime - TIME_TOL && t <= path->finishtime + TIME_TOL)
    {  if (up.cross(path->getdirection(t)).length() < 0.01)
         up += *eye - *ref;
       *eye = path->getposition(t);
       focaldistance = startfocaldistance + (finishfocaldistance - startfocaldistance) * (t  - path->starttime) / (path->finishtime - path->starttime);
       if (updatereference)
         {  if (fixreference)
              *ref = *eye + (reference - *eye).normalize() * focaldistance;
            else
              *ref = *eye + path->getdirection(t) * focaldistance;
         }
       else
         *ref = *eye + (*ref - *eye).normalize() * focaldistance;
     }
}

int Camera::isacamera(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Camera:",11) == 0;
}

void CameraReference::setdefaults(void)
{ v.setreal("sm::starttime",starttime);
  v.setreal("sm::finishtime",finishtime);
  v.setreal("sm::startheight",startheight);
  v.setreal("sm::finishheight",finishheight);
}

void CameraReference::getdefaults(void)
{ starttime = v.getreal("sm::starttime");
  finishtime = v.getreal("sm::finishtime");
  startheight = v.getreal("sm::startheight");
  finishheight = v.getreal("sm::startheight");
}

void CameraReference::load(Group *group)
{EntityList list;
  sscanf(group->getinstance(),"SM::CameraReference:%lf %lf %lf %lf",
           &starttime,&finishtime,&startheight,&finishheight);
  list = group->getlist().copy();
  path = new SimulationPath(list,starttime,finishtime,startheight,finishheight);
}

void CameraReference::save(Group *group)
{RCCHAR info[300];
 BitMask options(32),change(32);
  sprintf(info,"SM::CameraReference:%.4lf %.4lf %.4lf %.4lf",starttime,finishtime,startheight,finishheight);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);
}

int CameraReference::setup(int change)
{Dialog dialog("SimulationReference_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting camera reference path");
  dialog.add(dc = new RealDialogControl(100,&starttime,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1100,dc));
  dialog.add(dc = new RealDialogControl(101,&finishtime,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&startheight));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&finishheight));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  return dialog.process() == TRUE;
}

void CameraReference::update(double t,Point *ref,int *updated)
{ if (t >= path->starttime - TIME_TOL && t <= path->finishtime + TIME_TOL)
    {  *ref = path->getposition(t);
       *updated = 1;
    }
}

int CameraReference::isacamerareference(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::CameraReference:",20) == 0;
}

void Producer::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void Producer::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  size = v.getreal("sm::size");
  rate = v.getreal("sm::rate");
  height = v.getreal("sm::height");
  strcpy(actions,v.getstring("sm::actions"));
}

void Producer::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setreal("sm::rate",rate);
  v.setreal("sm::height",height);
  v.setstring("sm::actions",actions);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void Producer::load(Group *g)
{Entity *e;
 EntityList list;
  group = g;
  sscanf(group->getinstance(),"SM::Producer:%lf %lf %lf\377",&size,&rate,&height);
  GetItem(group->getinstance(),1,name,(RCCHAR)255);
  GetItem(group->getinstance(),2,actions,(RCCHAR)255);

  list = group->getlist();

  list.start();

  e = list.next();
  if (e->isa(point_entity))
    origin = ((PointE *)e)->getp();
  e = list.next();
  if (e->isa(line_entity))
    xaxis = ((Line *)e)->direction(0.0);
  e = list.next();
  if (e->isa(line_entity))
    yaxis = ((Line *)e)->direction(0.0);

}

void Producer::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Producer:%.4lf %.4lf %.4lf\377%s\377%s",
             size,rate,height,name,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  newlist = graphics();
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int Producer::setup(int change)
{Dialog dialog("SimulationProducer_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting producer");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&rate,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&height,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new StringDialogControl(104,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int Producer::isaproducer(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Producer:",13) == 0;
}

EntityList Producer::graphics(void)
{EntityList list;
 BitMask options(32);

  list.add(new PointE(origin));
  list.add(new Line(origin-xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size,origin+xaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size,origin+xaxis/4.0*size+yaxis/4.0*size));
  list.add(new Line(origin+xaxis/2.0*size,origin+xaxis/4.0*size-yaxis/4.0*size));

  return list;
}

void Producer::update(double t)
{Product *p;
 Vehicle *v;
 RealProduct *rp;
 RealVehicle *rv;
 Chart *c;
 int pos;
 RCCHAR command[300],parameters[300],product[300],destination[300];
  if (rate != 0.0 && t - lasttime > 1.0 / rate || t == 0.0 && rate == 0)
    {  lasttime = t;
       pos = 0;
       while (nextaction(_RCT("AtCreate"),actions,command,parameters,&pos))
         if (strcmp(command,"produce") == 0)
           {  GetItem(parameters,0,product,',');
              GetItem(parameters,1,destination,',');
              for (simulationdata->products.start() ; (p = (Product *) simulationdata->products.next()) != 0 ; )
                if (p->match(product))
                  {  simulationdata->realproducts.add(rp = new RealProduct(this,p,p->create(group,origin+Point(0.0,0.0,height),xaxis,yaxis)));
                     simulationdata->events.add(new SimulationEvent(MoveTo,rp,destination));

                     for (simulationdata->charts.start() ; (c = (Chart *) simulationdata->charts.next()) != 0 ; )
                       if (c->match((char*)"ChartA"))
                         c->plotcount(1,1,1.1,1.0);

                     break;
                  }
              for (simulationdata->vehicles.start() ; (v = (Vehicle *) simulationdata->vehicles.next()) != 0 ; )
                if (v->match(product))
                  {  simulationdata->realvehicles.add(rv = new RealVehicle(this,v,v->create(group,origin+Point(0.0,0.0,height),xaxis,yaxis),_RCT("")));
                     simulationdata->events.add(new SimulationEvent(MoveTo,rv,destination));
                     break;
                  }
           }
         else if (strcmp(command,"call") == 0)
           simulationdata->events.add(new SimulationEvent(Call,rp,parameters));
    }
}

void Buffer::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void Buffer::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  width = v.getreal("sm::bufferwidth");
  height = v.getreal("sm::bufferheight");
  productheight = v.getreal("sm::height");
  strcpy(actions,v.getstring("sm::actions"));
}

void Buffer::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::bufferwidth",width);
  v.setreal("sm::bufferheight",height);
  v.setreal("sm::height",height);
  v.setstring("sm::actions",actions);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void Buffer::load(Group *group)
{Entity *e;
 EntityList list;
  sscanf(group->getinstance(),"SM::Buffer:%lf %lf %lf\377",&width,&height,&productheight);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,actions);

  list = group->getlist();

  list.start();

  e = list.next();
  if (e->isa(point_entity))
    origin = ((PointE *)e)->getp();
  e = list.next();
  if (e->isa(line_entity))
    xaxis = ((Line *)e)->direction(0.0);
  e = list.next();
  if (e->isa(line_entity))
    yaxis = ((Line *)e)->direction(0.0);

}

void Buffer::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Buffer:%.4lf %.4lf %.4lf\377%s\377%s",
             width,height,productheight,name,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  newlist = graphics();
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int Buffer::setup(int change)
{Dialog dialog("SimulationBuffer_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting buffer");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&width,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&height,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&productheight,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new StringDialogControl(104,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int Buffer::isabuffer(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Buffer:",11) == 0;
}

EntityList Buffer::graphics(void)
{EntityList list;
 BitMask options(32);

  list.add(new PointE(origin));
  list.add(new Line(origin,origin+xaxis*width));
  list.add(new Line(origin+xaxis*width,origin+xaxis*width+yaxis*height));
  list.add(new Line(origin+xaxis*width+yaxis*height,origin+yaxis*height));
  list.add(new Line(origin+yaxis*height,origin));

  list.add(new Line(origin,origin+xaxis*width+yaxis*height));
  list.add(new Line(origin+xaxis*width,origin+yaxis*height));

  return list;
}

void Buffer::update(double)
{SimulationEvent *e;
 int ny;
  //  Move product in the event list onto the buffer
  for (simulationdata->events.start() ; (e = (SimulationEvent *) simulationdata->events.next()) != 0 ; )
    if (strcmp(name,e->destination) == 0)
      {  e->realproduct->moveonto(this);
         ny = int(height / e->realproduct->getwidth());
         e->realproduct->newtransform(origin+Point(e->realproduct->getlength()*int(nitems/ny),e->realproduct->getwidth()*int(nitems%ny),height),xaxis,yaxis,Point(1.0,1.0,1.0),Point(0.0,0.0,0.0));
         nitems++;
         simulationdata->events.del(e);
         simulationdata->events.start();
      }
}

void Consumer::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void Consumer::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  size = v.getreal("sm::size");
  height = v.getreal("sm::height");
  strcpy(actions,v.getstring("sm::actions"));
}

void Consumer::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setreal("sm::height",height);
  v.setstring("sm::actions",actions);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void Consumer::load(Group *group)
{Entity *e;
 EntityList list;
  sscanf(group->getinstance(),"SM::Consumer:%lf %lf\377",&size,&height);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,actions);

  list = group->getlist();

  list.start();

  e = list.next();
  if (e->isa(point_entity))
    origin = ((PointE *)e)->getp();
  e = list.next();
  if (e->isa(line_entity))
    xaxis = ((Line *)e)->direction(0.0);
  e = list.next();
  if (e->isa(line_entity))
    yaxis = ((Line *)e)->direction(0.0);

}

void Consumer::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Consumer:%.4lf %.4lf\377%s\377%s",
             size,height,name,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  newlist = graphics();
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int Consumer::setup(int change)
{Dialog dialog("SimulationConsumer_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting consumer");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(103,&height,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new StringDialogControl(104,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int Consumer::isaconsumer(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Consumer:",13) == 0;
}

EntityList Consumer::graphics(void)
{EntityList list;
 BitMask options(32);

  list.add(new PointE(origin));
  list.add(new Line(origin-xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size,origin+xaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size,origin+xaxis/4.0*size+yaxis/4.0*size));
  list.add(new Line(origin+xaxis/2.0*size,origin+xaxis/4.0*size-yaxis/4.0*size));
  list.add(new Line(origin-xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin-xaxis/2.0*size+yaxis/2.0*size));

  return list;
}

void Consumer::update(double)
{SimulationEvent *e;
  for (simulationdata->events.start() ; (e = (SimulationEvent *) simulationdata->events.next()) != 0 ; )
    if (strcmp(name,e->destination) == 0)
      {  simulationdata->realproducts.del(e->realproduct);
         delete e->realproduct;
         simulationdata->events.del(e);
         simulationdata->events.start();

      }
}

void Product::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void Product::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  size = v.getreal("sm::size");
  length = v.getreal("sm::length");
  width = v.getreal("sm::width");
  height = v.getreal("sm::height");
  xscale = v.getreal("sm::xscale");
  yscale = v.getreal("sm::yscale");
  zscale = v.getreal("sm::zscale");
  strcpy(figure,v.getstring("sm::figure"));
  strcpy(actions,v.getstring("sm::actions"));
}

void Product::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setreal("sm::length",length);
  v.setreal("sm::width",width);
  v.setreal("sm::height",height);
  v.setreal("sm::xscale",xscale);
  v.setreal("sm::yscale",yscale);
  v.setreal("sm::zscale",zscale);
  v.setstring("sm::figure",figure);
  v.setstring("sm::actions",actions);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void Product::load(Group *group)
{Entity *e;
 EntityList list;
  sscanf(group->getinstance(),"SM::Product:%lf %lf %lf %lf %lf %lf %lf\377",
                  &size,&length,&width,&height,&xscale,&yscale,&zscale);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,figure);
  GetItem(group->getinstance(),3,actions);

  list = group->getlist();

  list.start();

  e = list.next();
  if (e->isa(point_entity))
    origin = ((PointE *)e)->getp();
  e = list.next();
  if (e->isa(line_entity))
    xaxis = ((Line *)e)->direction(0.0);
  e = list.next();
  if (e->isa(line_entity))
    yaxis = ((Line *)e)->direction(0.0);

}

void Product::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Product:%.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf\377%s\377%s\377%s",
             size,length,width,height,xscale,yscale,zscale,name,figure,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  newlist = graphics();
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int Product::setup(int change)
{Dialog dialog("SimulationProduct_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting product");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&length,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&width,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&height,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&xscale,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(dc = new RealDialogControl(106,&yscale,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(107,&zscale,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(new StringDialogControl(108,figure,300));
  dialog.add(new StringDialogControl(109,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int Product::isaproduct(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Product:",12) == 0;
}

EntityList Product::graphics(void)
{EntityList list;
 BitMask options(32);

  list.add(new PointE(origin));
  list.add(new Line(origin-xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size-yaxis/2.0*size));

  list.add(new Line(origin-xaxis/2.0*size+yaxis/2.0*size,origin+yaxis*size));
  list.add(new Line(origin+xaxis/2.0*size+yaxis/2.0*size,origin+xaxis*size+yaxis*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin+xaxis*size));
  list.add(new Line(origin+yaxis*size,origin+xaxis*size+yaxis*size));
  list.add(new Line(origin+xaxis*size+yaxis*size,origin+xaxis*size));

  list.add(new Text(CHAR2RCCHR("Arial"),origin+xaxis*size*1.2,xaxis,yaxis,0.0,size/4.0,0.0,0.0,0,CHAR2RCCHR(name),0,options));

  return list;
}

Figure *Product::create(Group *group,Point origin,Point xaxis,Point yaxis)
{BitMask options(32);
 Figure *f;
 EntityHeader header;
  if (group != 0)
    header.change(group->getcolour(),group->getlayer(),group->getstyle(),group->getweight());
  header.setvisible(simulationdata->window->getvisibility());
  f = new Figure(header,figure,_RCT(""),origin,xaxis,yaxis,Point(xscale,yscale,zscale),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
  db.geometry.add(f);
  return f;
}


//
//  Vehicle
//
void Vehicle::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void Vehicle::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  size = v.getreal("sm::size");
  length = v.getreal("sm::length");
  width = v.getreal("sm::width");
  height = v.getreal("sm::height");
  xscale = v.getreal("sm::xscale");
  yscale = v.getreal("sm::yscale");
  zscale = v.getreal("sm::zscale");
  strcpy(figure,v.getstring("sm::figure"));
  strcpy(actions,v.getstring("sm::actions"));
}

void Vehicle::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setreal("sm::length",length);
  v.setreal("sm::width",width);
  v.setreal("sm::height",height);
  v.setreal("sm::xscale",xscale);
  v.setreal("sm::yscale",yscale);
  v.setreal("sm::zscale",zscale);
  v.setstring("sm::figure",figure);
  v.setstring("sm::actions",actions);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void Vehicle::load(Group *group)
{Entity *e;
 EntityList list;
  sscanf(group->getinstance(),"SM::Vehicle:%lf %lf %lf %lf %lf %lf %lf\377",
                  &size,&length,&width,&height,&xscale,&yscale,&zscale);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,figure);
  GetItem(group->getinstance(),3,actions);

  list = group->getlist();

  list.start();

  e = list.next();
  if (e->isa(point_entity))
    origin = ((PointE *)e)->getp();
  e = list.next();
  if (e->isa(line_entity))
    xaxis = ((Line *)e)->direction(0.0);
  e = list.next();
  if (e->isa(line_entity))
    yaxis = -((Line *)e)->direction(0.0);

}

void Vehicle::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Vehicle:%.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf\377%s\377%s\377%s",
             size,length,width,height,xscale,yscale,zscale,name,figure,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  newlist = graphics();
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int Vehicle::setup(int change)
{Dialog dialog("SimulationVehicle_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting vehicle");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&length,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&width,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&height,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&xscale,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(dc = new RealDialogControl(106,&yscale,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(107,&zscale,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(new StringDialogControl(108,figure,300));
  dialog.add(new StringDialogControl(109,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int Vehicle::isavehicle(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Vehicle:",12) == 0;
}

EntityList Vehicle::graphics(void)
{EntityList list;
 BitMask options(32);

  list.add(new PointE(origin));
  list.add(new Line(origin,origin+xaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size,origin+xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin-xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size-yaxis/2.0*size,origin-xaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size,origin-xaxis/4.0*size+yaxis/2.0*size));
  list.add(new Line(origin-xaxis/4.0*size+yaxis/2.0*size,origin+yaxis*size/2.0));
  list.add(new Line(origin+yaxis*size/2.0,origin));
  list.add(new Circle(origin-xaxis/4.0*size-yaxis/2.0*size,xaxis,yaxis,size/8.0,M_PI,M_PI));
  list.add(new Circle(origin+xaxis/4.0*size-yaxis/2.0*size,xaxis,yaxis,size/8.0,M_PI,M_PI));

  list.add(new Text(CHAR2RCCHR("Arial"),origin+xaxis*size*0.1+yaxis*size*0.1,xaxis,yaxis,0.0,size/4.0,0.0,0.0,0,CHAR2RCCHR(name),0,options));

  return list;
}

Figure *Vehicle::create(Group *group,Point origin,Point xaxis,Point yaxis)
{BitMask options(32);
 Figure *f;
 EntityHeader header;
  if (group != 0)
    header.change(group->getcolour(),group->getlayer(),group->getstyle(),group->getweight());
  header.setvisible(simulationdata->window->getvisibility());
  f = new Figure(header,figure,_RCT(""),origin,xaxis,yaxis,Point(xscale,yscale,zscale),Point(0.0,0.0,0.0),Point(0.0,0.0,0.0),0,0,0,NULL,options);
  db.geometry.add(f);
  return f;
}

void Conveyor::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  size = v.getreal("sm::size");
  speed = v.getreal("sm::speed");
  height = v.getreal("sm::height");
  strcpy(actions,v.getstring("sm::actions"));
}

void Conveyor::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setreal("sm::speed",speed);
  v.setreal("sm::height",height);
  v.setstring("sm::actions",actions);

}

void Conveyor::load(Group *group)
{EntityList list;
  sscanf(group->getinstance(),"SM::Conveyor:%lf %lf %lf\377",&size,&speed,&height);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,actions);

  list = group->getlist().copy();
  path = new SimulationPath(list,0.0,1.0,height,height);

}

void Conveyor::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Conveyor:%.4lf %.4lf %.4lf\377%s\377%s",
             size,speed,height,name,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

}

int Conveyor::setup(int change)
{Dialog dialog("SimulationConveyor_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting conveyor");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&speed,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&height,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new StringDialogControl(104,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int Conveyor::isaconveyor(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Conveyor:",13) == 0;
}

void Conveyor::update(double)
{SimulationEvent *e;
  //  Move product in the event list onto the conveyor
  for (simulationdata->events.start() ; (e = (SimulationEvent *) simulationdata->events.next()) != 0 ; )
    if (strcmp(name,e->destination) == 0)
      {  e->realproduct->moveonto(this);
         simulationdata->events.del(e);
         simulationdata->events.start();
      }
}

void Conveyor::move(RealProduct *rp,double dt,double *t)
{int pos;
 RCCHAR command[300],parameters[300];
  if (*t < 1.0)
    {  *t += dt * speed / path->getlength();
       if (*t >= 1.0)
         {  *t = 1.0;
            //  Move product to destination
            pos = 0;
            while (nextaction(_RCT("AtEnd"),actions,command,parameters,&pos))
              if (strcmp(command,"moveto") == 0)
                simulationdata->events.add(new SimulationEvent(MoveTo,rp,parameters));
              else if (strcmp(command,"call") == 0)
                simulationdata->events.add(new SimulationEvent(Call,rp,parameters));
         }
       rp->newtransform(path->getposition(*t),path->getdirection(*t),Point(0.0,0.0,1.0).cross(path->getdirection(*t)),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0));
    }
}

void Track::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  size = v.getreal("sm::size");
  speed = v.getreal("sm::speed");
  height = v.getreal("sm::height");
  strcpy(actions,v.getstring("sm::actions"));
}

void Track::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setreal("sm::speed",speed);
  v.setreal("sm::height",height);
  v.setstring("sm::actions",actions);
}

void Track::load(Group *group)
{EntityList list,pathlist,empty;
 Entity *e;
  sscanf(group->getinstance(),"SM::Track:%lf %lf %lf\377",&size,&speed,&height);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,actions);

  list = group->getlist().copy();
  for (list.start() ; (e = list.next()) != 0 ; )
    {  if (e->isa(point_entity) && ! pathlist.empty())
         {  pathlist.reverse();
            paths.add(new SimulationPath(pathlist,0.0,1.0,height,height));
            pathlist = empty;
         }
       pathlist.add(e);
    }
  if (! pathlist.empty())
    {  pathlist.reverse();
       paths.add(new SimulationPath(pathlist,0.0,1.0,height,height));
    }
}

void Track::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Track:%.4lf %.4lf %.4lf\377%s\377%s",
             size,speed,height,name,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

}

int Track::setup(int change)
{Dialog dialog("SimulationTrack_dialog");
 DialogControl *dc;
 int status;
  if (change)
    dialog.title("Change exisiting track");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&speed,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&height));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new StringDialogControl(104,actions,MAX_ACTIONLENGTH));
  dialog.add(new ButtonDialogControl(110));
  dialog.add(new ButtonDialogControl(111));
  status = dialog.process();
  if (status == TRUE)
    return 1;
  else if (status == FALSE)
    return 0;
  else
    return status;
}

int Track::isatrack(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Track:",10) == 0;
}

void Track::update(double)
{SimulationEvent *e;
 RCCHAR destination[300],decisionpoint[300];
  for (simulationdata->events.start() ; (e = (SimulationEvent *) simulationdata->events.next()) != 0 ; )
    {  GetItem(e->destination,0,destination,'~');
       GetItem(e->destination,1,decisionpoint,'~');
       if (strcmp(name,destination) == 0 && e->realvehicle != 0)
         {  e->realvehicle->moveonto(this,decisionpoint);
            simulationdata->events.del(e);
            simulationdata->events.start();
         }
    }
}

SimulationPath *Track::getpath(int j)
{int i;
  paths.start();
  for (i = 0 ;  i < j ; i++)
    paths.next();
  return (SimulationPath *) paths.next();
}

void Track::move(RealVehicle *rv,double dt,double *t)
{int i,j1,j2;
 SimulationPath *path;
 double tlength,mintlength;
 Point p,destinationp;
 int defined,pos;
 GeneralList *l1,*l2,pathlist,goodpathlist;
 DecisionPoint *dp;
 Conveyor *conveyor;
 Machine *machine;
 Buffer *buffer;
 RealProduct *realproduct;
 RCCHAR command[300],parameters[300];

  if (strlen(rv->getdestination()) > 0)
    {  j1 = (int)*t;
       path = getpath(j1);
       if (path != 0)
         *t += dt * speed / path->getlength();
       j2 = (int)*t;
       if (j1 != j2 || path == 0 && paths.length() > 0)
         {  *t = j2;

            if (path == 0)
              j1 = paths.length() - 1;
            p = getpath(j1)->getposition(1.0);

            defined = 0;
            for (simulationdata->decisionpoints.start() ; (dp = (DecisionPoint *) simulationdata->decisionpoints.next()) != 0 ; )
              if (dp->match(rv->getdestination()))
                {  destinationp = dp->getorigin()+Point(0.0,0.0,height);
                   defined = 1;
                   break;
                }

            if (defined && (p-destinationp).length() < db.getptoler())
              {  rv->newtransform(getpath(j1)->getposition(1.0),getpath(j1)->getdirection(1.0),Point(0.0,0.0,1.0).cross(getpath(j1)->getdirection(1.0)),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0));
                 rv->setdestination(_RCT(""));
                 *t = j1 + 0.9999999;    // Next time move onto new segment
                 pos = 0;
                 while (nextaction(_RCT("AtEnd"),dp->getactions(),command,parameters,&pos))
                   if (strcmp(command,"goto") == 0)
                     rv->setdestination(parameters);
                   else if (strcmp(command,"loadfrom") == 0)
                     {  for (simulationdata->conveyors.start() ; (conveyor = (Conveyor *) simulationdata->conveyors.next()) != 0 ; )
                          if (conveyor->match(parameters))
                            {  for (simulationdata->realproducts.start() ; (realproduct = (RealProduct *) simulationdata->realproducts.next()) != 0 ; )
                                 if (realproduct->match(conveyor) && realproduct->gett() >= 1.0)
                                   {  realproduct->moveonto(rv);
                                      break;
                                   }
                            }
                        for (simulationdata->machines.start() ; (machine = (Machine *) simulationdata->machines.next()) != 0 ; )
                          if (machine->match(parameters))
                            {  for (simulationdata->realproducts.start() ; (realproduct = (RealProduct *) simulationdata->realproducts.next()) != 0 ; )
                                 if (realproduct->match(machine))
                                   {  realproduct->moveonto(rv);
                                      break;
                                   }
                            }
                        for (simulationdata->buffers.start() ; (buffer = (Buffer *) simulationdata->buffers.next()) != 0 ; )
                          if (buffer->match(parameters))
                            {  for (simulationdata->realproducts.start() ; (realproduct = (RealProduct *) simulationdata->realproducts.next()) != 0 ; )
                                 if (realproduct->match(buffer))
                                   {  realproduct->moveonto(rv);
                                      break;
                                   }
                            }
                     }
                   else if (strcmp(command,"unloadto") == 0)
                     {  for (simulationdata->realproducts.start() ; (realproduct = (RealProduct *) simulationdata->realproducts.next()) != 0 ; )
                          if (realproduct->match(rv))
                             {  simulationdata->events.add(new SimulationEvent(MoveTo,realproduct,parameters));
                                break;
                             }
                     }
                 return;
              }
            else if (defined)
              {  l1 = new GeneralList;
                 l1->add((void *)j1);
                 pathlist.add(l1);

                 for (; ! pathlist.empty() ;)
                   {  pathlist.start();
                      l1 = (GeneralList *) pathlist.next();
                      pathlist.del(l1);
                      l1->start();
                      p = getpath((LONG)l1->next())->getposition(1.0);

                      if ((p-destinationp).length() < db.getptoler())
                        goodpathlist.add(l1);
                      else
                        {  for (i = 0 ; i < paths.length() ; i++)
                             if (! l1->inlist((void *)i) && (getpath(i)->getposition(0.0)-p).length() < db.getptoler())
                               {  l2 = new GeneralList;
                                  *l2 = l1->copy();
                                  l2->add((void *)i);
                                  pathlist.add(l2);
                               }
                           l1->destroy();
                           delete l1;
                        }
                   }
                 mintlength = -1.0;
                 for (goodpathlist.start() ; (l1 = (GeneralList *) goodpathlist.next()) != 0 ; )
                   {  tlength = 0.0;
                      for (l1->start() ; ! l1->atend() ; )
                        tlength += getpath((LONG)l1->next())->getlength();
                      if (mintlength < 0.0 || tlength < mintlength)
                        {  mintlength = tlength;
                           l1->reverse();
                           l1->start();
                           l1->del(l1->next());
                           l1->start();
                           if (! l1->atend())
                             {  j2 = (LONG) l1->next();
                                *t = j2;
                             }
                        }
                      l1->destroy();
                      delete l1;
                   }
              }
         }
       if (*t >= paths.length())
         {  *t = paths.length();
            rv->setdestination(_RCT(""));
         }
       path = getpath(j2);
       if (path != 0)
         rv->newtransform(path->getposition(*t-j2),path->getdirection(*t-j2),Point(0.0,0.0,1.0).cross(path->getdirection(*t-j2)),Point(1.0,1.0,1.0),Point(0.0,0.0,0.0));
    }
}

double Track::gett(RCCHAR *decisionpoint)
{DecisionPoint *dp;
 double t;
 int i;
 SimulationPath *path;
 Point p1,p2;

  t = 0.0;
  for (simulationdata->decisionpoints.start() ; (dp = (DecisionPoint *) simulationdata->decisionpoints.next()) != 0 ; )
    if (dp->match(decisionpoint))
      {  for (i = 0,paths.start() ; i < paths.length() ; i++)
           {  path = (SimulationPath *) paths.next();
              if ((path->getposition(0.0) - (dp->getorigin() + Point(0.0,0.0,height))).length() < db.getptoler())
                t = i;
           }
         break;
      }
  return t;
}

void DecisionPoint::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void DecisionPoint::getdefaults(void)
{ size = v.getreal("sm::size");
  strcpy(name,v.getstring("sm::name"));
  strcpy(trackname,v.getstring("sm::trackname"));
  strcpy(actions,v.getstring("sm::actions"));
}

void DecisionPoint::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setstring("sm::trackname",trackname);
  v.setstring("sm::actions",actions);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void DecisionPoint::load(Group *group)
{Entity *e;
 EntityList list;
  sscanf(group->getinstance(),"SM::DecisionPoint:%lf\377",&size);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,trackname);
  GetItem(group->getinstance(),3,actions);

  list = group->getlist();

  list.start();

  e = list.next();
  if (e->isa(point_entity))
    origin = ((PointE *)e)->getp();
  e = list.next();
  if (e->isa(line_entity))
    xaxis = ((Line *)e)->direction(0.0);
  e = list.next();
  if (e->isa(line_entity))
    yaxis = ((Line *)e)->direction(0.0);

}

void DecisionPoint::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::DecisionPoint:%.4lf\377%s\377%s\377%s",
             size,name,trackname,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  newlist = graphics();
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int DecisionPoint::setup(int change)
{Dialog dialog("SimulationDecisionPoint_dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting decision point");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(new StringDialogControl(102,trackname,300));
  dialog.add(new StringDialogControl(103,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int DecisionPoint::isadecisionpoint(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::DecisionPoint:",18) == 0;
}

EntityList DecisionPoint::graphics(void)
{EntityList list;
 BitMask options(32);

  list.add(new PointE(origin));
  list.add(new Line(origin-xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size-yaxis/2.0*size));

  list.add(new Circle(origin,xaxis,yaxis,size/4.0,0.0,2.0*M_PI));

  return list;
}

void Machine::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void Machine::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  size = v.getreal("sm::size");
  ninputsrequired = v.getreal("sm::ninputsrequired");
  height = v.getreal("sm::height");
  strcpy(actions,v.getstring("sm::actions"));
}

void Machine::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::size",size);
  v.setreal("sm::ninputsrequired",ninputsrequired);
  v.setreal("sm::height",height);
  v.setstring("sm::actions",actions);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void Machine::load(Group *g)
{Entity *e;
 EntityList list;
  group = g;
  sscanf(group->getinstance(),"SM::Machine:%lf %lf %lf\377",&size,&ninputsrequired,&height);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,actions);

  list = group->getlist();

  list.start();

  e = list.next();
  if (e->isa(point_entity))
    origin = ((PointE *)e)->getp();
  e = list.next();
  if (e->isa(line_entity))
    xaxis = ((Line *)e)->direction(0.0);
  e = list.next();
  if (e->isa(line_entity))
    yaxis = ((Line *)e)->direction(0.0);

}

void Machine::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Machine:%.4lf %.4lf %.4lf\377%s\377%s",
             size,ninputsrequired,height,name,actions);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  newlist = graphics();
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int Machine::setup(int change)
{Dialog dialog("SimulationMachine_Dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting machine");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&size,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&ninputsrequired,0.1,0.0,100000.0));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&height,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(new StringDialogControl(104,actions,MAX_ACTIONLENGTH));
  return dialog.process() == TRUE;
}

int Machine::isamachine(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Machine:",12) == 0;
}

EntityList Machine::graphics(void)
{EntityList list;
 BitMask options(32);

  list.add(new PointE(origin));
  list.add(new Line(origin-xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size-yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size-yaxis/2.0*size,origin+xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size+yaxis/2.0*size));
  list.add(new Line(origin-xaxis/2.0*size+yaxis/2.0*size,origin-xaxis/2.0*size-yaxis/2.0*size));

  list.add(new Line(origin-xaxis/4.0*size-yaxis/4.0*size,origin+xaxis/4.0*size-yaxis/4.0*size));
  list.add(new Line(origin+xaxis/4.0*size-yaxis/4.0*size,origin+xaxis/4.0*size+yaxis/4.0*size));
  list.add(new Line(origin+xaxis/4.0*size+yaxis/4.0*size,origin-xaxis/4.0*size+yaxis/4.0*size));
  list.add(new Line(origin-xaxis/4.0*size+yaxis/4.0*size,origin-xaxis/4.0*size-yaxis/4.0*size));

  list.add(new Line(origin-xaxis/2.0*size,origin+xaxis/2.0*size));
  list.add(new Line(origin+xaxis/2.0*size,origin+xaxis/4.0*size+yaxis/4.0*size));
  list.add(new Line(origin+xaxis/2.0*size,origin+xaxis/4.0*size-yaxis/4.0*size));

  return list;
}

void Machine::update(double)
{Product *p;
 RealProduct *rp;
 SimulationEvent *e;
 int pos;
 RCCHAR command[300],parameters[300],product[300],destination[300];
  for (simulationdata->events.start() ; ! full && (e = (SimulationEvent *) simulationdata->events.next()) != 0 ; )
    if (strcmp(name,e->destination) == 0)
      {  e->realproduct->moveonto(this);
         e->realproduct->newtransform(origin+Point(0.0,0.0,currentheight+height),xaxis,yaxis,Point(1.0,1.0,1.0),Point(0.0,0.0,0.0));
         currentheight += e->realproduct->getheight();
         simulationdata->events.del(e);
         simulationdata->events.start();
         ninputs++;
         realproducts.add(e->realproduct);
      }
  if (ninputs >= ninputsrequired)
    {  full = 1;
       ninputs = 0;
       currentheight = 0.0;
       for (realproducts.start() ; (rp = (RealProduct *) realproducts.next()) != 0 ; )
         {  simulationdata->realproducts.del(rp);
            delete rp;
         }
       realproducts.destroy();

       pos = 0;
       rp = 0;
       while (nextaction(_RCT("AtCreate"),actions,command,parameters,&pos))
         if (strcmp(command,"produce") == 0)
           {  GetItem(parameters,0,product,',');
              GetItem(parameters,1,destination,',');
              for (simulationdata->products.start() ; (p = (Product *) simulationdata->products.next()) != 0 ; )
                if (p->match(product))
                  {  simulationdata->realproducts.add(rp = new RealProduct(this,p,p->create(group,origin+Point(0.0,0.0,height),xaxis,yaxis)));
                     simulationdata->events.add(new SimulationEvent(MoveTo,rp,destination));
                     break;
                  }
           }
         else if (strcmp(command,"call") == 0 && rp != 0)
           simulationdata->events.add(new SimulationEvent(Call,rp,parameters));
    }
}

void Chart::setcoordinatesystem(Point o,Point xa,Point ya)
{ origin = o;
  xaxis = xa;
  yaxis = ya;
}

void Chart::getdefaults(void)
{ strcpy(name,v.getstring("sm::name"));
  width = v.getreal("sm::chartwidth");
  height = v.getreal("sm::chartheight");
  xminimum = v.getreal("sm::xminimum");
  yminimum = v.getreal("sm::yminimum");
  xmaximum = v.getreal("sm::xmaximum");
  ymaximum = v.getreal("sm::ymaximum");
  xstep = v.getreal("sm::xstep");
  ystep = v.getreal("sm::ystep");
  xstepsize = v.getreal("sm::xstepsize");
  ystepsize = v.getreal("sm::ystepsize");
  textheight = v.getreal("sm::textheight");
  strcpy(label,v.getstring("sm::label"));
  strcpy(xlabel,v.getstring("sm::xlabel"));
  strcpy(ylabel,v.getstring("sm::ylabel"));
}

void Chart::setdefaults(void)
{ v.setstring("sm::name",name);
  v.setreal("sm::chartwidth",width);
  v.setreal("sm::chartheight",height);
  v.setreal("sm::xminimum",xminimum);
  v.setreal("sm::yminimum",yminimum);
  v.setreal("sm::xmaximum",xmaximum);
  v.setreal("sm::ymaximum",ymaximum);
  v.setreal("sm::xstep",xstep);
  v.setreal("sm::ystep",ystep);
  v.setreal("sm::xstepsize",xstepsize);
  v.setreal("sm::ystepsize",ystepsize);
  v.setreal("sm::textheight",textheight);
  v.setstring("sm::label",label);
  v.setstring("sm::xlabel",xlabel);
  v.setstring("sm::ylabel",ylabel);

  origin.setxyz(0.0,0.0,0.0);
  xaxis.setxyz(1.0,0.0,0.0);
  yaxis.setxyz(0.0,1.0,0.0);

}

void Chart::load(Group *group)
{Entity *e;
 EntityList list;
  g = group;
  sscanf(group->getinstance(),"SM::Chart:%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\377",
            &width,&height,&xminimum,&yminimum,&xmaximum,&ymaximum,
            &xstep,&ystep,&xstepsize,&ystepsize,&textheight);
  GetItem(group->getinstance(),1,name);
  GetItem(group->getinstance(),2,label);
  GetItem(group->getinstance(),3,xlabel);
  GetItem(group->getinstance(),4,ylabel);

  list = group->getlist();

  list.start();

  e = list.next();
  while (e != 0 && e->isa(point_entity))
    e = list.next();
  if (e != 0 && e->isa(line_entity))
    {  origin = ((Line *)e)->getp1();
       xaxis = ((Line *)e)->direction(0.0);
    }
  e = list.next();
  if (e != 0 && e->isa(line_entity))
    yaxis = ((Line *)e)->direction(0.0);

}

void Chart::save(Group *group)
{BitMask change(32),options(32);
 RCCHAR info[300];
 Entity *e;
 EntityList list,newlist;

  if (group == 0)
    {  options.set(0);
       group = new Group(newlist,_RCT(""),_RCT(""),options);
       db.geometry.add(group);
    }

  //  Update the Non-Graphic information
  sprintf(info,"SM::Chart:%.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf %.4lf\377%s\377%s\377%s\377%s",
            width,height,xminimum,yminimum,xmaximum,ymaximum,
            xstep,ystep,xstepsize,ystepsize,textheight,name,label,xlabel,ylabel);
  change.set(4);  change.set(5);
  group->change(change,*group,_RCT(""),info,options);

  list = group->getlist();

  for (list.start() ; (e = list.next()) != 0 ; )
    {  group->del(e);
       if (group->getposition() != 0)
         db.geometry.del(e);
    }

  //  Update the Graphic information
  EntityHeader header;  //  Get current defaults
  db.header.setvisible(*group->getvisible());  //  Set to group defaults
  newlist = graphics();
  db.header.setvisible(*header.getvisible());  //  Set back to current defaults
  for (newlist.start() ; (e = newlist.next()) != 0 ; )
    {  if (group->getposition() != 0)
         db.geometry.add(e);
       group->add(e);
    }
  newlist.destroy();

}

int Chart::setup(int change)
{Dialog dialog("SimulationChart_Dialog");
 DialogControl *dc;
  if (change)
    dialog.title("Change exisiting chart");
  dialog.add(new StringDialogControl(100,name,300));
  dialog.add(dc = new RealDialogControl(101,&width,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1101,dc));
  dialog.add(dc = new RealDialogControl(102,&height,0.1,0.0,100000.0,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1102,dc));
  dialog.add(dc = new RealDialogControl(103,&xminimum,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1103,dc));
  dialog.add(dc = new RealDialogControl(104,&xmaximum,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1104,dc));
  dialog.add(dc = new RealDialogControl(105,&yminimum,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1105,dc));
  dialog.add(dc = new RealDialogControl(106,&ymaximum,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1106,dc));
  dialog.add(dc = new RealDialogControl(107,&xstep,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1107,dc));
  dialog.add(dc = new RealDialogControl(108,&ystep,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1108,dc));
  dialog.add(dc = new RealDialogControl(109,&xstepsize,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1109,dc));
  dialog.add(dc = new RealDialogControl(110,&ystepsize,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1110,dc));
  dialog.add(dc = new RealDialogControl(111,&textheight,ScaleByLength));
  dialog.add(new ScrollBarDialogControl(1111,dc));
  dialog.add(new StringDialogControl(112,label,300));
  dialog.add(new StringDialogControl(113,xlabel,300));
  dialog.add(new StringDialogControl(114,ylabel,300));
  return dialog.process() == TRUE;
}

int Chart::isachart(Entity *e)
{ return e->isa(group_entity) && strncmp(((Group *)e)->getinstance(),"SM::Chart:",10) == 0;
}

void Chart::plotcount(int colour,int layer,double period,double value)
{double xs,ys;
 EntityHeader header;
 PointE *p;
 RCCHAR *at;

  if (xmaximum - xminimum > 1.0E-10)
    xs = 1.0 / (xmaximum - xminimum) * width;
  else
    xs = 0.0;
  if (ymaximum - yminimum > 1.0E-10)
    ys = 1.0 / (ymaximum - yminimum) * height;
  else
    ys = 0.0;

  if (simulationdata->t - time > period)
    {  time += period;
       header.change(colour,layer,header.getstyle(),header.getweight());
       if (g != 0)
         header.setvisible(*g->getvisible());  //  Set to group defaults
       p = new PointE(header,Point(origin+xaxis*time*xs+yaxis*count*ys));
       db.geometry.add(p);
       if (g != 0)
         {  g->add(p);
            if ((at = new RCCHAR[strlen("Temp") + 1]) != NULL)
              {  strcpy(at,"Temp");
                 p->addat(at);
              }
         }
       count = 0.0;
    }
  count += value;
}

void Chart::clear(void)
{EntityList list;
 Entity *e;
  if (g != 0)
    {  list = g->getlist().copy();
       for (list.start() ; (e = list.next()) != 0 ; )
         if (e->getatlist().length() > 0)
           {  g->del(e);
              db.geometry.del(e);
           }
       list.destroy();
    }
}

EntityList Chart::graphics(void)
{EntityList list;
 BitMask options(32);
 double x,y,xmin,xmax,ymin,ymax,xs,ys;
 RCCHAR text[300];
 EntityHeader header;

  if (xmaximum - xminimum > 1.0E-10)
    xs = 1.0 / (xmaximum - xminimum) * width;
  else
    xs = 0.0;
  if (ymaximum - yminimum > 1.0E-10)
    ys = 1.0 / (ymaximum - yminimum) * height;
  else
    ys = 0.0;
  xmax = xmaximum * xs;
  xmin = xminimum * xs;
  ymax = ymaximum * ys;
  ymin = yminimum * ys;

  header.change(header.getcolour(),header.getlayer(),header.getstyle(),header.getweight()+4);

  list.add(new Line(header,origin+xaxis*xmin-yaxis*ymin,origin+xaxis*xmax-yaxis*ymin));
  list.add(new Line(header,origin-xaxis*xmin+yaxis*ymin,origin-xaxis*xmin+yaxis*ymax));

  list.add(new Line(header,origin-xaxis*(xmin-xstepsize/4.0)+yaxis*(ymax-xstepsize/2.0),origin-xaxis*xmin+yaxis*ymax));
  list.add(new Line(header,origin-xaxis*(xmin+xstepsize/4.0)+yaxis*(ymax-xstepsize/2.0),origin-xaxis*xmin+yaxis*ymax));
  list.add(new Line(header,origin+xaxis*(xmax-xstepsize/2.0)-yaxis*(ymin-ystepsize/4.0),origin+xaxis*xmax-yaxis*ymin));
  list.add(new Line(header,origin+xaxis*(xmax-xstepsize/2.0)-yaxis*(ymin+ystepsize/4.0),origin+xaxis*xmax-yaxis*ymin));

  for (x = xstep ; x < xmaximum ; x += xstep)
    {  list.add(new Line(origin+xaxis*x*xs-yaxis*(ymin-ystepsize/4.0),origin+xaxis*x*xs-yaxis*(ymin+ystepsize)));
       sprintf(text,"%.2lf",x);
       options.set(2);
       list.add(new Text(CHAR2RCCHR("Arial"),origin+xaxis*x*xs-yaxis*(ymin+ystepsize*1.1),xaxis,yaxis,0.0,textheight,0.0,90.0,0,CHAR2RCCHR(text),0,options));
    }
  options.clear(2);
  options.set(4);
  list.add(new Text(CHAR2RCCHR("Arial"),origin+xaxis*x*xs-yaxis*(ymin+xstepsize*1.1),xaxis,yaxis,0.0,textheight,0.0,0.0,0,CHAR2RCCHR(xlabel),0,options));
  options.clear(4);

  for (x = -xstep ; x > xminimum ; x -= xstep)
    {  list.add(new Line(origin+xaxis*x*xs-yaxis*(ymin-ystepsize/4.0),origin+xaxis*x*xs-yaxis*(ymin+ystepsize)));
       sprintf(text,"%.2lf",x);
       options.set(2);
       list.add(new Text(CHAR2RCCHR("Arial"),origin+xaxis*x*xs-yaxis*(ymin+ystepsize*1.1),xaxis,yaxis,0.0,textheight,0.0,90.0,0,CHAR2RCCHR(text),0,options));
    }

  for (y = ystep ; y < ymaximum ; y += ystep)
    {  list.add(new Line(origin-xaxis*(xmin-xstepsize/4.0)+yaxis*y*ys,origin-xaxis*(xmin+xstepsize)+yaxis*y*ys));
       sprintf(text,"%.2lf",y);
       options.set(2);
       list.add(new Text(CHAR2RCCHR("Arial"),origin-xaxis*(xmin+xstepsize*1.1)+yaxis*y*ys,xaxis,yaxis,0.0,textheight,0.0,0.0,0,CHAR2RCCHR(text),0,options));
    }
  list.add(new Text(CHAR2RCCHR("Arial"),origin-xaxis*(xmin+xstepsize*1.1)+yaxis*y*ys,xaxis,yaxis,0.0,textheight,0.0,0.0,0,CHAR2RCCHR(ylabel),0,options));

  for (y = -ystep ; y > yminimum ; y -= ystep)
    {  list.add(new Line(origin-xaxis*(xmin-xstepsize/4.0)+yaxis*y*ys,origin-xaxis*(xmin+xstepsize)+yaxis*y*ys));
       sprintf(text,"%.2lf",y);
       options.set(2);
       list.add(new Text(CHAR2RCCHR("Arial"),origin-xaxis*(xmin+xstepsize*1.1)+yaxis*y*ys,xaxis,yaxis,0.0,textheight,0.0,0.0,0,CHAR2RCCHR(text),0,options));
    }

  options.set(1);
  list.add(new Text(CHAR2RCCHR("Arial"),origin+xaxis*width/2.0+yaxis*height,xaxis,yaxis,0.0,textheight,0.0,0.0,0,CHAR2RCCHR(label),0,options));

  return list;
}

struct RBSimulation1_data
  {public:
     int state;
     Point p1;
     Producer producer;
  };

void RBSimulation1(int,void *data,Point *p2,View3dSurface *)
{RBSimulation1_data *rbdata = (RBSimulation1_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  if (rbdata->state == 1)
    rbdata->producer.setcoordinatesystem(*p2,xaxis,yaxis);
  else
    rbdata->producer.setcoordinatesystem(rbdata->p1,(*p2 - rbdata->p1).normalize(),zaxis.cross(*p2 - rbdata->p1).normalize());
  group = new Group(rbdata->producer.graphics(),_RCT(""),_RCT(""),options);
  group->draw(DM_INVERT);
  delete group;
}

//
//  Insert/Change Simulation Producer
//

void simulation_command1(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation1_data *rbdata = (RBSimulation1_data *) *data;
 EntityList list;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 Entity *e;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation1_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation producer command");
            *cstate = ExitState;
         }
       else
         {  list = state.getselection().copy();
            state.destroyselection(1);
            if (list.length() > 0)
              {  list.start();  e = list.next();
              }
            if (list.length() == 1 && Producer::isaproducer(e))
              {  rbdata->producer.load((Group *)e);
                 if (rbdata->producer.setup(1))
                   rbdata->producer.save((Group *)e);
                 *cstate = ExitState;
              }
            else if (list.length() != 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a producer only","Simulation producer ",MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else
              {  rbdata->producer.getdefaults();
                 if (rbdata->producer.setup(0))
                   {  rbdata->producer.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->producer.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),xaxis,yaxis);
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  xaxis = (((Coordinate3dEvent *) event)->getp() - rbdata->p1).normalize();
            yaxis = zaxis.cross(xaxis).normalize();
            rbdata->producer.setcoordinatesystem(rbdata->p1,xaxis,yaxis);
            rbdata->producer.save(0);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1 || *cstate == 2)
    {  rbdata->state = *cstate;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 1)
         cadwindow->prompt->normalprompt("Locate the origin of the producer");
       else
         cadwindow->prompt->normalprompt("Locate the direction of the producer");
       rubberband.begin(0);
       rubberband.add(RBSimulation1,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation2_data
  {public:
     int state;
     Point p1;
     Consumer consumer;
  };

void RBSimulation2(int,void *data,Point *p2,View3dSurface *)
{RBSimulation2_data *rbdata = (RBSimulation2_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  if (rbdata->state == 1)
    rbdata->consumer.setcoordinatesystem(*p2,xaxis,yaxis);
  else
    rbdata->consumer.setcoordinatesystem(rbdata->p1,(*p2 - rbdata->p1).normalize(),zaxis.cross(*p2 - rbdata->p1).normalize());
  group = new Group(rbdata->consumer.graphics(),_RCT(""),_RCT(""),options);
  group->draw(DM_INVERT);
  delete group;
}

//
//  Insert/Change Simulation Consumer
//

void simulation_command2(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation2_data *rbdata = (RBSimulation2_data *) *data;
 EntityList list;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 Entity *e;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation2_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation consumer command");
            *cstate = ExitState;
         }
       else
         {  list = state.getselection().copy();
            state.destroyselection(1);
            if (list.length() > 0)
              {  list.start();  e = list.next();
              }
            if (list.length() == 1 && Consumer::isaconsumer(e))
              {  rbdata->consumer.load((Group *)e);
                 if (rbdata->consumer.setup(1))
                   rbdata->consumer.save((Group *)e);
                 *cstate = ExitState;
              }
            else if (list.length() != 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a consumer only","Simulation consumer",MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else
              {  rbdata->consumer.getdefaults();
                 if (rbdata->consumer.setup(0))
                   {  rbdata->consumer.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->consumer.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),xaxis,yaxis);
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  xaxis = (((Coordinate3dEvent *) event)->getp() - rbdata->p1).normalize();
            yaxis = zaxis.cross(xaxis).normalize();
            rbdata->consumer.setcoordinatesystem(rbdata->p1,xaxis,yaxis);
            rbdata->consumer.save(0);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1 || *cstate == 2)
    {  rbdata->state = *cstate;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 1)
         cadwindow->prompt->normalprompt("Locate the origin of the consumer");
       else
         cadwindow->prompt->normalprompt("Locate the direction of the consumer");
       rubberband.begin(0);
       rubberband.add(RBSimulation2,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation3_data
  {public:
     int state;
     Point p1;
     Machine machine;
  };

void RBSimulation3(int,void *data,Point *p2,View3dSurface *)
{RBSimulation3_data *rbdata = (RBSimulation3_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  if (rbdata->state == 1)
    rbdata->machine.setcoordinatesystem(*p2,xaxis,yaxis);
  else
    rbdata->machine.setcoordinatesystem(rbdata->p1,(*p2 - rbdata->p1).normalize(),zaxis.cross(*p2 - rbdata->p1).normalize());
  group = new Group(rbdata->machine.graphics(),_RCT(""),_RCT(""),options);
  group->draw(DM_INVERT);
  delete group;
}

//
//  Insert/Change Simulation Machine
//

void simulation_command3(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation3_data *rbdata = (RBSimulation3_data *) *data;
 EntityList list;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 Entity *e;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation3_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation machine command");
            *cstate = ExitState;
         }
       else
         {  list = state.getselection().copy();
            state.destroyselection(1);
            if (list.length() > 0)
              {  list.start();  e = list.next();
              }
            if (list.length() == 1 && Machine::isamachine(e))
              {  rbdata->machine.load((Group *)e);
                 if (rbdata->machine.setup(1))
                   rbdata->machine.save((Group *)e);
                 *cstate = ExitState;
              }
            else if (list.length() != 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a machine only","Simulation producer ",MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else
              {  rbdata->machine.getdefaults();
                 if (rbdata->machine.setup(0))
                   {  rbdata->machine.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->machine.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),xaxis,yaxis);
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  xaxis = (((Coordinate3dEvent *) event)->getp() - rbdata->p1).normalize();
            yaxis = zaxis.cross(xaxis).normalize();
            rbdata->machine.setcoordinatesystem(rbdata->p1,xaxis,yaxis);
            rbdata->machine.save(0);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1 || *cstate == 2)
    {  rbdata->state = *cstate;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 1)
         cadwindow->prompt->normalprompt("Locate the origin of the machine");
       else
         cadwindow->prompt->normalprompt("Locate the direction of the machine");
       rubberband.begin(0);
       rubberband.add(RBSimulation3,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation4_data
  {public:
     Point start,end;
     Conveyor conveyor;
     EntityList list;
  };

void RBSimulation4(int,void *data,Point *p2,View3dSurface *)
{RBSimulation4_data *rbdata = (RBSimulation4_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

//
//  Insert/Change Simulation Conveyor Path
//

void simulation_command4(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation4_data *rbdata = (RBSimulation4_data *) *data;
 EntityList pathlist;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1;
 int linear;
 Entity *e;
 RCCHAR info[256];
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation4_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation conveyor command");
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            if (pathlist.length() > 0)
              {  pathlist.start();  e = pathlist.next();
              }
            if (pathlist.length() == 1 && Conveyor::isaconveyor(e))
              {  rbdata->conveyor.setdefaults();
                 rbdata->conveyor.load((Group *)e);
                 if (rbdata->conveyor.setup(1))
                   rbdata->conveyor.save((Group *)e);
                 *cstate = ExitState;
                 return;
              }
            for (pathlist.start(),linear = 1 ; (e = pathlist.next()) != NULL ; )
              if (! e->islinear())
                {  linear = 0;  break;
                }
            if (! linear)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a conveyor path or linear entities only","Simulation conveyor command",MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a conveyor path or the entities that define the conveyor path first","Simulation conveyor command",MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  loops.create(&pathlist);
                 if (loops.nloops() > 1)
                   { RCMessageBox(cadwindow->gethwnd(),"Please select a single connected path to follow","Simulation conveyor command",MB_ICONINFORMATION);
                      loops.destroy();
                      *cstate = ExitState;
                      break;
                   }
                 else
                   {  rbdata->list = loops.getloops();
                      rbdata->start = loops.getstart();
                      rbdata->end = loops.getend();
                   }
                 rbdata->conveyor.getdefaults();
                 if (rbdata->conveyor.setup(0))
                   {  rbdata->conveyor.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            rbdata->list.add(new PointE(rbdata->start));
            Group *group = new Group(rbdata->list,_RCT(""),info,options);
            if (group != NULL && group->getdefined())
              db.geometry.add(group);
            else
              delete group;
            rbdata->conveyor.save(group);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate near the start of the conveyor path");
       rubberband.begin(0);
       rubberband.add(RBSimulation4,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation5_data
  {public:
     Product product;
  };

void RBSimulation5(int,void *data,Point *p2,View3dSurface *)
{RBSimulation5_data *rbdata = (RBSimulation5_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  rbdata->product.setcoordinatesystem(*p2,xaxis,yaxis);
  group = new Group(rbdata->product.graphics(),_RCT(""),_RCT(""),options);
  group->draw(DM_INVERT);
  delete group;
}

//
//  Insert/Change Simulation Product
//

void simulation_command5(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation5_data *rbdata = (RBSimulation5_data *) *data;
 EntityList list;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 Entity *e;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation5_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation product command");
            *cstate = ExitState;
         }
       else
         {  list = state.getselection().copy();
            state.destroyselection(1);
            if (list.length() > 0)
              {  list.start();  e = list.next();
              }
            if (list.length() == 1 && Product::isaproduct(e))
              {  rbdata->product.load((Group *)e);
                 if (rbdata->product.setup(1))
                   rbdata->product.save((Group *)e);
                 *cstate = ExitState;
              }
            else if (list.length() != 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a product only","Simulation producer ",MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else
              {  rbdata->product.getdefaults();
                 if (rbdata->product.setup(0))
                   {  rbdata->product.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->product.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),xaxis,yaxis);
            rbdata->product.save(0);
         }
       *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the product");
       rubberband.begin(0);
       rubberband.add(RBSimulation5,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation6_data
  {public:
     Vehicle vehicle;
  };

void RBSimulation6(int,void *data,Point *p2,View3dSurface *)
{RBSimulation6_data *rbdata = (RBSimulation6_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  rbdata->vehicle.setcoordinatesystem(*p2,xaxis,yaxis);
  group = new Group(rbdata->vehicle.graphics(),_RCT(""),_RCT(""),options);
  group->draw(DM_INVERT);
  delete group;
}

//
//  Insert/Change Simulation Vehicle
//

void simulation_command6(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation6_data *rbdata = (RBSimulation6_data *) *data;
 EntityList list;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 Entity *e;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation6_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation vehicle command");
            *cstate = ExitState;
         }
       else
         {  list = state.getselection().copy();
            state.destroyselection(1);
            if (list.length() > 0)
              {  list.start();  e = list.next();
              }
            if (list.length() == 1 && Vehicle::isavehicle(e))
              {  rbdata->vehicle.load((Group *)e);
                 if (rbdata->vehicle.setup(1))
                   rbdata->vehicle.save((Group *)e);
                 *cstate = ExitState;
              }
            else if (list.length() != 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a vehicle only","Simulation producer ",MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else
              {  rbdata->vehicle.getdefaults();
                 if (rbdata->vehicle.setup(0))
                   {  rbdata->vehicle.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->vehicle.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),xaxis,yaxis);
            rbdata->vehicle.save(0);
         }
       *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the vehicle");
       rubberband.begin(0);
       rubberband.add(RBSimulation6,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation7_data
  {public:
     int state;
     Point start,end;
     Group *tracke,*decisionpointe;
     Track track;
     DecisionPoint decisionpoint;
     EntityList list;
  };

void RBSimulation7(int,void *data,Point *p2,View3dSurface *)
{RBSimulation7_data *rbdata = (RBSimulation7_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  if (rbdata->state == 1)
    {  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
         {  PointE p(rbdata->end);  p.draw(DM_INVERT);
         }
       else
         {  PointE p(rbdata->start);  p.draw(DM_INVERT);
         }
    }
  else
   {  rbdata->decisionpoint.setcoordinatesystem(*p2,xaxis,yaxis);
      group = new Group(rbdata->decisionpoint.graphics(),_RCT(""),_RCT(""),options);
      group->draw(DM_INVERT);
      delete group;
   }
}

//
//  Insert/Change Simulation Track Path
//

void simulation_command7(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation7_data *rbdata = (RBSimulation7_data *) *data;
 EntityList pathlist;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1;
 int linear,status;
 Entity *e;
 RCCHAR info[256];
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation7_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation track command");
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            for (pathlist.start(),linear = 1,rbdata->tracke = 0,rbdata->decisionpointe = 0 ; (e = pathlist.next()) != NULL ; )
              if (Track::isatrack(e) && rbdata->tracke == 0)
                rbdata->tracke = (Group *) e;
              else if (DecisionPoint::isadecisionpoint(e) && rbdata->decisionpointe == 0)
                rbdata->decisionpointe = (Group *) e;
              else if (! e->islinear())
                {  linear = 0;  break;
                }
            pathlist.del(rbdata->tracke);
            pathlist.del(rbdata->decisionpointe);
            if (rbdata->decisionpointe != 0)
              {  rbdata->decisionpoint.setdefaults();
                 rbdata->decisionpoint.load(rbdata->decisionpointe);
                 if (rbdata->decisionpoint.setup(1))
                   rbdata->decisionpoint.save(rbdata->decisionpointe);
                 *cstate = ExitState;
              }
            else if (rbdata->tracke != 0 && linear)
              {  rbdata->track.setdefaults();
                 rbdata->track.load(rbdata->tracke);
                 if ((status = rbdata->track.setup(1)) == 1)
                   {  rbdata->track.save(rbdata->tracke);
                      *cstate = ExitState;
                   }
                 else if (status == 0)
                   *cstate = ExitState;
                 else if (status == 110)
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        { RCMessageBox(cadwindow->gethwnd(),"Please select a single connected path to follow","Simulation track command",MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  rbdata->list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                      *cstate = 1;
                   }
                 else if (status == 111)
                   {  rbdata->decisionpoint.getdefaults();
                      if (rbdata->decisionpoint.setup(0))
                        {  rbdata->decisionpoint.setdefaults();
                           *cstate = 2;
                        }
                      else
                        *cstate = ExitState;
                   }
                 break;
              }
            else if (! linear)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a track or linear entities only","Simulation conveyor command",MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a track or the entities that define the track first","Simulation track command",MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  loops.create(&pathlist);
                 if (loops.nloops() > 1)
                   { RCMessageBox(cadwindow->gethwnd(),"Please select a single connected path to follow","Simulation track command",MB_ICONINFORMATION);
                      loops.destroy();
                      *cstate = ExitState;
                      break;
                   }
                 else
                   {  rbdata->list = loops.getloops();
                      rbdata->start = loops.getstart();
                      rbdata->end = loops.getend();
                   }
                 rbdata->track.getdefaults();
                 if (rbdata->track.setup(0))
                   {  rbdata->track.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            rbdata->list.add(new PointE(rbdata->start));
            if (rbdata->tracke != 0)
              {  rbdata->list.reverse();
                 for (rbdata->list.start() ; (e = rbdata->list.next()) != 0 ; )
                   rbdata->tracke->add(e);
                 rbdata->list.destroy();
              }
            else
              {  Group *group = new Group(rbdata->list,_RCT(""),info,options);
                 if (group != NULL && group->getdefined())
                   db.geometry.add(group);
                 else
                   delete group;
                 rbdata->track.save(group);
              }
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->decisionpoint.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),Point(1.0,0.0,0.0),Point(0.0,1.0,0.0));
            rbdata->decisionpoint.save(0);
         }
       *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  rbdata->state = 1;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate near the start of the track");
       rubberband.begin(0);
       rubberband.add(RBSimulation7,rbdata);
    }
  else if (*cstate == 2)
    {  rbdata->state = 2;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the decision point");
       rubberband.begin(0);
       rubberband.add(RBSimulation7,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation8_data
  {public:
     int state;
     Point p1;
     Buffer buffer;
  };

void RBSimulation8(int,void *data,Point *p2,View3dSurface *)
{RBSimulation8_data *rbdata = (RBSimulation8_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  if (rbdata->state == 1)
    rbdata->buffer.setcoordinatesystem(*p2,xaxis,yaxis);
  else
    rbdata->buffer.setcoordinatesystem(rbdata->p1,(*p2 - rbdata->p1).normalize(),zaxis.cross(*p2 - rbdata->p1).normalize());
  group = new Group(rbdata->buffer.graphics(),_RCT(""),_RCT(""),options);
  group->draw(DM_INVERT);
  delete group;
}

//
//  Insert/Change Simulation Buffer
//

void simulation_command8(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation8_data *rbdata = (RBSimulation8_data *) *data;
 EntityList list;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 Entity *e;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation8_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation buffer command");
            *cstate = ExitState;
         }
       else
         {  list = state.getselection().copy();
            state.destroyselection(1);
            if (list.length() > 0)
              {  list.start();  e = list.next();
              }
            if (list.length() == 1 && Buffer::isabuffer(e))
              {  rbdata->buffer.load((Group *)e);
                 if (rbdata->buffer.setup(1))
                   rbdata->buffer.save((Group *)e);
                 *cstate = ExitState;
              }
            else if (list.length() != 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a producer only","Simulation producer ",MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else
              {  rbdata->buffer.getdefaults();
                 if (rbdata->buffer.setup(0))
                   {  rbdata->buffer.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
            rbdata->buffer.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),xaxis,yaxis);
            *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  xaxis = (((Coordinate3dEvent *) event)->getp() - rbdata->p1).normalize();
            yaxis = zaxis.cross(xaxis).normalize();
            rbdata->buffer.setcoordinatesystem(rbdata->p1,xaxis,yaxis);
            rbdata->buffer.save(0);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1 || *cstate == 2)
    {  rbdata->state = *cstate;
       state.selmask.entity.clearandset(xyz_mask,end_list);
       if (*cstate == 1)
         cadwindow->prompt->normalprompt("Locate the origin of the buffer");
       else
         cadwindow->prompt->normalprompt("Locate the direction of the buffer");
       rubberband.begin(0);
       rubberband.add(RBSimulation8,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation9_data
  {public:
     Chart chart;
  };

void RBSimulation9(int,void *data,Point *p2,View3dSurface *)
{RBSimulation9_data *rbdata = (RBSimulation9_data *) data;
 Group *group;
 Point xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 EntityList empty;
 BitMask options(32);
  rbdata->chart.setcoordinatesystem(*p2,xaxis,yaxis);
  group = new Group(rbdata->chart.graphics(),_RCT(""),_RCT(""),options);
  group->draw(DM_INVERT);
  delete group;
}

//
//  Insert/Change Simulation Chart
//

void simulation_command9(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation9_data *rbdata = (RBSimulation9_data *) *data;
 EntityList list;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1,xaxis(1.0,0.0,0.0),yaxis(0.0,1.0,0.0),zaxis(0.0,0.0,1.0);
 Entity *e;
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation9_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation product command");
            *cstate = ExitState;
         }
       else
         {  list = state.getselection().copy();
            state.destroyselection(1);
            if (list.length() > 0)
              {  list.start();  e = list.next();
              }
            if (list.length() == 1 && Chart::isachart(e))
              {  rbdata->chart.load((Group *)e);
                 if (rbdata->chart.setup(1))
                   rbdata->chart.save((Group *)e);
                 *cstate = ExitState;
              }
            else if (list.length() != 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a chart only","Simulation producer ",MB_ICONINFORMATION);
                 list.destroy();
                 *cstate = ExitState;
              }
            else
              {  rbdata->chart.getdefaults();
                 if (rbdata->chart.setup(0))
                   {  rbdata->chart.setdefaults();
                      *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  rbdata->chart.setcoordinatesystem(((Coordinate3dEvent *) event)->getp(),xaxis,yaxis);
            rbdata->chart.save(0);
         }
       *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the origin of the chart");
       rubberband.begin(0);
       rubberband.add(RBSimulation9,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation10_data
  {Camera camera;
   Point start,end;
   EntityList list;
  };

void RBSimulation10(int,void *data,Point *p2,View3dSurface *)
{RBSimulation10_data *rbdata = (RBSimulation10_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

//
//  Insert/Change Simulation Camera Path
//

void simulation_command10(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation10_data *rbdata = (RBSimulation10_data *) *data;
 BitMask options(32);
 Point p,ref,p1;
 EntityList pathlist;
 int linear,point;
 Entity *e;
 Loops loops(0);
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation10_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation camera path command");
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            if (pathlist.length() > 0)
              {  pathlist.start();  e = pathlist.next();
              }
            if (pathlist.length() == 1 && Camera::isacamera(e))
              {BitMask change(32),options(32);
                 rbdata->camera.setdefaults();
                 rbdata->camera.load((Group *)e);
                 if (rbdata->camera.setup(1))
                   rbdata->camera.save((Group *)e);
                 *cstate = ExitState;
                 return;
              }
            linear = point = 0;
            if (pathlist.length() == 1 && e->isa(point_entity))
              {  p = ((PointE *) e)->getp();
                 point = 1;
              }
            else
              {  for (pathlist.start(),linear = 1 ; (e = pathlist.next()) != NULL ; )
                   if (! e->islinear())
                     {  linear = 0;  break;
                     }
              }
            if (! linear && ! point)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a single camera position or linear entities only","Simulation camera command",MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a camera path or the entities that define the camera path first","Simulation camera command",MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  if (linear)
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        { RCMessageBox(cadwindow->gethwnd(),"Please select a single connected path to follow","Simulation camera command",MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  rbdata->list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                   }
                 else
                   {  rbdata->list.add(e);
                      rbdata->start = p;
                      rbdata->end = p;
                   }
                 rbdata->camera.getdefaults();
                 if (rbdata->camera.setup(0))
                   {  rbdata->camera.setdefaults();
                      if (point)
                        {  if (rbdata->camera.getfixreference())
                             *cstate = 2;
                           else
                             {  rbdata->list.add(new PointE(rbdata->start));
                                ref.setxyz(0.0,0.0,0.0);
                                rbdata->list.add(new PointE(ref));
                                Group *group = new Group(rbdata->list,_RCT(""),_RCT(""),options);
                                if (group != NULL && group->getdefined())
                                  db.geometry.add(group);
                                else
                                  delete group;
                                rbdata->camera.save(group);
                                *cstate = ExitState;
                             }
                        }
                      else
                        *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            if (! rbdata->camera.getfixreference())
              {  rbdata->list.add(new PointE(rbdata->start));
                 ref.setxyz(0.0,0.0,0.0);
                 rbdata->list.add(new PointE(ref));
                 Group *group = new Group(rbdata->list,_RCT(""),_RCT(""),options);
                 if (group != NULL && group->getdefined())
                   db.geometry.add(group);
                 else
                   delete group;
                 rbdata->camera.save(group);
                 *cstate = ExitState;
              }
            else
              *cstate = 2;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
     case 2:
       if (event->isa(Coordinate3d))
         {  ref = ((Coordinate3dEvent *) event)->getp();
            rbdata->list.add(new PointE(rbdata->start));
            rbdata->list.add(new PointE(ref));
            Group *group = new Group(rbdata->list,_RCT(""),_RCT(""),options);
            if (group != NULL && group->getdefined())
              db.geometry.add(group);
            else
              delete group;
            rbdata->camera.save(group);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate near the start of the camera path");
       rubberband.begin(0);
       rubberband.add(RBSimulation10,rbdata);
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the view reference point");
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

struct RBSimulation11_data
  {public:
     Point start,end;
     CameraReference camerareference;
     EntityList list;
  };

void RBSimulation11(int,void *data,Point *p2,View3dSurface *)
{RBSimulation11_data *rbdata = (RBSimulation11_data *) data;
  if ((rbdata->start - *p2).length() > (rbdata->end - *p2).length())
    {  PointE p(rbdata->end);  p.draw(DM_INVERT);
    }
  else
    {  PointE p(rbdata->start);  p.draw(DM_INVERT);
    }
}

//
//  Insert/Change Simulation Camera Reference Path
//

void simulation_command11(int *cstate,HighLevelEvent *event,void **data)
{RBSimulation11_data *rbdata = (RBSimulation11_data *) *data;
 EntityList pathlist;
 BitMask options(32);
 Loops loops(0);
 Point p,ref,p1;
 int point,linear;
 Entity *e;
 RCCHAR info[256];
  switch (*cstate)
    {case InitialState:
       rbdata = new RBSimulation11_data;  *data = rbdata;
       if (rbdata == NULL)
         {  state.fail(Memory,"Simulation camera reference path command");
            *cstate = ExitState;
         }
       else
         {  pathlist = state.getselection().copy();
            state.destroyselection(1);
            if (pathlist.length() > 0)
              {  pathlist.start();  e = pathlist.next();
              }
            if (pathlist.length() == 1 && CameraReference::isacamerareference(e))
              {  rbdata->camerareference.setdefaults();
                 rbdata->camerareference.load((Group *)e);
                 if (rbdata->camerareference.setup(1))
                   {  rbdata->camerareference.save((Group *)e);
                   }
                 *cstate = ExitState;
                 return;
              }
            linear = point = 0;
            if (pathlist.length() == 1 && e->isa(point_entity))
              {  p = ((PointE *) e)->getp();
                 point = 1;
              }
            else
              {  for (pathlist.start(),linear = 1 ; (e = pathlist.next()) != NULL ; )
                   if (! e->islinear())
                     {  linear = 0;  break;
                     }
              }
            if (! linear && ! point)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a camera reference path or linear entities only","Simulation camera reference command",MB_ICONINFORMATION);
                 pathlist.destroy();
                 *cstate = ExitState;
              }
            else if (pathlist.length() == 0)
              { RCMessageBox(cadwindow->gethwnd(),"Please select a camera reference path or the entities that define the camera path first","Simulation camera reference command",MB_ICONINFORMATION);
                 *cstate = ExitState;
              }
            else
              {  if (linear)
                   {  loops.create(&pathlist);
                      if (loops.nloops() > 1)
                        { RCMessageBox(cadwindow->gethwnd(),"Please select a single connected path to follow","Simulation camera reference command",MB_ICONINFORMATION);
                           loops.destroy();
                           *cstate = ExitState;
                           break;
                        }
                      else
                        {  rbdata->list = loops.getloops();
                           rbdata->start = loops.getstart();
                           rbdata->end = loops.getend();
                        }
                   }
                 else
                   {  rbdata->list.add(e);
                      rbdata->start = p;
                      rbdata->end = p;
                   }
                 rbdata->camerareference.getdefaults();
                 if (rbdata->camerareference.setup(0))
                   {  rbdata->camerareference.setdefaults();
                      if (point)
                        {  Group *group = new Group(rbdata->list,_RCT(""),_RCT(""),options);
                           if (group != NULL && group->getdefined())
                             db.geometry.add(group);
                           else
                             delete group;
                           rbdata->camerareference.save(group);
                           *cstate = ExitState;
                        }
                      else
                        *cstate = 1;
                   }
                 else
                   *cstate = ExitState;
              }
         }
       break;
     case 1:
       rubberband.end(0);
       if (event->isa(Coordinate3d))
         {  p = ((Coordinate3dEvent *) event)->getp();
            if ((rbdata->start - p).length() > (rbdata->end - p).length())
              {  p1 = rbdata->start;  rbdata->start = rbdata->end;  rbdata->end = p1;
                 rbdata->list.reverse();
              }
            rbdata->list.add(new PointE(rbdata->start));
            Group *group = new Group(rbdata->list,_RCT(""),info,options);
            if (group != NULL && group->getdefined())
              db.geometry.add(group);
            else
              delete group;
            rbdata->camerareference.save(group);
            *cstate = ExitState;
         }
       else if (event->isa(Abort) || event->isa(Exit))
         *cstate = ExitState;
       break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate near the start of the camera reference path");
       rubberband.begin(0);
       rubberband.add(RBSimulation11,rbdata);
    }
  else if (*cstate == ExitState)
    delete rbdata;
}

SimulationPath::SimulationPath(EntityList l,double stime,double ftime,double sheight,double fheight)
{Entity *e;
 Point p;
  starttime = stime;
  finishtime = ftime;
  startheight = sheight;
  finishheight = fheight;
  list = l;
  length = 0.0;
  list.start();
  if ((e = list.next()) == NULL) return;
  if (! e->isa(point_entity)) return;
  while ((e = list.next()) != NULL)
    if (e->islinear())
      length += ((LinearEntity *)e)->length();
}

Point SimulationPath::getdirection(double t)
{Entity *e;
 double l1,l2,t1,s;
 Point p,tmp(1.0,0.0,0.0);
  t = (t - starttime) / (finishtime - starttime);
  l1 = length * t;
  list.start();
  if ((e = list.next()) == NULL) return tmp;
  if (! e->isa(point_entity)) return tmp;
  p = ((PointE *)e)->getp();
  if (list.peek() == 0) return tmp;
  while ((e = list.next()) != 0)
    if (e->islinear())
      {  l2 = ((LinearEntity *)e)->length();
         if (l2 < db.getptoler()) continue;
         if (l2 < l1 - db.getptoler())
           l1 -= l2;
         else
           {  if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
                {  s = -1.0;  t1 = (l2 - l1) / l2;
                }
              else
                {  s = 1.0;   t1 = l1 / l2;
                }
              if (t1 < 0.0) t1 = 0.0; else if (t1 > 1.0) t1 = 1.0;
              return ((LinearEntity *)e)->direction(t1) * s;
           }
         if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
           p = ((LinearEntity *)e)->start();
         else
           p = ((LinearEntity *)e)->end();
      }
  return tmp;
}

Point SimulationPath::getposition(double t)
{Entity *e;
 double l1,l2,t1;
 Point p,tmp(0.0,0.0,0.0);
  t = (t - starttime) / (finishtime - starttime);
  l1 = length * t;
  list.start();
  if ((e = list.next()) == NULL) return tmp;
  if (! e->isa(point_entity)) return tmp;
  p = ((PointE *)e)->getp();
  if (list.peek() == NULL)
    return p;
  while ((e = list.next()) != NULL)
    if (e->islinear())
      {  l2 = ((LinearEntity *)e)->length();
         if (l2 < db.getptoler()) continue;
         if (l2 < l1 - db.getptoler())
           l1 -= l2;
         else
           {  if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
                t1 = (l2 - l1) / l2;
              else
                t1 = l1 / l2;
              if (t1 < 0.0) t1 = 0.0; else if (t1 > 1.0) t1 = 1.0;
              return ((LinearEntity *)e)->position(t1) + Point(0.0,0.0,startheight+(finishheight-startheight)*t1);
           }
         if ((((LinearEntity *)e)->start() - p).length() > (((LinearEntity *)e)->end() - p).length())
           p = ((LinearEntity *)e)->start() + Point(0.0,0.0,startheight);
         else
           p = ((LinearEntity *)e)->end() + Point(0.0,0.0,finishheight);
      }
  return tmp;
}

RBSimulation12_data::RBSimulation12_data()
{Camera *c;
 CameraReference *cr;
 Entity *e;
 Producer *p;
 Product *pr;
 Consumer *cn;
 Conveyor *con;
 Machine *m;
 Chart *chart;
 Track *track;
 Vehicle *vehicle;
 Buffer *buffer;
 DecisionPoint *decisionpoint;
  for (db.geometry.start() ; (e = db.geometry.next()) != NULL ; )
    if (Camera::isacamera(e))
      {  if ((c = new Camera) == NULL) return;
         c->load((Group *)e);
         cameras.add(c);
      }
    else if (CameraReference::isacamerareference(e))
      {  if ((cr = new CameraReference) == NULL) return;
         cr->load((Group *)e);
         camerareferences.add(cr);
      }
    else if (Producer::isaproducer(e))
      {  if ((p = new Producer) == NULL) return;
         p->load((Group *)e);
         producers.add(p);
      }
    else if (Consumer::isaconsumer(e))
      {  if ((cn = new Consumer) == NULL) return;
         cn->load((Group *)e);
         consumers.add(cn);
      }
    else if (Conveyor::isaconveyor(e))
      {  if ((con = new Conveyor) == NULL) return;
         con->load((Group *)e);
         conveyors.add(con);
      }
    else if (Product::isaproduct(e))
      {  if ((pr = new Product) == NULL) return;
         pr->load((Group *)e);
         products.add(pr);
      }
    else if (Machine::isamachine(e))
      {  if ((m = new Machine) == NULL) return;
         m->load((Group *)e);
         machines.add(m);
      }
    else if (Chart::isachart(e))
      {  if ((chart = new Chart) == NULL) return;
         chart->load((Group *)e);
         chart->clear();
         charts.add(chart);
      }
    else if (Buffer::isabuffer(e))
      {  if ((buffer = new Buffer) == NULL) return;
         buffer->load((Group *)e);
         buffers.add(buffer);
      }
    else if (Vehicle::isavehicle(e))
      {  if ((vehicle = new Vehicle) == NULL) return;
         vehicle->load((Group *)e);
         vehicles.add(vehicle);
      }
    else if (Track::isatrack(e))
      {  if ((track = new Track) == NULL) return;
         track->load((Group *)e);
         tracks.add(track);
      }
    else if (DecisionPoint::isadecisionpoint(e))
      {  if ((decisionpoint = new DecisionPoint) == NULL) return;
         decisionpoint->load((Group *)e);
         decisionpoints.add(decisionpoint);
      }
  avicompressedstream = 0;
  avistream = 0;
  avifile = 0;
}

RBSimulation12_data::~RBSimulation12_data()
{Camera *camera;
 CameraReference *camerareference;
 Producer *producer;
 Product *product;
 Consumer *consumer;
 Conveyor *conveyor;
 Machine *machine;
 Chart *chart;
 Track *track;
 Vehicle *vehicle;
 Buffer *buffer;
 RealProduct *realproduct;
 RealVehicle *realvehicle;

  for (cameras.start() ; (camera = (Camera *) cameras.next()) != NULL ; )
    delete camera;
  cameras.destroy();

  for (camerareferences.start() ; (camerareference = (CameraReference *) camerareferences.next()) != NULL ; )
    delete camerareference;
  camerareferences.destroy();

  for (producers.start() ; (producer = (Producer *) producers.next()) != NULL ; )
    delete producer;
  producers.destroy();

  for (products.start() ; (product = (Product *) products.next()) != NULL ; )
    delete product;
  products.destroy();

  for (consumers.start() ; (consumer = (Consumer *) consumers.next()) != NULL ; )
    delete consumer;
  consumers.destroy();

  for (conveyors.start() ; (conveyor = (Conveyor *) conveyors.next()) != NULL ; )
    delete conveyor;
  conveyors.destroy();

  for (machines.start() ; (machine = (Machine *) machines.next()) != NULL ; )
    delete machine;
  machines.destroy();

  for (charts.start() ; (chart = (Chart *) charts.next()) != NULL ; )
    delete chart;
  charts.destroy();

  for (tracks.start() ; (track = (Track *) tracks.next()) != NULL ; )
    delete track;
  tracks.destroy();

  for (vehicles.start() ; (vehicle = (Vehicle *) vehicles.next()) != NULL ; )
    delete vehicle;
  vehicles.destroy();

  for (buffers.start() ; (buffer = (Buffer *) buffers.next()) != NULL ; )
    delete buffer;
  buffers.destroy();

  for (realproducts.start() ; (realproduct = (RealProduct *) realproducts.next()) != NULL ; )
    delete realproduct;
  realproducts.destroy();

  for (realvehicles.start() ; (realvehicle = (RealVehicle *) realvehicles.next()) != NULL ; )
    delete realvehicle;
  realvehicles.destroy();

}

void RBSimulation12(int draw,void *data,int,int,View3dSurface *)
{RBSimulation12_data *rbdata = (RBSimulation12_data *) data;
 EntityList newentities,entitiesleft,emptylist;
 long nentities;
 Entity *e;
 Camera *camera;
 CameraReference *camerareference;
 Producer *producer;
 Conveyor *conveyor;
 Consumer *consumer;
 RealProduct *realproduct;
 Machine *machine;
 Track *track;
 Buffer *buffer;
 RealVehicle *realvehicle;

 Point eye,ref,up,zaxis(0.0,0.0,1.0);
 double ambient,diffuse;
 RCCHAR name[128];
 int setreference;
 RECT rect;
 RCHBRUSH hbrush;
 MEMORYSTATUS ms;

  if (draw != RB_TIMER) return;

  setreference = 0;

  up = rbdata->bitmap->getview()->getup().normalize();
  eye = rbdata->bitmap->getview()->geteye();
  ref = rbdata->bitmap->getview()->getref();

  for (rbdata->camerareferences.start() ; (camerareference = (CameraReference *) rbdata->camerareferences.next()) != NULL ; )
    camerareference->update(rbdata->t,&ref,&setreference);

  for (rbdata->cameras.start() ; (camera = (Camera *) rbdata->cameras.next()) != 0 ; )
    camera->update(rbdata->t,! setreference,up,&eye,&ref);

  for (rbdata->producers.start() ; (producer = (Producer *) rbdata->producers.next()) != 0 ; )
    producer->update(rbdata->t);

  for (rbdata->conveyors.start() ; (conveyor = (Conveyor *) rbdata->conveyors.next()) != 0 ; )
    conveyor->update(rbdata->t);

  for (rbdata->machines.start() ; (machine = (Machine *) rbdata->machines.next()) != 0 ; )
    machine->update(rbdata->t);

  for (rbdata->consumers.start() ; (consumer = (Consumer *) rbdata->consumers.next()) != 0 ; )
    consumer->update(rbdata->t);

  for (rbdata->tracks.start() ; (track = (Track *) rbdata->tracks.next()) != 0 ; )
    track->update(rbdata->t);

  for (rbdata->buffers.start() ; (buffer = (Buffer *) rbdata->buffers.next()) != 0 ; )
    buffer->update(rbdata->t);

  for (rbdata->realvehicles.start() ; (realvehicle = (RealVehicle *) rbdata->realvehicles.next()) != 0 ; )
    realvehicle->update(rbdata->t);

  for (rbdata->realproducts.start() ; (realproduct = (RealProduct *) rbdata->realproducts.next()) != 0 ; )
    realproduct->update(rbdata->t);

  up = (eye - ref).cross(up).cross(eye - ref).normalize();
  if ((eye - ref).normalize().cross(zaxis).length() > db.getatoler())
    up = zaxis;
  if (rbdata->bitmap->getview()->changeview(eye,ref,up))
    {  rbdata->bitmap->updatematrix();
       rect.left = 0;  rect.top = 0;
       rect.right = int(rbdata->bitmap->getwidth());  rect.bottom = int(rbdata->bitmap->getheight());
       hbrush = CreateSolidBrush(rbdata->bitmap->getbackground());
       FillRect(rbdata->bitmap->gethdc(),&rect,hbrush);
       DeleteObject(hbrush);
       if (rbdata->shadebefore)
         {  if (rbdata->shading)
              {  RayTraceImage *rti = new RayTraceImage(rbdata->bitmap);
                 if (rti != 0)
                   {  if (rti->loadpolygons())
                        rti->raytrace();
                      delete rti;
                   }
              }
            else
              {  ambient = v.getreal("sh::ambient");
                 diffuse = v.getreal("sh::diffuse");
                 RenderImage image(rbdata->bitmap,0,ambient,diffuse);
                 image.render();
              }
         }
       if (rbdata->wireframe)
         rbdata->bitmap->paintevent();
       else if (rbdata->hide)
         {  HideImage image(rbdata->bitmap,0);
            image.hide();
         }
       if (rbdata->shadeafter)
         {  if (rbdata->shading)
              {  RayTraceImage *rti = new RayTraceImage(rbdata->bitmap);
                 if (rti != 0 && rti->loadpolygons())
                   rti->raytrace();
              }
            else
              {  ambient = v.getreal("sh::ambient");
                 diffuse = v.getreal("sh::diffuse");
                 RenderImage image(rbdata->bitmap,0,ambient,diffuse);
                 image.render();
              }
         }
       BitBlt(rbdata->window->gethdc(),0,0,int(rbdata->window->getwidth()),int(rbdata->window->getheight()),
                   rbdata->bitmap->gethdc(),0,0,SRCCOPY);
    }
  do {  nentities = 0;  entitiesleft = emptylist;
        for (newentities.start() ; (e = newentities.next()) != NULL ; )
          if (e->getalist().empty())
            {  nentities++;  db.geometry.del(e,0);
            }
          else
            entitiesleft.add(e);
        newentities.destroy();
        newentities = entitiesleft;
     } while (nentities > 0);
  newentities.destroy();
  rbdata->frame++;

  memset(&ms,0,sizeof(ms));
  ms.dwLength = sizeof(ms);
  GlobalMemoryStatus(&ms);
  if (ms.dwAvailPageFile < 5000000 && rbdata->format == 3)
    {  AbortEvent *aevent = new AbortEvent;
       state.sendevent(aevent);
       rbdata->frame = -1;
       return;
    }

  sprintf(name,"Cancel the simulation using the right button : Time %.2f",rbdata->t);
  cadwindow->prompt->normalprompt(name);
  if (rbdata->format == 1)
    {  sprintf(name,"%s%3.3d.BMP",rbdata->prefix,rbdata->frame);
       rbdata->window->saveimage(1,name,0);
    }
  else if (rbdata->format == 2)
    {  sprintf(name,"%s%3.3d.TGA",rbdata->prefix,rbdata->frame);
       rbdata->window->saveimage(2,name,0);
    }
  else if (rbdata->format == 3)
    if (! rbdata->bitmap->saveavistream(rbdata->avicompressedstream,rbdata->frame))
      {  AbortEvent *aevent = new AbortEvent;
         state.sendevent(aevent);
         rbdata->frame = -2;
         return;
      }
  rbdata->t += rbdata->dt;
  if (rbdata->t > rbdata->finishtime + TIME_TOL)
    {  AbortEvent *aevent = new AbortEvent;
       state.sendevent(aevent);
    }
}

class SimFListDialogControl : public ListDialogControl
  {private:
   public:
     SimFListDialogControl(int id,int n,RCCHAR **l,int *f) : ListDialogControl(id,n,l,f) {}
     void load(Dialog *dialog);
     void changefocus(Dialog *dialog,int updatescreen = 1);
  };

void SimFListDialogControl::load(Dialog *dialog)
{int type;
  ListDialogControl::load(dialog);
  if (dialog->currentvalue(103,&type))
    {  if (type == 0)
         {  dialog->getcontrol(105)->enable(dialog,0);
            dialog->getcontrol(204)->enable(dialog,0);
            dialog->getcontrol(106)->enable(dialog,0);
            ((StringDialogControl *)dialog->getcontrol(204))->change(dialog, (char*)"Image file prefix");
         }
       else if (type == 1 || type == 2)
         {  dialog->getcontrol(105)->enable(dialog,1);
            dialog->getcontrol(204)->enable(dialog,1);
            dialog->getcontrol(106)->enable(dialog,1);
            ((StringDialogControl *)dialog->getcontrol(204))->change(dialog, (char*)"Image file prefix");
         }
       else if (type == 3)
         {  dialog->getcontrol(105)->enable(dialog,1);
            dialog->getcontrol(204)->enable(dialog,1);
            dialog->getcontrol(106)->enable(dialog,1);
            ((StringDialogControl *)dialog->getcontrol(204))->change(dialog, (char*)"AVI file name");
         }
    }
}

void SimFListDialogControl::changefocus(Dialog *dialog,int updatescreen)
{int type;
  ListDialogControl::changefocus(dialog,updatescreen);
  if (dialog->currentvalue(103,&type))
    {  if (type == 0)
         {  dialog->getcontrol(105)->enable(dialog,0);
            dialog->getcontrol(204)->enable(dialog,0);
            dialog->getcontrol(106)->enable(dialog,0);
            ((StringDialogControl *)dialog->getcontrol(204))->change(dialog, (char*)"Image file prefix");
         }
       else if (type == 1 || type == 2)
         {  dialog->getcontrol(105)->enable(dialog,1);
            dialog->getcontrol(204)->enable(dialog,1);
            dialog->getcontrol(106)->enable(dialog,1);
            ((StringDialogControl *)dialog->getcontrol(204))->change(dialog, (char*)"Image file prefix");
         }
       else if (type == 3)
         {  dialog->getcontrol(105)->enable(dialog,1);
            dialog->getcontrol(204)->enable(dialog,1);
            dialog->getcontrol(106)->enable(dialog,1);
            ((StringDialogControl *)dialog->getcontrol(204))->change(dialog, (char*)"AVI file name");
         }
    }
}

class SimFButtonDialogControl : public ButtonDialogControl
  {public:
     SimFButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int SimFButtonDialogControl::select(Dialog *dialog)
{OPENFILENAME ofn;
 RCCHAR szDirName[256];
 RCCHAR szFile[256],szFileTitle[256];
 UINT  i;
 RCCHAR  szFilter[256];
 int type;
  _getcwd(szDirName, sizeof(szDirName));
  szFile[0] = '\0';
  dialog->currentvalue(103,&type);
  memset(&ofn, 0, sizeof(OPENFILENAME));
  if (type == 1)
    {  strcpy(szFilter,"Windows bitmap *.bmp|*.bmp|");
       ofn.lpstrTitle = _RCT("Select the prefix for the Windows bitmap files");
    }
  else if (type == 2)
    {  strcpy(szFilter,"TARGA *.tga|*.tga|");
       ofn.lpstrTitle = _RCT("Select the prefix for the TARGA files");
    }
  else if (type == 3)
    {  strcpy(szFilter,"AVI file name *.avi|*.avi|");
       ofn.lpstrTitle = _RCT("Select the AVI file");
    }
  for (i = 0; szFilter[i] != '\0'; i++)
    if (szFilter[i] == '|') szFilter[i] = '\0';
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = dialog->gethdlg();
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof(szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  if (type == 1)
    ofn.lpstrDefExt = _RCT("BMP");
  else if (type == 2)
    ofn.lpstrDefExt = _RCT("TGA");
  else if (type == 3)
    ofn.lpstrDefExt = _RCT("AVI");
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
  if (GetSaveFileName(&ofn))
    {  if (type != 3)
         {  if (strrchr(szFile,'.') != 0 && strrchr(szFile,'\\') == 0)
              *strrchr(szFile,'.') = 0;
            else if (strrchr(szFile,'.') != 0 && strrchr(szFile,'\\') != 0 && strrchr(szFile,'.') > strrchr(szFile,'\\'))
              *strrchr(szFile,'.') = 0;
         }
       ((StringDialogControl *)dialog->getcontrol(105))->change(dialog,ofn.lpstrFile);
    }
  return 0;
}

void simulation_command12(int *cstate,HighLevelEvent *,void **data)
{RBSimulation12_data *rbdata = (RBSimulation12_data *) *data;
 int i,shading,framessec,format,wireframe,hide,shadebefore,shadeafter,width,height,colour;
 double starttime,finishtime;
 Dialog dialog("Simulationrun_Dialog");
 DialogControl *dc;
 RCCHAR prefix[128],scene[256],label[256];
 static RCCHAR *formatl[] = {  _RCT("Don't save"),_RCT("Windows bitmap"),_RCT("TARGA"),_RCT("Windows AVI")  };
 AVISTREAMINFO strhdr;
 AVICOMPRESSOPTIONS *otable[1];
  switch (*cstate)
    {  case InitialState :
         if (cadwindow->getcurrentsurface() == NULL)
           { RCMessageBox(cadwindow->gethwnd(),"Please select a view to animate","Run simulation",MB_ICONINFORMATION);
              *cstate = ExitState;
              break;
           }
         framessec = v.getinteger("sm::framessec");
         format = v.getinteger("sm::format");
         strcpy(prefix,v.getstring("sm::prefix"));
         wireframe = v.getinteger("sm::wireframe");
         starttime = v.getreal("sm::starttime");
         finishtime = v.getreal("sm::finishtime");
         hide = v.getinteger("sm::hide");
         shadebefore = v.getinteger("sm::shadebefore");
         shadeafter = v.getinteger("sm::shadeafter");
         shading = v.getinteger("sm::shading");
         width = (int)(cadwindow->getcurrentwindow()->getwidth() + 0.5);
         height = (int)(cadwindow->getcurrentwindow()->getheight() + 0.5);
         colour = 255;
         dialog.add(dc = new RealDialogControl(100,&starttime,0.1,0.0,100000.0));
         dialog.add(new ScrollBarDialogControl(1100,dc));
         dialog.add(dc = new RealDialogControl(101,&finishtime,0.1,0.0,100000.0));
         dialog.add(new ScrollBarDialogControl(1101,dc));
         dialog.add(dc = new IntegerDialogControl(102,&framessec,2,10000));
         dialog.add(new ScrollBarDialogControl(1102,dc));
         dialog.add(new SimFListDialogControl(103,4,formatl,&format));
         dialog.add(dc = new IntegerDialogControl(104,&colour,0,255));
         dialog.add(new ScrollBarDialogControl(1104,dc));
         dialog.add(new StringDialogControl(105,prefix,100));
         dialog.add(new SimFButtonDialogControl(106));
         strcpy(label,_RCT(""));
         dialog.add(new StringDialogControl(204,label,100));
         dialog.add(dc = new IntegerDialogControl(107,&width,2,10000));
         dialog.add(new ScrollBarDialogControl(1107,dc));
         dialog.add(dc = new IntegerDialogControl(108,&height,2,10000));
         dialog.add(new ScrollBarDialogControl(1108,dc));
         dialog.add(new CheckBoxDialogControl(109,&wireframe));
         dialog.add(new CheckBoxDialogControl(110,&hide));
         dialog.add(new CheckBoxDialogControl(111,&shadebefore));
         dialog.add(new CheckBoxDialogControl(112,&shadeafter));
         dialog.add(new RadioButtonDialogControl(113,113,114,&shading));
         dialog.add(new RadioButtonDialogControl(114,113,114,&shading));
         if (dialog.process() == TRUE)
           {  v.setinteger("sm::framessec",framessec);
              v.setinteger("sm::format",format);
              v.setstring("sm::prefix",prefix);
              v.setinteger("sm::wireframe",wireframe);
              v.setreal("sm::starttime",starttime);
              v.setreal("sm::finishtime",finishtime);
              v.setinteger("sm::hide",hide);
              v.setinteger("sm::shadebefore",shadebefore);
              v.setinteger("sm::shadeafter",shadeafter);
              v.setinteger("am::shading",shading);
              simulationdata = rbdata = new RBSimulation12_data;
              *data = rbdata;
              if (rbdata == NULL)
                {  state.fail(Memory,"Run simulation command");
                   *cstate = ExitState;
                }
              else if (rbdata->cameras.length() == 0 && rbdata->camerareferences.length() == 0 && rbdata->conveyors.length() == 0 && rbdata->tracks.length() == 0)
                { RCMessageBox(cadwindow->gethwnd(),"No camera paths, no camera reference paths, no conveyors and no tracks were found.","Run simulation",MB_ICONINFORMATION);
                   *cstate = ExitState;
                }
              else
                {  rbdata->window = cadwindow->getcurrentwindow();
                   rbdata->window->sendsizeevent(width,height);
                   if ((rbdata->bitmap = new View3dOffScreenBitmap(rbdata->window)) != NULL)
                     {  if (! rbdata->bitmap->getdefined())
                          {  delete rbdata->bitmap;
                             rbdata->bitmap = NULL;
                             *cstate = ExitState;
                          }
                        else
                          {  rbdata->bitmap->setbackground(RGB(db.colourtable[colour].red,db.colourtable[colour].green,db.colourtable[colour].blue));
                             rbdata->starttime = starttime;
                             rbdata->finishtime = finishtime;
                             rbdata->t = starttime;
                             rbdata->dt = 1.0 / double(framessec);
                             rbdata->frame = 0;
                             strcpy(rbdata->prefix,prefix);
                             strcpy(rbdata->scene,scene);
                             rbdata->format = format;
                             rbdata->wireframe = wireframe;
                             rbdata->hide = hide;
                             rbdata->shadebefore = shadebefore;
                             rbdata->shadeafter = shadeafter;
                             rbdata->shading = shading;
                             for (i = 900 ; i < 1000 ; i++)
                               db.dismask.layer.clear(i);
                             *cstate = 1;
                             if (format == 3)
                               {
#if defined(AVI)
                                  AVIFileInit();
                                  if (AVIFileOpen(&rbdata->avifile,rbdata->prefix,OF_WRITE | OF_CREATE,0) != AVIERR_OK)
                                    {  delete rbdata->bitmap;
                                       rbdata->bitmap = 0;
                                       *cstate = ExitState;
                                    }
                                  else
                                    {  memset(&strhdr, 0, sizeof(strhdr));
                                       strhdr.fccType = streamtypeVIDEO;// stream type
                                       strhdr.fccHandler = 0;
                                       strhdr.dwScale = 1;
                                       strhdr.dwRate = 20;
                                       strhdr.dwSuggestedBufferSize  = long(width) * long(height) * 3;
                                       SetRect(&strhdr.rcFrame,0,0,width,height);
                                       if (AVIFileCreateStream(rbdata->avifile,&rbdata->avistream,&strhdr) != AVIERR_OK)
                                         {  delete rbdata->bitmap;
                                            rbdata->bitmap = 0;
                                            *cstate = ExitState;
                                         }
                                       else
                                         {  memset(&rbdata->opts,0,sizeof(rbdata->opts));
                                            otable[0] = & rbdata->opts;
                                            if (! AVISaveOptions(cadwindow->gethwnd(),ICMF_CHOOSE_KEYFRAME | ICMF_CHOOSE_DATARATE ,1,&rbdata->avistream,(LPAVICOMPRESSOPTIONS  *) &otable))
                                              {  delete rbdata->bitmap;
                                                 rbdata->bitmap = 0;
                                                 *cstate = ExitState;
                                              }
                                            else if (AVIMakeCompressedStream(&rbdata->avicompressedstream,rbdata->avistream,&rbdata->opts,0) != AVIERR_OK)
                                              {  delete rbdata->bitmap;
                                                 rbdata->bitmap = 0;
                                                 *cstate = ExitState;
                                              }
                                         }
                                    }
#endif
                               }
                          }
                     }
                   else
                     *cstate = ExitState;
                }
           }
         else
           {  rbdata = NULL;  *cstate = ExitState;
           }
         break;
       case 1:
         rubberband.end(0);
         program->setbusy(0);
         cadwindow->updatewindows(rbdata->window->getview());
         for (i = 900 ; i < 1000 ; i++)
           db.dismask.layer.set(i);
         *cstate = ExitState;
         break;
     }
  if (*cstate == 1)
    {  program->setbusy(1);
       state.selmask.entity.clearall();
       rubberband.begin(0);
       rubberband.add(RBSimulation12,rbdata);
       cadwindow->prompt->normalprompt("Cancel the simulation using the right button");
    }
  else if (*cstate == ExitState)
    {  if (rbdata != 0 && rbdata->format == 3)
         {
#if defined(AVI)
            if (rbdata->avicompressedstream)
              AVIStreamRelease(rbdata->avicompressedstream);
            if (rbdata->avistream)
	           AVIStreamRelease(rbdata->avistream);
            if (rbdata->avifile)
              AVIFileRelease(rbdata->avifile);
            AVIFileExit();
            if (rbdata->frame == -1)
             RCMessageBox(cadwindow->gethwnd(),"AVI file cannot be created - memory low.","Run Simulation",MB_ICONINFORMATION);
            else if (rbdata->frame == -2)
             RCMessageBox(cadwindow->gethwnd(),"AVI file cannot be created - Compressor does not support 24 bits.","Run Simulation",MB_ICONINFORMATION);
#endif
         }
       if (rbdata != NULL && rbdata->bitmap != NULL) delete rbdata->bitmap;
       delete rbdata;
    }
}

#endif
