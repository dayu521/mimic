#include "widget.h"
#include"register_type.h"
#include<QTranslator>
#include<QtDebug>

#include <QApplication>
#include<QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator tr;
    if(tr.load(QLocale(),QLatin1String("fuck"),QLatin1String("_")))
        a.installTranslator(&tr);
    Util::obj.registerCreateFunction();
    Util::obj.createAll();
//    //or
//    Util::obj.createObj<Util::objFd<Rbtree>>();
//    Util::obj.createObj<DemoSim>();
//    Util::obj.createObj<2>();

//    //those errors happened in compile-time
//    Util::obj.createObj<3>();
//    Util::obj.createObj<4>();
    Widget w;
    w.addMapping(
                {
                     {
                         {Util::objFd<Rbtree>},Util::name[Util::objFd<Rbtree>]
                     }/*,
                     {
                         {Util::objFd<DemoSim>},QStringLiteral("例子")
                     },
                     {
                         {Util::objFd<QuickSortSimulation>},QStringLiteral("快速排序")
                     }*/
                }
    );
    w.init();
    w.show();
    return a.exec();
}
