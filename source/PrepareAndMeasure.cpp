#include "../include/PrepareAndMeasure.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/Channel.hpp"

PrepareAndMeasure::PrepareAndMeasure(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel): Network(alice, alice_atmosphere, bob, bob_atmosphere, heightAboveSeaLevel) {

}

PrepareAndMeasure::~PrepareAndMeasure(){
}

double PrepareAndMeasure::getQBER() const {
    double totalTransmittance = 1.0;
    for (std::vector<Channel*>::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it){
        totalTransmittance *= (*it) -> getTransmittance();
    }

    const double numerator = m_bob.getNoise() * m_bob.getDetectorCount();
    const double denumerator = totalTransmittance * m_alice.getMeanPhotonNumberOfSignal() * m_bob.getQuantumEfficiencyOfDetector() * 2.0;
	const double quantumBitErrorRate = m_bob.getProbabilityOfPolarizationMeasurementError() + (numerator / denumerator);

    return quantumBitErrorRate;
}

std::string PrepareAndMeasure::getType() const {
    return "prepare-and-measure";
}
    
void PrepareAndMeasure::initChannels(Device &satellite) {
    
    addChannel(new Channel(m_alice, satellite, m_alice_atmosphere));
    addChannel(new Channel(satellite, m_bob, m_bob_atmosphere));
    
}

void PrepareAndMeasure::initChannels(Device &satellite1, Device &satellite2) {

    addChannel(new Channel(m_alice, satellite1, m_alice_atmosphere));
    addChannel(new Channel(satellite1, satellite2));
    addChannel(new Channel(satellite2, m_bob,  m_bob_atmosphere));
   
}
void PrepareAndMeasure::initChannels(Device &satellite1, Device &satellite2, Device &satellite3) {

    addChannel(new Channel(m_alice, satellite1, m_alice_atmosphere));
    addChannel(new Channel(satellite1, satellite2));
    addChannel(new Channel(satellite2, satellite3));
    addChannel(new Channel(satellite3, m_bob, m_bob_atmosphere));
}
