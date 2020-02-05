#include "Payload.h"

/**
 * @brief Construct a new Payload::Payload object
 *
 */
Payload::Payload() {}

/**
 * @brief Destroy the Payload::Payload object
 *
 */
Payload::~Payload() {}

float Payload::getCO2Concentration() const {}

float Payload::getPressure() const {}

float Payload::getTemperature() const {}

float Payload::getHumidity() const {}

bool Payload::isEnvironmentReady() const {}

void Payload::setLED(bool state) {}

void Payload::activateSyringe() {}

void Payload::takePicture() {}

mbed_error_status_t Payload::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}
