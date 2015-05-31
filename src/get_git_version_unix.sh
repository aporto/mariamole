rm -rf git_version.h
echo "#ifndef _GET_GIT_VERSION_H_" 
echo "#define _GET_GIT_VERSION_H_"
echo "//"
echo "#define STRINGIZE2(s) #s"
echo "#define STRINGIZE(s) STRINGIZE2(s)"
echo "//"
#set /p=#define GIT_VERSION_NUMBER
echo "#define GIT_VERSION_NUMBER" `git describe --tags --abbrev=1`
echo "#define GIT_VERSION_STRING STRINGIZE(GIT_VERSION_NUMBER)"
echo "#endif"

