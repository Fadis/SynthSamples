#include "mbed.h"
#include "fixed_math.hpp"

AnalogOut audio_out(p18);
DigitalIn button(p19);

class Envelope {
  bool note_stat;
  fixed32< 16 > prev;
  fixed32< 16 > off_time;
public:
  Envelope() : note_stat( true ), prev( 0.0f ) {}
  void off( fixed32< 16 > _off_time ) { note_stat = false; off_time = _off_time; }
  fixed32< 16 > operator()( fixed32< 16 > _time ) {
    if( note_stat ) {
      if( _time < 0.05f ) prev = _time / 0.05f;
      else if( _time < 0.85f ) prev = 1.0f - ( _time - 0.05f );
      else prev = 0.2f;
      return prev;
    }
    else {
      _time -= off_time;
      return ( prev - _time < 0.0f ) ? static_cast< fixed32< 16 > >( 0.0f ) : prev - _time;
    }
  }
};

class Const {
public:
  fixed32< 16 > operator()( fixed32< 16 > _time ) {
    static const fixed32< 16 > value = 0;
    return value;
  }
};

template< typename Source >
class FM {
public:
  FM() {}
  void off( fixed32< 16 > _off_time ) { envelope.off( _off_time ); }
  fixed32< 16 > operator()( fixed32< 16 > _time ) {
    fixed32< 16 > looped_time = _time - static_cast< int >( _time );
    return sint( ( looped_time * 880.0f + source( _time ) / 2 ) ) * envelope( _time );
  }
private:
  Source source;
  Envelope envelope;
};

int main() {
  while(1) {
    FM< FM< Const > > fm;
    fixed32< 16 > time, note_off_time;
    for( time = 0.0f; button; time += 1.0f/16000.0f ) {
      Timer used_time;
      used_time.start();
      audio_out = fm( time ) * 0.4f + 0.5f;
      used_time.stop();
      wait(1.0f/16000.0f-used_time.read());
    }
    fm.off( time );
    for( ; !button; time += 1.0f/16000.0f ) {
      Timer used_time;
      used_time.start();
      audio_out = fm( time ) * 0.4f + 0.5f;
      used_time.stop();
      wait(1.0f/16000.0f-used_time.read());
    }
  }
}

