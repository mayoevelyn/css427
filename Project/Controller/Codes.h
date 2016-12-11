// ----------------------------------------------------------------------------
// CSS 427 - Embedded Systems
// Author:  Thomas Dye, John Walter - 12/9/2016
// School:  University of Washington | Bothell
// Instructor:  Dr. Peng Yang
//
// Controller  - Command Codes
// ----------------------------------------------------------------------------

#ifndef CODES_H
#define CODES_H

// Communication Command Codes
const byte C_ACK = 0x00;
const byte C_SUCCESS = 0x01;
const byte C_FAILURE = 0x02;
const byte C_GET_VALVE_STATE = 0x10;
const byte C_SET_OPEN_VALVE = 0x11;
const byte C_SET_CLOSE_VALVE = 0x12;
const byte C_SET_TOGGLE_VALVE = 0x13;
const byte C_SET_TIME = 0x14;
const byte C_GET_TIME = 0x15;
const byte C_GET_ZONE_SENSORS = 0x16;
const byte C_GET_ALL_SENSORS = 0x17;
const byte C_GET_SCHEDULE = 0x18;
const byte C_SET_SCHEDULE = 0x19;
const byte C_VALVE_DATA = 0x30;
const byte C_TIME_DATA = 0x31;
const byte C_SENSOR_DATA = 0x32;

#endif
