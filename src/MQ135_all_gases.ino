/*
 * Project MQ135_all_gases
 * Description: Full usage of the MQ135 sensor.
 * Author: Sugar Eugen Sorin
 * Date: 25 January 2020
 */
#include "MQ135AG.h"

// MQ135 gas sensor
//
// Application
// They are used in air quality control equipments for buildings/offices, are suitable for
// detecting of NH3, NOx, alcohol, Benzene, smoke, CO2, etc
//
// Original creator of this library: https://github.com/GeorgK/MQ135

#define PIN_MQ135 A2
MQ135AG mq135_sensor(PIN_MQ135);

#define RESISTANCE_LOOP 50
#define RESISTANCE_DELAY 5000

char publishString [50];
float temperature = 20.0; // assume current temperature. Recommended to measure with DHT22
float humidity = 55.0; // assume current humidity. Recommended to measure with DHT22


// Get resistance and corrected resistance from sensor
void get_resistance() {
    float analogData = analogRead(PIN_MQ135);
    float resistance = mq135_sensor.getResistance();
    float correctedResistance = mq135_sensor.getCorrectedResistance(temperature, humidity);

    Particle.publish("Analog data", String::format("%f", analogData));
    Particle.publish("Resistance", String::format("%f", resistance));
    Particle.publish("Corrected resistance", String::format("%f", correctedResistance));
    delay(5000);
}

// This should be done in a 24 hour time.
void get_average_R0(int loop = 100) {
    int i = 0;
    float loopR0CO;
    float loopR0CO2;
    float loopR0Ethanol;
    float loopR0NH4;
    float loopR0Toluene;
    float loopR0Acetone;

    // Loop to get an average value
    while (i < loop) {
        loopR0CO += mq135_sensor.getRZeroCO();
        loopR0CO2 += mq135_sensor.getRZeroCO2();
        loopR0Ethanol += mq135_sensor.getRZeroEthanol();
        loopR0NH4 += mq135_sensor.getRZeroNH4();
        loopR0Toluene += mq135_sensor.getRZeroToluene();
        loopR0Acetone += mq135_sensor.getRZeroAcetone();
        i++;
        delay(5000);
    }

    // If max loop reached, calculate average values.
    if (i == loop) {
        float R0CO = loopR0CO/100;
        float R0CO2 = loopR0CO2/100;
        float R0Ethanol = loopR0Ethanol/100;
        float R0NH4 = loopR0NH4/100;
        float R0Toluene = loopR0Toluene/100;
        float R0Acetone = loopR0Acetone/100;

        Particle.publish("R0CO", String::format("%f", R0CO));
        Particle.publish("R0CO2", String::format("%f", R0CO2));
        Particle.publish("R0Ethanol", String::format("%f", R0Ethanol));
        Particle.publish("R0NH4", String::format("%f", R0NH4));
        Particle.publish("R0Toluene", String::format("%f", R0Toluene));
        Particle.publish("R0Acetone", String::format("%f", R0Acetone));
    }
}

// Get all PPM values based on one measured resistance
void get_ppm() {
    float resistance = mq135_sensor.getResistance();
    float CO = mq135_sensor.getCO(resistance);
    float CO2 = mq135_sensor.getCO2(resistance);
    float Ethanol = mq135_sensor.getEthanol(resistance);
    float NH4 = mq135_sensor.getNH4(resistance);
    float Toluene = mq135_sensor.getToluene(resistance);
    float Acetone = mq135_sensor.getAcetone(resistance);

    Particle.publish("CO ppm:", String::format("%f", CO));
    Particle.publish("CO2 ppm:", String::format("%f", CO2));
    Particle.publish("Ethanol ppm:", String::format("%f", Ethanol));
    Particle.publish("NH4 ppm:", String::format("%f", NH4));
    Particle.publish("Toluene ppm:", String::format("%f", Toluene));
    Particle.publish("Acetone ppm:", String::format("%f", Acetone));
    delay(5000);
}

// Get calibrated PPM values based on temperature and humidity
void get_calibrated_ppm() {
    float calibratedCO = mq135_sensor.getCalibratedCO(temperature, humidity);
    float calibratedCO2 = mq135_sensor.getCalibratedCO2(temperature, humidity);
    float calibratedEthanol = mq135_sensor.getCalibratedEthanol(temperature, humidity);
    float calibratedNH4 = mq135_sensor.getCalibratedNH4(temperature, humidity);
    float calibratedToluene = mq135_sensor.getCalibratedToluene(temperature, humidity);
    float calibratedAcetone = mq135_sensor.getCalibratedAcetone(temperature, humidity);

    Particle.publish("Calibrated CO ppm:", String::format("%f", calibratedCO));
    Particle.publish("Calibrated CO2 ppm:", String::format("%f", calibratedCO2));
    Particle.publish("Calibrated Ethanol ppm:", String::format("%f", calibratedEthanol));
    Particle.publish("Calibrated NH4 ppm:", String::format("%f", calibratedNH4));
    Particle.publish("Calibrated Toluene ppm:", String::format("%f", calibratedToluene));
    Particle.publish("Calibrated Acetone ppm:", String::format("%f", calibratedAcetone));
    delay(5000);
}

void setup() {
    Particle.publish("Status", "Goliath online");
    get_resistance();
}

void loop() {
    get_resistance();
    // get_average_R0(10);
    // get_ppm();
    // get_calibrated_ppm();
}
