#include "jmplayer.h"

#include <QApplication>
#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JMPlayer w;
    w.show();
    return a.exec();
}
