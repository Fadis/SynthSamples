#include "mbed.h"
AnalogOut audio_out(p18);
DigitalIn button(p19);
class Envelope {
  bool note_stat;
  float prev;
public:
  Envelope() : note_stat( true ), prev( 0.0f ) {}
  void off() { note_stat = false; }
  float operator()( float _time ) {
    if( note_stat ) {
      if( _time < 0.2f ) prev = _time / 0.2f;
      else if( _time < 0.7f ) prev = 1.0f - ( _time - 0.2f );
      else prev = 0.5f;
      return prev;
    }
    else
      return ( prev - _time < 0.0f ) ? 0.0f : prev - _time;
  }
};
int main() {
  while(1) {
    Envelope envelope;
    float time, note_off_time;
    for( time = 0.0f; button; time += 1.0f/16000.0f ) {
      Timer used_time;
      used_time.start();
      audio_out = envelope( time ) * sinf( time * 880.0f * 3.141592f * 2.0f ) * 0.4f + 0.5f;
      used_time.stop();
      wait(1.0f/16000.0f-used_time.read());
    }
    envelope.off();
    for( note_off_time = time; !button; time += 1.0f/16000.0f ) {
      Timer used_time;
      used_time.start();
      audio_out = envelope( time - note_off_time ) * sinf( time * 880.0f * 3.141592f * 2.0f ) * 0.4f + 0.5f;
      used_time.stop();
      wait(1.0f/16000.0f-used_time.read());
    }
  }
}
