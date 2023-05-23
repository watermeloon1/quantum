#ifndef PREPAREANDMEASURE_HPP
#define PREPAREANDMEASURE_HPP

#include "../include/Network.hpp"
#include <string>

struct Device;
class Atmosphere;

class PrepareAndMeasure : public Network
{
public:
    PrepareAndMeasure(const Device &alice, const Atmosphere *alice_atmosphere, const Device &bob, const Atmosphere *bob_atmosphere, const double heightAboveSeaLevel = 200.0, const double deviationRangeHeight = 0.0, const double deviationRangeLateral = 0.0);
    ~PrepareAndMeasure();
    
    void initChannels(Device &satellite) override;
    void initChannels(Device &satellite1, Device &satellite2) override;
    void initChannels(Device &satellite1, Device &satellite2, Device &satellite3) override;

private:
    double getQBER() const override;
};

#endif
