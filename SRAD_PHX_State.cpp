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
 * @brief gets flight state
 * 
 * The function uses a cascading switch case to determine which stage
 * of flight the rocket is in. At each stage, it calls a helper function
 * to determine if it should move to the next one.
 */
void FLIGHT::calculateState() {
    switch(STATE) {
        case(STATES::PRE_NO_CAL):
            if(calibrate()) {
                STATE = STATES::PRE_CAL;
            }
            break;

        case(STATES::PRE_CAL):
            if(isAscent()) {
                STATE = STATES::FLIGHT_ASCENT;
                AltitudeCalibrate(); // Recalibrating the altitude of the rocket when it is launching
            }
            break;

        case STATES::FLIGHT_ASCENT:
            if(isDescent()) {
                STATE = STATES::FLIGHT_DESCENT;
            }
            break;

        case STATES::FLIGHT_DESCENT:
            if(isLanded()) {
                STATE = STATES::POST_LANDED;
            }
            break;
    }
}
/**
 * Helper function to check if sensors are calibrated
 * @return returns true if sensors are calibrated
 */
bool FLIGHT::isCal() {
    return calibrated;
}

/**
 * Helper function to check if rocket is ascending
 * Fault tolerant for failure of LSM or ADXL.
 * 1. If LSM fails, default to ADXL
 * 2. If ADXL fails, default to BMP
 * @return returns true if rocket is ascending
 */
bool FLIGHT::isAscent() {
    static uint32_t liftoffTimer_ms;
    if(!output.sensorStatus.test(0)) {
        if(output.lsm_acc.z > accel_liftoff_threshold) {
            liftoffTimer_ms += deltaTime_ms;

            if(liftoffTimer_ms  > accel_liftoff_time_threshold) {
                return true;
            }
        } else {
            liftoffTimer_ms = 0;
        }
    } else if (output.adxl_acc.z > accel_liftoff_threshold) {  // if primary accel is known to be bad, check secondary
        if(output.lsm_acc.z > accel_liftoff_threshold) {
            liftoffTimer_ms += deltaTime_ms;

            if(liftoffTimer_ms  > accel_liftoff_time_threshold) {
                return true;
            }
        } else {
            liftoffTimer_ms = 0;
        }
    } else {  // if both accelerometers are bad, use altimeter
        // check if altitude is notably higher than 0 (or alt threshold)
    }
    return false;
}
bool FLIGHT::isDescent() {

}
bool FLIGHT::isLanded() {
    if (output.bmp_alt <=  output.offset_alt_fixed_temp + 10){ // if the current altitude is less than the offset altitude + 10m 
                                                                // then return True to indicate the rocket is landed
        return true;
    }
    else{
        return false;
    }
}

bool FLIGHT::calibrate() {
    // calibrate for GPS offset, possibly of the earth spinning?
    //
    // additionally calibrate altitude offset
}
bool FLIGHT::AltitudeCalibrate(){
    // save our current altitude
    // add the distance to the ground in
    // save to the altitude offset variable
 //offset = height from certain location to the sea (chart map) + height from the sensor to the bottom of the rocket

    const float fixed_offset = output.offset_alt_fixed_temp;//setting the offset to a separate constant variable
    while (output.bmp_alt >= fixed_offset){                 // if the current altidue is >/= to the constant offset 
    output.bmp_alt = output.bmp_alt - fixed_offset;         //calculate new altitude = current alt - offset
    break;                                                  // stop the loop so altitude does not keep subtracting offset
    //may be print the altitude or send it to the ground station or something?
    }

}