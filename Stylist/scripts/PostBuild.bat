:: Properties > Build Events > ...
:: call "$(ProjectDir)scripts\PostBuild.bat" "$(TargetDir)" "$(ProjectDir)"
@echo off

:: e.g. "C:\Dev\Stylist\x64\{Release|Debug}\Stylist.{dll|lib}"
::set PdbPath=%1Stylist.pdb
::set ExpPath=%1Stylist.exp
set DllPath=%1Stylist.dll
set LibPath=%1Stylist.lib
set ProjectDir=%2

::xcopy /y /d /i "%PdbPath%" "%ProjectDir%bin\"
::xcopy /y /d /i "%ExpPath%" "%ProjectDir%bin\"
xcopy /y /d /i "%DllPath%" "%ProjectDir%bin\"
xcopy /y /d /i "%LibPath%" "%ProjectDir%lib\"
