#include <iostream>
#include <cmath>
#include "rtl-sdr.h"
#include "receiver/receiverfactory.h"
#include "demodulation/demod.h"
#include "dsp/filter.h"
#include "dsp/resample.h"
#include "base/writeWAV.h"
#include "base/complex.h"
#include "base/moveFreq.h"

uint32_t getNumArg( char* exArg );                          // эта функция преобразует С-строку в число, так как входные аргументы передаются в виде массива строк
void processing( uint32_t Fc, uint32_t Fw, uint32_t time ); // эта функция делает всю работу


int main( int argc, char** argv ) {
	// инициализируем входные аргументы
	uint32_t Fc = 0;
	uint32_t Fw = 0;
	uint32_t time = 0;
	// в зависимости от количества входных аргументов мы передаём переменным эти значения
	switch( argc ) {
	case 4: time = getNumArg( argv[ 3 ] );
		__attribute__( ( fallthrough ) );
	case 3: Fw = getNumArg( argv[ 2 ] );
		__attribute__( ( fallthrough ) );
	case 2: Fc = getNumArg( argv[ 1 ] );
		__attribute__( ( fallthrough ) );
	case 1: break;
	}
	// тут обрабатываем сигнал
	processing( Fc, Fw, time );
}

uint32_t getNumArg( char* exArg ) {
	// если указатель на массив ненулевой то мы проходимся по нему выясняя длину, а потом с соответствующем преобразованием из char домнажаем на степень десятки и суммируем
	uint32_t param = 0;
	if( exArg != nullptr ) {
		uint16_t numSize = 0;
		for( numSize = 0; exArg[ numSize ] != 0; numSize++ ) ;
		for( uint16_t i = 0; i < numSize; i++ ) {
			param += ( exArg[ i ] - 48 ) * pow( 10, numSize - 1 - i );
		}
	}
	return param;
}

void processing( uint32_t Fc, uint32_t Fw, uint32_t time ) {
	// инициализируем всё - приёмник, частоту дискретизации, находим из размера пакетов и входного времени количество пакетов
    ReceiverFactory::ReceiverParams params{ ReceiverFactory::ReceiverParams::ReceiverType::hw, 1024 };
    auto recImpl = ReceiverFactory::create( params );
	uint32_t Fs = ( uint32_t )2.4e6;
	uint64_t fullSampleCount = Fs * time;
	uint32_t packetSize = ( uint32_t )1024 * 1024 * 4;// !!!!!!!!!размер пакета должен быть степенью двойки
	uint64_t cycleCount = fullSampleCount / packetSize;
	// задаём настройки приёмника
	RfSettings RfSet = { Fc, Fs, 496, 0, 1 };
    // ReceiverSettings RecSet = { RfSet, packetSize, 0 }; //наследование не позволяет использовать такой конструктор
    ReceiverSettings RecSet_1; // задал иным способом
    RecSet_1.rfSettings = RfSet;
    RecSet_1.sampleCount = packetSize;
    RecSet_1.n_read = 0;
    ReceiverSettings* RecSet = &RecSet_1; // с использованием указателя для передачи в обновленную getComplex
	// создаём объекты обработки
	Filter filt;
	Resample resam;
	Demodulation demod;
	Base::WriteWAV writeSig;
    // Base::MoveFreq movFreq;
	// тут мы инициализируем буфферы для хранения пакетов и их обработки, и вектор где хранится итоговая дорожка
	std::vector< Complex< uint8_t > >  packet( packetSize );
	std::vector< Base::Complex< float > >  packetFloat( packetSize );
	uint32_t resamplePacketSize1 = floor( packetSize / ( Fs / ( 3.6 * Fw ) ) );
	std::cout << "sizeint: " << resamplePacketSize1  << std::endl;
	std::cout << "sizefloat: " <<  packetSize / ( Fs / ( 3.6 * Fw ) ) << std::endl;
	std::vector< Base::Complex< float > > resampledPacket( resamplePacketSize1 );
	std::vector< float > demodulatedPacket( resamplePacketSize1 );
	uint32_t resamplePacketSize2 = floor( resamplePacketSize1 / ( 3.6 * Fw / 48000 ) );
	std::vector< float > packetAfterSecondResample( resamplePacketSize2 );
	std::vector< float > Signal( resamplePacketSize2 * cycleCount );

	std::cout << 48000 * time - Signal.size() << std::endl;
	// тут открывается тестовый файл
    // std::ifstream file( "/home/maksim/work/w_qt/Demodulation/Dest/signalFM_fd_500000.iqf", std::fstream::binary ); ////////////////
	// основной цикл работы
	for( uint32_t i = 0; i < cycleCount; i++ ) {
		std::cerr << "iter: " << i << std::endl;
        recImpl->getComplex( RecSet, packet );// получаем пакет данных
		uint32_t N = packet.size();// выясняем его размер, можно удалить и замень в соответствующих местах
		std::cout << "packetSize: " << N << std::endl;
		// передвигаем данные из массива uint8 в массив float
		for( uint32_t j = 0; j < N; j++ ) {
            packetFloat[ j ].re = ( float )packet[ j ].real();
            packetFloat[ j ].im = ( float )packet[ j ].imag();
		}

		// Fs = 500000; ////////////////тестовые данные

///загрузка тестовых данных, с тестовыми всё работает

// for( uint32_t j = 0; j < packetSize; j++ ) {
// file.read( ( char* )( &packetFloat[ j ].re() ), sizeof( float ) );
// file.read( ( char* )( &packetFloat[ j ].im() ), sizeof( float ) );
// }

///

        // movFreq.ToZero( packetFloat.data(), packetFloat.data(), N, Fs, Fc );// сдвигаем сигнал по частоте на 0
		std::cout << "Fww: " << 1.2 * Fw  << std::endl;
		filt.Filt< float >( packetFloat.data(), packetFloat.data(), N, Fs, 0, 1.2 * Fw );// фильтруем наш сигнал
		std::cout << "old: " << Fs << '\t' << "new: " << 3.6 * Fw << std::endl;
		resampledPacket = resam.resampled< float >( packetFloat, Fs, 3.6 * Fw );// ресемплируем
		// resampledPacket = resam.resampled< float >( packetFloat, Fs, 250000 );//для тестовых данных
		demodulatedPacket = demod.demodFM< float >( resampledPacket );// демодулируем ресемплированные данные
		packetAfterSecondResample = resam.resampled< float >( demodulatedPacket, 3.6 * Fw, 48000 );// ресемплируем к аудио частоте, промежуточная частота завязана на частоту фильтрации
		// packetAfterSecondResample = resam.resampled< float >( demodulatedPacket, 250000, 48000 );

		// Fs = 2.4e6; ///////////////
		// тут мы добавляем обработанный пакет к дрожке
		uint32_t Nadd = packetAfterSecondResample.size();
		// Signal.resize( Nadd );////////////
		for( uint32_t j = 0; j < Nadd; j++ ) {
			Signal[ j + i * Nadd ] = packetAfterSecondResample[ j ];
		}
	}

    // file.close(); ///////////////
	// пишем wav файл
    writeSig.writeSignal( "/home/anatoly/work/workC++/Demodulation/Dest/signal.wav", Signal );
}
