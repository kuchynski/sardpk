#include "cimage.h"

CImage::CImage(QWidget *parent) : QWidget(parent)
{
    ClearImage();
 //   QObject::startTimer(500);
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
}
void CImage::ClearImage()
{
    for(int n = 0; n < 6; n ++)
        for(int i = 0; i < max_size; i ++)
            data[n][i] = QPointF(0,0);
    for(int i = 0; i < max_size; i ++)
        my_van[i] = false;
    Paint();
}
void CImage::Paint()
{
    image = image_back;
    painter.begin(&image);
    const int size_y = height();
    const int begin_y = 0;
    const int end_y = size_y - begin_y;
    const int begin_x = 0;
    const int end_x = width() - begin_x;
    const int size_x = end_x - begin_x;
    const int y_null = begin_y + size_y / 3;


            painter.drawLine(begin_x+10, y_null, end_x - 10, y_null);
            painter.setBrush(QBrush(Qt::NoBrush));
            painter.setPen(QPen(Qt::white));
            painter.drawRoundedRect(QRect(begin_x, begin_y, size_x-1, size_y-1), 30, 30);

            QColor cm = QColor(100, 200, 255, 70);
            painter.setPen(QPen(cm));
            for(int j = 0; j < size_x; j ++)
                if(my_van[j])
      //          painter.drawLine(j+begin_x, begin_y[1] + 30, j+begin_x, end_y[1] - 30);
                    painter.drawLine(j+begin_x, y_null+20, j+begin_x, y_null-20);

            QColor color_pb[6] = {QColor(255, 255, 150, 200),
                                  QColor(205, 205, 0, 200),
                                  QColor(150, 255, 150, 200),
                                  QColor(0, 250, 0, 200),
                                  QColor(0, 255, 255, 200),
                                  QColor(0, 0, 100, 200)};
            QPointF data_real[max_size];
            for(int i = 0; i < 6; i ++)
                if(data[i][0] == QPointF(1, 1))
                {
                    for(int j = 0; j < size_x; j ++)
                    {

                        qreal tmp = data[i][j+1].ry() + y_null;
                        if(tmp <= begin_y) tmp = begin_y + 2;
                        if(tmp >= end_y) tmp = end_y - 1;
                        data_real[j].setY(tmp);
                        data_real[j].setX(data[i][j].rx()+ begin_x);
                    }
                    painter.setPen(QPen(color_pb[i], 1));
                    painter.drawPolyline(data_real, size_x);
                }

    painter.setPen(QPen(Qt::white));
   // painter.drawText(QPointF(begin_x + 15, begin_y[2] + 15), QString::number(width()));
    painter.end();
    update();
}
void CImage::paintEvent(QPaintEvent *p)
{
  //  QWidget::paintEvent(p);
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image, p->rect());
    painter.end();
}
void CImage::resizeEvent(QResizeEvent *r)
{
    QWidget::resizeEvent(r);
    emit IAmReSize();
    Paint();
}
//void CImage::timerEvent(QTimerEvent *event)
//{
    //timer_state = !timer_state;
  //  Paint();
//}
//___________________MOUSE_____________________________________MOUSE
void CImage::mousePressEvent(QMouseEvent *pe)
{
  //  QWidget::mousePressEvent(pe);
    if(pe->button() == Qt::LeftButton)
        begin_mouse_x = pe->x();
    if(pe->button() == Qt::RightButton)
    {
        begin_mouse_compress_x = pe->x();
        begin_mouse_compress_y = pe->y();
    }
}
void CImage::mouseMoveEvent(QMouseEvent *pe)
{
   // QWidget::mouseMoveEvent(pe);
    Qt::MouseButtons r = pe->buttons();
    if(pe->buttons() == Qt::LeftButton)
        if(abs(pe->x() - begin_mouse_x) >= 10)
        {
            emit Drag(pe->x() - begin_mouse_x);
            begin_mouse_x = pe->x();
        }
    if(pe->buttons() == Qt::RightButton)
    {
        if(abs(pe->y() - begin_mouse_compress_y) >= 5)
        {
            emit WheelY(-(pe->y() - begin_mouse_compress_y)/5);
            begin_mouse_compress_y = pe->y();
        }
        if(abs(pe->x() - begin_mouse_compress_x) >= 5)
        {
            emit WheelX(-(pe->x() - begin_mouse_compress_x)/5);
            begin_mouse_compress_x = pe->x();
        }
    }
}
void CImage::mouseReleaseEvent(QMouseEvent *pe)
{
    QWidget::mouseReleaseEvent(pe);
    //if(pe->button() == Qt::LeftButton)
        begin_mouse_x = 0;
//    if(pe->button() == Qt::RightButton)
    {
        begin_mouse_compress_x = 0;
        begin_mouse_compress_y = 0;
    }
}
void CImage::mouseDoubleClickEvent(QMouseEvent *pe)
{
    QWidget::mouseDoubleClickEvent(pe);
    emit DoubleClick();
}
void CImage::wheelEvent(QWheelEvent *pe)
{
    QWidget::wheelEvent(pe);
    int numDegrees = pe->delta() / 8;
    int numSteps = numDegrees / 5;
    if(pe->buttons() == Qt::LeftButton)
        emit WheelX(-numDegrees/3);
    else
        emit WheelY(-numDegrees);
}
void CImage::enterEvent(QEvent *pe)
{
    QWidget::enterEvent(pe);
//    setMouseTracking(true);
}
void CImage::leaveEvent(QEvent *pe)
{
    QWidget::leaveEvent(pe);
//    QWidget::leaveEvent(pe);
  //  setMouseTracking(false);
}
void CImage::SetImageBack(QImage &ib, QRect &r1, QRect &r2)
{
    image_back = QImage(r1.width(), r1.height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(r1, ib, r2);
    painter.end();
    Paint();
}


