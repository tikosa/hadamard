#include "widget.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    widget *w = new widget();
    w->show();
    return app.exec();
}
