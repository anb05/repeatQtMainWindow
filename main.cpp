#include "./mainwindow/mainwindow.h"
#include "./app/application.h"


int main(int argc, char *argv[])
{
    spr_sht::Application app(argc, argv);
    spr_sht::MainWindow* win = new spr_sht::MainWindow;

    win->show();
    int err = app.exec();

    delete win;

    return err;
}
