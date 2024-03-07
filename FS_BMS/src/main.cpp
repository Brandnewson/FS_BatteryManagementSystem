/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include "macros.h"
#include "multiplexer_functions.h"
#include <cmath> // for std::abs

// #include <iostream>
// #include <array>
// Define a struct to hold temperature and voltage values
struct TemperatureVoltagePair {
  float temperature;
  float voltage;
};

// Define the lookup table
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

void setup()
{
  // Initialize CD74HC4067 multiplexer
  pinMode(MUX_COMMON_PIN, OUTPUT);
  pinMode(MUX_S_PIN_0001, OUTPUT);
  pinMode(MUX_S_PIN_0010, OUTPUT);
  pinMode(MUX_S_PIN_0100, OUTPUT);
  pinMode(MUX_S_PIN_1000, OUTPUT);
  pinMode(MUX_ENABLE_PIN, OUTPUT);
  pinMode(MUX_DATA_PIN, INPUT);

  // Initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // Example: Lookup temperature for a voltage of 1.86
  // float voltageToLookup = 1.88;

  // digitalWrite(teensy_S0, 0);
  // digitalWrite(teensy_S1, 0);
  // digitalWrite(teensy_S2, 0);
  // digitalWrite(teensy_S3, 0);
  // digitalWrite(teensy_S4, 0);
  // digitalWrite(teensy_S5, 0);
  // digitalWrite(teensy_S6, 0);
  // digitalWrite(teensy_S7, 0);

  float voltageToLookup = analogRead(teensy_D_OUT1);
  
  int temperature = lookupTemperatureForVoltage(voltageToLookup);

  // Print the result
  Serial.print("Voltage: ");
  Serial.print(voltageToLookup);
  Serial.print(" volts, Temperature: ");
  Serial.println(temperature);

  delay(1000);
  
  // // Example: Select the first sensor on daughter board 1
  // selectSensor(1, 1);

  // // Read temperature data from the selected sensor
  // temperature = readTemperature();

  // // Example: Send temperature data over the CAN bus
  // sendTemperatureOverCAN(temperature);

  // // Toggle the LED based on temperature value
  // digitalWrite(LED_BUILTIN, temperature > 50 ? HIGH : LOW);

  // // Wait for a second
  // delay(1000);
}


void selectSensor(int daughtBoard, int sensorNumber)
{
  // Convert the sensor number to binary and apply it to the S pins
  digitalWrite(MUX_S_PIN_0001, bitRead(sensorNumber, 0));
  digitalWrite(MUX_S_PIN_0010, bitRead(sensorNumber, 1));
  digitalWrite(MUX_S_PIN_0100, bitRead(sensorNumber, 2));
  digitalWrite(MUX_S_PIN_1000, bitRead(sensorNumber, 3));

  // Enable the multiplexer
  digitalWrite(MUX_ENABLE_PIN, HIGH);
}

int readTemperature()
{
  // Read data from the data pin and process as needed
  // int temperature = analogRead(MUX_DATA_PIN);

  // Additional processing if required

  // return temperature;

  // Simulate temperature reading (replace with actual sensor reading when available)
  return random(0, 100);  // Simulating a temperature value between 0 and 100
}

void sendTemperatureOverCAN(int temperature)
{
  // Implement CAN bus communication to send the temperature data
  // Use appropriate CAN bus library functions
}