#include "../include/Entanglement.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/Channel.hpp"

Entanglement::Entanglement(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel) : Network(alice, alice_atmosphere, bob, bob_atmosphere, heightAboveSeaLevel) {

}

Entanglement::~Entanglement(){
}

double Entanglement::getQBER() const {
    double quantumBitSuccessRate = 1.0;
    for(std::vector<Channel*>::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        quantumBitSuccessRate *= 1.0 - (*it) -> getQuantumBitErrorRate();
    }
    return 1.0 - quantumBitSuccessRate;
}

std::string Entanglement::getType() const {
    return "entanglement";
}
    
void Entanglement::initChannels(Device &satellite) {

    Channel* satellite_alice = new Channel(satellite, m_alice, m_alice_atmosphere);
    Channel* satellite_bob = new Channel(satellite, m_bob, m_bob_atmosphere);

    addChannel(satellite_alice);
    addChannel(satellite_bob);
}

void Entanglement::initChannels(Device &satellite1, Device &satellite2) {

    Channel* alice_satellite1 = new Channel(m_alice, satellite1, m_alice_atmosphere);
    Channel* satellite2_satellite1 = new Channel(satellite2, satellite1);
    Channel* satellite2_bob = new Channel(satellite2, m_bob,  m_bob_atmosphere);

    addChannel(alice_satellite1);
    addChannel(satellite2_satellite1);
    addChannel(satellite2_bob);
}

void Entanglement::initChannels(Device &satellite1, Device &satellite2, Device &satellite3) {
    
    Channel* satellite1_alice = new Channel(satellite1, m_alice, m_alice_atmosphere);
    Channel* satellite1_satellite2 = new Channel(satellite1, satellite2);
    Channel* satellite3_satellite2 = new Channel(satellite3, satellite1);
    Channel* satellite3_bob = new Channel(satellite3, m_bob, m_bob_atmosphere);

    addChannel(satellite1_alice);
    addChannel(satellite1_satellite2);
    addChannel(satellite3_satellite2);
    addChannel(satellite3_bob);
}
