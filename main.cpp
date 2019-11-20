#include "./mainwindow/mainwindow.h"
#include "./app/application.h"


int main(int argc, char *argv[])
{
    spr_sht::Application a(argc, argv);
    spr_sht::MainWindow w;
    w.show();
    return a.exec();
}
