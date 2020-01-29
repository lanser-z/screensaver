#include <QApplication>
#include <QTranslator>
#include "screensaver.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
    QTranslator translator;
    translator.load("zh_CN");
    app.installTranslator(&translator);

    ScreenSaver screenSaver(nullptr);
	screenSaver.show();

	return app.exec();
}
