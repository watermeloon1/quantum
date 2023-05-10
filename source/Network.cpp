#include "../include/Network.hpp"

Network::Network(Device &alice, Device &bob, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral):
    m_alice(alice), m_bob(bob), m_heightAboveSeaLevel(heightAboveSeaLevel), ,m_deviationRangeHeight(deviationRangeHeight), m_deviationRangeLateral(deviationRangeLateral){

    }

Network::~Network(){
}

double Network::getOpticalDistance(){
    
}

double Network::getRandom(const double from, const double to){

}

bool Network::addChannel(Channel *channel){

}

bool Network::deleteChannel(Channel *channel){

}

void Network::dataLogger(const char *filename, const std::string &directory, const std::vector<double> &data){

}


std::string Network::makeFolder(const char *prefix, const double heightAboveSeaLevel, const std::string &technology){

}
