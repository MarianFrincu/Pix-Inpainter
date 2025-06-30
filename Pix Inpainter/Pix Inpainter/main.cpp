#include "PixInpainter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/icons/AppIcon"));

    PixInpainter window;
    window.show();
    return app.exec();
}
