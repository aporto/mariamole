#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("mariamole");
	a.setOrganizationName("MariaMole");
	MainWindow w;
	w.showMaximized();
	return a.exec();
}
