@echo off
echo Setting up DAGGer
echo -----------------------------------
cd DAGGer\scripts
call Windows-Setup.bat
echo -----------------------------------
pushd %~dp0
call DAGGer\vendor\premake\bin\premake5.exe vs2019
popd
pause