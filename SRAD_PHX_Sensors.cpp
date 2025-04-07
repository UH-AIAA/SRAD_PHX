/* SRAD Avionics Flight Software for AIAA-UH
 *
 * Copyright (c) 2024 Nathan Samuell + Dedah + Thanh! (www.github.com/nathansamuell, www.github.com/UH-AIAA)
 *
 * More information on the MIT license as well as a complete copy
 * of the license can be found here: https://choosealicense.com/licenses/mit/
 *
 * All above text must be included in any redistribution.
 */

#include "SRAD_PHX.h"

/**
 * Reads the Adafruit LSM6DS032 6 DoF Accelerometer/Gyroscope
 * @param LSM Referenece to initialized sensor instance
 * @returns True if the operation succeeds, False if the operation fails
 */
bool FLIGHT::read_LSM(Adafruit_LSM6DSO32 &LSM) {
    sensors_event_t accel, gyro, temp;

  // Attempt to read sensor data
    if(!LSM.getEvent(&accel, &gyro, &temp))
    {
        return false;  // Return false if read fails
    }

  // Store gyroscope data
    output.lsm_gyro.x = gyro.gyro.x;
    output.lsm_gyro.y = gyro.gyro.y;
    output.lsm_gyro.z = gyro.gyro.z;

    // Store accelerometer data
    output.lsm_acc.x = accel.acceleration.x;
    output.lsm_acc.y = accel.acceleration.y;
    output.lsm_acc.z = accel.acceleration.z;

    // Store temperature data
    output.lsm_temp = float(temp.temperature);

    return true;  // Return true if read succeeds
}

/**
 * Reads the Adafruit BMP388 Precision Barometer and Altimeter
 * @param BMP Reference to initialized sensor instance\
 * @return Returns true if operation succeeds
 */
bool FLIGHT::read_BMP(Adafruit_BMP3XX &BMP) {
    if (!BMP.performReading()) {
        return false;
    }
    output.bmp_temp = BMP.temperature;
    output.bmp_press = BMP.pressure;
    output.bmp_alt = BMP.readAltitude(1013.25) - output.off_alt;
    return true;
}

/**
 * Reads the Adafruit ADXL_375 High-G Accelerometer
 * @param ADXL Reference to initialized sensor instance
 * @return Returns true if operation succeeds
 */
bool FLIGHT::read_ADXL(Adafruit_ADXL375 &ADXL) {
    sensors_event_t event;
    if (!ADXL.getEvent(&event)) {
        return false;
    }
    output.adxl_acc.x = event.acceleration.x;
    output.adxl_acc.y = event.acceleration.y;
    output.adxl_acc.z = event.acceleration.z;

    output.adxl_temp = float(event.temperature);
    return true;
}

/**
 * Returns Adafruit BNO055 Absolute Orientation Sensor
 * @param BNO Initialized sensor instance
 * @return Returns true if operation succeeds
 */
bool FLIGHT::read_BNO(Adafruit_BNO055 &BNO) {
    sensors_event_t orientationData, angVelocityData, magnetometerData, accelerometerData;

    if (!BNO.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER)) {
        return false;
    }
    if (!BNO.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE)) {
        return false;
    }
    if (!BNO.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER)) {
        return false;
    }
    if (!BNO.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER)) {
        return false;
    }

    imu::Quaternion quat = BNO.getQuat();
    output.bno_orientation.w = quat.w();
    output.bno_orientation.x = quat.x();
    output.bno_orientation.y = quat.y();
    output.bno_orientation.z = quat.z();

    output.bno_gyro.x = angVelocityData.gyro.x;
    output.bno_gyro.y = angVelocityData.gyro.y;
    output.bno_gyro.z = angVelocityData.gyro.z;

    output.bno_acc.x = accelerometerData.acceleration.x;
    output.bno_acc.y = accelerometerData.acceleration.y;
    output.bno_acc.z = accelerometerData.acceleration.z;

    output.bno_temp = float(BNO.getTemp());
    return true;
}

/**
 * Reads Adafruit Ultimate GPS Breakout V3
 * @param GPS Initialized Sensor instance
 * @return Returns false if GPS isn't ready in 500ms or no satellite fix, returns true otherwise
 */
bool FLIGHT::read_GPS(Adafruit_GPS &GPS) {
    last_gps = GPS;                     // update the last-used GPS pointer
    if(!GPS.fix) {
        return false;
    }
    uint32_t startms = millis();        // starting time
    uint32_t timeout = startms + 500;   // if GPS doesn't receive new NMEA after 500ms, times out to prevent blocking
    while (true) {
        startms = millis();
        if(startms > timeout) {
            return false;
        } else if(GPS.available()) {
            GPS.read();
            if (GPS.newNMEAreceived()) {
                GPS.parse(GPS.lastNMEA());
                return true;
            }
        }
    }
}
