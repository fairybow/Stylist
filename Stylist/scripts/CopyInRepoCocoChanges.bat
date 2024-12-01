@echo off

call "WinGitRoot.bat"

xcopy /y /d /e /i "Stylist\submodules\Coco\include\" "C:\Dev\Coco\include\"
xcopy /y /d /e /i "Stylist\submodules\Coco\src\" "C:\Dev\Coco\src\"
