#include <QtGui/QApplication>
#include "VirtualBot.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    VirtualBot foo;
    foo.show();
    return app.exec();
}
