& .\DeployRealcadQt.bat

Write-Output "Done deployqt command."
Write-Output "Copying libraries and executables..."

Copy-Item ..\build-RealCAD-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\RealCAD.exe "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\build-RealCAD-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\ui_*.h "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\build-EnhancedToolsQt-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\EnhancedTools.dll "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\build-FlatPatternQt-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\FlatPattern.dll "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\build-InsertSpiralQt-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\InsertSpiral.dll "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\RealCADSketchUpAll\build-RealCADSketchUpQt-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\RealCADSketchUp.dll "..\deployment\RealCAD-v8.3" -Force
Copy-Item "..\RealCADSketchUpAll\build-RealCADSketchUpWarehouse-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\3D Warehouse.exe" "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\ImporterExporterProjects\build-ExporterQt-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\Exporter.dll "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\ImporterExporterProjects\build-ImporterQt-Desktop_Qt_5_14_2_MSVC2017_64bit-Release\release\Importer.dll "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\SDK_WIN_x64_2021-0-339\binaries\sketchup\x64\* "..\deployment\RealCAD-v8.3" -Force
Copy-Item ..\libxl-3.6.2.0\bin64\libxl.dll "..\deployment\RealCAD-v8.3" -Force

Write-Output "Done copying libraries and executables."