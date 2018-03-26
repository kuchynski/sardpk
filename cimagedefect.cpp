#include "cimagedefect.h"

CImageDefect::CImageDefect(QWidget *parent) : QWidget(parent)
{
    ClearImage();
 //   QObject::startTimer(500);
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
}
void CImageDefect::ClearImage()
{
    for(int n = 0; n < 6; n ++)
    {
        for(int i = 0; i < max_size; i ++)
        {
            data1[n][i] = QPointF(0,0);
            data2[n][i] = QPointF(0,0);
        }
        porog[n] = 0;
        defect_size[n] = 0;
    }
    Paint();
}
void CImageDefect::Paint()
{
    image = image_back;
    painter.begin(&image);
    const int size_y = height() / 3;
    const int begin_y[6] = {0, size_y, 2*size_y, 0, size_y, 2*size_y};
    const int end_y[6] = {size_y, 2*size_y, 3*size_y, size_y, 2*size_y, 3*size_y};
    const int size_x = width() / 2;
    const int begin_x[6] = {0, 0, 0, size_x, size_x, size_x};
    const int end_x[6] = {size_x, size_x, size_x, 2*size_x, 2*size_x, 2*size_x};
    const int y_null[6] = {begin_y[0] + size_y/2, begin_y[1] + size_y/2, begin_y[2] + size_y/2,
                           begin_y[3] + size_y/2, begin_y[4] + size_y/2, begin_y[5] + size_y/2};

    painter.setPen(QPen(Qt::white));
    painter.setBrush(QBrush(Qt::NoBrush));

    QColor cm = QColor(100, 200, 255, 70);
    painter.setPen(QPen(cm));

    QColor color_pb[6] = {QColor(255, 255, 150, 200),
                          QColor(205, 205, 0, 200),
                          QColor(150, 255, 150, 200),
                          QColor(0, 250, 0, 200),
                          QColor(0, 255, 255, 200),
                          QColor(0, 0, 100, 200)};
    QPointF data_real1[max_size];
    QPointF data_real2[max_size];
    for(int i = 0; i < 6; i ++)
    {
        if(defect_size[i] == 9876)
            continue;
        painter.setPen(QPen((porog[i] > defect_size[i])? Qt::white : Qt::red));
        painter.drawRoundedRect(QRect(begin_x[i], begin_y[i], size_x-1, size_y-1), 10, 10);
        for(int j = 0; j < size_x; j ++)
        {
            qreal tmp2 = data2[i][j+1].ry() + y_null[i];
            if(tmp2 <= begin_y[i]) tmp2 = begin_y[i] + 2;
            if(tmp2 >= end_y[i]) tmp2 = end_y[i] - 1;
            data_real2[j].setY(tmp2);
            data_real2[j].setX(j + begin_x[i]);
        }
        painter.setPen(QPen(color_pb[i], 2));
        painter.drawPolyline(data_real2, size_x);
        painter.setPen(QPen(Qt::white));
        painter.drawText(QPointF(begin_x[i] + 5, begin_y[i] + 15), QString::fromLocal8Bit("ПБ") + QString::number(pb_number[i]+1));
        painter.drawText(QPointF(begin_x[i] + 5, begin_y[i] + 30), QString::fromLocal8Bit("порог: ") + QString::number(porog[i]));
        painter.drawText(QPointF(end_x[i] - 50, begin_y[i] + 15), QString::fromLocal8Bit("вес: ") + QString::number(tare[i]) + QString::fromLocal8Bit("т"));
        painter.setPen(QPen((porog[i] > defect_size[i])? Qt::white : Qt::red));
        painter.drawText(QPointF(begin_x[i] + 5, begin_y[i] + 45), QString::fromLocal8Bit("уровень: ") + QString::number(defect_size[i]));
    }
    painter.end();
    update();
}
void CImageDefect::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image, p->rect());
    painter.end();
}
void CImageDefect::resizeEvent(QResizeEvent *r)
{
    QWidget::resizeEvent(r);
    emit IAmReSize();
    Paint();
}
void CImageDefect::SetImageBack(QImage &ib, QRect &r1, QRect &r2)
{
    image_back = QImage(r1.width(), r1.height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(r1, ib, r2);
    painter.end();
    Paint();
}


