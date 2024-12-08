@echo off

call "WinGitRoot.bat"

rmdir /s /q "C:\Dev\Coco\include\"
rmdir /s /q "C:\Dev\Coco\src\"
rmdir /s /q "C:\Dev\Coco\old\"

xcopy /y /e /i "Stylist\submodules\Coco\include\" "C:\Dev\Coco\include\"
xcopy /y /e /i "Stylist\submodules\Coco\src\" "C:\Dev\Coco\src\"
xcopy /y /e /i "Stylist\submodules\Coco\old\" "C:\Dev\Coco\old\"
