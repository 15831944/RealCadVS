#!/bin/bash

# set a temp directory to be removed later
mkdir ../deployment-temp
cp -R -v ../build-RealCAD-*-Release/RealCAD.app ../deployment-temp
# Run macdeployqt (for Qt-version and compiler) on .app to deploy Qt Frameworks
../../Qt/5.14.2/clang_64/bin/macdeployqt ../deployment-temp/RealCAD.app
# Frameworks
cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework ../deployment-temp/RealCAD.app/Contents/Frameworks
#cp -R -L -v ../SDK_Mac_2020-0-362/SketchUpAPI.framework ../deployment-temp/RealCAD.app/Contents/Frameworks
cp -R -L -v ../SDK_Mac_2021-0-338/SketchUpAPI.framework ../deployment-temp/RealCAD.app/Contents/Frameworks
# Dynamic Libraries
cp -v ../build-EnhancedToolsQt-*-Release/libEnhancedTools.1.0.0.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ../build-FlatPatternQt-*-Release/libFlatPattern.1.0.0.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ../ImporterExporterProjects/build-ExporterQt-*-Release/libExporter.1.0.0.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ../ImporterExporterProjects/build-ImporterQt-*-Release/libImporter.1.0.0.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ../build-InsertSpiralQt-*-Release/libInsertSpiral.1.0.0.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ../RealCADSketchUpAll/build-RealCADSketchUpQt-*-Release/libRealCADSketchUp.1.0.0.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v '../RealCADSketchUpAll/build-RealCADSketchUpWarehouse-'*'-Release/3D Warehouse.app/Contents/MacOs/3D Warehouse' ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
# Static files
cp -v ./acad.lin ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ./acad.pat ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ./keyserver.ini ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ./txt.shx ../deployment-temp/RealCAD.app/Contents/MacOS
cp -v ./LANDWorksCAD.icns ../deployment-temp/RealCAD.app/Contents/Resources
cp -v ./ArborCAD.icns ../deployment-temp/RealCAD.app/Contents/Resources
cp -v ./CAD.icns ../deployment-temp/RealCAD.app/Contents/Resources
mkdir ../deployment-temp/RealCAD.app/Contents/Library
mkdir ../deployment-temp/RealCAD.app/Contents/Library/QuickLook
cp -R -v ./RealCADQuickLookPlugin.qlgenerator ../deployment-temp/RealCAD.app/Contents/Library/QuickLook
# Duplicate into ArborCAD/LANDWorksCAD and copy unique files
cp -R -v ../deployment-temp/RealCAD.app ../deployment-temp/LANDWorksCAD.app
cp -v ../build-LANDWorksCADQt-*-Release/libLANDWorksCAD.1.0.0.dylib ../deployment-temp/LANDWorksCAD.app/Contents/MacOS
cp -f -v ./Info_Land.plist ../deployment-temp/LANDWorksCAD.app/Contents/Info.plist
cp -R -v ../deployment-temp/RealCAD.app ../deployment-temp/ArborCAD.app
cp -v ../build-ArborCADQt-*-Release/libArborCAD.1.0.0.dylib ../deployment-temp/ArborCAD.app/Contents/MacOS
cp -f -v ./Info_Arbor.plist ../deployment-temp/ArborCAD.app/Contents/Info.plist

cp -R -f -v ../deployment-temp/* ../deployment
rm -R -v ../deployment-temp



#cd $1
#rm -R -v /Applications/RealCAD.app/Contents/Frameworks
#rm -R -v /Applications/RealCAD.app/Contents/Plugins
#cp -R -v ./RealCAD.app/Contents/Frameworks /Applications/RealCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Plugins /Applications/RealCAD.app/Contents/Plugins
#deploy libxl to apps
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib ../deployment-temp/RealCAD.app/Contents/MacOS
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework ../deployment-temp/RealCAD.app/Contents/Frameworks
#cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework /Applications/RealCAD.app/Contents/Frameworks
#cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework /Applications/ArborCAD.app/Contents/Frameworks
#cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework /Applications/LANDWorksCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework ./RealCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework /Applications/RealCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework /Applications/ArborCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework /Applications/LANDWorksCAD.app/Contents/Frameworks
#deploy SketchUp Warehouse framework
#cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' ./RealCAD.app/Contents/MacOS
#cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' /Applications/RealCAD.app/Contents/MacOS
#cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' /Applications/ArborCAD.app/Contents/MacOS
#cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy RealCAD to apps
#cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/LANDWorksCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/ArborCAD.app/Contents/MacOS
#deploy app plugins to realcad
#cp -v ./RealCAD.app/Contents/MacOS/libArborCAD.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libLANDWorksCAD.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#deploy libs to apps
#cp -v ./RealCAD.app/Contents/MacOS/libArborCAD.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libLANDWorksCAD.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy importer module
#cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy  exporter module
#cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy SketchUp read module
#cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy the SketchUp framework
#cp -R -L -v ../SDK_Mac_18-0-16976/SketchUpAPI.framework /Applications/RealCAD.app/Contents/Frameworks
#cp -R -L -v ../SDK_Mac_18-0-16976/SketchUpAPI.framework /Applications/ArborCAD.app/Contents/Frameworks
#cp -R -L -v ../SDK_Mac_18-0-16976/SketchUpAPI.framework /Applications/LANDWorks.app/Contents/Frameworks
#deploy SketchUp Warehouse
#cp -R -L -v './RealCAD.app/Contents/MacOS/3D Warehouse.app' /Applications/RealCAD.app/Contents/MacOS
#cp -R -L -v './RealCAD.app/Contents/MacOS/3D Warehouse.app' /Applications/ArborCAD.app/Contents/MacOS
#cp -R -L -v './RealCAD.app/Contents/MacOS/3D Warehouse.app' /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy enhanced tools module
#cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy flat pattern module
#cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy insrt spiral module
#cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
