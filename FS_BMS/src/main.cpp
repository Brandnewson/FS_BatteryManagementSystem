/**
 * BMS Temperature Sensor Emulator
 *
 * Runs through 22 different temperature sensors and send the appropriate temperature to the orion BMS sensor
 * 
 */
#include "Arduino.h"
#include "macros.h"
#include "multiplexer_functions.h"
#include <cmath> // for std::abs

const float MAX_TEENSY_VOLTAGE = 3.3;
const int MAX_ITERATIONS_ALPHA = 15;  // how many pins of mux alpha to loop through
const int MAX_ITERATIONS_BRAVO = 6;   // how many pins of mux bravo to loop through

void loopThroughMUX(int pin1, int pin2, int pin3, int pin4, int maxIterations, String muxName);

const float MAX_TEENSY_VOLTAGE = 3.3;
// Define a struct to hold temperature and voltage values
struct TemperatureVoltagePair {
  float temperature;
  float voltage;
};

// Define the lookup table based on the ENEPAC
TemperatureVoltagePair temperatureVoltageTable[] = {
  {-40, 2.44},
  {-35, 2.42},
  {-30, 2.40},
  {-25, 2.38},
  {-20, 2.35},
  {-15, 2.32},
  {-10, 2.27},
  {-5, 2.23},
  {0, 2.17},
  {5, 2.11},
  {10, 2.05},
  {15, 1.99},
  {20, 1.92},
  {25, 1.86},
  {30, 1.80},
  {35, 1.74},
  {40, 1.68},
  {45, 1.63},
  {50, 1.59},
  {55, 1.55},
  {60, 1.51},
  {65, 1.48},
  {70, 1.45},
  {75, 1.43},
  {80, 1.40},
  {85, 1.38},
  {90, 1.37},
  {95, 1.35},
  {100, 1.34},
  {105, 1.33},
  {110, 1.32},
  {120, 1.30}
};

// Function to look up temperature for a given voltage
float lookupTemperatureForVoltage(float voltage) {
    int tableSize = sizeof(temperatureVoltageTable) / sizeof(temperatureVoltageTable[0]);
    for (int i = 0; i < tableSize - 1; i++) {
      // return an interpolated temp based on lookup table
      if (voltage <= temperatureVoltageTable[i].voltage && voltage >= temperatureVoltageTable[i + 1].voltage) {
          float v1 = temperatureVoltageTable[i].voltage;
          float v2 = temperatureVoltageTable[i + 1].voltage;
          int t1 = temperatureVoltageTable[i].temperature;
          int t2 = temperatureVoltageTable[i + 1].temperature;

          // Perform linear interpolation
          float temperature = t1 + ((t2 - t1) * (voltage - v1) / (v2 - v1));
          return temperature;
      }
    }
    // Voltage is outside the table range
    return -1.0f;
}

void loopThroughMUX(int pin1, int pin2, int pin3, int pin4, int maxIterations, String muxName) {
  for (int i = 0; i <= maxIterations; i++) {
    // Convert i to binary representation
    String binary = String(i, BIN);
    
    // Pad the binary string with zeros to make it appropriate length
    while (binary.length() < (pin4 != LOW ? 4 : 3)) {
      binary = "0" + binary;
    }
    
    // Set the select pins based on the binary representation
    digitalWrite(pin1, binary[binary.length() - 1] == '1' ? HIGH : LOW);
    digitalWrite(pin2, binary[binary.length() - 2] == '1' ? HIGH : LOW);
    digitalWrite(pin3, binary[binary.length() - 3] == '1' ? HIGH : LOW);
    if (pin4 != LOW) digitalWrite(pin4, binary[binary.length() - 4] == '1' ? HIGH : LOW);
    
    // Read sensor value
    float sensorValue = analogRead(teensy_D_OUT1);
    float voltageToLookup = sensorValue * (MAX_TEENSY_VOLTAGE / 1023.0);
    float temperature = lookupTemperatureForVoltage(voltageToLookup);

    // Print the result
    Serial.print(muxName);
    Serial.print(" (Binary): ");
    Serial.print(binary);
    Serial.print(", Voltage: ");
    Serial.print(voltageToLookup);
    Serial.print(" volts, Temperature: ");
    Serial.println(temperature);

    delay(1000);  // please adjust this value accordingly, it is 1000 for easier visual inspection, but in practice has to be faster
  }
}


void sendTemperatureOverCAN(int temperature)
{
  // Implement CAN bus communication to send the temperature data
  // Use appropriate CAN bus library functions
}

void setup()
{
  // Initialize CD74HC4067 multiplexer
  pinMode(MUX_COMMON_PIN, OUTPUT);
  pinMode(MUX_ALPHA_S_PIN_0001, OUTPUT);
  pinMode(MUX_ALPHA_S_PIN_0010, OUTPUT);
  pinMode(MUX_ALPHA_S_PIN_0100, OUTPUT);
  pinMode(MUX_ALPHA_S_PIN_1000, OUTPUT);
  pinMode(MUX_BRAVO_S_PIN_0001, OUTPUT);
  pinMode(MUX_BRAVO_S_PIN_0010, OUTPUT);
  pinMode(MUX_BRAVO_S_PIN_0100, OUTPUT);
  
  // pinMode(MUX_DATA_PIN, INPUT);
  pinMode(teensy_D_OUT1, INPUT); 

  // Initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop()
{
  // Loop through the first mux (MUX ALPHA)
  loopThroughMUX(MUX_ALPHA_S_PIN_0001, MUX_ALPHA_S_PIN_0010, MUX_ALPHA_S_PIN_0100, MUX_ALPHA_S_PIN_1000, MAX_ITERATIONS_ALPHA, "MUX_ALPHA");
  // Loop through the first mux (MUX BRAVO)
  loopThroughMUX(MUX_BRAVO_S_PIN_0001, MUX_BRAVO_S_PIN_0010, MUX_BRAVO_S_PIN_0100, LOW, MAX_ITERATIONS_BRAVO, "MUX_BRAVO");

  // Send temperature data over the CAN bus
  // sendTemperatureOverCAN(temperature);

}
