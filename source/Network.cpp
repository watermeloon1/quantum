#include "../include/Network.hpp"
#include "../include/Channel.hpp"
#include <random>
#include <sys/stat.h>
#include <fstream>

Network::Network(Device &alice, Device &bob, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral):
    m_alice(alice), m_bob(bob), m_heightAboveSeaLevel(heightAboveSeaLevel), m_deviationRangeHeight(deviationRangeHeight), m_deviationRangeLateral(deviationRangeLateral){

    }

Network::~Network(){
}

double Network::getOpticalDistance() const {
    double totalDistance = 0.0;
    for (std::vector<Channel*>::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        totalDistance += (*it) -> getOpticalDistance();
    }
    return totalDistance;
}

double Network::getRandom(const double deviation) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(-deviation, deviation);
    return dist(gen);
}

void Network::addChannel(Channel *channel) {
    m_channels.push_back(channel);
}

void Network::deleteChannel(Channel *channel) {
    std::vector<Channel*>::iterator iter = std::find(m_channels.begin(), m_channels.end(), channel);
    if (iter != m_channels.end()) {
        m_channels.erase(iter);
        delete channel;
    } else {
        std::cout << "Channel pointer not found in vector" << std::endl;
    }
}

bool Network::dataLogger(const std::string &filename, const std::string &directory, const std::vector<double> &data) {
    const std::string filepath = "data/" + directory + "/" + filename + ".dat";

    std::string stringData;
    for (std::vector<double>::const_iterator it = data.begin(); it != data.end(); ++it) {
        stringData += std::to_string(*it) + '\n';
    }

    std::ofstream file(filepath, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return false;
    }

    file << stringData;
    file.close();

    return true;
}


std::string Network::makeFolder(const char *prefix, const double heightAboveSeaLevel, const std::string &technology){
    const char* folder = "data/";

    std::string dirName = std::string(prefix) + std::to_string(static_cast<int>(heightAboveSeaLevel)) + technology;
    std::string directory = std::string(folder) + dirName;

    int status = mkdir(directory.c_str(), 0777);
    if (status == -1) {
        std::cerr << "ERROR: could not create directory \" " << directory << " \"" << std::endl;
        return "ERROR";
    }
    return dirName;
}
