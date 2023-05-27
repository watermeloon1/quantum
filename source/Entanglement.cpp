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

    addChannel(new Channel(satellite, m_alice, m_alice_atmosphere));
    addChannel(new Channel(satellite, m_bob, m_bob_atmosphere));
}

void Entanglement::initChannels(Device &satellite1, Device &satellite2) {

    addChannel(new Channel(m_alice, satellite1, m_alice_atmosphere));
    addChannel(new Channel(satellite2, satellite1));
    addChannel(new Channel(satellite2, m_bob,  m_bob_atmosphere));
}

void Entanglement::initChannels(Device &satellite1, Device &satellite2, Device &satellite3) {
    
    addChannel(new Channel(satellite1, m_alice, m_alice_atmosphere));
    addChannel(new Channel(satellite1, satellite2));
    addChannel(new Channel(satellite3, satellite2));
    addChannel(new Channel(satellite3, m_bob, m_bob_atmosphere));
}
