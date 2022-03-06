#!/bin/bash

cd $1
cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib ./RealCAD.app/Contents/MacOS
cp -R -v ../libxl-mac-3.6.2.0/LibXL.framework ./RealCAD.app/Contents/Frameworks
cp -v ../build-EnhancedToolsQt-Desktop_Qt_5_8_0_clang_64bit-Release/libEnhancedTools.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -v ../build-FlatPatternQt-Desktop_Qt_5_8_0_clang_64bit-Release/libFlatPattern.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -v ../build-InsertSpiralQt-Desktop_Qt_5_8_0_clang_64bit-Release/libInsertSpiral.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -v ../build-ArborCADQt-Desktop_Qt_5_8_0_clang_64bit-Release/libArborCAD.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -v ../build-LANDWorksCADQt-Desktop_Qt_5_8_0_clang_64bit-Release/libLANDWorksCAD.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
#cp -v ../build-RealCADOpenDWG-Desktop_Qt_5_6_0_clang_64bit-Release/libRealCADOpenDWG.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -v ../build-RealCADTxDevQt-Desktop_Qt_5_8_0_clang_64bit-Release/ImporterQt/libImporter.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -v ../build-RealCADTxDevQt-Desktop_Qt_5_8_0_clang_64bit-Release/ExporterQt/libExporter.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -v ../build-RealCADSketchUpAll-Desktop_Qt_5_8_0_clang_64bit-Release/RealCADSketchUpQt/libRealCADSketchUp.1.0.0.dylib ./RealCAD.app/Contents/MacOS/
cp -R -v '../build-RealCADSketchUpAll-Desktop_Qt_5_8_0_clang_64bit-Release/RealCADSketchUpWarehouse/3D Warehouse.app' ./RealCAD.app/Contents/MacOS/
