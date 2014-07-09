#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("mariamole");
	a.setOrganizationName("dalpix");
	MainWindow * w = new MainWindow();
	w->showMaximized();
	int res = a.exec();
	#ifdef Q_OS_WIN
		delete w; // hack to avoid segfault when closing app on Linux. Still need to check this on docs.
	#endif
	return res;
}
