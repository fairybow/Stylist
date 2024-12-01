:: Properties > Build Events > ...
:: From consumer test solution:
:: call "{ this script path }" "{ main repo project dir }" "$(TargetDir)" "$(ProjectDir)"
@echo off

set MainRepoProjectDir=%1
set TestRepoTargetDir=%2
set TestRepoProjectDir=%3

::xcopy /y /d /i "%MainRepoProjectDir%bin\Stylist.pdb" "%TestRepoTargetDir%"
::xcopy /y /d /i "%MainRepoProjectDir%bin\Stylist.exp" "%TestRepoTargetDir%"
xcopy /y /d /i "%MainRepoProjectDir%bin\Stylist.dll" "%TestRepoTargetDir%"
xcopy /y /d /e /i "%MainRepoProjectDir%include\" "%TestRepoProjectDir%Stylist\include\"
xcopy /y /d /e /i "%MainRepoProjectDir%lib\" "%TestRepoProjectDir%Stylist\lib\"
