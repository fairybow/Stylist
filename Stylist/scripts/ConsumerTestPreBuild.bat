:: Properties > Build Events > ...
:: From consumer test solution:
:: call "{ this script path }" "{ main repo project dir }" "$(TargetDir)" "$(ProjectDir)" "$(IntDir)"
@echo off

set MainRepoProjectDir=%1
set TestRepoTargetDir=%2
set TestRepoProjectDir=%3
set TestRepoIntDir=%4

::xcopy /y /d /i "%MainRepoProjectDir%bin\Stylist.pdb" "%TestRepoTargetDir%"
::xcopy /y /d /i "%MainRepoProjectDir%bin\Stylist.exp" "%TestRepoTargetDir%"
:: Just copy whole dll folder (from stylist to test repo) then copy from test repo to test repo...
xcopy /y /d /i "%MainRepoProjectDir%bin\%TestRepoIntDir%Stylist.dll" "%TestRepoTargetDir%"
xcopy /y /d /e /i "%MainRepoProjectDir%include\" "%TestRepoProjectDir%Stylist\include\"
xcopy /y /d /e /i "%MainRepoProjectDir%lib\" "%TestRepoProjectDir%Stylist\lib\"
