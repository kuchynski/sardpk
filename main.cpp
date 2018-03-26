#include <QtGui>
#include "ccommander.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
   // QSplashScreen splash(QPixmap("first.jpg"));

    //splash.show();

    QWidget widget_main;
    widget_main.setAttribute(Qt::WA_PaintOnScreen);
    QBoxLayout *layout_main = new QBoxLayout(QBoxLayout::LeftToRight);
    layout_main->setMargin(0);
    layout_main->setSpacing(0);
    widget_main.setLayout(layout_main);
    widget_main.resize(1000, 700);
    CCommander commander;
    layout_main->addWidget(&commander);

 //   QTime time;
  //  time.start();
   // while(time.elapsed() < 1000){}

//    widget_main.showFullScreen();
    widget_main.show();
    //splash.finish(&widget_main);

    return app.exec();
}
