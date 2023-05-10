#ifndef PREPAREANDMEASURE_HPP
#define PREPAREANDMEASURE_HPP

#include "../include/Network.hpp"
#include <string>

struct Device;

class PrepareAndMeasure : public Network
{
public:
    PrepareAndMeasure(Device &alice, Device &bob, const double heightAboveSeaLevel, const double deviationRangeHeight = 0.0, const double deviationRangeLateral = 0.0);
    ~PrepareAndMeasure();

    void simulateSingleSatelliteDefault(double precision) override;
    void simulateDoubleSatelliteDefault(double precision) override;
    void simulateTripleSatelliteDefault(double precision) override;

private:
    double getQBER() override;
    static std::string type;
};

#endif
