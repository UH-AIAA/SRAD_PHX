/* SRAD Avionics Flight Software for AIAA-UH
 *
 * Copyright (c) 2025 Nathan Samuell + Dedah + Thanh! (www.github.com/nathansamuell, www.github.com/UH-AIAA)
 *
 * More information on the MIT license as well as a complete copy
 * of the license can be found here: https://choosealicense.com/licenses/mit/
 *
 * All above text must be included in any redistribution.
 */

#include "SRAD_PHX.h"

/** 
 * @brief tracks time during flight
 * 
 * This function calls the millis() function from the
 * Arduino library to track milliseconds since the board
 * was powered on. It updates three variables: 
 * 
 * 1. `deltaTime_ms`
 * 2. `runningTime_ms`
 * 3. `output.totalTime_ms`
 */
void FLIGHT::incrementTime() {
    uint64_t newRunningTime_ms = millis();
    deltaTime_ms = newRunningTime_ms - runningTime_ms;
    runningTime_ms = newRunningTime_ms;
    output.totalTime_ms = newRunningTime_ms;
}

/**
 * @brief writes data stored in `output` to file
 * @param headers If true, function will only right headers and return early
 * @param File A reference to Arduino file type from SD.h
 * 
 * This function can write data headers or current data to SD card.
 */
void FLIGHT::writeSD(bool headers, File& outputFile) {
    if(headers) {
        outputFile.println(data_header);
        outputFile.flush();
        return;
    }

    outputFile.print(output.totalTime_ms); outputFile.print(", ");
    if(last_gps.fix) {
        outputFile.print(last_gps.latitudeDegrees, 6); outputFile.print(", ");
        outputFile.print(last_gps.longitudeDegrees, 6); outputFile.print(",");
        outputFile.print((int32_t)last_gps.satellites); outputFile.print(",");
        outputFile.print(last_gps.speed, 3); outputFile.print(",");
        outputFile.print(last_gps.angle, 3); outputFile.print(",");
        outputFile.print(last_gps.altitude, 3); outputFile.print(",");
    } else {
        outputFile.print("-1,No fix,-1,No fix,0,-1,-1,-1,");
    }
    outputFile.print(output.bno_orientation.w, 5); outputFile.print(",");
    outputFile.print(output.bno_orientation.x, 5); outputFile.print(",");
    outputFile.print(output.bno_orientation.y, 5); outputFile.print(",");
    outputFile.print(output.bno_orientation.z, 5); outputFile.print(",");
    outputFile.print(output.bno_gyro.x, 5); outputFile.print(",");
    outputFile.print(output.bno_gyro.y, 5); outputFile.print(",");
    outputFile.print(output.bno_gyro.z, 5); outputFile.print(",");
    outputFile.print(output.bno_acc.x, 4); outputFile.print(",");
    outputFile.print(output.bno_acc.y, 4); outputFile.print(",");
    outputFile.print(output.bno_acc.z, 4); outputFile.print(",");
    outputFile.print(output.adxl_acc.x, 2); outputFile.print(",");
    outputFile.print(output.adxl_acc.y, 2); outputFile.print(",");
    outputFile.print(output.adxl_acc.z, 2); outputFile.print(",");
    outputFile.print(output.bmp_press, 6); outputFile.print(",");
    outputFile.print(output.bmp_alt, 4); outputFile.print(",");
    outputFile.print(output.lsm_temp, 2); outputFile.print(",");
    outputFile.print(output.adxl_temp, 2); outputFile.print(",");
    outputFile.print(output.bno_temp, 2); outputFile.print(",");
    outputFile.print(output.bmp_temp, 2); outputFile.println();
    outputFile.print(output.sensorStatus.test(0)); outputFile.print(", ");
    outputFile.print(output.sensorStatus.test(1)); outputFile.print(", ");
    outputFile.print(output.sensorStatus.test(2)); outputFile.print(", ");
    outputFile.print(output.sensorStatus.test(3)); outputFile.print(", ");
    outputFile.print(output.sensorStatus.test(4)); outputFile.println();
    outputFile.flush();

    return;
}

/**
 * @brief writes data stored in `output` to a serial port
 * @param headers If true, function will only right headers and return early
 * @param Serial1 The serial port to write data to
 * 
 * This function can write data headers or current data to a serial port.
 */
void FLIGHT::writeSERIAL(bool headers, Stream& outputSerial) {
    if(headers) {
        outputSerial.println(data_header);
        outputSerial.flush();
        return;
    }

    outputSerial.print(output.totalTime_ms); outputSerial.print(", ");
    if(last_gps.fix) {
        outputSerial.print(last_gps.latitudeDegrees, 6); outputSerial.print(", ");
        outputSerial.print(last_gps.longitudeDegrees, 6); outputSerial.print(",");
        outputSerial.print((int32_t)last_gps.satellites); outputSerial.print(",");
        outputSerial.print(last_gps.speed, 3); outputSerial.print(",");
        outputSerial.print(last_gps.angle, 3); outputSerial.print(",");
        outputSerial.print(last_gps.altitude, 3); outputSerial.print(",");
    } else {
        outputSerial.print("-1,No fix,-1,No fix,0,-1,-1,-1,");
    }
    outputSerial.print(output.bno_orientation.w, 5); outputSerial.print(",");
    outputSerial.print(output.bno_orientation.x, 5); outputSerial.print(",");
    outputSerial.print(output.bno_orientation.y, 5); outputSerial.print(",");
    outputSerial.print(output.bno_orientation.z, 5); outputSerial.print(",");
    outputSerial.print(output.bno_gyro.x, 5); outputSerial.print(",");
    outputSerial.print(output.bno_gyro.y, 5); outputSerial.print(",");
    outputSerial.print(output.bno_gyro.z, 5); outputSerial.print(",");
    outputSerial.print(output.bno_acc.x, 4); outputSerial.print(",");
    outputSerial.print(output.bno_acc.y, 4); outputSerial.print(",");
    outputSerial.print(output.bno_acc.z, 4); outputSerial.print(",");
    outputSerial.print(output.adxl_acc.x, 2); outputSerial.print(",");
    outputSerial.print(output.adxl_acc.y, 2); outputSerial.print(",");
    outputSerial.print(output.adxl_acc.z, 2); outputSerial.print(",");
    outputSerial.print(output.bmp_press, 6); outputSerial.print(",");
    outputSerial.print(output.bmp_alt, 4); outputSerial.print(",");
    outputSerial.print(output.lsm_temp, 2); outputSerial.print(",");
    outputSerial.print(output.adxl_temp, 2); outputSerial.print(",");
    outputSerial.print(output.bno_temp, 2); outputSerial.print(",");
    outputSerial.print(output.bmp_temp, 2); outputSerial.println();
    outputSerial.print(output.sensorStatus.test(0)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(1)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(2)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(3)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(4)); outputSerial.println();
    outputSerial.flush();

    return;
}

void FLIGHT::writeDEBUG(bool headers, Stream &outputSerial) {
    if(headers) {
        outputSerial.println(data_header);
        outputSerial.flush();
        return;
    }

    outputSerial.print("Uptime (ms): ");outputSerial.print(output.totalTime_ms); outputSerial.print(", \n");
    if(last_gps.fix) {
        outputSerial.print("GPS Latitude Degrees: ");outputSerial.print(last_gps.latitudeDegrees, 6); outputSerial.println(", ");
        outputSerial.print("GPS Longitude Degrees: ");outputSerial.print(last_gps.longitudeDegrees, 6); outputSerial.println(",");
        outputSerial.print("GPS satellites: ");outputSerial.print((int32_t)last_gps.satellites); outputSerial.print(",");
        outputSerial.print("GPS speed: ");outputSerial.print(last_gps.speed, 3); outputSerial.print(",");
        outputSerial.print("GPS angle: ");outputSerial.print(last_gps.angle, 3); outputSerial.print(",");
        outputSerial.print("GPS altitude: ");outputSerial.println(last_gps.altitude, 3); outputSerial.println();
    } else {
        outputSerial.println("-1,No fix,-1,No fix,0,-1,-1,-1,\n");
    }
    //BNO data
        //orientation
    outputSerial.print("BNO W-Orientation: ");outputSerial.print(output.bno_orientation.w, 5); outputSerial.print(",");
    outputSerial.print("BNO X-Orientation: ");outputSerial.print(output.bno_orientation.x, 5); outputSerial.print(",");
    outputSerial.print("BNO Y-Orientation: ");outputSerial.print(output.bno_orientation.y, 5); outputSerial.print(",");
    outputSerial.print("BNO Z-Orientation: ");outputSerial.print(output.bno_orientation.z, 5); outputSerial.println(",");
        //gyro
    outputSerial.print("BNO X-Gyro: ");outputSerial.print(output.bno_gyro.x, 5); outputSerial.print(",");
    outputSerial.print("BNO Y-Gyro: ");outputSerial.print(output.bno_gyro.y, 5); outputSerial.print(",");
    outputSerial.print("BNO Z-Gyro: ");outputSerial.print(output.bno_gyro.z, 5); outputSerial.println(",");
        //Accel
    outputSerial.print("BNO X-Accel: ");outputSerial.print(output.bno_acc.x, 4); outputSerial.print(",");
    outputSerial.print("BNO Y-Accel: ");outputSerial.print(output.bno_acc.y, 4); outputSerial.print(",");
    outputSerial.print("BNO Z-Accel: ");outputSerial.print(output.bno_acc.z, 4); outputSerial.println(",");

    //ADXL data
    outputSerial.print("ADXL X_Accel: ");outputSerial.print(output.adxl_acc.x, 2); outputSerial.print(",");
    outputSerial.print("ADXL Y_Accel: ");outputSerial.print(output.adxl_acc.y, 2); outputSerial.print(",");
    outputSerial.print("ADXL Z_Accel: ");outputSerial.print(output.adxl_acc.z, 2); outputSerial.println(",");

    //BMP data
    outputSerial.print("BMP Pressure: ");outputSerial.print(output.bmp_press, 6); outputSerial.print(",");
    outputSerial.print("BMP Altitude: ");outputSerial.print(output.bmp_alt, 4); outputSerial.println(",");

    //Temperature data
    outputSerial.print("LSM Temp: ");outputSerial.print(output.lsm_temp, 2); outputSerial.print(",");
    outputSerial.print("ADXL Temp: ");outputSerial.print(output.adxl_temp, 2); outputSerial.print(",");
    outputSerial.print("BNO Temp: ");outputSerial.print(output.bno_temp, 2); outputSerial.print(",");
    outputSerial.print("BMP Temp: ");outputSerial.print(output.bmp_temp, 2); outputSerial.println("\n");

    //Sensor status
    outputSerial.println("Sensor Status:");
    outputSerial.print(output.sensorStatus.test(0)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(1)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(2)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(3)); outputSerial.print(", ");
    outputSerial.print(output.sensorStatus.test(4)); outputSerial.println("\n");
    outputSerial.flush();

    return;
}

void FLIGHT::writeDataToTeensy(Stream &outputSerial) {
    TransmitFlightData transfer = prepareToTransmit(output);
    myTransfer.sendDatum(transfer);
}

void FLIGHT::readDataFromTeensy(Stream &inputSerial) {
    TransmitFlightData receiveStruct;
    if(myTransfer.available()) {
        myTransfer.rxObj(receiveStruct);
    }
    output = decodeTransmission(receiveStruct);
}

void FLIGHT::initTransferSerial(Stream &transferSerial) {
    myTransfer.begin(transferSerial);
}

FlightData FLIGHT::decodeTransmission(TransmitFlightData s) {
    return {
        s.lsm_gyro, s.lsm_acc,
        s.adxl_acc,
        s.bno_gyro, s.bno_acc, s.bno_mag,
        s.bno_orientation,
        s.lsm_temp, s.adxl_temp, s.bno_temp,
        s.bmp_temp, s.bmp_press, s.bmp_alt
    };
}

TransmitFlightData FLIGHT::prepareToTransmit(FlightData s) {
    return {
        s.lsm_gyro, s.lsm_acc,
        s.adxl_acc,
        s.bno_gyro, s.bno_acc, s.bno_mag,
        s.bno_orientation,
        s.lsm_temp, s.adxl_temp, s.bno_temp,
        s.bmp_temp, s.bmp_press, s.bmp_alt
    };