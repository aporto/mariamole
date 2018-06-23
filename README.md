mariamole
=========

MariaMole: An alternative Arduino IDE

More info at [http://alexporto.com/mariamole]

Download the last Windows release from the Release page. Get the file [https://github.com/aporto/mariamole/releases MariaMole_Installer.exe]

For Linux and Macintosh, the last compiled binaries are at [http://alexporto.com/mariamole]. Note that these binaries are compiled by other people, and may be using old code. Only the Windows version is always compiled with the last version of this code.

If you want to build your own binaries, check the instructions below

===How to build this project:===
This project requires:--
* Qt 5.4.1 (Other versions may be compatible, but this is recommended, since things may change)
* QScintilla 2.8.3
* git command line tools added to operating system path

For Windows:
* Create an environment variable called QSCI pointing to your QScintilla install. This must be the directory that contains both folders "release" (which contains qscintilla2.dll, qscintilla2.lib and obj files) and "Qsci" folder (which contains the .h header files). If you follow QScintilla build instructions, the path set to QSCI variable will probably end in "Qt4Qt5"
* Make sure git command line tools are added to Windows PATH, because the Visual Studio project will call'em to set the hardcoded build version


