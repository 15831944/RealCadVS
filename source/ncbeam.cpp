
#include "ncwin.h"


   
   
   
 
Beam::Beam(const EntityHeader&,short _iType, short _iProfile, Point _p1,Point _p2,Point _zaxis,char *n,char *_strLabel,double _width,double _height,double _dblThickness,double _dblLip,short nattributes_,double *attributes_,const BitMask &o) : options(32), optTopPlane(32), optBottomPlane(32), optOutsidePlane(32), optInsidePlane(32)
{ defined = 1;
}

Beam::Beam(short _iType, short _iProfile, Point _ptStart,Point _ptEnd,Point _ptZAxis,char *n,char *_strLabel,double _dblWidth,double _dblHeight, short nattributes_,double *attributes_,const BitMask &o) : options(32) , optTopPlane(32), optBottomPlane(32), optOutsidePlane(32), optInsidePlane(32)
{ defined = 1;
}

Beam::Beam(short _iType, short _iProfile, Point _ptStart,Point _ptEnd,Point _ptZAxis,char *n,char *_strLabel,double _dblWidth,double _dblHeight,double _dblThickness,double _dblLip, short nattributes_,double *attributes_,const BitMask &o) : options(32), optTopPlane(32), optBottomPlane(32), optOutsidePlane(32), optInsidePlane(32)
{ defined = 1;
}

Beam::~Beam()
{
}

void Beam::SetDefaultLineProperties()
{
}

void Beam::SetCentreLineProperties(int iState, int iColour, int iStyle, double dblWeight)
{
}

void Beam::SetOutsideLineProperties(int iState, int iColour, int iStyle, double dblWeight)
{
}

void Beam::SetInsideLineProperties(int iState, int iColour, int iStyle, double dblWeight)
{
}


Plane *Beam::getleaf(char *leaf)
{ return 0;
}

int Beam::issame(int,const Entity &) const
{ return 0;
}

double Beam::pick(View3dSurface *,double,double,Point *)
{ return 0.1;
}

int Beam::pick(View3dSurface *,int,int,int,int,int)
{ return 0;
}

int Beam::pick(View3dSurface *,int,Point *,int)
{ return 0;
}

void Beam::draw(int,Transform *,View3dSurface *)
{
}

void Beam::addtodisplaylist(View3dSurface *,int)
{
}

int Beam::hide(HideImage *image)
{ return 0;
}

int Beam::add(Figure *)
{ return 0;
}

void Beam::del(Figure *)
{
}

int Beam::add(Line *)
{ return 0;
}

void Beam::del(Line *)
{
}

int Beam::npoints(void)
{ return 1;
}

Point Beam::point(int)
{ return Point(0.0,0.0,0.0);
}

Point Beam::nearp(Point)
{ return Point(0.0,0.0,0.0);
}

void Beam::nfacets(double,int *,int *)
{ 
}

int Beam::facet(const BitMask &,double,int,int,View3dSurface *,PolygonCallBack,void *)
{ return 0;
}

void Beam::triangulate(double)
{
}

void Beam::dbinsert(Database *,int draw)
{ 
}

void Beam::dbdelete(void)
{ 
}

void Beam::dbsetassociated(void)
{
}

void Beam::dbtraverseassociated(EntityCallBack,void *)
{
}

void Beam::fixupreferences(Entity ** index)
{ 
}

int Beam::pickpoint(View3dSurface *,int,int,int,Point *)
{ return 0;
}

int Beam::stretchent(Database *,Transform &,SortedPointTable *,int erase)
{ return 0;
}

int Beam::move(Database *,Transform &,int erase)
{ return 0;
}

Entity *Beam::clone(Transform &)
{ return 0;
}

int Beam::project(Database *,Transform &,int)
{ return 0;
}

void Beam::change(const BitMask &change,const EntityHeader &eh,char *strBeamType,double dblWidth,double dblHeight,double dblThickness, double dblLip, int iType, 
            int iDrawOutsideLineBold, int iOutsideLineColour, int iOutsideLineStyle, double dblOutsideLineWeight,
            int iDrawCentreLine, int iCentreLineColour, int iCentreLineStyle, double dblCentreLineWeight,
            int iDrawInsideLineBold, int iInsideLineColour, int iInsideLineStyle, double dblInsideLineWeight,
            short nattributes,double *attributes,BitMask &o)
{
}

EntityList Beam::changegeometry(Point pn1,Point pn2)
{static EntityList l;
  return l;
}

int Beam::setup(BitMask *change,char *strBeamType,double *dblWidth,double *dblHeight,double *dblThickness, double *dblLip, int *iType,
            int *iDrawOutsideLineBold, int *iOutsideLineColour, int *iOutsideLineStyle, double *dblOutsideLineWeight,
            int *iDrawCentreLine, int *iCentreLineColour, int *iCentreLineStyle, double *dblCentreLineWeight,
            int *iDrawInsideLineBold, int *iInsideLineColour, int *iInsideLineStyle, double *dblInsideLineWeight,
            short *nattributes_,double *attributes_,BitMask *o)
{ return 0;
}


void Beam::getdefaults(BOOL bFromProfile, char *strBeamType,double *dblWidth,double *dblHeight,double *dblThickness, double *dblLip, int *iType,
            int *iDrawOutsideLineBold, int *iOutsideLineColour, int *iOutsideLineStyle, double *dblOutsideLineWeight,
            int *iDrawCentreLine, int *iCentreLineColour, int *iCentreLineStyle, double *dblCentreLineWeight,
            int *iDrawInsideLineBold, int *iInsideLineColour, int *iInsideLineStyle, double *dblInsideLineWeight,
            short *nattributes,double *attributes,BitMask *o)
{
}

void Beam::setdefaults(char *strBeamType,double dblWidth,double dblHeight,double dblThickness, double dblLip, int iType,
            int iDrawOutsideLineBold, int iOutsideLineColour, int iOutsideLineStyle, double dblOutsideLineWeight,
            int iDrawCentreLine, int iCentreLineColour, int iCentreLineStyle, double dblCentreLineWeight,
            int iDrawInsideLineBold, int iInsideLineColour, int iInsideLineStyle, double dblInsideLineWeight,
            short nattributes,double *attributes,BitMask &o)
{
}

void Beam::change(const BitMask &change,const EntityHeader &eh,char *n,double w,double h,short nattributes_,double *attributes_,const BitMask &op)
{
}

void Beam::getinfo(char *strBeamType,double *dblWidth,double *dblHeight,double *dblThickness, double *dblLip, int *iType,
               int *iDrawOutsideLineBold, int *iOutsideLineColour, int *iOutsideLineStyle, double *dblOutsideLineWeight,
               int *iDrawCentreLine, int *iCentreLineColour, int *iCentreLineStyle, double *dblCentreLineWeight,
               int *iDrawInsideLineBold, int *iInsideLineColour, int *iInsideLineStyle, double *dblInsideLineWeight,
               short *nattributes,double *attributes,BitMask *o)
{
}

void Beam::verify(void)
{
}

int Beam::explode(void)
{ return 0;
}

void Beam::extents(View3dSurface *,Point *,Point *)
{
}

int Beam::save(CadFile *)
{ return 0;
}

Entity *Beam::load(int,CadFile *)
{ return 0;
}

int Beam::save(UndoFile *)
{ return 0;
}

Entity *Beam::load(UndoFile *)
{ return 0;
}

int Beam::savegeometry(CadFile *)
{ return 0;
}

int Beam::savedxf(int,DXFOut *)
{ return 0;
}

int Beam::loadgeometry(CadFile *)
{ return 0;
}

int Beam::saveattributes(CadFile *)
{ return 0;
}

int Beam::loadattributes(CadFile *)
{ return 0;
}

double Beam::getarea(int nsegs,double error)
{ return 0;
}

int Beam::exportiges(FILE *outfile,int section,int options,int *ndirectory,int *nparameter)
{ return 0;
}

