#include <QtWidgets>
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    QGraphicsView w;

    QGraphicsScene *scene = new QGraphicsScene(0, 0, 300, 300, &w);
    scene -> setBackgroundBrush(Qt::blue);
    w.setScene(scene);

    w.show();
    return a.exec();
}
