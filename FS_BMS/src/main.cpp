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

const float MAX_TEENSY_VOLTAGE = 3.3;
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
  // Loop through MUX_ALPHA
  for (int alpha = 0; alpha <= 15; alpha++) {
    // Convert alpha to binary representation
    String alphaBinary = String(alpha, BIN);
    
    // Pad the binary string with zeros to make it 4 bits long
    while (alphaBinary.length() < 4) {
      alphaBinary = "0" + alphaBinary;
    }
    
    // Set the select pins for MUX_ALPHA
    digitalWrite(MUX_ALPHA_S_PIN_0001, alphaBinary[3] == '1' ? HIGH : LOW);
    digitalWrite(MUX_ALPHA_S_PIN_0010, alphaBinary[2] == '1' ? HIGH : LOW);
    digitalWrite(MUX_ALPHA_S_PIN_0100, alphaBinary[1] == '1' ? HIGH : LOW);
    digitalWrite(MUX_ALPHA_S_PIN_1000, alphaBinary[0] == '1' ? HIGH : LOW);
    
    // Read sensor value
    float sensorValue = analogRead(teensy_D_OUT1);
    float voltageToLookup = sensorValue * (MAX_TEENSY_VOLTAGE / 1023.0);
    float temperature = lookupTemperatureForVoltage(voltageToLookup);

    // Print the result for MUX_ALPHA
    Serial.print("MUX_ALPHA (Binary): ");
    Serial.print(alphaBinary);
    Serial.print(", Voltage: ");
    Serial.print(voltageToLookup);
    Serial.print(" volts, Temperature: ");
    Serial.println(temperature);

    delay(1000);
  }

  // Loop through MUX_BRAVO
  for (int bravo = 0; bravo <= 6; bravo++) {
    // Convert bravo to binary representation
    String bravoBinary = String(bravo, BIN);
    
    // Pad the binary string with zeros to make it 3 bits long
    while (bravoBinary.length() < 3) {
      bravoBinary = "0" + bravoBinary;
    }
    
    // Set the select pins for MUX_BRAVO
    digitalWrite(MUX_BRAVO_S_PIN_0001, bravoBinary[2] == '1' ? HIGH : LOW);
    digitalWrite(MUX_BRAVO_S_PIN_0010, bravoBinary[1] == '1' ? HIGH : LOW);
    digitalWrite(MUX_BRAVO_S_PIN_0100, bravoBinary[0] == '1' ? HIGH : LOW);

    // Read sensor value
    float sensorValue = analogRead(teensy_D_OUT1);
    float voltageToLookup = sensorValue * (MAX_TEENSY_VOLTAGE / 1023.0);
    float temperature = lookupTemperatureForVoltage(voltageToLookup);

    // Print the result for MUX_BRAVO
    Serial.print("MUX_BRAVO (Binary): ");
    Serial.print(bravoBinary);
    Serial.print(", Voltage: ");
    Serial.print(voltageToLookup);
    Serial.print(" volts, Temperature: ");
    Serial.println(temperature);

    delay(1000);
  }

  // digitalWrite(MUX_ALPHA_S_PIN_0001, LOW);
  // digitalWrite(MUX_ALPHA_S_PIN_0010, LOW);
  // digitalWrite(MUX_ALPHA_S_PIN_0100, LOW);
  // digitalWrite(MUX_ALPHA_S_PIN_1000, LOW);
  // digitalWrite(MUX_BRAVO_S_PIN_0001, LOW);
  // digitalWrite(MUX_BRAVO_S_PIN_0010, LOW);
  // digitalWrite(MUX_BRAVO_S_PIN_0100, LOW);

  // float sensorValue = (analogRead(teensy_D_OUT1));

  // float voltageToLookup = (sensorValue * (MAX_TEENSY_VOLTAGE / 1023.0));
  
  // float temperature = lookupTemperatureForVoltage(voltageToLookup);

  // // Print the result
  // Serial.print("Voltage: ");
  // Serial.print(voltageToLookup);
  // Serial.print(" volts, Temperature: ");
  // Serial.println(temperature);

  // delay(1000);
  
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
  // digitalWrite(MUX_S_PIN_0001, bitRead(sensorNumber, 0));
  // digitalWrite(MUX_S_PIN_0010, bitRead(sensorNumber, 1));
  // digitalWrite(MUX_S_PIN_0100, bitRead(sensorNumber, 2));
  // digitalWrite(MUX_S_PIN_1000, bitRead(sensorNumber, 3));

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