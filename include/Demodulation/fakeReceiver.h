#ifndef FAKERECEIVER_H
#define FAKERECEIVER_H

#include <cstdint>
#include "gen_sin.h"
#include "genNoise.h"

struct noiseParams {
	float W;
};

struct sinParams {
	float amp;
	uint64_t freq;
};

struct signalParams {
	uint32_t fd;
	uint64_t size;
};

class FakeReceiver {
public:
	noiseParams m_noiseP;
	std::vector< sinParams > m_sinesP;
	signalParams m_signalP;

	FakeReceiver( noiseParams NP, std::vector< sinParams > SnP, signalParams SgP );
	template < typename Type >
	std::vector< Base::Complex< Type > > GenSignal();
private:
};

FakeReceiver::FakeReceiver( noiseParams NP, std::vector< sinParams > SnP, signalParams SgP ) : m_noiseP( NP ), m_sinesP( SnP ), m_signalP( SgP ) {
}

template < typename Type >
std::vector< Base::Complex< Type > > FakeReceiver::GenSignal() {
	GenNoise WNGen;
	Generator_sin SnGen;
	std::vector< Base::Complex< Type > > data = WNGen.GenWN< Type >( m_noiseP.W, m_signalP.size );
	for( uint32_t i = 0; i < m_sinesP.size(); i++ ) {
		std::vector< Base::Complex< Type > > dataSin = SnGen.gen_sin< Type >( m_sinesP[ i ].amp, m_sinesP[ i ].freq, m_signalP.fd,  m_signalP.size );
		for( uint64_t j = 0; j < m_signalP.size; j++ ) {
			data[ j ] += dataSin[ j ];
		}
	}
	return data;
}
#endif