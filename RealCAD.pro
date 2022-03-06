#-------------------------------------------------
#
# Project created by QtCreator 2013-04-08T23:18:10
#
#-------------------------------------------------

message( "qt version is: " $$QT_VERSION )
QT       += core gui widgets webenginewidgets webchannel network svg xml opengl printsupport
QT       += concurrent


TARGET    = RealCAD
TEMPLATE  = app

CONFIG -= embed_manifest_exe

DEFINES += UNICODE _USING_QT USING_WIDGETS VERSION=REALCAD CADAUSTRALIAVERSION=1
DEFINES += NEW_DRAGGING
DEFINES += USING_GLDLISTS

#DEFINES += MAJOR_VERSION=7
#DEFINES += MINOR_VERSION=12
DEFINES += MAJOR_VERSION=8
DEFINES += MINOR_VERSION=3

CONFIG(debug, debug|release) {
message( "Building debug version" )
DEFINES -= QT_NO_DEBUG_OUTPUT
DEFINES += NEW_RUBBERB
DEFINES += _RCDEBUG
win32 {
DEFINES += _CRT_NO_VA_START_VALIDATION
DEFINES -= _MAC
QMAKE_CFLAGS += -MP
QMAKE_CXXFLAGS += -MP
#TODO: use strict strings
QMAKE_CFLAGS -= -Zc:strictStrings
QMAKE_CXXFLAGS -= -Zc:strictStrings
}
} else {
message( "Building release version" )
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += NEW_RUBBERB
win32 {
DEFINES -= _MAC
DEFINES += _CRT_NO_VA_START_VALIDATION
QMAKE_CFLAGS += -MP
QMAKE_CXXFLAGS += -MP
#CONFIG += force_debug_info
QMAKE_CFLAGS -= -Zc:strictStrings
QMAKE_CXXFLAGS -= -Zc:strictStrings
QMAKE_CFLAGS_RELEASE -= -Zc:strictStrings
QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings
QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings
QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO -= -Zc:strictStrings
}
}

macx {
message( "Building Mac version" )
DEFINES += _MAC
CONFIG += c++11
#CONFIG += c++14
#QMAKE_MAC_SDK = macosx10.13
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13
QMAKE_CXXFLAGS_WARN_ON =  -w -fdiagnostics-show-option
QMAKE_LFLAGS += -rpath @executable_path/../Frameworks
QMAKE_INFO_PLIST = Info_Real.plist
#QMAKE_CFLAGS_RELEASE += -g
#CONFIG += force_debug_info
LIBS += -framework cocoa
}

win32 {
message( "Building Windows version" )
CONFIG -= embed_manifest_exe
QMAKE_CXXFLAGS_WARN_ON = /W0

LIBS += -L"c:/Program Files (x86)\Windows Kits\10\Lib\10.0.10586.0\um\x64"

LIBS += \
        OpenGL32.lib \
        GLU32.lib \
        Advapi32.lib
}

RESOURCES += RealCAD.qrc

#TRANSLATIONS = RealCAD_es.ts

# the application icon for Windows
win32 {
RC_FILE  += RealCAD.rc
}

# the application icon for the Mac
macx {
ICON = RealCAD.icns
}

SOURCES += \
    source/attributedelete_dialog.cpp \
    source/attributeedit_dialog.cpp \
    source/attributelist_dialog.cpp \
    source/attributenew_dialog.cpp \
    source/bitmap_dialog.cpp \
    source/calculator_dialog.cpp \
    source/Chamfer_Dialog.cpp \
    source/circleverify_dialog.cpp \
    source/composeddrawing_dialog.cpp \
    source/converttocurve_dialog.cpp \
    source/copyalong_dialog.cpp \
    source/copyarray_dialog.cpp \
    source/copybitmap_dialog.cpp \
    source/copycomposeddrawing_dialog.cpp \
    source/createview_dialog.cpp \
    source/createworkplane_dialog.cpp \
    source/createzone_dialog.cpp \
    source/curve_dialog.cpp \
    source/curveverify_dialog.cpp \
    source/definecolour_dialog.cpp \
    source/definepatternstyle_dialog.cpp \
    source/definestyle_dialog.cpp \
    source/deletedrawing_dialog.cpp \
    source/deleteview_dialog.cpp \
    source/deleteworkplane_dialog.cpp \
    source/deletezone_dialog.cpp \
    source/dimensiontext_dialog.cpp \
    source/drawingverify_dialog.cpp \
    source/ellipseinsert_dialog.cpp \
    source/ellipseverify_dialog.cpp \
    source/explode_dialog.cpp \
    source/figure_dialog.cpp \
    source/figuretext_dialog.cpp \
    source/figureverify_dialog.cpp \
    source/findandreplace_dialog.cpp \
    source/findinfiles_dialog.cpp \
    source/GeneratePoints_Dialog.cpp \
    source/grid_dialog.cpp \
    source/group_dialog.cpp \
    source/groupverify_dialog.cpp \
    source/information_dialog.cpp \
    source/insertfiguretext_dialog.cpp \
    source/layergroup_dialog.cpp \
    source/layerspinbox.cpp \
    source/leader_dialog.cpp \
    source/line_dialog.cpp \
    source/LineAngle_Dialog.cpp \
    source/lineparallel_dialog.cpp \
    source/lineverify_dialog.cpp \
    source/lttextinsert_dialog.cpp \
    source/mainwindow.cpp \
    source/maskchange_dialog.cpp \
    source/measure_dialog.cpp \
    source/measureangle_dialog.cpp \
    source/measurearea_dialog.cpp \
    source/measurelengtharea_dialog.cpp \
    source/mirror_dialog.cpp \
    source/missing_dialog.cpp \
    source/modifyparametricfigure_dialog.cpp \
    source/move_dialog.cpp \
    source/Ncanim.cpp \
    source/ncapi.cpp \
    source/ncappl.cpp \
    source/Ncarcb.cpp \
    source/ncarcha.cpp \
    source/ncarchb.cpp \
    source/ncarchc.cpp \
    source/Ncarrang.cpp \
    source/Ncattrib.cpp \
    source/ncbasica.cpp \
    source/ncbasicb.cpp \
    source/ncbeam.cpp \
    source/ncbmapa.cpp \
    source/Ncbmapb.cpp \
    source/Ncbmask.cpp \
    source/Nccira.cpp \
    source/Nccirb.cpp \
    source/Nccirc.cpp \
    source/Nccird.cpp \
    source/Ncclip.cpp \
    source/Nccomm.cpp \
    source/Nccurvea.cpp \
    source/Nccurveb.cpp \
    source/nccustom.cpp \
    source/Ncdialog.cpp \
    source/Ncdimena.cpp \
    source/Ncdimenb.cpp \
    source/Ncdimenc.cpp \
    source/Ncdimend.cpp \
    source/Ncdimene.cpp \
    source/Ncdimenf.cpp \
    source/ncdimeng.cpp \
    source/Ncdxfin.cpp \
    source/Ncdxfout.cpp \
    source/Ncellipa.cpp \
    source/Ncellipb.cpp \
    source/ncent.cpp \
    source/Ncfiga.cpp \
    source/Ncfigb.cpp \
    source/Ncfile.cpp \
    source/ncfilem.cpp \
    source/Ncgrid.cpp \
    source/Ncgroupa.cpp \
    source/Ncgroupb.cpp \
    source/Nchidea.cpp \
    source/Nchideb.cpp \
    source/nchpglin.cpp \
    source/Nchpgout.cpp \
    source/ncigesin.cpp \
    source/Ncigsout.cpp \
    source/Ncinters.cpp \
    source/Nclight.cpp \
    source/Nclinea.cpp \
    source/Nclineb.cpp \
    source/nclinec.cpp \
    source/Nclist.cpp \
    source/ncmain.cpp \
    source/Ncmask.cpp \
    source/Ncmatrix.cpp \
    source/Ncmeas.cpp \
    source/ncmeduin.cpp \
    source/Ncmeta.cpp \
    source/Ncnc.cpp \
    source/ncpanel.cpp \
    source/Ncpatcha.cpp \
    source/Ncpatchb.cpp \
    source/Ncplanea.cpp \
    source/Ncplaneb.cpp \
    source/ncplanec.cpp \
    source/Ncpoint.cpp \
    source/Ncpointb.cpp \
    source/Ncprint.cpp \
    source/Ncraya.cpp \
    source/Ncrayb.cpp \
    source/Ncreva.cpp \
    source/Ncrevb.cpp \
    source/Ncrulea.cpp \
    source/Ncruleb.cpp \
    source/Ncshadea.cpp \
    source/Ncshadeb.cpp \
    source/ncsim.cpp \
    source/Nctexta.cpp \
    source/Nctextb.cpp \
    source/Nctextc.cpp \
    source/Nctrans.cpp \
    source/Nctrim.cpp \
    source/Ncval.cpp \
    source/Ncviewa.cpp \
    source/Ncviewb.cpp \
    source/Ncviewc.cpp \
    source/ncwina.cpp \
    source/ncwinb.cpp \
    source/ncwinc.cpp \
    source/Ncwork.cpp \
    source/nczone.cpp \
    source/offset_dialog.cpp \
    source/offsetd_dialog.cpp \
    source/oneviewchange_dialog.cpp \
    source/patch_dialog.cpp \
    source/patchcorners_dialog.cpp \
    source/patchmesh_dialog.cpp \
    source/patchmove_dialog.cpp \
    source/patchtriangulate_dialog.cpp \
    source/patchverify_dialog.cpp \
    source/planeverify_dialog.cpp \
    source/plotscale_dialog.cpp \
    source/pointverify_dialog.cpp \
    source/polygon_dialog.cpp \
    source/positionalong_dialog.cpp \
    source/positionoffset_dialog.cpp \
    source/printpreview_dialog.cpp \
    source/project_dialog.cpp \
    source/ptin_poly.cpp \
    source/qtdefs.cpp \
    source/RCCentralWidget.cpp \
    source/rccombolist.cpp \
    source/RCDialog.cpp \
    source/RCGraphicsItems.cpp \
    source/RCGraphicsScene.cpp \
    source/RCGraphicsView.cpp \
    source/RCOpenGL.cpp \
    source/RCPlatform.cpp \
    source/rctoolbox.cpp \
    source/RCtoolbutton.cpp \
    source/RCView.cpp \
    source/rectangle_dialog.cpp \
    source/reference_dialog.cpp \
    source/renamedrawing_dialog.cpp \
    source/resetview_dialog.cpp \
    source/resetworkplane_dialog.cpp \
    source/rotate_dialog.cpp \
    source/ruledsurface_dialog.cpp \
    source/ruledsurfaceverify_dialog.cpp \
    source/scale_dialog.cpp \
    source/selectdrawing_dialog.cpp \
    source/selectdrawingview_dialog.cpp \
    source/selecthatchpattern_dialog.cpp \
    source/selectview_dialog.cpp \
    source/selectworkplane_dialog.cpp \
    source/shade_dialog.cpp \
    source/snapmask_dialog.cpp \
    source/standard_Dbl_dialog.cpp \
    source/standard_Int_dialog.cpp \
    source/standardstring_dialog.cpp \
    source/stretch_dialog.cpp \
    source/styleset_dialog.cpp \
    source/surfaceofrev_dialog.cpp \
    source/surfaceofrevinsert_dialog.cpp \
    source/surfaceofrevverify_dialog.cpp \
    source/systemoptions_dialog.cpp \
    source/text_dialog.cpp \
    source/textinsert_dialog.cpp \
    source/textinsertcounter_dialog.cpp \
    source/textverify_dialog.cpp \
    source/textverifychange_dialog.cpp \
    source/trace_dialog.cpp \
    source/translate_dialog.cpp \
    source/trim_dialog.cpp \
    source/UnitTests.cpp \
    source/viewclip_dialog.cpp \
    source/weight_dialog.cpp \
    source/weightspinbox.cpp \
    source/zone_dialog.cpp \
    source/zoomzone_dialog.cpp \
    source/leaderverify_dialog.cpp \
    source/dimension_dialog.cpp \
    source/dimensionterminator_dialog.cpp \
    source/dimensionprefix0_dialog.cpp \
    source/dimensionprefix_dialog.cpp \
    source/dimensionvalue_dialog.cpp \
    source/dimensionsuffix0_dialog.cpp \
    source/dimensioncterminator_dialog.cpp \
    source/rcprintpreview_dialog.cpp \
    source/nccaptra.cpp \
    source/selectapplication_dialog.cpp \
    source/selectunloadapplication_dialog.cpp \
    source/toolbars_dialog.cpp \
    source/licensekey_dialog.cpp \
    source/licenseagreement_dialog.cpp \
    source/license_dialog.cpp \
    source/networksoftwarelicense_dialog.cpp \
    source/aboutrealcad_dialog.cpp \
    source/unlicense_dialog.cpp \
    source/smtp.cpp \
    source/_tools.cpp \
    source/v7license_dialog.cpp \
    source/rcdoublespinbox.cpp \
    source/rcintegerspinbox.cpp \
    source/dxfin_dialog.cpp \
    source/dxfout_dialog.cpp \
    source/keyboardshortcuts_dialog.cpp \
    source/rcglprinterwidget.cpp \
    source/rcsplashscreen_dialog.cpp \
    source/material_dialog.cpp \
    source/lightverify_dialog.cpp \
    source/sunposition_dialog.cpp \
    source/surfaceproperty_dialog.cpp \
    source/bumpmap_dialog.cpp \
    source/texture_dialog.cpp \
    source/raytrace_dialog.cpp \
    source/rconecoordinateentry.cpp \
    source/RCFigureBrowser.cpp \
    source/dwgimportoptions_dialog.cpp \
    source/rcplanesamplelabel.cpp \
    source/bitmaptransparency_dialog.cpp \
    source/rcopenglwidget.cpp \
    source/rcgraphicspreview.cpp \
    source/systempath_dialog.cpp

HEADERS  += \
    source/attributedelete_dialog.h \
    source/attributeedit_dialog.h \
    source/attributelist_dialog.h \
    source/attributenew_dialog.h \
    source/bitmap_dialog.h \
    source/calculator_dialog.h \
    source/Chamfer_Dialog.h \
    source/circleverify_dialog.h \
    source/composeddrawing_dialog.h \
    source/converttocurve_dialog.h \
    source/copyalong_dialog.h \
    source/copyarray_dialog.h \
    source/copybitmap_dialog.h \
    source/copycomposeddrawing_dialog.h \
    source/createview_dialog.h \
    source/createworkplane_dialog.h \
    source/createzone_dialog.h \
    source/curve_dialog.h \
    source/curveverify_dialog.h \
    source/definecolour_dialog.h \
    source/definepatternstyle_dialog.h \
    source/definestyle_dialog.h \
    source/deletedrawing_dialog.h \
    source/deleteview_dialog.h \
    source/deleteworkplane_dialog.h \
    source/deletezone_dialog.h \
    source/dimensiontext_dialog.h \
    source/drawingverify_dialog.h \
    source/ellipseinsert_dialog.h \
    source/ellipseverify_dialog.h \
    source/explode_dialog.h \
    source/figure_dialog.h \
    source/figuretext_dialog.h \
    source/figureverify_dialog.h \
    source/findandreplace_dialog.h \
    source/findinfiles_dialog.h \
    source/GeneratePoints_Dialog.h \
    source/GraphicsRBItem.h \
    source/grid_dialog.h \
    source/group_dialog.h \
    source/groupverify_dialog.h \
    source/ijl.h \
    source/information_dialog.h \
    source/insertfiguretext_dialog.h \
    source/layergroup_dialog.h \
    source/layerspinbox.h \
    source/leader_dialog.h \
    source/line_dialog.h \
    source/LineAngle_Dialog.h \
    source/lineparallel_dialog.h \
    source/lineverify_dialog.h \
    source/lttextinsert_dialog.h \
    source/mainwindow.h \
    source/maskchange_dialog.h \
    source/measure_dialog.h \
    source/measureangle_dialog.h \
    source/measurearea_dialog.h \
    source/measurelengtharea_dialog.h \
    source/mirror_dialog.h \
    source/missing_dialog.h \
    source/modifyparametricfigure_dialog.h \
    source/move_dialog.h \
    source/ncbmask.h \
    source/Nccom.h \
    source/ncdialog.h \
    source/Ncdxf.h \
    source/ncent.h \
    source/ncfile.h \
    source/ncgrid.h \
    source/nchide.h \
    source/Ncinters.h \
    source/nclist.h \
    source/ncmatrix.h \
    source/ncpoint.h \
    source/ncray.h \
    source/ncshade.h \
    source/ncsurface.h \
    source/ncval.h \
    source/ncview.h \
    source/ncwin.h \
    source/ncwork.h \
    source/offset_dialog.h \
    source/offsetd_dialog.h \
    source/oneviewchange_dialog.h \
    source/patch_dialog.h \
    source/patchcorners_dialog.h \
    source/patchmesh_dialog.h \
    source/patchmove_dialog.h \
    source/patchtriangulate_dialog.h \
    source/patchverify_dialog.h \
    source/planeverify_dialog.h \
    source/plotscale_dialog.h \
    source/pointverify_dialog.h \
    source/polygon_dialog.h \
    source/positionalong_dialog.h \
    source/positionoffset_dialog.h \
    source/printpreview_dialog.h \
    source/project_dialog.h \
    source/ptin_poly.h \
    source/qtdefs.h \
    source/RCCentralwidget.h \
    source/rccombolist.h \
    source/RCDialog.h \
    source/RCGraphicsItems.h \
    source/RCGraphicsScene.h \
    source/RCGraphicsView.h \
    source/RCImageResources.h \
    source/RCOpenGL.h \
    source/RCPlatform.h \
    source/RCStringResources.h \
    source/rctoolbox.h \
    source/RCtoolbutton.h \
    source/RCView.h \
    source/rectangle_dialog.h \
    source/reference_dialog.h \
    source/renamedrawing_dialog.h \
    source/resetview_dialog.h \
    source/resetworkplane_dialog.h \
    source/resource.h \
    source/resource1.h \
    source/rotate_dialog.h \
    source/ruledsurface_dialog.h \
    source/ruledsurfaceverify_dialog.h \
    source/scale_dialog.h \
    source/selectdrawing_dialog.h \
    source/selectdrawingview_dialog.h \
    source/selecthatchpattern_dialog.h \
    source/selectview_dialog.h \
    source/selectworkplane_dialog.h \
    source/shade_dialog.h \
    source/snapmask_dialog.h \
    source/standard_Dbl_dialog.h \
    source/standard_int_dialog.h \
    source/standardstring_dialog.h \
    source/stretch_dialog.h \
    source/Styleset_Dialog.h \
    source/surfaceofrev_dialog.h \
    source/surfaceofrevinsert_dialog.h \
    source/surfaceofrevverify_dialog.h \
    source/systemoptions_dialog.h \
    source/text_dialog.h \
    source/textinsert_dialog.h \
    source/textinsertcounter_dialog.h \
    source/textverify_dialog.h \
    source/TextVerifyChange_Dialog.h \
    source/trace_dialog.h \
    source/translate_dialog.h \
    source/trim_dialog.h \
    source/ui_mainwindow.hxx \
    source/UnitTests.h \
    source/viewclip_dialog.h \
    source/weight_dialog.h \
    source/weightspinbox.h \
    source/zone_dialog.h \
    source/zoomzone_dialog.h \
    source/leaderverify_dialog.h \
    source/dimension_dialog.h \
    source/dimensionterminator_dialog.h \
    source/dimensionprefix0_dialog.h \
    source/dimensionprefix_dialog.h \
    source/dimensionvalue_dialog.h \
    source/dimensionsuffix0_dialog.h \
    source/dimensioncterminator_dialog.h \
    source/rcprintpreview_dialog.h \
    source/RCStyleSheets.h \
    source/NCAPI.H \
    source/selectapplication_dialog.h \
    source/selectunloadapplication_dialog.h \
    source/toolbars_dialog.h \
    source/licensekey_dialog.h \
    source/licenseagreement_dialog.h \
    source/license_dialog.h \
    source/networksoftwarelicense_dialog.h \
    source/aboutrealcad_dialog.h \
    source/unlicense_dialog.h \
    source/smtp.h \
    source/_tools.h \
    source/v7license_dialog.h \
    source/rcdoublespinbox.h \
    source/rcintegerspinbox.h \
    source/dxfin_dialog.h \
    source/dxfout_dialog.h \
    source/MacNativeCode.h \
    source/keyboardshortcuts_dialog.h \
    source/rcglprinterwidget.h \
    source/rcsplashscreen_dialog.h \
    source/material_dialog.h \
    source/lightverify_dialog.h \
    source/sunposition_dialog.h \
    source/surfaceproperty_dialog.h \
    source/bumpmap_dialog.h \
    source/texture_dialog.h \
    source/raytrace_dialog.h \
    source/rconecoordinateentry.h \
    source/RCFigureBrowser.h \
    source/dwgimportoptions_dialog.h \
    source/rcplanesamplelabel.h \
    source/bitmaptransparency_dialog.h \
    source/rcopenglwidget.h \
    source/rcgraphicspreview.h \
    source/systempath_dialog.h

OTHER_FILES +=

FORMS += \
    dialogs/rccombolist.ui \
    dialogs/GENERATEPOINTS_DIALOG.ui \
    dialogs/LINEANGLE_DIALOG.ui \
    dialogs/CHAMFER_DIALOG.ui \
    dialogs/RECTANGLE_DIALOG.ui \
    dialogs/POLYGON_DIALOG.ui \
    dialogs/LINEPARALLEL_DIALOG.ui \
    dialogs/PRINTPREVIEW_DIALOG.ui \
    dialogs/WEIGHT_DIALOG.ui \
    dialogs/STYLESET_DIALOG.ui \
    dialogs/LAYER_DIALOG.ui \
    dialogs/DEFINESTYLE_DIALOG.ui \
    dialogs/DEFINECOLOUR_DIALOG.ui \
    dialogs/MASKCHANGE_DIALOG.ui \
    dialogs/LAYERGROUP_DIALOG.ui \
    dialogs/CALCULATOR_DIALOG.ui \
    dialogs/GRID_DIALOG.ui \
    dialogs/SELECTWORKPLANE_DIALOG.ui \
    dialogs/CREATEWORKPLANE_DIALOG.ui \
    dialogs/DELETEWORKPLANE_DIALOG.ui \
    dialogs/RESETWORKPLANE_DIALOG.ui \
    dialogs/ELLIPSEINSERT_DIALOG.ui \
    dialogs/CURVE_DIALOG.ui \
    dialogs/STANDARD_DBL_DIALOG.ui \
    dialogs/STANDARD_INT_DIALOG.ui \
    dialogs/STANDARDSTRING_DIALOG.ui \
    dialogs/CONVERTTOCURVE_DIALOG.ui \
    dialogs/TEXTINSERT_DIALOG.ui \
    dialogs/TEXTINSERTCOUNTER_DIALOG.ui \
    dialogs/DIMENSIONTEXT_DIALOG.ui \
    dialogs/TEXTVERIFY_DIALOG.ui \
    dialogs/TEXT_DIALOG.ui \
    dialogs/LTTEXTINSERT_DIALOG.ui \
    dialogs/TEXTVERIFYCHANGE_DIALOG.ui \
    dialogs/FIGURETEXT_DIALOG.ui \
    dialogs/BITMAP_DIALOG.ui \
    dialogs/SYSTEMOPTIONS_DIALOG.ui \
    dialogs/TRACE_DIALOG.ui \
    dialogs/SNAPMASK_DIALOG.ui \
    dialogs/POSITIONALONG_DIALOG.ui \
    dialogs/POSITIONOFFSET_DIALOG.ui \
    dialogs/REFERENCE_DIALOG.ui \
    dialogs/TRIM_DIALOG.ui \
    dialogs/EXPLODE_DIALOG.ui \
    dialogs/SELECTVIEW_DIALOG.ui \
    dialogs/RESETVIEW_DIALOG.ui \
    dialogs/PLOTSCALE_DIALOG.ui \
    dialogs/VIEWCLIP_DIALOG.ui \
    dialogs/CREATEVIEW_DIALOG.ui \
    dialogs/DELETEVIEW_DIALOG.ui \
    dialogs/ZONE_DIALOG.ui \
    dialogs/DELETEZONE_DIALOG.ui \
    dialogs/CREATEZONE_DIALOG.ui \
    dialogs/ZOOMZONE_DIALOG.ui \
    dialogs/MEASUREANGLE_DIALOG.ui \
    dialogs/MEASUREAREA_DIALOG.ui \
    dialogs/MEASURELENGTHAREA_DIALOG.ui \
    dialogs/GROUP_DIALOG.ui \
    dialogs/GROUPVERIFY_DIALOG.ui \
    dialogs/LINE_DIALOG.ui \
    dialogs/LINEVERIFY_DIALOG.ui \
    dialogs/STRETCH_DIALOG.ui \
    dialogs/TRANSLATE_DIALOG.ui \
    dialogs/MOVE_DIALOG.ui \
    dialogs/COPYARRAY_DIALOG.ui \
    dialogs/COPYALONG_DIALOG.ui \
    dialogs/PROJECT_DIALOG.ui \
    dialogs/SCALE_DIALOG.ui \
    dialogs/MIRROR_DIALOG.ui \
    dialogs/ROTATE_DIALOG.ui \
    dialogs/OFFSET_DIALOG.ui \
    dialogs/OFFSETD_DIALOG.ui \
    dialogs/CIRCLEVERIFY_DIALOG.ui \
    dialogs/POINTVERIFY_DIALOG.ui \
    dialogs/COPYBITMAP_DIALOG.ui \
    dialogs/ELLIPSEVERIFY_DIALOG.ui \
    dialogs/CURVEVERIFY_DIALOG.ui \
    dialogs/PLANEVERIFY_DIALOG.ui \
    dialogs/ATTRIBUTEDELETE_DIALOG.ui \
    dialogs/ATTRIBUTEEDIT_DIALOG.ui \
    dialogs/ATTRIBUTELIST_DIALOG.ui \
    dialogs/ATTRIBUTENEW_DIALOG.ui \
    dialogs/INFORMATION_DIALOG.ui \
    dialogs/SURFACEOFREVINSERT_DIALOG.ui \
    dialogs/PATCHCORNERS_DIALOG.ui \
    dialogs/PATCHMESH_DIALOG.ui \
    dialogs/PATCHMOVE_DIALOG.ui \
    dialogs/SURFACEOFREVVERIFY_DIALOG.ui \
    dialogs/PATCHVERIFY_DIALOG.ui \
    dialogs/RULEDSURFACEVERIFY_DIALOG.ui \
    dialogs/PATCHTRIANGULATE_DIALOG.ui \
    dialogs/SHADE_DIALOG.ui \
    dialogs/RULEDSURFACE_DIALOG.ui \
    dialogs/PATCH_DIALOG.ui \
    dialogs/SURFACEOFREV_DIALOG.ui \
    dialogs/FIGUREVERIFY_DIALOG.ui \
    dialogs/MODIFYPARAMETRICFIGURE_DIALOG.ui \
    dialogs/MISSING_DIALOG.ui \
    dialogs/FINDANDREPLACE_DIALOG.ui \
    dialogs/FINDINFILES_DIALOG.ui \
    dialogs/INSERTFIGURETEXT_DIALOG.ui \
    dialogs/SELECTHATCHPATTERN_DIALOG.ui \
    dialogs/SELECTDRAWING_DIALOG.ui \
    dialogs/COMPOSEDDRAWING_DIALOG.ui \
    dialogs/SELECTDRAWINGVIEW_DIALOG.ui \
    dialogs/ONEVIEWCHANGE_DIALOG.ui \
    dialogs/DELETEDRAWING_DIALOG.ui \
    dialogs/RENAMEDRAWING_DIALOG.ui \
    dialogs/DRAWINGVERIFY_DIALOG.ui \
    dialogs/COPYCOMPOSEDDRAWING_DIALOG.ui \
    dialogs/MEASURE_DIALOG.ui \
    dialogs/DIMENSION_DIALOG.ui \
    dialogs/SURFACEPROPERTY_DIALOG.ui \
    dialogs/DIMENSIONPREFIX_DIALOG.ui \
    dialogs/DIMENSIONCTERMINATOR_DIALOG.ui \
    dialogs/DIMENSIONPREFIX0_DIALOG.ui \
    dialogs/DIMENSIONSUFFIX0_DIALOG.ui \
    dialogs/DIMENSIONTERMINATOR_DIALOG.ui \
    dialogs/DIMENSIONVALUE_DIALOG.ui \
    dialogs/DIMENSIONVERIFY_DIALOG.ui \
    dialogs/DIMENSIONVERIFYPREFIX_DIALOG.ui \
    dialogs/DIMENSIONVERIFYTERMINATOR_DIALOG.ui \
    dialogs/DIMENSIONVERIFYVALUE_DIALOG.ui \
    dialogs/LEADER_DIALOG.ui \
    dialogs/LEADERVERIFY_DIALOG.ui \
    dialogs/DEFINEPATTERNSTYLE_DIALOG.ui \
    dialogs/SELECTAPPLICATION_DIALOG.ui \
    dialogs/SELECTUNLOADAPPLICATION_DIALOG.ui \
    dialogs/TOOLBARS_DIALOG.ui \
    dialogs/LICENSEKEY_DIALOG.ui \
    dialogs/NETWORKSOFTWARELICENSE_DIALOG.ui \
    dialogs/LICENSE_DIALOG.ui \
    dialogs/LICENSEAGREEMENT_DIALOG.ui \
    dialogs/aboutrealcad_dialog.ui \
    dialogs/UNLICENSE_DIALOG.ui \
    dialogs/v7license_dialog.ui \
    dialogs/DXFIN_DIALOG.ui \
    dialogs/DXFOUT_DIALOG.ui \
    dialogs/KEYBOARDSHORTCUTS_DIALOG.ui \
    dialogs/rcsplashscreen_dialog.ui \
    dialogs/MATERIAL_DIALOG.ui \
    dialogs/LIGHTVERIFY_DIALOG.ui \
    dialogs/SUNPOSITION_DIALOG.ui \
    dialogs/BUMPMAP_DIALOG.ui \
    dialogs/TEXTURE_DIALOG.ui \
    dialogs/RAYTRACE_DIALOG.ui \
    dialogs/rconecoordinateentry.ui \
    dialogs/RCFigureBrowser.ui \
    dialogs/dwgimportoptions_dialog.ui \
    dialogs/bitmaptransparency_dialog.ui \
    dialogs/TEXTVERIFY_DIALOG_v7.ui \
    dialogs/FIGURE_DIALOG_v7.ui \
    dialogs/BITMAP_DIALOG_v7.ui \
    dialogs/DIMENSION_DIALOG_v7.ui \
    dialogs/SYSTEMOPTIONS_DIALOG_v7.ui \
    dialogs/FINDANDREPLACE_DIALOG_v7.ui \
    dialogs/PRINTPREVIEW_DIALOG_v7.ui \
    dialogs/SELECTDRAWINGVIEW_DIALOG_v7.ui \
    dialogs/FIGURE_DIALOG_v8x.ui \
    dialogs/FIGURE_DIALOG.ui \
    dialogs/systempath_dialog.ui

OBJECTIVE_SOURCES += \
    source/MacNativeCode.mm
