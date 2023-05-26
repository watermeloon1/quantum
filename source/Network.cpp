#include "../include/Network.hpp"
#include "../include/Channel.hpp"
#include "../include/Device.hpp"
#include <random>
#include <sys/stat.h>
#include <fstream>

Network::Network(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel): m_alice(alice), m_alice_atmosphere(alice_atmosphere), m_bob(bob), m_bob_atmosphere(bob_atmosphere), m_heightAboveSeaLevel(heightAboveSeaLevel){        
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

void Network::deleteChannels() {
    for (std::vector<Channel*>::iterator it = m_channels.begin(); it != m_channels.end(); ++it) {
        delete *it;
    }
    m_channels.clear();
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

void Network::setPosition(Device* satellite, double longitude, const double deviationRangeHeight, const double deviationRangeLateral, const double deviationRangeLongitudinal) {

        satellite -> setLatitude(0.0 + getRandom(deviationRangeLateral));
        satellite -> setLongitude(longitude + getRandom(deviationRangeLongitudinal));
        satellite -> setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(deviationRangeHeight));
}

#define MAX_LONGITUDE 180.0
void Network::simulateSingleSatellite(const double precision, const double deviationRangeHeight, const double deviationRangeLateral, const double deviationRangeLongitudinal) {
    
    std::vector<double> v_qber;
    std::vector<double> v_optical; 
    std::vector<double> v_longitude;

    Device satellite = Device("satellite", 0.0, 0.0, m_heightAboveSeaLevel);

    initChannels(satellite); // pure virtual function with heap memory allocs

    for (double longitude = -(MAX_LONGITUDE); longitude < MAX_LONGITUDE; longitude += precision) {
        setPosition(&satellite, longitude, deviationRangeHeight, deviationRangeLateral, deviationRangeLongitudinal);
        
        try {
            updateChannels();
        } catch (const std::runtime_error &e) {
            continue;
        }

        // seg fault on getQBER() call
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

void Network::simulateDoubleSatellite(const double precision, const double deviationRangeHeight, const double deviationRangeLateral, const double deviationRangeLongitudinal) {

    (void) deviationRangeLongitudinal;
    
    std::vector<double> v_qber;
    std::vector<double> v_optical; 
    std::vector<double> v_longitude1;
    std::vector<double> v_longitude2;

    Device satellite1 = Device("satellite1", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite2 = Device("satellite2", 0.0, 0.0, m_heightAboveSeaLevel);

    initChannels(satellite1, satellite2); // pure virtual function with heap memory allocs

    for (double longitude1 = -(MAX_LONGITUDE); longitude1 < MAX_LONGITUDE; longitude1 += precision) {
        setPosition(&satellite1, longitude1, deviationRangeHeight, deviationRangeLateral, deviationRangeLongitudinal);
        
        for (double longitude2 = longitude1; longitude2 < MAX_LONGITUDE; longitude2 += precision) {        
            setPosition(&satellite2, longitude2, deviationRangeHeight, deviationRangeLateral, deviationRangeLongitudinal);

            try {
                updateChannels();    
            } catch (const std::runtime_error &e) {
                continue;
            }
            
            v_qber.push_back(getQBER());
            v_optical.push_back(getOpticalDistance());
            v_longitude1.push_back(longitude1);
            v_longitude2.push_back(longitude2);
        }        
    }

    deleteChannels(); // frees the newly made channels in the pure virtual initChannels function
    
    if (0 < v_qber.size()){
        const std::string folder = makeFolder("2D", m_heightAboveSeaLevel, getType());
        if (folder != "ERROR"){
            dataLogger("qber", folder, v_qber);
            dataLogger("optical", folder, v_optical);
            dataLogger("longitude1", folder, v_longitude1);
            dataLogger("longitude2", folder, v_longitude2);
            //etc
        }
    } else {
        std::cerr << "WARNING: the network is unable to establish communication between " << m_alice.getName() <<
            " and " << m_bob.getName() << std::endl;
    }
}

void Network::simulateTripleSatellite(const double precision, const double deviationRangeHeight, const double deviationRangeLateral, const double deviationRangeLongitudinal) {

    (void) deviationRangeLongitudinal; 
    std::vector<double> v_qber;
    std::vector<double> v_optical; 
    std::vector<double> v_longitude1;
    std::vector<double> v_longitude2;
    std::vector<double> v_longitude3;

    Device satellite1 = Device("satellite1", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite2 = Device("satellite2", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite3 = Device("satellite3", 0.0, 0.0, m_heightAboveSeaLevel);

    initChannels(satellite1, satellite2, satellite3); // pure virtual function with heap memory allocs

    for (double longitude1 = -(MAX_LONGITUDE); longitude1 < MAX_LONGITUDE; longitude1 += precision) {
        setPosition(&satellite1, longitude1, deviationRangeHeight, deviationRangeLateral, deviationRangeLongitudinal);
        
        for (double longitude2 = longitude1; longitude2 < MAX_LONGITUDE; longitude2 += precision) {
            setPosition(&satellite2, longitude2, deviationRangeHeight, deviationRangeLateral, deviationRangeLongitudinal);

            for (double longitude3 = longitude2; longitude3 < MAX_LONGITUDE; longitude3 += precision) {
                setPosition(&satellite3, longitude3, deviationRangeHeight, deviationRangeLateral, deviationRangeLongitudinal);
            
                try {
                    updateChannels();    
                } catch (const std::runtime_error &e) {
                    continue;
                }
                
                v_qber.push_back(getQBER());
                v_optical.push_back(getOpticalDistance());
                v_longitude1.push_back(longitude1);
                v_longitude2.push_back(longitude2);
                v_longitude3.push_back(longitude3);
            }
        }
    }

    deleteChannels(); // frees the newly made channels in the pure virtual initChannels function
    
    if (0 < v_qber.size()){
        const std::string folder = makeFolder("3D", m_heightAboveSeaLevel, getType());
        if (folder != "ERROR"){
            dataLogger("qber", folder, v_qber);
            dataLogger("optical", folder, v_optical);
            dataLogger("longitude1", folder, v_longitude1);
            dataLogger("longitude2", folder, v_longitude2);
            dataLogger("longitude3", folder, v_longitude3);
            //etc
        }
    } else {
        std::cerr << "WARNING: the network is unable to establish communication between " << m_alice.getName() <<
            " and " << m_bob.getName() << std::endl;
    }
}

// TODO: decrease the number of simulation functions to one with template metaprogramming
