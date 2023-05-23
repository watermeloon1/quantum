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
    
}
void PrepareAndMeasure::initChannels(Device &satellite1, Device &satellite2) {
    
}
void PrepareAndMeasure::initChannels(Device &satellite1, Device &satellite2, Device &satellite3) {
    
}


/*
void PrepareAndMeasure::simulateSingleSatelliteDefault(double precision){
    Device satellite = Device("satellite", 0.0, 0.0, m_heightAboveSeaLevel);

    std::vector<double> v_qber;
    std::vector<double> v_optical;
    std::vector<double> v_longitude;

    for (double longitude = -(MAX_LONGITUDE); longitude < MAX_LONGITUDE; longitude += precision){
        satellite.setLatitude(getRandom(m_deviationRangeLateral));
        satellite.setLongitude(longitude);
        satellite.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

        Channel *alice_satellite;
        try {
            alice_satellite = new Channel(m_alice, satellite, m_alice_atmosphere);
        } catch (const std::runtime_error &e){
            continue;
        }

        Channel *satellite_bob;
        try {
            satellite_bob = new Channel(satellite, m_bob, m_bob_atmosphere);
        } catch (const std::runtime_error &e){
            delete alice_satellite;
            continue;
        }

        addChannel(alice_satellite);
        addChannel(satellite_bob);

        v_qber.push_back(getQBER());
        v_optical.push_back(getOpticalDistance());
        v_longitude.push_back(longitude);

        deleteChannel(alice_satellite);
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

void PrepareAndMeasure::simulateDoubleSatelliteDefault(double precision){
    Device satellite1 = Device("satellite_one", 0.0, 0.0, m_heightAboveSeaLevel);
    Device satellite2 = Device("satellite_two", 0.0, 0.0, m_heightAboveSeaLevel);

    std::vector<double> v_qber;
    std::vector<double> v_optical;
    std::vector<double> v_longitude1;
    std::vector<double> v_longitude2;

    for (double longitude1 = -(MAX_LONGITUDE); longitude1 < MAX_LONGITUDE; longitude1 += precision){
        satellite1.setLatitude(getRandom(m_deviationRangeLateral));
        satellite1.setLongitude(longitude1);
        satellite1.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

        for (double longitude2 = longitude1; longitude2 < MAX_LONGITUDE; longitude2 += precision){
            satellite2.setLatitude(getRandom(m_deviationRangeLateral));
            satellite2.setLongitude(longitude2);
            satellite2.setHeightAboveSeaLevel(m_heightAboveSeaLevel + getRandom(m_deviationRangeHeight));

            Channel *alice_satellite1;
            try {
                alice_satellite1 = new Channel(m_alice, satellite1, m_alice_atmosphere);
            } catch (const std::runtime_error &e){
                continue;
            }

            Channel *satellite1_satellite2;
            try {
                satellite1_satellite2 = new Channel(satellite1, satellite2);
            } catch (const std::runtime_error &e){
                delete alice_satellite1;
                continue;
            }

            Channel *satellite2_bob;
            try {
                satellite2_bob = new Channel(satellite2, m_bob, m_bob_atmosphere);
            } catch (const std::runtime_error &e){
                delete alice_satellite1;
                delete satellite1_satellite2;
                continue;
            }

            addChannel(alice_satellite1);
            addChannel(satellite1_satellite2);
            addChannel(satellite2_bob);

            v_qber.push_back(getQBER());
            v_optical.push_back(getOpticalDistance());
            v_longitude1.push_back(longitude1);
            v_longitude2.push_back(longitude2);

            deleteChannel(alice_satellite1);
            deleteChannel(satellite1_satellite2);
            deleteChannel(satellite2_bob);
        }
    }
    if (0 < v_qber.size()){
        const std::string folder = makeFolder("1D", m_heightAboveSeaLevel, m_type);
        if (folder != "ERROR"){
            dataLogger("qber", folder, v_qber);
            dataLogger("optical", folder, v_optical);
            dataLogger("longitude", folder, v_longitude1);
            dataLogger("longitude", folder, v_longitude2);
            //etc
        }
    } else {
        std::cerr << "WARNING: the network is unable to establish communication between " << m_alice.getName() <<
            " and " << m_bob.getName() << std::endl;
    }
}

void PrepareAndMeasure::simulateTripleSatelliteDefault(double precision){
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


                Channel *alice_satellite1;
                try {
                    alice_satellite1 = new Channel(m_alice, satellite1, m_alice_atmosphere);
                } catch (const std::runtime_error &e){
                    continue;
                }

                Channel *satellite1_satellite2;
                try {
                    satellite1_satellite2 = new Channel(satellite1, satellite2);
                } catch (const std::runtime_error &e){
                    delete alice_satellite1;
                    continue;
                }

                Channel *satellite2_satellite3;
                try {
                    satellite2_satellite3 = new Channel(satellite2, satellite3);
                } catch (const std::runtime_error &e){
                    delete alice_satellite1;
                    delete satellite1_satellite2;
                    continue;
                }

                Channel *satellite3_bob;
                try {
                    satellite3_bob = new Channel(satellite3, m_bob, m_bob_atmosphere);
                } catch (const std::runtime_error &e){
                    delete alice_satellite1;
                    delete satellite1_satellite2;
                    delete satellite2_satellite3;
                    continue;
                }

                addChannel(alice_satellite1);
                addChannel(satellite1_satellite2);
                addChannel(satellite2_satellite3);
                addChannel(satellite3_bob);

                v_qber.push_back(getQBER());
                v_optical.push_back(getOpticalDistance());
                v_longitude1.push_back(longitude1);
                v_longitude2.push_back(longitude2);
                v_longitude2.push_back(longitude3);

                deleteChannel(alice_satellite1);
                deleteChannel(satellite1_satellite2);
                deleteChannel(satellite2_satellite3);
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
*/
