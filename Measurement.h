#ifndef __SENSORS_H__
#define __SENSORS_H__

#define MAX_SENSOR_INSTANCES 2

#include "Arduino.h"

class __attribute__((packed)) Measurement{
public:
	enum Type {
		RSSI = 0, // link quality // unit: raw
		BATTERY_CELL_VOLTAGE, // voltage of each cell // prec: 2 // unit: Volt
		TEMPERATURE, // temperature measurement, prec: 0 // unit: Celsius
		RPM, //p rec: 0 
		FUEL, // prec: 0 // unit: percent
		BARO_ALTITUDE, // prec: 2 // unit: meter
		ACCELERATION_X, // G sensor // prec: 2 // unit: G
		ACCELERATION_Y, 
		ACCELERATION_Z, 
		AIR_SPEED,  // prec: 1 // unit: m/s
		CURRENT, // ampers // prec: 1 // unit: Amper
		GPS_SPEED, // prec: 3 // unit: knots
		GPS_LATITUDE, // signed integer in format: +/-DDMMMMMM where the degree minutes decimal point is after the second digit so it translates to DD MM.MMMM
		GPS_LONGITUDE,
		GPS_ALTITUDE, // prec: 2 // unit: meter
		GPS_COURSE, // direction in degrees // prec: 2
		GPS_DATE,
		GPS_TIME, // utc
		LAST
	};

	struct __attribute__((packed)) BatteryCellVoltage{
		uint8_t cellCount; // total cell count in pack
		uint8_t cellIndex; 
		uint16_t cellValue;

	};

	struct __attribute__((packed)) GpsCoord{
		uint16_t year;
		uint8_t month;
		uint8_t day;
	};
	struct __attribute__((packed)) GpsDate{
		uint16_t year;
		uint8_t month;
		uint8_t day;
	};

	struct __attribute__((packed)) GpsTime{
		uint8_t hour;
		uint8_t min;
		uint16_t sec;
	};

	union Data {
		int32_t genericValue;
		uint8_t rssi;
		BatteryCellVoltage batteryVoltage;
		GpsTime gpsTime;
		GpsDate gpsDate;
	};


private:
	uint8_t type;
	uint8_t sensorId;
	Data data;

public:
	Measurement();

	uint8_t getType()const
	{
		return type;
	}

	void setSensorId(uint8_t sensorId_);

	uint8_t getSensorId() const;

	const Data& getData() const;

	void updateRssi(uint8_t rssi);

	void updateGpsPosFromSport(uint32_t packet);
	void updateGpsAltitude(int32_t altitude);
	void updateGpsDateTimeFromSport(uint32_t packet);
	void updateCurrent(uint32_t packet);
	void updateBatteryCellVoltage(uint32_t packet, uint8_t cellIndex);
	void updateTemperature(int32_t packet);
	void updateRpm(uint32_t packet);	
	void updateFuel(uint32_t packet);

	void updateBaroAltitude(int32_t packet);

	void updateAccelX(int32_t packet);
	void updateAccelY(int32_t packet);
	void updateAccelZ(int32_t packet);

	void updateAirSpeed(uint32_t packet);
	void updateGpsSpeed(uint32_t packet);
	void updateGpsCourse(uint32_t packet);





};



#endif