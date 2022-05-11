#include "ncwin.h"
#include <QtGlobal>

extern void transformlist(int project, int ncopies, EntityList list, Transform t, int surface,int save=true);
extern void RBLine1(int dm, void *data, Point *p2, View3dSurface *v);
extern bool copyAlongMapContains(EntityList list);


struct ScaleRubberBandData {

#ifdef USING_WIDGETS
	qreal zValue;	
#endif
	Point p1, p2, xaxis, yaxis,zaxis;
	int state, mode;
	double textHeight;
	Point offset;
	
	EntityList list;
};
ScaleRubberBandData* scaleRBData = 0;
void drawRectangle(int insert,  Point p1, Point p2, Point p3, ScaleRubberBandData *rbdata = 0)
{
	Line line1(p1, p2);		
	line1.draw(DM_INVERT);
		
	Line line2(p2, p3);		
	line2.draw(DM_INVERT);
		
	Line line3(p3, p3 + p1 - p2); line3.zValue = rbdata->zValue;		
	line3.draw(DM_INVERT);
		
	Line line4(p3 + p1 - p2, p1);		
	line4.draw(DM_INVERT);
}
double lastAngle;
Point lastPt;
#ifdef NEW_RUBBERB
void boundingBoxRubberBand(int dm, void **data, Point *p2, View3dSurface *v) {
	if (dm == RB_ERASE)
	{
		cadwindow->clearAllRubberBands();
		return;
	}

	ScaleRubberBandData* rbdata = (ScaleRubberBandData *)(*data);
	if (rbdata == 0) rbdata = scaleRBData;
	if (rbdata->state == 2) {
		p2->z = rbdata->zValue;
	
		if (db.getdragdetail() == 0)
		{
			cadwindow->startdrawingbatch();
			View3dWindow* view = dynamic_cast<View3dWindow*>(v);
			if (view != 0) {
				
				Point pt = view->boundingBox.getDraggingPt2(view, *p2);
				
				if (rbdata->mode == 0) {
					drawRectangle(0, rbdata->p1, rbdata->p1 + rbdata->xaxis*rbdata->xaxis.dot(pt - rbdata->p1), pt, rbdata);
					lastPt = pt;					
					view->boundingBox.scaleTransfomList(view, pt);				
					
				}
				else if (rbdata->mode == 1) {					
					Line axis(rbdata->p1, rbdata->p2);
					axis.draw(DM_INVERT);
					Line line(rbdata->p1, *p2);
					line.draw(DM_INVERT);
					
					lastPt =*p2;
					Transform t;
					double angle;					
					view->boundingBox.getRotateTransfom(view, *p2, t,angle);
					RCCHAR text[300];
					BitMask toptions(32);
					strcpy(text, db.formatnumber(text, angle/M_PI*180, 0));
					Text texte(CHAR2RCCHR("Arial"),*p2 + rbdata->offset, Point(1.0, 0.0, 0.0), Point(0.0, 1.0, 0.0), 0.0, rbdata->textHeight, 0, 0.0, 0, text, 0, toptions);
					if (::v.getinteger("wn::distancetracking"))
					{
						texte.draw(DM_INVERT);
					}
					//transformlist(0, 0, rbdata->list, t, 0, 0);					
					Entity*e = 0;
					for (rbdata->list.start(); (e = rbdata->list.next()) != NULL; )
						e->draw(DM_INVERT, &t);
				}
			}
			cadwindow->enddrawingbatch();

		}
	}
}
#endif
#include <QTime>


void boundingBoxScale_command(int *cstate, HighLevelEvent *event, void **data)
{

	ScaleRubberBandData *rbdata = (ScaleRubberBandData *)*data;
	ResourceString rs17(NCLINEB + 17);
	switch (*cstate)
	{
	case InitialState:

		if ((rbdata = new ScaleRubberBandData) == NULL)
		{
			state.fail(Memory, rs17.gets());
			*cstate = ExitState;
		}
		else {
			
			*data = rbdata;
			scaleRBData = rbdata;
						
			if (event->isa(NewCommand))
			{
				View3dWindow* view =(View3dWindow*) ((NewCommandEvent *)event)->getwindow();
				if (view != 0) {
					
					rbdata->mode = view->boundingBox.protractorIsDisplayed();					
					rbdata->xaxis = view->getxaxis();
					rbdata->yaxis = view->getyaxis();
					rbdata->zaxis = view->getxaxis().cross(view->getyaxis());
					rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
					rbdata->list= view->boundingBox.getSelectedList();
					rbdata->p1 = view->boundingBox.getRuberBandPt1(view);
					if (rbdata->mode == 1)
						rbdata->p2 = view->boundingBox.handleEndPt(view);
					rbdata->textHeight = 6.0 * view->getscale();
					rbdata->offset = view->getuaxis() * 6.0 * view->getscale() + view->getvaxis() * 6.0 * view->getscale();
					*cstate = 2;
					
				}
				else
					*cstate = ExitState;
			}
		}
		break;	
	case 2:
		rubberband.end(0);
		if (event->isa(Coordinate3d))
		{
			View3dWindow* view = ((Coordinate3dEvent *)event)->getwindow();
			if (view != 0) {
				
				if (rbdata->mode == 0) {	
					Transform t;
					view->boundingBox.getScaleTransfom(view, lastPt, t);
					transformlist(0, 0, rbdata->list, t, 0,0);					
					db.saveundo(UD_BOUNDINGBOX, NULL);
					view->boundingBox.updateUndoData();
				}
				else {
					Transform t;
					double angle;					
					view->boundingBox.getRotateTransfom(view, lastPt, t,angle);
					transformlist(0, 0, rbdata->list, t, 0);					
				}
				view->boundingBox.update(lastPt);
				view->boundingBox.clear();
				//state.setselection(rbdata->list);
			}
			
		}		
		*cstate = ExitState;
		break;
	};

	if (*cstate != ExitState)
	{
		rbdata->state = *cstate;
		rubberband.begin(0);
		rubberband.add((RubberbandCallback)boundingBoxRubberBand, rbdata);
		state.selmask.entity.clearandset(xyz_mask, end_list);
	}
	else {
		delete scaleRBData;
		scaleRBData = 0;
	}
}


BoundingBox::BoundingBox() {
	minPt = maxPt = Point();	
	isProtractor = false;
	dragMode = Undefined;
}

BoundingBox::~BoundingBox() {
	undoStack.clear();
	redoStack.clear();
	entityIds.clear();
}
void BoundingBox::clear() {	
	minPt = maxPt = Point();
	entityIds.clear();
	dragMode = Undefined;
	isProtractor = false;
	ctrlKeyIsPressed = false;
	
}
bool  BoundingBox::isValid() {
	return !(minPt == maxPt);
}
double BoundingBox::getProtractorSize() const {
	return min(width(), height()) / 3;	
}
void BoundingBox::onLeftButtonDownEvent(View3dWindow *s, const Point& pt, int buttons) {
	if (!isValid()) return;	
	ctrlKeyIsPressed = (buttons & MK_CONTROL) == MK_CONTROL;
	
	if (dragLeftTopHandle(s, pt))
		dragMode = LeftTop;
	else if (dragRightTopHandle(s, pt))
		dragMode = RightTop;
	else if (dragLeftBotHandle(s, pt))
		dragMode = LeftBottom;
	else if (dragRightBotHandle(s, pt))
		dragMode = RightBottom;
	else if (ptIsOnEdge(leftTopPt(s), rightTopPt(s), pt))
		dragMode = TopEdge;
	else if (ptIsOnEdge(rightTopPt(s), rightBotPt(s), pt))
		dragMode = RightEdge;
	else if (ptIsOnEdge(leftBotPt(s), rightBotPt(s), pt))
		dragMode = BottomEdge;
	else if (ptIsOnEdge(leftTopPt(s), leftBotPt(s), pt))
		dragMode = LeftEdge;
	else
		dragMode = Undefined;

	if (dragMode != Undefined) {
		dragPt1 = getDraggingPt1(s, pt);
		item.center = centerPt();
		item.minPt = minPt;
		item.maxPt = maxPt;

		NewCommandEvent *nevent = new NewCommandEvent(70001, s);
		state.sendevent(nevent);
		return;
	}
	if (!isProtractor) {
		double size = getProtractorSize() /4;
		Point temp = centerPt();
		Point center = transformToScreen(s, temp.x, temp.y);
		QRect rect(center.x - size / 2, center.y - size / 2, size, size);
		if (rect.contains(pt.x, pt.y)) {
			isProtractor = true;
			dragPt1 = center;
			NewCommandEvent *nevent = new NewCommandEvent(70001, s);
			state.sendevent(nevent);
			return;
		}
	}
	
}
void BoundingBox::scaleTransfomList(View3dWindow* view, const Point& pt) {		
	Transform t;
	getScaleTransfom(view,pt, t);	
	EntityList list = getSelectedList();
	transformlist(0, 0, list, t, 0,0);	
	update(pt);
	newItem.center = getCenterPt(view,pt);
	newItem.minPt = minPt;
	newItem.maxPt = maxPt;

}


void BoundingBox::computeBoundingBox(View3dWindow* view3d,EntityList list) {

	if (copyAlongMapContains(list)) {
		cadwindow->MessageBox("Path entity can't be scaled!", "Copy Along", MB_ICONINFORMATION);
		return;
	}

	minPt = maxPt = Point();
	entityIds.clear();
		
	Entity* e = 0;
	for (list.start(); (e = list.next()) != nullptr; ) {
		entityIds.append(e->getid());
		if( minPt ==maxPt) {
			e->extents(view3d, &minPt, &maxPt);
			continue;
		}
		Point p1, p2;
		e->extents(view3d, &p1, &p2);

		if (p1.x <minPt.x) minPt.x = p1.x;
		if (p1.y < minPt.y) minPt.y = p1.y;
		if (p1.z < minPt.z) minPt.z = p1.z;
		if (p2.x > maxPt.x) maxPt.x = p2.x;
		if (p2.y > maxPt.y) maxPt.y = p2.y;
		if (p2.z > maxPt.z) maxPt.z = p2.z;
	}	
	
}

Point BoundingBox::calculateScale(const Point& p) {
	
	Point oldMinPt = minPt;
	Point oldMaxPt = maxPt;
	update(p);
	double oldWidth = qAbs(oldMaxPt.x - oldMinPt.x);
	double oldHeight= qAbs(oldMaxPt.y - oldMinPt.y);
	return getScale(width(), height(), oldWidth, oldHeight);
}
Point BoundingBox::getScale(double curWidth, double curHeight, double prevWidth, double prevHeight) {
	Point scale(1, 1, 1);
	if (prevWidth > db.getptoler() && prevHeight > db.getptoler())
		scale.setxyz(curWidth / prevWidth, curHeight / prevHeight, 1.0);
	return scale;
}
void BoundingBox::doScaleTransform(int undoing, const BoundingBoxData& data) {
	Transform t;
	getScaleTransfom(undoing, data, t);
	EntityList list = getEntityList(data.entityIds);
	transformlist(0, 0, list, t, 0, 0);
}
void BoundingBox::getScaleTransfom(int undoing, const BoundingBoxData& data, Transform& t) {
	View3dWindow* view = cadwindow->getcurrentwindow();
	Point xaxis = view->getxaxis();
	Point yaxis = view->getyaxis();
	Point center= data.prevItem.center;
	Point scale;
	if (undoing == 1) {
		scale = getScale(data.prevItem.width(), data.prevItem.height(), data.curntItem.width(), data.curntItem.height());
	}
	else if (undoing == 2) {
		scale = getScale(data.curntItem.width(), data.curntItem.height(), data.prevItem.width(), data.prevItem.height());
	}
	t.scale(center, xaxis, yaxis, xaxis.cross(yaxis), scale);
}

void BoundingBox::undo(int undoing) {	
	if (protractorIsDisplayed()) return;
	BoundingBoxData data;
	if (undoing == 1){  //undo opeartion  	 
		data = undoStack.pop();
		doScaleTransform(undoing, data);
		redoStack.push(data);
	}
	else if (undoing == 2) {//redo operation
		data = redoStack.pop();
		doScaleTransform(undoing, data);
		undoStack.push(data);
	}

}
void BoundingBox::updateUndoData() {
	BoundingBoxData data;
	data.prevItem = item;
	data.curntItem = newItem;
	data.entityIds = entityIds;
	undoStack.push(data);
}
Point BoundingBox::getRuberBandPt1(View3dWindow* view) const {
	Point rbPt1;
	view->coordinate3d(dragPt1.x, view->getheight() - dragPt1.y, &rbPt1);
	return rbPt1;
}

Point BoundingBox::getDraggingPt1(View3dWindow *s, const Point& pt) const {
	if (protractorIsDisplayed()) {
		return handleStartPt(s);
	}
	else
		switch (dragMode) {
			case TopEdge: return leftBotPt(s);
			case RightEdge: return leftTopPt(s);
			case BottomEdge: return leftTopPt(s);
			case LeftEdge:  return rightTopPt(s);
			default: return getOppositePtToSelectPt(s,pt);
		}
}
Point BoundingBox::getDraggingPt2(View3dWindow *s, const Point& pt) const {
	
	Point temp;
	switch (dragMode) {
		case TopEdge:
			temp = maxPt;// rightTopPt(s);
			temp.y = pt.y;
			return temp;
		case RightEdge: 
			temp= Point(pt.x, minPt.y, minPt.z); //rightBotPt(s);
			return temp;
		case BottomEdge: 
			temp = Point(maxPt.x, pt.y, minPt.z);// rightBotPt(s);
			return temp;			
		case LeftEdge:  
			temp = minPt; // leftBotPt(s);
			temp.x = pt.x;			
			return temp;
		default: return pt;
	}

}
Point BoundingBox::getOppositePtToSelectPt(View3dSurface *s, const Point& pt) const{
	bool isForCenter = ctrlKeyIsPressed;
	switch (dragMode) {
		case LeftTop: return rightBotPt(s, isForCenter);
		case LeftBottom: return rightTopPt(s, isForCenter);
		case RightTop: return leftBotPt(s, isForCenter);
		case RightBottom: return leftTopPt(s, isForCenter);
		case TopEdge: return (leftBotPt(s, isForCenter) + rightBotPt(s, isForCenter)) / 2;
		case RightEdge: return (leftBotPt(s, isForCenter) + leftTopPt(s, isForCenter)) / 2;
		case BottomEdge: return (leftTopPt(s, isForCenter) + rightTopPt(s, isForCenter)) / 2;
		case LeftEdge:   return (rightTopPt(s, isForCenter) + rightBotPt(s, isForCenter)) / 2;
		default:return pt;
	}
}
//this will be possible useless
void BoundingBox::doRotation(View3dWindow* view, const Point& p, double& angle) {

	Point cPt = getRuberBandPt1(view);
	Point xaxis = handleEndPt(view) - cPt;
	Point yaxis = p - cPt;
	if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
	{
		xaxis.normalize();
		yaxis.normalize();
		Point zaxis;
		if ((zaxis = xaxis.cross(yaxis)).length() < db.getptoler())
			zaxis = Point(0.0, 0.0, 1.0);
		double cosa = xaxis.dot(yaxis);
		if (cosa < -1.0)
			cosa = -1.0;
		else if (cosa > 1.0)
			cosa = 1.0;
		angle = acos(cosa);
		Transform t,t1;
		t1.translate(0,0,0);
		double unitAngle = 5 * 3.1416 / 180;
		int n = qAbs(angle / unitAngle);
		Point dir;
		if (angle > 0)//at right side of xaxis
			dir = Point(1, 0, 0);
		else
			dir = Point(0, 1, 0);
		Entity*e = 0;
		EntityList list = getSelectedList();
		for (int i = 1; i <= n; i++) {
			//Point move = cPt + dir * HandleSize*i*unitAngle;
			//t1.translate(move.x, move.y, move.z);
			t.rotate(cPt, zaxis, unitAngle*i);
			t.apply(t1);
			for (list.start(); (e = list.next()) != NULL; )
				e->draw(DM_INVERT, &t);
		}
	}
}
Point BoundingBox::getCenterPt(View3dWindow* view, const Point& p) const{
	Point center = centerPt();
	if (ctrlKeyIsPressed)
		center = getOppositePtToSelectPt((View3dSurface *)view, p);
	return center;
}

void BoundingBox::getRotateTransfom(View3dWindow* view, const Point& p, Transform& t,double& angle) {
	
	Point cPt = getRuberBandPt1(view);
	Point xaxis = handleEndPt(view) -cPt ;
	Point yaxis = p - cPt;
	if (xaxis.length() > db.getptoler() && yaxis.length() > db.getptoler())
	{
		xaxis.normalize(); 
		yaxis.normalize();
		Point zaxis;
		if ((zaxis = xaxis.cross(yaxis)).length() < db.getptoler())
			zaxis = Point(0.0, 0.0, 1.0);
		double cosa = xaxis.dot(yaxis);
		if (cosa < -1.0) 
			cosa = -1.0; 
		else if (cosa > 1.0) 
			cosa = 1.0;
		angle = acos(cosa);
		Transform t1;		
		t1.translate(0, 0, 0);
		t.rotate(cPt, zaxis, angle);
		t.apply(t1);
	}
}

void BoundingBox::getScaleTransfom(View3dWindow* view, const Point& p, Transform& t) {
	Point scale = calculateScale(p);
	Point xaxis = view->getxaxis();
	Point yaxis = view->getyaxis();
	
	Point center = getCenterPt(view,p);		
	t.scale(center, xaxis, yaxis, xaxis.cross(yaxis), scale);
}
EntityList BoundingBox::getEntityList(const QList<long>& entityIds) {
	EntityList list;
	for (int i = 0; i < entityIds.length(); i++) {
		Entity* e = db.geometry.getentity(entityIds.at(i));
		if (e != nullptr)
			list.add(e);
	}
	return list;
}
EntityList BoundingBox::getSelectedList() {
	return getEntityList(entityIds);
}

bool BoundingBox::rotateHandleIsSelecteded(View3dWindow *s,const Point& pt) {	
	return (protractorIsDisplayed() && ptIsOnEdge(handleStartPt(s), handleEndPt(s), pt));	
}
double BoundingBox::getRotationHandleLength() const {
	return 0.65*height();
}
Point BoundingBox::handleStartPt(View3dWindow *s) const{
	Point temp = centerPt();	
	return transformToScreen(s, temp.x, temp.y);
}
Point BoundingBox::handleEndPt(View3dWindow *s) const {
	double length = getRotationHandleLength();
	Point p1 = getRuberBandPt1(s);
	return p1 + s->getyaxis()*length;
}


void BoundingBox::update(const Point& p) {
	if (p == Point()) return;
	switch (dragMode) {
		case LeftTop:     minPt.x = p.x; maxPt.y = p.y; break;
		case LeftBottom:  minPt = p; break;
		case RightTop:    maxPt = p; break;
		case RightBottom: minPt.y = p.y; maxPt.x = p.x; break;
		case TopEdge:     maxPt.y = p.y; break;
		case RightEdge:   maxPt.x = p.x; break;
		case BottomEdge:  minPt.y = p.y;;  break;
		case LeftEdge:    minPt.x = p.x; break;
	}

}
void BoundingBox::deleteEntity(View3dWindow* view, EntityList deleteList) {
	Entity* e = 0;
	for (deleteList.start(); (e = deleteList.next()) != NULL; ) {
		if (entityIds.contains(e->getid()))
			deleteEntityId(e->getid());
	}
	if (entityIds.length() == 0) {
		clear();
		return;
	}
	EntityList temp;
	for (int i = 0; i < entityIds.length(); i++)
		temp.add(db.geometry.getentity(entityIds.at(i)));
	computeBoundingBox(view, temp);
}
void BoundingBox::deleteEntityId(long id) {
	for (int i = 0; i < entityIds.length(); i++) {
		if (entityIds.at(i) == id)
			entityIds.removeAt(i);
	}
}
bool BoundingBox::dragging() const {
	return dragMode != Undefined;
}

bool BoundingBox::ptIsOnEdge(const Point& edgeStart, const Point& edgeEnd, const Point& pt) const
{
	if (qAbs(edgeStart.y - edgeEnd.y) < 1.0E-5) {
		double x = edgeStart.x;
		double y = edgeStart.y - HandleSize / 2;
		return QRect(x, y, qAbs(edgeStart.x - edgeEnd.x), HandleSize).contains(pt.x,pt.y);		
	}
	else if (qAbs(edgeStart.x - edgeEnd.x) < 1.0E-5) {
		double x = edgeEnd.x- HandleSize / 2;
		double y = edgeEnd.y;
		return QRect(x, y, HandleSize,qAbs(edgeStart.y - edgeEnd.y)).contains(pt.x, pt.y);
	}
}

bool BoundingBox::dragLeftTopHandle(View3dSurface* s,const Point& p) const {
	return getLeftTopHandle(s).contains(p.x, p.y);
}
bool BoundingBox::dragRightTopHandle(View3dSurface*s,const Point& p) const {
	return getRightTopHandle(s).contains(p.x, p.y);
}

bool BoundingBox::dragLeftBotHandle(View3dSurface*s, const Point& p) const {
	return getLeftBotHandle(s).contains(p.x, p.y);
}

bool BoundingBox::dragRightBotHandle(View3dSurface*s, const Point& p) const {
	return getRightBotHandle(s).contains(p.x, p.y);
}


QRect BoundingBox::getLeftTopHandle(View3dSurface* s) const {
	Point p = leftTopPt(s);	
	double x = p.x - HandleSize / 2;
	double y = p.y - HandleSize / 2;
	return QRect(x, y, HandleSize, HandleSize);
}
QRect BoundingBox::getLeftBotHandle(View3dSurface* s) const {
	Point p = leftBotPt(s);	
	double x = p.x - HandleSize / 2;
	double y = p.y - HandleSize / 2;
	return QRect(x, y, HandleSize, HandleSize);
}
QRect BoundingBox::getRightTopHandle(View3dSurface* s) const {
	Point p = rightTopPt(s);	
	double x = p.x - HandleSize / 2;
	double y = p.y - HandleSize / 2;
	return QRect(x, y, HandleSize, HandleSize);
}
QRect BoundingBox::getRightBotHandle(View3dSurface* s) const {
	Point p = rightBotPt(s);
	double x = p.x - HandleSize / 2;
	double y = p.y - HandleSize / 2;	
	return QRect(x, y, HandleSize, HandleSize);
}

Point BoundingBox::centerPt() const {
	return (minPt + maxPt) / 2;
}
Point BoundingBox::transformToScreen(View3dSurface* s, double x, double y) const {
	Point handle(x, y, 0);
	handle = s->modeltoview(handle);
	handle = s->viewtoscreen(handle);
	return handle;
}

Point BoundingBox::leftTopPt(View3dSurface* s,bool forCenter) const {
	Point temp(minPt.x, maxPt.y, 0);
	if (forCenter)
		return temp;
	else
		return transformToScreen(s, temp.x, temp.y);
}
Point BoundingBox::leftBotPt(View3dSurface* s, bool forCenter) const {
	Point temp = minPt;
	if (forCenter)
		return temp;
	else
		return transformToScreen(s, temp.x, temp.y);
}
Point BoundingBox::rightTopPt(View3dSurface* s, bool forCenter) const {
	Point temp= maxPt;
	if (forCenter)
		return temp;
	else
		return transformToScreen(s, temp.x, temp.y);
}
Point BoundingBox::rightBotPt(View3dSurface* s, bool forCenter) const {
	Point temp= Point(maxPt.x, minPt.y, minPt.z);
	if (forCenter)
		return temp;
	else
		return transformToScreen(s, temp.x, temp.y);
}

double BoundingBox::width() const {
	return qAbs(maxPt.x - minPt.x);
}

double BoundingBox::height() const {
	return qAbs(maxPt.y - minPt.y);
}
