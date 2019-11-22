#include "./mainwindow/mainwindow.h"
#include "./app/application.h"
#include <QSplashScreen>
#include <QPixmap>
#include <thread>

static void loadModule();

int main(int argc, char *argv[])
{
    spr_sht::Application app(argc, argv);
	QSplashScreen* splash = new QSplashScreen;
	const QPixmap px(":/images/spl_scr");
	splash->setPixmap(px);
	splash->show();

	splash->showMessage(QObject::tr("Setting up the main window..."),
						Qt::AlignRight | Qt::AlignTop,
						Qt::blue);

	spr_sht::MainWindow* win = new spr_sht::MainWindow;

	loadModule();

    win->show();

	splash->finish(win);
	delete splash;
    int err = app.exec();

    return err;
}

static void loadModule()
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
}
