#include <gtest/gtest.h>
#include "../include/Demodulation/resample.h"
#include "../libs/Base/include/Base/reader.h"
#include <vector>
#include <fstream>

TEST( resample_test, output_values ) {


    std::ifstream file( "dat/resamVALUE_testXX", std::ifstream::binary );
    if( !file.is_open() ) {
        throw std::runtime_error( "cant open file " );
    }

    std::vector< float > vec( 1000 );

    file.read( reinterpret_cast< char* >( vec.data() ), 1000 * sizeof( float ) );

    std::ifstream file2( "dat/resamVALUE_testOK", std::fstream::binary );
    if( !file2.is_open() ) {
        throw std::runtime_error( "cant open file " );
    }

    std::vector< float > vec2( 100 ); // example

    file2.read( reinterpret_cast< char* >( vec2.data() ), 100 * sizeof( float ) );

    uint32_t freqOLD = 500000;
    uint32_t freqNEW = 50000;

    Resample test_obj;
    std::vector< float > data = test_obj.resampled< float >( vec, freqOLD, freqNEW ); //

    for( uint64_t i = 0; i < 100; ++i ) {
        ASSERT_EQ( data[ i ], vec2[ i ] );

    }
}

TEST( resample_test, output_Cmplx_values ) {


    std::ifstream file( "dat/resamComVALUE_testXX", std::ifstream::binary );
    if( !file.is_open() ) {
        throw std::runtime_error( "cant open file " );
    }

    std::vector< Complex< float > > vec3( 1000 );

    for( uint32_t i = 0; i < 1000; i++ ) {
        file.read( reinterpret_cast< char* >( &( vec3[ i ].re() ) ), sizeof( float ) );
        file.read( reinterpret_cast< char* >( &( vec3[ i ].im() ) ), sizeof( float ) );
    }
    file.close();


    std::ifstream file2( "dat/resamComVALUE_testOK", std::fstream::binary );
    if( !file2.is_open() ) {
        throw std::runtime_error( "cant open file " );
    }

    std::vector< Complex< float > > vec4( 100 ); // example

    for( uint32_t i = 0; i < 100; i++ ) {
        file2.read( reinterpret_cast< char* >( &( vec4[ i ].re() ) ), sizeof( float ) );
        file2.read( reinterpret_cast< char* >( &( vec4[ i ].im() ) ), sizeof( float ) );
    }
    file2.close();

    uint32_t freqOLD = 500000;
    uint32_t freqNEW = 50000;

    Resample test_obj;
    std::vector< Complex< float > > data = test_obj.resampled( vec3, freqOLD, freqNEW ); //
    // freqOLD = 1;
    for( uint64_t i = 0; i < 100; ++i ) {
        ASSERT_EQ( data[ i ].re(), vec4[ i ].re() );
        ASSERT_EQ( data[ i ].im(), vec4[ i ].im() );

    }
}

TEST( resample_test, correct_fd_real ) {

    std::ifstream file1( "dat/resamVALUE_testOK", std::ifstream::binary );
    std::vector< float > vec1( 100 );

    file1.read( reinterpret_cast< char* >( vec1.data() ), 100 * sizeof( float ) );

    file1.close();
    Resample test_obj1;
    uint32_t freqOLD = 10;
    uint32_t freqNEW = 5000;

    try {

        std::vector< float > data1 = test_obj1.resampled< float >( vec1, freqOLD, freqNEW );



    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "wrong new Fd" ) << "something wrong\n";


    }


}
TEST( resample_test, correct_fd_cmplx ) {

    std::ifstream file2( "dat/resamComVALUE_testOK", std::fstream::binary );
    std::vector< Complex< float > > vec2( 100 ); // example

    for( uint32_t i = 0; i < 100; i++ ) {
        file2.read( reinterpret_cast< char* >( &( vec2[ i ].re() ) ), sizeof( float ) );
        file2.read( reinterpret_cast< char* >( &( vec2[ i ].im() ) ), sizeof( float ) );
    }
    file2.close();
    Resample test_obj2;

    uint32_t freqOLD = 5;
    uint32_t freqNEW = 100;

    try {

        std::vector< Complex< float > > data = test_obj2.resampled( vec2, freqOLD, freqNEW );


    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "wrong new Fd" ) << "something wrong\n";

    }


}
TEST( resample_test, fdOLD_null_re ) {

    std::ifstream file1( "dat/resamVALUE_testOK", std::ifstream::binary );
    std::vector< float > vec1( 100 );

    file1.read( reinterpret_cast< char* >( vec1.data() ), 100 * sizeof( float ) );

    file1.close();
    Resample test_obj1;
    uint32_t freqOLD = 0;
    uint32_t freqNEW = 5000;

    try {

        std::vector< float > data1 = test_obj1.resampled< float >( vec1, freqOLD, freqNEW );



    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "Fd cant be equal 0" ) << "something wrong\n";


    }


}
TEST( resample_test, fdNEW_null_re ) {

    std::ifstream file1( "dat/resamVALUE_testOK", std::ifstream::binary );
    std::vector< float > vec1( 100 );

    file1.read( reinterpret_cast< char* >( vec1.data() ), 100 * sizeof( float ) );

    file1.close();
    Resample test_obj1;
    uint32_t freqOLD = 100;
    uint32_t freqNEW = 0;

    try {

        std::vector< float > data1 = test_obj1.resampled< float >( vec1, freqOLD, freqNEW );



    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "Fd cant be equal 0" ) << "something wrong\n";


    }


}
TEST( resample_test, fdOLD_cmplx ) {

    std::ifstream file2( "dat/resamComVALUE_testOK", std::fstream::binary );
    std::vector< Complex< float > > vec2( 100 ); // example

    for( uint32_t i = 0; i < 100; i++ ) {
        file2.read( reinterpret_cast< char* >( &( vec2[ i ].re() ) ), sizeof( float ) );
        file2.read( reinterpret_cast< char* >( &( vec2[ i ].im() ) ), sizeof( float ) );
    }
    file2.close();
    Resample test_obj2;

    uint32_t freqOLD = 0;
    uint32_t freqNEW = 100;

    try {

        std::vector< Complex< float > > data = test_obj2.resampled( vec2, freqOLD, freqNEW );


    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "Fd cant be equal 0" ) << "something wrong\n";

    }


}
TEST( resample_test, fdNEW_cmplx ) {

    std::ifstream file2( "dat/resamComVALUE_testOK", std::fstream::binary );
    std::vector< Complex< float > > vec2( 100 ); // example

    for( uint32_t i = 0; i < 100; i++ ) {
        file2.read( reinterpret_cast< char* >( &( vec2[ i ].re() ) ), sizeof( float ) );
        file2.read( reinterpret_cast< char* >( &( vec2[ i ].im() ) ), sizeof( float ) );
    }
    file2.close();
    Resample test_obj2;

    uint32_t freqOLD = 110;
    uint32_t freqNEW = 0;

    try {

        std::vector< Complex< float > > data = test_obj2.resampled( vec2, freqOLD, freqNEW );


    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "Fd cant be equal 0" ) << "something wrong\n";

    }


}
TEST( resample_test, null_size_re ) {

    std::vector< float > vec( 0 );
    ;

    Resample test_obj;

    try {

        std::vector< float > data = test_obj.resampled< float >( vec, 100, 5 );



    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "size cant be equal 0" ) << "something wrong\n";

    }


}
TEST( resample_test, short_data ) {

    std::vector< float > vec( 5 );
    // vec[ 0 ] = 10.0;

    Resample test_obj;

    try {

        std::vector< float > data = test_obj.resampled< float >( vec, 100, 10 );
        std::cerr << data.size();


    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "data length is too short" ) << "something wrong\n";

    }


}
TEST( resample_test, null_size_cmplx ) {

    std::vector< Complex< float > > vec( 0 );

    Resample test_obj;

    try {

        std::vector< Complex< float > > data = test_obj.resampled( vec, 100, 50 );


    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "size cant be equal 0" ) << "something wrong\n";

    }

}

TEST( resample_test, short_cmplx_data ) {

    std::vector< Complex< float > > vec( 5 );
    // vec[ 0 ] = 10.0;

    Resample test_obj;

    try {

        std::vector< Complex< float > > data = test_obj.resampled( vec, 100, 10 );
        std::cerr << data.size();


    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "data length is too short" ) << "something wrong\n";

    }


}
TEST( resample_test, no_resam_re ) {

    std::vector< float > vec( 20 );

    Resample test_obj;

    try {

        std::vector< float > data = test_obj.resampled< float >( vec, 100, 90 );



    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "no resample with these frequencies" ) << "something wrong\n";

    }


}
TEST( resample_test, no_resam_cmplx ) {

    std::vector< Complex< float > > vec( 20 );

    Resample test_obj;

    try {

        std::vector<  Complex< float > > data = test_obj.resampled< float >( vec, 100, 90 );



    } catch( std::runtime_error& e ) {

        ASSERT_STREQ( e.what(), "no resample with these frequencies" ) << "something wrong\n";

    }


}
