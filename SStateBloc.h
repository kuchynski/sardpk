#ifndef SSTATEBLOC_H
#define SSTATEBLOC_H

#include "CStackFourth.h"

struct SStateBloc
{
  bool enabled;
  unsigned char state_pedal;
  unsigned connect_pb;
  unsigned kol_of_van;
  unsigned direction;
  double speed;
  SElement12<int> data_pb;
  SElementFourth<int> data_dm;

  SStateBloc(){};
  SStateBloc(const SStateBloc &sb){*this = sb;}
  bool operator==(const SStateBloc &sb)const
  {return (enabled == sb.enabled) && (state_pedal == sb.state_pedal) &&
          (connect_pb == sb.connect_pb) && (kol_of_van == sb.kol_of_van) &&
          (speed == sb.speed) && (direction == sb.direction);}
  bool operator!=(const SStateBloc &sb)const
  {return !(*this == sb);}
  SStateBloc& operator=(const SStateBloc &sb)
  { enabled = sb.enabled; state_pedal = sb.state_pedal;
    connect_pb = sb.connect_pb;
    speed = sb.speed;
    data_pb = sb.data_pb; data_dm = sb.data_dm;
    direction = sb.direction; kol_of_van = sb.kol_of_van; return *this;}
};
#endif // SSTATEBLOC_H
