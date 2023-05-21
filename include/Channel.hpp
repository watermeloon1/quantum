#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include "../include/global.hpp"

struct Device;
class Atmosphere;

class Channel
{
public:
    Channel(const Device &initiator, const Device &receiver, const Atmosphere *atmosphere);
    Channel(const Device &initiator, const Device &receiver);
    ~Channel();

    global::Direction getDirection() const;
    double getQuantumBitErrorRate() const;
    double getTheta() const;
    double getZenith() const;
    double getOpticalDistance() const;
    double getTransmittance() const;
    void update();

private:
    global::Direction direction;

    const Device &m_initiator;
    const Device &m_receiver;
    const Atmosphere *m_atmosphere;

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
