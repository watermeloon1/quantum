#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include "../include/global.hpp"

struct Device;
class Atmosphere;

class Channel
{
public:
    Channel(Device &initiator, Device &receiver, Atmosphere &atmosphere);
    ~Channel();

    global::Direction getDirection() const;
    double getQuantumBitErrorRate() const;
    double getTheta() const;
    double getZenith() const;
    double getOpticalDistance() const;
    double getTransmittance() const;

private:
    global::Direction direction;

    Device &m_initiator;
    Device &m_receiver;
    Atmosphere &m_atmosphere;

    std::vector<double> opticalSectors;
    double opticalDistance;
    double deltaHeight;
    double surfaceDistance;
    double zenith;
    double theta;

    double coherenceLength;
    double beamWidening;
    double targetingError;
    double totalScattering;
    double dynamicTransmittance;
    double staticTransmittance;

    double transmittance;
    double quantumBitErrorRate;

    void initDirection();
    void initTheta();
    void initDeltaHeight();
    void initOpticalDistance();
    void initSurfaceDistance();
    void initZenith();
    void initOpticalSectors();

    void initTransmittance();
    void initQuantumBitErrorRate();
};

#endif
