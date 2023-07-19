#ifndef GEN_SIN_H
#define GEN_SIN_H
#include <vector>
#include "../../libs/Base/include/Base/complex.h"
#include <cmath>

class Generator_sin {

public:

    template < typename Type >
    std::vector< Complex< Type > > gen_sin( float amp, uint64_t freq, uint32_t fd,  uint64_t s_len );

private:

};

template < typename Type >
std::vector< Complex< Type > > Generator_sin::gen_sin( float amp, uint64_t freq, uint32_t fd,  uint64_t s_len ) {

    if( s_len == 0 ) {
        throw std::runtime_error( "length cant be equal 0" );
    }
    if( fd == 0 ) {

        throw std::runtime_error( "Fd cant be equal 0" );
    }

    std::vector< Complex< Type > > data( s_len );
    for( uint64_t i = 0; i < s_len; i++ ) {

        data [ i ].re() = amp * cos( 2 * M_PI * freq * i / fd );
        data [ i ].im() = amp * sin( 2 * M_PI * freq * i / fd );

    }
    return data;

}






#endif // GEN_SIN_H
