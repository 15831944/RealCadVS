#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTGlobal>
#if (QT_VERSION < 0x50000)
#pragma message( "QT_VERSION < 0x50000" )
#include <QtGui/QMainWindow>
#else
#pragma message( "QT_VERSION >= 0x50000" )
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSizeGrip>
#include <QtWidgets/QShortcut>
#include <QMdiSubWindow>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewWidget>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QSharedPointer>
#endif
#include "RCSplashScreen_Dialog.h"
//QT_BEGIN_NAMESPACE
#define MAX_OPENLIST 9

class RCMdiArea;
class RCCentralWidget;
class RCView;
class View3dWindow;
//class RCSplashScreen_Dialog;

namespace Ui {
class _QtMainWindow;
}
class QtMainWindow : public QMainWindow
{
    friend class MainWindow;
    Q_OBJECT

public:
    QString curFile;
    bool geomRestored;
    bool stateRestored;
    int lastContextId;
    int isTiled;
    RCMdiArea *MdiArea;
    RCCentralWidget *centerWidget;
    RCSplashScreen_Dialog *m_splashScreen;

public:
    explicit QtMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~QtMainWindow();

    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    void readSettings();
    void saveSettings();
    bool isClosing() { return m_isClosing; }
    QString strippedName(const QString &fullFileName);
    void hideSplashScreen();
    void deleteSplashScreen();
    void setSplashScreen(RCSplashScreen_Dialog *ssdlg) { m_splashScreen=ssdlg; }
    RCSplashScreen_Dialog* SplashScreen() { return m_splashScreen; }
    void resetButtonMenus();

#ifndef USE_UI
public:
    QAction *action_File_New;
    QAction *action_File_New_from_Template;
    QAction *action_File_Open;
    QAction *action_File_Save;
    QAction *action_File_Save_as;
    QAction *action_File_Save_Options;
    QAction *action_File_Print;
    QAction *action_File_Commands;
    QAction *action_File_Exit;
    QAction *action_File_Print_Preview_Window;
    QAction *action_File_Print_Preview_Selection;
    QAction *action_File_Export_DXF;
    QAction *action_File_Export_DWG;
    QAction *action_File_Export_IGES;
    QAction *action_File_Export_Stroked_IGES;
    QAction *action_File_Export_HPGL;
    QAction *action_File_Import_DXF;
    QAction *action_File_Import_DWG;
    QAction *action_File_Import_IGES;
    QAction *action_File_Import_HPGL;
    QAction *action_File_Import_Medusa;
    QAction *action_File_Import_SKP;
    QAction *action_File_Import_3DWarehouse;
    QAction *action_Edit_Info;
    QAction *action_Edit_Undo;
    QAction *action_Edit_Redo;
    QAction *action_Edit_Cut;
    QAction *action_Edit_Copy;
    QAction *action_Edit_Copy_2D;
    QAction *action_Edit_Copy_2D_Window;
    QAction *action_Edit_Copy_Bitmap;
    QAction *action_Edit_Paste;
    QAction *action_Edit_Delete;
    QAction *action_Edit_New_Attribute;
    QAction *action_Edit_Delete_Attribute;
    QAction *action_Edit_Attribute;
    QAction *action_Edit_List_Attribute;
    QAction *action_Edit_Find_and_replace;
    QAction *action_Edit_Find_in_files;
    QAction *action_Transform_Stretch;
    QAction *action_Transform_Translate_Copy;
    QAction *action_Transform_Move;
    QAction *action_Transform_Copy_Array;
    QAction *action_Transform_Copy_Along;
    QAction *action_Transform_Project;
    QAction *action_Transform_Scale;
    QAction *action_Transform_Mirror;
    QAction *action_Transform_Rotate;
    QAction *action_Options_Cursor_on_work_plane;
    QAction *action_Options_Cursor_on_view;
    QAction *action_Options_Cursor_on_Depth;
    QAction *action_Options_Select_workplane;
    QAction *action_Options_Create_workplane;
    QAction *action_Options_Delete_workplane;
    QAction *action_Options_Change_Workplane_Origin;
    QAction *action_Options_Change_Workplane_Three_points;
    QAction *action_Options_Change_Workplane_Reset_to_standard;
    QAction *action_Options_Change_Workplane_Onto_Entity;
    //QAction *action_Options_Line;
    QAction *action_Options_Plane;
    QAction *action_Options_Ruled_surface;
    QAction *action_Options_Surface_of_revolution;
    QAction *action_Options_Patch;
    QAction *action_Options_Text;
    QAction *action_Options_Figure;
    QAction *action_Options_Bitmap;
    QAction *action_Options_Group;
    QAction *action_Options_Dimension;
    QAction *action_Options_Leader;
    // KMJ temporary option item for lights
    QAction *action_Options_Lights;
    //
    QAction *action_Options_Grid;
#ifdef _MAC
    QAction *action_Options_Preferences;
#endif
    QAction *action_Options_System;
    //QAction *action_Options_Applications_Animation;
    //QAction *action_Options_Applications_Architect;
    //QAction *action_Options_Applications_Machining;
    QAction *action_Arrange_To_front;
    QAction *action_Arrange_To_back;
    QAction *action_Arrange_Forward_one;
    QAction *action_Arrange_Back_one;
    QAction *action_Tools_Customize_Display_custom_menu_horizontal;
    QAction *action_Tools_Customize_Display_custom_menu_vertical;
    QAction *action_Tools_Customize_Add_to_custom_menu;
    QAction *action_Tools_Customize_Delete_from_custom_menu;
    QAction *action_Tools_Customize_Keyboard_Shortcuts;
    QAction *action_Tools_Run_program;
    QAction *action_Tools_Open_program;
    QAction *action_Tools_New_program;
    QAction *action_Tools_Load_application;
    QAction *action_Tools_Unload_application;
    QAction *action_Tools_Display_command_window;
    //QAction *action_Window_Display_3D_view;
    //QAction *action_Window_Display_composed_drawing;
    //QAction *action_Window_Display_layer_window;
    //QAction *action_Window_Cascade;
    QAction *action_Window_Tile;
    QAction *action_Window_Tile_Horizontal;
    QAction *action_Window_Tile_Vertical;
    //QAction *action_Window_Regenerate_window;
    QAction *action_Window_Regenerate_all;
    //QAction *action_Window_3Dview_Create;
    //QAction *action_Window_3Dview_Delete;
    //QAction *action_Window_3Dview_Change_All;
    //QAction *action_Window_3Dview_Change_Eye_point;
    //QAction *action_Window_3Dview_Change_Reference_point;
    //QAction *action_Window_3Dview_Change_Modify_incremental;
    //QAction *action_Window_3Dview_Change_Architectural_perspective;
    //QAction *action_Window_3Dview_Change_Focal_distance;
    //QAction *action_Window_3Dview_Change_Clip_planes;
    //QAction *action_Window_3Dview_ChangeTo_perspective;
    //QAction *action_Window_3Dview_ChangeTo_parallel;
    //QAction *action_Window_3Dview_Change_Rotate;
    //QAction *action_Window_3Dview_Change_To_workplane;
    //QAction *action_Window_3Dview_Change_Dynamic_rotate;
    //QAction *action_Window_3Dview_Change_Dynamic_rotate_about_axis;
    //QAction *action_Window_3Dview_Change_Dynamic_360_rotate;
    //QAction *action_Window_3Dview_Change_Dynamic_walk;
    //QAction *action_Window_3Dview_Change_Dynamic_fly;
    //QAction *action_Window_3Dview_Change_Plot_scale;
    //QAction *action_Window_3Dview_Change_Reset_to_standard;
    //QAction *action_Window_Zone_Create;
    //QAction *action_Window_Zone_Delete;
    //QAction *action_Window_Compose_Drawing_Create;
    //QAction *action_Window_Compose_Drawing_Delete;
    //QAction *action_Window_Compose_Drawing_Rename;
    //QAction *action_Window_Compose_Drawing_Add_3D_view;
    //QAction *action_Window_Compose_Drawing_Change_3D_view;
    //QAction *action_Window_Compose_Drawing_Delete_3D_view;
    //QAction *action_Window_Compose_Drawing_Scroll_3D_view;
    //QAction *action_Window_Compose_Drawing_Fit_2D_view;
    //QAction *action_Window_Compose_Drawing_Align_view;
    //QAction *action_Window_Compose_Drawing_Show_menu;
    //QAction *action_Window_Compose_Drawing_Copy;
    //QAction *action_Window_Shade;
    //QAction *action_Window_Hide;
    //QAction *action_Window_Hide_and_dash_hidden_lines;
    //QAction *action_Window_Raytrace_Insert_light;
    //QAction *action_Window_Raytrace_Insert_light_at_Sun_position;
    //QAction *action_Window_Raytrace_Lights;
    //QAction *action_Window_Raytrace_Surface_properties;
    //QAction *action_Window_Raytrace_Raytrace;
    //QAction *action_Window_Raytrace_Show_Menu;
    QAction *action_Window_Tool_bars;
    //QAction *action_Window_Show_properties;

    //QAction *action_Window_Switch_screens;
#ifndef _MAC
    QAction *action_Window_Show_FullScreen;
#else
//#ifdef _RCDEBUG
    QAction *action_Window_Show_FullScreen;
//#endif
#endif
    QAction *action_Window_Show_Maximised;
    QAction *action_Window_Show_Normal;

    // old help items
    /*
    QAction *action_Help_Contents_and_Index;
    QAction *action_Help_Current_command;
    QAction *action_Help_View_RealCAD_Reference_command;
    QAction *action_Help_View_RealCAD_GettingStarted_command;
    QAction *action_Help_View_LANDWorksCAD_Reference_command;
    QAction *action_Help_View_LANDWorksCAD_GettingStarted_command;
    QAction *action_Help_View_Training_Videos_command;
    QAction *action_Help_Visit_RealCAD_Website_command;
    QAction *action_Help_Visit_LANDWorksCAD_Website_command;
    QAction *action_Help_ProgramUpdates_command;
    QAction *action_Help_TechnicalSupport_command;
    QAction *action_Help_RemoteAssistance_command;
    */
    // new help items
    QAction *action_Help_Manual;
    QAction *action_Help_CheatSheet;
    QAction *action_Help_GetHelp;
    QAction *action_Help_WelcomeScreen;
    QAction *action_Help_ShareMyScreen;
    QAction *action_Help_OnlineTraining;
    QAction *action_Help_CheckForUpdates;
    QAction *action_Help_DeactivateLicense;
    //QAction *action_Help_TellaFriend;

    QAction *action_Help_About;
    QAction *separatorAct;

    // Popup menu actions
    QAction *action_Popup_Menu_LocateReferencePoint;
    QAction *action_Popup_Menu_LocateNear;
    QAction *action_Popup_Menu_LocateAlong;
    QAction *action_Popup_Menu_LocateOrigin;
    QAction *action_Popup_Menu_LocateEndPoint;
    QAction *action_Popup_Menu_LocateIntersection;
    QAction *action_Popup_Menu_LocateOffsetPoint;
    QAction *action_Popup_Menu_LocateApparentIntersection;
    QAction *action_Popup_Menu_ZoomWindow;
    QAction *action_Popup_Menu_ZoomScroll;
    QAction *action_Popup_Menu_ZoomIn;
    QAction *action_Popup_Menu_ZoomOut;
    QAction *action_Popup_Menu_ZoomFit;
    QAction *action_Popup_Menu_ZoomPrevious;
    QAction *action_Popup_Menu_SelectChain;
    QAction *action_Popup_Menu_SelectPolyWindow;
    //QAction *action_Popup_Menu_ViewByRotation;
    QAction *action_Popup_Menu_SnapDragSettings;

    // PopUp1_Menu actions
    QAction *action_Popup1_Menu_TrimLength;
    QAction *action_Popup1_Menu_Trim;
    QAction *action_Popup1_Menu_BlankOption;
    QAction *action_Popup1_Menu_Properties;
    QAction *action_Popup1_Menu_Material;
    //QAction *action_Popup1_Menu_DimensionAddPoint;
    //QAction *action_Popup1_Menu_DimensionMovePoint;
    //QAction *action_Popup1_Menu_DimensionDeletePoint;
    //QAction *action_Popup1_Menu_DimensionMoveText;
    QAction *action_Popup1_Menu_EditText;
    QAction *action_Popup1_Menu_Corner; // v8
    QAction *action_Popup1_Menu_Explode; // v8
    QAction *action_Popup1_Menu_Divide; // v8
    QAction *action_Popup1_Menu_Select; // v8
    QAction *action_Popup1_Menu_Hide; // v8
    QAction *action_Popup1_UnhideAll;
    QAction *action_Popup1_Menu_Clone; // v8

    // divide submenu actions
    QAction *action_Divide_Entity; // v8
    QAction *action_Divide_Length; // v8
    QAction *action_Divide_Number; // v8
    QAction *action_Delete_Segment; // v8

    // clone submenu actions
    QAction *action_Clone_Colour; // v8
    QAction *action_Clone_Layer; // v8
    QAction *action_Clone_Style; // v8
    QAction *action_Clone_Weight; // v8
    QAction *action_Clone_All; // v8

    // Select submenu actions
    QAction *action_Select_SameColour;
    QAction *action_Select_SameLayer;
    QAction *action_Select_SameStyle;
    QAction *action_Select_SameWeight;
    QAction *action_Select_SameEntity;
    QAction *action_Select_SameName;

    // Hide submenu actions
    QAction *action_Hide_SameColour;
    QAction *action_Hide_SameLayer;
    QAction *action_Hide_SameStyle;
    QAction *action_Hide_SameWeight;
    QAction *action_Hide_SameEntity;
    QAction *action_Hide_SameName;

    // list for accelerator shortcuts
    QList<QShortcut*> mWindowShortcuts;
    //QShortcut *SelectAllShortCut;
    //QShortcut *GridToggleShortCut;
    //QShortcut *GridDisplayShortCut;
    //QShortcut *SwitchWindowShortCut;

    //QShortcut *TransformMoveShortCut;
    //QShortcut *TransformCopyShortCut;
    //QShortcut *TransformScaleShortCut;
    //QShortcut *TransformRotateShortCut;
    //QShortcut *TransformProjectShortCut;
    //QShortcut *TransformOffsetShortCut;

    //QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menu_Print_preview;
    QMenu *menu_Export;
    QMenu *menu_Import;
    QMenu *menu_Edit;
    QMenu *menu_Transform;
    QMenu *menu_Options;
    QMenu *menu_Change_Workplane;
    //QMenu *menu_Applications;
    QMenu *menu_Arrange;
    QMenu *menu_Tools;
    QMenu *menu_Customize;
    QMenu *menu_Window;
    //QMenu *menu3D_View;
    //QMenu *menu3D_view_c_hange;
    //QMenu *menu_Zone_2;
    //QMenu *menu_Compose_drawing;
    //QMenu *menu_Raytrace;
    QMenu *menu_Help;

    // context menus
    QMenu *Popup_Menu;
    QMenu *Popup1_Menu;

    // new Popup1_Menu sub menus
    QMenu *menu_Select;
    QMenu *menu_Hide;
    QMenu *menu_Divide; // v8;
    QMenu *menu_Clone; // v8;

    enum { MaxRecentFiles = MAX_OPENLIST };
    QAction *recentFileActs[MaxRecentFiles];

    void setupUi(QtMainWindow *_QtMainWindow)
    {
        if (_QtMainWindow->objectName().isEmpty())
            _QtMainWindow->setObjectName(QStringLiteral("QtMainWindow"));
        _QtMainWindow->resize(800, 600);
        _QtMainWindow->setMinimumSize(QSize(100, 100));
        //_QtMainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        _QtMainWindow->setDockNestingEnabled(true);
        _QtMainWindow->setDockOptions(dockOptions()|QMainWindow::AllowNestedDocks|QMainWindow::AnimatedDocks);
        QSizeGrip sizeGrip(this);
        action_File_New = new QAction(_QtMainWindow);
        action_File_New->setObjectName(QStringLiteral("action_File_New"));
        action_File_New_from_Template = new QAction(_QtMainWindow);
        action_File_New_from_Template->setObjectName(QStringLiteral("action_File_New_from_Template"));
        action_File_Open = new QAction(_QtMainWindow);
        action_File_Open->setObjectName(QStringLiteral("action_File_Open"));
        action_File_Save = new QAction(_QtMainWindow);
        action_File_Save->setObjectName(QStringLiteral("action_File_Save"));
        action_File_Save_as = new QAction(_QtMainWindow);
        action_File_Save_as->setObjectName(QStringLiteral("action_File_Save_as"));
        action_File_Save_Options = new QAction(_QtMainWindow);
        action_File_Save_Options->setObjectName(QStringLiteral("action_File_Save_Options"));
        action_File_Print = new QAction(_QtMainWindow);
        action_File_Print->setObjectName(QStringLiteral("action_File_Print"));
        action_File_Commands = new QAction(_QtMainWindow);
        action_File_Commands->setEnabled(false);
        action_File_Commands->setObjectName(QStringLiteral("action_File_Commands"));
        action_File_Exit = new QAction(_QtMainWindow);
        action_File_Exit->setObjectName(QStringLiteral("action_File_Exit"));
        action_File_Print_Preview_Window = new QAction(_QtMainWindow);
        action_File_Print_Preview_Window->setObjectName(QStringLiteral("action_File_Print_Preview_Window"));
        action_File_Print_Preview_Selection = new QAction(_QtMainWindow);
        action_File_Print_Preview_Selection->setObjectName(QStringLiteral("action_File_Print_Preview_Selection"));
        action_File_Export_DXF = new QAction(_QtMainWindow);
        action_File_Export_DXF->setObjectName(QStringLiteral("action_File_Export_DXF"));
        action_File_Export_DWG = new QAction(_QtMainWindow);
        action_File_Export_DWG->setObjectName(QStringLiteral("action_File_Export_DWG"));
        action_File_Export_IGES = new QAction(_QtMainWindow);
        action_File_Export_IGES->setObjectName(QStringLiteral("action_File_Export_IGES"));
        action_File_Export_Stroked_IGES = new QAction(_QtMainWindow);
        action_File_Export_Stroked_IGES->setObjectName(QStringLiteral("action_File_Export_Stroked_IGES"));
        action_File_Export_HPGL = new QAction(_QtMainWindow);
        action_File_Export_HPGL->setObjectName(QStringLiteral("action_File_Export_HPGL"));
        action_File_Import_DXF = new QAction(_QtMainWindow);
        action_File_Import_DXF->setObjectName(QStringLiteral("action_File_Import_DXF"));
        action_File_Import_DWG = new QAction(_QtMainWindow);
        action_File_Import_DWG->setObjectName(QStringLiteral("action_File_Import_DWG"));
        action_File_Import_IGES = new QAction(_QtMainWindow);
        action_File_Import_IGES->setObjectName(QStringLiteral("action_File_Import_IGES"));
        action_File_Import_HPGL = new QAction(_QtMainWindow);
        action_File_Import_HPGL->setObjectName(QStringLiteral("action_File_Import_HPGL"));
        action_File_Import_Medusa = new QAction(_QtMainWindow);
        action_File_Import_Medusa->setObjectName(QStringLiteral("action_File_Import_Medusa"));

        action_File_Import_SKP = new QAction(_QtMainWindow);
        action_File_Import_SKP->setObjectName(QStringLiteral("action_File_Import_SKP"));
        action_File_Import_3DWarehouse = new QAction(_QtMainWindow);
        action_File_Import_3DWarehouse->setObjectName(QStringLiteral("action_File_Import_3DWarehouse"));

        action_Edit_Undo = new QAction(_QtMainWindow);
        action_Edit_Undo->setObjectName(QStringLiteral("action_Edit_Undo"));
        action_Edit_Redo = new QAction(_QtMainWindow);
        action_Edit_Redo->setObjectName(QStringLiteral("action_Edit_Redo"));
        action_Edit_Cut = new QAction(_QtMainWindow);
        action_Edit_Cut->setData(202);
        action_Edit_Cut->setObjectName(QStringLiteral("action_Edit_Cut"));
        action_Edit_Copy = new QAction(_QtMainWindow);
        action_Edit_Copy->setData(203);
        action_Edit_Copy->setObjectName(QStringLiteral("action_Edit_Copy"));
        action_Edit_Copy_2D = new QAction(_QtMainWindow);
        action_Edit_Copy_2D->setData(210);
        action_Edit_Copy_2D->setObjectName(QStringLiteral("action_Edit_Copy_2D"));
        action_Edit_Copy_2D_Window = new QAction(_QtMainWindow);
        action_Edit_Copy_2D_Window->setObjectName(QStringLiteral("action_Edit_Copy_2D_Window"));
        action_Edit_Copy_Bitmap = new QAction(_QtMainWindow);
        action_Edit_Copy_Bitmap->setObjectName(QStringLiteral("action_Edit_Copy_Bitmap"));
        action_Edit_Paste = new QAction(_QtMainWindow);
        action_Edit_Paste->setData(204);
        action_Edit_Paste->setObjectName(QStringLiteral("action_Edit_Paste"));
        action_Edit_Delete = new QAction(_QtMainWindow);
        action_Edit_Delete->setData(205);
        action_Edit_Delete->setObjectName(QStringLiteral("action_Edit_Delete"));
        action_Edit_New_Attribute = new QAction(_QtMainWindow);
        action_Edit_New_Attribute->setData(206);
        action_Edit_New_Attribute->setObjectName(QStringLiteral("action_Edit_New_Attribute"));
        action_Edit_Delete_Attribute = new QAction(_QtMainWindow);
        action_Edit_Delete_Attribute->setData(207);
        action_Edit_Delete_Attribute->setObjectName(QStringLiteral("action_Edit_Delete_Attribute"));
        action_Edit_Attribute = new QAction(_QtMainWindow);
        action_Edit_Attribute->setData(208);
        action_Edit_Attribute->setObjectName(QStringLiteral("action_Edit_Attribute"));
        action_Edit_List_Attribute = new QAction(_QtMainWindow);
        action_Edit_List_Attribute->setData(209);
        action_Edit_List_Attribute->setObjectName(QStringLiteral("action_Edit_List_Attribute"));
        action_Edit_Find_and_replace = new QAction(_QtMainWindow);
        action_Edit_Find_and_replace->setObjectName(QStringLiteral("action_Edit_Find_and_replace"));
        action_Edit_Find_in_files = new QAction(_QtMainWindow);
        action_Edit_Find_in_files->setObjectName(QStringLiteral("action_Edit_Find_in_files"));
        action_Edit_Info = new QAction(_QtMainWindow);
        action_Edit_Info->setObjectName(QStringLiteral("action_Edit_Info"));
        action_Transform_Stretch = new QAction(_QtMainWindow);
        action_Transform_Stretch->setObjectName(QStringLiteral("action_Transform_Stretch"));
        action_Transform_Translate_Copy = new QAction(_QtMainWindow);
        action_Transform_Translate_Copy->setObjectName(QStringLiteral("action_Transform_Translate_Copy"));
        action_Transform_Move = new QAction(_QtMainWindow);
        action_Transform_Move->setObjectName(QStringLiteral("action_Transform_Move"));
        action_Transform_Copy_Array = new QAction(_QtMainWindow);
        action_Transform_Copy_Array->setObjectName(QStringLiteral("action_Transform_Copy_Array"));
        action_Transform_Copy_Along = new QAction(_QtMainWindow);
        action_Transform_Copy_Along->setObjectName(QStringLiteral("action_Transform_Copy_Along"));
        action_Transform_Project = new QAction(_QtMainWindow);
        action_Transform_Project->setObjectName(QStringLiteral("action_Transform_Project"));
        action_Transform_Scale = new QAction(_QtMainWindow);
        action_Transform_Scale->setObjectName(QStringLiteral("action_Transform_Scale"));
        action_Transform_Mirror = new QAction(_QtMainWindow);
        action_Transform_Mirror->setObjectName(QStringLiteral("action_Transform_Mirror"));
        action_Transform_Rotate = new QAction(_QtMainWindow);
        action_Transform_Rotate->setObjectName(QStringLiteral("action_Transform_Rotate"));
        action_Options_Cursor_on_work_plane = new QAction(_QtMainWindow);
        action_Options_Cursor_on_work_plane->setObjectName(QStringLiteral("action_Options_Cursor_on_work_plane"));
        action_Options_Cursor_on_work_plane->setCheckable(true);
        action_Options_Cursor_on_view = new QAction(_QtMainWindow);
        action_Options_Cursor_on_view->setObjectName(QStringLiteral("action_Options_Cursor_on_view"));
        action_Options_Cursor_on_view->setCheckable(true);
        action_Options_Cursor_on_Depth = new QAction(_QtMainWindow);
        action_Options_Cursor_on_Depth->setObjectName(QStringLiteral("action_Options_Cursor_on_Depth"));
        //action_Options_Cursor_on_Depth->setCheckable(true);
        action_Options_Select_workplane = new QAction(_QtMainWindow);
        action_Options_Select_workplane->setObjectName(QStringLiteral("action_Options_Select_workplane"));
        action_Options_Create_workplane = new QAction(_QtMainWindow);
        action_Options_Create_workplane->setObjectName(QStringLiteral("action_Options_Create_workplane"));
        action_Options_Delete_workplane = new QAction(_QtMainWindow);
        action_Options_Delete_workplane->setObjectName(QStringLiteral("action_Options_Delete_workplane"));
        action_Options_Change_Workplane_Origin = new QAction(_QtMainWindow);
        action_Options_Change_Workplane_Origin->setObjectName(QStringLiteral("action_Options_Change_Workplane_Origin"));
        action_Options_Change_Workplane_Three_points = new QAction(_QtMainWindow);
        action_Options_Change_Workplane_Three_points->setObjectName(QStringLiteral("action_Options_Change_Workplane_Three_points"));
        action_Options_Change_Workplane_Reset_to_standard = new QAction(_QtMainWindow);
        action_Options_Change_Workplane_Reset_to_standard->setObjectName(QStringLiteral("action_Options_Change_Workplane_Reset_to_standard"));
        action_Options_Change_Workplane_Onto_Entity = new QAction(_QtMainWindow);
        action_Options_Change_Workplane_Onto_Entity->setObjectName(QStringLiteral("action_Options_Change_Workplane_Onto_Entity"));
        //action_Options_Line = new QAction(_QtMainWindow);
        //action_Options_Line->setObjectName(QStringLiteral("action_Options_Line"));
        action_Options_Plane = new QAction(_QtMainWindow);
        action_Options_Plane->setObjectName(QStringLiteral("action_Options_Plane"));
        action_Options_Ruled_surface = new QAction(_QtMainWindow);
        action_Options_Ruled_surface->setObjectName(QStringLiteral("action_Options_Ruled_surface"));
        action_Options_Surface_of_revolution = new QAction(_QtMainWindow);
        action_Options_Surface_of_revolution->setObjectName(QStringLiteral("action_Options_Surface_of_revolution"));
        action_Options_Patch = new QAction(_QtMainWindow);
        action_Options_Patch->setObjectName(QStringLiteral("action_Options_Patch"));
        action_Options_Text = new QAction(_QtMainWindow);
        action_Options_Text->setObjectName(QStringLiteral("action_Options_Text"));
        action_Options_Figure = new QAction(_QtMainWindow);
        action_Options_Figure->setObjectName(QStringLiteral("action_Options_Figure"));
        action_Options_Bitmap = new QAction(_QtMainWindow);
        action_Options_Bitmap->setObjectName(QStringLiteral("action_Options_Bitmap"));
        action_Options_Group = new QAction(_QtMainWindow);
        action_Options_Group->setObjectName(QStringLiteral("action_Options_Group"));
        action_Options_Dimension = new QAction(_QtMainWindow);
        action_Options_Dimension->setObjectName(QStringLiteral("action_Options_Dimension"));
        action_Options_Leader = new QAction(_QtMainWindow);
        action_Options_Leader->setObjectName(QStringLiteral("action_Options_Leader"));
        // KMJ temporary option item for lights
        action_Options_Lights = new QAction(_QtMainWindow);
        action_Options_Lights->setObjectName(QStringLiteral("action_Options_Lights"));
        //
        action_Options_Grid = new QAction(_QtMainWindow);
        action_Options_Grid->setObjectName(QStringLiteral("action_Options_Grid"));
#ifdef _MAC
        action_Options_Preferences = new QAction(_QtMainWindow);
        action_Options_Preferences->setObjectName(QStringLiteral("action_Options_Preferences"));
        action_Options_Preferences->setMenuRole(QAction::PreferencesRole);
#endif
        action_Options_System = new QAction(_QtMainWindow);
        action_Options_System->setObjectName(QStringLiteral("action_Options_System"));
        //action_Options_Applications_Animation = new QAction(_QtMainWindow);
        //action_Options_Applications_Animation->setObjectName(QStringLiteral("action_Options_Applications_Animation"));
        //action_Options_Applications_Architect = new QAction(_QtMainWindow);
        //action_Options_Applications_Architect->setObjectName(QStringLiteral("action_Options_Applications_Architect"));
        //action_Options_Applications_Machining = new QAction(_QtMainWindow);
        //action_Options_Applications_Machining->setObjectName(QStringLiteral("action_Options_Applications_Machining"));
        action_Arrange_To_front = new QAction(_QtMainWindow);
        action_Arrange_To_front->setObjectName(QStringLiteral("action_Arrange_To_front"));
        action_Arrange_To_back = new QAction(_QtMainWindow);
        action_Arrange_To_back->setObjectName(QStringLiteral("action_Arrange_To_back"));
        action_Arrange_Forward_one = new QAction(_QtMainWindow);
        action_Arrange_Forward_one->setObjectName(QStringLiteral("action_Arrange_Forward_one"));
        action_Arrange_Back_one = new QAction(_QtMainWindow);
        action_Arrange_Back_one->setObjectName(QStringLiteral("action_Arrange_Back_one"));
        action_Tools_Customize_Display_custom_menu_horizontal = new QAction(_QtMainWindow);
        action_Tools_Customize_Display_custom_menu_horizontal->setObjectName(QStringLiteral("action_Tools_Customize_Display_custom_menu_horizontal"));
        action_Tools_Customize_Display_custom_menu_vertical = new QAction(_QtMainWindow);
        action_Tools_Customize_Display_custom_menu_vertical->setObjectName(QStringLiteral("action_Tools_Customize_Display_custom_menu_vertical"));
        action_Tools_Customize_Add_to_custom_menu = new QAction(_QtMainWindow);
        action_Tools_Customize_Add_to_custom_menu->setObjectName(QStringLiteral("action_Tools_Customize_Add_to_custom_menu"));
        action_Tools_Customize_Delete_from_custom_menu = new QAction(_QtMainWindow);
        action_Tools_Customize_Delete_from_custom_menu->setObjectName(QStringLiteral("action_Tools_Customize_Delete_from_custom_menu"));
        action_Tools_Customize_Keyboard_Shortcuts = new QAction(_QtMainWindow);
        action_Tools_Customize_Keyboard_Shortcuts->setObjectName(QStringLiteral("action_Tools_Customize_Keyboard_Shortcuts"));
        action_Tools_Run_program = new QAction(_QtMainWindow);
        action_Tools_Run_program->setObjectName(QStringLiteral("action_Tools_Run_program"));
        action_Tools_Open_program = new QAction(_QtMainWindow);
        action_Tools_Open_program->setObjectName(QStringLiteral("action_Tools_Open_program"));
        action_Tools_New_program = new QAction(_QtMainWindow);
        action_Tools_New_program->setObjectName(QStringLiteral("action_Tools_New_program"));
        action_Tools_Load_application = new QAction(_QtMainWindow);
        action_Tools_Load_application->setObjectName(QStringLiteral("action_Tools_Load_application"));
        action_Tools_Unload_application = new QAction(_QtMainWindow);
        action_Tools_Unload_application->setObjectName(QStringLiteral("action_Tools_Unload_application"));
        action_Tools_Display_command_window = new QAction(_QtMainWindow);
        action_Tools_Display_command_window->setObjectName(QStringLiteral("action_Tools_Display_command_window"));
        //action_Window_Display_3D_view = new QAction(_QtMainWindow);
        //action_Window_Display_3D_view->setObjectName(QStringLiteral("action_Window_Display_3D_view"));
        //action_Window_Display_composed_drawing = new QAction(_QtMainWindow);
        //action_Window_Display_composed_drawing->setObjectName(QStringLiteral("action_Window_Display_composed_drawing"));
        //action_Window_Display_layer_window = new QAction(_QtMainWindow);
        //action_Window_Display_layer_window->setObjectName(QStringLiteral("action_Window_Display_layer_window"));
        //action_Window_Cascade = new QAction(_QtMainWindow);
        //action_Window_Cascade->setObjectName(QStringLiteral("action_Window_Cascade"));
        action_Window_Tile = new QAction(_QtMainWindow);
        action_Window_Tile->setObjectName(QStringLiteral("action_Window_Tile"));
        action_Window_Tile_Horizontal = new QAction(_QtMainWindow);
        action_Window_Tile_Horizontal->setObjectName(QStringLiteral("action_Window_Tile_Horizontal"));
        action_Window_Tile_Vertical = new QAction(_QtMainWindow);
        action_Window_Tile_Vertical->setObjectName(QStringLiteral("action_Window_Tile_Vertical"));
        //action_Window_Regenerate_window = new QAction(_QtMainWindow);
        //action_Window_Regenerate_window->setObjectName(QStringLiteral("action_Window_Regenerate_window"));
        action_Window_Regenerate_all = new QAction(_QtMainWindow);
        action_Window_Regenerate_all->setObjectName(QStringLiteral("action_Window_Regenerate_all"));
        //action_Window_3Dview_Create = new QAction(_QtMainWindow);
        //action_Window_3Dview_Create->setObjectName(QStringLiteral("action_Window_3Dview_Create"));
        //action_Window_3Dview_Delete = new QAction(_QtMainWindow);
        //action_Window_3Dview_Delete->setObjectName(QStringLiteral("action_Window_3Dview_Delete"));
        //action_Window_3Dview_Change_All = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_All->setObjectName(QStringLiteral("action_Window_3Dview_Change_All"));
        //action_Window_3Dview_Change_Eye_point = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Eye_point->setObjectName(QStringLiteral("action_Window_3Dview_Change_Eye_point"));
        //action_Window_3Dview_Change_Reference_point = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Reference_point->setObjectName(QStringLiteral("action_Window_3Dview_Change_Reference_point"));
        //action_Window_3Dview_Change_Modify_incremental = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Modify_incremental->setObjectName(QStringLiteral("action_Window_3Dview_Change_Modify_incremental"));
        //action_Window_3Dview_Change_Architectural_perspective = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Architectural_perspective->setObjectName(QStringLiteral("action_Window_3Dview_Change_Architectural_perspective"));
        //action_Window_3Dview_Change_Focal_distance = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Focal_distance->setObjectName(QStringLiteral("action_Window_3Dview_Change_Focal_distance"));
        //action_Window_3Dview_Change_Clip_planes = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Clip_planes->setObjectName(QStringLiteral("action_Window_3Dview_Change_Clip_planes"));
        //action_Window_3Dview_ChangeTo_perspective = new QAction(_QtMainWindow);
        //action_Window_3Dview_ChangeTo_perspective->setObjectName(QStringLiteral("action_Window_3Dview_ChangeTo_perspective"));
        //action_Window_3Dview_ChangeTo_parallel = new QAction(_QtMainWindow);
        //action_Window_3Dview_ChangeTo_parallel->setObjectName(QStringLiteral("action_Window_3Dview_ChangeTo_parallel"));
        //action_Window_3Dview_Change_Rotate = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Rotate->setObjectName(QStringLiteral("action_Window_3Dview_Change_Rotate"));
        //action_Window_3Dview_Change_To_workplane = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_To_workplane->setObjectName(QStringLiteral("action_Window_3Dview_Change_To_workplane"));
        //action_Window_3Dview_Change_Dynamic_rotate = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Dynamic_rotate->setObjectName(QStringLiteral("action_Window_3Dview_Change_Dynamic_rotate"));
        //action_Window_3Dview_Change_Dynamic_rotate_about_axis = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Dynamic_rotate_about_axis->setObjectName(QStringLiteral("action_Window_3Dview_Change_Dynamic_rotate_about_axis"));
        //action_Window_3Dview_Change_Dynamic_360_rotate = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Dynamic_360_rotate->setObjectName(QStringLiteral("action_Window_3Dview_Change_Dynamic_360_rotate"));
        //action_Window_3Dview_Change_Dynamic_walk = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Dynamic_walk->setObjectName(QStringLiteral("action_Window_3Dview_Change_Dynamic_walk"));
        //action_Window_3Dview_Change_Dynamic_fly = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Dynamic_fly->setObjectName(QStringLiteral("action_Window_3Dview_Change_Dynamic_fly"));
        //action_Window_3Dview_Change_Plot_scale = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Plot_scale->setObjectName(QStringLiteral("action_Window_3Dview_Change_Plot_scale"));
        //action_Window_3Dview_Change_Reset_to_standard = new QAction(_QtMainWindow);
        //action_Window_3Dview_Change_Reset_to_standard->setObjectName(QStringLiteral("action_Window_3Dview_Change_Reset_to_standard"));
        //action_Window_Zone_Create = new QAction(_QtMainWindow);
        //action_Window_Zone_Create->setObjectName(QStringLiteral("action_Window_Zone_Create"));
        //action_Window_Zone_Delete = new QAction(_QtMainWindow);
        //action_Window_Zone_Delete->setObjectName(QStringLiteral("action_Window_Zone_Delete"));
        //action_Window_Compose_Drawing_Create = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Create->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Create"));
        //action_Window_Compose_Drawing_Delete = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Delete->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Delete"));
        //action_Window_Compose_Drawing_Rename = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Rename->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Rename"));
        //action_Window_Compose_Drawing_Add_3D_view = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Add_3D_view->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Add_3D_view"));
        //action_Window_Compose_Drawing_Change_3D_view = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Change_3D_view->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Change_3D_view"));
        //action_Window_Compose_Drawing_Delete_3D_view = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Delete_3D_view->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Delete_3D_view"));
        //action_Window_Compose_Drawing_Scroll_3D_view = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Scroll_3D_view->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Scroll_3D_view"));
        //action_Window_Compose_Drawing_Fit_2D_view = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Fit_2D_view->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Fit_2D_view"));
        //action_Window_Compose_Drawing_Align_view = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Align_view->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Align_view"));
        //action_Window_Compose_Drawing_Show_menu = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Show_menu->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Show_menu"));
        //action_Window_Compose_Drawing_Copy = new QAction(_QtMainWindow);
        //action_Window_Compose_Drawing_Copy->setObjectName(QStringLiteral("action_Window_Compose_Drawing_Copy"));
        //action_Window_Shade = new QAction(_QtMainWindow);
        //action_Window_Shade->setObjectName(QStringLiteral("action_Window_Shade"));
        //action_Window_Hide = new QAction(_QtMainWindow);
        //action_Window_Hide->setObjectName(QStringLiteral("action_Window_Hide"));
        //action_Window_Hide_and_dash_hidden_lines = new QAction(_QtMainWindow);
        //action_Window_Hide_and_dash_hidden_lines->setObjectName(QStringLiteral("action_Window_Hide_and_dash_hidden_lines"));
        //action_Window_Raytrace_Insert_light = new QAction(_QtMainWindow);
        //action_Window_Raytrace_Insert_light->setObjectName(QStringLiteral("action_Window_Raytrace_Insert_light"));
        //action_Window_Raytrace_Insert_light_at_Sun_position = new QAction(_QtMainWindow);
        //action_Window_Raytrace_Insert_light_at_Sun_position->setObjectName(QStringLiteral("action_Window_Raytrace_Insert_light_at_Sun_position"));
        //action_Window_Raytrace_Lights = new QAction(_QtMainWindow);
        //action_Window_Raytrace_Lights->setObjectName(QStringLiteral("action_Window_Raytrace_Lights"));
        //action_Window_Raytrace_Surface_properties = new QAction(_QtMainWindow);
        //action_Window_Raytrace_Surface_properties->setObjectName(QStringLiteral("action_Window_Raytrace_Surface_properties"));
        //action_Window_Raytrace_Raytrace = new QAction(_QtMainWindow);
        //action_Window_Raytrace_Raytrace->setObjectName(QStringLiteral("action_Window_Raytrace_Raytrace"));
        //action_Window_Raytrace_Show_Menu = new QAction(_QtMainWindow);
        //action_Window_Raytrace_Show_Menu->setObjectName(QStringLiteral("action_Window_Raytrace_Show_Menu"));
        action_Window_Tool_bars = new QAction(_QtMainWindow);
        action_Window_Tool_bars->setObjectName(QStringLiteral("action_Window_Tool_bars"));
        //action_Window_Show_properties = new QAction(_QtMainWindow);
        //action_Window_Show_properties->setObjectName(QStringLiteral("action_Window_Show_properties"));

        //action_Window_Switch_screens = new QAction(_QtMainWindow);
        //action_Window_Switch_screens->setObjectName(QStringLiteral("action_Window_Switch_screens"));
#ifndef _MAC
        action_Window_Show_FullScreen = new QAction(_QtMainWindow);
        action_Window_Show_FullScreen->setObjectName(QStringLiteral("action_Window_Show_FullScreen"));
#else
//#ifdef _RCDEBUG
        action_Window_Show_FullScreen = new QAction(_QtMainWindow);
        action_Window_Show_FullScreen->setObjectName(QStringLiteral("action_Window_Show_FullScreen"));
//#endif
#endif
        action_Window_Show_Maximised = new QAction(_QtMainWindow);
        action_Window_Show_Maximised->setObjectName(QStringLiteral("action_Window_Show_Maximised"));
        action_Window_Show_Normal = new QAction(_QtMainWindow);
        action_Window_Show_Normal->setObjectName(QStringLiteral("action_Window_Show_Normal"));

        // old help items
        /*
        action_Help_Contents_and_Index = new QAction(_QtMainWindow);
        action_Help_Contents_and_Index->setObjectName(QStringLiteral("action_Help_Contents_and_Index"));
        action_Help_Current_command = new QAction(_QtMainWindow);
        action_Help_Current_command->setObjectName(QStringLiteral("action_Help_Current_command"));
        action_Help_View_RealCAD_Reference_command = new QAction(_QtMainWindow);
        action_Help_View_RealCAD_Reference_command->setObjectName(QStringLiteral("action_Help_View_RealCAD_Reference_command"));
        action_Help_View_RealCAD_GettingStarted_command = new QAction(_QtMainWindow);
        action_Help_View_RealCAD_GettingStarted_command->setObjectName(QStringLiteral("action_Help_View_RealCAD_GettingStarted_command"));
        action_Help_View_LANDWorksCAD_Reference_command = new QAction(_QtMainWindow);
        action_Help_View_LANDWorksCAD_Reference_command->setObjectName(QStringLiteral("action_Help_View_LANDWorksCAD_Reference_command"));
        action_Help_View_LANDWorksCAD_GettingStarted_command = new QAction(_QtMainWindow);
        action_Help_View_LANDWorksCAD_GettingStarted_command->setObjectName(QStringLiteral("action_Help_View_LANDWorksCAD_GettingStarted_command"));
        action_Help_View_Training_Videos_command = new QAction(_QtMainWindow);
        action_Help_View_Training_Videos_command->setObjectName(QStringLiteral("action_Help_View_Training_Videos_command"));
        action_Help_Visit_RealCAD_Website_command = new QAction(_QtMainWindow);
        action_Help_Visit_RealCAD_Website_command->setObjectName(QStringLiteral("action_Help_Visit_RealCAD_Website_command"));
        action_Help_Visit_LANDWorksCAD_Website_command = new QAction(_QtMainWindow);
        action_Help_Visit_LANDWorksCAD_Website_command->setObjectName(QStringLiteral("action_Help_Visit_LANDWorksCAD_Website_command"));
        action_Help_ProgramUpdates_command = new QAction(_QtMainWindow);
        action_Help_ProgramUpdates_command->setObjectName(QStringLiteral("action_Help_ProgramUpdates_command"));
        action_Help_TechnicalSupport_command = new QAction(_QtMainWindow);
        action_Help_TechnicalSupport_command->setObjectName(QStringLiteral("action_Help_TechnicalSupport_command"));
        action_Help_RemoteAssistance_command = new QAction(_QtMainWindow);
        action_Help_RemoteAssistance_command->setObjectName(QStringLiteral("action_Help_RemoteAssistance_command"));
        */
        // new help items
        action_Help_Manual = new QAction(_QtMainWindow);
        action_Help_Manual->setObjectName(QStringLiteral("action_Help_Manual"));

        action_Help_CheatSheet = new QAction(_QtMainWindow);
        action_Help_CheatSheet->setObjectName(QStringLiteral("action_Help_CheatSheet"));

        action_Help_GetHelp = new QAction(_QtMainWindow);
        action_Help_GetHelp->setObjectName(QStringLiteral("action_Help_GetHelp"));

        action_Help_WelcomeScreen = new QAction(_QtMainWindow);
        action_Help_WelcomeScreen->setObjectName(QStringLiteral("action_Help_WelcomeScreen"));

        action_Help_ShareMyScreen = new QAction(_QtMainWindow);
        action_Help_ShareMyScreen->setObjectName(QStringLiteral("action_Help_ShareMyScreen"));

        action_Help_OnlineTraining = new QAction(_QtMainWindow);
        action_Help_OnlineTraining->setObjectName(QStringLiteral("action_Help_OnlineTraining"));

        action_Help_CheckForUpdates = new QAction(_QtMainWindow);
        action_Help_CheckForUpdates->setObjectName(QStringLiteral("action_Help_CheckForUpdates"));

        action_Help_DeactivateLicense = new QAction(_QtMainWindow);
        action_Help_DeactivateLicense->setObjectName(QStringLiteral("action_Help_DeactivateLicense"));

        //action_Help_TellaFriend = new QAction(_QtMainWindow);
        //action_Help_TellaFriend->setObjectName(QStringLiteral("action_Help_TellaFriend"));

        action_Help_About = new QAction(_QtMainWindow);
        action_Help_About->setObjectName(QStringLiteral("action_Help_About"));
        //centralWidget = new QWidget(_QtMainWindow);
        //centralWidget->setObjectName(QStringLiteral("centralWidget"));
        //_QtMainWindow->setCentralWidget(centralWidget);

        // Popup_Menu
        Popup_Menu = new QMenu(this);
        Popup_Menu->setObjectName("Popup_Menu");
        action_Popup_Menu_LocateReferencePoint = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateReferencePoint->setObjectName(QStringLiteral("action_Popup_Menu_LocateReferencePoint"));
        action_Popup_Menu_LocateNear = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateNear->setObjectName(QStringLiteral("action_Popup_Menu_LocateNear"));
        action_Popup_Menu_LocateAlong = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateAlong->setObjectName(QStringLiteral("action_Popup_Menu_LocateAlong"));
        action_Popup_Menu_LocateOrigin = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateOrigin->setObjectName(QStringLiteral("action_Popup_Menu_LocateOrigin"));
        action_Popup_Menu_LocateEndPoint = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateEndPoint->setObjectName(QStringLiteral("action_Popup_Menu_LocateEndPoint"));
        action_Popup_Menu_LocateIntersection = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateIntersection->setObjectName(QStringLiteral("action_Popup_Menu_LocateIntersection"));
        action_Popup_Menu_LocateOffsetPoint = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateOffsetPoint->setObjectName(QStringLiteral("action_Popup_Menu_LocateOffsetPoint"));
        action_Popup_Menu_LocateApparentIntersection = new QAction(_QtMainWindow);
        action_Popup_Menu_LocateApparentIntersection->setObjectName(QStringLiteral("action_Popup_Menu_LocateApparentIntersection"));
        action_Popup_Menu_ZoomWindow = new QAction(_QtMainWindow);
        action_Popup_Menu_ZoomWindow->setObjectName(QStringLiteral("action_Popup_Menu_ZoomWindow"));
        action_Popup_Menu_ZoomScroll = new QAction(_QtMainWindow);
        action_Popup_Menu_ZoomScroll->setObjectName(QStringLiteral("action_Popup_Menu_ZoomScroll"));
        action_Popup_Menu_ZoomIn = new QAction(_QtMainWindow);
        action_Popup_Menu_ZoomIn->setObjectName(QStringLiteral("action_Popup_Menu_ZoomIn"));
        action_Popup_Menu_ZoomOut = new QAction(_QtMainWindow);
        action_Popup_Menu_ZoomOut->setObjectName(QStringLiteral("action_Popup_Menu_ZoomOut"));
        action_Popup_Menu_ZoomFit = new QAction(_QtMainWindow);
        action_Popup_Menu_ZoomFit->setObjectName(QStringLiteral("action_Popup_Menu_ZoomFit"));
        action_Popup_Menu_ZoomPrevious = new QAction(_QtMainWindow);
        action_Popup_Menu_ZoomPrevious->setObjectName(QStringLiteral("action_Popup_Menu_ZoomPrevious"));
        action_Popup_Menu_SelectChain = new QAction(_QtMainWindow);
        action_Popup_Menu_SelectChain->setObjectName(QStringLiteral("action_Popup_Menu_SelectChain"));
        action_Popup_Menu_SelectPolyWindow = new QAction(_QtMainWindow);
        action_Popup_Menu_SelectPolyWindow->setObjectName(QStringLiteral("action_Popup_Menu_SelectPolyWindow"));
        //action_Popup_Menu_ViewByRotation = new QAction(_QtMainWindow);
        //action_Popup_Menu_ViewByRotation->setObjectName(QStringLiteral("action_Popup_Menu_ViewByRotation"));
        action_Popup_Menu_SnapDragSettings = new QAction(_QtMainWindow);
        action_Popup_Menu_SnapDragSettings->setObjectName(QStringLiteral("action_Popup_Menu_SnapDragSettings"));

        // PopUp1_Menu
        // Select submenu actions
        action_Select_SameColour = new QAction(_QtMainWindow);
        action_Select_SameColour->setObjectName(QStringLiteral("action_Select_SameColour"));
        action_Select_SameLayer = new QAction(_QtMainWindow);
        action_Select_SameLayer->setObjectName(QStringLiteral("action_Select_SameLayer"));
        action_Select_SameStyle = new QAction(_QtMainWindow);
        action_Select_SameStyle->setObjectName(QStringLiteral("action_Select_SameStyle"));
        action_Select_SameWeight = new QAction(_QtMainWindow);
        action_Select_SameWeight->setObjectName(QStringLiteral("action_Select_SameWeight"));
        action_Select_SameEntity = new QAction(_QtMainWindow);
        action_Select_SameEntity->setObjectName(QStringLiteral("action_Select_SameEntity"));
        action_Select_SameName = new QAction(_QtMainWindow);
        action_Select_SameName->setObjectName(QStringLiteral("action_Select_SameName"));
        menu_Select = new QMenu(this);
        menu_Select->setObjectName(QStringLiteral("Select_Menu"));
        menu_Select->addAction(action_Select_SameColour);
        menu_Select->addAction(action_Select_SameLayer);
        menu_Select->addAction(action_Select_SameStyle);
        menu_Select->addAction(action_Select_SameWeight);
        menu_Select->addAction(action_Select_SameEntity);
        menu_Select->addAction(action_Select_SameName);

        // Hide submenu actions
        action_Hide_SameColour = new QAction(_QtMainWindow);
        action_Hide_SameColour->setObjectName(QStringLiteral("action_Hide_SameColour"));
        action_Hide_SameLayer = new QAction(_QtMainWindow);
        action_Hide_SameLayer->setObjectName(QStringLiteral("action_Hide_SameLayer"));
        action_Hide_SameStyle = new QAction(_QtMainWindow);
        action_Hide_SameStyle->setObjectName(QStringLiteral("action_Hide_SameStyle"));
        action_Hide_SameWeight = new QAction(_QtMainWindow);
        action_Hide_SameWeight->setObjectName(QStringLiteral("action_Hide_SameWeight"));
        action_Hide_SameEntity = new QAction(_QtMainWindow);
        action_Hide_SameEntity->setObjectName(QStringLiteral("action_Hide_SameEntity"));
#ifdef _RCDEBUG
        action_Hide_SameName = new QAction(_QtMainWindow);
        action_Hide_SameName->setObjectName(QStringLiteral("action_Hide_SameName"));
#else
        action_Hide_SameName=0;
#endif
        action_Popup1_UnhideAll = new QAction(_QtMainWindow);
        action_Popup1_UnhideAll->setObjectName(QStringLiteral("action_Popup1_UnhideAll"));
        menu_Hide = new QMenu(this);
        menu_Hide->setObjectName(QStringLiteral("Hide_Menu"));
        //menu_Hide->setObjectName(QStringLiteral("Select_Menu"));
        menu_Hide->addAction(action_Hide_SameColour);
        menu_Hide->addAction(action_Hide_SameLayer);
        menu_Hide->addAction(action_Hide_SameStyle);
        menu_Hide->addAction(action_Hide_SameWeight);
        menu_Hide->addAction(action_Hide_SameEntity);
#ifdef _RCDEBUG
        menu_Hide->addAction(action_Hide_SameName);
#endif

        // added for v8
        action_Divide_Entity = new QAction(_QtMainWindow);
        action_Divide_Entity->setObjectName(QStringLiteral("action_Divide_Entity"));
        action_Divide_Length = new QAction(_QtMainWindow);
        action_Divide_Length->setObjectName(QStringLiteral("action_Divide_Length"));
        action_Divide_Number = new QAction(_QtMainWindow);
        action_Divide_Number->setObjectName(QStringLiteral("action_Divide_Number"));
        action_Delete_Segment = new QAction(_QtMainWindow);
        action_Delete_Segment->setObjectName(QStringLiteral("action_Delete_Segment"));
        menu_Divide = new QMenu(this);
        menu_Divide->addAction(action_Divide_Entity);
        menu_Divide->addAction(action_Divide_Length);
        menu_Divide->addAction(action_Divide_Number);
        menu_Divide->addAction(action_Delete_Segment);

        action_Clone_Colour = new QAction(_QtMainWindow);
        action_Clone_Colour->setObjectName(QStringLiteral("action_Clone_Colour"));
        action_Clone_Layer = new QAction(_QtMainWindow);
        action_Clone_Layer->setObjectName(QStringLiteral("action_Clone_Layer"));
        action_Clone_Style = new QAction(_QtMainWindow);
        action_Clone_Style->setObjectName(QStringLiteral("action_Clone_Style"));
        action_Clone_Weight = new QAction(_QtMainWindow);
        action_Clone_Weight->setObjectName(QStringLiteral("action_Clone_Weight"));
        action_Clone_All = new QAction(_QtMainWindow);
        action_Clone_All->setObjectName(QStringLiteral("action_Clone_All"));
        menu_Clone = new QMenu(this);
        menu_Clone->addAction(action_Clone_Colour);
        menu_Clone->addAction(action_Clone_Layer);
        menu_Clone->addAction(action_Clone_Style);
        menu_Clone->addAction(action_Clone_Weight);
        menu_Clone->addAction(action_Clone_All);
        //

        Popup1_Menu = new QMenu(this);
        Popup1_Menu->setObjectName("Popup1_Menu");
        action_Popup1_Menu_BlankOption = new QAction(_QtMainWindow);
        action_Popup1_Menu_BlankOption->setObjectName(QStringLiteral("action_Popup1_Menu_BlankOption"));
        action_Popup1_Menu_Trim = new QAction(_QtMainWindow);
        action_Popup1_Menu_Trim->setObjectName(QStringLiteral("action_Popup1_Menu_Trim"));
        action_Popup1_Menu_TrimLength = new QAction(_QtMainWindow);
        action_Popup1_Menu_TrimLength->setObjectName(QStringLiteral("action_Popup1_Menu_TrimLength"));
        action_Popup1_Menu_Corner = new QAction(_QtMainWindow);
        action_Popup1_Menu_Corner->setObjectName(QStringLiteral("action_Popup1_Menu_Corner"));
        action_Popup1_Menu_Explode = new QAction(_QtMainWindow);
        action_Popup1_Menu_Explode->setObjectName(QStringLiteral("action_Popup1_Menu_Explode"));
        action_Popup1_Menu_Properties = new QAction(_QtMainWindow);
        action_Popup1_Menu_Properties->setObjectName(QStringLiteral("action_Popup1_Menu_Properties"));
        action_Popup1_Menu_Material = new QAction(_QtMainWindow);
        action_Popup1_Menu_Material->setObjectName(QStringLiteral("action_Popup1_Menu_Material"));
        //action_Popup1_Menu_DimensionAddPoint = new QAction(_QtMainWindow);
        //action_Popup1_Menu_DimensionAddPoint->setObjectName(QStringLiteral("action_Popup1_Menu_DimensionAddPoint"));
        //action_Popup1_Menu_DimensionMovePoint = new QAction(_QtMainWindow);
        //action_Popup1_Menu_DimensionMovePoint->setObjectName(QStringLiteral("action_Popup1_Menu_DimensionMovePoint"));
        //action_Popup1_Menu_DimensionDeletePoint = new QAction(_QtMainWindow);
        //action_Popup1_Menu_DimensionDeletePoint->setObjectName(QStringLiteral("action_Popup1_Menu_DimensionDeletePoint"));
        //action_Popup1_Menu_DimensionMoveText = new QAction(_QtMainWindow);
        //action_Popup1_Menu_DimensionMoveText->setObjectName(QStringLiteral("action_Popup1_Menu_DimensionMoveText"));
        action_Popup1_Menu_EditText = new QAction(_QtMainWindow);
        action_Popup1_Menu_EditText->setObjectName(QStringLiteral("action_Popup1_Menu_EditText"));


        for (int i = 0; i < MaxRecentFiles; ++i) {
            recentFileActs[i] = new QAction(this);
            recentFileActs[i]->setVisible(false);
            connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
        }

        // accelerator shortcuts
        // QKeySequence::SelectAll
        QShortcut *shortcut = new QShortcut(QKeySequence(tr("Ctrl+A")),
                                           _QtMainWindow,
                                           SLOT(processShortcutCommand()),
                                           //SLOT(on_SelectAllShortCut_activated()),
                                           0,//SLOT(on_GridToggleShortCut_activated()),
                                           Qt::WindowShortcut);
        shortcut->setProperty("cmid",2012);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+S")),
                                           _QtMainWindow,
                                           SLOT(on_GridToggleShortCut_activated()),
                                           0,//SLOT(on_GridToggleShortCut_activated()),
                                           Qt::WindowShortcut);
        shortcut->setProperty("cmid",421);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+G")),
                                           _QtMainWindow,
                                           SLOT(on_GridDisplayShortCut_activated()),
                                           0,//SLOT(on_GridDisplayShortCut_activated()),
                                           Qt::WindowShortcut);
        shortcut->setProperty("cmid",420);
        mWindowShortcuts.append(shortcut);

        /*
#ifdef _MAC
        shortcut = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_Tab),
#else
        SwitchWindowShortCut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab),
#endif
                                             _QtMainWindow,
                                             SLOT(on_SwitchWindowShortCut_activated()),
                                             0,//SLOT(on_SwitchWindowShortCut_activated()),
                                             Qt::WindowShortcut);
        // KMJ: ToDo make a switch window command for this short cut
        shortcut->setProperty("cmid",0);
        mWindowShortcuts.append(shortcut);
        */

        shortcut = new QShortcut(QKeySequence(tr("Alt+M")),
                                               _QtMainWindow,
                                               SLOT(on_TransformMoveShortCut_activated()),
                                               0,//SLOT(on_TransformMoveShortCut_activated()),
                                               Qt::WindowShortcut);
        shortcut->setProperty("cmid",306);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+C")),
                                              _QtMainWindow,
                                              SLOT(on_TransformCopyShortCut_activated()),
                                              0,//SLOT(on_TransformCopyShortCut_activated()),
                                              Qt::WindowShortcut);
        shortcut->setProperty("cmid",301);
        mWindowShortcuts.append(shortcut);

        //shortcut = new QShortcut(QKeySequence(tr("Alt+S")),
        //                                       _QtMainWindow,
        //                                       SLOT(on_TransformScaleShortCut_activated()),
        //                                       0,//SLOT(on_TransformScaleShortCut_activated()),
        //                                       Qt::WindowShortcut);
        //shortcut->setProperty("cmid",303);

        shortcut = new QShortcut(QKeySequence(tr("Alt+R")),
                                                _QtMainWindow,
                                                SLOT(on_TransformRotateShortCut_activated()),
                                                0,//SLOT(on_TransformRotateShortCut_activated()),
                                                Qt::WindowShortcut);
        shortcut->setProperty("cmid",305);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_I/*tr("Alt+i"*/),
                                                _QtMainWindow,
                                                SLOT(on_TransformMirrorShortCut_activated()),
                                                0,//SLOT(on_TransformRotateShortCut_activated()),
                                                Qt::WindowShortcut);
        shortcut->setProperty("cmid",304);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+T")),
                                                 _QtMainWindow,
                                                 SLOT(on_TransformStretchShortCut_activated()),
                                                 0,
                                                 Qt::WindowShortcut);
        shortcut->setProperty("cmid",300);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+A")),
                                                _QtMainWindow,
                                                SLOT(on_TransformCopyArrayShortCut_activated()),
                                                0,
                                                Qt::WindowShortcut);
        shortcut->setProperty("cmid",307);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+N")),
                                                _QtMainWindow,
                                                SLOT(on_TransformCopyAlongShortCut_activated()),
                                                0,
                                                Qt::WindowShortcut);
        shortcut->setProperty("cmid",308);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+P")),
                                                 _QtMainWindow,
                                                 SLOT(on_TransformProjectShortCut_activated()),
                                                 0,
                                                 Qt::WindowShortcut);
        shortcut->setProperty("cmid",302);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+D")),
                                                 _QtMainWindow,
                                                 SLOT(on_TransformOffsetDistanceShortCut_activated()),
                                                 0,
                                                 Qt::WindowShortcut);
        shortcut->setProperty("cmid",1906);
        mWindowShortcuts.append(shortcut);

        shortcut = new QShortcut(QKeySequence(tr("Alt+O")),
                                                _QtMainWindow,
                                                SLOT(on_TransformOffsetDigitizeShortCut_activated()),
                                                0,
                                                Qt::WindowShortcut);
        shortcut->setProperty("cmid",1907);
        mWindowShortcuts.append(shortcut);

        menuBar = new QMenuBar(_QtMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 24));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        //menu_Print_preview = new QMenu(menuFile);
        //menu_Print_preview->setObjectName(QStringLiteral("menu_Print_preview"));
        menu_Export = new QMenu(menuFile);
        menu_Export->setObjectName(QStringLiteral("menu_Export"));
        menu_Import = new QMenu(menuFile);
        menu_Import->setObjectName(QStringLiteral("menu_Import"));
        menu_Edit = new QMenu(menuBar);
        menu_Edit->setObjectName(QStringLiteral("menu_Edit"));
        menu_Transform = new QMenu(menuBar);
        menu_Transform->setObjectName(QStringLiteral("menu_Transform"));
        menu_Options = new QMenu(menuBar);
        menu_Options->setObjectName(QStringLiteral("menu_Options"));
        menu_Change_Workplane = new QMenu(menu_Options);
        menu_Change_Workplane->setObjectName(QStringLiteral("menu_Change_Workplane"));
        //menu_Applications = new QMenu(menu_Options);
        //menu_Applications->setObjectName(QStringLiteral("menu_Applications"));
        menu_Arrange = new QMenu(menuBar);
        menu_Arrange->setObjectName(QStringLiteral("menu_Arrange"));
        menu_Tools = new QMenu(menuBar);
        menu_Tools->setObjectName(QStringLiteral("menu_Tools"));
        menu_Customize = new QMenu(menu_Tools);
        menu_Customize->setObjectName(QStringLiteral("menu_Customize"));
        menu_Window = new QMenu(menuBar);
        menu_Window->setObjectName(QStringLiteral("menu_Window"));
        //menu3D_View = new QMenu(menu_Window);
        //menu3D_View->setObjectName(QStringLiteral("menu3D_View"));
        //menu3D_view_change = new QMenu(menu_Window);
        //menu3D_view_change->setObjectName(QStringLiteral("menu3D_view_c_hange"));
        //menu_Zone_2 = new QMenu(menu_Window);
        //menu_Zone_2->setObjectName(QStringLiteral("menu_Zone_2"));
        //menu_Compose_drawing = new QMenu(menu_Window);
        //menu_Compose_drawing->setObjectName(QStringLiteral("menu_Compose_drawing"));
        //menu_Raytrace = new QMenu(menu_Window);
        //menu_Raytrace->setObjectName(QStringLiteral("menu_Raytrace"));
        //menu_Raytrace->setEnabled(false);
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QStringLiteral("menu_Help"));
        _QtMainWindow->setMenuBar(menuBar);


        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menu_Edit->menuAction());
        menuBar->addAction(menu_Transform->menuAction());
        menuBar->addAction(menu_Options->menuAction());
        menuBar->addAction(menu_Arrange->menuAction());
        menuBar->addAction(menu_Tools->menuAction());
        menuBar->addAction(menu_Window->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menuFile->addAction(action_File_New);
        menuFile->addAction(action_File_New_from_Template);
        menuFile->addAction(action_File_Open);
        menuFile->addAction(action_File_Save);
        menuFile->addAction(action_File_Save_as);
        menuFile->addAction(action_File_Save_Options);
        menuFile->addSeparator();
        //menuFile->addAction(menu_Print_preview->menuAction());
        menuFile->addAction(action_File_Print);
        menuFile->addAction(action_File_Print_Preview_Window);
        menuFile->addAction(action_File_Print_Preview_Selection);
        menuFile->addSeparator();
        menuFile->addAction(menu_Export->menuAction());
        menuFile->addAction(menu_Import->menuAction());
        menuFile->addAction(action_File_Commands);
        menuFile->addSeparator();
        menuFile->addAction(action_Edit_Info);
        menuFile->addAction(action_Edit_Find_in_files);

        separatorAct = menuFile->addSeparator();
        for (int i = 0; i < MaxRecentFiles; ++i)
            menuFile->addAction(recentFileActs[i]);

        menuFile->addSeparator();
        menuFile->addAction(action_File_Exit);
        //menu_Print_preview->addAction(action_File_Print_Preview_Window);
        //menu_Print_preview->addAction(action_File_Print_Preview_Selection);
        menu_Export->addAction(action_File_Export_DXF);
        menu_Export->addAction(action_File_Export_DWG);
        menu_Export->addAction(action_File_Export_IGES);
        menu_Export->addAction(action_File_Export_Stroked_IGES);
        menu_Export->addAction(action_File_Export_HPGL);
        menu_Import->addAction(action_File_Import_DXF);
        menu_Import->addAction(action_File_Import_DWG);
        menu_Import->addAction(action_File_Import_IGES);
        menu_Import->addAction(action_File_Import_HPGL);
        //menu_Import->addAction(action_File_Import_Medusa);
        menu_Import->addAction(action_File_Import_SKP);
        menu_Import->addAction(action_File_Import_3DWarehouse);
        menu_Edit->addAction(action_Edit_Undo);
        menu_Edit->addAction(action_Edit_Redo);
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Edit_Cut);
        menu_Edit->addAction(action_Edit_Copy);
        menu_Edit->addAction(action_Edit_Copy_2D);
        menu_Edit->addAction(action_Edit_Copy_2D_Window);
        menu_Edit->addAction(action_Edit_Copy_Bitmap);
        menu_Edit->addAction(action_Edit_Paste);
        menu_Edit->addAction(action_Edit_Delete);
        // menu_Edit->addAction(action_Popup1_UnhideAll);
        menu_Edit->addSeparator();
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Edit_New_Attribute);
        menu_Edit->addAction(action_Edit_Delete_Attribute);
        menu_Edit->addAction(action_Edit_Attribute);
        menu_Edit->addAction(action_Edit_List_Attribute);
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Edit_Find_and_replace);
        menu_Edit->addSeparator();
        menu_Transform->addAction(action_Transform_Stretch);
        menu_Transform->addAction(action_Transform_Translate_Copy);
        menu_Transform->addAction(action_Transform_Move);
        menu_Transform->addAction(action_Transform_Copy_Array);
        menu_Transform->addAction(action_Transform_Copy_Along);
        menu_Transform->addAction(action_Transform_Project);
        menu_Transform->addAction(action_Transform_Scale);
        menu_Transform->addAction(action_Transform_Mirror);
        menu_Transform->addAction(action_Transform_Rotate);
        menu_Options->addAction(action_Options_Cursor_on_work_plane);
        menu_Options->addAction(action_Options_Cursor_on_view);
        menu_Options->addAction(action_Options_Cursor_on_Depth);
        menu_Options->addSeparator();
        menu_Options->addAction(action_Options_Select_workplane);
        menu_Options->addAction(action_Options_Create_workplane);
        menu_Options->addAction(menu_Change_Workplane->menuAction());
        menu_Options->addAction(action_Options_Delete_workplane);
        menu_Options->addSeparator();
        //menu_Options->addAction(action_Options_Line);
        menu_Options->addAction(action_Options_Plane);
        menu_Options->addAction(action_Options_Ruled_surface);
        menu_Options->addAction(action_Options_Surface_of_revolution);
        menu_Options->addAction(action_Options_Patch);
        menu_Options->addAction(action_Options_Text);
        menu_Options->addAction(action_Options_Figure);
        menu_Options->addAction(action_Options_Bitmap);
        menu_Options->addAction(action_Options_Group);
        menu_Options->addAction(action_Options_Dimension);
        menu_Options->addAction(action_Options_Leader);
        // kmj temporary option item for light options
        menu_Options->addAction(action_Options_Lights);
        //
        menu_Options->addSeparator();
        menu_Options->addAction(action_Options_Grid);
        menu_Options->addSeparator();
#ifdef _MAC
        menu_Options->addAction(action_Options_Preferences);
#endif
        menu_Options->addAction(action_Options_System);
        menu_Options->addSeparator();
        //menu_Options->addAction(menu_Applications->menuAction());
        menu_Change_Workplane->addAction(action_Options_Change_Workplane_Origin);
        menu_Change_Workplane->addAction(action_Options_Change_Workplane_Three_points);
        menu_Change_Workplane->addAction(action_Options_Change_Workplane_Reset_to_standard);
        menu_Change_Workplane->addAction(action_Options_Change_Workplane_Onto_Entity);
        //menu_Applications->addAction(action_Options_Applications_Animation);
        //menu_Applications->addAction(action_Options_Applications_Architect);
        //menu_Applications->addAction(action_Options_Applications_Machining);
        menu_Arrange->addAction(action_Arrange_To_front);
        menu_Arrange->addAction(action_Arrange_To_back);
        menu_Arrange->addAction(action_Arrange_Forward_one);
        menu_Arrange->addAction(action_Arrange_Back_one);
        menu_Tools->addAction(menu_Customize->menuAction());
        menu_Tools->addAction(action_Tools_Run_program);
        menu_Tools->addAction(action_Tools_Open_program);
        menu_Tools->addAction(action_Tools_New_program);
        menu_Tools->addAction(action_Tools_Load_application);
        menu_Tools->addAction(action_Tools_Unload_application);
        menu_Tools->addAction(action_Tools_Display_command_window);
        menu_Tools->addSeparator();
        menu_Customize->addAction(action_Tools_Customize_Display_custom_menu_horizontal);
        menu_Customize->addAction(action_Tools_Customize_Display_custom_menu_vertical);
        menu_Customize->addAction(action_Tools_Customize_Add_to_custom_menu);
        menu_Customize->addAction(action_Tools_Customize_Delete_from_custom_menu);
        menu_Customize->addAction(action_Tools_Customize_Keyboard_Shortcuts);
        //menu_Window->addAction(action_Window_Display_3D_view);
        //menu_Window->addAction(action_Window_Display_composed_drawing);
        //menu_Window->addAction(action_Window_Display_layer_window);
        //menu_Window->addAction(action_Window_Cascade);
        menu_Window->addAction(action_Window_Tile);
        menu_Window->addAction(action_Window_Tile_Horizontal);
        menu_Window->addAction(action_Window_Tile_Vertical);
        //menu_Window->addAction(action_Window_Regenerate_window);
        menu_Window->addAction(action_Window_Regenerate_all);
        menu_Window->addSeparator();
        //menu_Window->addAction(menu3D_View->menuAction());
        //menu_Window->addAction(menu3D_view_c_hange->menuAction());
        //menu_Window->addAction(menu_Zone_2->menuAction());
        menu_Window->addSeparator();
        //menu_Window->addAction(menu_Compose_drawing->menuAction());
        menu_Window->addSeparator();
        //menu_Window->addAction(action_Window_Shade);
        //menu_Window->addAction(action_Window_Hide);
        //menu_Window->addAction(action_Window_Hide_and_dash_hidden_lines);
        //menu_Window->addAction(menu_Raytrace->menuAction());
        menu_Window->addSeparator();
        menu_Window->addAction(action_Window_Tool_bars);
        //menu_Window->addAction(action_Window_Show_properties);
        menu_Window->addSeparator();

        //menu_Window->addAction(action_Window_Switch_screens);
#ifndef _MAC
        menu_Window->addAction(action_Window_Show_FullScreen);
#else
//#ifdef _RCDEBUG
        menu_Window->addAction(action_Window_Show_FullScreen);
//#endif
#endif
        menu_Window->addAction(action_Window_Show_Maximised);
        menu_Window->addAction(action_Window_Show_Normal);

        //menu3D_View->addAction(action_Window_3Dview_Create);
        //menu3D_View->addAction(action_Window_3Dview_Delete);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_All);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Eye_point);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Reference_point);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Modify_incremental);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Architectural_perspective);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Focal_distance);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Clip_planes);
        //menu3D_view_change->addAction(action_Window_3Dview_ChangeTo_perspective);
        //menu3D_view_change->addAction(action_Window_3Dview_ChangeTo_parallel);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Rotate);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_To_workplane);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Dynamic_rotate);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Dynamic_rotate_about_axis);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Dynamic_360_rotate);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Dynamic_walk);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Dynamic_fly);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Plot_scale);
        //menu3D_view_change->addAction(action_Window_3Dview_Change_Reset_to_standard);
        //menu_Zone_2->addAction(action_Window_Zone_Create);
        //menu_Zone_2->addAction(action_Window_Zone_Delete);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Create);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Delete);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Rename);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Add_3D_view);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Change_3D_view);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Delete_3D_view);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Scroll_3D_view);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Fit_2D_view);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Align_view);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Show_menu);
        //menu_Compose_drawing->addAction(action_Window_Compose_Drawing_Copy);
        //menu_Raytrace->addAction(action_Window_Raytrace_Insert_light);
        //menu_Raytrace->addAction(action_Window_Raytrace_Insert_light_at_Sun_position);
        //menu_Raytrace->addAction(action_Window_Raytrace_Lights);
        //menu_Raytrace->addAction(action_Window_Raytrace_Surface_properties);
        //menu_Raytrace->addAction(action_Window_Raytrace_Raytrace);
        //menu_Raytrace->addAction(action_Window_Raytrace_Show_Menu);

        // help menu is created in ncmain.cpp
        //menu_Help->addAction(action_Help_Contents_and_Index);
        //menu_Help->addAction(action_Help_Current_command);
        //menu_Help->addSeparator();
        //menu_Help->addAction(action_Help_ProgramUpdates_command);
        //menu_Help->addAction(action_Help_TechnicalSupport_command);
        //menu_Help->addAction(action_Help_RemoteAssistance_command);
        //menu_Help->addSeparator();
        //menu_Help->addAction(action_Help_About);

        // context menus
        // Popup_Menu
        Popup_Menu->addAction(action_Popup_Menu_LocateReferencePoint);
        Popup_Menu->addAction(action_Popup_Menu_LocateNear);
        Popup_Menu->addAction(action_Popup_Menu_LocateAlong);
        Popup_Menu->addAction(action_Popup_Menu_LocateOrigin);
        Popup_Menu->addAction(action_Popup_Menu_LocateEndPoint);
        Popup_Menu->addAction(action_Popup_Menu_LocateIntersection);
        Popup_Menu->addAction(action_Popup_Menu_LocateOffsetPoint);
        Popup_Menu->addAction(action_Popup_Menu_LocateApparentIntersection);
        Popup_Menu->addSeparator();
        Popup_Menu->addAction(action_Popup_Menu_ZoomWindow);
        Popup_Menu->addAction(action_Popup_Menu_ZoomScroll);
        Popup_Menu->addAction(action_Popup_Menu_ZoomIn);
        Popup_Menu->addAction(action_Popup_Menu_ZoomOut);
        Popup_Menu->addAction(action_Popup_Menu_ZoomFit);
        Popup_Menu->addAction(action_Popup_Menu_ZoomPrevious);
        Popup_Menu->addSeparator();
        Popup_Menu->addAction(action_Popup_Menu_SelectChain);
        Popup_Menu->addAction(action_Popup_Menu_SelectPolyWindow);
        //Popup_Menu->addSeparator();
        //Popup_Menu->addAction(action_Popup_Menu_ViewByRotation);
        Popup_Menu->addSeparator();
        Popup_Menu->addAction(action_Popup_Menu_SnapDragSettings);
        Popup_Menu->addSeparator();
        Popup_Menu->addAction(action_Popup1_UnhideAll);

        // Popup1_Menu
        Popup1_Menu->addAction(action_Popup1_Menu_BlankOption );
        Popup1_Menu->addAction(action_Popup1_Menu_Trim );
        Popup1_Menu->addAction(action_Popup1_Menu_TrimLength);
        Popup1_Menu->addAction(action_Popup1_Menu_Corner);
        Popup1_Menu->addAction(action_Popup1_Menu_Explode);
        Popup1_Menu->addMenu(menu_Divide);
        Popup1_Menu->addMenu(menu_Select);
        Popup1_Menu->addMenu(menu_Hide);
        Popup1_Menu->addAction(action_Popup1_UnhideAll);
        Popup1_Menu->addMenu(menu_Clone);
        Popup1_Menu->addAction(action_Popup1_Menu_Properties);
        Popup1_Menu->addMenu(menu_Arrange);
        Popup1_Menu->addAction(action_Popup1_Menu_Material);

        retranslateUi(_QtMainWindow);

        QMetaObject::connectSlotsByName(_QtMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *_QtMainWindow)
    {
        _QtMainWindow->setWindowTitle(QApplication::translate("_QtMainWindow", "Prompt", 0));
        action_File_New->setText(QApplication::translate("_QtMainWindow", "&New", 0));
        action_File_New->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+N", 0));
        action_File_New_from_Template->setText(QApplication::translate("_QtMainWindow", "New from &Template", 0));
        action_File_Open->setText(QApplication::translate("_QtMainWindow", "&Open", 0));
        action_File_Open->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+O", 0));
        action_File_Save->setText(QApplication::translate("_QtMainWindow", "&Save", 0));
        action_File_Save->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+S", 0));
        action_File_Save_as->setText(QApplication::translate("_QtMainWindow", "Save &as...", 0));
        action_File_Save_Options->setText(QApplication::translate("_QtMainWindow", "Save Op&tions", 0));
        action_File_Print->setText(QApplication::translate("_QtMainWindow", "&Print", 0));
        action_File_Print->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+P", 0));
        action_File_Commands->setText(QApplication::translate("_QtMainWindow", "&Commands", 0));
        action_File_Exit->setText(QApplication::translate("_QtMainWindow", "E&xit", 0));
        action_File_Exit->setToolTip(QApplication::translate("_QtMainWindow", "Exit the application", 0));
        action_File_Exit->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+Q", 0));
        action_File_Print_Preview_Window->setText(QApplication::translate("_QtMainWindow", "Print Preview &Window", 0));
        action_File_Print_Preview_Selection->setText(QApplication::translate("_QtMainWindow", "Print Preview Scaled &Region", 0));
        action_File_Export_DXF->setText(QApplication::translate("_QtMainWindow", "&DXF", 0));
        action_File_Export_DXF->setToolTip(QApplication::translate("_QtMainWindow", "Export DXF", 0));
        action_File_Export_DWG->setText(QApplication::translate("_QtMainWindow", "DW&G", 0));
        action_File_Export_DWG->setToolTip(QApplication::translate("_QtMainWindow", "Export DWG", 0));
        action_File_Export_IGES->setText(QApplication::translate("_QtMainWindow", "&IGES", 0));
        action_File_Export_IGES->setToolTip(QApplication::translate("_QtMainWindow", "Export IGES", 0));
        action_File_Export_Stroked_IGES->setText(QApplication::translate("_QtMainWindow", "&Stroked IGES", 0));
        action_File_Export_Stroked_IGES->setToolTip(QApplication::translate("_QtMainWindow", "Export Stroked IGES", 0));
        action_File_Export_HPGL->setText(QApplication::translate("_QtMainWindow", "&HPGL", 0));
        action_File_Export_HPGL->setToolTip(QApplication::translate("_QtMainWindow", "Export HPGL", 0));
        action_File_Import_DXF->setText(QApplication::translate("_QtMainWindow", "&DXF", 0));
        action_File_Import_DXF->setToolTip(QApplication::translate("_QtMainWindow", "Import DXF", 0));
        action_File_Import_DWG->setText(QApplication::translate("_QtMainWindow", "DW&G", 0));
        action_File_Import_DWG->setToolTip(QApplication::translate("_QtMainWindow", "Import DWG", 0));
        action_File_Import_IGES->setText(QApplication::translate("_QtMainWindow", "&IGES", 0));
        action_File_Import_IGES->setToolTip(QApplication::translate("_QtMainWindow", "Import IGES", 0));
        action_File_Import_HPGL->setText(QApplication::translate("_QtMainWindow", "&HPGL", 0));
        action_File_Import_HPGL->setToolTip(QApplication::translate("_QtMainWindow", "Import HPGL", 0));
        action_File_Import_Medusa->setText(QApplication::translate("_QtMainWindow", "&Medusa", 0));
        action_File_Import_Medusa->setToolTip(QApplication::translate("_QtMainWindow", "Import Medusa", 0));
        action_File_Import_SKP->setText(QApplication::translate("_QtMainWindow", "SKP", 0));
        action_File_Import_SKP->setToolTip(QApplication::translate("_QtMainWindow", "Import SKP", 0));
        action_File_Import_3DWarehouse->setText(QApplication::translate("_QtMainWindow", "&3D Warehouse", 0));
        action_File_Import_3DWarehouse->setToolTip(QApplication::translate("_QtMainWindow", "Import from 3D Warehouse", 0));
        action_Edit_Undo->setText(QApplication::translate("_QtMainWindow", "&Undo", 0));
        action_Edit_Undo->setToolTip(QApplication::translate("_QtMainWindow", "Undo the last action", 0));
        action_Edit_Undo->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+Z", 0));
        action_Edit_Redo->setText(QApplication::translate("_QtMainWindow", "&Redo", 0));
        action_Edit_Redo->setToolTip(QApplication::translate("_QtMainWindow", "Redo the last action", 0));
        action_Edit_Redo->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+Y", 0));
        action_Edit_Cut->setText(QApplication::translate("_QtMainWindow", "&Cut", 0));
        action_Edit_Cut->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+X", 0));
        action_Edit_Copy->setText(QApplication::translate("_QtMainWindow", "C&opy", 0));
        action_Edit_Copy->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+C", 0));
        action_Edit_Copy_2D->setText(QApplication::translate("_QtMainWindow", "Copy &2D", 0));
        action_Edit_Copy_2D->setShortcut(QApplication::translate("_QtMainWindow", "Shift+Ctrl+C", 0));
        action_Edit_Copy_2D_Window->setText(QApplication::translate("_QtMainWindow", "Copy 2D &Window", 0));
        action_Edit_Copy_Bitmap->setText(QApplication::translate("_QtMainWindow", "Copy &Bitmap", 0));
        action_Edit_Paste->setShortcut(QApplication::translate("_QtMainWindow", "Ctrl+V", 0));
        action_Edit_Paste->setText(QApplication::translate("_QtMainWindow", "&Paste", 0));
        // this should work on Windows and Mac but it only works on Windows in Qt5.2
        // we currently the handle Mac del shortcut in the MainWindow::charevent function
        //action_Edit_Delete->setShortcut(QApplication::translate("_QtMainWindow", "Del", 0));
        action_Edit_Delete->setText(QApplication::translate("_QtMainWindow", "&Delete", 0));
        action_Edit_Delete->setShortcut(QKeySequence(QKeySequence::Delete));
        action_Edit_New_Attribute->setText(QApplication::translate("_QtMainWindow", "&New Attribute", 0));
        action_Edit_Delete_Attribute->setText(QApplication::translate("_QtMainWindow", "D&elete Attribute", 0));
        action_Edit_Attribute->setText(QApplication::translate("_QtMainWindow", "Ed&it Attribute", 0));
        action_Edit_List_Attribute->setText(QApplication::translate("_QtMainWindow", "&List Attribute", 0));
        action_Edit_Find_and_replace->setText(QApplication::translate("_QtMainWindow", "&Find and replace", 0));
        action_Edit_Find_in_files->setText(QApplication::translate("_QtMainWindow", "F&ind in files", 0));
        action_Edit_Info->setText(QApplication::translate("_QtMainWindow", "File In&fo", 0));

        action_Transform_Stretch->setText(QApplication::translate("_QtMainWindow", "&Stretch", 0));
        action_Transform_Translate_Copy->setText(QApplication::translate("_QtMainWindow", "&Translate (Copy)", 0));
        action_Transform_Move->setText(QApplication::translate("_QtMainWindow", "M&ove", 0));
        action_Transform_Copy_Array->setText(QApplication::translate("_QtMainWindow", "Copy &Array", 0));
        action_Transform_Copy_Along->setText(QApplication::translate("_QtMainWindow", "Copy &Along", 0));
        action_Transform_Project->setText(QApplication::translate("_QtMainWindow", "&Project", 0));
        action_Transform_Scale->setText(QApplication::translate("_QtMainWindow", "S&cale", 0));
        action_Transform_Mirror->setText(QApplication::translate("_QtMainWindow", "&Mirror", 0));
        action_Transform_Rotate->setText(QApplication::translate("_QtMainWindow", "&Rotate", 0));

        action_Options_Cursor_on_work_plane->setText(QApplication::translate("_QtMainWindow", "Cursor on &work plane", 0));
        action_Options_Cursor_on_view->setText(QApplication::translate("_QtMainWindow", "Cursor on &view", 0));
        action_Options_Cursor_on_Depth->setText(QApplication::translate("_QtMainWindow", "Cursor dept&h", 0));
        action_Options_Select_workplane->setText(QApplication::translate("_QtMainWindow", "&Select workplane", 0));
        action_Options_Create_workplane->setText(QApplication::translate("_QtMainWindow", "&Create workplane", 0));
        action_Options_Delete_workplane->setText(QApplication::translate("_QtMainWindow", "&Delete workplane", 0));
        action_Options_Change_Workplane_Origin->setText(QApplication::translate("_QtMainWindow", "&Origin", 0));
        action_Options_Change_Workplane_Three_points->setText(QApplication::translate("_QtMainWindow", "&Three points", 0));
        action_Options_Change_Workplane_Reset_to_standard->setText(QApplication::translate("_QtMainWindow", "&Reset to standard", 0));
        action_Options_Change_Workplane_Onto_Entity->setText(QApplication::translate("_QtMainWindow", "O&nto Entity", 0));
        //action_Options_Line->setText(QApplication::translate("_QtMainWindow", "&Line", 0));
        action_Options_Plane->setText(QApplication::translate("_QtMainWindow", "&Plane", 0));
        action_Options_Ruled_surface->setText(QApplication::translate("_QtMainWindow", "&Ruled surface", 0));
        action_Options_Surface_of_revolution->setText(QApplication::translate("_QtMainWindow", "&Surface of revolution", 0));
        action_Options_Patch->setText(QApplication::translate("_QtMainWindow", "&Patch", 0));
        action_Options_Text->setText(QApplication::translate("_QtMainWindow", "&Text", 0));
        action_Options_Figure->setText(QApplication::translate("_QtMainWindow", "&Component", 0));
        action_Options_Bitmap->setText(QApplication::translate("_QtMainWindow", "&Bitmap", 0));
        action_Options_Group->setText(QApplication::translate("_QtMainWindow", "&Group", 0));
        action_Options_Dimension->setText(QApplication::translate("_QtMainWindow", "D&imension", 0));
        action_Options_Leader->setText(QApplication::translate("_QtMainWindow", "&Leader", 0));
        // KMJ temporary option item for lights
        action_Options_Lights->setText(QApplication::translate("_QtMainWindow", "L&ights", 0));
        //
        action_Options_Grid->setText(QApplication::translate("_QtMainWindow", "G&rid", 0));
        action_Options_System->setText(QApplication::translate("_QtMainWindow", "S&ystem", 0));
        //action_Options_Applications_Animation->setText(QApplication::translate("_QtMainWindow", "&Animation", 0));
        //action_Options_Applications_Architect->setText(QApplication::translate("_QtMainWindow", "A&rchitect", 0));
        //action_Options_Applications_Machining->setText(QApplication::translate("_QtMainWindow", "&Machining", 0));
        action_Arrange_To_front->setText(QApplication::translate("_QtMainWindow", "To &front", 0));
        action_Arrange_To_back->setText(QApplication::translate("_QtMainWindow", "To &back", 0));
        action_Arrange_Forward_one->setText(QApplication::translate("_QtMainWindow", "F&orward one", 0));
        action_Arrange_Back_one->setText(QApplication::translate("_QtMainWindow", "B&ack one", 0));
        action_Tools_Customize_Display_custom_menu_horizontal->setText(QApplication::translate("_QtMainWindow", "&Display custom menu horizontal", 0));
        action_Tools_Customize_Display_custom_menu_vertical->setText(QApplication::translate("_QtMainWindow", "Display custom menu &vertical", 0));
        action_Tools_Customize_Add_to_custom_menu->setText(QApplication::translate("_QtMainWindow", "&Add to custom menu", 0));
        action_Tools_Customize_Delete_from_custom_menu->setText(QApplication::translate("_QtMainWindow", "&Delete from custom menu", 0));
        action_Tools_Customize_Keyboard_Shortcuts->setText(QApplication::translate("_QtMainWindow", "&Keyboard Shortcuts", 0));
        action_Tools_Run_program->setText(QApplication::translate("_QtMainWindow", "&Run program", 0));
        action_Tools_Run_program->setEnabled(false);
        action_Tools_Open_program->setText(QApplication::translate("_QtMainWindow", "&Open program", 0));
        action_Tools_Open_program->setEnabled(false);
        action_Tools_New_program->setText(QApplication::translate("_QtMainWindow", "&New program", 0));
        action_Tools_New_program->setEnabled(false);
        action_Tools_Load_application->setText(QApplication::translate("_QtMainWindow", "&Load application", 0));
        action_Tools_Unload_application->setText(QApplication::translate("_QtMainWindow", "&Unload application", 0));
        action_Tools_Display_command_window->setText(QApplication::translate("_QtMainWindow", "&Display command window", 0));
        action_Tools_Display_command_window->setEnabled(false);
        //action_Window_Display_3D_view->setText(QApplication::translate("_QtMainWindow", "Display &3D view", 0));
        //action_Window_Display_composed_drawing->setText(QApplication::translate("_QtMainWindow", "&Display composed drawing", 0));
        //action_Window_Display_layer_window->setText(QApplication::translate("_QtMainWindow", "Display &layer window", 0));
        //action_Window_Display_layer_window->setToolTip(QApplication::translate("_QtMainWindow", "Display layer window", 0));
        //action_Window_Cascade->setText(QApplication::translate("_QtMainWindow", "C&ascade", 0));
        action_Window_Tile->setText(QApplication::translate("_QtMainWindow", "&Tile", 0));
        action_Window_Tile_Horizontal->setText(QApplication::translate("_QtMainWindow", "Tile Horizontal", 0));
        action_Window_Tile_Vertical->setText(QApplication::translate("_QtMainWindow", "Tile Vertical", 0));
        //action_Window_Regenerate_window->setText(QApplication::translate("_QtMainWindow", "Regenerate &window", 0));
        action_Window_Regenerate_all->setText(QApplication::translate("_QtMainWindow", "Regenerate", 0));
        //action_Window_3Dview_Create->setText(QApplication::translate("_QtMainWindow", "&Create", 0));
        //action_Window_3Dview_Delete->setText(QApplication::translate("_QtMainWindow", "&Delete", 0));
        //action_Window_3Dview_Change_All->setText(QApplication::translate("_QtMainWindow", "&All", 0));
        //action_Window_3Dview_Change_Eye_point->setText(QApplication::translate("_QtMainWindow", "&Eye point", 0));
        //action_Window_3Dview_Change_Reference_point->setText(QApplication::translate("_QtMainWindow", "&Reference point", 0));
        //action_Window_3Dview_Change_Modify_incremental->setText(QApplication::translate("_QtMainWindow", "&Modify incremental", 0));
        //action_Window_3Dview_Change_Modify_incremental->setEnabled(false);
        //action_Window_3Dview_Change_Architectural_perspective->setText(QApplication::translate("_QtMainWindow", "Architectural perspecti&ve", 0));
        //action_Window_3Dview_Change_Focal_distance->setText(QApplication::translate("_QtMainWindow", "Focal d&istance", 0));
        //action_Window_3Dview_Change_Clip_planes->setText(QApplication::translate("_QtMainWindow", "&Clip planes", 0));
        //action_Window_3Dview_ChangeTo_perspective->setText(QApplication::translate("_QtMainWindow", "To &perspective", 0));
        //action_Window_3Dview_ChangeTo_parallel->setText(QApplication::translate("_QtMainWindow", "To para&llel", 0));
        //action_Window_3Dview_Change_Rotate->setText(QApplication::translate("_QtMainWindow", "R&otate", 0));
        //action_Window_3Dview_Change_To_workplane->setText(QApplication::translate("_QtMainWindow", "To &workplane", 0));
        //action_Window_3Dview_Change_Dynamic_rotate->setText(QApplication::translate("_QtMainWindow", "&Dynamic rotate", 0));
        //action_Window_3Dview_Change_Dynamic_rotate->setEnabled(false);
        //action_Window_3Dview_Change_Dynamic_rotate_about_axis->setText(QApplication::translate("_QtMainWindow", "Dynamic rotate about &axis", 0));
        //action_Window_3Dview_Change_Dynamic_rotate_about_axis->setEnabled(false);
        //action_Window_3Dview_Change_Dynamic_360_rotate->setText(QApplication::translate("_QtMainWindow", "Dynamic &360 rotate", 0));
        //action_Window_3Dview_Change_Dynamic_360_rotate->setEnabled(false);
        //action_Window_3Dview_Change_Dynamic_walk->setText(QApplication::translate("_QtMainWindow", "Dynamic &walk", 0));
        //action_Window_3Dview_Change_Dynamic_walk->setEnabled(false);
        //action_Window_3Dview_Change_Dynamic_fly->setText(QApplication::translate("_QtMainWindow", "Dynamic &fly", 0));
        //action_Window_3Dview_Change_Dynamic_fly->setEnabled(false);
        //action_Window_3Dview_Change_Plot_scale->setText(QApplication::translate("_QtMainWindow", "Plot &scale", 0));
        //action_Window_3Dview_Change_Reset_to_standard->setText(QApplication::translate("_QtMainWindow", "Reset to s&tandard", 0));
        //action_Window_Zone_Create->setText(QApplication::translate("_QtMainWindow", "&Create", 0));
        //action_Window_Zone_Delete->setText(QApplication::translate("_QtMainWindow", "&Delete", 0));
        //action_Window_Compose_Drawing_Create->setText(QApplication::translate("_QtMainWindow", "&Create", 0));
        //action_Window_Compose_Drawing_Delete->setText(QApplication::translate("_QtMainWindow", "&Delete", 0));
        //action_Window_Compose_Drawing_Rename->setText(QApplication::translate("_QtMainWindow", "&Rename", 0));
        //action_Window_Compose_Drawing_Add_3D_view->setText(QApplication::translate("_QtMainWindow", "&Add 3D view", 0));
        //action_Window_Compose_Drawing_Change_3D_view->setText(QApplication::translate("_QtMainWindow", "&Change 3D view", 0));
        //action_Window_Compose_Drawing_Delete_3D_view->setText(QApplication::translate("_QtMainWindow", "D&elete 3D view", 0));
        //action_Window_Compose_Drawing_Scroll_3D_view->setText(QApplication::translate("_QtMainWindow", "&Scroll 3D view", 0));
        //action_Window_Compose_Drawing_Fit_2D_view->setText(QApplication::translate("_QtMainWindow", "&Fit 2D view", 0));
        //action_Window_Compose_Drawing_Align_view->setText(QApplication::translate("_QtMainWindow", "A&lign view", 0));
        //action_Window_Compose_Drawing_Show_menu->setText(QApplication::translate("_QtMainWindow", "S&how menu", 0));
        //action_Window_Compose_Drawing_Copy->setText(QApplication::translate("_QtMainWindow", "C&opy", 0));
        //action_Window_Shade->setText(QApplication::translate("_QtMainWindow", "&Shade", 0));
        //action_Window_Hide->setText(QApplication::translate("_QtMainWindow", "&Hide", 0));
        //action_Window_Hide_and_dash_hidden_lines->setText(QApplication::translate("_QtMainWindow", "H&ide and dash hidden lines", 0));
        //action_Window_Raytrace_Insert_light->setText(QApplication::translate("_QtMainWindow", "&Insert light", 0));
        //action_Window_Raytrace_Insert_light_at_Sun_position->setText(QApplication::translate("_QtMainWindow", "Insert light at &Sun position", 0));
        //action_Window_Raytrace_Lights->setText(QApplication::translate("_QtMainWindow", "&Lights", 0));
        //action_Window_Raytrace_Surface_properties->setText(QApplication::translate("_QtMainWindow", "&Surface properties", 0));
        //action_Window_Raytrace_Raytrace->setText(QApplication::translate("_QtMainWindow", "&Raytrace", 0));
        //action_Window_Raytrace_Show_Menu->setText(QApplication::translate("_QtMainWindow", "S&how Menu", 0));
        action_Window_Tool_bars->setText(QApplication::translate("_QtMainWindow", "Tool bars", 0));
        //action_Window_Show_properties->setText(QApplication::translate("_QtMainWindow", "Show properties", 0));

        //action_Window_Switch_screens->setText(QApplication::translate("_QtMainWindow", "Switch screens", 0));
#ifndef _MAC
        action_Window_Show_FullScreen->setText(QApplication::translate("_QtMainWindow", "Show Full Screen", 0));
#else
//#ifdef _RCDEBUG
        action_Window_Show_FullScreen->setText(QApplication::translate("_QtMainWindow", "Show Full Screen", 0));
//#endif
#endif
        action_Window_Show_Maximised->setText(QApplication::translate("_QtMainWindow", "Show Maximised", 0));
        action_Window_Show_Normal->setText(QApplication::translate("_QtMainWindow", "Show Normal", 0));

        // old help items
        /*
        action_Help_Contents_and_Index->setText(QApplication::translate("_QtMainWindow", "&Contents and Index", 0));
        action_Help_Current_command->setText(QApplication::translate("_QtMainWindow", "C&urrent command", 0));
        action_Help_Current_command->setShortcut(QApplication::translate("_QtMainWindow", "F1", 0));
        action_Help_View_RealCAD_Reference_command->setText(QApplication::translate("_QtMainWindow", "View RealCAD Reference Manual...",0));
        action_Help_View_RealCAD_GettingStarted_command->setText(QApplication::translate("_QtMainWindow", "View RealCAD Getting Started Tutorial...",0));
        action_Help_View_LANDWorksCAD_Reference_command->setText(QApplication::translate("_QtMainWindow", "View Landworks Reference Manual...",0));
        action_Help_View_LANDWorksCAD_GettingStarted_command->setText(QApplication::translate("_QtMainWindow", "View Landworks Getting Started Tutorial...",0));
        action_Help_View_Training_Videos_command->setText(QApplication::translate("_QtMainWindow", "View Training Videos...",0));
        action_Help_Visit_RealCAD_Website_command->setText(QApplication::translate("_QtMainWindow", "Visit RealCAD (tm) Web Site...",0));
        action_Help_Visit_LANDWorksCAD_Website_command->setText(QApplication::translate("_QtMainWindow", "Visit Landworks Web Site...",0));
        action_Help_ProgramUpdates_command->setText(QApplication::translate("_QtMainWindow", "&Program Updates...", 0));
        action_Help_TechnicalSupport_command->setText(QApplication::translate("_QtMainWindow", "&Technical Support", 0));
        action_Help_RemoteAssistance_command->setText(QApplication::translate("_QtMainWindow", "&Remote Assistance", 0));
        */
        // new help items
        action_Help_Manual->setText(QApplication::translate("_QtMainWindow", "&Manual", 0));
        action_Help_CheatSheet->setText(QApplication::translate("_QtMainWindow", "&Cheat sheet", 0));
        action_Help_GetHelp->setText(QApplication::translate("_QtMainWindow", "&Get help", 0));
        action_Help_WelcomeScreen->setText(QApplication::translate("_QtMainWindow", "&Welcome screen", 0));
        action_Help_ShareMyScreen->setText(QApplication::translate("_QtMainWindow", "&Share my screen", 0));
        action_Help_OnlineTraining->setText(QApplication::translate("_QtMainWindow", "&Online training", 0));
        action_Help_CheckForUpdates->setText(QApplication::translate("_QtMainWindow", "Check for &updates", 0));
        action_Help_DeactivateLicense->setText(QApplication::translate("_QtMainWindow", "&Deactivate license", 0));
        //action_Help_TellaFriend->setText(QApplication::translate("_QtMainWindow", "&Tell a friend", 0));
        action_Help_About->setText(QApplication::translate("_QtMainWindow", "&About", 0));

        menuFile->setTitle(QApplication::translate("_QtMainWindow", "&File", 0));
        //menu_Print_preview->setTitle(QApplication::translate("_QtMainWindow", "P&rint preview", 0));
        menu_Export->setTitle(QApplication::translate("_QtMainWindow", "&Export", 0));
        menu_Import->setTitle(QApplication::translate("_QtMainWindow", "&Import", 0));
        menu_Edit->setTitle(QApplication::translate("_QtMainWindow", "&Edit", 0));
        menu_Transform->setTitle(QApplication::translate("_QtMainWindow", "&Transform", 0));
        menu_Options->setTitle(QApplication::translate("_QtMainWindow", "&Options", 0));
        menu_Change_Workplane->setTitle(QApplication::translate("_QtMainWindow", "C&hange workplane", 0));
        //menu_Applications->setTitle(QApplication::translate("_QtMainWindow", "&Applications", 0));
        menu_Arrange->setTitle(QApplication::translate("_QtMainWindow", "&Arrange", 0));
        menu_Tools->setTitle(QApplication::translate("_QtMainWindow", "&Tools", 0));
        menu_Customize->setTitle(QApplication::translate("_QtMainWindow", "&Customize", 0));
        menu_Window->setTitle(QApplication::translate("_QtMainWindow", "&Window", 0));
        //menu3D_View->setTitle(QApplication::translate("_QtMainWindow", "3D &View", 0));
        //menu3D_view_change->setTitle(QApplication::translate("_QtMainWindow", "3D view c&hange", 0));
        //menu_Zone_2->setTitle(QApplication::translate("_QtMainWindow", "&Zone", 0));
        //menu_Compose_drawing->setTitle(QApplication::translate("_QtMainWindow", "&Compose drawing", 0));
        //menu_Raytrace->setTitle(QApplication::translate("_QtMainWindow", "&Raytrace", 0));
        menu_Help->setTitle(QApplication::translate("_QtMainWindow", "&Help", 0));

        // Popu_Menu actions
        action_Popup_Menu_LocateReferencePoint->setText(QApplication::translate("_QtMainWindow", "Locate a reference point", 0));
        action_Popup_Menu_LocateNear->setText(QApplication::translate("_QtMainWindow", "Locate near", 0));
        action_Popup_Menu_LocateAlong->setText(QApplication::translate("_QtMainWindow", "Locate along", 0));
        action_Popup_Menu_LocateOrigin->setText(QApplication::translate("_QtMainWindow", "Locate origin", 0));
        action_Popup_Menu_LocateEndPoint->setText(QApplication::translate("_QtMainWindow", "Locate an end point", 0));
        action_Popup_Menu_LocateIntersection->setText(QApplication::translate("_QtMainWindow", "Locate intersection point", 0));
        action_Popup_Menu_LocateOffsetPoint->setText(QApplication::translate("_QtMainWindow", "Locate an offset point", 0));
        action_Popup_Menu_LocateApparentIntersection->setText(QApplication::translate("_QtMainWindow", "Locate an apparent intersection", 0));
        action_Popup_Menu_ZoomWindow->setText(QApplication::translate("_QtMainWindow", "Zoom window", 0));
        action_Popup_Menu_ZoomScroll->setText(QApplication::translate("_QtMainWindow", "Zoom scroll", 0));
        action_Popup_Menu_ZoomIn->setText(QApplication::translate("_QtMainWindow", "Zoom in", 0));
        action_Popup_Menu_ZoomOut->setText(QApplication::translate("_QtMainWindow", "Zoom out", 0));
        action_Popup_Menu_ZoomFit->setText(QApplication::translate("_QtMainWindow", "Zoom fit", 0));
        action_Popup_Menu_ZoomPrevious->setText(QApplication::translate("_QtMainWindow", "Zoom previous", 0));
        action_Popup_Menu_SelectChain->setText(QApplication::translate("_QtMainWindow", "Select a chain of entities", 0));
        action_Popup_Menu_SelectPolyWindow->setText(QApplication::translate("_QtMainWindow", "Select entities using a polywindow", 0));
        //action_Popup_Menu_ViewByRotation->setText(QApplication::translate("_QtMainWindow", "Change view by rotation", 0));
        action_Popup_Menu_SnapDragSettings->setText(QApplication::translate("_QtMainWindow", "Snap and drag settings", 0));

        // Popup1_Menu actions
        // Select submenu actions
        menu_Select->setTitle(QApplication::translate("_QtMainWindow", "&Select", 0));
        action_Select_SameColour->setText(QApplication::translate("_QtMainWindow", "all same &Colour", 0));
        action_Select_SameLayer->setText(QApplication::translate("_QtMainWindow", "all same &Layer", 0));
        action_Select_SameStyle->setText(QApplication::translate("_QtMainWindow", "all same &Style", 0));
        action_Select_SameWeight->setText(QApplication::translate("_QtMainWindow", "all same &Weight", 0));
        action_Select_SameEntity->setText(QApplication::translate("_QtMainWindow", "all same &Entity", 0));
        action_Select_SameName->setText(QApplication::translate("_QtMainWindow", "all same &Name", 0));

        // Hide submenu actions
        menu_Hide->setTitle(QApplication::translate("_QtMainWindow", "&Hide", 0));
        action_Hide_SameColour->setText(QApplication::translate("_QtMainWindow", "all same &Colour", 0));
        action_Hide_SameLayer->setText(QApplication::translate("_QtMainWindow", "all same &Layer", 0));
        action_Hide_SameStyle->setText(QApplication::translate("_QtMainWindow", "all same &Style", 0));
        action_Hide_SameWeight->setText(QApplication::translate("_QtMainWindow", "all same &Weight", 0));
        action_Hide_SameEntity->setText(QApplication::translate("_QtMainWindow", "all same &Entity", 0));
#ifdef _RCDEBUG
        action_Hide_SameName->setText(QApplication::translate("_QtMainWindow", "all same &Name (debug only)", 0));
#endif
        action_Popup1_UnhideAll->setText(QApplication::translate("_QtMainWindow", "&Unhide all", 0));

        action_Popup1_Menu_TrimLength->setText(QApplication::translate("_QtMainWindow", "&Length", 0));
        action_Popup1_Menu_Trim->setText(QApplication::translate("_QtMainWindow", "&Trim/Extend", 0));
        action_Popup1_Menu_BlankOption->setText(QApplication::translate("_QtMainWindow", "&    ", 0));
        action_Popup1_Menu_Corner->setText(QApplication::translate("_QtMainWindow", "&Corner", 0));
        action_Popup1_Menu_Explode->setText(QApplication::translate("_QtMainWindow", "&Explode", 0));
        action_Popup1_Menu_Properties->setText(QApplication::translate("_QtMainWindow", "Show &Properties", 0));
        action_Popup1_Menu_Material->setText(QApplication::translate("_QtMainWindow", "Render &Material", 0));
        //action_Popup1_Menu_EditText->setText(QApplication::translate("_QtMainWindow", "Edit Text", 0));

        // added for v8
        menu_Divide->setTitle(QApplication::translate("_QtMainWindow", "&Divide", 0));
        action_Divide_Entity->setText(QApplication::translate("_QtMainWindow", "by &Entity", 0));
        action_Divide_Length->setText(QApplication::translate("_QtMainWindow", "by &Length", 0));
        action_Divide_Number->setText(QApplication::translate("_QtMainWindow", "by &Number", 0));
        action_Delete_Segment->setText(QApplication::translate("_QtMainWindow", "delete &Segment", 0));

        menu_Clone->setTitle(QApplication::translate("_QtMainWindow", "C&lone", 0));
        action_Clone_Colour->setText(QApplication::translate("_QtMainWindow", "&Colour", 0));
        action_Clone_Layer->setText(QApplication::translate("_QtMainWindow", "&Layer", 0));
        action_Clone_Style->setText(QApplication::translate("_QtMainWindow", "&Style", 0));
        action_Clone_Weight->setText(QApplication::translate("_QtMainWindow", "&Weight", 0));
        action_Clone_All->setText(QApplication::translate("_QtMainWindow", "&All above", 0));
        //

    } // retranslateUi
#endif

public slots:
    void newFile();
    void newFromTemplate();
    void open();
    void save();
    void saveAs();
    void saveOptions();
    void openRecentFile();
    void printSheet(QPrinter* printer);
    void printSheetSelect(QPrinter* printer);
    void printRTSheet(QPrinter* printer);
    void printRTSheetSelect(QPrinter* printer);
    void showSplashScreen(int licensed=0);

    // common slot for all user definable shortcuts
    void processShortcutCommand();

    void on_action_File_New_triggered();

    void on_action_File_New_from_Template_triggered();

    void on_action_File_Open_triggered();

    void on_action_File_Save_triggered();

    void on_action_File_Save_as_triggered();

    void on_action_File_Save_Options_triggered();

    void on_action_File_Print_triggered();

    void on_action_File_Commands_triggered();

    void on_action_File_Exit_triggered();

    void on_action_File_Print_Preview_Window_triggered();

    void on_action_File_Print_Preview_Selection_triggered();

    void on_action_File_Export_DXF_triggered();

    void on_action_File_Export_DWG_triggered();

    void on_action_File_Export_IGES_triggered();

    void on_action_File_Export_Stroked_IGES_triggered();

    void on_action_File_Export_HPGL_triggered();

    void on_action_File_Import_DXF_triggered();

    void on_action_File_Import_DWG_triggered();

    void on_action_File_Import_IGES_triggered();

    void on_action_File_Import_HPGL_triggered();

    void on_action_File_Import_Medusa_triggered();

    void on_action_File_Import_SKP_triggered();

    void on_action_File_Import_3DWarehouse_triggered();

    // edit
    void on_action_Edit_Undo_triggered();
    void on_action_Edit_Redo_triggered();
    void on_action_Edit_Delete_triggered();
    void on_action_Edit_Cut_triggered();
    void on_action_Edit_Copy_triggered();
    void on_action_Edit_Copy_2D_triggered();
    void on_action_Edit_Copy_2D_Window_triggered();
    void on_action_Edit_Copy_Bitmap_triggered();
    void on_action_Edit_Paste_triggered();
    void on_action_Edit_New_Attribute_triggered();
    void on_action_Edit_Delete_Attribute_triggered();
    void on_action_Edit_Attribute_triggered();
    void on_action_Edit_List_Attribute_triggered();
    void on_action_Edit_Find_and_replace_triggered();
    void on_action_Edit_Find_in_files_triggered();
    void on_action_Edit_Info_triggered();


    // transform slots
    void on_action_Transform_Stretch_triggered();
    void on_action_Transform_Translate_Copy_triggered();
    void on_action_Transform_Move_triggered();
    void on_action_Transform_Copy_Array_triggered();
    void on_action_Transform_Copy_Along_triggered();
    void on_action_Transform_Project_triggered();
    void on_action_Transform_Scale_triggered();
    void on_action_Transform_Mirror_triggered();
    void on_action_Transform_Rotate_triggered();

    // short cuts
    void on_SelectAllShortCut_activated();
    void on_GridToggleShortCut_activated();
    void on_GridDisplayShortCut_activated();
    void on_SwitchWindowShortCut_activated();

    void on_TransformMoveShortCut_activated();
    void on_TransformCopyShortCut_activated();
    void on_TransformScaleShortCut_activated();
    void on_TransformRotateShortCut_activated();
    void on_TransformMirrorShortCut_activated();
    void on_TransformStretchShortCut_activated();
    void on_TransformCopyArrayShortCut_activated();
    void on_TransformCopyAlongShortCut_activated();
    void on_TransformProjectShortCut_activated();
    void on_TransformOffsetDigitizeShortCut_activated();
    void on_TransformOffsetDistanceShortCut_activated();

    // options
    void on_action_Options_Cursor_on_work_plane_triggered();
    void on_action_Options_Cursor_on_view_triggered();
    void on_action_Options_Grid_triggered();
    void on_action_Options_Cursor_on_Depth_triggered();
    void on_action_Options_Select_workplane_triggered();
    void on_action_Options_Create_workplane_triggered();
    void on_action_Options_Delete_workplane_triggered();
    void on_action_Options_Change_Workplane_Reset_to_standard_triggered();
    void on_action_Options_Change_Workplane_Origin_triggered();
    void on_action_Options_Change_Workplane_Three_points_triggered();
    void on_action_Options_Change_Workplane_Onto_Entity_triggered();
    //void on_action_Options_Line_triggered();
    void on_action_Options_Plane_triggered();
    void on_action_Options_Ruled_surface_triggered();
    void on_action_Options_Surface_of_revolution_triggered();
    void on_action_Options_Patch_triggered();
    void on_action_Options_Group_triggered();
    void on_action_Options_Text_triggered();
    void on_action_Options_Figure_triggered();
    void on_action_Options_Bitmap_triggered();
    void on_action_Options_Dimension_triggered();
    void on_action_Options_Leader_triggered();
    // KMJ temporary item for Lights
    void on_action_Options_Lights_triggered();
    //
#ifdef _MAC
    void on_action_Options_Preferences_triggered();
#endif
    void on_action_Options_System_triggered();

    // arrange
    void on_action_Arrange_To_front_triggered();
    void on_action_Arrange_To_back_triggered();
    void on_action_Arrange_Forward_one_triggered();
    void on_action_Arrange_Back_one_triggered();

    // tools
    void on_action_Tools_Customize_Display_custom_menu_horizontal_triggered();
    void on_action_Tools_Customize_Display_custom_menu_vertical_triggered();
    void on_action_Tools_Customize_Add_to_custom_menu_triggered();
    void on_action_Tools_Customize_Delete_from_custom_menu_triggered();
    void on_action_Tools_Customize_Keyboard_Shortcuts_triggered();
    void on_action_Tools_Load_application_triggered();
    void on_action_Tools_Unload_application_triggered();

    // window
    //void on_action_Window_Display_3D_view_triggered();
    //void on_action_Window_Display_composed_drawing_triggered();
    //void on_action_Window_Cascade_triggered();
    void on_action_Window_Tile_triggered();
    void on_action_Window_Tile_Horizontal_triggered();
    void on_action_Window_Tile_Vertical_triggered();

    //void on_action_Window_Compose_Drawing_Create_triggered();
    //void on_action_Window_Compose_Drawing_Delete_triggered();
    //void on_action_Window_Compose_Drawing_Rename_triggered();
    //void on_action_Window_Compose_Drawing_Add_3D_view_triggered();
    //void on_action_Window_Compose_Drawing_Change_3D_view_triggered();
    //void on_action_Window_Compose_Drawing_Delete_3D_view_triggered();
    //void on_action_Window_Compose_Drawing_Scroll_3D_view_triggered();
    //void on_action_Window_Compose_Drawing_Fit_2D_view_triggered();
    //void on_action_Window_Compose_Drawing_Align_view_triggered();
    //void on_action_Window_Compose_Drawing_Show_menu_triggered();
    //void on_action_Window_Compose_Drawing_Copy_triggered();

    //void on_action_Window_3Dview_Create_triggered();
    //void on_action_Window_3Dview_Delete_triggered();
    //void on_action_Window_3Dview_Change_All_triggered();
    //void on_action_Window_3Dview_Change_Eye_point_triggered();
    //void on_action_Window_3Dview_Change_Reference_point_triggered();
    //void on_action_Window_3Dview_Change_Modify_incremental_triggered();
    //void on_action_Window_3Dview_Change_Architectural_perspective_triggered();
    //void on_action_Window_3Dview_Change_Clip_planes_triggered();
    //void on_action_Window_3Dview_Change_Focal_distance_triggered();
    //void on_action_Window_3Dview_ChangeTo_perspective_triggered();
    //void on_action_Window_3Dview_ChangeTo_parallel_triggered();
    //void on_action_Window_3Dview_Change_Rotate_triggered();
    //void on_action_Window_3Dview_Change_To_workplane_triggered();
    //void on_action_Window_3Dview_Change_Dynamic_rotate_triggered();
    //void on_action_Window_3Dview_Change_Dynamic_rotate_about_axis_triggered();
    //void on_action_Window_3Dview_Change_Dynamic_360_rotate_triggered();
    //void on_action_Window_3Dview_Change_Dynamic_walk_triggered();
    //void on_action_Window_3Dview_Change_Dynamic_fly_triggered();
    //void on_action_Window_3Dview_Change_Plot_scale_triggered();
    //void on_action_Window_3Dview_Change_Reset_to_standard_triggered();
    //void on_action_Window_Zone_Create_triggered();
    //void on_action_Window_Zone_Delete_triggered();
    //void on_action_Window_Shade_triggered();
    //void on_action_Window_Hide_triggered();
    //void on_action_Window_Hide_and_dash_hidden_lines_triggered();

    void on_action_Window_Show_FullScreen_triggered();
    void on_action_Window_Show_Maximised_triggered();
    void on_action_Window_Show_Normal_triggered();
    void on_action_Window_Tool_bars_triggered();

    // old help menu items
    void on_action_Help_Contents_and_Index_triggered();
    void on_action_Help_Current_command_triggered();
    void on_action_Help_View_RealCAD_Reference_command_triggered();
    void on_action_Help_View_RealCAD_GettingStarted_command_triggered();
    void on_action_Help_View_LANDWorksCAD_Reference_command_triggered();
    void on_action_Help_View_LANDWorksCAD_GettingStarted_command_triggered();
    void on_action_Help_View_Training_Videos_command_triggered();
    void on_action_Help_Visit_RealCAD_Website_command_triggered();
    void on_action_Help_Visit_LANDWorksCAD_Website_command_triggered();
    void on_action_Help_ProgramUpdates_command_triggered();
    void on_action_Help_TechnicalSupport_command_triggered();
    void on_action_Help_RemoteAssistance_command_triggered();

    // new help menu items
    void on_action_Help_Manual_triggered();
    void on_action_Help_CheatSheet_triggered();
    void on_action_Help_GetHelp_triggered();
    void on_action_Help_WelcomeScreen_triggered();
    void on_action_Help_ShareMyScreen_triggered();
    void on_action_Help_OnlineTraining_triggered();
    void on_action_Help_CheckForUpdates_triggered();
    void on_action_Help_DeactivateLicense_triggered();
    void on_action_Help_TellaFriend_triggered();
    void on_action_Help_About_triggered();

    // Popup_Menu actions
    void on_action_Popup_Menu_LocateReferencePoint_triggered();
    void on_action_Popup_Menu_LocateNear_triggered();
    void on_action_Popup_Menu_LocateAlong_triggered();
    void on_action_Popup_Menu_LocateOrigin_triggered();
    void on_action_Popup_Menu_LocateEndPoint_triggered();
    void on_action_Popup_Menu_LocateIntersection_triggered();
    void on_action_Popup_Menu_LocateOffsetPoint_triggered();
    void on_action_Popup_Menu_LocateApparentIntersection_triggered();
    void on_action_Popup_Menu_ZoomWindow_triggered();
    void on_action_Popup_Menu_ZoomScroll_triggered();
    void on_action_Popup_Menu_ZoomIn_triggered();
    void on_action_Popup_Menu_ZoomOut_triggered();
    void on_action_Popup_Menu_ZoomFit_triggered();
    void on_action_Popup_Menu_ZoomPrevious_triggered();
    void on_action_Popup_Menu_SelectChain_triggered();
    void on_action_Popup_Menu_SelectPolyWindow_triggered();
    void on_action_Popup_Menu_ViewByRotation_triggered();
    void on_action_Popup_Menu_SnapDragSettings_triggered();

    // PopUp1_Menu actions
    void on_action_Popup1_Menu_TrimLength_triggered();
    void on_action_Popup1_Menu_Trim_triggered();
    void on_action_Popup1_Menu_Corner_triggered();
    void on_action_Popup1_Menu_Explode_triggered();
    void on_action_Popup1_Menu_Properties_triggered();
    void on_action_Popup1_Menu_Material_triggered();
    void on_action_Popup1_Menu_DimensionAddPoint_triggered();
    void on_action_Popup1_Menu_DimensionMovePoint_triggered();
    void on_action_Popup1_Menu_DimensionDeletePoint_triggered();
    void on_action_Popup1_Menu_DimensionMoveText_triggered();
    void on_action_Popup1_Menu_EditText_triggered();

    // select menu actions
    void on_action_Select_SameColour_triggered();
    void on_action_Select_SameLayer_triggered();
    void on_action_Select_SameStyle_triggered();
    void on_action_Select_SameWeight_triggered();
    void on_action_Select_SameEntity_triggered();
    void on_action_Select_SameName_triggered();

    // hide menu actions
    void on_action_Hide_SameColour_triggered();
    void on_action_Hide_SameLayer_triggered();
    void on_action_Hide_SameStyle_triggered();
    void on_action_Hide_SameWeight_triggered();
    void on_action_Hide_SameEntity_triggered();
    void on_action_Hide_SameName_triggered();
    void on_action_Popup1_UnhideAll_triggered();

    // divide menu actions
    void on_action_Divide_Entity_triggered();
    void on_action_Divide_Length_triggered();
    void on_action_Divide_Number_triggered();
    void on_action_Delete_Segment_triggered();

    // clone menu actions
    void on_action_Clone_Colour_triggered();
    void on_action_Clone_Layer_triggered();
    void on_action_Clone_Style_triggered();
    void on_action_Clone_Weight_triggered();
    void on_action_Clone_All_triggered();

public slots:
    void timerFunc() { ; }
    void popup1MenuRequested(const QPoint &pos);
    void popupMenuRequested(const QPoint &pos);
    void closeTabWindow(int index);
    void currentTabChanged(int index);
    void closeSubWindow(QWidget *wdw);
    void subWindowActivated(QMdiSubWindow *sbw);
    void onViewActivated(RCView *view);
    void onViewResize(RCView *view);
    void onViewClosed(RCView *view);
    View3dWindow* activeWindow();
    RCView* activeView();
    void showFullScreen();
    void screenChanged(QScreen *screen);
    void screenCountChanged(int count);
    void onToolBarVisibilyChanged(bool);

    // tests
    void testSplitHorizontal(int cycles=10);
    void testSplitVertical(int cycles=10);
    void testTiling(int cycles=10);
    void testNew(int cycles=10);
private:

    bool m_isClosing;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *transformMenu;
    QMenu *optionsMenu;
    QMenu *arrangeMenu;
    QMenu *toolsMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QAction *newAct;
    QAction *newFromAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *saveOptionsAct;
    QAction *exitAct;

signals:
    //void changed(const QMimeData *mimedata = 0);

private:
    //explicit QtMainWindow(QWidget *parent);

    void createMenus();
    void createToolbars();
    void createDockWidgets();
    void createActions();
	
protected:
    void paintEvent(QPaintEvent *e);
    void changeEvent(QEvent *e);
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
#ifdef USE_UI
    Ui::QtMainWindow *ui;
#endif
};

#endif // MAINWINDOW_H
