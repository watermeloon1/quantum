#include "../include/Entanglement.hpp"
#include "../include/Device.hpp"
#include "../include/Atmosphere.hpp"
#include "../include/Channel.hpp"

std::string Entanglement::m_type = "ent";

Entanglement::Entanglement(Device &alice, Device &bob, const double heightAboveSeaLevel, const double deviationRangeHeight, const double deviationRangeLateral):
    Network(alice, bob, heightAboveSeaLevel, deviationRangeHeight, deviationRangeLateral){

}

Entanglement::~Entanglement(){
}

Atmosphere atm = Atmosphere("Midlatitude", "Summer", "Clear");

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
            satellite_alice = new Channel(m_alice, satellite, atm);
        } catch (const std::runtime_error &e){
            continue;
        }

        Channel *satellite_bob;
        try {
            satellite_bob = new Channel(satellite, m_bob, atm);
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

    const std::string folder = makeFolder("1D", m_heightAboveSeaLevel, m_type);

    if (folder != "ERROR"){
        dataLogger("qber", folder, v_qber);
        dataLogger("optical", folder, v_optical);
        dataLogger("longitude", folder, v_longitude);
        //etc
    }
}

void Entanglement::simulateDoubleSatelliteDefault(double precision){
}
void Entanglement::simulateTripleSatelliteDefault(double precision){
}
double Entanglement::getQBER(){
}
