#include "mbed.h"
AnalogOut audio_out(p18);
const float freq_table[ 8 ] = {
  523.25113f, 587.32953f, 659.25511f, 698.45646f,
  783.99087f, 880.00000f, 987.76660f, 1046.5022f,
};
int main() {
  while(1) {
    for( int note = 0; note != 8; ++note )
      for( float time = 0.0f; time < 1.0f; time += 1.0f/16000.0f ) {
        Timer used_time;
        used_time.start();
        audio_out =
          sinf( time * freq_table[ note ] * 3.141592f * 2.0f ) *
          0.5f + 0.5f;
        used_time.stop();
        wait(1.0f/16000.0f-used_time.read());
    }
  }
}