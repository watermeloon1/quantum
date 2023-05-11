#ifndef ENTANGLEMENT_HPP
#define ENTANGLEMENT_HPP

#include "../include/Network.hpp"
#include <string>

struct Device;

class Entanglement : public Network
{
public:
    Entanglement(Device &alice, Device &bob, const double heightAboveSeaLevel = 200.0, const double deviationRangeHeight = 0.0, const double deviationRangeLateral = 0.0);
    ~Entanglement();

    void simulateSingleSatelliteDefault(double precision) override;
    void simulateDoubleSatelliteDefault(double precision) override;
    void simulateTripleSatelliteDefault(double precision) override;

    void simulateSingleSatelliteUplink(double precision);

private:
    double getQBER() override;
    static std::string m_type;
};

#endif
