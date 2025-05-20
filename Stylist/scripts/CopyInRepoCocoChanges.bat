@echo off

call "WinGitRoot.bat"

rmdir /s /q "C:\Dev\Coco\Coco\include\"
rmdir /s /q "C:\Dev\Coco\Coco\src\"
rmdir /s /q "C:\Dev\Coco\Coco\old\"

xcopy /y /e /i "Stylist\submodules\Coco\Coco\include\" "C:\Dev\Coco\Coco\include\"
xcopy /y /e /i "Stylist\submodules\Coco\Coco\src\" "C:\Dev\Coco\Coco\src\"
xcopy /y /e /i "Stylist\submodules\Coco\Coco\old\" "C:\Dev\Coco\Coco\old\"
