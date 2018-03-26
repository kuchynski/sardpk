#include "cmonitor.h"

CMonitor::CMonitor(QWidget *parent) : QWidget(parent), image(width_image, hieght_image),//, QImage::Format_ARGB32_Premultiplied),
    paint_all(true), paint_state(true)
{
    ClearData();
    begin_y[0] = 0;
    begin_y[1] = 100;
    end_y[0] = begin_y[1]-1;
    end_y[1] = hieght_image;
    size_y[0] = end_y[0] - begin_y[0];
    size_y[1] = end_y[1] - begin_y[1];
    center_y[0] = (end_y[0] + begin_y[0]) / 2;
    center_y[1] = (end_y[1] + begin_y[1]) / 2;
    begin_x = 0;
    end_x = width_image;
    size_x = end_x - begin_x;
    center_x = (end_x + begin_x) / 2;
}
void CMonitor::SetComPort(CComPort *cp)
{
    com_port = cp;
    timer_id = QObject::startTimer(param.GetMonitorCompressX());
}

void CMonitor::Paint()
{
    if(!visiable)
        return;
    update();
}
void CMonitor::paintEvent(QPaintEvent *p)
{
    if(!visiable)
        return;

    // QWidget::paintEvent(p);
    QColor color_datchik[14] = {QColor(255, 255, 150, 200),
                                QColor(205, 205, 0, 200),
                                QColor(150, 255, 150, 200),
                                QColor(0, 250, 0, 200),
                                QColor(0, 255, 255, 200),
                                QColor(0, 0, 100, 200),
                                QColor(255, 255, 150, 200),
                                QColor(205, 205, 0, 200),
                                QColor(150, 255, 150, 200),
                                QColor(0, 250, 0, 200),
                                QColor(0, 255, 255, 200),
                                QColor(0, 0, 100, 200),
                                QColor(255, 0, 0, 190),
                                QColor(255, 0, 0, 190)};
     const unsigned number_datchik = param.GetMonitorValue();
     const int compressy = param.GetMonitorCompressY();

     painter.begin(&image);

     painter.setRenderHint(QPainter::Antialiasing, true);
    // if(paint_all)
     {
         painter.setBrush(QBrush(Qt::black));
         painter.drawRect(QRect(begin_x, begin_y[1], size_x, size_y[1]));

         //if(number_datchik)
        // {
             for(int i = 0; i < size_x; i ++)
             {
                 if(number_datchik <= 11)
                 {
                     int y = center_y[1] + data_pb[i][number_datchik]/compressy;
                     if(y > end_y[1])
                         y = end_y[1];
                     if(y < begin_y[1])
                         y = begin_y[1];
                     data[i].setY(y);
                 }
                 else if(number_datchik <= 13)
                     data[i].setY(center_y[1] - data_dm[i][number_datchik-12]);
             }
             painter.setPen(QPen(color_datchik[number_datchik]));
             painter.drawPolyline(data, size_x);
        // }
     }
 /*    else
     {
         //if(number_datchik)
        // {
             painter.drawPixmap(QRect(begin_x, begin_y[1], end_x - 2, end_y[1]),
                               image, QRect(begin_x+1, begin_y[1], end_x - 2, end_y[1]));
//             painter.drawImage(QRect(begin_x, begin_y[1], end_x - 2, end_y[1]),
  //                             image, QRect(begin_x+1, begin_y[1], end_x - 2, end_y[1]));
             painter.setPen(QPen(Qt::black));
             painter.drawLine(end_x - 2, begin_y[1], end_x - 2, end_y[1] - 1);
             painter.setPen(QPen(color_datchik[number_datchik]));
             int y1, y2;
             if(number_datchik <= 11)
             {
                 y1 = center_y[1] - data_pb[width_image-2][number_datchik]/compressy;
                 y2 = center_y[1] - data_pb[width_image-1][number_datchik]/compressy;
             }
             else if(number_datchik <= 13)
             {
                 y1 = center_y[1] - data_dm[width_image-2][number_datchik-12];
                 y2 = center_y[1] - data_dm[width_image-1][number_datchik-12];
             }
             if(y1 > end_y[1])
                 y1 = end_y[1];
             if(y1 < begin_y[1])
                 y1 = begin_y[1];
             if(y2 > end_y[1])
                 y2 = end_y[1];
             if(y2 < begin_y[1])
                 y2 = begin_y[1];
             painter.drawLine(end_x - 3, y1, end_x - 2, y2);
        // }
     }*/
     painter.setPen(QPen(Qt::white));
     const int coord_dat_x[14] = {-160, -120, -80, -40, 0, 40, -160, -120, -80, -40, 0, 40, -200, 80};
     const int coord_dat_y[14] = {-25, -25, -25, -25, -25, -25, 25, 25, 25, 25, 25, 25, 25, 25};
   //  if(paint_all)
     {
         QLinearGradient gradient(begin_x, begin_y[0], center_x, end_y[0]);
         gradient.setColorAt(0, Qt::black);
         gradient.setColorAt(0.25, Qt::black);
         gradient.setColorAt(0.5, Qt::darkGray);
         gradient.setColorAt(0.75, Qt::black);
         gradient.setColorAt(1, Qt::black);
         painter.setBrush(gradient);
         painter.drawRect(begin_x, begin_y[0], end_x, end_y[0]);
         painter.drawText(QPointF(begin_x + 50, center_y[0] + 5), QString::fromLocal8Bit("—¬ﬂ«‹"));
         for(int i = 0; i < 12; i++)
             painter.drawText(QPointF(center_x + coord_dat_x[i], center_y[0] + coord_dat_y[i]),
                              QString::fromLocal8Bit("œ¡") + QString::number(i+1));
         painter.drawText(QPointF(center_x + coord_dat_x[12], center_y[0] + coord_dat_y[12]),
                          QString::fromLocal8Bit("ƒÃ1"));
         painter.drawText(QPointF(center_x + coord_dat_x[13], center_y[0] + coord_dat_y[13]),
                          QString::fromLocal8Bit("ƒÃ2"));
     }
    // if(paint_state || paint_all)//—Œ—“ﬂÕ»≈
     {
         painter.setPen(QPen(Qt::darkGreen));
         painter.setBrush(QBrush((state_bloc.enabled)? Qt::green : Qt::black));
         painter.drawEllipse(QRect(begin_x + 25, center_y[0]-7, 15, 15));
         unsigned maska = 0x01;
         for(int i = 0; i < 12; i++, maska = maska << 1)
         {
             painter.setBrush(QBrush((state_bloc.connect_pb & maska)? Qt::green : Qt::black));
             painter.drawEllipse(QRect(center_x + coord_dat_x[i]+3, center_y[0] + coord_dat_y[i]+4, 10, 10));
         }
         painter.setPen(QPen(Qt::darkRed));
         painter.setBrush(QBrush((state_bloc.state_pedal & 0x01)? Qt::red : Qt::black));
         painter.drawEllipse(QRect(center_x + coord_dat_x[12]+3, center_y[0] + coord_dat_y[12]+4, 10, 10));
         painter.setBrush(QBrush((state_bloc.state_pedal & 0x02)? Qt::red : Qt::black));
         painter.drawEllipse(QRect(center_x + coord_dat_x[13]+3, center_y[0] + coord_dat_y[13]+4, 10, 10));

         painter.setPen((state_bloc.direction)? QPen(Qt::white) : QPen(Qt::black));
         painter.setBrush(QBrush(Qt::black));
         painter.drawRect(QRect(center_x + 125, center_y[0]-30, 153, 63));
         QFont font("Courier New", 15, 20);
         font.setBold(true);
         painter.setFont(font);
         painter.setPen((state_bloc.direction == 2)? QPen(Qt::white) : QPen(Qt::black));
         painter.drawText(QPointF(center_x + 130, center_y[0]-10), QString::fromLocal8Bit("<"));
         painter.setPen((state_bloc.direction == 1)? QPen(Qt::white) : QPen(Qt::black));
         painter.drawText(QPointF(center_x + 260, center_y[0]-10), QString::fromLocal8Bit(">"));
         painter.setPen((state_bloc.speed)? QPen(Qt::green) : QPen(Qt::black));
         painter.drawText(QPointF(center_x + 160, center_y[0]-10), QString::number((int)((double)state_bloc.speed * 3.6)) + QString::fromLocal8Bit(" ÍÏ/˜   "));
         painter.setPen((state_bloc.kol_of_van)? QPen(Qt::green) : QPen(Qt::black));
         painter.drawText(QPointF(center_x + 140, center_y[0]+25), QString::number(state_bloc.kol_of_van) + QString::fromLocal8Bit(" ‚‡„ÓÌÓ‚     "));
     }

 //painter.setBrush(QBrush(Qt::black));
 //painter.drawRect(QRect(center_x + 50, begin_y[0], end_x, size_y[0]));
 //painter.setPen(QPen(Qt::red));
 //QFont font("Courier New", 80, 60);
 //font.setBold(true);
 //painter.setFont(font);
 //painter.drawText(QPointF(center_x + 50, center_y[0]+30), QString::number(tmp_ves));
    painter.end();

    painter.begin(this);
    painter.drawPixmap(0, 0, image);
//    painter.drawImage(0, 0, image);
    painter.end();
    paint_all = false;
}

void CMonitor::SetVisible(bool v)
{
    visiable = v;
    //paint_all = visiable;
  //  if(visiable)
//        Paint();
}

void CMonitor::resizeEvent(QResizeEvent *r)
{
    paint_all = true;
    QWidget::resizeEvent(r);
}

void CMonitor::ClearData()
{
    for(int i = 0; i < width_image; i ++)
    {
        data_pb[i] = 0;
        data_dm[i] = 0;
    }
    ClearImage();
}
void CMonitor::ClearImage()
{
    paint_all = true;
    for(int i = 0; i < width_image; i ++)
    {
        data[i].setX(i);
        data[i].setY(-1);
    }
    Paint();
}
void CMonitor::NewParam()
{
    paint_all = true;
    QObject::killTimer(timer_id);
   // if(param.GetMonitorValue())
        timer_id = QObject::startTimer(5+param.GetMonitorCompressX());
 //   else
   // {
      //  timer_id = QObject::startTimer(100);
    //    ClearImage();
   // }
}
void CMonitor::timerEvent(QTimerEvent *event)
{
    for(int i = 1; i < width_image; i ++)
    {
        data_pb[i-1] = data_pb[i];
        data_dm[i-1] = data_dm[i];
    }
    SStateBloc sb;
    com_port->GetData(sb);
    paint_state = (state_bloc != sb);
    state_bloc = sb;
    data_pb[width_image-1] = state_bloc.data_pb;
    data_dm[width_image-1] = state_bloc.data_dm;
//double koof = 0.3;
//SElementFourth<int> pb_null = param.GetPbNull();
//tmp_ves = (int)((double)(abs(state_bloc.pb_null[2] - pb_null[2])) * koof);
    Paint();
}
void CMonitor::mouseDoubleClickEvent(QMouseEvent *pe)
{
    if(pe->y() >= begin_y[1])
        emit ClickGraph();
}
void CMonitor::wheelEvent(QWheelEvent *pe)
{
    int numDegrees = pe->delta() / 8;
    int numSteps = numDegrees / 5;
    if(pe->buttons() == Qt::LeftButton)
        emit WheelX(-numSteps / 3);
    else
        emit WheelY(-numSteps / 3);
}
