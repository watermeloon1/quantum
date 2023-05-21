#ifndef ENTANGLEMENT_HPP
#define ENTANGLEMENT_HPP

#include "../include/Network.hpp"
#include <string>

struct Device;
class Atmosphere;

class Entanglement : public Network
{
public:
    Entanglement(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel = 200.0, const double deviationRangeHeight = 0.0, const double deviationRangeLateral = 0.0);
    ~Entanglement();

    void simulateDoubleSatelliteDefault(double precision) override;
    void simulateTripleSatelliteDefault(double precision) override;

    void simulateSingleSatelliteUplink(double precision);

    std::string getType() const override;
    void initChannels(Device &satellite) override;
    void initChannels(Device &satellite1, Device &satellite2) override;
    void initChannels(Device &satellite1, Device &satellite2, Device &satellite3) override;


private:
    double getQBER() const override;
    static std::string m_type;
};

#endif
