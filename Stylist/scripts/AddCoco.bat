@echo off

call "WinGitRoot.bat"

git submodule add https://github.com/fairybow/Coco Stylist\submodules\Coco
git submodule update --init --recursive
