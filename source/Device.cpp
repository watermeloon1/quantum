#include "../include/Device.hpp"

Device::Device(const std::string &name, const double latitude, const double longitude, const double heightAboveSeaLevel):
    m_name(name), m_latitude(latitude), m_longitude(longitude), m_heightAboveSeaLevel(heightAboveSeaLevel) {
        initType();
        initDefaultOpticalParameters();
}

Device::~Device(){
}

void Device::initType() {
    if (m_heightAboveSeaLevel == 0) {
        type = "Base";
    }
    else {
        type = "Satellite";
    }
}

void Device::initDefaultOpticalParameters() {
    focus = global::optimal;
    noise = 0.000005;
    waveLength = 860;
    apertureDiameter = 0.2;
    mirrorDiameter = .8;
    quantumEfficiencyOfDetector = 0.7;
    meanPhotonNumberOfSignal = 0.1;
    probabilityOfPolarizationMeasurementError = 0.0001;
    targetingAngularError = 0.5;
    detectorCount = 4;
}

std::string Device::getType() const {
    return type;
}

std::string Device::getName() const {
    return m_name;
}


double Device::getLatitude() const {
    return m_latitude;
}

double Device::getLongitude() const {
    return m_longitude;
}

double Device::getHeightAboveSeaLevel() const {
    return m_heightAboveSeaLevel;
}

global::Focus Device::getFocus() const {
    return focus;
}

double Device::getNoise() const {
    return noise;
}

double Device::getWaveLength() const {
    return waveLength;
}

double Device::getApertureDiameter() const {
    return apertureDiameter;
}

double Device::getMirrorDiameter() const {
    return mirrorDiameter;
}

double Device::getQuantumEfficiencyOfDetector() const {
    return quantumEfficiencyOfDetector;
}

double Device::getMeanPhotonNumberOfSignal() const {
    return meanPhotonNumberOfSignal;
}

double Device::getProbabilityOfPolarizationMeasurementError() const {
    return probabilityOfPolarizationMeasurementError;
}

double Device::getTargetingAngularError() const {
    return targetingAngularError;
}

unsigned short Device::getDetectorCount() const {
    return detectorCount;
}

void Device::setLatitude(const double latitude) {
    m_latitude = latitude;
}

void Device::setLongitude(const double longitude) {
    m_longitude = longitude;
}

void Device::setHeightAboveSeaLevel(const double heightAboveSeaLevel) {
    m_heightAboveSeaLevel = heightAboveSeaLevel;
}
