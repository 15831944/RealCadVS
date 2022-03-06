#!/bin/bash

#/Users/Anderson/Qt/5.14.1/clang_64/bin/macdeployqt ./RealCAD.app

cd $1
rm -R -v /Applications/RealCAD.app/Contents/Frameworks
rm -R -v /Applications/RealCAD.app/Contents/Plugins
cp -R -v ./RealCAD.app/Contents/Frameworks /Applications/RealCAD.app/Contents/Frameworks
cp -R -v ./RealCAD.app/Contents/Plugins /Applications/RealCAD.app/Contents/Plugins
#deploy libxl to apps
cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib ./RealCAD.app/Contents/MacOS
cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework ./RealCAD.app/Contents/Frameworks
cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework /Applications/RealCAD.app/Contents/Frameworks
cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework /Applications/ArborCAD.app/Contents/Frameworks
cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework /Applications/LANDWorksCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework ./RealCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework /Applications/RealCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework /Applications/ArborCAD.app/Contents/Frameworks
#cp -R -v ./RealCAD.app/Contents/Frameworks/LibXL.framework /Applications/LANDWorksCAD.app/Contents/Frameworks
#deploy SketchUp Warehouse framework
cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' ./RealCAD.app/Contents/MacOS
cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' /Applications/RealCAD.app/Contents/MacOS
cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' /Applications/ArborCAD.app/Contents/MacOS
cp -v '../build-RealCADSketchUpAll-Desktop_Qt_5_12_1_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app/Contents/MacOs/3D Warehouse' /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy RealCAD to apps
cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/LANDWorksCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/ArborCAD.app/Contents/MacOS
#deploy app plugins to realcad
cp -v ./RealCAD.app/Contents/MacOS/libArborCAD.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libLANDWorksCAD.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#deploy libs to apps
cp -v ./RealCAD.app/Contents/MacOS/libArborCAD.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libLANDWorksCAD.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy importer module
cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy  exporter module
cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy SketchUp read module
cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy the SketchUp framework
cp -R -L -v ../SDK_Mac_18-0-16976/SketchUpAPI.framework /Applications/RealCAD.app/Contents/Frameworks
cp -R -L -v ../SDK_Mac_18-0-16976/SketchUpAPI.framework /Applications/ArborCAD.app/Contents/Frameworks
cp -R -L -v ../SDK_Mac_18-0-16976/SketchUpAPI.framework /Applications/LANDWorks.app/Contents/Frameworks
#deploy SketchUp Warehouse
#cp -R -L -v './RealCAD.app/Contents/MacOS/3D Warehouse.app' /Applications/RealCAD.app/Contents/MacOS
#cp -R -L -v './RealCAD.app/Contents/MacOS/3D Warehouse.app' /Applications/ArborCAD.app/Contents/MacOS
#cp -R -L -v './RealCAD.app/Contents/MacOS/3D Warehouse.app' /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy enhanced tools module
cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy flat pattern module
cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy insrt spiral module
cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
