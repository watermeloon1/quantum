#include "../include/Network.hpp"
#include "../include/Channel.hpp"
#include "../include/Device.hpp"
#include <random>
#include <sys/stat.h>
#include <fstream>

Network::Network(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral):
    m_alice(alice), m_alice_atmosphere(alice_atmosphere), m_bob(bob), m_bob_atmosphere(bob_atmosphere), m_heightAboveSeaLevel(heightAboveSeaLevel), m_deviationRangeHeight(deviationRangeHeight), m_deviationRangeLateral(deviationRangeLateral){        
    }

Network::~Network(){
}

std::string Network::getType() const {
    return m_type;
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

void Network::deleteChannels() {
    for (std::vector<Channel*>::iterator it = m_channels.end(); it !=  m_channels.begin(); --it) {
        m_channels.erase(it);
        delete (*it);
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

// may throw a runtime_error, handled in simulation
void Network::updateChannels() {
     for (std::vector<Channel*>::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
         (*it) -> update();
     }
}

#define MAX_LONGITUDE 180.0
void Network::simulateSingleSatellite(double precision) {

    std::vector<double> v_qber;
    std::vector<double> v_optical; 
    std::vector<double> v_longitude;

    Device satellite = Device("satellite", 0.0, 0.0, m_heightAboveSeaLevel);
    initChannels(satellite); // pure virtual function with heap memory allocs

    for (double longitude = -(MAX_LONGITUDE); longitude < MAX_LONGITUDE; longitude += precision) {
        satellite.setLatitude(getRandom(m_deviationRangeLateral));
        satellite.setLongitude(longitude);
        satellite.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

        try {
            updateChannels();    
        } catch (const std::runtime_error &e) {
            continue;
        }
        
        v_qber.push_back(getQBER());
        v_optical.push_back(getOpticalDistance());
        v_longitude.push_back(longitude);
    }

    deleteChannels(); // frees the newly made channels in the pure virtual initChannels function
    
    if (0 < v_qber.size()){
        const std::string folder = makeFolder("1D", m_heightAboveSeaLevel, getType());
        if (folder != "ERROR"){
            dataLogger("qber", folder, v_qber);
            dataLogger("optical", folder, v_optical);
            dataLogger("longitude", folder, v_longitude);
            //etc
        }
    } else {
        std::cerr << "WARNING: the network is unable to establish communication between " << m_alice.getName() <<
            " and " << m_bob.getName() << std::endl;
    }
}
