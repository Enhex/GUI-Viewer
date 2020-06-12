set source="%~dp0\"

set build_folder=viewer-build

cd ..
mkdir %build_folder%
cd %build_folder%

REM conan install %source% --build=outdated
conan install %source% --build=outdated -s build_type=Debug

cd %source%
premake5 vs2019 --location=../%build_folder%/

REM need to run as admin:
mklink /D ..\%build_folder%\gui %~dp0\gui