#include "widget.h"

#include <QtWidgets/QApplication>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::unique_ptr<widget> w (new widget());
    w->show();
    return app.exec();
}
