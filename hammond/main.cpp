#include "mbed.h"
#include "fixed_math.hpp"

AnalogOut audio_out(p18);
DigitalIn button(p19);
class Envelope {
  bool note_stat;
  fixed32< 16 > prev;
public:
  Envelope() : note_stat( true ), prev( 0.0f ) {}
  void off() { note_stat = false; }
  fixed32< 16 > operator()( fixed32< 16 > _time ) {
    if( note_stat ) {
      if( _time < 0.2f ) prev = _time / 0.2f;
      else if( _time < 0.7f ) prev = 1.0f - ( _time - 0.2f );
      else prev = 0.5f;
      return prev;
    }
    else
      return ( prev - _time < 0.0f ) ? static_cast< fixed32< 16 > >( 0.0f ) : prev - _time;
  }
};

struct HT {
  static const float level_16 = 0.5f;
  static const float level_8 = 1.0f;
  static const float level_513 = 0.5f;
  static const float level_4 = 0.3f;
  static const float level_223 = 1.0f;
  static const float level_2 = 0.5f;
  static const float level_135 = 0.8f;
  static const float level_113 = 0.1f;
  static const float level_1 = 0.2f;
};

template< typename Traits >
class Hammond {
public:
  Hammond(){}
  fixed32< 16 > operator()( fixed32< 16 > _time ) {
    static const fixed32< 16 > scale_16 = 220.0f;
    static const fixed32< 16 > scale_8 = 440.0f;
    static const fixed32< 16 > scale_513 = 659.3f;
    static const fixed32< 16 > scale_4 = 880.0f;
    static const fixed32< 16 > scale_223 = 1318.5f;
    static const fixed32< 16 > scale_2 = 1760.0f;
    static const fixed32< 16 > scale_135 = 2217.0f;
    static const fixed32< 16 > scale_113 = 2637.0f;
    static const fixed32< 16 > scale_1 = 3520.0f;
    fixed32< 16 > sum = 0.0f;
    sum += sint( _time * scale_16 ) * Traits::level_16;
    sum += sint( _time * scale_8 ) * Traits::level_8;
    sum += sint( _time * scale_513 ) * Traits::level_513;
    sum += sint( _time * scale_4 ) * Traits::level_4;
    sum += sint( _time * scale_223 ) * Traits::level_223;
    sum += sint( _time * scale_2 ) * Traits::level_2;
    sum += sint( _time * scale_135 ) * Traits::level_135;
    sum += sint( _time * scale_113 ) * Traits::level_113;
    sum += sint( _time * scale_1) * Traits::level_1;
    fixed32< 16 > max = 0.0f;
    max += Traits::level_16;
    max += Traits::level_8;
    max += Traits::level_513;
    max += Traits::level_4;
    max += Traits::level_223;
    max += Traits::level_2;
    max += Traits::level_135;
    max += Traits::level_113;
    max += Traits::level_1;
    sum /= max;
    return sum;
  }
private:
};

int main() {
  while(1) {
    fixed32< 16 > time;
    Hammond< HT > hammond;
    for( time = 0.0f; button; time += 1.0f/16000.0f ) {
      Timer used_time;
      used_time.start();
      audio_out = hammond( time ) * 0.4f + 0.5f;
      used_time.stop();
      wait(static_cast<float>(1.0f/16000.0f-used_time.read()));
    }
    for( ; !button; ) {
    }
  }
}
