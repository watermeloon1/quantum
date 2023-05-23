#include "../include/PrepareAndMeasure.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/Channel.hpp"

PrepareAndMeasure::PrepareAndMeasure(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral):
    Network(alice, alice_atmosphere, bob, bob_atmosphere, heightAboveSeaLevel, deviationRangeHeight, deviationRangeLateral){

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

    
void PrepareAndMeasure::initChannels(Device &satellite) {
    
    Channel* alice_satellite = new Channel(m_alice, satellite, m_alice_atmosphere);
    Channel* satellite_bob = new Channel(satellite, m_bob, m_bob_atmosphere);

    addChannel(alice_satellite);
    addChannel(satellite_bob);
    
}
void PrepareAndMeasure::initChannels(Device &satellite1, Device &satellite2) {
    
    Channel* alice_satellite1 = new Channel(m_alice, satellite1, m_alice_atmosphere);
    Channel* satellite1_satellite2 = new Channel(satellite1, satellite2);
    Channel* satellite2_bob = new Channel(satellite2, m_bob,  m_bob_atmosphere);

    addChannel(alice_satellite1);
    addChannel(satellite1_satellite2);
    addChannel(satellite2_bob);
   
}
void PrepareAndMeasure::initChannels(Device &satellite1, Device &satellite2, Device &satellite3) {
    
    Channel* alice_satellite1 = new Channel(m_alice, satellite1, m_alice_atmosphere);
    Channel* satellite1_satellite2 = new Channel(satellite1, satellite2);
    Channel* satellite2_satellite3 = new Channel(satellite2, satellite3);
    Channel* satellite3_bob = new Channel(satellite3, m_bob, m_bob_atmosphere);

    addChannel(alice_satellite1);
    addChannel(satellite1_satellite2);
    addChannel(satellite2_satellite3);
    addChannel(satellite3_bob);
}
