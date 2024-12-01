@echo off

call "WinGitRoot.bat"

git submodule deinit -f "Stylist\submodules\Coco"
git rm -f "Stylist\submodules\Coco"
rmdir /s /q ".git\modules\Stylist\submodules\Coco"
