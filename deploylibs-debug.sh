#!/bin/bash

cd $1
# uncomment the items you want to deploy for debugging
#deploy libxl to apps
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib ./RealCAD.app/Contents/MacOS
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ../libxl-mac-3.6.2.0/lib/libxl.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy RealCAD to apps
#cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/LANDWorksCAD.app/Contents/MacOS
cp -v ./RealCAD.app/Contents/MacOS/RealCAD /Applications/ArborCAD.app/Contents/MacOS
#deploy app plugins to realcad
#cp -v ./RealCAD.app/Contents/MacOS/libArborCAD.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libLANDWorksCAD.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#deploy libs to apps
#cp -v ./RealCAD.app/Contents/MacOS/libArborCAD.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libLANDWorksCAD.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy importer plugin
#cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libImporter.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy  exporter plugin
#cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libExporter.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy SketchUp read module
#cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libRealCADSketchUp.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy enhanced tools module
#cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libEnhancedTools.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy flat pattern module
#cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libFlatPattern.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
#deploy insert spiral module
#cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/RealCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/ArborCAD.app/Contents/MacOS
#cp -v ./RealCAD.app/Contents/MacOS/libInsertSpiral.1.0.0.dylib /Applications/LANDWorksCAD.app/Contents/MacOS
