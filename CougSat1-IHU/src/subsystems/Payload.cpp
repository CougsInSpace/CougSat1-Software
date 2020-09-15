#include "Payload.h"

/**
 * @brief Construct a new Payload::Payload object
 *
 */
Payload::Payload()
{
 // Initialize sensors and pins.
 // get base value of clock
 
}

/**
 * @brief Destroy the Payload::Payload object
 *
 */
Payload::~Payload() {
  
}

float Payload::getCO2Concentration() const 
{

}

float Payload::getPressure() const {

}

float Payload::getTemperature() const {

}

float Payload::getHumidity() const {

}

bool Payload::isEnvironmentReady() const {

}

void Payload::setLED(bool state) {

}

void Payload::activateSyringe() {

}

void Payload::takePicture() {

}

void Payload::runTest()
{
  int CO2, pressure, humidity;
  float curTemp, curPressure;
  string inputLine;
  fstream outFile;
  float timePassed = timer.read() - this->lastRun;
  if(timePassed >= this->interval){ // Read each Data Item, and write to file
      outFile.open(writeFileName, ios::out);
      CO2 = getCO2Concentration();
      pressure = getHumidity();
      humidity = getHumidity();
      curTemp = getTemperature();

      inputLine = std::to_string(CO2) + ",";
      inputLine += std::to_string(pressure) + ",";
      inputLine.append(std::to_string(humidity));
      inputLine.append(",");
      inputLine.append(std::to_string(curTemp));
      inputLine.append(",");
      inputLine.append(std::to_string(curPressure));
      inputLine.append("\n");
      cout << inputLine;
      outFile << inputLine;

      outFile.seekg(0, std::ios::end);
      int file_size = outFile.tellg();
      if(file_size > this->fileSize)
      {
        // Create new file -- set old file to be ready to send depending on
        // how comms wants us to handle this. Likely set flag

      }

  }
  
  

}

mbed_error_status_t Payload::initialize() {
  return MBED_ERROR_UNSUPPORTED;
}
