#include "../include/Entanglement.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/Channel.hpp"

std::string Entanglement::m_type = "entanglement-based";

Entanglement::Entanglement(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral):
    Network(alice, alice_atmosphere, bob, bob_atmosphere, heightAboveSeaLevel, deviationRangeHeight, deviationRangeLateral){

}

Entanglement::~Entanglement(){
}

double Entanglement::getQBER(){
    double quantumBitSuccessRate = 1.0;
    for(std::vector<Channel*>::const_iterator it = m_channels.begin(); it != m_channels.end(); ++it){
        quantumBitSuccessRate *= 1.0 - (*it) -> getQuantumBitErrorRate();
    }
    return 1.0 - quantumBitSuccessRate;
}

void Entanglement::simulateSingleSatelliteDefault(double precision){
    Device satellite = Device("satellite", 0.0, 0.0, m_heightAboveSeaLevel);

    std::vector<double> v_qber;
    std::vector<double> v_optical;
    std::vector<double> v_longitude;

    for (double longitude = -(MAX_LONGITUDE); longitude < MAX_LONGITUDE; longitude += precision){
        satellite.setLongitude(longitude + getRandom(m_deviationRangeLateral));
        satellite.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

        Channel *satellite_alice;
        try {
            satellite_alice = new Channel(satellite, m_alice, m_alice_atmosphere);
        } catch (const std::runtime_error &e){
            continue;
        }

        Channel *satellite_bob;
        try {
            satellite_bob = new Channel(satellite, m_bob, m_bob_atmosphere);
        } catch (const std::runtime_error &e){
            delete satellite_alice;
            continue;
        }

        addChannel(satellite_alice);
        addChannel(satellite_bob);

        v_qber.push_back(getQBER());
        v_optical.push_back(getOpticalDistance());
        v_longitude.push_back(longitude);

        // calls delete too
        deleteChannel(satellite_alice);
        deleteChannel(satellite_bob);
    }

    if (0 < v_qber.size()){
        const std::string folder = makeFolder("1D", m_heightAboveSeaLevel, m_type);
        if (folder != "ERROR"){
            dataLogger("qber", folder, v_qber);
            dataLogger("optical", folder, v_optical);
            dataLogger("longitude", folder, v_longitude);
            //etc
        }
    }
}

void Entanglement::simulateDoubleSatelliteDefault(double precision){
    Device satellite1 = Device("satellite_one", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite2 = Device("satellite_two", 0.0, 0.0, m_heightAboveSeaLevel);

    std::vector<double> v_qber;
    std::vector<double> v_optical;
    std::vector<double> v_longitude1;
    std::vector<double> v_longitude2;

    for (double longitude1 = -(MAX_LONGITUDE); longitude1 < MAX_LONGITUDE; longitude1 += precision){
        satellite1.setLongitude(longitude1 + getRandom(m_deviationRangeLateral));
        satellite1.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));
        for (double longitude2 = longitude1; longitude2 < MAX_LONGITUDE; longitude2 += precision){
            satellite2.setLongitude(longitude2 + getRandom(m_deviationRangeLateral));
            satellite2.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

            Channel *alice_satellite1;
            try {
                alice_satellite1 = new Channel(m_alice, satellite1, m_alice_atmosphere);
            } catch (const std::runtime_error &e){
                continue;
            }

            Channel *satellite2_satellite1;
            try {
                satellite2_satellite1 = new Channel(satellite2, satellite1);
            } catch (const std::runtime_error &e){
                delete alice_satellite1;
                continue;
            }

            Channel *satellite2_bob;
            try {
                satellite2_bob = new Channel(satellite2, m_bob, m_bob_atmosphere);
            } catch (const std::runtime_error &e){
                delete alice_satellite1;
                delete satellite2_satellite1;
                continue;
            }

            addChannel(alice_satellite1);
            addChannel(satellite2_satellite1);
            addChannel(satellite2_bob);

            v_qber.push_back(getQBER());
            v_optical.push_back(getOpticalDistance());
            v_longitude1.push_back(longitude1);
            v_longitude2.push_back(longitude2);

            deleteChannel(alice_satellite1);
            deleteChannel(satellite2_satellite1);
            deleteChannel(satellite2_bob);
        }
    }

    if (0 < v_qber.size()){
        const std::string folder = makeFolder("2D", m_heightAboveSeaLevel, m_type);
        if (folder != "ERROR"){
            dataLogger("qber", folder, v_qber);
            dataLogger("optical", folder, v_optical);
            dataLogger("longitude1", folder, v_longitude1);
            dataLogger("longitude2", folder, v_longitude2);
            //etc
        }
    }
}


void Entanglement::simulateTripleSatelliteDefault(double precision){
}
