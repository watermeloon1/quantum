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
        satellite.setLatitude(getRandom(m_deviationRangeLateral));
        satellite.setLongitude(longitude);
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
    } else {
        std::cerr << "WARNING: the network is unable to establish communication between " << m_alice.getName() <<
            " and " << m_bob.getName() << std::endl;
    }
}

void Entanglement::simulateDoubleSatelliteDefault(double precision){
    Device satellite1 = Device("satellite_one", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite2 = Device("satellite_two", 0.0, 0.0, m_heightAboveSeaLevel);

    std::vector<double> v_qber;
    std::vector<double> v_optical;
    std::vector<double> v_longitude1;
    std::vector<double> v_longitude2;

    std::vector<double> temp;
    double count = 0;

    for (double longitude1 = -(MAX_LONGITUDE); longitude1 < MAX_LONGITUDE; longitude1 += precision){
        satellite1.setLatitude(getRandom(m_deviationRangeLateral));
        satellite1.setLongitude(longitude1);
        satellite1.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

        for (double longitude2 = longitude1; longitude2 < MAX_LONGITUDE; longitude2 += precision){
            satellite2.setLatitude(getRandom(m_deviationRangeLateral));
            satellite2.setLongitude(longitude2);
            satellite2.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

            temp.push_back(count);
            count += 1.0;

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

            temp.pop_back();

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
            dataLogger("temp", folder, temp);
            //etc
        }
    } else {
        std::cerr << "WARNING: the network is unable to establish communication between " << m_alice.getName() <<
            " and " << m_bob.getName() << std::endl;
    }
}


void Entanglement::simulateTripleSatelliteDefault(double precision){
    Device satellite1 = Device("satellite_one", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite2 = Device("satellite_two", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite3 = Device("satellite_three", 0.0, 0.0, m_heightAboveSeaLevel);

    std::vector<double> v_qber;
    std::vector<double> v_optical;
    std::vector<double> v_longitude1;
    std::vector<double> v_longitude2;
    std::vector<double> v_longitude3;

    for (double longitude1 = -(MAX_LONGITUDE); longitude1 < MAX_LONGITUDE; longitude1 += precision){
        satellite1.setLatitude(getRandom(m_deviationRangeLateral));
        satellite1.setLongitude(longitude1);
        satellite1.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

        for (double longitude2 = longitude1; longitude2 < MAX_LONGITUDE; longitude2 += precision){
            satellite2.setLatitude(getRandom(m_deviationRangeLateral));
            satellite2.setLongitude(longitude2);
            satellite2.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

            for (double longitude3 = longitude2; longitude3 < MAX_LONGITUDE; longitude3 += precision){
                satellite3.setLatitude(getRandom(m_deviationRangeLateral));
                satellite3.setLongitude(longitude3);
                satellite3.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

                Channel *satellite1_alice;
                try {
                    satellite1_alice = new Channel(satellite1, m_alice, m_alice_atmosphere);
                } catch (const std::runtime_error &e){
                    continue;
                }

                Channel *satellite1_satellite2;
                try {
                    satellite1_satellite2 = new Channel(satellite1, satellite2);
                } catch (const std::runtime_error &e){
                    delete satellite1_alice;
                    continue;
                }

                Channel *satellite3_satellite2;
                try {
                    satellite3_satellite2 = new Channel(satellite3, satellite2);
                } catch (const std::runtime_error &e){
                    delete satellite1_alice;
                    delete satellite1_satellite2;
                    continue;
                }

                Channel *satellite3_bob;
                try {
                    satellite3_bob = new Channel(satellite3, m_bob, m_bob_atmosphere);
                } catch (const std::runtime_error &e){
                    delete satellite1_alice;
                    delete satellite1_satellite2;
                    delete satellite3_satellite2;
                    continue;
                }

                addChannel(satellite1_alice);
                addChannel(satellite1_satellite2);
                addChannel(satellite3_satellite2);
                addChannel(satellite3_bob);

                v_qber.push_back(getQBER());
                v_optical.push_back(getOpticalDistance());
                v_longitude1.push_back(longitude1);
                v_longitude2.push_back(longitude2);
                v_longitude2.push_back(longitude3);

                deleteChannel(satellite1_alice);
                deleteChannel(satellite1_satellite2);
                deleteChannel(satellite3_satellite2);
                deleteChannel(satellite3_bob);
            }
        }
    }

    if (0 < v_qber.size()){
        const std::string folder = makeFolder("3D", m_heightAboveSeaLevel, m_type);
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
