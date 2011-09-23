#ifndef TINYFM_FIXED_MATH_HPP
#define TINYFM_FIXED_MATH_HPP

#include "fixed.hpp"
#include "sin_table.hpp"

template< unsigned int _pos >
inline fixed32<_pos> sint( fixed32<_pos> _x ) {
  fixed32<_pos> temp;
  temp.set( sin_table[ static_cast<int>( _x << 10 ) & 0x3FF ] << ( _pos - 16 ) );
  return temp;
}

template< unsigned int _pos >
inline fixed32<_pos> cost( fixed32<_pos> _x ) {
  fixed32<_pos> temp;
  temp.set( sin_table[ static_cast<int>( ( _x << 10 ) + 512 ) & 0x3FF ] << ( _pos - 16 ) );
  return temp;
}

template<unsigned int _pos>
inline fixed32<_pos> triangle( fixed32<_pos> _x ) {
  fixed32<_pos> quad = _x * 4;
  if( quad < 1 )
    return quad;
  else if( quad < 3 )
    return 2 - quad;
  else
    return -4 + quad;
}

template<unsigned int _pos>
inline fixed32<_pos> fuzz( fixed32<_pos> _x ) {
  if( _x < -2 )
    return -1;
  else if( _x < 0 ) {
    fixed32<_pos> temp;
    const unsigned int elem =  -_x << 8;
    temp.set( exp2_table[ elem ] << ( _pos - 15 ) );
    temp = -temp;
    return temp;
  }
  else if( _x < 2 ) {
    fixed32<_pos> temp;
    const unsigned int elem =  _x << 8;
    temp.set( exp2_table[ elem ] << ( _pos - 15 ) );
    return temp;
  }
  else
    return 1;
}

#endif
