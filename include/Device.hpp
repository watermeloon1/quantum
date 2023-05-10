#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <string>
#include "../include/global.hpp"

struct Device {
public:
    Device(const std::string &name, const double latitude, const double longitude, const double heightAboveSeaLevel = 0.0);
    ~Device();

    const std::string m_name;

    std::string getType() const;

    double getLatitude() const;
    double getLongitude() const;
    double getHeightAboveSeaLevel() const;

    global::Focus getFocus() const;
    double getNoise() const;
    double getWaveLength() const;
    double getApertureDiameter() const;
    double getMirrorDiameter() const;
    double getQuantumEfficiencyOfDetector() const;
    double getMeanPhotonNumberOfSignal() const;
    double getProbabilityOfPolarizationMeasurementError() const;
    double getTargetingAngularError() const;
    unsigned short getDetectorCount() const;

    void setLatitude(const double latitude);
    void setLongitude(const double longitude);
    void setHeightAboveSeaLevel(const double heightAboveSeaLevel);

private:
    std::string type;

    double m_latitude;
    double m_longitude;
    double m_heightAboveSeaLevel;

    global::Focus focus;
    double noise;
    double waveLength;
    double apertureDiameter;
    double mirrorDiameter;
    double quantumEfficiencyOfDetector;
    double meanPhotonNumberOfSignal;
    double probabilityOfPolarizationMeasurementError;
    double targetingAngularError;
    unsigned short detectorCount;

    void initType();
    void initDefaultOpticalParameters();
};

#endif
