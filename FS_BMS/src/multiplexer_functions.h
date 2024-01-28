#ifndef MULTIPLEXER_FUNCTIONS_H
#define MULTIPLEXER_FUNCTIONS_H

// Declare functions related to the CD74HC4067 multiplexer
void selectSensor(int daughtBoard, int sensorNumber);
int readTemperature();
void sendTemperatureOverCAN(int temperature);

#endif
