// implementations of the qtdefs qt to windows functions
// other functions are defined and implemented in qtdefs.h

#include "ncwin.h"
#include "qtdefs.h"
#include <QDir>
#include <QFileInfo>
#ifdef _MAC
#include <QFileOpenEvent>
#include "MacNativeCode.h"
#endif
#include "RCStringResources.h"
#include "RCImageResources.h"
#include "GeneratePoints_Dialog.h"
#include "LineAngle_Dialog.h"
#include "chamfer_dialog.h"
#include "Rectangle_Dialog.h"
#include "Polygon_Dialog.h"
#include "LineParallel_Dialog.h"
#include "PrintPreview_Dialog.h"
#include "weight_dialog.h"
#include "maskchange_dialog.h"
#include "Styleset_Dialog.h"
#include "definestyle_dialog.h"
#include "definecolour_dialog.h"
#include "calculator_dialog.h"
#include "grid_dialog.h"
#include "standard_Dbl_dialog.h"
#include "standard_Int_dialog.h"
#include "standardstring_dialog.h"
#include "CreateWorkplane_Dialog.h"
#include "DeleteWorkplane_Dialog.h"
#include "ResetWorkplane_Dialog.h"
#include "SelectWorkplane_Dialog.h"
#include "rccombolist.h"
#include "ellipseinsert_dialog.h"
#include "curve_dialog.h"
#include "converttocurve_dialog.h"
#include "textinsert_dialog.h"
#include "textinsertcounter_dialog.h"
#include "lttextinsert_dialog.h"
#include "dimensiontext_dialog.h"
#include "lttextinsert_dialog.h"
#include "text_dialog.h"
#include "textverify_dialog.h"
#include "textverifychange_dialog.h"
#include "figuretext_dialog.h"
#include "systemoptions_dialog.h"
#include "bitmap_dialog.h"
#include "trace_dialog.h"
#include "snapmask_dialog.h"
#include "positionalong_dialog.h"
#include "positionoffset_dialog.h"
#include "reference_dialog.h"
#include "trim_dialog.h"
#include "explode_dialog.h"
#include "resetview_dialog.h"
#include "plotscale_dialog.h"
#include "viewclip_dialog.h"
#include "createview_dialog.h"
#include "deleteview_dialog.h"
#include "zone_dialog.h"
#include "deletezone_dialog.h"
#include "selectview_dialog.h"
#include "createzone_dialog.h"
#include "zoomzone_dialog.h"
#include "measurelengtharea_dialog.h"
#include "measurearea_dialog.h"
#include "measureangle_dialog.h"
#include "group_dialog.h"
#include "groupverify_dialog.h"
#include "line_dialog.h"
#include "lineverify_dialog.h"
#include "stretch_dialog.h"
#include "translate_dialog.h"
#include "move_dialog.h"
#include "copyarray_dialog.h"
#include "copyalong_dialog.h"
#include "project_dialog.h"
#include "scale_dialog.h"
#include "mirror_dialog.h"
#include "rotate_dialog.h"
#include "offset_dialog.h"
#include "offsetd_dialog.h"
#include "circleverify_dialog.h"
#include "pointverify_dialog.h"
#include "copybitmap_dialog.h"
#include "ellipseverify_dialog.h"
#include "curveverify_dialog.h"
#include "planeverify_dialog.h"
#include "attributenew_dialog.h"
#include "attributelist_dialog.h"
#include "attributeedit_dialog.h"
#include "attributedelete_dialog.h"
#include "information_dialog.h"
#include "surfaceofrevinsert_dialog.h"
#include "patchcorners_dialog.h"
#include "patchmesh_dialog.h"
#include "patchmove_dialog.h"
#include "patchverify_dialog.h"
#include "patchtriangulate_dialog.h"
#include "shade_dialog.h"
#include "ruledsurfaceverify_dialog.h"
#include "surfaceofrevverify_dialog.h"
#include "ruledsurface_dialog.h"
#include "patch_dialog.h"
#include "surfaceofrev_dialog.h"
#include "figureverify_dialog.h"
#include "figure_dialog.h"
#include "modifyparametricfigure_dialog.h"
#include "missing_dialog.h"
#include "findandreplace_dialog.h"
#include "findinfiles_dialog.h"
#include "insertfiguretext_dialog.h"
#include "selecthatchpattern_dialog.h"
#include "selectdrawing_dialog.h"
#include "composeddrawing_dialog.h"
#include "selectdrawingview_dialog.h"
#include "oneviewchange_dialog.h"
#include "deletedrawing_dialog.h"
#include "renamedrawing_dialog.h"
#include "drawingverify_dialog.h"
#include "copycomposeddrawing_dialog.h"
#include "measure_dialog.h"
#include "definepatternstyle_dialog.h"
#include "leader_dialog.h"
#include "leaderverify_dialog.h"
#include "dimension_dialog.h"
#include "dimensionterminator_dialog.h"
#include "dimensionprefix0_dialog.h"
#include "dimensionprefix_dialog.h"
#include "dimensionvalue_dialog.h"
#include "dimensionsuffix0_dialog.h"
#include "dimensioncterminator_dialog.h"
#include "selectapplication_dialog.h"
#include "selectunloadapplication_dialog.h"
#include "toolbars_dialog.h"
#include "licensekey_dialog.h"
#include "licenseagreement_dialog.h"
#include "license_dialog.h"
#include "v7license_dialog.h"
#include "networksoftwarelicense_dialog.h"
#include "aboutrealcad_dialog.h"
#include "unlicense_dialog.h"
#include "dxfin_dialog.h"
#include "dxfout_dialog.h"
#include "keyboardshortcuts_dialog.h"
#include "material_dialog.h"
#include "lightverify_dialog.h"
#include "sunposition_dialog.h"
#include "surfaceproperty_dialog.h"
#include "bumpmap_dialog.h"
#include "texture_dialog.h"
#include "raytrace_dialog.h"
#include "dwgimportoptions_dialog.h"
//#include "bitmaptransparencydialog.h"
#include "bitmaptransparency_dialog.h"
#include "systempath_dialog.h"

#include "RCGraphicsView.h"
#include "RCGraphicsScene.h"
#include "RCView.h"
#include "RCCentralWidget.h"
#include "rcdoublespinbox.h"
#include "rcintegerspinbox.h"
#include "weightspinbox.h"
#include "_tools.h"

extern void open_skp(RCCHAR *filename);
extern void EnterIdle();
extern void file_open_event(int nocadfiles,RCCHAR *filename);
extern void file_open(int nocadfiles,RCCHAR *fname);
//extern LRESULT CALLBACK MainEventProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
qreal QGMAXZVALUE = QGMINZVALUE;

QString getItemTypeName(QGraphicsItem *gi)
{
    if(dynamic_cast<QGraphicsLineItem *>(gi) != 0)
        return "QGraphicsLineItem";
    else if(dynamic_cast<QGraphicsEllipseItem *>(gi) != 0)
        return "QGraphicsEllipseItem";
    else if(dynamic_cast<QGraphicsPathItem *>(gi) != 0)
        return "QGraphicsPathItem";
    else if(dynamic_cast<QGraphicsPolygonItem *>(gi) != 0)
        return "QGraphicsPolygonItem";
    else if(dynamic_cast<QGraphicsRectItem *>(gi) != 0)
        return "QGraphicsRectItem";
    else if(dynamic_cast<QGraphicsSimpleTextItem *>(gi) != 0)
        return "QGraphicsSimpleTextItem";
    else if(dynamic_cast<QGraphicsTextItem *>(gi) != 0)
        return "QGraphicsTextItem";
    else if(dynamic_cast<QGraphicsPixmapItem *>(gi) != 0)
        return "QGraphicsPixmapItem";
    else if(dynamic_cast<QGraphicsItemGroup *>(gi) != 0)
        return "QGraphicsItemGroup";
    else
        return "QGraphicsItem";
}

mywidget::mywidget()
{
    resize(QSize(100,100));
}
void mywidget::resizeEvent(QResizeEvent *)
{
    //qDebug() << "Entering : mywidget::resizeEvent";
}

bool GlobalEventHandler::eventFilter(QObject *obj, QEvent *event)
{
    //qDebug() << "GEH watched obj : " << obj;
    //qDebug() << "GEH event type  : " << event->type();
#if 0
    if(event->type() == QEvent::Drop)
    {
        if(obj->objectName() == "MainWindow")
        {
            const QMimeData *mimeData = ((QDropEvent*)event)->mimeData();

            qDebug() << "in QtMainWindow::dropEvent";

            // only looking for files at the moment
            if (mimeData->hasUrls())
            {
                QStringList pathList;
                QList<QUrl> urlList = mimeData->urls();
                if(urlList.isEmpty())
                    return true;

                QString filepath = urlList.first().toLocalFile();
                if(filepath.isEmpty())
                    return true;

                if(filepath.toLower().right(4) == ".cad")
                {
                    QFileOpenEvent *openEvent = new QFileOpenEvent(filepath);
                    app->postEvent(app,openEvent);
                }
            }
            return true;
        }
        else
            return false;
    }
#endif
    // standard event processing
    return QObject::eventFilter(obj, event);
}

RCApplication::RCApplication(int &argc, char ** argv)
    : QApplication(argc,argv)
{
    mSketchUpBrowser = 0;
    mIdleTimer = 0;
    mbtn = Qt::NoButton;
    mlastkey = 0;
    mlastWebPageReply=0;
    mdialogFinishedFlag = RCDIALOG_NOT_FINISHED;
    mFlashTimer=0;
    mFlashEntity=0;
    eventLoopStarted=false;
    //qApp->setAttribute(Qt::AA_UseHighDpiPixmaps);
	registerDialogType();
}

void RCApplication::registerDialogType() {
	qRegisterMetaType<Styleset_Dialog*>();
	qRegisterMetaType<Rectangle_Dialog*>();
	qRegisterMetaType<GeneratePoints_Dialog*>();
	qRegisterMetaType<LineAngle_Dialog*>();
	qRegisterMetaType<Chamfer_Dialog*>();
	qRegisterMetaType<Polygon_Dialog*>();
	qRegisterMetaType<LineParallel_Dialog*>();
	qRegisterMetaType<PrintPreview_Dialog*>();
	qRegisterMetaType<Weight_Dialog*>();
	qRegisterMetaType<MaskChange_Dialog*>();	
	qRegisterMetaType<LayerGroup_Dialog*>();
	qRegisterMetaType<DefineColour_Dialog*>();
	qRegisterMetaType<Calculator_Dialog*>();
	qRegisterMetaType<Grid_Dialog*>();
	qRegisterMetaType<Standard_Dbl_Dialog*>();
	qRegisterMetaType<Standard_Int_Dialog*>();
	qRegisterMetaType<StandardString_Dialog*>();
	qRegisterMetaType<SelectWorkplane_Dialog*>();
	qRegisterMetaType<ResetWorkplane_Dialog*>();
	qRegisterMetaType<DeleteWorkplane_Dialog*>();
	qRegisterMetaType<CreateWorkplane_Dialog*>();
	qRegisterMetaType<EllipseInsert_Dialog*>();
	qRegisterMetaType<Curve_Dialog*>();
	qRegisterMetaType<ConvertToCurve_Dialog*>();
	qRegisterMetaType<TextInsert_Dialog*>();
	qRegisterMetaType<TextInsertCounter_Dialog*>();
	qRegisterMetaType<DimensionText_Dialog*>();
	qRegisterMetaType<LTTextInsert_Dialog*>();
	qRegisterMetaType<Text_Dialog*>();
	qRegisterMetaType<TextVerify_Dialog*>();
	qRegisterMetaType<TextVerifyChange_Dialog*>();
	qRegisterMetaType<FigureText_Dialog*>();
	qRegisterMetaType<SystemOptions_Dialog*>();
	qRegisterMetaType<Bitmap_Dialog*>();
	qRegisterMetaType<Trace_Dialog*>();
	qRegisterMetaType<SnapMask_Dialog*>();
	qRegisterMetaType<PositionAlong_Dialog*>();
	qRegisterMetaType<PositionOffset_Dialog*>();
	qRegisterMetaType<Reference_Dialog*>();
	qRegisterMetaType<Trim_Dialog*>();
	qRegisterMetaType<Explode_Dialog*>();
	qRegisterMetaType<ResetView_Dialog*>();
	qRegisterMetaType<PlotScale_Dialog*>();
	qRegisterMetaType<ViewClip_Dialog*>();
	qRegisterMetaType<CreateView_Dialog*>();
	qRegisterMetaType<DeleteView_Dialog*>();
	qRegisterMetaType<Zone_Dialog*>();
	qRegisterMetaType<DeleteZone_Dialog*>();
	qRegisterMetaType<SelectView_Dialog*>();
	qRegisterMetaType<CreateZone_Dialog*>();
	qRegisterMetaType<ZoomZone_Dialog*>();
	qRegisterMetaType<MeasureLengthArea_Dialog*>();
	qRegisterMetaType<MeasureArea_Dialog*>();
	qRegisterMetaType<MeasureAngle_Dialog*>();
	qRegisterMetaType<Group_Dialog*>();
	qRegisterMetaType<GroupVerify_Dialog*>();
	qRegisterMetaType<Line_Dialog*>();
	qRegisterMetaType<LineVerify_Dialog*>();
	qRegisterMetaType<Stretch_Dialog*>();
	qRegisterMetaType<Translate_Dialog*>();
	qRegisterMetaType<Move_Dialog*>();
	qRegisterMetaType<CopyArray_Dialog*>();
	qRegisterMetaType<CopyAlong_Dialog*>();
	qRegisterMetaType<Project_Dialog*>();
	qRegisterMetaType<Scale_Dialog*>();
	qRegisterMetaType<Mirror_Dialog*>();
	qRegisterMetaType<Rotate_Dialog*>();
	qRegisterMetaType<Offset_Dialog*>();
	qRegisterMetaType<Offsetd_Dialog*>();
	qRegisterMetaType<CircleVerify_Dialog*>();
	qRegisterMetaType<PointVerify_Dialog*>();
	qRegisterMetaType<CopyBitmap_Dialog*>();
	qRegisterMetaType<EllipseVerify_Dialog*>();
	qRegisterMetaType<CurveVerify_Dialog*>();
	qRegisterMetaType<PlaneVerify_Dialog*>();
	qRegisterMetaType<AttributeNew_Dialog*>();
	qRegisterMetaType<AttributeList_Dialog*>();
	qRegisterMetaType<AttributeEdit_Dialog*>();
	qRegisterMetaType<AttributeDelete_Dialog*>();
	qRegisterMetaType<Information_Dialog*>();
	qRegisterMetaType<SurfaceOfRevInsert_Dialog*>();
	//Patch
	qRegisterMetaType<Patch_Dialog*>();
	qRegisterMetaType<PatchCorners_Dialog*>();
	qRegisterMetaType<PatchMesh_Dialog*>();
	qRegisterMetaType<PatchMove_Dialog*>();
	qRegisterMetaType<PatchVerify_Dialog*>();
	qRegisterMetaType<PatchTriangulate_Dialog*>();
	qRegisterMetaType<Shade_Dialog*>();
	qRegisterMetaType<RuledSurfaceVerify_Dialog*>();
	qRegisterMetaType<SurfaceOfRevVerify_Dialog*>();
	qRegisterMetaType<RuledSurface_Dialog*>();	
	qRegisterMetaType<SurfaceOfRev_Dialog*>();
	qRegisterMetaType<FigureVerify_Dialog*>();
	qRegisterMetaType<Figure_Dialog*>();
	qRegisterMetaType<ModifyParametricFigure_Dialog*>();
	qRegisterMetaType<Missing_Dialog*>();
	qRegisterMetaType<FindAndReplace_Dialog*>();
	qRegisterMetaType<FindInFiles_Dialog*>();
	qRegisterMetaType<InsertFigureText_Dialog*>();
	qRegisterMetaType<SelectHatchPattern_Dialog*>();
	qRegisterMetaType<SelectDrawing_Dialog*>();
	qRegisterMetaType<ComposedDrawing_Dialog*>();
	qRegisterMetaType<SelectDrawingView_Dialog*>();
	qRegisterMetaType<OneViewChange_Dialog*>();
	qRegisterMetaType<DeleteDrawing_Dialog*>();
	qRegisterMetaType<RenameDrawing_Dialog*>();
	qRegisterMetaType<DrawingVerify_Dialog*>();
	qRegisterMetaType<CopyComposedDrawing_Dialog*>();
	qRegisterMetaType<Measure_Dialog*>();
	qRegisterMetaType<Leader_Dialog*>();
	qRegisterMetaType<LeaderVerify_Dialog*>();
	qRegisterMetaType<DefinePatternStyle_Dialog*>();
	qRegisterMetaType<Dimension_Dialog*>();
	qRegisterMetaType<DimensionTerminator_Dialog*>();
	qRegisterMetaType<DimensionPrefix0_Dialog*>();
	qRegisterMetaType<DimensionPrefix_Dialog*>();
	qRegisterMetaType<DimensionValue_Dialog*>();
	qRegisterMetaType<DimensionSuffix0_Dialog*>();
	qRegisterMetaType<DimensionCTerminator_Dialog*>();
	qRegisterMetaType<SelectApplication_Dialog*>();
	qRegisterMetaType<SelectUnloadApplication_Dialog*>();
	qRegisterMetaType<ToolBars_Dialog*>();
	qRegisterMetaType<LicenseKey_Dialog*>();
	qRegisterMetaType<LicenseAgreement_Dialog*>();
	qRegisterMetaType<License_Dialog*>();
	qRegisterMetaType<V7License_Dialog*>();
	qRegisterMetaType<NetworkSoftwareLicense_Dialog*>();
	qRegisterMetaType<AboutRealCAD_Dialog*>();
	qRegisterMetaType<Unlicense_Dialog*>();
	qRegisterMetaType<DXFIn_Dialog*>();
	qRegisterMetaType<DXFOut_Dialog*>();
	qRegisterMetaType<KeyboardShortcuts_Dialog*>();
	qRegisterMetaType<Material_Dialog*>();
	qRegisterMetaType<LightVerify_Dialog*>();
	qRegisterMetaType<SunPosition_Dialog*>();
	qRegisterMetaType<SurfaceProperty_Dialog*>();
	qRegisterMetaType<Bumpmap_Dialog*>();
	qRegisterMetaType<Texture_Dialog*>();
	qRegisterMetaType<Raytrace_Dialog*>();
	qRegisterMetaType<DWGImportOptions_Dialog*>();
	qRegisterMetaType<BitmapTransparency_Dialog*>();
	qRegisterMetaType<SystemPath_Dialog*>();
	
}
StateMachine *RCApplication::getStateMachine()
{
    return &state;
}

void RCApplication::checkautosavetimer()
{
    program->checkautosavetimer();
}

void RCApplication::webPageReplyFinished(QNetworkReply *reply)
{
    mlastWebPageReply = reply;

}

void RCApplication::dialogFinished(int result)
{
    mdialogFinishedFlag = result;
}

QString RCApplication::getStyleSheetText(QString stylePath)
{
    qDebug() << "style sheet path: " << stylePath;
    QFile file(stylePath);
    file.open(QFile::ReadOnly);
    return QString::fromLatin1(file.readAll());
}

void RCApplication::onReady3DWarehouseOutput()
{
    qInfo() << "onReady3DWarehouseOutput";
    if(mSketchUpBrowser)
    {
        QByteArray data = mSketchUpBrowser->readAllStandardOutput();
        QList<QByteArray> datalist;
        datalist = data.split('\n');
        for(int i=0; i < datalist.size(); i++)
        {
            if(datalist.at(i).startsWith("RCDownloadFile:"))
            {
                QString filePath = datalist.at(i).mid(15);
                qInfo() << "filePath" << filePath;
                open_skp(filePath.data());
                ((QtMainWindow*)cadwindow->gethwnd())->raise();
                return;
            }
        }
    }
}

void RCApplication::onFinished3DWarehouse(int, QProcess::ExitStatus)
{
    qDebug() << "Entering onFinshed3DWarehouse";
    //QMessageBox::information(0,"onFinished3DWarehouse","3D Warehouse was closed");
    if(mSketchUpBrowser)
    {
        //QMessageBox::information(0,"onFinished3DWarehouse","deleting Warehouse object");
        mSketchUpBrowser->deleteLater();
        mSketchUpBrowser = 0;
    }
}

RCApplication::~RCApplication()
{
    qDebug() << "Entering RCApplication() destructor";
}

void RCApplication::cleanUpOnQuit()
{
    qDebug() << "Entering RCApplication() cleanUpOnQuit";
}

void RCApplication::onIdle()
{
    // do idle processing here
    // we are checking for dialog auto ok mouse clicks
    EnterIdle();
}

/*
int RCApplication::exec()
{
    //qDebug() << "Entering : RCApplication::exec";
    return QApplication::exec();
}
*/

bool RCApplication::event(QEvent * event)
{
    return QApplication::event(event);
}

bool RCApplication::notify(QObject * receiver, QEvent * e)
{
    //qDebug() << "Entering : RCApplication::notify";
    // KMJ : TODO : the question here is whether to clear the Qt events before
    // calling the state.process() or the program->processmessages(...) functions
    // and if it is best to just call stat.proces() or use program->processmessages(...)
#if 1
    // method 4
    // convert Qt events to windows events and messages
    // then call the MainEventProc function to emulate what windows does
    // call the TransparentEventProc in certain cases
    //
    // MainEventProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
    // TransparentEventProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)

    // convert the different event types to Windows messages

    HighLevelEvent *he;
    HWND hWnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    bool status=false;
    bool debugEvent = false;
    //mbtn = Qt::NoButton;

    //program->resetinterrupt();

    if( debugEvent )
    {
        //qDebug() << "App Event reciever  : " << receiver;
        //qDebug() << "App Event event : " << e;
        //if(e->type() == QEvent::MouseButtonPress ||
        //   e->type() == QEvent::MouseButtonRelease )
        //    qDebug() << "App Event was a mouse button event";
    }

    //status = QApplication::notify(receiver,e);
    /*
    if ((he = commandfile.getnextevent()) == 0)
    {
         program->processmessages(1);
    }
    else
    {
         state.sendevent(he);
         state.process();
         while ((status = program->processmessages(2)) == 1) ;
    }
    */

    //if(receiver->objectName().contains("_X_CEdit"))
    //    return QApplication::notify(receiver,e);
    //QApplication::notify(receiver,e);

    if(receiver->isWidgetType())
    {
        //if(receiver->inherits("QDialog"))
        //    qDebug() << "receiver is a dialog";

        // mouse buttons press event
        if( e->type() == QEvent::MouseButtonPress )
        {
            //QWidget *qw = app->widgetAt(((QMouseEvent*)e)->globalPos());
            //qDebug() << "widget : " << qw << " found at " << ((QMouseEvent*)e)->globalPos();

            //qDebug() << "Mouse Button Down Event";
            //qDebug() << "App Event reciever (QObject*)receiver : " << (QObject*)receiver;
            //qDebug() << "App Event reciever (QWidget*)receiver : " << (QWidget*)receiver;
            hWnd = (HWND)receiver;
            // make the wParam
            wParam = 0;
            if(((QMouseEvent*)e)->modifiers() & Qt::ShiftModifier )
                wParam |= MK_SHIFT;
            if(((QMouseEvent*)e)->modifiers() & Qt::ControlModifier )
                wParam |= MK_CONTROL;

            // set the Windows message type
            switch(((QMouseEvent*)e)->button())
            {
            case Qt::LeftButton :
                    message = WM_LBUTTONDOWN;
                    wParam |= MK_LBUTTON;
                    mbtn = Qt::LeftButton;
                    break;
            case Qt::RightButton :
                    message = WM_RBUTTONDOWN;
                    wParam |= MK_RBUTTON;
                    mbtn = Qt::RightButton;
                    break;
            case Qt::MidButton :
                    message = WM_MBUTTONDOWN;
                    wParam |= MK_MBUTTON;
                    mbtn = Qt::MidButton;
                    break;
            }
            // make the lParam
            mlastMousPos = ((QMouseEvent*)e)->pos();
            int px = ((QMouseEvent*)e)->pos().x();
            int py = ((QMouseEvent*)e)->pos().y();
            lParam = MAKELPARAM((WORD)px, (WORD)py);

            // dont send right button messages when a modal dialog is up
            // NOTE: there may be a better way to do this
            if(!(((QMouseEvent*)e)->button() && app->activeModalWidget()))
                status = MainEventProc( hWnd, message, wParam, lParam );

        }
        else
        if( e->type() == QEvent::MouseButtonDblClick )
        {
            //qDebug() << "Mouse Button Down Event";
            //qDebug() << "App Event reciever (QObject*)receiver : " << (QObject*)receiver;
            //qDebug() << "App Event reciever (QWidget*)receiver : " << (QWidget*)receiver;
            hWnd = (HWND)receiver;
            // make the wParam
            wParam = 0;
            if(((QMouseEvent*)e)->modifiers() & Qt::ShiftModifier )
                wParam |= MK_SHIFT;
            if(((QMouseEvent*)e)->modifiers() & Qt::ControlModifier )
                wParam |= MK_CONTROL;

            // set the Windows message type
            switch(((QMouseEvent*)e)->button())
            {
            case Qt::LeftButton :
                    message = WM_LBUTTONDBLCLK;
                    wParam |= MK_LBUTTON;
                    mbtn = Qt::LeftButton;
                    break;
            case Qt::RightButton :
                    message = WM_RBUTTONDBLCLK;
                    wParam |= MK_RBUTTON;
                    mbtn = Qt::RightButton;
                    //program->setinterrupt(true);
                    break;
            case Qt::MidButton :
                    message = WM_MBUTTONDBLCLK;
                    wParam |= MK_MBUTTON;
                    mbtn = Qt::MidButton;
                    break;
            }
            // make the lParam
            mlastMousPos = ((QMouseEvent*)e)->pos();
            int px = ((QMouseEvent*)e)->pos().x();
            int py = ((QMouseEvent*)e)->pos().y();
            lParam = MAKELPARAM((WORD)px, (WORD)py);

            status = MainEventProc( hWnd, message, wParam, lParam );

        }
        else
        if( e->type() == QEvent::MouseButtonRelease )
        {
            //qDebug() << "Mouse Button Up Event";
            //qDebug() << "App Event reciever (QObject*)receiver : " << (QObject*)receiver;
            //qDebug() << "App Event reciever (QWidget*)receiver : " << (QWidget*)receiver;
            hWnd = (HWND)receiver;
            // make the wParam
            wParam = 0;
            if(((QMouseEvent*)e)->modifiers() & Qt::ShiftModifier )
                wParam |= MK_SHIFT;
            if(((QMouseEvent*)e)->modifiers() & Qt::ControlModifier )
                wParam |= MK_CONTROL;

            // set the Windows message type
            Qt::MouseButtons mbutts = ((QMouseEvent*)e)->buttons();

            if(mbutts.testFlag(Qt::LeftButton))
                wParam != MK_LBUTTON;
            if(mbutts.testFlag(Qt::MidButton))
                wParam != MK_MBUTTON;
            if(mbutts.testFlag(Qt::RightButton))
                wParam != MK_RBUTTON;

            switch(((QMouseEvent*)e)->button())
            {
            case Qt::LeftButton :
                    message = WM_LBUTTONUP;
                    //wParam |= MK_LBUTTON;
                    mbtn = Qt::LeftButton;
                    break;
            case Qt::RightButton :
                    message = WM_RBUTTONUP;
                    //wParam |= MK_RBUTTON;
                    mbtn = Qt::RightButton;
                    break;
            case Qt::MidButton :
                    message = WM_MBUTTONUP;
                    //wParam |= MK_MBUTTON;
                    mbtn = Qt::MidButton;
                    break;
            }
            // make the lParam
            mlastMousPos = ((QMouseEvent*)e)->pos();
            int px = ((QMouseEvent*)e)->pos().x();
            int py = ((QMouseEvent*)e)->pos().y();
            lParam = MAKELPARAM((WORD)px, (WORD)py);

            status = MainEventProc( hWnd, message, wParam, lParam );
            // we want left mouse up to be proccessed first
            //state.process();

        }
        else
        if( e->type() == QEvent::MouseMove )
        {
            //qDebug() << "Mouse Move Event";
            //qDebug() << "App Event reciever (QObject*)receiver : " << (QObject*)receiver;
            //qDebug() << "App Event reciever (QWidget*)receiver : " << (QWidget*)receiver;
            hWnd = (HWND)receiver;

            // make the wParam
            wParam = 0;
            if(((QMouseEvent*)e)->modifiers() & Qt::ShiftModifier )
                wParam |= MK_SHIFT;
            if(((QMouseEvent*)e)->modifiers() & Qt::ControlModifier )
                wParam |= MK_CONTROL;
            if(((QMouseEvent*)e)->buttons() & Qt::LeftButton)
            {
                wParam |= MK_LBUTTON;
                mbtn = Qt::LeftButton;
            }
            if(((QMouseEvent*)e)->buttons() & Qt::MidButton)
            {
                wParam |= MK_MBUTTON;
                mbtn = Qt::MidButton;
            }
            if(((QMouseEvent*)e)->buttons() & Qt::RightButton)
            {
                wParam |= MK_RBUTTON;
                mbtn = Qt::RightButton;
            }
            // set the Windows message type
            message = WM_MOUSEMOVE;

            // the mbtn member has the last button pressed
            /*
            switch(mbtn)
            {
            case Qt::LeftButton :
                    wParam |= MK_LBUTTON;
                    break;
            case Qt::RightButton :
                    wParam |= MK_RBUTTON;
                    break;
            case Qt::MidButton :
                    wParam |= MK_MBUTTON;
                    break;
            }
            */
            // make the lParam
            mlastMousPos = ((QMouseEvent*)e)->pos();
            //WORD px = ((QMouseEvent*)e)->localPos().x();
            //WORD py = ((QMouseEvent*)e)->localPos().y();
            WORD px = ((QMouseEvent*)e)->pos().x();
            WORD py = ((QMouseEvent*)e)->pos().y();
            //qDebug() << "mouse move x : " << px;
            //qDebug() << "mouse move y : " << py;
            lParam = MAKELPARAM((WORD)px, (WORD)py);

            if ((((QObject*)hWnd)->objectName().contains("centerWidget"))
                    && ((((QWidget*)hWnd)->childAt(((QMouseEvent*)e)->pos().x(),((QMouseEvent*)e)->pos().y())))
                    && ((((QWidget*)hWnd)->childAt(((QMouseEvent*)e)->pos().x(),((QMouseEvent*)e)->pos().y()))->objectName().contains("QGLWidget"))) {
                //pass or rubber bands (and other 'preview' stuff) will be erased
              }
            else if ((((QObject*)hWnd)->objectName().contains("MainWindow"))
                     && ((((QWidget*)hWnd)->childAt(((QMouseEvent*)e)->pos().x(),((QMouseEvent*)e)->pos().y())))
                     && ((((QWidget*)hWnd)->childAt(((QMouseEvent*)e)->pos().x(),((QMouseEvent*)e)->pos().y()))->objectName().contains("QGLWidget"))) {
                //pass or rubber bands (and other 'preview' stuff) will be erased
            }
            else {
            status = MainEventProc( hWnd, message, wParam, lParam );
            }


        }
        else
        if( e->type() == QEvent::Wheel )
        {
            /*
            // * example of using the Qt wheel delta
            QPoint numPixels = envent->pixelDelta();
            QPoint numDegrees = envent->angleDelta() / 8;

            if (!numPixels.isNull()) {
                scrollWithPixels(numpixels);
            } else if (!numDegrees.isNull()) {
                QPoint numSteps = numDegrees / 15;
                scrollWithDegrees(numSteps);
            }
            */

            //qDebug() << "Mouse Wheel Event";
            //qDebug() << "App Event reciever (QObject*)receiver : " << (QObject*)receiver;
            //qDebug() << "App Event reciever (QWidget*)receiver : " << (QWidget*)receiver;

            hWnd = (HWND)(QWidget*)receiver;

            // make the wParam
            wParam = 0;
            WORD lowWord = 0, highWord = 0;
            // The high-order word indicates the distance the wheel is rotated
            //QPoint numPixels  = ((QWheelEvent*)e)->pixelDelta();
            QPoint numDegrees = ((QWheelEvent*)e)->angleDelta() / 8;
            QPoint angleDelta = ((QWheelEvent*)e)->angleDelta();
            if(!numDegrees.isNull())
                angleDelta = numDegrees / 15;

            // KMJ: NOTE : angleDelta returns the angle in x on Windows if the Alt key is down and y otherwise or on Mac
            int delta = angleDelta.x()==0 ? angleDelta.y() : angleDelta.x();
            //if(abs(delta) != 120)
            //    return false;

            highWord = angleDelta.x()==0 ? angleDelta.y() : angleDelta.x();

            //qDebug() << "In Wheel Event : angleDelta() : " << angleDelta.x();
            //qDebug() << "In Wheel Event : angleDelta.y() : " << angleDelta.y();
            //qDebug() << "In Wheel Event : highWord       : " << highWord;

            // The low-order word indicates whether various virtual keys are down.
            if(((QWheelEvent*)e)->modifiers() & Qt::ShiftModifier )
                lowWord |= MK_SHIFT;
            if(((QWheelEvent*)e)->modifiers() & Qt::ControlModifier )
                lowWord |= MK_CONTROL;
            if(((QWheelEvent*)e)->buttons() & Qt::LeftButton)
            {
                lowWord |= MK_LBUTTON;
                mbtn = Qt::LeftButton;
            }
            if(((QWheelEvent*)e)->buttons() & Qt::MidButton)
            {
                lowWord |= MK_MBUTTON;
                mbtn = Qt::MidButton;
            }
            if(((QWheelEvent*)e)->buttons() & Qt::RightButton)
            {
                lowWord |= MK_RBUTTON;
                mbtn = Qt::RightButton;
            }
            wParam = MAKEWPARAM((WORD)lowWord, (WORD)highWord);

            // set the Windows message type
            message = WM_MOUSEWHEEL;

            // the mbtn member has the last button pressed
            /*
            switch(mbtn)
            {
            case Qt::LeftButton :
                    wParam |= MK_LBUTTON;
                    break;
            case Qt::RightButton :
                    wParam |= MK_RBUTTON;
                    break;
            case Qt::MidButton :
                    wParam |= MK_MBUTTON;
                    break;
            }
            */
            // make the lParam
            mlastMousPos = ((QMouseEvent*)e)->pos();
            //WORD px = ((QMouseEvent*)e)->localPos().x();
            //WORD py = ((QMouseEvent*)e)->localPos().y();
            WORD px = ((QMouseEvent*)e)->pos().x();
            WORD py = ((QMouseEvent*)e)->pos().y();
            //WORD px = ((QMouseEvent*)e)->globalPos().x();
            //WORD py = ((QMouseEvent*)e)->globalPos().y();
            //QPoint gp = ((QWidget*)receiver)->mapFromGlobal(((QWheelEvent*)e)->globalPos());
            //WORD px = gp.x();
            //WORD py = gp.y();
            //qDebug() << "notify mouse pos x : " << px;
            //qDebug() << "notify mouse pos y : " << py;
            lParam = MAKELPARAM((WORD)px, (WORD)py);

            status = MainEventProc( hWnd, message, wParam, lParam );
        }
        else
        if( e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease )
        {
            hWnd = (HWND)(QWidget*)receiver;
            // make the wParam
            // keys used by REALCAD are here
            // all other keys use the QT codes
            wParam = 0;
            switch(((QKeyEvent*)e)->key())
            {
            case Qt::Key_Escape :
                wParam = VK_ESCAPE;
                break;
            case Qt::Key_Shift :
                wParam = VK_SHIFT;
                break;
            case Qt::Key_Control :
                wParam = VK_CONTROL;
                break;
            case Qt::Key_Tab :
                wParam = VK_TAB;
                break;
            case Qt::Key_Return :
            case Qt::Key_Enter :
                wParam = VK_RETURN;
                break;
            case Qt::Key_F1 :
                wParam = VK_F1;
                break;
            case Qt::Key_F2 :
                wParam = VK_F2;
                wParam = '!';
                break;
            case Qt::Key_F3 :
                wParam = VK_F3;
                break;
            case Qt::Key_F4 :
                wParam = VK_F4;
                break;
            case Qt::Key_F5 :
                wParam = VK_F5;
                break;
            case Qt::Key_F6 :
                wParam = VK_F6;
                break;
            case Qt::Key_F7 :
                wParam = VK_F7;
                break;
            case Qt::Key_F8 :
                wParam = VK_F8;
                break;
            case Qt::Key_F9 :
                wParam = VK_F9;
                break;
            case Qt::Key_F11 :
                wParam = VK_F11;
                break;
            case Qt::Key_End :
                wParam = VK_END;
                break;
            case Qt::Key_PageUp :
                wParam = VK_PRIOR;
                break;
            case Qt::Key_PageDown :
                wParam = VK_NEXT;
                break;
            case Qt::Key_Up :
                wParam = VK_UP;
                break;
            case Qt::Key_Down :
                wParam = VK_DOWN;
                break;
            case Qt::Key_Left :
                wParam = VK_LEFT;
                break;
            case Qt::Key_Right :
                wParam = VK_RIGHT;
                break;
            case Qt::Key_Back :
                wParam = VK_BACK;
                break;
            case Qt::Key_Insert :
                wParam = VK_INSERT;
                break;
            case Qt::Key_Delete :
                wParam = VK_DELETE;
                break;
            case Qt::Key_Backspace :
                wParam = VK_BACK;
                break;
            case Qt::Key_Menu :
            case Qt::Key_Alt :
                wParam = VK_MENU;
                break;
            case Qt::Key_Home :
                wParam = VK_HOME;
                break;
            case Qt::Key_Space :
                wParam = 0x20;//VK_SPACE
                program->setinterrupt(true);
                break;
            default:
                wParam = (WORD)((QKeyEvent*)e)->key();
                break;
            }

            // make the lParam
            lParam = 0;
            // these are the windows key flags
            // bit 0-15 = count
            lParam = ((QKeyEvent*)e)->count() & 0xFFFF ;
            // bit 16-23 = scan code (not using this)
            // bit 24 = exteded key flag (not using this)
            // bit 25-28 = reserved not used
            // bit 29 = context code always 0
            // bit 30 = previous key state. The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
            lParam |= (((QKeyEvent*)e)->isAutoRepeat() << 30);
            // 31 = transition state always 0

            // set the Windows message type
            if(e->type() == QEvent::KeyPress)
                message = WM_KEYDOWN;
            else if(e->type() == QEvent::KeyRelease)
                message = WM_KEYUP;

            status = MainEventProc( hWnd, message, wParam, lParam );

            if( e->type() == QEvent::KeyPress || wParam == VK_ESCAPE)
            {
                mlastkey = ((QKeyEvent*)e)->key();
                mlastmodifiers = QGuiApplication::keyboardModifiers();

                // make the lParam
                lParam = 0;
                // these are the windows key flags
                // bit 0-15 = count
                lParam = ((QKeyEvent*)e)->count() & 0xFFFF ;
                // bit 16-23 = scan code (not using this)
                // bit 24 = exteded key flag (not using this)
                // bit 25-28 = reserved not used
                // bit 29 = context code always 0
                if( (((QKeyEvent*)e)->modifiers() & Qt::AltModifier) == Qt::AltModifier )
                    lParam |= 1 << 29;
                // bit 30 = previous key state. The value is 1 if the key is down before the message is sent, or it is zero if the key is up.
                lParam |= (((QKeyEvent*)e)->isAutoRepeat() << 30);
                // 31 = transition state always 0
                if(e->type() == QEvent::KeyRelease)
                    lParam |= 1 << 30;


                 message = WM_CHAR;
                 // Windows get the key press and the key release events when the Escape key is pressed
                 // we only want the escape to abort one command step at a time so we only look at the release event
                 // this is the same as the Mac
                 //if(e->type() == QEvent::KeyPress && wParam != VK_ESCAPE ||
                 //   (e->type() == QEvent::KeyRelease && wParam == VK_ESCAPE))
                 if((e->type() == QEvent::KeyRelease && wParam == VK_ESCAPE))
                 {
                    status = MainEventProc( hWnd, message, wParam, lParam );
                    return true;
                 }
                 else if(e->type() == QEvent::KeyPress && wParam != VK_ESCAPE)
                     status = MainEventProc( hWnd, message, wParam, lParam );
             }
             else if( e->type() == QEvent::KeyRelease && wParam == VK_ESCAPE)
             {
                 //if(e->type() == QEvent::KeyRelease && wParam == VK_ESCAPE)
                 status = MainEventProc( hWnd, message, wParam, lParam );
                 mlastkey = 0;
                 mlastmodifiers = 0;
             }
        }
        else if( e->type() == QEvent::Scroll)
        {
            //qDebug() << "scroll event";
            //message = WM_TIMER;
            //wParam = 0;
            //lParam = 0;
            //status = MainEventProc( hWnd, message, wParam, lParam );
        }
        else if( e->type() == QEvent::Timer )
        {
            //qDebug() << "timer event";
            message = WM_TIMER;
            wParam = 0;
            lParam = 0;
            status = MainEventProc( hWnd, message, wParam, lParam );
        }

     }
     else if( e->type() == QEvent::FileOpen)
     {
        // KMJ: this is used on the mac to open files when the app is closed or running
        // it is used on win and mac when dropping files on the main window

        qDebug() << "QEvent::FileOpen";

        file_open_event(0,((QFileOpenEvent*)e)->file().data());
        // always return true
        return true;
     }

    // filter out the tab char for the Entry LineEdit
    // these have already been handled in MainEventProc see the code above

    if(message == WM_CHAR && wParam == VK_TAB && receiver->objectName().contains("_CEdit"))
        ;
    else if(! status)
    {
        if(e->type() == QEvent::Drop)
        {
            qDebug() << "DROP-DROP-DROP";
            qDebug() << "event type : " << e->type();
            qDebug() << "event receiver : " << receiver;
            qDebug() << "event receiver name : " << receiver->objectName();
            qDebug() << "DROP-DROP-DROP";

            if(receiver->objectName() == "MainWindow")
            {
                const QMimeData *mimeData = ((QDropEvent*)e)->mimeData();

                qDebug() << "in QtMainWindow::dropEvent";

                // only looking for files at the moment
                if (mimeData->hasUrls())
                {
                    QStringList pathList;
                    QList<QUrl> urlList = mimeData->urls();
                    if(urlList.isEmpty())
                        return true;

                    QString filepath = urlList.first().toLocalFile();
                    if(filepath.isEmpty())
                        return true;

                    if(filepath.toLower().right(4) == ".cad" ||
                       filepath.toLower().right(4) == ".dwg" ||
                       filepath.toLower().right(4) == ".dxf" )
                    {
                        QFileOpenEvent *openEvent = new QFileOpenEvent(filepath);
                        app->postEvent(app,openEvent);
                        // set status to true for completeness
                        status = true;
                    }
                }
            }
        }
        // this will call the mainwindow drop event and accept the file drop
        // other types of drag and drop on the window taken care of in that function
        // any other widget drop events will be handled by this line too
        status = QApplication::notify(receiver,e);
    }

     // filter out any events we don't want to upset the state machine events
     // this should best be done in the appropriate widget function!!

     if( ! status )
     {
         switch(e->type())
         {
         case QEvent::CursorChange:
             return status;
         }
     }

     //if(state.getcommandactive() && state.getcommandid() >= 31000 &&
     //   program->pluginmessboxup)
     //    return status;

     if ((he = commandfile.getnextevent()) == 0)
     {
         //qDebug() << "status : " << status;
         //qDebug() << "getcommandid : " << state.getcommandid();
         //qDebug() << "getevent : " << state.getevent();
         //qDebug() << "eventavailable : " << state.eventavailable();

         if( ! status )
            status = program->processmessages(1);
     }
     else
     {
          state.sendevent(he);
          state.process();
          while ((status = program->processmessages(2)) == 1) ;
     }

     // this was a possibility for the auto ok feature
     // but rather difficult to do
     //onIdle();

    return status;
#endif
}

RCEventLoop::RCEventLoop(QObject * parent)
    : QEventLoop(parent)
{
}

const QMetaObject * RCEventLoop::metaObject()
{
    return metaObject();
}

int RCEventLoop::exec(ProcessEventsFlags flags)
{
    state.process();
    return 0;
}

void RCMdiArea::removeSubWindow(QWidget * widget)
{
    RCMdiArea::removeSubWindow(widget);
    tileSubWindows();
}

void RCMdiArea::tileSubWindows()
{
    mtiledMode = QtTiling;
    QMdiArea::tileSubWindows();
}


/**
 * @brief RCMdiArea::tileVertically
 *
 * impementation of RCMdiArea
 *
 */
void RCMdiArea::tileVertically()
{

    tileSubWindows();
    QList<QMdiSubWindow *> windows = subWindowList();
    //if(windows.size() < 2)
    //{
    //    tileSubWindows();
    //    return;
    //}
    int wHeight = height() / windows.size();
    int y = 0;
    for(int i=0; i < windows.size(); i++)
    {
        windows.at(i)->resize(width(),wHeight);
        windows.at(i)->move(0,y);
        y += wHeight;
    }
    mtiledMode = verticalTiling;
}

/**
 * @brief RCMdiArea::tileHorizontally
 */
void RCMdiArea::tileHorizontally()
{
    tileSubWindows();
    QList<QMdiSubWindow *> windows = subWindowList();
    //if(windows.size() < 2)
    //{
    //    tileSubWindows();
    //    return;
    //}
    int wWidth = width() / windows.size();
    int x = 0;
    for(int i=0; i < windows.size(); i++)
    {
        windows.at(i)->resize(wWidth,height());
        windows.at(i)->move(x,0);
        x += wWidth;
    }
    mtiledMode = horizontalTiling;
}

/**
 * @brief RCMdiSubWindow::closeEvent
 * @param closeEvent
 */
void RCMdiSubWindow::closeEvent(QCloseEvent * closeEvent)
{
    qDebug() << "entering RCMdiSubWindow::closeEvent : " ;
    if(mdiArea()->subWindowList().size() == 1)
    {
        // put up the warning about closing the last window
        ((QtMainWindow*)cadwindow->gethwnd())->closeSubWindow(this);
        closeEvent->ignore();
    }
    else
    {
        //mdiArea()->removeSubWindow(this);
        //mdiArea()->tileSubWindows();
        ((QtMainWindow*)cadwindow->gethwnd())->closeSubWindow(this);
        closeEvent->accept();
        //emit isClosing();
    }
}

void RCMdiSubWindow::hideEvent(QHideEvent * hideEvent)
{
    //qDebug() << "entering RCMdiSubWindow::hideEvent : " ;
    QMdiSubWindow::hideEvent(hideEvent);
    //hideEvent->accept();
    //mdiArea()->tileSubWindows();
}

void RCMdiSubWindow::moveEvent(QMoveEvent * moveEvent)
{
    qDebug() << "entering RCMdiSubWindow::moveEvent : " ;
    //move(moveEvent->oldPos());
    //moveEvent->ignore();
    //moveEvent->accept();
    QMdiSubWindow::moveEvent(moveEvent);
    if(cadwindow != 0)
    {
        View3dWindow *window = cadwindow->matchview3dwindow(this->widget());
        if(window !=0)
        {
            //qDebug() << "width  : " << width();
            //qDebug() << "height : " << height();
            window->sizeevent(width(),height());
            //window->zoom(window->getumin(),window->getvmin(),window->getumax(),window->getvmax());
        }
    }
}

void RCMdiSubWindow::resizeEvent(QResizeEvent * event)
{
    QMdiSubWindow::resizeEvent(event);
    qDebug() << "entering RCMdiSubWindow::resizeEvent : " ;
    qDebug() << "resize : event width   : " << event->size().width();
    qDebug() << "resize : event height  : " << event->size().height();
    qDebug() << "resize : window width  : " << width();
    qDebug() << "resize : window height : " << height();

    if(cadwindow != 0)
    {
        View3dWindow *win = cadwindow->matchview3dwindow(this->widget());
        //if(win !=0)
        //    win->sizeevent(width(),height());
        if(win !=0)
            win->sizeevent(event->size().width(),event->size().height());
    }
}


/**
 * @brief RCstyleIndicator::RCstyleIndicator
 * @param parent
 * Implementation of the RCstyleIndicator widget class.
 */
RCstyleIndicator::RCstyleIndicator(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
}

/**
 * @brief RCstyleIndicator::paintEvent
 * @param event
 */
void RCstyleIndicator::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    QColor bcolr(::v.getreal("wn::backgroundcolour.red")*255.0,
                 ::v.getreal("wn::backgroundcolour.green")*255.0,
                 ::v.getreal("wn::backgroundcolour.blue")*255.0);
    QBrush backBrush(bcolr);
    //painter.fillRect(rect().left()+10,rect().top(),rect().width()-20,rect().height(),backBrush);
    painter.fillRect(rect().left()+10,rect().top(),rect().width(),rect().height(),backBrush);

    LineStyle *linestyle;

    QColor lcolr(db.colourtable[db.header.getcolour()].red,
                 db.colourtable[db.header.getcolour()].green,
                 db.colourtable[db.header.getcolour()].blue);

    // get the pen width from the mm with
    //int ydotspermm = double(GetDeviceCaps(cadwindow->gethdc(),VERTRES)) / double(GetDeviceCaps(cadwindow->gethdc(),VERTSIZE));
    //int penWidth = (ydotspermm < 3 ? 3 : ydotspermm) * db.lineweights.weighttomm(v.getinteger("df::weight")) + 0.5;
    // get the pen with in pixels from the fixed pixel widths
    int penWidth = db.lineweights.weighttopx(v.getinteger("df::weight"));
    //qDebug() << "line width : " << db.lineweights.weighttomm(index.row()+1) << " penwidth : " << penWidth;

    QPen lpen(Qt::SolidLine);
    lpen.setColor(lcolr);
    lpen.setWidth(penWidth);
    lpen.setCapStyle(Qt::FlatCap);
    painter.setPen(lpen);

    if ((linestyle = db.linestyles.indextostyle(db.header.getstyle())) == NULL)
    {
        painter.drawLine(rect().left()+20,rect().height() - (rect().height()/2),
                         rect().right()-20,rect().height() - (rect().height()/2));
    }
    else
    {
        linestyle->start(0.5,rect().width()-40);
        linestyle->draw(&painter,rect().left()+20,rect().height() - (rect().height()/2),
                                rect().right()-20,rect().height() - (rect().height()/2));

    }
}

/**
 * @brief RCstyleIndicator::~RCstyleIndicator
 */
RCstyleIndicator::~RCstyleIndicator()
{
}

/**
 * @brief RCCursor::RCCursor
 */
#if 1
RCCursor::RCCursor()
{
    mcursor = QCursor();
    m_name = "";
}
#else
RCCursor::RCCursor()
    : QCursor()
{
    m_name = "";
}
#endif
/**
 * @brief RCCursor::RCCursor
 * @param shape
 */
#if 1
RCCursor::RCCursor(Qt::CursorShape shape)
{
    mcursor = QCursor(shape);
    switch (shape)
    {
    case Qt::ArrowCursor :
        m_name = "Qt::ArrowCursor";
        break;
    case Qt::WaitCursor :
        m_name = "Qt::WaitCursor";
    default:
        m_name = "Qt::CursorShape";
        break;
    }
}
#else
RCCursor::RCCursor(Qt::CursorShape shape)
    : QCursor(shape)
{
    switch (shape)
    {
    case Qt::ArrowCursor :
        m_name = "Qt::ArrowCursor";
        break;
    case Qt::WaitCursor :
        m_name = "Qt::WaitCursor";
    default:
        m_name = "Qt::CursorShape";
        break;
    }
}
#endif

/**
 * @brief RCCursor::RCCursor
 * @param bitmapName
 * @param maskName
 * @param hotX
 * @param hotY
 */
#if 1
RCCursor::RCCursor(QString bitmapName, QString maskName, int hotX, int hotY)
{
    //qInfo() << "name : " << bitmapName;
    //int dpr = app->desktop()->screen()->devicePixelRatio();
    //QPixmap bm(bitmapName);
    //bm = bm.scaled(bm.width()*2,bm.height()*2,Qt::KeepAspectRatio);
   // QPixmap ms(maskName);
    //ms = ms.scaled(ms.width()*2,ms.height()*2,Qt::KeepAspectRatio);

    //mcursor = QCursor(QBitmap(bm),QBitmap(ms),hotX,hotY);
    mcursor = QCursor(QPixmap(bitmapName),QPixmap(maskName),hotX,hotY);
    m_name = bitmapName;
    //qInfo() << "DPR  : " << mcursor.bitmap()->devicePixelRatioF();
}
#else
RCCursor::RCCursor(QString bitmapName, QString maskName, int hotX, int hotY)
    : QCursor(QBitmap(bitmapName),QBitmap(maskName),hotX,hotY)
{
    m_name = bitmapName;
}
#endif
/**
 * @brief RCCursor::RCCursor
 * @param pixmapName
 * @param hotX
 * @param hotY
 */
#if 1
RCCursor::RCCursor(QString pixmapName, int hotX, int hotY)
{
    mcursor = QCursor(QPixmap(pixmapName),hotX,hotY);
    m_name = pixmapName;
}
#else
RCCursor::RCCursor(QString pixmapName, int hotX, int hotY)
    : QCursor(QPixmap(pixmapName),hotX,hotY)
{
    m_name = pixmapName;
}
#endif
/**
 * @brief operator ==
 * @param me
 * @param you
 * @return
 */
#if 1
bool operator==( RCCursor me, RCCursor you )
{
    if( me.m_name == "QT::CursorShape" && you.m_name == "QT::CursorShape")
        return me.mcursor.shape() == you.mcursor.shape() ? true : false;
    else
        return me.m_name == you.m_name ? true : false;
    return false;
}
#else
bool operator==( RCCursor me, RCCursor you )
{
    if( me.m_name == "QT::CursorShape" && you.m_name == "QT::CursorShape")
        return me.shape() == you.shape() ? true : false;
    else
        return me.m_name == you.m_name ? true : false;
    return false;
}
#endif
//
/**
 * @brief RCColourEditValidator::fixup
 * @param input
 */
void RCColourEditValidator::fixup(QString & input) const
{
    int colour = input.toInt();
    if( colour < 0) colour = 0;
    if( colour > 255) colour = 255;
    input = QString("%1").arg(colour,3);
}
/**
 * @brief RCColourEditValidator::validate
 * @param input
 * @param pos
 * @return QValidator::State
 */
QValidator::State RCColourEditValidator::validate(QString & input, int & pos) const
{
    int colour = input.toInt();
    if( colour >= 0 && colour < 256)
        return QValidator::Acceptable;
    else
        return QValidator::Invalid;
}

/**
 * @brief RCWeightEditValidator::validate
 * @param input
 * @param pos
 * @return
 */
QValidator::State RCWeightEditValidator::validate(QString & input, int & pos) const
{
    double value = input.toDouble();
    for (int i = 1 ; i <= db.lineweights.numweights() ; i++)
    {
        if(value == db.lineweights.weighttomm(i))
            return QValidator::Acceptable;
        else if(value > db.lineweights.weighttomm(i) && value < db.lineweights.weighttomm(i))
            return QValidator::Invalid;
    }
}

/**
 * @brief RCWeightEditValidator::fixup
 * @param input
 */
void RCWeightEditValidator::fixup(QString & input) const
{
    int w = db.lineweights.mmtoweight(input.toDouble());
    input = QString("%1").arg(db.lineweights.weighttomm(w));
}

/**
 * @brief RCStyleEditValidator::validate
 * @param input
 * @param pos
 * @return
 */
QValidator::State RCStyleEditValidator::validate(QString & input, int & pos) const
{
    int style = input.toInt();
    if( style >= 0 && style < 256)
        return QValidator::Acceptable;
    else
        return QValidator::Invalid;
}

/**
 * @brief RCStyleEditValidator::fixup
 * @param input
 */
void RCStyleEditValidator::fixup(QString & input) const
{
    int style = input.toInt();
    if( style < 0) style = 0;
    if( style > 255) style = 255;
    input = QString("%1").arg(style,3);
}

/**
 * @brief RCLayerEditValidator::validate
 * @param input
 * @param pos
 * @return
 */
QValidator::State RCLayerEditValidator::validate(QString & input, int & pos) const
{
    int layer = atoi(input.data());
    if(layer >= 0 && layer < MAX_LAYERS )
        return QValidator::Acceptable;
    else
        return QValidator::Invalid;
}

/**
 * @brief RCLayerEditValidator::fixup
 * @param input
 */
void RCLayerEditValidator::fixup(QString & input) const
{
    int layer = atoi(input.data());
    if(layer < 0) layer = 0;
    if(layer > MAX_LAYERS) layer = MAX_LAYERS-1;
    input = QString("%1").arg(layer,3);
}

void RCStyleEntryToolBar::layerButtonPressed()
{
    WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("0"),home.getinifilename());
}

void RCStyleEntryToolBar::weightButtonPressed()
{
    WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("1"),home.getinifilename());
}

void RCStyleEntryToolBar::colourButtonPressed()
{
    WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("2"),home.getinifilename());
    //int cstate = -1;
    //UserEvent  colEvent(1000);
    //gp_dialog_command(&cstate,&colEvent,0);
}

void RCStyleEntryToolBar::styleButtonPressed()
{
    WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("3"),home.getinifilename());
}

void RCStyleEntryToolBar::entityButtonPressed()
{
    WritePrivateProfileString(_RCT("DialogSettings"),_RCT("GenProps::CurrentTab"),_RCT("4"),home.getinifilename());
}

/**
 * @brief Implementation of RCStyleEntryToolBar::weightTextChanged
 * @param value
 */
void RCStyleEntryToolBar::weightTextEdited(QString value)
{
    // using a QValidator so this is not used at the moment
}
/**
 * @brief RCStyleEntryToolBar::weightTextEditFinished
 */
void RCStyleEntryToolBar::weightTextEditFinished()
{
     QLineEdit *ew = (QLineEdit*)sender();
    if(ew)
    {
        int w = db.lineweights.mmtoweight(ew->text().toDouble());
        BitMask change(32);
        change.set(3);
        db.header.change(db.header.getcolour(),db.header.getlayer(),db.header.getstyle(),w);
        v.setinteger("df::weight",w);

        if(cadwindow)
            cadwindow->updatetitle();
    }
}

/**
 * @brief RCStyleEntryToolBar::colourTextEdited
 * @param value
 */
void RCStyleEntryToolBar::colourTextEdited(QString value)
{
    // using a QValidator so this is not used at the moment
    int colour = value.toInt();
    BitMask change(32);
    change.set(0);
    db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
    v.setinteger("df::colour",colour);

    if(cadwindow)
        cadwindow->updatetitle();
}
/**
 * @brief RCStyleEntryToolBar::colourTextEditFinished
 */
void RCStyleEntryToolBar::colourTextEditFinished()
{
    QLineEdit *ew = (QLineEdit*)sender();
    if(ew)
    {
        QString value = ew->text();
        int colour = value.toInt();
        BitMask change(32);
        change.set(0);
        db.header.change(colour,db.header.getlayer(),db.header.getstyle(),db.header.getweight());
        v.setinteger("df::colour",colour);

        if(cadwindow)
            cadwindow->updatetitle();
    }
}
/**
 * @brief RCStyleEntryToolBar::layerTextEdited
 * @param value
 */
void RCStyleEntryToolBar::layerTextEdited(QString value)
{
    // using a QValidator so this is not used at the moment
}
/**
 * @brief RCStyleEntryToolBar::layerTextEditFinished
 */
void RCStyleEntryToolBar::layerTextEditFinished()
{
    QLineEdit *ew = (QLineEdit*)sender();
    if(ew)
    {
        int layer = atoi(ew->text().data());
        if(layer < 0) layer = 0;
        if(layer > MAX_LAYERS) layer = MAX_LAYERS-1;
        BitMask change(32);
        change.set(1);
        db.header.change(db.header.getcolour(),layer,db.header.getstyle(),db.header.getweight());
        v.setinteger("df::layer",layer);

        if(cadwindow)
            cadwindow->updatetitle();
    }
}
/**
 * @brief RCStyleEntryToolBar::styleTextEdited
 * @param value
 */
void RCStyleEntryToolBar::styleTextEdited(QString value)
{
    int style = value.toInt();
    BitMask change(32);
    change.set(2);
    db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
    v.setinteger("df::style",style);

    if(cadwindow)
        cadwindow->updatetitle();
}
/**
 * @brief RCStyleEntryToolBar::styleTextEditFinished
 */
void RCStyleEntryToolBar::styleTextEditFinished()
{
    QLineEdit *ew = (QLineEdit*)sender();
    if(ew)
    {
        int style = ew->text().toInt();
        BitMask change(32);
        change.set(2);
        db.header.change(db.header.getcolour(),db.header.getlayer(),style,db.header.getweight());
        v.setinteger("df::style",style);

        if(cadwindow)
            cadwindow->updatetitle();
    }
}
/**
 * @brief styleSetIndexChanged
 * @param id
 */
//void RCStyleEntryToolBar::styleSetActivated(int id)
void RCStyleEntryToolBar::styleSetIndexChanged(int id)
{

    RCCHAR key[300],value[300],name[300],*s;
    int i,colour,style,layer,c,st,l;
    double weight,w;
    EntityList list;
    Entity *e;
    BitMask change(32);

    qDebug() << "Entering :  styleSetIndexChanged current index : " << ((QComboBox*)cadwindow->styleentry->gettypehwnd())->currentIndex();
    qDebug() << "Entering :  styleSetIndexChanged id : " << id;
    //if((i = ((QComboBox*)cadwindow->styleentry->gettypehwnd())->currentIndex()) >= 0)
	if(id>=0)
    {
        //qDebug() << "changing the style bar settings : " << i;

        strcpy(name,((QComboBox*)cadwindow->styleentry->gettypehwnd())->itemText(id));

        c = colour = v.getinteger("df::colour");
        l = layer = v.getinteger("df::layer");
        st = style = v.getinteger("df::style");

        w = weight = db.lineweights.weighttomm(v.getinteger("df::weight"));

        for (i = 1 ; ; i++)
        {
            sprintf(key,"StyleSet%d",i);
            GetPrivateProfileString(_RCT("StyleSet"),key,_RCT(""),value,300,home.getinifilename());
            if (strlen(value) == 0)
              break;
            s = strtok(value,_RCT(","));
            if (_stricmp(s,name) == 0)
            {
                 s = strtok(0,_RCT(","));
                 if (s != 0)
                   c = atoi(s);
                 s = strtok(0,_RCT(","));
                 if (s != 0)
                   l = atoi(s);
                 s = strtok(0,_RCT(","));
                 if (s != 0)
                   st = atoi(s);
                 s = strtok(0,_RCT(","));
                 if (s != 0)
                   w = atof(s);
            }
        }

        if (c != colour || layer != l || st != style || fabs(w - weight) > 0.01)
        {
            db.header.change(c,l,st,db.lineweights.mmtoweight(w));
            v.setinteger("df::colour",c);
            v.setinteger("df::layer",l);
            v.setinteger("df::style",st);
            v.setinteger("df::weight",db.lineweights.mmtoweight(w));
            cadwindow->updatetitle();
        }

        list = state.getselection();
        change.set(0);
        change.set(1);
        change.set(2);
        change.set(3);

        if (! list.empty())
        {
            db.saveundo(UD_STARTBLOCK,NULL);
            for (list.start() ; (e = list.next()) != NULL ; )
              {  e->change(change,c,l,st,db.lineweights.mmtoweight(w));
                 if (e->isa(figure_entity))
                   {  cadwindow->invalidatedisplaylist(e);
                      e->draw(DM_NORMAL);
                   }

              }
            db.saveundo(UD_ENDBLOCK,NULL);
        }
    }
}

void RCStyleEntryToolBar::weightDisplayToggled(bool checked)
{
    mWeightCB->setChecked(checked);
}

void RCStyleEntryToolBar::styleDisplayChanged(bool checked)
{
    db.setlineweightdisplay((int)checked);
    v.setinteger("db::lineweightdisplay",db.getlineweightdisplay());
    cadwindow->paintall();
}

/**
 * @brief RCCoordindateEntryValidator::fixup
 * @param input
 *
 * makes sure there is at least a 0.00 in the input
 * RealCAD complains if it is empty
 */
void RCCoordindateEntryValidator::fixup(QString & input) const
{
 qDebug() << "RCCoordindateEntryValidator::fixup";
    //if(input.length() < 1)
    //     input = "0.00";

    QValidator::fixup(input);
}

/**
 * @brief RCCoordindateEntryValidator::validate
 * @param input
 * @param pos
 * @return
 *
 * removes any of the mode switching characters before they go to the RealCAD validator
 */
QValidator::State RCCoordindateEntryValidator::validate(QString & input, int & pos) const
{
    qDebug() << "RCCoordindateEntryValidator::validate : in :" << input;
    qDebug() << "RCCoordindateEntryValidator::validate : pos :" << pos;

    bool ok=false;
    QString rchar;
    QStringList rchars;
    rchars << "m" << "M" << "P" << "p" << "X" << "x" << "Y" << "y" << "Z" << "z" << "R" << "r" << "D" << "d"
           << "T" << "t" << "A" << "a" << "B" << "b" << "L" << "l" << "I" << "i" << "\\";

    foreach( rchar, rchars )
        input.remove(rchar);

    double value = input.toDouble(&ok);
    if(ok)
        return QValidator::Acceptable;
    else
        return QValidator::Intermediate;
}


/**
 * @brief RCCoordindateEntryToolBar::mousePressEvent
 * @param event
 */
void RCCoordindateEntryToolBar::mousePressEvent(QMouseEvent * event)
{
    //qDebug() << "RCCoordindateEntryToolBar::mousePressEvent";
    if(event->button() == Qt::LeftButton)
    {
        //qDebug() << "RCCoordindateEntryToolBar::mousePressEvent : left button click";

        QWidget *child = childAt(event->pos());
        //qDebug() << "RCCoordindateEntryToolBar::mousePressEvent : child at pos " << child;
        if( child && qobject_cast<QLabel *>((QObject*)child) != 0)
        {
            //qDebug() << "RCCoordindateEntryToolBar::mousePressEvent : coordinateentry " << cadwindow->coordinateentry;
            if(cadwindow && cadwindow->coordinateentry)
                cadwindow->coordinateentry->mousemoveevent(event->pos().x(),event->pos().y(),1);
            event->accept();
        }
        else if( child && qobject_cast<QLineEdit *>((QObject*)child) != 0)
        {
            //qDebug() << "RCCoordindateEntryToolBar::mousePressEvent : coordinateentry " << cadwindow->coordinateentry;
            child->setFocus();
            event->accept();
        }
    }
    QToolBar::mousePressEvent(event);
}
//
void RCCoordindateEntryToolBar::editTextChanged(const QString & text)
{
    qDebug() << "RCCoordindateEntryToolBar::editTxtChanged";
}

//
_rcgdiobj::_rcgditype _rcgdiobj::type()
{
    return undefined;
}
_rcgdiobj::_rcgditype _rcfont::type()
{
    return font ;
}
_rcgdiobj::_rcgditype _rcpen::type()
{
    return pen ;
}
_rcgdiobj::_rcgditype _rcbrush::type()
{
    return brush ;
}
_rcgdiobj::_rcgditype _rcpixmap::type()
{
    return pixmap ;
}
_rcgdiobj::_rcgditype _rcbitmap::type()
{
    return bitmap ;
}
_rcgdiobj::_rcgditype _rcrgn::type()
{
    return region ;
}

RCCHAR * RCSTRTOK(register RCCHAR *s, register const RCCHAR *delim)
{
    register RCCHAR *spanp;
    register RCCHAR c, sc;
    RCCHAR *tok;
    static RCCHAR *last;


    if (s == NULL && (s = last) == NULL)
        return (NULL);

    /*
     * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = *s++;
    for (spanp = (RCCHAR *)delim; (sc = *spanp++) != 0;) {
        if (c == sc)
            goto cont;
    }

    if (c == 0) {		/* no non-delimiter characters */
        last = NULL;
        return (NULL);
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;) {
        c = *s++;
        spanp = (RCCHAR *)delim;
        do {
            if ((sc = *spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                last = s;
                return (tok);
            }
        } while (sc != 0);
    }
    /* NOTREACHED */
}

// a replacement for the windows functions of the same name
// with arguments suitable for Qt
RCBOOL RCGETCLIENTRECT( RCHWND hWnd, RCLPRECT lpRect)
{
    if(hWnd && lpRect)
    {
        QWidget *qw = (QWidget*)hWnd;
        lpRect->top = ((QWidget*)hWnd)->rect().top();
        lpRect->bottom = ((QWidget*)hWnd)->rect().bottom();
        lpRect->left = ((QWidget*)hWnd)->rect().left();
        lpRect->right = ((QWidget*)hWnd)->rect().right();
        return TRUE;
    }
    else
        return FALSE;
}

RCHWND RCCREATEWINDOW( RCPCHAR lpClassName,
             RCPCHAR lpWindowName,
             RCDWORD style,
             RCINT x,
             RCINT y,
             RCINT w,
             RCINT h,
             RCHWND hParent,
             RCHMENU hMenu,
             RCHINSTANCE hInstance,
             RCPVOID lpParam)
{
    //qDebug() << "Entering : RCCREATEWINDOW ";
#ifdef USING_WIDGETS
    if( strcmp(lpClassName,"MainWindow") == 0 ){
        QtMainWindow *qw = new QtMainWindow();
        if(qw){
#ifdef _MAC
            //qw->setWindowFlags(qw->windowFlags() & ~Qt::WindowFullscreenButtonHint);
            qw->setWindowFlags(qw->windowFlags() | Qt::WindowFullscreenButtonHint);
#else
            qw->setWindowFlags(qw->windowFlags() | Qt::WindowFullscreenButtonHint);
#endif
            qw->setAttribute(Qt::WA_AlwaysShowToolTips,true);

            qw->setObjectName("MainWindow");
            //QSettings settings;
            qDebug() << "restoring geometry from ini file";
            QRect qrect;
            qrect.setLeft((x == CW_USEDEFAULT) ? 0 : x);
            qrect.setTop((y == CW_USEDEFAULT) ? 0 : y);
            qrect.setWidth(w);
            qrect.setHeight(h);


            // adjust the size
            // set the initial rect
            qw->setGeometry(qrect);
            // show it to fix the initial frame size
            //qw->show();
            // get the frame rect
            QRect frect = qw->frameGeometry();
            // get width for resizing
            int widthDiff = frect.width()-qrect.width();
            int trueWidth = qrect.width()-widthDiff;
            // get the height for resizing
            int heightDiff = frect.height()-qrect.height();
            int trueHeight = qrect.height()-heightDiff;
            qw->move((x == CW_USEDEFAULT) ? 0 : x,(y == CW_USEDEFAULT) ? 0 : y);
            qw->resize(trueWidth,trueHeight);
            qw->setWindowIcon(QIcon(QPixmap("")));
            qw->setWindowIcon(QIcon(QPixmap(":/images/ico1002.png","png")));

            return qw;
        }
    }
    else if( strcmp(lpClassName,"MdiArea") == 0 )
    {
        if(hParent)
        {
#ifdef USING_TABBED_WINDOWS
           QTabWidget *qw = new QTabWidget();
            if(qw)
            {
                qw->setObjectName("QTabWidget");
                qw->setTabsClosable(true);
                //qw->setMovable(true);
                qw->setUsesScrollButtons(true);
                if(((QtMainWindow*)hParent)->objectName() == "MainWindow" )
                {
                    ((QtMainWindow*)hParent)->setCentralWidget(qw);
                    // connect to the slots to handle the tabs
                    ((QtMainWindow*)hParent)->connect(qw,SIGNAL(tabCloseRequested(int)),(QtMainWindow*)hParent,SLOT(closeTabWindow(int)));
                    ((QtMainWindow*)hParent)->connect(qw,SIGNAL(currentChanged(int)),(QtMainWindow*)hParent,SLOT(currentTabChanged(int)));
                }

                qDebug() << "central widget contents rect    : " << ((QtMainWindow*)hParent)->contentsRect();
                qDebug() << "central widget children rect    : " << ((QtMainWindow*)hParent)->childrenRect();
                qDebug() << "tabwidget geometry : " << ((QTabWidget*)qw)->geometry();

                return qw;
            }
#endif
             // check if there is a central widget and what type it is
#ifdef USING_QMDIAREA
             RCMdiArea *qw=0;
             if(((QtMainWindow*)hParent)->centralWidget() == 0)
             {
                 QWidget *centralWidget = new QWidget((QtMainWindow*)hParent);
                 centralWidget->setObjectName(QStringLiteral("centralWidget"));

                 QVBoxLayout *vLayout = new QVBoxLayout(centralWidget);
                 vLayout->setObjectName(QStringLiteral("centralLayout"));
                 vLayout->setSpacing(0);
                 vLayout->setMargin(0);
                 vLayout->setContentsMargins(0, 0, 0, 0);

                 qw = new RCMdiArea();
                 qw->setObjectName("QMdiArea");
                 ((QtMainWindow*)hParent)->MdiArea = qw;
                 //qw->setWindowFlags(Qt::WindowShadeButtonHint);
                 qw->setViewMode(QMdiArea::TabbedView);
                 qw->setOption(QMdiArea::DontMaximizeSubWindowOnActivation,false);
                 //qw->setActivationOrder(QMdiArea::ActivationHistoryOrder);
                 qw->setActivationOrder(QMdiArea::StackingOrder);
                 qw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                 qw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

                 vLayout->addWidget(qw);
                 ((QtMainWindow*)hParent)->setCentralWidget(centralWidget);

                 //((QtMainWindow*)hParent)->setCentralWidget(qw);
                 //qw->resize(((QtMainWindow*)hParent)->size());
                 //((QtMainWindow*)hParent)->resize(((QtMainWindow*)hParent)->size());
                 //((QtMainWindow*)hParent)->connect(qw,SIGNAL(tabCloseRequested(int)),(QtMainWindow*)hParent,SLOT(closeSubWindow(int)));
                 ((QtMainWindow*)hParent)->connect((QMdiArea*)qw,SIGNAL(subWindowActivated(QMdiSubWindow*)),(QtMainWindow*)hParent,SLOT(subWindowActivated(QMdiSubWindow*)));
             }
             else
             {
                 //qw = (RCMdiArea*)((QtMainWindow*)hParent)->centralWidget();
                 qw = (RCMdiArea*)((QtMainWindow*)hParent)->MdiArea;
                 ((QtMainWindow*)hParent)->connect((QMdiArea*)qw,SIGNAL(subWindowActivated(QMdiSubWindow*)),(QtMainWindow*)hParent,SLOT(subWindowActivated(QMdiSubWindow*)));
             }
             // see if is a sub window in the midi area if not add one
             // if there is one return it
             /*
             if(qw->subWindowList().size() == 0)
             {
                 QMdiSubWindow *subWin = new QMdiSubWindow;
                 subWin->setObjectName("QMdiSubWindow");
                 subWin->resize(w,h);
                 subWin->setAttribute(Qt::WA_DeleteOnClose);
                 qw->addSubWindow(subWin);
                 return subWin;
             }
             else
                 return qw->subWindowList().at(0);
             */
             qDebug() << "QtMainWindow geometry : " << ((QtMainWindow*)hParent)->geometry();
             qDebug() << "QMdiArea geometry on creation : " << qw->geometry();
             return qw;
#endif
             RCCentralWidget *qw=0;
             if(((QtMainWindow*)hParent)->centralWidget() == 0)
             {
                 qw = new RCCentralWidget((QtMainWindow*)hParent);
                 qw->setObjectName(QStringLiteral("centerWidget"));
                 /*
                 QVBoxLayout *vLayout = new QVBoxLayout(qw);
                 vLayout->setObjectName(QStringLiteral("centralLayout"));
                 vLayout->setSpacing(0);
                 vLayout->setMargin(0);
                 vLayout->setContentsMargins(0, 0, 0, 0);

                 vLayout->addWidget(qw);
                 */
                 qDebug() << "RCCentralWidget geometry on creation : " << qw->geometry();
                 ((QtMainWindow*)hParent)->setCentralWidget(qw);
                 ((QtMainWindow*)hParent)->centerWidget = qw;
                 ((QtMainWindow*)hParent)->adjustSize();

             }
             else
             {
                 qw = (RCCentralWidget*)((QtMainWindow*)hParent)->centerWidget;
             }

             qDebug() << "QtMainWindow geometry : " << ((QtMainWindow*)hParent)->geometry();
             qDebug() << "RCCentralWidget geometry on creation : " << qw->geometry();
             return qw;
        }
    }
    else if( strcmp(lpClassName,"EDIT") == 0 ){
        // create an line edit widget
        QLineEdit *Edit = new QLineEdit();
        if(Edit) {
            Edit->setObjectName("QLineEdit");
            QRect qrect;
            qrect.setLeft(x);
            qrect.setTop(y);
            qrect.setWidth(w);
            qrect.setHeight(h);
            Edit->setGeometry(qrect);
            // if an edit control has NULL parent create a toolbar to hold it
            if( hParent == NULL )
            {
                QToolBar *ToolBar = new QToolBar((QWidget*)hParent);
                if(ToolBar){
                    ToolBar->setWindowTitle(app->tr("EDIT"));
                    ToolBar->setObjectName("QToolBar");
                    ToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    ToolBar->addWidget(Edit);
                    Edit->setParent(ToolBar);
                    return ToolBar;
                }
            }
            else {
                if(qobject_cast<QToolBar*>((QObject*)hParent))
                    Edit->setParent((QToolBar*)hParent);
                else
                    Edit->setParent((QWidget*)hParent);
                return Edit;
            }
        }
    }
    else if( strcmp(lpClassName,"STATIC") == 0 ){
        // create a label widget
        QLabel *Static = new QLabel();
        if(Static){
            Static->setObjectName("QLabel");
            QRect qrect;
            qrect.setLeft(x);
            qrect.setTop(y);
            qrect.setWidth(w);
            qrect.setHeight(h);
            Static->setGeometry(qrect);
            Static->setFrameStyle(QFrame::Raised | QFrame::Panel);
            Static->setFrameRect(QRect(0,0,0,0));
            Static->setMargin(3);
            // if an edit control has NULL parent create a toolbar to hold it
            if( hParent == NULL )
            {
                QToolBar *ToolBar = new QToolBar((QWidget*)hParent);
                if(ToolBar){
                    ToolBar->setWindowTitle(app->tr("STATIC"));
                    ToolBar->setObjectName("QToolBar");
                    ToolBar->setFloatable(true);
                    ToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    ToolBar->addWidget(Static);
                    Static->setParent(ToolBar);
                    return ToolBar;
                }
            }
            else{
                if(qobject_cast<QToolBar*>((QObject*)hParent))
                    Static->setParent((QToolBar*)hParent);
                else
                    Static->setParent((QWidget*)hParent);
                return Static;
            }
        }
    }
    else if( strcmp(lpClassName,"COMBOBOX") == 0 ){
        // create a combobox widget
        QComboBox *ComboBox = new QComboBox();
        if(ComboBox){
            ComboBox->setObjectName("QComboBox");
            QRect qrect;
            qrect.setLeft(x);
            qrect.setTop(y);
            qrect.setWidth(w);
            qrect.setHeight(h);
            ComboBox->setGeometry(qrect);
            // if the control has NULL parent create a toolbar to hold it
            if( hParent == NULL )
            {
                QToolBar *ToolBar = new QToolBar((QWidget*)hParent);
                if(ToolBar){
                    ToolBar->setWindowTitle(app->tr("COMBOBOX"));
                    ToolBar->setObjectName("QToolBar");
                    ToolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
                    ToolBar->addWidget(ComboBox);
                    ComboBox->setParent(ToolBar);
                    return ToolBar;
                }
            }
            else{
                if(qobject_cast<QToolBar*>((QObject*)hParent))
                    ComboBox->setParent((QToolBar*)hParent);
                else
                    ComboBox->setParent((QWidget*)hParent);
                return ComboBox;
            }
        }
    }
    else if( strcmp(lpClassName,"TOOLBUTTON") == 0 ){
        // create a combobox widget
        QToolButton *ToolButton = new QToolButton();
        if(ToolButton){
            ToolButton->setObjectName("QToolButton");
            QRect qrect;
            qrect.setLeft(x);
            qrect.setTop(y);
            qrect.setWidth(w);
            qrect.setHeight(h);
            ToolButton->setGeometry(qrect);
            // if an edit control has NULL parent create a toolbar to hold it
            if( hParent == NULL )
            {
                QToolBar *ToolBar = new QToolBar((QWidget*)hParent);
                if(ToolBar){
                    ToolBar->setWindowTitle(app->tr("TOOLBUTTON"));
                    ToolBar->setObjectName("QToolBar");
                    ToolBar->setAllowedAreas(Qt::AllToolBarAreas);
                    ToolBar->addWidget(ToolButton);
                    ToolButton->setParent(ToolBar);
                    return ToolBar;
                }
            }
            else{
                ToolButton->setParent((QWidget*)hParent);
                return ToolButton;
            }
        }
    }
    else if( strcmp(lpClassName,"CadWindow") == 0)
    {
        // create a 3D view window
        //qDebug() << "style & RCGLWINTYPE = " << (style & RCGLWINTYPE);

        if((style & RCSCENEVW) == RCSCENEVW)
        {
#ifdef USING_TABBED_WINDOWS
            qDebug() << "tabwidget geometry : " << ((QTabWidget*)hParent)->geometry();
            qDebug() << "tab heigth : " << ((QTabWidget*)hParent)->tabBar()->geometry();
#endif
#ifdef USING_MDIAREA
            RCGraphicsView *sceneView = new RCGraphicsView();
            if(sceneView != 0)
            {
                QRect qrect;
                qrect.setLeft(x);
                qrect.setTop(y);
                qrect.setWidth(w);
                qrect.setHeight(h);
                qDebug() << "sceneView input rect : " << qrect;

                sceneView->setObjectName("QGraphicsView");
                //sceneView->setGeometry(qrect);
                //sceneView->setGeometry(0,0,w,h);
                //sceneView->move(0,0);
                //sceneView->resize(w,h);

                qDebug() << "sceneView geometry initial rect : " << sceneView->geometry();

                // various settings
                sceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                sceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                sceneView->setCacheMode(QGraphicsView::CacheNone);
                sceneView->setMouseTracking(true);
                sceneView->setDragMode(QGraphicsView::NoDrag);
                //sceneView->setDragMode(QGraphicsView::RubberBandDrag);
                //sceneView->setDragMode(QGraphicsView::ScrollHandDrag);
                sceneView->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing,true);
                sceneView->setOptimizationFlag(QGraphicsView::DontClipPainter,false);
                sceneView->setOptimizationFlag(QGraphicsView::DontSavePainterState,false);
                sceneView->setOptimizationFlag(QGraphicsView::IndirectPainting,false);

                RCCHAR text[300];
                GetPrivateProfileString("Antialiasing","On","Y",text,300,home.getinifilename());
                if(RCSTRING(text) == "Y")
                    sceneView->setRenderHint(QPainter::Antialiasing, true);
                else
                    sceneView->setRenderHint(QPainter::Antialiasing, false);

                sceneView->setRenderHint(QPainter::TextAntialiasing, true);
                sceneView->setRenderHint(QPainter::SmoothPixmapTransform, false);
                //sceneView->setRenderHint(QPainter::HighQualityAntialiasing, false);
                sceneView->setRenderHint(QPainter::Qt4CompatiblePainting, false);
                sceneView->setRenderHint(QPainter::NonCosmeticDefaultPen, false);
                //sceneView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
                //sceneView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
                //sceneView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
                sceneView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
                //sceneView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
                //sceneView->setAlignment(Qt::AlignCenter);
                sceneView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
                sceneView->setTransformationAnchor(QGraphicsView::NoAnchor);
                //sceneView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
                //sceneView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
                sceneView->setResizeAnchor(QGraphicsView::NoAnchor);
                //sceneView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
                //sceneView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
#endif
#ifdef USING_TABBED_WINDOWS
                if(hParent != 0)
                {
                    ((QTabWidget*)hParent)->addTab(sceneView,"View");
                    sceneView->resize(((QTabWidget*)hParent)->geometry().width(),
                                      ((QTabWidget*)hParent)->geometry().height()-
                                      ((QTabWidget*)hParent)->tabBar()->geometry().height());
                    qDebug() << "tabwidget geometry after add tab : " << ((QTabWidget*)hParent)->geometry();
                    qDebug() << "tab heigth : " << ((QTabWidget*)hParent)->tabBar()->geometry();
                    qDebug() << "scene view geometry after adding to tab : " << sceneView->geometry();
                    //sceneView->adjustSize();
#endif
#ifdef USING_QMDIAREA
                QMdiArea *qmda=0;
                if(hParent != 0)
                    qmda = (QMdiArea*)hParent;
                else
                    qmda = ((QtMainWindow*)cadwindow->gethwnd())->MdiArea;
                    //qmda = (QMdiArea*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget();
                if(qmda == 0)
                    return 0;

                // make a new QMdiSubWindow and add it to the QMdiArea
                //QMdiSubWindow *subWin = new QMdiSubWindow;
                RCMdiSubWindow *subWin = new RCMdiSubWindow;
                subWin->setAttribute(Qt::WA_DeleteOnClose);
                //qmda->connect(subWin,SIGNAL(destroyed(QObject*)),qmda,SLOT(tileSubWindows()));
                QObject::connect(subWin,SIGNAL(destroyed(QObject*)),qmda,SLOT(tileSubWindows()));
                subWin->setObjectName("QMdiSubWindow");
                //subWin->resize(w,h);
                qmda->addSubWindow(subWin);
                subWin->showMaximized();
                qmda->setActiveSubWindow(subWin);

                QVBoxLayout *vLayout = new QVBoxLayout;
                vLayout->setObjectName(QStringLiteral("verticalLayout"));
                vLayout->setSpacing(0);
                vLayout->setContentsMargins(0, 0, 0, 0);
                vLayout->addWidget(sceneView);
                subWin->setLayout(vLayout);

                subWin->setWidget(sceneView);
                sceneView->setWindowTitle("QGraphicsView");
                //sceneView->resize(((QMdiSubWindow*)hParent)->geometry().width(),
                //                  ((QMdiSubWindow*)hParent)->geometry().height());
                //sceneView->resize(subWin->size());
                qDebug() << "MdiSubWindow parent geometry after adding     : " << ((QMdiSubWindow*)hParent)->geometry();
                qDebug() << "MdiSubWindow geometry after adding            : " << subWin->geometry();
                qDebug() << "sceneView geometry after adding to sub window : " << subWin->widget()->geometry();
#endif
            int pparam = lpParam != 0 ? *((int*)lpParam) : 0;
            //RCView *sceneView = new RCView("RCView",new RCGraphicsView());
            RCView *sceneView = new RCView("RCView");
            sceneView->setObjectName("RCView");
            if(sceneView !=0)
            {
                RCCentralWidget *qmda=0;
                if(hParent != 0)
                    qmda = (RCCentralWidget*)hParent;
                else
                    qmda = ((QtMainWindow*)cadwindow->gethwnd())->centerWidget;
                    //qmda = (QMdiArea*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget();
                if(qmda == 0)
                    return 0;

                ((QtMainWindow*)hParent)->connect(sceneView,SIGNAL(wasResized(RCView*)),(QtMainWindow*)cadwindow->gethwnd(),SLOT(onViewResize(RCView*)));
                ((QtMainWindow*)hParent)->connect(sceneView,SIGNAL(isClosing(RCView*)),(QtMainWindow*)cadwindow->gethwnd(),SLOT(onViewClosed(RCView*)));
                ((QtMainWindow*)hParent)->connect(sceneView,SIGNAL(wasActivated(RCView*)),(QtMainWindow*)cadwindow->gethwnd(),SLOT(onViewActivated(RCView*)));

                RCGraphicsView *rcgv = new RCGraphicsView(sceneView);
                if(rcgv)
                {
                    rcgv->setObjectName("RCGraphicsView");
                }

                QWidget *gw=0;
                if((style & RCGLWINTYPE) == RCGLWINTYPE)
                {
#if 1
                    QGLWidget *glw = new QGLWidget(QGLFormat(QGL::SampleBuffers));
#else
                    RCGLWidget *glw = new RCGLWidget(QGLFormat(QGL::SampleBuffers));
#endif
                    if(glw)
                    {
                        gw=glw;
                        glw->setObjectName("QGLWidget");
                        rcgv->setViewport(gw);
                        sceneView->setView(rcgv);
                        qDebug() << "gl widget geometry after adding : " << glw->geometry();

                    }
                }
                else if((style & RCWINTYPE) == RCWINTYPE)
                {
                    gw = new QWidget();
                    if(gw)
                    {
                        gw->setObjectName("QWidget");
                        if(rcgv)
                            rcgv->setViewport(gw);
                        //sceneView->setViewport(gw);
                    }
                }

                RCGraphicsScene *scene = 0;
                if(((style & RCSCENEVW) == RCSCENEVW) && ((style & RCV3DWINTYPE) == RCV3DWINTYPE))
                {
                    // use a new scene for each view
#if 1
                    scene = new RCGraphicsScene(rcgv);
                    // use one scene for all 3d views
                    scene->setObjectName("QGraphicsScene");
#else
                    if(program->m_scene == 0 )
                    {
                        scene = new RCGraphicsScene();
                        scene->setObjectName("QGraphicsScene");
                        program->m_scene = scene;
                    }
                    else
                        scene = program->m_scene;
#endif
                }
                else if(((style & RCSCENEVW) == RCSCENEVW) && ((style & RCV2DWINTYPE) == RCV2DWINTYPE))
                {
                    // use seperate scenes for 2d views
                    scene = new RCGraphicsScene();
                    scene->setObjectName("QGraphicsScene");
                }

                if(scene != 0)
                {
                    rcgv->setScene(scene);
                    //sceneView->setScene(scene);
                    qDebug() << "scene viewport   rect : " << sceneView->view()->viewport()->rect();
                    qDebug() << "scene view scene rect : " << sceneView->view()->sceneRect();
                }

                //
                //if(rcgv)
                //    sceneView->setView(rcgv);

                if(/*((pparam & 1) != 1) &&*/ qmda && sceneView)
                     qmda->addView(sceneView);
                //sceneView->setSceneRect(QRect());
                //sceneView->viewport()->resize(sceneView->view()->size());
                //((QtMainWindow*)hParent)->connect(sceneView,SIGNAL(wasActivated(RCView*)),(QtMainWindow*)cadwindow->gethwnd(),SLOT(onViewActivated(RCView*)));

                //qDebug() << "scene viewport   rect : " << sceneView->view()->viewport()->rect();
                //qDebug() << "scene view scene rect : " << sceneView->view()->sceneRect();

                //qDebug() << "sceneView rect   : " << sceneView->rect();
                //qDebug() << "scene rect       : " << scene->sceneRect();
                //qDebug() << "gw rect          : " << gw->rect();

                //qDebug() << "gw DVP          : " << gw->devicePixelRatio();
                //qDebug() << "gw physicalDpiX : " << gw->physicalDpiX();
                //qDebug() << "gw physicalDpiY : " << gw->physicalDpiY();
                //qDebug() << "gw logicalDpiX : " << gw->logicalDpiX();
                //qDebug() << "gw logicalDpiY : " << gw->logicalDpiY();

                qDebug() << "sceneView parent : " << sceneView->parent();

                return sceneView;
            }
        }
        else if((style & RCGLWINTYPE) == RCGLWINTYPE)
        {
            QGLWidget *subview = new QGLWidget();
            if(subview != 0)
            {
                subview->setMouseTracking(true);
                subview->setObjectName("QGLWidget");
                if(hParent != 0)
                    ((QMdiArea*)hParent)->addSubWindow(subview);

                QRect qrect;
                qrect.setLeft(x);
                qrect.setTop(y);
                qrect.setWidth(w);
                qrect.setHeight(h);

                ((QMdiSubWindow*)subview->parent())->setObjectName("QMdiSubWindow");
                //((QMdiSubWindow*)subview->parent())->setGeometry(qrect);
                ((QMdiSubWindow*)subview->parent())->setWindowIcon(QIcon(QPixmap("")));
                ((QMdiSubWindow*)subview->parent())->setWindowIcon(QIcon(QPixmap(":/images/ico1002.png","png")));
                ((QMdiSubWindow*)subview->parent())->setWindowTitle("GL Widget View");

                return subview;
            }
        }
        else
        {
            QWidget *subview = new QWidget();
            if(subview != 0)
            {
                subview->setMouseTracking(true);
                subview->setObjectName("QWidget");
                if(hParent != 0)
                    ((QMdiArea*)hParent)->addSubWindow(subview);

                QRect qrect;
                qrect.setLeft(x);
                qrect.setTop(y);
                qrect.setWidth(w);
                qrect.setHeight(h);

                ((QMdiSubWindow*)subview->parent())->setObjectName("QMdiSubWindow");
                ((QMdiSubWindow*)subview->parent())->setGeometry(qrect);
                ((QMdiSubWindow*)subview->parent())->setWindowIcon(QIcon(QPixmap("")));
                ((QMdiSubWindow*)subview->parent())->setWindowIcon(QIcon(QPixmap(":/images/ico1002.png","png")));
                ((QMdiSubWindow*)subview->parent())->setWindowTitle("Simple Widget View");

                return subview;
            }
        }

    }
    else if( strcmp(lpClassName,"msctls_updown32") == 0 )
    {
        return 0;
    }
    else if( strcmp(lpClassName,"PrintPreviewWindow") == 0 )
    {
        return 0;
    }
    return 0;
#endif
#ifdef DERIVE_FROM_WIDGETS
    QWidget *qw = (QWidget*)lpParam;
    qw->setParent((QWidget*)hParent);
    //QWidget *qw = new QWidget((QWidget*)hParent);

    QRect qrect;
    qrect.setLeft((x == CW_USEDEFAULT) ? 0 : x);
    qrect.setTop((y == CW_USEDEFAULT) ? 0 : y);
    qrect.setWidth(w);
    qrect.setHeight(h);
    qw->setGeometry(qrect);

    Qt::WindowFlags windowFlags = 0;
    if( (WS_CHILD & style) == WS_CHILD )
        windowFlags |= Qt::Widget;
    if( (WS_CLIPCHILDREN & style) == WS_CLIPCHILDREN )
        windowFlags |= Qt::Window;
    if( (WS_BORDER & style) == WS_BORDER )
        windowFlags |= Qt::Dialog;
    if( (WS_VISIBLE & style) == WS_VISIBLE )
        qw->setVisible(true);
    if( (ES_AUTOHSCROLL & style) == ES_AUTOHSCROLL )
    {
        QScrollArea *scrollArea = new QScrollArea;
        scrollArea->setWidgetResizable(false);
        scrollArea->setWidget(qw);
    }
    if( (WS_OVERLAPPEDWINDOW & style) == WS_OVERLAPPEDWINDOW )
    {
        windowFlags |= Qt::WindowTitleHint;
        windowFlags |= Qt::WindowCloseButtonHint;
        windowFlags |= Qt::WindowMinMaxButtonsHint;
    }
    qw->setWindowFlags(windowFlags);
    return qw;
#endif
}

RCHWND RCCREATEWINDOWEX( RCDWORD dwExStyle,
             RCPCHAR lpClassName,
             RCPCHAR lpWindowName,
             RCDWORD style,
             RCINT x,
             RCINT y,
             RCINT w,
             RCINT h,
             RCHWND hParent,
             RCHMENU hMenu,
             RCHINSTANCE hInstance,
             RCPVOID lpParam)
{
    return RCCREATEWINDOW(
                 //dwExStyle,
                 lpClassName,
                 lpWindowName,
                 style,
                 x,
                 y,
                 w,
                 h,
                 hParent,
                 hMenu,
                 hInstance,
                 lpParam);
}

RCBOOL RCMOVEWINDOW( RCHWND hWnd,
                     RCINT X,
                     RCINT Y,
                     RCINT nWidth,
                     RCINT nHeight,
                     RCBOOL bRepaint)
{
    if(hWnd)
    {
        //((QWidget*)hWnd)->move(X,Y);
        //((QWidget*)hWnd)->resize(nWidth,nHeight);
        ((QWidget*)hWnd)->setGeometry(X,Y,nWidth,nHeight);
        if(bRepaint)
            ((QWidget*)hWnd)->update();
        return TRUE;
    }
    else
        return FALSE;
}

RCBOOL RCSHOWWINDOW( RCHWND hWnd, RCINT show)
{
    if(hWnd == 0)
        return FALSE;

    switch(show)
    {
    case SW_MAXIMIZE :
        ((QWidget*)hWnd)->showMaximized();
        break;
    case SW_MINIMIZE :
        ((QWidget*)hWnd)->showMinimized();
        break;
    case SW_HIDE :
        ((QWidget*)hWnd)->setWindowState(Qt::WindowNoState );
        ((QWidget*)hWnd)->hide();
        break;
    case SW_SHOW :
    case SW_RESTORE :
    case SW_SHOWNORMAL :
    default:
        ((QWidget*)hWnd)->showNormal();
        break;
    }
    return true;
}

RCBOOL RCBRINGWINDOWTOTOP( RCHWND hWnd )
{
    if(hWnd)
    {
        ((QWidget*)hWnd)->raise();
        return TRUE;
    }
    return FALSE;
}

RCBOOL RCINVALIDATERECT( RCHWND hWnd, const RECT* lpRect, RCBOOL bErase )
{
    if(hWnd)
    {
        ((QWidget*)hWnd)->update();
        return TRUE;
    }
    return FALSE;
}

RCBOOL RCDESTROYWINDOW( RCHWND hWnd )
{
    if(hWnd)
    {
        //qDebug() << "Entering Destroy Window : hWnd : " << (QWidget*)hWnd;
        ((QWidget*)hWnd)->deleteLater();
        //delete hWnd;
        hWnd = 0;
        return true;
    }
    return false;
}

RCBOOL RCBITBLT( RCHDC hdcDest, // handle to destination DC
                 RCINT nXDest,  // x-coord of destination upper-left corner
                 RCINT nYDest,  // y-coord of destination upper-left corner
                 RCINT nWidth,  // width of destination rectangle
                 RCINT nHeight, // height of destination rectangle
                 RCHDC hdcSrc,  // handle to source DC
                 RCINT nXSrc,   // x-coordinate of source upper-left corner
                 RCINT nYSrc,   // y-coordinate of source upper-left corner
                 RCDWORD dwRop  // raster operation code);
                 )
{
    return true;
}

RCHGDIOBJ RCSELECTOBJECT( RCHDC hDc, RCHGDIOBJ hObj )
{
    //qDebug() << "Entering : RCSELECTOBJECT ";
#if 0
    if(hDc && hObj)
    {
        if(hObj->type() == _rcgdiobj::font)
        {
            static QFont oldFont = ((QWidget*)hDc)->font();
            ((QWidget*)hDc)->setFont(*(QFont*)((RCHFONT)hObj));
            static _rcfont rcfont = static_cast<_rcfont&>(oldFont);
            return (RCHGDIOBJ)&rcfont;
        }
        else
        {
           return hObj;
        }
    }
    else if(!hDc && hObj)
    {
        if(hObj->type() == _rcgdiobj::font)
        {
            static QFont oldFont = app->font();
            app->setFont(*((QFont*)hObj));
            return (RCHGDIOBJ)&oldFont;
        }
        else
        {
            return hObj;
        }
    }
#endif
    return hObj;
}

RCINT RCSETROP2( RCHDC hdc,         // handle to DC
                 RCINT fnDrawMode   // drawing mode
                 )
{
    return 0;
}

RCBOOL RCMOVETOEX( RCHDC hdc ,RCINT xp, RCINT yp, RCLPPOINT plPoint)
{
    if(hdc)
    {
        View3dWindow* v=0;
        if(dynamic_cast<View3dWindow*>((Window*)hdc) != 0)
            v = (View3dWindow*)hdc;
        if(v != 0)
        {
            //((View3dWindow*)((RCView*)hdc)->getView3dWindow())->moveto(xp,yp);
            v->moveto(xp,yp);
            return true;
        }
        //((Surface*)hdc)->moveto(xp,yp);
        //return true;
    }
    return false;
}

RCBOOL RCLINETO( RCHDC hdc,    // device context handle
                 RCINT nXEnd,  // x-coordinate of ending point
                 RCINT nYEnd   // y-coordinate of ending point
                 )
{
    if(hdc)
    {
        if(dynamic_cast<View3dWindow*>((Window*)hdc) != 0)
        {
            View3dWindow* v =  (View3dWindow*)hdc;

            QGraphicsItem *qgi = 0;
            qgi = v->lineto(qgi,nXEnd,nYEnd);
            if(qgi)
            {
                //qgi->setZValue(QGDRAGZVALUE(QGMAXZVALUE));
                if(v->qgi)
                    ((QGraphicsItemGroup*)v->qgi)->addToGroup(qgi);
            }
            return true;
        }

        //((Surface*)hdc)->lineto(nXEnd,nYEnd);
        //return true;
    }
    return false;
}

RCINT RCESCAPE( RCHDC hdc,           // handle to DC
                RCINT nEscape,       // escape function
                RCINT cbInput,       // size of input structure
                RCLPCSTR lpvInData,  // input structure
                RCPVOID lpvOutData   // output structure
                )
{
    return 0;
}

RCUINT RCGETSYSTEMPALETTEENTRIES( RCHDC hdc,              // handle to DC
                                  RCUINT iStartIndex,     // first entry to be retrieved
                                  RCUINT nEntries,        // number of entries to be retrieved
                                  LPPALETTEENTRY lppe     // array that receives entries
                                  )
{
    return 0;
}

RCINT GetDeviceCaps( RCHDC hdc,     // handle to DC
                     RCINT nIndex   // index of capability
                     )
{
    if(hdc)
    {
        //qDebug() << "app devicePixelRatio" <<   app->devicePixelRatio();
        //qDebug() << "widget devicePixelRatio" <<   ((QWidget*)hdc)->devicePixelRatio();
        //qDebug() << "widget devicePixelRatioF" <<   ((QWidget*)hdc)->devicePixelRatioF();
        //qDebug() << "physicalDpiX" <<   ((QWidget*)hdc)->physicalDpiX();// * ((QWidget*)hdc)->devicePixelRatio();
        //qDebug() << "physicalDpiY" <<   ((QWidget*)hdc)->physicalDpiY();//* ((QWidget*)hdc)->devicePixelRatio();
        //qDebug() << "logicalDpiX" <<   ((QWidget*)hdc)->logicalDpiX();//* ((QWidget*)hdc)->devicePixelRatio();
        //qDebug() << "logicalDpiY" <<   ((QWidget*)hdc)->logicalDpiY();//* ((QWidget*)hdc)->devicePixelRatio();
        //qDebug() << "width" <<   ((QWidget*)hdc)->width();
        //qDebug() << "height" <<   ((QWidget*)hdc)->height();
        //qDebug() << "widthMM" <<   ((QWidget*)hdc)->widthMM();
        //qDebug() << "heightMM" <<   ((QWidget*)hdc)->heightMM();

        switch(nIndex)
        {
        case LOGPIXELSX : // Logical pixels/inch in X
            return ((QWidget*)hdc)->logicalDpiX();// * ((QWidget*)hdc)->devicePixelRatio();
        case LOGPIXELSY : // Logical pixels/inch in Y
            return ((QWidget*)hdc)->logicalDpiY();// * ((QWidget*)hdc)->devicePixelRatio();
        case HORZRES :
            return ((QWidget*)hdc)->width();//* ((QWidget*)hdc)->devicePixelRatio();
        case VERTRES :
            return ((QWidget*)hdc)->height();//* ((QWidget*)hdc)->devicePixelRatio();
        case HORZSIZE :
            return ((QWidget*)hdc)->widthMM();// * ((QWidget*)hdc)->devicePixelRatio();
        case VERTSIZE :
            return ((QWidget*)hdc)->heightMM();// * ((QWidget*)hdc)->devicePixelRatio();
        }
    }
    else
        return 0;
}

RCCOLORREF RCSETTEXTCOLOR( RCHDC hdc,           // handle to DC
                           RCCOLORREF crColor   // text color
                           )
{
    return 0;
}

RCINT RCSETBKMODE( RCHDC hdc,      // handle to DC
                   RCINT iBkMode   // background mode
                   )
{
    return 0;
}

RCCOLORREF RCSETBKCOLOR( RCHDC hdc,           // handle to DC
                         RCCOLORREF crColor   // background color value
                         )
{
    if(hdc)
    {
        QPalette palette(((QWidget*)hdc)->palette());
        QColor oldcol = palette.color(QPalette::Background);
        palette.setColor(QPalette::Background,QColor(RCGetRValue(crColor),
                                                 RCGetGValue(crColor),
                                                 RCGetBValue(crColor)));
        ((QWidget*)hdc)->setAutoFillBackground(true);
        ((QWidget*)hdc)->setPalette(palette);
        ((QWidget*)hdc)->show();
        RCCOLORREF oldcolref = RGB(oldcol.red(),oldcol.green(),oldcol.blue());
        return oldcolref;
    }
    return 0;
}

RCUINT RCGETOUTLINETEXTMETRICS( RCHDC hdc,                    // handle to DC
                                RCUINT cbData,                // size of metric data array
                                RCLPOUTLINETEXTMETRIC lpOTM   // metric data array
                                )
{
    return 0;
}

RCBOOL RCGETTEXTEXTENTPOINT32( RCHDC hdc,           // handle to DC
                               RCLPCTSTR lpString,  // text string
                               RCINT c,      // characters in string
                               RCLPSIZE lpSize      // string size
                               )
{
    if(hdc)
    {
        QString qstring(lpString,c);
        QRect qrect = ((QWidget*)hdc)->fontMetrics().boundingRect(qstring);
        if(lpSize)
        {
            lpSize->cx = qrect.width();
            lpSize->cy = qrect.height();
        }
        return TRUE;
    }
    return FALSE;
}

RCBOOL RCGETTEXTEXTENTPOINT32W( RCHDC hdc,           // handle to DC
                               RCLPCWSTR lpString,  // text string
                               RCINT c,      // characters in string
                               RCLPSIZE lpSize      // string size
                               )
{
    return RCGETTEXTEXTENTPOINT32(hdc,(RCLPCTSTR)lpString,c,lpSize);
}

RCBOOL RCGETTEXTEXTENTPOINT( RCHDC hdc,           // handle to DC
                             RCLPCTSTR lpString,  // text string
                             RCINT cbString,      // number of characters in string
                             RCLPSIZE lpSize      // string size
                             )
{
    return 0;//return GetTextExtentPoint( hdc, lpString, cbString, lpSize );
}

RCBOOL RCGETTEXTEXTENT( RCHDC  hdc,           // handle to DC
                        const RCCHAR* lpString,  // text string
                        RCINT  c      // characters in string
                             )
{
    return 0;//return GetTextExtentPoint( hdc, lpString, cbString, lpSize );
}

RCBOOL RCTEXTOUTW( RCHDC hdc,           // handle to DC
                   RCINT nXStart,       // x-coordinate of starting position
                   RCINT nYStart,       // y-coordinate of starting position
                   RCLPCTSTR lpString,  // character string
                   RCINT cbString       // number of characters
                   )
{
    return true;
}

RCBOOL RCTEXTOUT( RCHDC hdc,           // handle to DC
                   RCINT nXStart,       // x-coordinate of starting position
                   RCINT nYStart,       // y-coordinate of starting position
                   RCLPCTSTR lpString,  // character string
                   RCINT cbString       // number of characters
                   )
{
    return true;
}
RCINT RCSETPOLYFILLMODE( RCHDC hdc,            // handle to device context
                         RCINT iPolyFillMode   // polygon fill mode
                         )
{
    return 0;
}

RCBOOL RCPOLYPOLYGON( RCHDC hdc,                  // handle to DC
                      CONST RCPOINT *lpPoints,    // array of vertices
                      CONST RCINT *lpPolyCounts,  // array of count of vertices
                      RCINT nCount                // count of polygons
                      )
{
    return true;
}

RCCOLORREF RCSETPIXEL ( RCHDC hdc,           // handle to DC
                        RCINT X,             // x-coordinate of pixel
                        RCINT Y,             // y-coordinate of pixel
                        RCCOLORREF crColor   // pixel color
                        )
{
    return 0;
}

RCBOOL RCGETWINDOWRECT( RCHWND hWnd,
                        RCLPRECT lpRect
                        )
{
    if(hWnd)
    {
        QRect qrect = ((QWidget*)hWnd)->frameGeometry();
        QPoint sPoint = qrect.topLeft();
        if( ! ((QWidget*)hWnd)->isWindow())
            sPoint = ((QWidget*)hWnd)->mapToGlobal(qrect.topLeft());
        lpRect->left = sPoint.x();
        lpRect->top =  sPoint.y();
        lpRect->bottom = qrect.bottom();
        lpRect->right = qrect.right();
        return true;
    }
    return false;
}

RCHWND RCGETPARENT( RCHWND hWnd )
{
    return (RCHWND*)((QWidget*)hWnd)->parent();
}

RCHDC RCGETDC( RCHWND hWnd   // handle to window
               )
{
    if( hWnd )
        return hWnd;
    else
        return app->desktop();
}

RCINT RCRELEASEDC( RCHWND hWnd,  // handle to window
                   RCHDC hDC     // handle to DC
                   )
{
    return 0;
}

RCHDC RCCREATECOMPATIBLEDC( RCHDC hdc   // handle to DC
                           )
{
    return 0;
}

RCHBITMAP RCCREATECOMPATIBLEBITMAP( RCHDC hdc,        // handle to DC
                                    RCINT nWidth,     // width of bitmap, in pixels
                                    RCINT nHeight     // height of bitmap, in pixels
                                    )
{
    //_rcpixmap *qpmap = new _rcpixmap(nWidth, nHeight);
    return (RCHBITMAP)NULL;
}

RCBOOL RCDELETEDC( RCHDC hdc   // handle to DC
                   )
{
    return true;
}

RCINT RCGETDIBITS( RCHDC hdc,           // handle to DC
                   RCHBITMAP hbmp,      // handle to bitmap
                   RCUINT uStartScan,   // first scan line to set
                   RCUINT cScanLines,   // number of scan lines to copy
                   RCLPVOID lpvBits,    // array for bitmap bits
                   RCLPBITMAPINFO lpbi, // bitmap data buffer
                   RCUINT uUsage        // RGB or palette index
                   )
{
    return 0;
}

RCCOLORREF RCGETPIXEL( RCHDC hdc,    // handle to DC
                       RCINT nXPos,  // x-coordinate of pixel
                       RCINT nYPos   // y-coordinate of pixel
                       )
{
    return 0;
}

RCBOOL RCSETWINDOWTEXT( RCHWND hWnd,
                        RCLPCTSTR lpString
                        )
{
    if( hWnd)
    {
        qDebug() << "lpString : " << QString(lpString);
        // add object types as required
        if(qobject_cast<QLineEdit *>((QWidget*)hWnd) != 0)
            ((QLineEdit*)hWnd)->setText(QString(lpString));
        else if(qobject_cast<QTextEdit *>((QWidget*)hWnd) != 0)
            ((QTextEdit*)hWnd)->setText(QString(lpString));
        else
            ((QWidget*)hWnd)->setWindowTitle(QString(lpString));

        return TRUE;
    }
    else
        return FALSE;
}

RCBOOL RCSETMENU( RCHWND hWnd,
                  RCHMENU hMenu
                  )
{
    return true;
}

RCHMENU RCGETMENU( RCHWND hWnd )
{
    if(hWnd)
    {
        if(qobject_cast<QtMainWindow *>((QObject*)hWnd) != 0)
        {
            return ((QtMainWindow*)hWnd)->menuBar;
        }
    }
    return 0;
}

RCHMENU RCGETSUBMENU( RCHMENU hMenu, int nPos )
{
    return 0;
}

RCBOOL RCREMOVEMENU( RCHMENU hMenu,
                     RCUINT uPosition,
                     RCUINT uFlags )
{
    return true;
}

RCBOOL RCAPPENDMENU( RCHMENU hMenu,
                     RCUINT uFlags,
                     RCUINT_PTR uIDNewItem,
                     RCLPCTSTR lpNewItem )
{
    return true;
}

RCINT RCGETCLIPRGN( RCHDC hdc,    // handle to DC
                    RCHRGN hrgn   // handle to region
                   )
{
    return 0;
}

RCINT RCINTERSECTCLIPRGN( RCHDC hdc,         // handle to DC
                          RCINT nLeftRect,   // x-coord of upper-left corner
                          RCINT nTopRect,    // y-coord of upper-left corner
                          RCINT nRightRect,  // x-coord of lower-right corner
                          RCINT nBottomRect  // y-coord of lower-right corner
                          )
{
    return 0;
}

RCINT RCSELECTCLIPRGN( RCHDC hdc,    // handle to DC
                       RCHRGN hrgn   // handle to region
                      )
{
    return 0;
}

RCBOOL RCPTVISIBLE( RCHDC hdc,  // handle to DC
                    RCINT X,    // x-coordinate of point
                    RCINT Y     // y-coordinate of point
                    )
{
    return true;
}

/*! Fill a rectangle using a pointer to an RHDC
 * The RCHDC argument is a pointer to a QPainter object
 */
RCINT RCFILLRECT( RCHDC hDC,           // handle to DC
                  CONST RCRECT *lprc,  // rectangle
                  RCHBRUSH hbr         // handle to brush
                 )
{
    // do not use outside a widget paint function
    // KMJ : not good
    /*
    if(hDC && lprc && hbr)
    {
        QPainter painter;
        painter.begin((QWidget*)hDC);
        RCFILLRECT(&painter,lprc,hbr);
        painter.end();
        return 1;
    }
    */
    return 0;
}

/*! Overloaded version of RCFILLRECT using a Qt painter
 * The RCHDC argument is a pointer to a QPainter object
 */
RCINT RCFILLRECT( QPainter *hDC,       // handle to a QTpainter
                  CONST RCRECT *lprc,  // rectangle
                  RCHBRUSH hbr         // handle to a brush
                 )
{
    // do not use outside a widget paint function
    if(hDC && lprc && hbr)
    {
        QBrush qhbr = *(QBrush*)((RCHBRUSH)hbr);
        ((QPainter*)hDC)->fillRect(QRect(lprc->left,lprc->top,abs(lprc->right-lprc->left),abs(lprc->bottom-lprc->top)),qhbr);
        return 1;
    }
    return 0;
}

RCBOOL RCGETTEXTMETRICS( RCHDC hdc,            // handle to DC
                         RCLPTEXTMETRIC lptm   // text metrics
                         )
{
    if(hdc)
    {
        QFontMetrics qfm = ((QWidget*)hdc)->fontMetrics();
        lptm->tmAscent = qfm.ascent();
        lptm->tmAveCharWidth = qfm.averageCharWidth();
        //lptm->tmBreakChar = ' ';
        lptm->tmCharSet = ANSI_CHARSET;
        lptm->tmDescent = qfm.descent();
        lptm->tmDigitizedAspectX = 1;
        lptm->tmDigitizedAspectY = 1;
        lptm->tmExternalLeading = qfm.leading();
        //lptm->tmFirstChar =
        lptm->tmHeight = qfm.height();
        lptm->tmInternalLeading = 0;
        lptm->tmItalic = ((QWidget*)hdc)->fontInfo().italic();
        //lptm->tmLastChar
        lptm->tmMaxCharWidth = qfm.maxWidth();
        //lptm->tmOverhang
        //lptm->tmPitchAndFamily = ((QWidget*)hdc)->fontInfo().family() | ((QWidget*)hdc)->fontInfo().fixedPitch();
        lptm->tmStruckOut = ((QWidget*)hdc)->fontInfo().strikeOut();
        lptm->tmUnderlined = ((QWidget*)hdc)->fontInfo().underline();
        lptm->tmWeight = ((QWidget*)hdc)->fontInfo().weight();
        return TRUE;
    }
    return FALSE;
}


RCBOOL RCGETTEXTMETRICSW( RCHDC hdc,            // handle to DC
                          RCLPTEXTMETRICW lptm   // text metrics
                         )
{
    return true;
}

RCUINT_PTR RCSETTIMER( RCHWND hWnd,
                       RCUINT_PTR nIDEvent,
                       RCUINT uElapse,
                       RCTIMERPROC lpTimerFunc
                       )
{
    return 0;
}

RCHWND RCSETCLIPBOARDVIEWER( RCHWND hWndNewViewer )
{
    return 0;
}

RCVOID RCDRAGACCEPTFILES( RCHWND hWnd, RCBOOL fAccept )
{
    return;
}

RCDWORD RCSETCLASSLONG( RCHWND hWnd, RCINT nIndex, RCLONG dwNewLong )
{
    return 0;
}

RCBOOL RCKILLTIMER( RCHWND hWnd, RCUINT_PTR uIDEvent )
{
   return  0;//KillTimer( hWnd, uIDEvent );
}

RCBOOL SetWindowPos( RCHWND hWnd,
                     RCHWND hWndInsertAfter,
                     RCINT X,
                     RCINT Y,
                     RCINT cx,
                     RCINT cy,
                     RCUINT uFlags
                     )
{
    if(hWnd){
        ((QWidget*)hWnd)->setGeometry(X,Y,cx,cy);
        return true;
    }
    return false;
    //SetWindowPos( hWnd, hWndInsertAfter, X,  Y, cx, cy,  uFlags);
}

RCUINT RCSETTEXTALIGN( RCHDC hdc,     // handle to DC
                       RCUINT fMode   // text-alignment option
                       )
{
    return 0;//SetTextAlign( hdc, fMode );
}

RCBOOL RCOPENCLIPBOARD( RCHWND hWndNewOwner )
{
    return  0;//OpenClipboard( hWndNewOwner );
}

RCBOOL RCSETSCROLLRANGE( RCHWND hWnd,
                         RCINT nBar,
                         RCINT nMinPos,
                         RCINT nMaxPos,
                         RCBOOL bRedraw
                        )
{
    if(hWnd)
    {
            switch(nBar)
            {
            case SB_CTL:
                if(qobject_cast<QSlider *>((QWidget*)hWnd) != 0)
                {
                    ((QSlider*)hWnd)->setMinimum(nMinPos);
                    ((QSlider*)hWnd)->setMaximum(nMaxPos);
                    //if(bRedraw)
                    //    ((QSlider*)hWnd)->update();
                    return true;
                }
                else if(qobject_cast<QScrollBar *>((QWidget*)hWnd) != 0)
                {
                    ((QScrollBar*)hWnd)->setMinimum(nMinPos);
                    ((QScrollBar*)hWnd)->setMaximum(nMaxPos);
                    if(bRedraw)
                        ((QScrollBar*)hWnd)->update();
                    return true;
                }
                break;
            case SB_HORZ:
            case SB_VERT:
                break;
            }
    }
    return 0;//SetScrollRange( hWnd, nBar, nMinPos, nMaxPos, bRedraw);
}

RCBOOL RCINVERTRECT( RCHDC hDC,           // handle to DC
                     CONST RCRECT *lprc   // rectangle
                     )
{
    return 0;//InvertRect( hDC, lprc );
}

RCBOOL RCPOLYGON( RCHDC hdc,                // handle to DC
                  CONST RCPOINT *lpPoints,  // polygon vertices
                  RCINT nCount              // count of polygon vertices
                  )
{
    return 0;//Polygon( hdc, lpPoints, nCount );
}

RCBOOL RCELLIPSE( RCHDC hdc,        // handle to DC
                  RCINT nLeftRect,  // x-coord of upper-left corner of rectangle
                  RCINT nTopRect,   // y-coord of upper-left corner of rectangle
                  RCINT nRightRect, // x-coord of lower-right corner of rectangle
                  RCINT nBottomRect // y-coord of lower-right corner of rectangle
                  )
{
    return 0;Ellipse( hdc, nLeftRect, nTopRect, nRightRect, nBottomRect );
}

RCHWND RCSETCAPTURE( RCHWND hWnd )
{
    QWidget *lastWindow = app->focusWidget();
    ((QWidget*)hWnd)->setFocus();
    return (RCHWND)lastWindow;
    //return SetCapture( hWnd );
}

RCBOOL RCGETSCROLLRANGE( RCHWND hWnd,
                         RCINT nBar,
                         RCLPINT lpMinPos,
                         RCLPINT lpMaxPos
                         )
{
    if(hWnd)
    {
            switch(nBar)
            {
            case SB_CTL:
                if(qobject_cast<QSlider *>((QWidget*)hWnd) != 0)
                {
                    *lpMinPos = ((QSlider*)hWnd)->minimum();
                    *lpMaxPos = ((QSlider*)hWnd)->maximum();
                    return true;
                }
                else if(qobject_cast<QScrollBar *>((QWidget*)hWnd) != 0)
                {
                    *lpMinPos = ((QScrollBar*)hWnd)->minimum();
                    *lpMaxPos = ((QScrollBar*)hWnd)->maximum();
                    return true;
                }
                break;
            case SB_HORZ:
            case SB_VERT:
                break;
            }
    }
    return 0;
    //GetScrollRange( hWnd, nBar, lpMinPos, lpMaxPos );
}

RCINT RCSETSCROLLPOS( RCHWND hWnd,
                      RCINT nBar,
                      RCINT nPos,
                      RCBOOL bRedraw
                      )
{
    if(hWnd)
    {
            switch(nBar)
            {
            case SB_CTL:
                if(qobject_cast<QSlider *>((QWidget*)hWnd) != 0)
                {
                    int oldpos = ((QSlider*)hWnd)->value();
                    ((QSlider*)hWnd)->setValue(nPos);
                    return oldpos;
                }
                else if(qobject_cast<QScrollBar *>((QWidget*)hWnd) != 0)
                {
                    int oldpos = ((QScrollBar*)hWnd)->value();
                    ((QScrollBar*)hWnd)->setValue(nPos);
                    return oldpos;
                }
                break;
                break;
            case SB_HORZ:
            case SB_VERT:
                break;
            }
    }
    return 0;
    //SetScrollPos( hWnd, nBar, nPos, bRedraw );
}

RCBOOL RCGETWINDOWPLACEMENT( RCHWND hWnd, RCWINDOWPLACEMENT *lpwndpl )
{
    if(hWnd)
    {
        QRect geom;
        POINT wMaxPos,wMinPos;
        RECT wrect;

        if( hWnd && ((QWidget*)hWnd)->isWindow() && !(((QWidget*)hWnd)->windowFlags() & Qt::Tool) )
        {
            // positions in workspace (client) coordinates
            geom = ((QWidget*)hWnd)->frameGeometry();
            wMinPos.x = geom.x();
            wMinPos.y = geom.y();
            wMaxPos.x = geom.right();
            wMaxPos.y = geom.bottom();
            wrect.left = geom.left();
            wrect.top = geom.top();
            wrect.right = geom.right();
            wrect.bottom = geom.bottom();
        }
        else
        {
            // positions in screen coordinates
            if(hWnd && ((QWidget*)hWnd)->windowState() & Qt::WindowMaximized)
            {
                geom = ((QWidget*)hWnd)->normalGeometry();
                wMinPos.x = geom.x();
                wMinPos.y = geom.y();
                wMaxPos.x = geom.right();
                wMaxPos.y = geom.bottom();
                wrect.left = geom.x();
                wrect.top = geom.y();
                wrect.right = geom.right();
                wrect.bottom = geom.bottom();
            }
            else
            {
                geom = ((QWidget*)hWnd)->frameGeometry();
                wMinPos.x = ((QWidget*)hWnd)->pos().x();
                wMinPos.y = ((QWidget*)hWnd)->pos().y();
                wMaxPos.x = geom.right();
                wMaxPos.y = geom.bottom();
                wrect.left = ((QWidget*)hWnd)->pos().x();
                wrect.top = ((QWidget*)hWnd)->pos().y();
                wrect.right = geom.right();
                wrect.bottom = geom.bottom();
            }
        }

        lpwndpl->length = sizeof(RCWINDOWPLACEMENT);
        lpwndpl->ptMaxPosition = wMaxPos;
        lpwndpl->ptMinPosition = wMinPos;
        lpwndpl->rcNormalPosition = wrect;
        lpwndpl->showCmd = ((QWidget*)hWnd)->windowState();
        switch(((QWidget*)hWnd)->windowState())
        {
        case Qt::WindowMinimized :
            lpwndpl->showCmd = SW_MINIMIZE;
            break;
        case Qt::WindowMaximized :
            lpwndpl->showCmd = SW_MAXIMIZE;
            break;
        case Qt::WindowFullScreen :
            lpwndpl->showCmd = ((QWidget*)hWnd)->windowState();
            break;
        case Qt::WindowActive :
        case Qt::WindowNoState :
        default:
            lpwndpl->showCmd = SW_SHOW;
        }

        return TRUE;
    }
    return  FALSE;
    //GetWindowPlacement(hWnd, lpwndpl);
}

RCLONG RCSETWINDOWLONG( RCHWND hWnd, RCINT nIndex, RCLONG dwNewLong )
{
    return 0;//SetWindowLong( hWnd, nIndex, dwNewLong );
}

RCBOOL RCSETWINDOWPLACEMENT( RCHWND hWnd, RCWINDOWPLACEMENT *lpwndpl )
{
    return 0;//SetWindowPlacement( hWnd, lpwndpl );
}

RCLONG RCGETWINDOWLONG( RCHWND hWnd, RCINT nIndex )
{
    return 0;//GetWindowLong( hWnd, nIndex );
}

RCHWND RCGETWINDOW( RCHWND hWnd, RCUINT uCmd )
{
    return 0;//GetWindow( hWnd, uCmd);
}

RCBOOL RCCLIENTTOSCREEN( RCHWND hWnd,       // handle to window
                         RCLPPOINT lpPoint  // screen coordinates
                         )
{
    if(hWnd)
    {
        //qDebug() << "RCCLIENTTOSCREEN window name : " << (QWidget*)hWnd;
        QPoint qp = ((QWidget*)hWnd)->mapToGlobal(QPoint(lpPoint->x,lpPoint->y));
        lpPoint->x = qp.x(); lpPoint->y = qp.y();
    }
    return 0;//ClientToScreen( hWnd, lpPoint );
}

RCBOOL RCSCREENTOCLIENT( RCHWND hWnd,        // handle to window
                         RCLPPOINT lpPoint   // screen coordinates
                         )
{
    if(hWnd)
    {
        QPoint qp = ((QWidget*)hWnd)->mapFromGlobal(QPoint(lpPoint->x,lpPoint->y));
        lpPoint->x = qp.x(); lpPoint->y = qp.y();
    }
    return 0;//ScreenToClient( hWnd, lpPoint );
}

RCLRESULT RCSENDMESSAGE( RCHWND hWnd,
                         RCUINT Msg,
                         RCWPARAM wParam,
                         RCLPARAM lParam
                         )
{
    if(hWnd)
    {
        switch(Msg)
        {
        case LB_GETSELCOUNT :
            if( qobject_cast<QListWidget *>((QObject*)hWnd) != 0 )
            {
                if(((QListWidget*)hWnd)->selectionMode() == QAbstractItemView::SingleSelection ||
                   ((QListWidget*)hWnd)->selectionMode() == QAbstractItemView::NoSelection)
                    return LB_ERR;
                return ((QListWidget*)hWnd)->selectedItems().size();
            }
            break;
        case LB_GETSELITEMS :
            if( qobject_cast<QListWidget *>((QObject*)hWnd) != 0 )
            {
                if(((QListWidget*)hWnd)->selectionMode() == QAbstractItemView::SingleSelection ||
                   ((QListWidget*)hWnd)->selectionMode() == QAbstractItemView::NoSelection)
                    return LB_ERR;
                int i,cnt=0,*items = (int*)lParam;
                for(i=0; i<((QListWidget*)hWnd)->count(); i++)
                {
                    if(!((QListWidget*)hWnd)->item(i)->isHidden() &&
                        ((QListWidget*)hWnd)->item(i)->isSelected())
                    {
                        items[cnt] = i;
                        cnt++;
                    }
                }
                return cnt;
            }
            break;
        case LB_GETTEXT :
            if( qobject_cast<QListWidget *>((QObject*)hWnd) != 0 )
            {
                int index = (int)wParam;
                if(index >= 0 && index <= ((QListWidget*)hWnd)->count())
                {
                    QListWidgetItem *item = ((QListWidget*)hWnd)->item(index);
                    QString text = item->text();
                    strcpy((RCLPSTR)lParam,text);
                    return strlen((RCLPSTR)lParam);
                }
                return (RCLRESULT)LB_ERR;
            }
            break;
        case WM_NCACTIVATE :
            //qDebug() << ((QObject*)hWnd)->objectName();
            if(qobject_cast<RCView *>((QObject*)hWnd) != 0)
            {
                if(wParam)
                    ((RCCentralWidget*)((RCView*)hWnd)->centralWidget())->setActiveWindow((RCView*)hWnd);
                else
                    ((RCCentralWidget*)((RCView*)hWnd)->centralWidget())->unsetActiveWindow((RCView*)hWnd);
            }
            else if( qobject_cast<QGraphicsView *>((QObject*)hWnd) != 0 )
            {
                //qDebug() << (QWidget*)cadwindow->getsubwindow()->gethwnd();
                if(qobject_cast<QTabWidget *>((QWidget*)cadwindow->getsubwindow()->gethwnd()) != 0 )
                {
                    QTabWidget *qtbw = (QTabWidget*)cadwindow->getsubwindow()->gethwnd();
                    int tabid = qtbw->indexOf((QWidget*)hWnd);
                    if(qtbw->currentIndex() != tabid && wParam == 1)
                    {
                        ((QTabWidget*)cadwindow->getsubwindow()->gethwnd())->setCurrentIndex(tabid);
                        ((QGraphicsView*)hWnd)->scene()->update();
                    }
                }
                else if(qobject_cast<QMdiArea *>((QWidget*)cadwindow->getsubwindow()->gethwnd()) != 0 )
                {
                    QMdiArea *qmda = (QMdiArea*)cadwindow->getsubwindow()->gethwnd();
                    QMdiSubWindow *qsbw = (QMdiSubWindow*)((QGraphicsView*)hWnd)->parentWidget();
                    //qDebug() << "qsbw       : " << qsbw;
                    //qDebug() << "active sub : " << qmda->activeSubWindow();
                    qmda->setActiveSubWindow(qsbw);
                    ((QGraphicsView*)hWnd)->scene()->update();
                }
            }
            break;
        }
    }
    return 0;//SendMessage( hWnd, Msg, wParam, lParam );
}

RCLRESULT RCSENDMESSAGEW( RCHWND hWnd,
                          RCUINT Msg,
                          RCWPARAM wParam,
                          RCLPARAM lParam
                          )
{
    return 0;//SendMessageW( hWnd, Msg, wParam, lParam );
}

RCHWND RCGETFOCUS(void)
{
    return app->focusWidget();
    //GetFocus();
}

RCHWND RCSETFOCUS( RCHWND hWnd )
{
    RCHWND result=0;
    if(hWnd)
    {
        result = app->focusWidget();
        ((QWidget*)hWnd)->setFocus(Qt::NoFocusReason);
    }
    return result;
    //SetFocus( hWnd );
}

RCHWND RCCHILDWINDOWFROMPOINT( RCHWND hWndParent, RCPOINT Point )
{
    RCHWND result=0;
    if(hWndParent)
    {
        result = ((QWidget*)hWndParent)->childAt(Point.x,Point.y);
    }
    return result;
    //ChildWindowFromPoint( hWndParent, Point );
}

RCBOOL RCPEEKMESSAGE( RCLPMSG lpMsg,
                      RCHWND hWnd,
                      RCUINT wMsgFilterMin,
                      RCUINT wMsgFilterMax,
                      RCUINT wRemoveMsg
                      )
{
    return 0;//PeekMessage( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg );
}

RCBOOL RCISICONIC( RCHWND hWnd )
{
    if(hWnd)
    {
        if(((QWidget*)hWnd)->isWindow())
        {
            if(((QWidget*)hWnd)->isMinimized() || !((QWidget*)hWnd)->isVisible())
                return 1;
            else
                return 0;
        }
    }
    return 0;//IsIconic( hWnd );
}

RCBOOL RCBEGINPATH( RCHDC hdc )
{
    return 0;//BeginPath( hdc );
}

RCBOOL RCENDPATH(  RCHDC hdc  )
{
    return 0;//EndPath( hdc  );
}

RCINT RCSETSTRETCHBLTMODE(  RCHDC hdc,           // handle to DC
                            RCINT iStretchMode   // bitmap stretching mode
                            )
{
    return  0;//SetStretchBltMode( hdc, iStretchMode );
}

RCBOOL RCSTRETCHBLT(  RCHDC hdcDest,      // handle to destination DC
                      RCINT nXOriginDest, // x-coord of destination upper-left corner
                      RCINT nYOriginDest, // y-coord of destination upper-left corner
                      RCINT nWidthDest,   // width of destination rectangle
                      RCINT nHeightDest,  // height of destination rectangle
                      RCHDC hdcSrc,       // handle to source DC
                      RCINT nXOriginSrc,  // x-coord of source upper-left corner
                      RCINT nYOriginSrc,  // y-coord of source upper-left corner
                      RCINT nWidthSrc,    // width of source rectangle
                      RCINT nHeightSrc,   // height of source rectangle
                      RCDWORD dwRop       // raster operation code
                      )
{
    return 0;
    /*StretchBlt( hdcDest,      // handle to destination DC
       nXOriginDest, // x-coord of destination upper-left corner
       nYOriginDest, // y-coord of destination upper-left corner
       nWidthDest,   // width of destination rectangle
       nHeightDest,  // height of destination rectangle
       hdcSrc,       // handle to source DC
       nXOriginSrc,  // x-coord of source upper-left corner
       nYOriginSrc,  // y-coord of source upper-left corner
       nWidthSrc,    // width of source rectangle
       nHeightSrc,   // height of source rectangle
       dwRop       // raster operation code
        );
        */
}

RCBOOL RCSELECTCLIPPATH( RCHDC hdc,    // handle to DC
                         RCINT iMode   // clipping mode
                         )
{
    return 0;//SelectClipPath( hdc, iMode );
}

RCINT RCEXTSELECTCLIPRGN( RCHDC hdc,  // handle to DC
                          RCHRGN hrgn, // handle to region
                          RCINT fnMode        // region-selection mode
                          )
{
    return 0;//ExtSelectClipRgn( hdc, hrgn, fnMode );
}

RCBOOL RCRECTANGLE( RCHDC hdc,         // handle to DC
                    RCINT nLeftRect,   // x-coord of upper-left corner of rectangle
                    RCINT nTopRect,    // y-coord of upper-left corner of rectangle
                    RCINT nRightRect,  // x-coord of lower-right corner of rectangle
                    RCINT nBottomRect  // y-coord of lower-right corner of rectangle
                    )
{
    return 0;//Rectangle(  hdc, nLeftRect, nTopRect, nRightRect, nBottomRect );
}

RCBOOL RCENABLEWINDOW( RCHWND hWnd, RCBOOL bEnable )
{
    bool result=false;
    if(hWnd)
    {
        result = ((QWidget*)hWnd)->isEnabled();
        ((QWidget*)hWnd)->setEnabled((bool)bEnable);
    }
    return result;
    //EnableWindow( hWnd, bEnable );
}

RCBOOL RCPOSETMESSAGE( RCHWND hWnd, RCUINT Msg, RCWPARAM wParam, RCLPARAM lParam )
{
    RCBOOL status = FALSE;
    if(hWnd)
    {
        if( qobject_cast<RCDialog *>((QObject*)hWnd) != 0 )
        {
            if(Msg == WM_COMMAND && wParam == IDOK)
            {
                ((QPushButton*)((RCDialog*)hWnd)->getControl(1))->click();
                status = TRUE;
            }
            else if(Msg == WM_COMMAND && wParam == IDCANCEL)
            {
                ((QPushButton*)((RCDialog*)hWnd)->getControl(2))->click();
                status = TRUE;
            }
        }
     }
    return status;//PostMessage( hWnd, Msg, wParam, lParam );
}

RCINT RCGETWINDOWTEXT( RCHWND hWnd, RCLPTSTR lpString, RCINT nMaxCount )
{
    if(hWnd)
    {
        //qDebug() << "RCGETWINDOWTEXT : object " << ((QWidget*)hWnd)->objectName();
        //if( ((QWidget*)hWnd)->objectName().contains("QLineEdit") )
        if( qobject_cast<QLineEdit *>((QObject*)hWnd) != 0 )
        {
            //qDebug() << "RCGETWINDOWTEXT : text " << ((QLineEdit*)hWnd)->text();
            strncpy(lpString,((QLineEdit*)hWnd)->text().constData(),nMaxCount);
            return strlen(lpString);
        }
        else if( qobject_cast<QPlainTextEdit *>((QObject*)hWnd) != 0 )
        {
            //qDebug() << "RCGETWINDOWTEXT : text " << ((QPlainTextEdit*)hWnd)->toPlainText();
            strncpy(lpString,((QPlainTextEdit*)hWnd)->toPlainText().constData(),nMaxCount);
            return strlen(lpString);
        }
        else if( qobject_cast<QTextEdit *>((QObject*)hWnd) != 0 )
        {
            //qDebug() << "RCGETWINDOWTEXT : text " << ((QTextEdit*)hWnd)->toPlainText();
            strncpy(lpString,((QTextEdit*)hWnd)->toPlainText().constData(),nMaxCount);
            return strlen(lpString);
        }
        //else if( ((QWidget*)hWnd)->objectName() == "QLabel"     )
        else if( qobject_cast<QLabel *>((QObject*)hWnd) != 0 )
        {
            //qDebug() << "RCGETWINDOWTEXT : text " << ((QLabel*)hWnd)->text();
            strncpy(lpString,((QLabel*)hWnd)->text().constData(),nMaxCount);
            return strlen(lpString);
        }
        else
        {
            //qDebug() << "RCGETWINDOWTEXT : text " << ((QLabel*)hWnd)->text();
            strncpy(lpString,((QWidget*)hWnd)->windowTitle().constData(),nMaxCount);
            return strlen(lpString);
        }
    }
    return 0;
}

RCINT RCGETWINDOWTEXTW( RCHWND hWnd, RCLPTSTR lpString, RCINT nMaxCount )
{
    return RCGETWINDOWTEXT(hWnd,lpString,nMaxCount) ;
}

RCBOOL IsWindow( RCHWND hWnd )
{
    //qDebug() << "Widget Type : " << ((QWidget*)hWnd)->windowType();
    if( (hWnd && ((QWidget*)hWnd)->windowType() >= 0  ) ||
        (hWnd && ((QWidget*)hWnd)->windowType() <= 31 ) )
    {
        //qDebug() << "hWnd is a QWidget or QWindow: ";
        return TRUE;
    }
    else
    {
        //qDebug() << "hWnd is not a QWidget or QWindow: ";
        return FALSE;
    }
}

RCHWND RCSETPARENT( RCHWND hWndChild, RCHWND hWndNewParent )
{
    return 0;//SetParent( hWndChild, hWndNewParent );
}

RCBOOL RCREDRAWWINDOW( RCHWND hWnd,               // handle to window
                       CONST RCRECT *lprcUpdate,  // update rectangle
                       RCHRGN hrgnUpdate,         // handle to update region
                       RCUINT flags               // array of redraw flags
                       )
{
    return 0;//RedrawWindow( hWnd, lprcUpdate, hrgnUpdate, flags );
}

RCBOOL RCENABLEMENUITEM( RCHMENU hMenu, RCUINT uIDEnableItem, RCUINT uEnable )
{
    return 1;//EnableMenuItem( hMenu, uIDEnableItem, uEnable );
}

RCHWND  RCHTMLHELPA(
                   RCHWND    hwndCaller,
                   RCLPCSTR  pszFile,
                   RCUINT    uCommand,
                   RCDWORD   dwData )
{
    return 0;//HtmlHelpA( hwndCaller, pszFile, uCommand, dwData );
}

BOOL RCWINHELPA( RCHWND hWndMain,
                RCLPCTSTR lpszHelp,
                RCUINT uCommand,
                RCULONG_PTR dwData
                )
{
    return 0;//WinHelpA( hWndMain, lpszHelp, uCommand, dwData );
}

RCINT RCTRANSLATEACCELERATOR( RCHWND hWnd, RCHACCEL hAccTable, RCLPMSG lpMsg )
{
    return 0;//TranslateAccelerator( hWnd, hAccTable, lpMsg );
}

RCBOOL RCTRANSLATEMESSAGE( const RCMSG *lpMsg )
{
    return 0;//TranslateMessage( lpMsg );
}

RCLRESULT RCDISPATCHMESSAGE( const RCMSG *lpmsg )
{
    //app->processEvents();
    return 0;//DispatchMessage( lpmsg );
}

RCBOOL RCGETMESSAGE( RCLPMSG lpMsg,
                     RCHWND hWnd,
                     RCUINT wMsgFilterMin,
                     RCUINT wMsgFilterMax
                     )
{
    return 1;//GetMessage( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax );
}

RCINT RCGETCLASSNAME( RCHWND hWnd,
                      RCCHAR* lpClassName,
                      RCINT nMaxCount
                      )
{
    return 0;//GetClassName( hWnd, lpClassName, nMaxCount );
}

RCBOOL RCPATBLT( RCHDC hdc,      // handle to DC
                 RCINT nXLeft,   // x-coord of upper-left rectangle corner
                 RCINT nYLeft,   // y-coord of upper-left rectangle corner
                 RCINT nWidth,   // width of rectangle
                 RCINT nHeight,  // height of rectangle
                 RCDWORD dwRop   // raster operation code
                 )
{
    return  0;//PatBlt( hdc, nXLeft, nYLeft, nWidth, nHeight, dwRop );
}

RCINT RCSETDIBITS( RCHDC hdc,                  // handle to DC
                   RCHBITMAP hbmp,             // handle to bitmap
                   RCUINT uStartScan,          // starting scan line
                   RCUINT cScanLines,          // number of scan lines
                   CONST RCVOID *lpvBits,      // array of bitmap bits
                   CONST RCBITMAPINFO *lpbmi,  // bitmap data
                   RCUINT fuColorUse           // type of color indexes to use
                   )
{
    return  0;//SetDIBits( hdc, hbmp, uStartScan, cScanLines, lpvBits, lpbmi, fuColorUse );
}

RCINT RCFRAMERECT( RCHDC hDC,         // handle to DC
                   CONST RCRECT *lprc,  // rectangle
                   RCHBRUSH hbr         // handle to brush
                  )
{
    return 0;//FrameRect( hDC, lprc, hbr );
}

HMONITOR RCMONITORFROMWINDOW( RCHWND hwnd,       // handle to a window
                              RCDWORD dwFlags    // determine return value
                              )
{
    return 0;//MonitorFromWindow( hwnd, dwFlags );
}

RCHRESULT RCPLAYSOUND( RCLPCWSTR pszSoundNamepqc )
{
    return 0;//PlaySound( pszSoundNamepqc );
}

RCBOOL RCPLAYSOUND( RCLPCSTR pszSound, RCHMODULE hmod, RCDWORD fdwSound )
{
    return 0;//PlaySound(  pszSound, hmod, fdwSound );
}

RCDWORD  RCGETMODULEFILENAME( RCHMODULE hModule, RCLPTSTR lpFilename, RCDWORD nSize )
{
    // use Qt methods or platform specific methods
    // qt method
    QString mName;
    if( hModule == 0 ) // get the main apps name
        mName = app->instance()->applicationFilePath();//applicationName();
    else // hModule is a loaded library QLibrary
       mName = ((QLibrary*)hModule)->fileName();
    if( mName.size() > 0 )
    {
        int i=0;
        for( i=0; i<nSize; i++ )
        {
            if( i > mName.size() )
                break;
            *lpFilename = mName[i];
            lpFilename++;
        }
        *lpFilename=0;
        return i;
    }else
        return 0;
    // windows method
    // return GetModuleFileName( hModule, lpFilename, nSize );
    // mac method
}

RCBOOL RCSETDLGITEMTEXT( RCHWND hDlg, RCINT nIDDlgItem, RCLPCTSTR lpString )
{
    if(hDlg)
    {
        QWidget *ditem = (QWidget*)RCGETDLGITEM(hDlg,nIDDlgItem);
        if(ditem)
        {
            QString text(lpString);
            //qDebug() << "in item text" << text;
            if(qobject_cast<QLineEdit *>(ditem) != 0)
            {
                ((QLineEdit*)ditem)->setText(text);
                //qDebug() << "item text after" << ((QLineEdit*)ditem)->text();
            }
            else if(qobject_cast<QTextEdit *>(ditem) != 0)
                ((QTextEdit*)ditem)->setText(text);
            else if(qobject_cast<QPlainTextEdit *>(ditem) != 0)
                ((QPlainTextEdit*)ditem)->setPlainText(text);
            else if(qobject_cast<QLabel *>(ditem)    != 0)
                ((QLabel*)ditem)->setText(text);
            else if(qobject_cast<QComboBox *>(ditem)    != 0)
                ((QComboBox*)ditem)->setCurrentText(text);
            else if(qobject_cast<RCComboList *>(ditem)    != 0)
                ((RCComboList*)ditem)->setCurrentText(text);
            else if(qobject_cast<WeightSpinBox *>(ditem)    != 0)
                ((WeightSpinBox*)ditem)->setValue(text.toDouble());
            else if(qobject_cast<QDoubleSpinBox *>(ditem)    != 0)
                ((QDoubleSpinBox*)ditem)->setValue(text.toDouble());
            else if(qobject_cast<QSpinBox *>(ditem)    != 0)
                ((QSpinBox*)ditem)->setValue(text.toInt());
            else if(qobject_cast<QDateEdit *>(ditem)    != 0)
                ((QDateEdit*)ditem)->setDate(QDate::fromString(text,"yyyy/MM/dd"));
            else if(qobject_cast<QTimeEdit *>(ditem)    != 0)
                ((QTimeEdit*)ditem)->setTime(QTime::fromString(text,"HH:mm:ss"));

            return true;
        }
    }
    return false;
    //SetDlgItemText( hDlg, nIDDlgItem, lpString );
}

RCBOOL RCSETDLGITEMTEXT( RCHWND hDlg, RCINT nIDDlgItem, char * lpString )
{
    return RCSETDLGITEMTEXT(hDlg, nIDDlgItem, QString(lpString).data());
    //SetDlgItemText( hDlg, nIDDlgItem, lpString );
}

RCHWND RCGETDLGITEM( RCHWND hDlg, RCINT nIDDlgItem )
{
    RCHWND result = 0;
    if(hDlg)
    {
        result = ((QWidget*)hDlg)->findChild<QWidget*>(RCintToName(nIDDlgItem));
        //qDebug() << "dlg item : " << (QWidget*)result;
     }
    return result;
    //GetDlgItem( hDlg, nIDDlgItem );
}

RCLRESULT RCSENDITEMMESSAGE(  RCHWND hDlg,
                              RCINT nIDDlgItem,
                              RCUINT Msg,
                              RCWPARAM wParam,
                              RCLPARAM lParam
                              )
{
    if(hDlg)
    {
        QWidget *ditem = (QWidget*)RCGETDLGITEM(hDlg,nIDDlgItem);
        if(ditem)
        {
            switch(Msg)
            {
            case LB_GETCOUNT :
            case CB_GETCOUNT :
            {
                if(ditem)
                {
                    if(qobject_cast<QComboBox *>(ditem) != 0 )
                    {
                        return (RCLRESULT)((QComboBox*)ditem)->count();
                    }
                    else if(qobject_cast<RCComboList *>(ditem) != 0 )
                    {
                        return (RCLRESULT)((RCComboList*)ditem)->count();
                    }
                    else if(qobject_cast<QListWidget*>(ditem) != 0 )
                    {
                        return (RCLRESULT)((QListWidget*)ditem)->count();
                    }
                }
            }
            break;
            case LB_ADDSTRING :
            case CB_ADDSTRING :
            {
                if(ditem)
                {
                    if(qobject_cast<QComboBox *>(ditem) != 0 )
                    {
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        //QStringList qsl(text);
                        //((QComboBox*)ditem)->addItems(qsl);
                        ((QComboBox*)ditem)->addItem(text);
                        return (RCLRESULT)((QComboBox*)ditem)->findText(text);
                    }
                    else if(qobject_cast<RCComboList *>(ditem) != 0 )
                    {
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        ((RCComboList*)ditem)->addItem(text);
                        return (RCLRESULT)((RCComboList*)ditem)->findText(text);
                    }
                    else if(qobject_cast<QListWidget*>(ditem) != 0 )
                    {
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        QListWidgetItem *newItem = new QListWidgetItem;
                        newItem->setText(text);
                        ((QListWidget*)ditem)->addItem(newItem);
                        return (RCLRESULT)((QListWidget*)ditem)->count();
                    }
                }
            }
            break;
            case EM_SETSEL :
            {
                if(ditem)
                {
                    if(qobject_cast<QComboBox *>(ditem) != 0 )
                    {
                    }
                    else if(qobject_cast<QSpinBox *>(ditem) != 0 )
                    {

                    }
                    else if(qobject_cast<QDoubleSpinBox *>(ditem) != 0 )
                    {
                    }
                    else if(qobject_cast<QLineEdit *>(ditem) != 0 )
                    {
                    }
                    else if(qobject_cast<QTextEdit *>(ditem) != 0 )
                    {
                    }
                }
            }
            break;
            case CB_RESETCONTENT:
            case LB_RESETCONTENT:
            {
                if(ditem)
                {
                    if(qobject_cast<QComboBox *>(ditem) != 0 )
                    {
                        ((QComboBox*)ditem)->clear();
                        return 0;
                    }
                    else if(qobject_cast<RCComboList *>(ditem) != 0 )
                    {
                        ((RCComboList*)ditem)->clear();
                        return 0;
                    }
                    else if(qobject_cast<QListWidget *>(ditem) != 0 )
                    {
                        ((QListWidget*)ditem)->clear();
                        return 0;
                    }
                }
            }
            break;
            case CB_FINDSTRINGEXACT:
            {
                if(ditem)
                {
                    if(qobject_cast<QComboBox *>(ditem) != 0 )
                    {
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        return ((QComboBox*)ditem)->findText(text);
                    }
                    else if(qobject_cast<RCComboList *>(ditem) != 0 )
                    {
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        return ((RCComboList*)ditem)->findText(text);
                    }
                }
            }
            break;
            case LB_FINDSTRING:
            {
                if(ditem)
                {
                    if(qobject_cast<QListWidget *>(ditem) != 0 )
                    {
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        QList<QListWidgetItem *> found;
                        found = ((QListWidget*)ditem)->findItems(text,Qt::MatchExactly);
                        if(found.size() > 0)
                            return ((QListWidget*)ditem)->row(found.at(0));
                        else
                            return (RCLRESULT)(-1);
                    }
                    else if(qobject_cast<RCComboList *>(ditem) != 0 )
                    {
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        return ((RCComboList*)ditem)->findText(text);
                    }
                }
            }
            break;
            case CB_DELETESTRING:
            case LB_DELETESTRING:
            {
                if(ditem)
                {
                    if(qobject_cast<QComboBox *>(ditem) != 0 )
                    {
                        int index = (int)wParam;
                        if(index >= 0 && index <= ((QComboBox*)ditem)->count())
                        {
                            ((QComboBox*)ditem)->removeItem(index);
                            return (RCLRESULT)((QComboBox*)ditem)->count();
                        }
                        return (RCLRESULT)(-1);
                    }
                    else if(qobject_cast<RCComboList *>(ditem) != 0 )
                    {
                        int index = (int)wParam;
                        if(index >= 0 && index <= ((RCComboList*)ditem)->count())
                        {
                            ((RCComboList*)ditem)->removeItem(index);
                            return (RCLRESULT)((RCComboList*)ditem)->count();
                        }
                        return (RCLRESULT)(-1);
                    }
                    else if(qobject_cast<QListWidget *>(ditem) != 0 )
                    {
                        int index = (int)wParam;
                        if(index >= 0 && index <= ((QListWidget*)ditem)->count())
                        {
                            QListWidgetItem *item = ((QListWidget*)ditem)->takeItem(index);
                            delete item;
                            return (RCLRESULT)((QListWidget*)ditem)->count();
                        }
                        return (RCLRESULT)(-1);
                    }
                }
            }
            break;
            case CB_SELECTSTRING:
            {
                if(ditem)
                {
                    if(qobject_cast<QComboBox *>(ditem) != 0 )
                    {
                        int index = (int)wParam;
                        if(index >= 0 && index <= ((QComboBox*)ditem)->count())
                        {
                            ((QComboBox*)ditem)->removeItem(index);
                            return (RCLRESULT)((QComboBox*)ditem)->count();
                        }
                        return (RCLRESULT)(-1);
                    }
                    else if(qobject_cast<RCComboList *>(ditem) != 0 )
                    {
                        int index = (int)wParam;
                        QString text((RCLPSTR)((RCWPARAM)lParam));
                        index = ((RCComboList*)ditem)->findText(text, Qt::MatchRecursive );
                        ((RCComboList*)ditem)->setCurrentIndex(index);
                        return index;
                    }
                }
                break;
            }
            case LB_GETSELCOUNT:
            {
                if(qobject_cast<RCComboList *>(ditem) != 0 )
                {
                    if(((QListWidget*)ditem)->selectionMode() == QAbstractItemView::SingleSelection ||
                       ((QListWidget*)ditem)->selectionMode() == QAbstractItemView::NoSelection)
                        return LB_ERR;
                    return ((RCComboList*)ditem)->selectedItems().size();
                }
                else if(qobject_cast<QListWidget *>(ditem) != 0 )
                {
                    if(((QListWidget*)ditem)->selectionMode() == QAbstractItemView::SingleSelection ||
                       ((QListWidget*)ditem)->selectionMode() == QAbstractItemView::NoSelection)
                        return LB_ERR;
                    return ((QListWidget*)ditem)->selectedItems().size();
                }
                break;
            }
            case LB_GETSELITEMS :
            {
                if(qobject_cast<RCComboList *>(ditem) != 0 )
                {
                    if(((QListWidget*)ditem)->selectionMode() == QAbstractItemView::SingleSelection ||
                       ((QListWidget*)ditem)->selectionMode() == QAbstractItemView::NoSelection)
                        return LB_ERR;
                    int i,cnt=0,*items = (int*)(RCLPARAM)lParam;
                    for(i=0; i<((RCComboList*)ditem)->count(); i++)
                    {
                        if(!((RCComboList*)ditem)->item(i)->isHidden() &&
                            ((RCComboList*)ditem)->item(i)->isSelected())
                        {
                            items[cnt] = i;
                            cnt++;
                        }
                    }
                    return cnt;
                }
                else if(qobject_cast<QListWidget *>(ditem) != 0 )
                {
                    if(((QListWidget*)ditem)->selectionMode() == QAbstractItemView::SingleSelection ||
                       ((QListWidget*)ditem)->selectionMode() == QAbstractItemView::NoSelection)
                        return LB_ERR;
                    int i,cnt=0,*items = (int*)(RCLPARAM)lParam;
                    for(i=0; i<((QListWidget*)ditem)->count(); i++)
                    {
                        if(!((QListWidget*)ditem)->item(i)->isHidden() &&
                            ((QListWidget*)ditem)->item(i)->isSelected())
                        {
                            items[cnt] = i;
                            cnt++;
                        }
                    }
                    return cnt;
                }
                break;
            }
            case LB_GETTEXT :
            {
                if(qobject_cast<RCComboList *>(ditem) != 0 )
                {
                    int index = (int)wParam;
                    if(index >= 0 && index <= ((RCComboList*)ditem)->count())
                    {
                        QString text("");
                        QListWidgetItem *item = ((RCComboList*)ditem)->item(index);
                        if(item)
                            text = item->text();
                        strcpy((RCLPSTR)lParam,text);
                        return strlen((RCLPSTR)lParam);
                    }
                    return (RCLRESULT)LB_ERR;
                }
                else if(qobject_cast<QListWidget *>(ditem) != 0 )
                {
                    int index = (int)wParam;
                    if(index >= 0 && index <= ((QListWidget*)ditem)->count())
                    {
                        QString text("");
                        QListWidgetItem *item = ((QListWidget*)ditem)->item(index);
                        if(item)
                            text = item->text();
                        strcpy((RCLPSTR)lParam,text);
                        return strlen((RCLPSTR)lParam);
                    }
                    return (RCLRESULT)LB_ERR;
                }
            }
            case LB_GETCURSEL :
            {
                if(qobject_cast<QListWidget *>(ditem) != 0 )
                {
                    int index = ((QListWidget*)ditem)->currentRow();
                    if(index < 0)
                        return (RCLRESULT)LB_ERR;
                    else
                        return index;
                }
            }
            case BM_GETCHECK :
            {
                if(qobject_cast<QRadioButton *>(ditem) != 0 )
                {
                    int checked = (int)((QRadioButton*)ditem)->isChecked();
                    return checked;
                }
            }
            }
        }
    }
    return 0;
    //SendDlgItemMessage( hDlg, nIDDlgItem, Msg, wParam, lParam );
}

RCLRESULT RCSENDITEMMESSAGEW( RCHWND hDlg,
                              RCINT nIDDlgItem,
                              RCUINT Msg,
                              RCWPARAM wParam,
                              RCLPARAM lParam
                              )
{
    return RCSENDITEMMESSAGE(hDlg,nIDDlgItem,Msg,wParam,lParam);
    //SendDlgItemMessageW( hDlg, nIDDlgItem, Msg, wParam, lParam );
}

RCBOOL RCENDDIALOG( RCHWND hDlg, RCINT_PTR nResult )
{
    if(hDlg)
    {
        ((QDialog*)hDlg)->done(nResult);
        return true;
    }
    return false;
    //EndDialog( hDlg, nResult );
}

RCBOOL RCCHECKDLGBUTTON( RCHWND hDlg, RCINT nIDButton, RCUINT uCheck )
{
    bool result=false;
    if(hDlg)
    {
        QWidget *btn = (QWidget*)RCGETDLGITEM(hDlg,nIDButton);
        if(btn)
        {
            //result = RCISDLGBUTTONCHECKED(hDlg,nIDButton);
            if(qobject_cast<QCheckBox *>(btn) != 0)
            {
                if(((QCheckBox*)btn)->isTristate())
                    ((QCheckBox*)btn)->setCheckState((Qt::CheckState)uCheck);
                else
                    ((QCheckBox*)btn)->setChecked((bool)uCheck);
            }
            else if(qobject_cast<QRadioButton *>(btn) != 0)
            {
                ((QRadioButton*)btn)->setChecked((bool)uCheck);
            }
            else
                ((QAbstractButton*)btn)->setChecked((bool)uCheck);
        }
    }
    return result;
    //CheckDlgButton( hDlg, nIDButton, uCheck );
}

RCUINT RCISDLGBUTTONCHECKED( RCHWND hDlg, RCINT nIDButton )
{
    if(hDlg)
    {
        QWidget *btn = ((QWidget*)hDlg)->findChild<QAbstractButton *>(RCintToName(nIDButton));
        if(btn)
        {
            if(qobject_cast<QCheckBox *>(btn) != 0)
            {
                if(((QCheckBox*)btn)->isTristate())
                    return (RCUINT)((QCheckBox*)btn)->checkState();
                else
                    return (RCUINT)((QCheckBox*)btn)->isChecked();
            }
            else if(qobject_cast<QRadioButton*>(btn) != 0)
            {
                return (RCUINT)((QRadioButton*)btn)->isChecked();
            }
            else
                return (RCUINT)((QAbstractButton*)btn)->isChecked();
        }
    }
    return 0;
    //IsDlgButtonChecked( hDlg, nIDButton );
}

RCUINT RCGETDLGITEMTEXT( RCHWND hDlg, RCINT nIDDlgItem, RCLPTSTR lpString, RCINT nMaxCount )
{
    if(hDlg)
    {
        QWidget *ditem = (QWidget*)RCGETDLGITEM(hDlg,nIDDlgItem);
        if(ditem)
        {
            if(qobject_cast<QLineEdit *>(ditem) != 0)
            {
                QString txt = ((QLineEdit*)ditem)->text();
                strncpy(lpString,txt.data(),nMaxCount);
                return strlen(lpString);
            }
            else if(qobject_cast<QPlainTextEdit *>(ditem) != 0)
            {
                QString txt = ((QPlainTextEdit*)ditem)->toPlainText();
                strncpy(lpString,txt.data(),nMaxCount);
                return strlen(lpString);
            }
            else if(qobject_cast<QTextEdit *>(ditem) != 0)
            {
                QString txt = ((QTextEdit*)ditem)->toPlainText();
                strncpy(lpString,txt.data(),nMaxCount);
                return strlen(lpString);
            }
            else if(qobject_cast<QLabel *>(ditem)    != 0)
            {
                //lpString = ((QLabel*)ditem)->text().data();
                QString txt = ((QLabel*)ditem)->text();
                strncpy(lpString,txt.data(),nMaxCount);
                return strlen(lpString);
            }
            else if(qobject_cast<QComboBox *>(ditem)    != 0)
            {
                //lpString = ((QComboBox*)ditem)->currentText().data();
                QString    txt = ((QComboBox*)ditem)->currentText();
                strncpy(lpString,txt.data(),nMaxCount);
                return strlen(lpString);
            }
            else if(qobject_cast<RCComboList *>(ditem)    != 0)
            {
                if(((RCComboList*)ditem)->count() > 0)
                {
                    QString txt = ((RCComboList*)ditem)->currentText();
                    strncpy(lpString,txt.data(),nMaxCount);
                }
                else
                    strcpy(lpString,_RCT(""));
                return strlen(lpString);
            }
            else if(qobject_cast<QListWidget *>(ditem)    != 0)
            {
                if(((QListWidget*)ditem)->count() > 0 &&
                   ((QListWidget*)ditem)->currentRow() >= 0)
                {
                    QString txt = ((QListWidget*)ditem)->currentItem()->text();
                    strncpy(lpString,txt.data(),nMaxCount);
                }
                else
                    strcpy(lpString,_RCT(""));
                return strlen(lpString);
            }
            else if(qobject_cast<RCDoubleSpinBox *>(ditem)    != 0)
            {
                // get the unevaluated text
                QString text = ((QDoubleSpinBox*)ditem)->cleanText();
                strcpy(lpString,text.data());
                return strlen(lpString);
            }
            else if(qobject_cast<WeightSpinBox *>(ditem)    != 0)
            {
                // get the unevaluated text
                QString text = ((WeightSpinBox*)ditem)->cleanText();
                strcpy(lpString,text.data());
                return strlen(lpString);
            }
            else if(qobject_cast<QDoubleSpinBox *>(ditem)    != 0)
            {
                // get the evaluated text value
                double value = ((QDoubleSpinBox*)ditem)->value();
                sprintf(lpString,"%.*lf",db.getnumericdisplaytolerance(),value);
                return strlen(lpString);
            }
            else if(qobject_cast<RCIntegerSpinBox *>(ditem)    != 0)
            {
                QString text = ((QSpinBox*)ditem)->cleanText();
                strcpy(lpString,text.data());
                return strlen(lpString);
            }
            else if(qobject_cast<QSpinBox *>(ditem)    != 0)
            {
                int value = ((QSpinBox*)ditem)->value();
                sprintf(lpString,"%ld",value);
                return strlen(lpString);
            }
            else if(qobject_cast<QDateEdit *>(ditem)    != 0)
            {
                QString sdate = ((QDateEdit*)ditem)->date().toString("yyyy/MM/dd");
                strcpy(lpString,sdate.data());
                return strlen(lpString);
            }
            else if(qobject_cast<QTimeEdit *>(ditem)    != 0)
            {
                QString stime = ((QTimeEdit*)ditem)->time().toString("HH:mm:ss");
                strcpy(lpString,stime.data());
                return strlen(lpString);
            }
            return true;
        }
    }
    return  0;
    //GetDlgItemText( hDlg, nIDDlgItem, lpString, nMaxCount );
}

RCUINT RCGETDLGITEMTEXTW( RCHWND hDlg, RCINT nIDDlgItem, RCLPTSTR lpString, RCINT nMaxCount )
{
    return RCGETDLGITEMTEXT(hDlg,nIDDlgItem,lpString, nMaxCount);
    //GetDlgItemTextW( hDlg, nIDDlgItem, lpString, nMaxCount );
}

RCHWND RCCREATEDIALOG( RCHINSTANCE hInstance,
                       RCLPCTSTR lpTemplate,
                       RCHWND hWndParent,
                       RCDLGPROC lpDialogFunc
                       )
{
    RCHWND result = 0;
    if(hWndParent)
    {
        QString dlgName = QString(lpTemplate);
		if (dlgName.length() > 0) {
			QString typeName = dlgName + QString("*");
			char* pTypeName = qstringToChars(typeName);
			int typeId = QMetaType::type(pTypeName);
			const QMetaObject *metaObj = QMetaType::metaObjectForType(typeId);
			if (metaObj != nullptr) {
				RCDialog* pDlg = nullptr;
				//Trace Dialog doesn't inherit from RCDialog
				if (dlgName == QString("Trace_Dialog")) {
					result = new Trace_Dialog((QWidget*)hWndParent);
					return result;
				}
				if (dlgName == QString("MaskChange_Dialog"))
					pDlg = dynamic_cast<RCDialog*>(metaObj->newInstance(Q_ARG(HANDLE, hWndParent), Q_ARG(HANDLE, hInstance), Q_ARG(int, 0), Q_ARG(int, 0)));
				else
					pDlg = dynamic_cast<RCDialog*>(metaObj->newInstance(Q_ARG(HANDLE, hWndParent), Q_ARG(HANDLE, hInstance), Q_ARG(int, 0)));
				if (pDlg != nullptr) {
					app->processEvents();
					if (pDlg->toActivateParent()) {
						pDlg->show();
						pDlg->raise();
						pDlg->activateWindow();
					}
					result = pDlg;
				}				
			}
		}   
    }
    return result;   
}

RCINT_PTR RCDIALOGBOXINDIRECT( RCHINSTANCE hInstance,
                               RCLPCDLGTEMPLATE lpTemplate,
                               RCHWND hWndParent,
                               RCDLGPROC lpDialogFunc
                               )
{
    return 0;//DialogBoxIndirect( hInstance, lpTemplate, hWndParent, lpDialogFunc );
}

RCINT_PTR RCDIALOGBOX( RCHINSTANCE hInstance, // this is the RealCad Dialog object
                       RCLPCTSTR lpTemplate,
                       RCHWND hWndParent,
                       RCDLGPROC lpDialogFunc
                       )
{
    RCINT_PTR result=0;
    if(hWndParent)
    {
        QString dlgName = QString(lpTemplate);		
		
		if (dlgName.length() > 0)
		{
			QString typeName = dlgName + QString("*");
			char* pTypeName = qstringToChars(typeName);			
			int typeId = QMetaType::type(pTypeName);
			const QMetaObject *metaObj = QMetaType::metaObjectForType(typeId);
			if (metaObj != nullptr) {
				QDialog* pDlg = nullptr;
				
				if(dlgName==QString("MaskChange_Dialog"))
					pDlg = dynamic_cast<QDialog*>(metaObj->newInstance(Q_ARG(HANDLE, hWndParent), Q_ARG(HANDLE, hInstance), Q_ARG(int, 0), Q_ARG(int, 0)));
				else
					pDlg = dynamic_cast<QDialog*>(metaObj->newInstance(Q_ARG(HANDLE, hWndParent), Q_ARG(HANDLE, hInstance), Q_ARG(int, 0)));
				if (pDlg != nullptr) {
					app->processEvents();
					result= pDlg->exec();
				}
			}
		}	
    }
    return result;    
}

RCDWORD RCGETGLYPHOUTLINE( RCHDC hdc,             // handle to DC
                           RCUINT uChar,          // character to query
                           RCUINT uFormat,        // data format
                           RCLPGLYPHMETRICS lpgm,   // glyph metrics
                           RCDWORD cbBuffer,      // size of data buffer
                           RCLPVOID lpvBuffer,    // data buffer
                           CONST RCMAT2 *lpmat2   // transformation matrix
                          )
{
    return 0;//GetGlyphOutline( hdc, uChar, uFormat, lpgm, cbBuffer, lpvBuffer, lpmat2  );
}

RCDWORD RCGETGLYPHOUTLINEW( RCHDC hdc,             // handle to DC
                            RCUINT uChar,          // character to query
                            RCUINT uFormat,        // data format
                            RCLPGLYPHMETRICS lpgm,   // glyph metrics
                            RCDWORD cbBuffer,      // size of data buffer
                            RCLPVOID lpvBuffer,    // data buffer
                            CONST RCMAT2 *lpmat2   // transformation matrix
                          )
{
    return 0;//GetGlyphOutlineW( hdc, uChar, uFormat, lpgm, cbBuffer, lpvBuffer, lpmat2  );
}

RCBOOL RCEXTTEXTOUT( RCHDC hdc,          // handle to DC
                     RCINT X,            // x-coordinate of reference point
                     RCINT Y,            // y-coordinate of reference point
                     RCUINT fuOptions,   // text-output options
                     CONST RCRECT* lprc, // optional dimensions
                     RCLPCTSTR lpString, // string
                     RCUINT cbCount,     // number of characters in string
                     CONST RCINT* lpDx   // array of spacing values
                    )
{
    return 0;//(RCBOOL)ExtTextOut( hdc, X, Y, fuOptions, lprc, lpString, cbCount, lpDx  );
}

RCINT RCENUMFONTFAMILIES(  RCHDC hdc,                        // handle to DC
  RCLPCTSTR lpszFamily,             // font family
  RCFONTENUMPROC lpEnumFontFamProc, // callback function
  RCLPARAM lParam                   // additional data
                       )
{
    return 0;//(RCINT)EnumFontFamilies( hdc, lpszFamily, lpEnumFontFamProc, lParam  );
}

RCINT RCENUMFONTFAMILIESW(  RCHDC hdc,                        // handle to DC
  RCLPCTSTR lpszFamily,             // font family
  RCFONTENUMPROCW lpEnumFontFamProc, // callback function
  RCLPARAM lParam                   // additional data
                       )
{
    return 0;//(RCINT)EnumFontFamiliesW( hdc, lpszFamily, lpEnumFontFamProc, lParam  );
}

RCWINOLEAPI RCSTGISSTORAGEFILE(  RCWCHAR* pwcsName )
{
    return (RCWINOLEAPI)!S_OK;//StgIsStorageFile( pwcsName );
}

RCUINT_PTR CALLBACK OFNHookProcOldStyle( RCHWND hdlg,
    RCUINT uiMsg,
    RCWPARAM wParam,
    RCLPARAM lParam
)
{
    return 0;
}

RCBOOL RCGETSAVEFILENAME( RCLPOPENFILENAME lpofn )
{
    /*
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = cadwindow->gethwnd();
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile= szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrDefExt =  rsnc2.getws();
    ofn.lpstrFileTitle = title;
    */
    // using the Window open file structure to pass data

    QString filter(lpofn->lpstrFilter);
    QString initdir(lpofn->lpstrInitialDir);
    QString defext(lpofn->lpstrDefExt);
    QString caption(lpofn->lpstrTitle);
    if(caption.isEmpty())
        caption = "Save File";
#if 1
    QString filePath;
    QStringList fileNames;
    QFileDialog dialog((QWidget*)lpofn->hwndOwner,caption,initdir,defext);
    dialog.setNameFilter(filter);
    dialog.selectFile(QString(lpofn->lpstrFile));

    // there is a Qt bug  which hangs the native Mac file open dialog
    // so use the Qt dialog on the Mac for now
#ifdef _MAC
    dialog.setOption(QFileDialog::DontUseNativeDialog,false);
    //dialog.setOption(QFileDialog::DontUseNativeDialog,true);
#else
    dialog.setOption(QFileDialog::DontUseNativeDialog,false);
#endif

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix(defext);

    if(dialog.exec())
        fileNames = dialog.selectedFiles();

    if(fileNames.size() > 0)
    {
        filePath = fileNames[0];
        QFileInfo saveFile(filePath);
        QString fileName = saveFile.fileName();
        //if(!fileName.endsWith(defext,Qt::CaseInsensitive))
        //    fileName += "." + defext;
        QString fullFileName;
        //if(!filePath.endsWith(defext,Qt::CaseInsensitive))
        //    fullFileName = filePath + "." + defext;
        //else
            fullFileName = filePath;
        if(lpofn->lpstrFile !=0)
        {
            strcpy(lpofn->lpstrFile,&fullFileName);
            lpofn->nMaxFile = fullFileName.size();
        }
        if(lpofn->lpstrFileTitle !=0)
        {
            strcpy(lpofn->lpstrFileTitle,&fileName);
            lpofn->nMaxFileTitle = fileName.size();
        }
    }

    return filePath.length() > 0 ? TRUE : FALSE;
#else
    QString exten = defext;
    QString filePath = QFileDialog::getSaveFileName((QWidget*)lpofn->hwndOwner,
                                                    app->tr("Save File"),
                                                    initdir,
                                                    filter,
                                                    &exten);
    if(filePath.length() > 0)
    {
        QFileInfo saveFile(filePath);
        QString fileName = saveFile.fileName();
        if(!fileName.endsWith(defext,Qt::CaseInsensitive))
            fileName += "." + defext;
        strcpy(lpofn->lpstrFile,&filePath);
        QString fullFileName;
        if(!filePath.endsWith(defext,Qt::CaseInsensitive))
            fullFileName = filePath + "." + defext;
        else
            fullFileName = filePath;
        if(lpofn->lpstrFile !=0)
        {
            strcpy(lpofn->lpstrFile,&fullFileName);
            lpofn->nMaxFile = fullFileName.size();
        }
        if(lpofn->lpstrFileTitle !=0)
        {
            strcpy(lpofn->lpstrFileTitle,&fileName);
            lpofn->nMaxFileTitle = fileName.size();
        }
    }

    return filePath.length() > 0 ? TRUE : FALSE;
#endif
}

RCCHAR *_itoa( RCINT value, RCCHAR *str, RCINT radix  )
{
    return 0;//_itot( value, str, radix  );
}

RCINT  RCSTRETCHDIBITS(  RCHDC hdc,                      // handle to DC
  RCINT  XDest,                    // x-coord of destination upper-left corner
  RCINT  YDest,                    // y-coord of destination upper-left corner
  RCINT  nDestWidth,               // width of destination rectangle
  RCINT  nDestHeight,              // height of destination rectangle
  RCINT  XSrc,                     // x-coord of source upper-left corner
  RCINT  YSrc,                     // y-coord of source upper-left corner
  RCINT  nSrcWidth,                // width of source rectangle
  RCINT  nSrcHeight,               // height of source rectangle
  CONST RCVOID *lpBits,           // bitmap bits
  CONST RCBITMAPINFO *lpBitsInfo, // bitmap data
  RCUINT iUsage,                  // usage options
  RCDWORD dwRop                   // raster operation code
                    )
{
    return 0;
    /*
   StretchDIBits( hdc,                      // handle to DC
   XDest,                    // x-coord of destination upper-left corner
   YDest,                    // y-coord of destination upper-left corner
   nDestWidth,               // width of destination rectangle
   nDestHeight,              // height of destination rectangle
   XSrc,                     // x-coord of source upper-left corner
   YSrc,                     // y-coord of source upper-left corner
   nSrcWidth,                // width of source rectangle
   nSrcHeight,               // height of source rectangle
   lpBits,           // bitmap bits
   lpBitsInfo, // bitmap data
   iUsage,                  // usage options
   dwRop                   // raster operation code
   );
   */
}

RCDWORD RCGETTEMPPATH( RCDWORD nBufferLength, RCLPTSTR lpBuffer )
{
    QString tempPath = QDir::tempPath();
    if( ! tempPath.endsWith(QDir::separator()) )
        tempPath.append(QDir::separator());
    strncpy(lpBuffer,tempPath.data(),nBufferLength);
    if(nBufferLength < tempPath.length())
        return tempPath.length();
    else
        return strlen(lpBuffer);
}

RCUINT RCGETTEMPFILENAME( RCLPCTSTR lpPathName, RCLPCTSTR lpPrefixString,  RCUINT uUnique,  RCLPTSTR lpTempFileName)
{
    QString qTemplateName, qPathName(lpPathName), qPrefixString(lpPrefixString);
    qTemplateName = qPathName + "/" + qPrefixString;
    QTemporaryFile tempFile(qTemplateName);
    if(tempFile.open())
    {
        strcpy(lpTempFileName,tempFile.fileName());
        return 1;
    }
    return 0;//GetTempFileName( lpPathName, lpPrefixString, uUnique, lpTempFileName );
}

RCBOOL   RCDELETEFILE( RCLPCTSTR lpFileName )
{
    QFile::remove(QString(lpFileName));
    return 0;//DeleteFile( lpFileName);
}

RCSHORT GetFileTitle( RCLPCTSTR lpszFile,
                      RCLPTSTR lpszTitle,
                      RCWORD cbBuf
                      )
{
    QString title = QFileInfo(QString(lpszFile)).fileName();
    strncpy(lpszTitle,title.data(),cbBuf);
    return 0;//GetFileTitle( lpszFile, lpszTitle,  cbBuf );
}

RCBOOL  RCCOPYFILE( RCLPCTSTR lpExistingFileName, RCLPCTSTR lpNewFileName, RCBOOL bFailIfExists )
{
    // if bFailIfExists is true do not overwrite if the new file exists
    // else overwrite the new file
    // the QFile::copy function will not overwrite an existing file
    bool newfileExists = QFile::exists(QString(lpNewFileName));
    if(bFailIfExists && newfileExists)
        return 0;
    QFile::remove(QString(lpNewFileName));
    QFile::copy(QString(lpExistingFileName),QString(lpNewFileName));
    return 1;
}

RCHMODULE RCLOADLIBRARY( RCLPCTSTR lpFileName )
{
    return 0;//LoadLibrary( lpFileName );
}

RCFARPROC WINAPI RCGETPROCADDRESS( RCHMODULE hModule, RCLPCSTR lpProcName )
{
    return 0;//GetProcAddress( hModule, lpProcName );
}

RCFARPROC WINAPI RCGETPROCADDRESS( RCHMODULE hModule, LPCSTR lpProcName )
{
    return 0;//GetProcAddress( hModule, lpProcName );
}

RCBOOL RCCREATEPROCESS(
        RCLPCTSTR lpApplicationName,
        RCLPTSTR lpCommandLine,
        RCLPSECURITY_ATTRIBUTES lpProcessAttributes,
        RCLPSECURITY_ATTRIBUTES lpThreadAttributes,
        RCBOOL bInheritHandles,
        RCDWORD dwCreationFlags,
        RCLPVOID lpEnvironment,
        RCLPCTSTR lpCurrentDirectory,
        RCLPSTARTUPINFO lpStartupInfo,
        RCLPPROCESS_INFORMATION lpProcessInformation
        )
{
    return 0;/*CreateProcess(
                lpApplicationName,
                lpCommandLine,
                lpProcessAttributes,
                lpThreadAttributes,
                bInheritHandles,
                dwCreationFlags,
                lpEnvironment,
                lpCurrentDirectory,
                lpStartupInfo,
                lpProcessInformation
                );
                */
}

RCBOOL RCFREELIBRARY( RCHMODULE hModule )
{
    return hModule == 0 ? false : ((QLibrary*)hModule)->unload();
    //FreeLibrary( hModule );
}

RCINT RCGETSCROLLPOS( RCHWND hWnd, RCINT nBar )
{
    return 0;//GetScrollPos( hWnd, nBar );
}


typedef std::vector<RC_finddata_t> RCfindfile_List;
class RC_findfile_container
{
private:
    int lastid;
    RCfindfile_List *mlist;

public:
    RC_findfile_container()
    {
        mlist=0;
        lastid=0;
    }
    RC_findfile_container(const RC_findfile_container &other)
    {
        if(other.mlist)
            mlist = new RCfindfile_List(*other.mlist);
        else
            mlist=0;
        lastid = other.lastid;
    }
    RC_findfile_container(RCfindfile_List *inlist)
    {
        mlist = inlist;
        lastid = 0;
    }
    ~RC_findfile_container()
    {
        delete mlist;
    }
    RC_findfile_container& operator=(const RC_findfile_container& c)
    {
        if(this == &c)
            return *this;
        delete mlist;
        mlist = new RCfindfile_List(*c.mlist);
        lastid = c.lastid;
        return *this;
    }

    RCfindfile_List *list()
    {
        return mlist;
    }
    RC_finddata_t *next()
    {
        if(mlist && mlist->size() > 0 && lastid < mlist->size())
            return &mlist->at(lastid++);
        else
            return 0;
    }
    void clear()
    {
        delete mlist;
        mlist = 0;
        lastid = 0;
    }
    bool empty()
    {
        if(mlist==0)
            return true;
        else
            return false;
    }
    void debug()
    {
        qDebug() << "list ptr : " << mlist;
        if(mlist)
        {
            qDebug() << "list size: " << mlist->size();
            for(int i=0; i < mlist->size(); i++)
                qDebug() << "mlist[" << i << "] : " << QString(((RC_finddata_t)mlist->at(i)).name);
        }
        qDebug() << "last id  : " << lastid;
    }
};
class rcffIndex
{
public:
    int ffcId;
    rcffIndex(int id=-1)
    {
        ffcId = id;
    }
};
static std::vector<RC_findfile_container> RCfindfilecontainer_List;
static std::vector<rcffIndex>RCffhandle_List;

RCintptr_t RCFINDFIRST(const RCCHAR *filespec,
   struct RC_finddata_t *fileinfo)
{
    if(filespec && fileinfo)
    {
        //qDebug() << "filespec : " << QString(filespec);
        QFileInfo qfilespec;
        qfilespec.setFile(QString(filespec));
        QString qfile = qfilespec.fileName();
        QString qsuffix = qfilespec.suffix();
        QStringList qfst(qfile);
        QDir qdir = qfilespec.absoluteDir();
        if(qsuffix.length() > 0)
            qdir.setNameFilters(qfst);
        qdir.setSorting(QDir::Name);
        QDir::Filters filter = fileinfo->filter.mfilter;// | QDir::Files | QDir::NoDotAndDotDot | QDir::Readable;
        qdir.setFilter(filter);

        //qDebug() << "RCfindfilecontainer_List.size() : " << RCfindfilecontainer_List.size();
        //qDebug() << "info file : " << qfile;
        //qDebug() << "info suffix : " << qsuffix;
        //qDebug() << "dir : " << qdir.dirName();
        //qDebug() << "path : " << qdir.path();
        //qDebug() << "name filters : " << qdir.nameFilters();
        //qDebug() << "filter : " << qdir.filter();
        //qDebug() << "sorting : " << qdir.sorting();

        QFileInfoList qlist = qdir.entryInfoList();
        if(qlist.count() > 0)
        {
            RCfindfile_List *RCfindfiledata_list = new RCfindfile_List();
            if(RCfindfiledata_list != 0 )
            {
                //qDebug() << "RCFINDFIRST : list size : " << RCfindfiledata_list->size();
                for (int i = 0; i < qlist.count(); ++i)
                {
                    QFileInfo fileInfo = qlist.at(i);
                    //qDebug() << "RCFINDFIRST : qfileInfo : " << fileInfo.fileName();
                    RC_finddata_t fd;
                    fd.size = fileInfo.size();
                    strcpy(fd.name,fileInfo.fileName());
                    //qDebug() << "RCFINDFIRST : fd.name : " << QString(fd.name);
                    RCfindfiledata_list->push_back(fd);
                    //qDebug() << "RCFINDFIRST : list size : " << RCfindfiledata_list->size();
                }
                RC_finddata_t fd = (RC_finddata_t)RCfindfiledata_list->front();
                //qDebug() << "fd.name : " << QString(fd.name);
                fileinfo->size = fd.size;
                strcpy(fileinfo->name,fd.name);
                // look for an empty slot to reuse
                for(int h=0; h < RCffhandle_List.size(); h++)
                {
                    rcffIndex thisid = RCffhandle_List.at(h);
                    //RC_findfile_container thisffc = RCfindfilecontainer_List.at(thisid.ffcId);
                    if(RCfindfilecontainer_List.at(thisid.ffcId).empty())
                    {
                        //RC_findfile_container rcffc;
                        //rcffc = RC_findfile_container(RCfindfiledata_list);
                        //rcffc.debug();
                        RCfindfilecontainer_List[thisid.ffcId] = RC_findfile_container(RCfindfiledata_list);
                        //RCfindfilecontainer_List.at(thisid.ffcId).debug();
                        return thisid.ffcId;
                    }
                }
                // no empty slots so add it to the end of the list
                RCfindfilecontainer_List.push_back(RC_findfile_container(RCfindfiledata_list));
                rcffIndex handle = rcffIndex(RCfindfilecontainer_List.size()-1);
                RCffhandle_List.push_back(handle);
                //qDebug() << "RCFINDFIRST RCFF list size : " << RCfindfilecontainer_List.size() << endl;
                return handle.ffcId;
            }
        }
    }
    return -1;//_findfirst( filespec, fileinfo );
}

int RCFINDNEXT(
   intptr_t handle,
   struct RC_finddata_t *fileinfo
)
{
    if(handle >= 0 && fileinfo)
    {
        //qDebug() << "RCFINDNEXT : RCFF list size : " << RCfindfilecontainer_List.size() << endl;
        rcffIndex ffindex = RCffhandle_List.at(handle);
        if(RCfindfilecontainer_List.size() > 0 && ffindex.ffcId < RCfindfilecontainer_List.size())
        {
            RC_finddata_t *fd = RCfindfilecontainer_List.at(ffindex.ffcId).next();
            if( fd != 0)
            {
                //qDebug() << "fd  << : " << fd;
                //qDebug() << "fd.size" << fd->size;
                //qDebug() << "fd.name" << QString(fd->name);
                fileinfo->size = fd->size;
                strcpy(fileinfo->name,fd->name);
                //qDebug() << "RCFINDNEXT : fd.name : " << QString(fd->name);
                return 0;
            }
        }
    }
    return -1;
}

int RCFINDCLOSE( intptr_t handle )
{
    if(handle >= 0)
    {
        rcffIndex ffindex = RCffhandle_List.at(handle);
        if(RCfindfilecontainer_List.size() > 0 && ffindex.ffcId < RCfindfilecontainer_List.size())
        {
            //((RC_findfile_container*)
            RCfindfilecontainer_List.at(ffindex.ffcId).clear();
            //if(RCfindfilecontainer_List.at(ffindex.ffcId).empty())
            //    qDebug() << "empty";
            //else
            //    qDebug() << "not empty";
            //qDebug() << "RCFINDCLOSE RCFF list size : " << RCfindfilecontainer_List.size() << endl;
            return 0;
            /*
            RC_findfile_container ffc = RCfindfilecontainer_List.at(ffindex.ffcId);
            std::vector<RC_findfile_container>::iterator iter;
            for(iter=RCfindfilecontainer_List.begin(); iter < RCfindfilecontainer_List.end(); iter++)
            {
                if(&*iter == &RCfindfilecontainer_List.at(ffindex.ffcId))
                {
                    ((RC_findfile_container)*iter).clear();
                    qDebug() << "RCFINDCLOSE RCFF list size : " << RCfindfilecontainer_List.size() << endl;
                    return 0;
                }
            }
            */
        }
    }
    return -1;
}

RCUINT  RCWINEXEC( RCLPCSTR lpCmdLine, RCUINT uCmdShow )
{
    QProcess process;
    process.startDetached(RCSTRING(lpCmdLine));
    return   0;//WinExec( lpCmdLine, uCmdShow );

}

RCSIZE_T RCSTRFTIME( RCCHAR *strDest,
                 RCSIZE_T maxsize,
                 const RCCHAR *format,
                 const struct tm *timeptr
                 )
{
    return 0;// strftime( strDest, maxsize, format, timeptr  );
}

RCHFILE   RCOPENFILE( RCLPCSTR lpFileName, RCLPOFSTRUCT lpReOpenBuff, RCUINT uStyle )
{
    return  0;// OpenFile( lpFileName, lpReOpenBuff, uStyle );
}

RCDWORD RCCHECKMENUITEM( RCHMENU hmenu, RCUINT uIDCheckItem, RCUINT uCheck )
{
    return 0;// CheckMenuItem( hmenu, uIDCheckItem, uCheck );
}

RCINT RCGETMENUSTRING( RCHMENU hMenu,
                   RCUINT uIDItem,
                   RCLPTSTR lpString,
                   RCINT nMaxCount,
                   RCUINT uFlag
                   )
{
    return 0;//
    /*
    return GetMenuString( hMenu,
                       uIDItem,
                       lpString,
                       nMaxCount,
                       uFlag
                       );
                       */
}

RCBOOL RCPrintDlg( RCLPPRINTDLG lppd )
{
    return 0;//PrintDlg( lppd );
}

RCHDC RCCRETEIC( RCLPCTSTR lpszDriver,       // driver name
  RCLPCTSTR lpszDevice,       // device name
  RCLPCTSTR lpszOutput,       // port or file name
  CONST RCDEVMODE *lpdvmInit  // optional initialization data
               )
{
    return 0;//CreateIC( lpszDriver, lpszDevice, lpszOutput, lpdvmInit  );
}

RCINT RCDOCABORT( RCHDC hdc )
{
    return 0;//AbortDoc( hdc );
}

RCINT RCEndPage( RCHDC hdc )
{
    return 0;//EndPage( hdc );
}

RCINT EndDoc( RCHDC hdc )
{
    return 0;//EndDoc( hdc );
}

RCINT RCStartDoc( RCHDC hdc, // handle to DC
              CONST RCDOCINFO* lpdi // contains file names
              )
{
    return 0;//StartDoc( hdc, lpdi  );
}

RCINT RCStartPage( RCHDC hDC )
{
    return 0;//StartPage( hDC );
}

RCHDC RCCreateEnhMetaFile( RCHDC hdcRef,            // handle to reference DC
  RCLPCTSTR lpFilename,    // file name
  CONST RCRECT* lpRect,    // bounding rectangle
  RCLPCTSTR lpDescription  // description string
                        )
{
    return 0;
    /*
    return CreateEnhMetaFile( hdcRef,            // handle to reference DC
        lpFilename,    // file name
       lpRect,    // bounding rectangle
        lpDescription  // description string
                            );
    */
}

RCHENHMETAFILE RCCloseEnhMetaFile( RCHDC hdc )
{
    return 0;//CloseEnhMetaFile( hdc );
}

RCBOOL RCDeleteEnhMetaFile( RCHENHMETAFILE hemf )
{
    return 0;//DeleteEnhMetaFile( hemf );
}

RCBOOL SetWindowExtEx( RCHDC hdc, // handle to device context
                       RCINT nXExtent,  // new horizontal window extent
                       RCINT nYExtent,  // new vertical window extent
                       RCLPSIZE lpSize  // original window extent
                      )
{
    return 0;//
    /*
    return SetWindowExtEx( hdc,       // handle to device context
                           nXExtent,  // new horizontal window extent
                           nYExtent,  // new vertical window extent
                           lpSize  // original window extent
                           );
                           */
}

RCBOOL RCCloseFigure( RCHDC hdc )
{
    return 0;//CloseFigure( hdc );
}

RCHBITMAP RCLoadBitmap( RCHINSTANCE hInstance, RCLPTSTR lpBitmapName )
{
    if(hInstance != 0){
        for(int i=0; i<RC_NUM_NAMED_BITMAP_RESOURCES; i++)
        {
            QString lwrresr(RCnamedBitmapResources[i].name);
            QString lwrname(lpBitmapName);
            if( strcmp(lwrresr.toLower(), lwrname.toLower())  == 0 &&
                stricmp(RCnamedBitmapResources[i].typeName, "BITMAP") == 0  )
            {
                QString qsname(RCnamedBitmapResources[i].fileName);
                QPixmap *pxmap = new QPixmap();
                pxmap->load(":/images/"+qsname);
                if(pxmap && !pxmap->isNull() && cadwindow != 0)
                    pxmap->setDevicePixelRatio(((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio());
                else if(pxmap && !pxmap->isNull() && cadwindow == 0)
                    pxmap->setDevicePixelRatio(app->devicePixelRatio());
                return (RCHBITMAP)(pxmap);
                //return (RCHBITMAP)( new QPixmap(":/images/"+qsname));
            }
        }
        // custom application icon??
        // just use the given name as is
        QString qsname(lpBitmapName);
        QPixmap *pxmap = new QPixmap(qsname);
        if(pxmap && cadwindow != 0)
            pxmap->setDevicePixelRatio(((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio());
        else if(pxmap && cadwindow == 0)
            pxmap->setDevicePixelRatio(app->devicePixelRatio());
        //pxmap->setDevicePixelRatio(2.0);
        return (RCHBITMAP)(pxmap);
        //return (RCHBITMAP)( new QPixmap(qsname));
    }
    else{
        QString qsname(lpBitmapName);
        QPixmap *pxmap = new QPixmap(qsname);
        if(pxmap && cadwindow != 0)
            pxmap->setDevicePixelRatio(((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio());
        else if(pxmap && cadwindow == 0)
            pxmap->setDevicePixelRatio(app->devicePixelRatio());
        //pxmap->setDevicePixelRatio(2.0);
        return (RCHBITMAP)(pxmap);
        //return (RCHBITMAP)( new QPixmap(qsname));
    }
}

RCHBITMAP RCLoadBitmap( RCHINSTANCE hInstance, LPTSTR lpBitmapName )
{
    if(hInstance != 0){
        for(int i=0; i<RC_NUM_NAMED_BITMAP_RESOURCES; i++)
        {
            if( stricmp(RCnamedBitmapResources[i].name,lpBitmapName)  == 0 &&
                stricmp(RCnamedBitmapResources[i].typeName,"BITMAP") == 0  )
            {
                QString qsname(RCnamedBitmapResources[i].fileName);
                return (RCHBITMAP)( new QPixmap(":/images/"+qsname));
            }
        }
        return (RCHBITMAP)NULL;
    }
    else{
        return (RCHBITMAP)NULL;
    }
}

RCHBITMAP RCLoadBitmap( RCHINSTANCE hInstance, ULONG_PTR lpBitmapName )
{
    if(hInstance != 0){
        for(int i=0; i<RC_NUM_NAMED_BITMAP_RESOURCES; i++)
        {
            if( RCnumberedBitmapResources[i].id == lpBitmapName  &&
                strcmp(RCnumberedBitmapResources[i].typeName,"BITMAP") == 0  )
            {
                QString qsname(RCnumberedBitmapResources[i].fileName);
                return (RCHBITMAP)(new QPixmap(":/images/"+qsname));
            }
        }
        return (RCHBITMAP)NULL;
    }
    else{
        return (RCHBITMAP)NULL;
    }
}

RCHENHMETAFILE GetEnhMetaFile( RCLPCTSTR lpszMetaFile )
{
    return 0;//GetEnhMetaFile( lpszMetaFile );
}

RCHMETAFILE GetMetaFile( RCLPCTSTR lpszMetaFile )
{
    return 0;//(RCHMETAFILE)GetMetaFile( lpszMetaFile );
}

RCINT CALLBACK RCEnumMetaFileProc(  RCHDC hDC,                // handle to DC
  RCHANDLETABLE *lpHTable,  // metafile handle table
  RCMETARECORD *lpMFR,      // metafile record
  RCINT nObj,               // count of objects
  RCLPARAM lpClientData     // optional data
                                )
{
    return 0;
}

RCBOOL RCEnumMetaFile(  RCHDC hdc,                // handle to DC
  RCHMETAFILE hmf,          // handle to Windows-format metafile
  RCMFENUMPROC lpMetaFunc,  // callback function
  RCLPARAM lParam           // optional data
                    )
{
    return 0;//
    /*
    return EnumMetaFile(   hdc,
                           hmf,
                           lpMetaFunc,
                           lParam           // optional data
                        );
                        */
}

RCBOOL RCDeleteMetaFile( RCHMETAFILE hmf  )
{
    return 0;//DeleteMetaFile( hmf );
}

RCBOOL   SetDllDirectory( RCLPCTSTR lpPathName )
{
    app->addLibraryPath(QString(lpPathName));
    return 0;//  SetDllDirectory( lpPathName );
}

RCBOOL RCGetOpenFileName( RCLPOPENFILENAME lpofn )
{
    /*
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = cadwindow->gethwnd();
    ofn.lpstrFilter = szFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile= szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = szDirName;
    ofn.lpstrDefExt =  rsnc2.getws();
    ofn.lpstrFileTitle = title;
    */
    // using the Window open file structure to pass data

    QString filter(lpofn->lpstrFilter);
    QString initdir(lpofn->lpstrInitialDir);
    QString defext(lpofn->lpstrDefExt);
    QString caption(lpofn->lpstrTitle);
    if(caption.length() < 1)
        caption = "Open File";
#if 1
    QString filePath;
    QStringList fileNames;
    QFileDialog dialog((QWidget*)lpofn->hwndOwner,caption,initdir,defext);
    dialog.setNameFilter(filter);
    QStringList history;
    history.append(QString(lpofn->lpstrFile));
    //dialog.setLabelText(QFileDialog::FileName,QString(lpofn->lpstrFile));
    dialog.selectFile(QString(lpofn->lpstrFile));

    // there is a Qt bug  which hangs the native Mac file open dialog
    // so use the Qt dialog on the Mac for now
#ifdef _MAC
    dialog.setOption(QFileDialog::DontUseNativeDialog,false);
    //dialog.setOption(QFileDialog::DontUseNativeDialog,true);
#else
    dialog.setOption(QFileDialog::DontUseNativeDialog,false);
#endif

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    qDebug() << "LOWORD(lpofn->Flags) : " << LOWORD(lpofn->Flags);
    if(LOWORD(lpofn->Flags) & QFileDialog::ShowDirsOnly == QFileDialog::ShowDirsOnly)
    {
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);
    }
    qDebug() << "HIWORD(lpofn->Flags) : " << HIWORD(lpofn->Flags);
    if(HIWORD(lpofn->Flags) == QFileDialog::ExistingFile)
        dialog.setFileMode(QFileDialog::ExistingFile);
    else if(HIWORD(lpofn->Flags) == QFileDialog::ExistingFiles)
        dialog.setFileMode(QFileDialog::ExistingFiles);

    fileNames.empty();
    if(dialog.exec())
        fileNames = dialog.selectedFiles();

    if(fileNames.size() > 0 )
    {
        RCCHAR *nameptr = lpofn->lpstrFile;
        lpofn->nMaxFile=0;
        for(int i=0; i<fileNames.size(); i++)
        {
            filePath = QDir::fromNativeSeparators(fileNames[i]);
            QFileInfo openFile(filePath);
            QString fileName = openFile.fileName();
            if(lpofn->lpstrFile != 0)
            {
                if(i==0)
                {
                    if(fileNames.size() > 1)
                    {
                        strcpy(nameptr,openFile.path().data());
                        qDebug() << "nameptr : " << QString(nameptr);
                        nameptr += openFile.path().length()+1;
                        *nameptr = 0;
                        lpofn->nMaxFile += openFile.path().length()+1;
                        qDebug() << "lpofn->nMaxFile : " << lpofn->nMaxFile;
                        strcpy(nameptr,openFile.fileName().data());
                        qDebug() << "nameptr : " << QString(nameptr);
                        nameptr += openFile.fileName().length()+1;
                        *nameptr = 0;
                        lpofn->nMaxFile += openFile.fileName().length()+1;
                        qDebug() << "lpofn->nMaxFile : " << lpofn->nMaxFile;
                    }
                    else
                    {
                        strcpy(nameptr,openFile.filePath().data());
                        qDebug() << "nameptr : " << QString(nameptr);
                        nameptr += openFile.filePath().length()+1;
                        *nameptr = 0;
                        lpofn->nMaxFile += openFile.filePath().length()+1;
                        qDebug() << "lpofn->nMaxFile : " << lpofn->nMaxFile;
                    }
                }
                else
                {
                    strcat(nameptr,openFile.fileName().data());
                    qDebug() << "nameptr : " << QString(nameptr);
                    nameptr += openFile.fileName().length()+1;
                    *nameptr = 0;
                    lpofn->nMaxFile += openFile.fileName().length()+1;
                    qDebug() << "lpofn->nMaxFile : " << lpofn->nMaxFile;
                }
            }
        }
        //filePath = fileNames[0];
        QFileInfo openFile(filePath);
        filePath = QDir::fromNativeSeparators(fileNames[0]);
        QString fileName = openFile.fileName();
        lpofn->nFileOffset = openFile.path().size()+1;
        qDebug() << "lpofn->lpstrFile : " << QString(lpofn->lpstrFile);
        qDebug() << "filePath : " << filePath;
        if(lpofn->lpstrFileTitle !=0)
            strcpy(lpofn->lpstrFileTitle,&fileName);
    }
#else
    QString filePath = QFileDialog::getOpenFileName((QWidget*)lpofn->hwndOwner,
                                                    app->tr("Open File"),
                                                    initdir,
                                                    filter,
                                                    &defext);

    filePath = QDir::fromNativeSeparators(filePath);
    QFileInfo openFile(filePath);
    QString fileName = openFile.fileName();
    strcpy(lpofn->lpstrFile,&filePath);
    lpofn->nMaxFile = filePath.size();
    strcpy(lpofn->lpstrFileTitle,&fileName);
    lpofn->nMaxFileTitle = fileName.size();
#endif
    return fileNames.size() > 0 ? TRUE : FALSE;
    // return GetOpenFileName( lpofn );
}

RCBOOL RCGetOpenFileNameA( RCLPOPENFILENAMEA lpofn )
{
    return 0;// GetOpenFileName( lpofn );
}

RCULONG RCinet_addr( const RCCHAR* cp)
{
    return 0;//inet_addr(cp);
}

RChostent* RCgethostbyname( const RCCHAR* name )
{
    return 0;//gethostbyname( name );
}

RCINT send( RCSOCKET s,
            const RCCHAR* buf,
            RCINT len,
            RCINT flags
            )
{
    return 0;//send(s,buf, len, flags );
}

RCINT RCrecv(
        RCSOCKET s,
        RCCHAR* buf,
        RCINT len,
        RCINT flags
        )
{
    return  0;//recv( s, buf, len, flags );
}

RCBOOL RCSHGetSpecialFolderPath( RCHWND hwndOwner,
                               RCLPSTR lpszPath,
                               RCINT nFolder,
                               RCBOOL fCreate
                               )
{
    return 0;// SHGetSpecialFolderPath( hwndOwner, lpszPath, nFolder, fCreate );
}

RCULONG FAR PASCAL RCMapiSendMail(
        RCLHANDLE lhSession,
        RCULONG ulUIParam,
        RClpMapiMessage lpMessage,
        RCFLAGS flFlags,
        RCULONG ulReserved
        )
{
    return 0;//MAPISendMail( (LHANDLE)lhSession,  ulUIParam, (lpMapiMessage)lpMessage, flFlags, ulReserved );
}

/**
 * @brief RCTrackPopupMenu
 * @param hMenu
 * @param uFlags
 * @param x
 * @param y
 * @param nReserved
 * @param hWnd
 * @param prcRect
 * @return
 *
 * Works like the Windows function if the same name but not complete
 * the TPM_RETURNCMD is supported to return the command id
 * otherwise the command event is run from here
 *
 */
RCBOOL RCTrackPopupMenu( RCHMENU hMenu,
    RCUINT uFlags,
    RCINT x,
    RCINT y,
    RCINT nReserved,
    RCHWND hWnd,
    RCHWND prcRect
     )
{
    // the context menu is implemented in MainWindow.cpp
    int id=0;
    if(hMenu && qobject_cast<QMenu *>((QWidget*)hMenu) != 0)
    {
        if(((QMenu*)hMenu)->objectName() == "Popup_Menu")
        {
            QPoint qpos(x,y);
            app->processEvents();
            ((QtMainWindow*)hWnd)->popupMenuRequested(qpos);
            app->processEvents();
            id = ((QtMainWindow*)hWnd)->lastContextId;
            //qDebug() << "lastContextId : " << ((QtMainWindow*)hWnd)->lastContextId;
            ((QtMainWindow*)hWnd)->lastContextId = 0;
            if((uFlags & TPM_RETURNCMD) == TPM_RETURNCMD )
                return id;
            state.sendevent(new NewCommandEvent(id,0,1));
        }
        else if(((QMenu*)hMenu)->objectName() == "Popup1_Menu")
        {
            QPoint qpos(x,y);
            app->processEvents();
            ((QtMainWindow*)hWnd)->popup1MenuRequested(qpos);
            app->processEvents();
            id = ((QtMainWindow*)hWnd)->lastContextId;
            //qDebug() << "lastContextId : " << ((QtMainWindow*)hWnd)->lastContextId;
            ((QtMainWindow*)hWnd)->lastContextId = 0;
            if((uFlags & TPM_RETURNCMD) == TPM_RETURNCMD )
                return id;
            state.sendevent(new NewCommandEvent(id,0,1));
        }
    }
    return 0;//TrackPopupMenu( hMenu, uFlags, x, y, nReserved, hWnd, prcRect );
}

RCBOOL RCInsertMenu( RCHMENU hMenu,
    RCUINT uPosition,
    RCUINT uFlags,
    RCPTR uIDNewItem,
    RCLPCTSTR lpNewItem
)
{

    return 0;//InsertMenu( hMenu, uPosition, uFlags, uIDNewItem, lpNewItem );
}

RCBOOL RCInsertMenu( RCHMENU hMenu,
    RCUINT uPosition,
    RCUINT uFlags,
    RCPTR uIDNewItem,
    const char* lpNewItem
)
{
    return 0;//InsertMenu( hMenu, uPosition, uFlags, uIDNewItem, lpNewItem );
}

RCUINT RCDragQueryFile( RCHDROP hDrop,
    RCUINT iFile,
    RCLPTSTR lpszFile,
    RCUINT cch
)
{
    return 0;//DragQueryFile( hDrop, iFile, lpszFile, cch );
}

RCCHAR *RCITOA( RCINT value, RCCHAR *str, RCINT radix  )
{
    return 0;
}

RCHFONT RCCreateFontW(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  const char* lpszFace           // typeface name
                   )
{
    return 0;
}

RCHFONT RCCreateFontW(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  LPCTSTR lpszFace           // typeface name
                   )
{
    return 0;
}

RCHFONT RCCreateFontW(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  const RCCHAR* lpszFace           // typeface name
                   )
{
    return 0;
}

RCHFONT RCCreateFont(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  LPCTSTR lpszFace           // typeface name
                   )
{
    return RCCreateFont(    nHeight,               // height of font
                           nWidth,                // average character width
                           nEscapement,           // angle of escapement
                           nOrientation,          // base-line orientation angle
                           fnWeight,              // font weight
                           fdwItalic,           // italic attribute option
                           fdwUnderline,        // underline attribute option
                           fdwStrikeOut,        // strikeout attribute option
                           fdwCharSet,          // character set identifier
                           fdwOutputPrecision,  // output precision
                           fdwClipPrecision,    // clipping precision
                           fdwQuality,          // output quality
                           fdwPitchAndFamily,   // pitch and family
                           _RCTT(lpszFace)           // typeface name
                            );
}

RCHFONT RCCreateFont(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD BfdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  const char * lpszFace           // typeface name
                   )
{
    return 0;
}

RCHFONT RCCreateFont(
  RCINT nHeight,               // height of font
  RCINT nWidth,                // average character width
  RCINT nEscapement,           // angle of escapement
  RCINT nOrientation,          // base-line orientation angle
  RCINT fnWeight,              // font weight
  RCDWORD fdwItalic,           // italic attribute option
  RCDWORD fdwUnderline,        // underline attribute option
  RCDWORD fdwStrikeOut,        // strikeout attribute option
  RCDWORD fdwCharSet,          // character set identifier
  RCDWORD fdwOutputPrecision,  // output precision
  RCDWORD fdwClipPrecision,    // clipping precision
  RCDWORD fdwQuality,          // output quality
  RCDWORD fdwPitchAndFamily,   // pitch and family
  RCCHAR* lpszFace           // typeface name
                   )
{
    _rcfont *qfont = new _rcfont();
    if(qfont)
    {
        switch(HIWORD(fdwPitchAndFamily))
        {
        case FF_SWISS :
        case FF_MODERN :
            ((QFont*)qfont)->setStyleHint(QFont::SansSerif);
            break;
        case FF_ROMAN :
            ((QFont*)qfont)->setStyleHint(QFont::Serif);
            break;
        case FF_SCRIPT :
            qfont->setStyleHint(QFont::Cursive);
            break;
        case FF_DECORATIVE :
            ((QFont*)qfont)->setStyleHint(QFont::Decorative);
            break;
        case FF_DONTCARE :
        default :
            ((QFont*)qfont)->setStyleHint(QFont::AnyStyle);
        }

        switch(LOWORD(fdwPitchAndFamily))
        {
        case VARIABLE_PITCH :
            ((QFont*)qfont)->setFixedPitch(false);
            break;
        case FIXED_PITCH :
        case DEFAULT_PITCH :
        default :
            ((QFont*)qfont)->setFixedPitch(true);
            break;
        }

        ((QFont*)qfont)->setFamily(QString(lpszFace));

        return (RCHFONT)qfont;
    }
    else
        return 0;
}

RCBOOL RCDeleteObject(  RCHGDIOBJ hObject )
{
    if(hObject != 0)
    {
        delete hObject;
        return TRUE;
    }
    else
        return FALSE;
}

RCHGDIOBJ RCGetStockObject(  RCINT fnObject  )
{
    switch(fnObject)
    {
    case WHITE_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(255,255,255)));
        return (RCHBRUSH)rcbrush;
    }
    case GRAY_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(125,125,125)));
        return (RCHBRUSH)rcbrush;
    }
    case BLACK_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(0,0,0)));
        return (RCHBRUSH)rcbrush;
    }
    case LTGRAY_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(191,191,191)));
        return (RCHBRUSH)rcbrush;
    }
    case NULL_BRUSH :
        break;
    case BLACK_PEN :
    {
        _rcpen *rcpen = new _rcpen();
        rcpen->setColor(QColor(qRgb(0,0,0)));
        return (RCHPEN)rcpen;
    }
    case WHITE_PEN :
    {
        _rcpen *rcpen = new _rcpen();
        rcpen->setColor(QColor(qRgb(255,255,255)));
        return (RCHPEN)rcpen;
    }
    case NULL_PEN :
        break;
    }
    return 0;
}

RCHANDLE RCGetClipboardData( RCUINT uFormat )
{
    return 0;
}

RCLPVOID WINAPI RCGlobalLock( RCHGLOBAL hMem )
{
    return 0;
}

RCBOOL WINAPI RCGlobalUnlock( RCHGLOBAL hMem )
{
    return 0;
}

RCBOOL RCCloseClipboard(  )
{
    return 0;
}

RCHGLOBAL WINAPI RCGlobalAlloc( RCUINT uFlags, RCSIZE_T dwBytes )
{
    return 0;
}

RCHANDLE RCSetClipboardData( RCUINT uFormat, RCHANDLE hMem )
{
    return 0;
}

RCSHORT RCGetKeyState( RCINT nVirtKey )
{
    RCSHORT state = 0;
    // KMJ : Qt does not get modifiers during mouse wheel events
    // so we keep the last key down in the app object and look at that here
    int keymod = QGuiApplication::keyboardModifiers();
    if(keymod != QGuiApplication::queryKeyboardModifiers())
        keymod = QGuiApplication::queryKeyboardModifiers();
    //Qt::MouseButtons mousebuts = QGuiApplication::mouseButtons();
    Qt::MouseButtons mousebuts = app->mouseButtons();

    //if(mousebuts == Qt::LeftButton || app->mbtn == Qt::LeftButton)
    //    qDebug() << "RCGetKeyState : mousebuts        : left button";
    //else if(mousebuts == Qt::RightButton || app->mbtn == Qt::RightButton)
    //    qDebug() << "RCGetKeyState : mousebuts        : right button";
    //else if(mousebuts == Qt::MiddleButton || app->mbtn == Qt::MiddleButton)
    //    qDebug() << "RCGetKeyState : mousebuts        : middle button";
    //qDebug() << "RCGetKeyState : nVirtKey             : " << nVirtKey;
    //qDebug() << "RCGetKeyState : keymod               : " << keymod;
    //qDebug() << "RCGetKeyState : app->mlastkey      : " << app->mlastkey;

    if( nVirtKey == VK_SHIFT && (Qt::ShiftModifier & keymod) == Qt::ShiftModifier )
        return (state = (RCSHORT)0x8000);
    if( nVirtKey == VK_CONTROL && (Qt::ControlModifier & keymod) == Qt::ControlModifier )
        return (state = (RCSHORT)0x8000);
    //if( nVirtKey == VK_MENU && ((Qt::AltModifier & keymod) == Qt::AltModifier || (Qt::Key_Alt == app->mlastkey)) )
    if( nVirtKey == VK_MENU && (Qt::AltModifier & keymod) == Qt::AltModifier )
        return (state = (RCSHORT)0x8000);
    if( nVirtKey == VK_LBUTTON && (mousebuts == Qt::LeftButton || app->mbtn == Qt::LeftButton))
    {
        app->mbtn = Qt::NoButton;
        return (state = (RCSHORT)0x8000);
    }
    if( nVirtKey == VK_RBUTTON && (mousebuts == Qt::RightButton || app->mbtn == Qt::RightButton))
    {
        app->mbtn = Qt::NoButton;
        return (state = (RCSHORT)0x8000);
    }
    if( nVirtKey == app->mlastkey )
        return (state = (RCSHORT)0x8000);
    else
        return 0;
}

RCSHORT RCGetAsyncKeyState( int vKey )
{
    // just calling GetKeyState for now
   return RCGetKeyState( vKey );
}

RCDWORD WINAPI RCGetSysColor( RCINT nIndex )
{
    QColor qcolor = app->palette().color(QPalette::Normal,QPalette::Window);
    switch(nIndex)
    {
    case COLOR_BTNFACE :
        qcolor = app->palette().color(QPalette::Normal,QPalette::Button);
        return RGB(qcolor.red(),qcolor.green(),qcolor.blue());
        break;
    case COLOR_HIGHLIGHTTEXT :
         qcolor = app->palette().color(QPalette::Normal,QPalette::HighlightedText);
        return RGB(qcolor.red(),qcolor.green(),qcolor.blue());
        break;
    case COLOR_HIGHLIGHT :
         qcolor = app->palette().color(QPalette::Normal,QPalette::Highlight);
        return RGB(qcolor.red(),qcolor.green(),qcolor.blue());
        break;
    case COLOR_BTNSHADOW :
    //case COLOR_3DSHADOW :
         qcolor = app->palette().color(QPalette::Normal,QPalette::Dark);
        return RGB(qcolor.red(),qcolor.green(),qcolor.blue());
        break;
    case COLOR_BTNTEXT :
         qcolor = app->palette().color(QPalette::Normal,QPalette::ButtonText);
        return RGB(qcolor.red(),qcolor.green(),qcolor.blue());
        break;
    case COLOR_3DHIGHLIGHT :
         qcolor = app->palette().color(QPalette::Normal,QPalette::Dark);
        return RGB(qcolor.red(),qcolor.green(),qcolor.blue());
        break;
    }
    return 0;
}

RCBOOL RCReleaseCapture()
{
    return 1;
}

RCVOID RCPostQuitMessage( RCINT nExitCode )
{
    qApp->closeAllWindows();
    return;
}

RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, RCCHAR* lpCursorName )
{
    for(int i=0; i<RC_NUM_NAMED_CURSOR_RESOURCES; i++)
    {
        if(QString(RCnamedCursorResources[i].name).compare(QString(lpCursorName),Qt::CaseInsensitive) == 0
           && strcmp(RCnamedCursorResources[i].typeName,"CURSOR") == 0  )
        {
            QString qsname(RCnamedCursorResources[i].fileName);
            // if this is a bitmap cursor make the mask name
            QString qmname;
            int id=-1;
            if((id=qsname.indexOf("_cur")) >= 0)
            {
                qmname = qsname;
                qmname = qmname.replace(id,4,"_msk");
                //qDebug() << "Loading cursor file : " << qsname;
                //qDebug() << "Loading mask file   : " << qmname;mag

                RCCursor *cur=0;
#ifndef _MAC
#if 1
                //qInfo() << "HDPIXscale : " << RCGetHDPIXscale(0);
                int dpr = app->devicePixelRatio();
                int width = max(app->primaryScreen()->size().width(),app->primaryScreen()->size().height()) * dpr;

                if(  dpr > 1 || (dpr == 1 && width >2000) )
                {
                    if(dpr == 2)
                    {
                        qsname.replace(".","@2x.");
                        qmname.replace(".","@2x.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,8,54);
                        else
                            cur = new RCCursor(qsname,qmname);
                    }
                    else if(dpr == 3)
                    {
                        qsname.replace(".","@3x.");
                        qmname.replace(".","@3x.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,12,27);
                        else
                            cur = new RCCursor(qsname,qmname);
                    }
                    else
                    {
                        app->setAttribute(Qt::AA_UseHighDpiPixmaps,false);
                        qsname.replace(".","_medium.");
                        qmname.replace(".","_medium.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,8,54);
                        else
                            cur = new RCCursor(qsname,qmname);
                        app->setAttribute(Qt::AA_UseHighDpiPixmaps,true);
                    }
                }
                else
                {

                    if(qsname.contains("erase"))
                        cur = new RCCursor(qsname,qmname,4,27);
                    else
                        cur = new RCCursor(qsname,qmname);
                }

#endif
#else
                if(qsname.contains("erase"))
                    cur = new RCCursor(qsname,qmname,4,27);
                else
                    cur = new RCCursor(qsname,qmname);
#endif
                //qDebug() << "cursor is       : " << cur;
                //qDebug() << "cursor name  is : " << cur->m_name;
                //qDebug() << "cursor shape is : " << cur->shape();
                return cur;
            }
            else // assume it is a pixmap cursor
                return new RCCursor(qsname);
        }
    }
    return (RCHCURSOR)( new RCCursor(Qt::ArrowCursor));
}

RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, ULONG_PTR lpCursorName)
{
    for(int i=0; i<RC_NUM_NUMBERED_CURSOR_RESOURCES; i++)
    {
        if( RCnumberedCursorResources[i].id == lpCursorName  == 0 &&
            strcmp(RCnumberedCursorResources[i].typeName,"CURSOR") == 0  )
        {
            QString qsname(RCnumberedCursorResources[i].fileName);
            // if this is a bitmap cursor make the mask name
            QString qmname;
            int id=-1;
            if((id=qsname.indexOf("_cur")) >= 0)
            {
                qmname = qsname.replace(id,4,"_msk");
                RCCursor *cur=0;
#ifndef _MAC
#if 1
                //qInfo() << "HDPIXscale : " << RCGetHDPIXscale(0);
                int dpr = app->devicePixelRatio();
                int width = max(app->primaryScreen()->size().width(),app->primaryScreen()->size().height()) * dpr;

                if(  dpr > 1 || (dpr == 1 && width >2000) )
                {
                    if(dpr == 2)
                    {
                        qsname.replace(".","@2x.");
                        qmname.replace(".","@2x.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,8,54);
                        else
                            cur = new RCCursor(qsname,qmname);
                    }
                    else if(dpr == 3)
                    {
                        qsname.replace(".","@3x.");
                        qmname.replace(".","@3x.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,12,27);
                        else
                            cur = new RCCursor(qsname,qmname);
                    }
                    else
                    {
                        app->setAttribute(Qt::AA_UseHighDpiPixmaps,false);
                        qsname.replace(".","_medium.");
                        qmname.replace(".","_medium.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,8,54);
                        else
                            cur = new RCCursor(qsname,qmname);
                        app->setAttribute(Qt::AA_UseHighDpiPixmaps,true);
                    }
                }
                else
                {

                    if(qsname.contains("erase"))
                        cur = new RCCursor(qsname,qmname,4,27);
                    else
                        cur = new RCCursor(qsname,qmname);
                }

#endif
#else
                if(qsname.contains("erase"))
                    cur = new RCCursor(qsname,qmname,4,27);
                else
                    cur = new RCCursor(qsname,qmname);
#endif
                return new RCCursor(qsname,qmname);
            }
            else // assume it is a pixmap cursor
                return new RCCursor(qsname);
        }
    }
    return (RCHCURSOR)( new RCCursor(Qt::ArrowCursor));
}

RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, Qt::CursorShape qtCursorName)
{
    return new RCCursor(qtCursorName);
}

RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, char *  lpCursorName )
{
    return (RCHCURSOR)0;
}

RCHCURSOR RCLoadCursor( RCHINSTANCE hInstance, LPCTSTR lpCursorName )
{
    for(int i=0; i<RC_NUM_NAMED_CURSOR_RESOURCES; i++)
    {
        if( strcmp(RCnamedCursorResources[i].name,lpCursorName)  == 0 &&
            strcmp(RCnamedCursorResources[i].typeName,"CURSOR") == 0  )
        {
            QString qsname(RCnamedCursorResources[i].fileName);
            // if this is a bitmap cursor make the mask name
            QString qmname;
            int id=-1;
            if((id=qsname.indexOf("_cur")) >= 0)
            {
                qmname = qsname.replace(id,4,"_msk");
                RCCursor *cur=0;
#ifndef _MAC
#if 1
                //qInfo() << "HDPIXscale : " << RCGetHDPIXscale(0);
                int dpr = app->devicePixelRatio();
                int width = max(app->primaryScreen()->size().width(),app->primaryScreen()->size().height()) * dpr;

                if(  dpr > 1 || (dpr == 1 && width >2000) )
                {
                    if(dpr == 2)
                    {
                        qsname.replace(".","@2x.");
                        qmname.replace(".","@2x.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,8,54);
                        else
                            cur = new RCCursor(qsname,qmname);
                    }
                    else if(dpr == 3)
                    {
                        qsname.replace(".","@3x.");
                        qmname.replace(".","@3x.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,12,27);
                        else
                            cur = new RCCursor(qsname,qmname);
                    }
                    else
                    {
                        app->setAttribute(Qt::AA_UseHighDpiPixmaps,false);
                        qsname.replace(".","_medium.");
                        qmname.replace(".","_medium.");
                        if(qsname.contains("erase"))
                            cur = new RCCursor(qsname,qmname,8,54);
                        else
                            cur = new RCCursor(qsname,qmname);
                        app->setAttribute(Qt::AA_UseHighDpiPixmaps,true);
                    }
                }
                else
                {

                    if(qsname.contains("erase"))
                        cur = new RCCursor(qsname,qmname,4,27);
                    else
                        cur = new RCCursor(qsname,qmname);
                }

#endif
#else
                if(qsname.contains("erase"))
                    cur = new RCCursor(qsname,qmname,4,27);
                else
                    cur = new RCCursor(qsname,qmname);
#endif
                return new RCCursor(qsname,qmname);
            }
            else // assume it is a pixmap cursor
                return new RCCursor(qsname);
        }
    }
    return (RCHCURSOR)( new RCCursor(Qt::ArrowCursor));
}

RCHBRUSH RCCreateSolidBrush(  RCCOLORREF crColor )
{
    _rcbrush *rcbrush = new _rcbrush();
    rcbrush->setColor(QColor(qRgb(GetRValue(crColor),GetGValue(crColor),GetBValue(crColor))));
    return (RCHBRUSH)rcbrush;
}

RCHCURSOR RCSetCursor( RCHCURSOR hCursor )
{
    // KMJ : not needed
    RCHCURSOR old = (RCHCURSOR)app->overrideCursor();
    if(old != 0)
    {
        app->restoreOverrideCursor();
    }
    if(hCursor)
    {
        app->setOverrideCursor((QCursor&)*hCursor);
    }
    return old;
}

RCHRESULT Sleep( RCDWORD  dwMilliseconds )
{
    return 0;
}

RCDWORD RCtimeGetTime()
{
    QTime qtime = QTime::currentTime();
    int time = qtime.msecsSinceStartOfDay();
    //qDebug() << "getTime : " << time;
    return time;
}

/*
time_t time( time_t * _Time)
{
    return 0;
}
*/

RCBOOL RCEmptyClipboard()
{
    return 0;
}

RCSIZE_T WINAPI RCGlobalSize( HGLOBAL hMem )
{
    return 0;
}

u_short RChtons( u_short hostshort )
{
    return 0;
}

WINSOCK_API_LINKAGE SOCKET WSAAPI RCsocket( int af, int type, int protocol )
{
    return 0;
}

WINSOCK_API_LINKAGE int WSAAPI RCconnect( SOCKET s,
                                        const struct sockaddr FAR * name,
                                        int namelen
                                        )
{
    return 0;
}

WINSOCK_API_LINKAGE int WSAAPI closesocket( SOCKET s )
{
    return 0;
}

RCBOOL RCGetCursorPos( RCLPPOINT lpPoint )
{
    QPoint cp = QCursor::pos();
    lpPoint->x = cp.x();
    lpPoint->y = cp.y();
    return true;
    /*
    QWidget *desktop = app->desktop()->screen(app->desktop()->screenNumber(cadwindow->getWidget()));
    if(desktop)
    {
        cp = desktop->cursor().pos();
        //if(cadwindow && cadwindow->getWidget())
        //    cp = ((QtMainWindow*)cadwindow->getWidget())->cursor().pos();
        lpPoint->x = cp.x();
        lpPoint->y = cp.y();
        return true;
    }
    QCursor *appcur = app->overrideCursor();
    QPoint cp;
    if(appcur)
    {
        cp = appcur->pos();
        lpPoint->x = cp.x();
        lpPoint->y = cp.y();
        return true;
    }
    else
    {
        if(cadwindow && cadwindow->getWidget())
            cp = ((QtMainWindow*)cadwindow->getWidget())->cursor().pos();
        lpPoint->x = cp.x();
        lpPoint->y = cp.y();
        return true;
    }
    lpPoint->x = 0;
    lpPoint->y = 0;
    */
    return 0;
}

RCHWND RCWindowFromPoint( RCPOINT Point )
{
    QWidget *qw = app->widgetAt(Point.x,Point.y);
    return (RCHWND)qw;
}

RCLONG WINAPI RCRegCloseKey( RCHKEY hKey )
{
    return 0;
}

RCDWORD WINAPI RCGetVersion()
{
    return 3;
}

RCHMODULE WINAPI RCGetModuleHandle( RCLPCTSTR lpModuleName )
{
    return (RCHMODULE*)program->getloadedapp(lpModuleName);
}

RCHMODULE WINAPI RCGetModuleHandle( LPCTSTR lpModuleName )
{
    return 0;
}

RCHMODULE WINAPI RCGetModuleHandle( const char * lpModuleName )
{
    return (RCHMODULE*)program->getloadedapp(QString(lpModuleName).data());
}

RCBOOL RCGetUpdateRect(
        RCHWND hWnd,       // handle to window
        RCLPRECT lpRect,   // update rectangle coordinates
        RCBOOL bErase      // erase state
        )
{
    return 0;
}

RCHDC RCBeginPaint( RCHWND hwnd, RCLPPAINTSTRUCT lpPaint )
{
    return 0;
}

RCBOOL RCEndPaint( RCHWND hWnd, CONST RCPAINTSTRUCT *lpPaint )
{
    return 0;
}

RCLRESULT RCDefWindowProc(
        RCHWND hWnd,
        RCUINT Msg,
        RCWPARAM wParam,
        RCLPARAM lParam
        )
{
    return 0;
}

RCINT RCLoadString(
        RCHINSTANCE hInstance,
        RCUINT uID,
        RCLPTSTR lpBuffer,
        RCINT nBufferMax
)
{
    return 0;
}

RCINT RCLoadString(
        RCHINSTANCE hInstance,
        RCUINT uID,
        LPTSTR lpBuffer,
        RCINT nBufferMax
)
{
    //LoadString(program->gethinst(),id,string,DEFAULT_RESOURCE_STRING_LENGTH)
    // find the id in the resource string table
    for(int i=0; i<RC_NUM_NUMBERED_STRING_RESOURCES; i++)
    {
        if( RCnumberedStringResources[i].id == uID )
        {
            strncpy(lpBuffer,RCnumberedStringResources[i].str,nBufferMax);
            return strlen(lpBuffer);
        }
    }
    return 0;
}

RCINT RCLoadString(
        RCHINSTANCE hInstance,
        LPTSTR lpName,
        LPTSTR lpBuffer,
        RCINT nBufferMax
)
{
    //LoadString(program->gethinst(),id,string,DEFAULT_RESOURCE_STRING_LENGTH)
    // find the id in the resource string table
    for(int i=0; i<RC_NUM_NAMED_STRING_RESOURCES; i++)
    {
        if( strcmp(RCnamedStringResources[i].name,lpName) == 0 )
        {
            strncpy(lpBuffer,RCnamedStringResources[i].str,nBufferMax);
            return strlen(lpBuffer);
        }
    }
    return 0;
}

RCHBRUSH RCGetSysColorBrush(  RCINT nIndex  )
{
    switch(nIndex)
    {
    case WHITE_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(255,255,255)));
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    case GRAY_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(125,125,125)));
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    case BLACK_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(0,0,0)));
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    case LTGRAY_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(QColor(qRgb(191,191,191)));
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    case NULL_BRUSH :
    {
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setStyle(Qt::NoBrush);
        return (RCHBRUSH)rcbrush;
        break;
    }
    case COLOR_BTNFACE :
    {
        QColor qcolor = app->palette().color(QPalette::Normal,QPalette::Button);
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(qcolor);
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    case COLOR_HIGHLIGHTTEXT :
    {
         QColor qcolor = app->palette().color(QPalette::Normal,QPalette::HighlightedText);
         _rcbrush *rcbrush = new _rcbrush();
         rcbrush->setColor(qcolor);
         rcbrush->setStyle(Qt::SolidPattern);
         return (RCHBRUSH)rcbrush;
    }
    case COLOR_HIGHLIGHT :
    {
         QColor qcolor = app->palette().color(QPalette::Normal,QPalette::Highlight);
         _rcbrush *rcbrush = new _rcbrush();
         rcbrush->setColor(qcolor);
         rcbrush->setStyle(Qt::SolidPattern);
         return (RCHBRUSH)rcbrush;
     }
    case COLOR_BTNSHADOW :
    //case COLOR_3DSHADOW :
    {
        QColor qcolor = app->palette().color(QPalette::Normal,QPalette::Dark);
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(qcolor);
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    case COLOR_BTNTEXT :
    {
        QColor qcolor = app->palette().color(QPalette::Normal,QPalette::ButtonText);
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(qcolor);
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    case COLOR_3DHIGHLIGHT :
    {
        QColor qcolor = app->palette().color(QPalette::Normal,QPalette::Dark);
        _rcbrush *rcbrush = new _rcbrush();
        rcbrush->setColor(qcolor);
        rcbrush->setStyle(Qt::SolidPattern);
        return (RCHBRUSH)rcbrush;
    }
    }
}

RCBOOL WINAPI RCGetUserName( RCLPSTR lpBuffer, RCLPDWORD lpnSize )
{
#ifdef _MAC
    QString name;
    GetUserNameMac(name);
    strcpy(lpBuffer,name);
    return (name.length() > 0);
#else
    char wlpBuffer[UNLEN + 1]={0};
    GetUserNameA(wlpBuffer,lpnSize);
    strcpy(lpBuffer,wlpBuffer);
    return (lpnSize > 0);
#endif
}

RCHMENU RCLoadMenu( RCHINSTANCE hInstance, RCLPCTSTR lpMenuName )
{
    return 0;
}

RCHMENU RCLoadMenu( RCHINSTANCE hInstance, LPCTSTR lpMenuName )
{
    return 0;
}

RCBOOL RCDestroyMenu( RCHMENU hMenu )
{
    return 0;
}

RCBOOL RCSetCursorPos( RCINT X, RCINT Y )
{
    return 0;
}

RCBOOL RCDrawMenuBar( RCHWND hWnd )
{
    return 0;
}

RCBOOL RCIsZoomed( RCHWND hWnd )
{
    return 0;
}

RCVOID RCDragFinish( RCHDROP hDrop )
{
    return ;
}

RCLRESULT RCCallNextHookEx(
        RCHHOOK hhk,
        RCINT nCode,
        RCWPARAM wParam,
        RCLPARAM lParam
)
{
    return 0;
}

RCHACCEL RCLoadAccelerators( RCHINSTANCE hInstance, RCLPCTSTR lpTableName )
{
    if(cadwindow != 0 && cadwindow->gethwnd() != 0)
    {
        qDebug() << "size of shortcuts : " << ((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts.size();
        RCHACCEL accel = new RCVACCEL(((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts.size());
        int i=0;
        RCVACCELITER iter = accel->begin();
        for(i=0,iter=accel->begin(); i<((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts.size(); i++,++iter)
        {
            QShortcut *qshortcut = ((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts.at(i);
            if(qshortcut != 0)
            {
                qDebug() << "shortcut id : " << qshortcut->id();
                qDebug() << "what is this : " << qshortcut->whatsThis();
                qDebug() << "key sequence : " << qshortcut->key().toString();
                qDebug() << "";

                RCACCEL ac = *iter;
                ac.whatsThis = qshortcut->whatsThis();
                QString kstring = qshortcut->key().toString();
                ac.kstring = kstring;
                ac.fVirt = FVIRTKEY; // always using vkey
                if(kstring.toUpper().contains("CTRL"))
                    ac.fVirt |= FCONTROL;
                if(kstring.toUpper().contains("SHIFT"))
                    ac.fVirt |= FSHIFT;
                if(kstring.toUpper().contains("ALT"))
                    ac.fVirt |= FALT;
                // split the string on +
                QStringList ksequence = kstring.split('+');
                QString skey;
                if(!ksequence.isEmpty())
                    skey = ksequence.back();
                ac.key = skey.data()->toLatin1();
                ac.cmd = qshortcut->property("cmid").toInt();
                *iter = ac;
            }
        }
        return (RCHACCEL)accel;
    }
    return 0;
}

RCBOOL RCUnregisterClass( RCLPCTSTR lpClassName, RCHINSTANCE hInstance )
{
    return 0;
}

RCLONG WINAPI RRCegCreateKey(
        RCHKEY hKey,
        RCLPCTSTR lpSubKey,
        RCPHKEY phkResult
)
{
    return 0;
}

RCLONG WINAPI RCRegDeleteKey( RCHKEY hKey, RCLPCTSTR lpSubKey )
{
    return 0;
}

RCINT RCWSAStartup(  RCWORD wVersionRequested, LPWSADATA lpWSAData )
{
    return 0;
}

RCLONG WINAPI RCRegSetValueEx(
        RCHKEY hKey,
        LPCTSTR lpValueName,
        RCDWORD Reserved,
        RCDWORD dwType,
        const RCBYTE* lpData,
        RCDWORD cbData
        )
{
    return 0;
}

RCLONG WINAPI RCRegDeleteValue(
        RCHKEY hKey,
        LPCTSTR lpValueName
        )
{
    return 0;
}

RCLONG WINAPI RCRegCreateKey(
        RCHKEY hKey,
        LPCTSTR lpSubKey,
        RCPHKEY phkResult
        )
{
    return 0;
}

RCLONG WINAPI RCRegDeleteKey(
        RCHKEY hKey,
        LPCTSTR lpSubKey
        )
{
    return 0;
}

RCDWORD WINAPI RCGetTickCount(void)
{
    return 0;
}

RCBOOL RCPtInRect(  CONST RCRECT *lprc,  // rectangle
                    RCPOINT pt           // point
                  )
{
    QRect rect(lprc->left,lprc->top,lprc->right-lprc->left,lprc->bottom-lprc->top);
    bool status = rect.contains(QPoint(pt.x,pt.y));
    return status;
}

RCINT RCGetDlgCtrlID( RCHWND hwndCtl )
{
    if(hwndCtl)
        return RCnameToInt(((QWidget*)hwndCtl)->objectName());
    return 0;
}

RCBOOL RCGetMonitorInfo(  HMONITOR hMonitor,  LPMONITORINFO lpmi  )
{
    return 0;
}

RCHWND RCGetDesktopWindow(void)
{
    return 0;
}

HMONITOR RCMonitorFromRect(  const RECT *lprc, DWORD dwFlags )
{
    return 0;
}

HGLOBAL WINAPI RCGlobalFree( HGLOBAL hMem )
{
    return 0;
}

HHOOK RCSetWindowsHookEx(
        int idHook,
        HOOKPROC lpfn,
        RCHINSTANCE hMod,
        DWORD dwThreadId
        )
{
    return 0;
}

DWORD WINAPI RCGetCurrentThreadId(void)
{
    return 0;
}

BOOL RCUnhookWindowsHookEx( HHOOK hhk )
{
    return 0;
}

//#define GetRValue(rgb)      (LOBYTE(rgb))
//#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
//#define GetBValue(rgb)      (LOBYTE((rgb)>>16))
BYTE RCGetRValue(  DWORD rgb )
{
    return LOBYTE(rgb);
}

BYTE RCGetGValue(  DWORD rgb )
{
    return LOBYTE(((WORD)(rgb)) >> 8);
}

BYTE RCGetBValue(  DWORD rgb  )
{
    return LOBYTE((rgb)>>16);
}

RCHPEN RCCreatePen(
        int fnPenStyle,    // pen style
        int nWidth,        // pen width
        RCCOLORREF crColor   // pen color
        )
{
    _rcpen *qpen = new _rcpen();
    if(qpen)
    {
        switch(fnPenStyle)
        {
        case PS_SOLID :
        default :
            ((QPen*)qpen)->setStyle(Qt::SolidLine);
            break;
        case PS_DASH :
            ((QPen*)qpen)->setStyle(Qt::DashLine);
            break;
        case PS_DOT :
            ((QPen*)qpen)->setStyle(Qt::DotLine);
            break;
        case PS_DASHDOT :
            ((QPen*)qpen)->setStyle(Qt::DashDotLine);
            break;
        case PS_DASHDOTDOT :
            ((QPen*)qpen)->setStyle(Qt::DashDotDotLine);
            break;
        case PS_NULL :
        case PS_INSIDEFRAME :
            ((QPen*)qpen)->setStyle(Qt::NoPen);
        }
        ((QPen*)qpen)->setWidth(nWidth);
        ((QPen*)qpen)->setColor(QColor(RCGetRValue(crColor),RCGetGValue(crColor),RCGetBValue(crColor)));
        return qpen;
    }
    else
        return NULL;
}

BOOL RCGdiFlush(void)
{
    return 0;
}

int RCGetObject(
        HGDIOBJ hgdiobj,  // handle to graphics object
        int cbBuffer,     // size of buffer for object information
        LPVOID lpvObject  // buffer for object information
        )
{
    return 0;
}

int WINAPI RCGetSystemMetrics( int nIndex )
{

    // #define SM_CXSCREEN         0
    // #define SM_CYSCREEN         1
    // #define SM_CXMAXIMIZED      61
    // #define SM_CYMAXIMIZED      62
    // #define SM_CYCAPTION        4
    // #define SM_CXFRAME          32
    // #define SM_CYFRAME          33
    // #define SM_CYMINIMIZED      58
    // #define SM_CXDLGFRAME       7

    switch(nIndex)
    {
    case SM_CXSCREEN :
        return app->desktop()->screenGeometry().width();
        break;
    case SM_CYSCREEN :
        return app->desktop()->screenGeometry().height();
        break;
    case SM_CXMAXIMIZED :
        return app->desktop()->screen()->maximumWidth();
        break;
    case SM_CYMAXIMIZED :
        return app->desktop()->screen()->maximumHeight();
        break;
    }

    return 0;
}


RCHFONT RCCreateFontIndirect(  CONST LOGFONT* lplf  )
{
    return 0;
}

RCHFONT RCCreateFontIndirectW(  CONST LOGFONTW* lplf  )
{
    return 0;
}

RCHRGN RCCreateRectRgn(  int nLeftRect,   // x-coordinate of upper-left corner
                     int nTopRect,    // y-coordinate of upper-left corner
                     int nRightRect,  // x-coordinate of lower-right corner
                     int nBottomRect  // y-coordinate of lower-right corner
                     )
{
    return 0;
}

MCIERROR RCmciSendCommand(
        MCIDEVICEID IDDevice,
        UINT        uMsg,
        DWORD       fdwCommand,
        DWORD_PTR   dwParam
        )
{
    return 0;
}

BOOL RCEnumDisplayMonitors(
        HDC hdc,                   // handle to display DC
        const RECT* lprcClip,          // clipping rectangle
        MONITORENUMPROC lpfnEnum,  // callback function
        LPARAM dwData              // data for callback function
        )
{
    return 0;
}

RCHICON RCLoadIcon( RCHINSTANCE hInstance, RCSTRING lpIconName )
{
    return QIcon("");
}

RCHICON RCLoadIcon( RCHINSTANCE hInstance, LPTSTR lpIconName )
{
    if(hInstance != 0){
        for(int i=0; i<RC_NUM_NAMED_ICON_RESOURCES; i++)
        {
            if( strcmp(RCnamedIconResources[i].name,lpIconName)  == 0 &&
                strcmp(RCnamedIconResources[i].typeName,"ICON") == 0  )
            {
                QString qsname(RCnamedIconResources[i].fileName);
                return QIcon(qsname);
            }
        }
        return QIcon("");
    }
    else{
        return QIcon("");
    }
}


RCHICON RCLoadIcon( RCHINSTANCE hInstance, ULONG_PTR lpIconName )
{
    if(hInstance != 0){
        int num = RC_NUM_NUMBERED_ICON_RESOURCES;
        for(int i=0; i<RC_NUM_NUMBERED_ICON_RESOURCES; i++)
        {
            if( RCnumberedIconResources[i].id == lpIconName  &&
                strcmp(RCnumberedIconResources[i].typeName,"ICON") == 0  )
            {
                QString qsname(RCnumberedIconResources[i].fileName);
                return QIcon(":/images/"+qsname);
            }
        }
        return QIcon("");
    }
    else{
        return QIcon("");
    }
}


UINT WINAPI RCSetErrorMode( UINT uMode )
{
    return 0;
}

int RCCopyAcceleratorTable(
        RCHACCEL hAccelSrc,
        RCVPACCEL lpAccelDst,
        int cAccelEntries
        )
{
    if(hAccelSrc == 0)
        return 0;
    int len = hAccelSrc->size();
    if(lpAccelDst == 0)
        return len;
    int len2 = lpAccelDst->size();
    if(len2 == 0)
        return 0;
    RCVACCELITER it1,it2;
    int i=0;
    for(i=0,it1=hAccelSrc->begin(),it2=lpAccelDst->begin(); i < cAccelEntries; i++,++it1,++it2)
    {
        if(i > len)
            break;
        *it2 = *it1;
    }

    return i;
}

RCHACCEL RCCreateAcceleratorTable(
        RCVPACCEL lpaccl,
        int cEntries
        )
{
    if(lpaccl == 0)
        return 0;
    if(cadwindow != 0 && cadwindow->gethwnd() != 0)
    {
        if(cEntries == 0)
            return 0;
        if(cadwindow->gethwnd() != 0)
        {
            int i=0;
            for(i=0; i<((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts.size(); i++)
            {
                QShortcut *oldsc = ((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts[i];
                QObject::disconnect(oldsc,SIGNAL(activated()),(QtMainWindow*)cadwindow->gethwnd(),SLOT(processShortcutCommand()));
                oldsc->setParent(0);
                oldsc->deleteLater();
            }
            ((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts.clear();
            RCVACCEL *newaccel = new RCVACCEL(cEntries);
            RCVACCELITER it1,it2;
            for(i=0,it1=newaccel->begin(),it2=lpaccl->begin(); i < cEntries; i++,++it1,++it2)
            {
                *it1 = *it2;
                // update the Qt accelerators
                RCACCEL ac = *it1;
                ac.kstring;
                if(ac.kstring.isEmpty())
                {
                    // built the string from the ac data
                    if((ac.fVirt & FSHIFT) != 0)
                        ac.kstring = "Shift+";
                    if((ac.fVirt & FALT) != 0)
                        ac.kstring = "Alt+";
                    if((ac.fVirt & FCONTROL) != 0)
                        ac.kstring = "Ctrl+";
                    if((ac.fVirt & FVIRTKEY) != 0)
                        ac.kstring += QKeySequence(ac.key).toString();//QString(ac.key);
                    *it1 = ac;
                }
                QKeySequence qkeyseq(ac.kstring);
                QShortcut *qscut=0;
                qscut = new QShortcut(qkeyseq,(QtMainWindow*)cadwindow->gethwnd());
                qscut->setContext(Qt::WindowShortcut);
                qscut->setProperty("cmid",ac.cmd);
                QObject::connect(qscut,SIGNAL(activated()),(QtMainWindow*)cadwindow->gethwnd(),SLOT(processShortcutCommand()));
                ((QtMainWindow*)cadwindow->gethwnd())->mWindowShortcuts.append(qscut);
            }

            return (RCHACCEL)newaccel;
        }
    }
    else
        return 0;
}

BOOL RCDestroyAcceleratorTable( RCHACCEL hAccel )
{
    delete hAccel;
    return 0;
}

void WINAPI RCGlobalMemoryStatus( LPMEMORYSTATUS lpBuffer )
{
    return;
}

HRESULT RCStgOpenStorage(   const WCHAR* pwcsName,   IStorage* pstgPriority,
  DWORD grfMode,   SNB snbExclude,   DWORD reserved,   IStorage** ppstgOpen )
{
    return 0;
}

HRESULT RCStgCreateDocfile(
  const WCHAR* pwcsName,
  DWORD grfMode,
  DWORD reserved,
  IStorage** ppstgOpen
)
{
    return 0;
}

HDC RCCreateDC(  LPCTSTR lpszDriver,        // driver name
  LPCTSTR lpszDevice,        // device name
  LPCTSTR lpszOutput,        // not used; should be NULL
  CONST DEVMODE* lpInitData  // optional printer data
               )
{
    return 0;
}

BOOL RCGetVersionEx(  LPOSVERSIONINFO lpVersionInformation )
{
    return 0;
}

int RCMultiByteToWideChar(
        UINT CodePage,
        DWORD dwFlags,
        LPCSTR lpMultiByteStr,
        int cbMultiByte,
        LPWSTR lpWideCharStr,
        int cchWideChar
        )
{
    return 0;
}

int RCMultiByteToWideChar(
        UINT CodePage,
        DWORD dwFlags,
        LPCSTR lpMultiByteStr,
        int cbMultiByte,
        RCCHAR* lpWideCharStr,
        int cchWideChar
        )
{
    return 0;
}
UINT RCGetACP(void)
{
    return 0;
}

HLOCAL RCLocalAlloc(  UINT uFlags,   UINT uBytes )
{
    return 0;
}

DWORD WINAPI RCWaitForSingleObject( HANDLE hHandle, DWORD dwMilliseconds )
{
    return 0;
}

char *RCgetenv( const char *varname  )
{
    return getenv(varname);
}

RCCHAR *RCgetenv( const RCCHAR *varname  )
{
    return (RCCHAR*)getenv((char*)varname);
}
/*
IJLERR RCijlInit( JPEG_CORE_PROPERTIES* jcprops )
{
#ifdef _MAC
    return (IJLERR)0;
#else
    return ijlInit( jcprops );
#endif
}

IJLERR RCijlFree( JPEG_CORE_PROPERTIES* jcprops )
{
#ifdef _MAC
    return (IJLERR)0;
#else
    return ijlFree( jcprops );
#endif
}

IJLERR RCijlWrite( JPEG_CORE_PROPERTIES* jcprops, IJLIOTYPE iotype )
{
#ifdef _MAC
    return (IJLERR)0;
#else
    return ijlWrite( jcprops, iotype );
#endif
}
*/
RCLPWSTR *RCCommandLineToArgvW( RCLPCWSTR lpCmdLine, RCINT *pNumArgs )
{
    // count the number of
    return 0;//CommandLineToArgvW( lpCmdLine, pNumArgs );
}

/// Converts path seperator characters to Qt seperator characters
RCSTRING RCFixupPathSeperator(RCSTRING path)
{
    // look for any un-escaped windows seperators
    // and excape them
    QString escaped = path.replace("\\\\","\\");
    escaped = escaped.replace("\\","/");
    return QDir::toNativeSeparators(escaped);
}

RCSTRING RCFixupPathSeperator(RCCHAR *path)
{
    return RCFixupPathSeperator(RCSTRING(path));
}

RCINT RCGETPRIVATEPROFILESTRING( QString App, QString Key, QString Default, QString &ReturnedString ,DWORD nSize , QString FileName)
{
    // uses the QSettings class
    // behaves like the Windows function of the same name
    // if lpAppname id NULL return all section names seperated by \0
    // if lpKeyName is NULL return all key name in the section separated by \0
    QSettings settings(FileName,QSettings::IniFormat);
    QString qkey;

    //settings.sync();

    if(App.length() == 0) // return all section names
    {
        // get the list of section names
        QStringList groups = settings.childGroups();
        // break them out into a list of null seperated RCCHAR strings
        ReturnedString = groups.join("");
        int slen = ReturnedString.length();
        if( slen > (int)nSize )
        {
            ReturnedString[slen] = QChar::Null;
            ReturnedString[slen-1] = QChar::Null;
            return (int)nSize;
        }
        return ReturnedString.length();
    }
    else if( Key.length() == 0) // return all key names in this section
    {
        // get the list of key names for the given section
        settings.beginGroup(App);
        QStringList keys = settings.childKeys();
        settings.endGroup();
        // break them out into a list of null seperated RCCHAR strings
        ReturnedString = keys.join(",");
        int slen = ReturnedString.length();
        ReturnedString.replace(",",QString(QChar::Null));
        if( slen > (int)nSize )
        {
            ReturnedString[slen] = QChar::Null;
            ReturnedString[slen-1] = QChar::Null;
            return (int)nSize;
        }
        return slen;
    }
    else // normal section/key behaviour
    {
        QString ret;
        qkey = App + "/" + Key;
        ret = settings.value(qkey,Default).toStringList().join(",");
        ReturnedString = ret;
        int slen = ReturnedString.length();
        if( slen > (int)nSize )
        {
            ReturnedString[slen] = QChar::Null;
            ReturnedString[slen-1] = QChar::Null;
            return (int)nSize;
        }
        return slen;
    }
    ReturnedString = "";
    return 0; // failure
    //return GetPrivateProfileStringA(  (LPCSTR)lpAppName, (LPCSTR)lpKeyName, (LPCSTR)lpDefault, (LPSTR)lpReturnedString ,  nSize , (LPCSTR)lpFileName);
}

RCBOOL RCWRITEPRIVATEPROFILESTRING( const RCCHAR * App, const RCCHAR * Key, const RCCHAR *  String, const RCCHAR *  FileName )
{
    QString qkey,inapp(App),inkey(Key),instring(String),infile(FileName);
    QSettings settings(infile,QSettings::IniFormat);

    //settings.sync();

    qkey = inapp + QString("/") + inkey;
    settings.setValue(qkey,instring);

    return true;
    //return WritePrivateProfileStringA(  (LPCSTR)lpAppName,  (LPCSTR)lpKeyName, (LPCSTR)lpString, (LPCSTR)lpFileName );
}
RCINT RCSTRNCMP( const RCCHAR *string1, const RCCHAR *string2, RCSIZE_T count )
{
    const RCCHAR *cs;
    RCCHAR *cd;
    cd = (RCCHAR*)string2;
    int cnt=0;
    if(*string1 == 0) return -1;
    for( cs=string1; *cs != 0 && cnt < count ; cs++)
    {
        if(*cd != *cs)
        {
            return *cs > *cd ? 1 : -1;
        }
        cd++;
        cnt++;
    }
    return 0;
    //_tcsncicmp( (const char*)string1, (const char*)string2, count  );
}

RCINT RCfprintf( FILE *stream, const RCCHAR *format , ...)
{
    va_list args;
    va_start(args, format);
    RCSTRING rcformat(format);
    wchar_t *wstring = new wchar_t[rcformat.length()+1];
    rcformat.toWCharArray(wstring);
    wstring[rcformat.length()+1]=0;
    RCINT ret = vfwprintf( stream, wstring , args );
    va_end(args);
    delete [] wstring;
    return ret;
}

RCUINT RCGETWINDOWSDIRECTORY( RCPCHAR lpBuffer, RCUINT uSize )
{
#ifdef _MAC
    QString qlpBuffer(lpBuffer);
    QString rootpath = QDir::rootPath();
    if(uSize < rootpath.length())
        return rootpath.length();
    RCSTRCPY(lpBuffer,rootpath.data());
    return rootpath.length();
#else
    char clpBuffer[MAX_PATH];
    RCUINT count = GetWindowsDirectoryA( clpBuffer,  MAX_PATH);
    if(uSize < count)
        return count;
    //QString qlpBuffer(clpBuffer);
    //lpBuffer = QString(clpBuffer).data();
    strcpy(lpBuffer,clpBuffer);
    return strlen(lpBuffer);
#endif
}

RCUINT RCGETWINDOWSDIRECTORY( LPWSTR lpBuffer, RCUINT uSize )
{
#ifdef _MAC
    return 0;
#else
    return GetWindowsDirectoryW( (LPWSTR)lpBuffer,  uSize);
#endif
}

RCUINT RCGETWINDOWSDIRECTORY( char * lpBuffer, UINT uSize )
{
#ifdef _MAC
    QString qlpBuffer(lpBuffer);
    QString rootpath = QDir::rootPath();
    if(sizeof(lpBuffer) < rootpath.length())
        return rootpath.length();
    strcpy(lpBuffer,rootpath.toLatin1().data());
    return rootpath.length();
#else
    return GetWindowsDirectoryA( lpBuffer,  uSize);
#endif
}

#undef RCSSCANF

#define sscanf RCSSCANF

/*
the windows way to get the current screen scaling factor
HDC hdc = GetDC(NULL);
if (hdc)
{
    _dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
    _dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(NULL, hdc);
}

int ScaleX(int x) { _Init(); return MulDiv(x, _dpiX, 96); }
int ScaleY(int y) { _Init(); return MulDiv(y, _dpiY, 96); }
*/

RCINT RCGetHDPIXscale(RCHWND hWnd)
{
#ifdef _MAC
    return 1;
#else
    int _dpiX = 96;
    if(hWnd == 0)
        _dpiX = app->primaryScreen()->logicalDotsPerInchX();
    else
        _dpiX = ((QWidget*)hWnd)->logicalDpiX();
    int scale = _dpiX/96 + (_dpiX % 96 != 0);
    return scale;
#endif
}

RCINT RCGetHDPIYscale(RCHWND hWnd)
{
#ifdef _MAC
    return 1;
#else
    int _dpiY = 96;
    if(hWnd == 0)
        _dpiY = app->primaryScreen()->logicalDotsPerInchY();
    else
        _dpiY = ((QWidget*)hWnd)->logicalDpiY();
    int scale = _dpiY/96 + (_dpiY % 96 != 0);
    return scale;
#endif
}

QString elideTextMiddle(const QString inString, int elideWidth)
{
    QString elidString("...");
    QString outString;
    if(elideWidth < elidString.length() + 2)
        return inString;
    if(inString.length() < elideWidth - elidString.length() + 2)
        return inString;
    int leftLen  = (elideWidth - elidString.length()) / 2;
    int rightLen = (elideWidth - elidString.length()) / 2;
    outString = inString.left(leftLen) + elidString + inString.right(rightLen);
    return outString;
}
