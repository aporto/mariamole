@echo off
cd %~dp0
del git_version.h
set PATH=C:\Program Files (x86)\Git\bin;%PATH%
echo #ifndef _GET_GIT_VERSION_H_ > git_version.h
echo #define _GET_GIT_VERSION_H_ >> git_version.h
echo // >> git_version.h
echo #define STRINGIZE2(s) #s >> git_version.h
echo #define STRINGIZE(s) STRINGIZE2(s) >> git_version.h
echo // >> git_version.h
set /p=#define GIT_VERSION_NUMBER >> git_version.h
git describe --abbrev=1>> git_version.h
echo #define GIT_VERSION_STRING STRINGIZE(GIT_VERSION_NUMBER) >> git_version.h
echo #endif >> git_version.h

