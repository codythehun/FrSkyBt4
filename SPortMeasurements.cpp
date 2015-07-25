#include "Measurement.h"
#include "SPortMeasurements.h"
#include "Utils.h"


void updateRssiFromSport(Measurement& msr, uint8_t rssi)
{
	msr.type = Measurement::RSSI;
	msr.data.rssi = rssi;  
}
void updateGpsAltitudeFromSport(Measurement& msr, int32_t altitude)
{
	msr.type = Measurement::GPS_ALTITUDE;
	msr.data.genericValue = altitude;
}

void updateGpsPosFromSport(Measurement& msr, uint32_t packet)
{
	uint32_t gps_long_lati_b1w, gps_long_lati_a1w;
	gps_long_lati_b1w = (packet & 0x3fffffff) / 10000;
	gps_long_lati_a1w = (packet & 0x3fffffff) % 10000;
	switch ((packet & 0xc0000000) >> 30) {
		case 0:
			msr.type = Measurement::GPS_LATITUDE;
			msr.data.genericValue = (((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
		case 1:
			msr.type = Measurement::GPS_LATITUDE;
			msr.data.genericValue = -(((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
		case 2:
			msr.type = Measurement::GPS_LONGITUDE;
			msr.data.genericValue = (((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
		case 3:
			msr.type = Measurement::GPS_LONGITUDE;
			msr.data.genericValue = -(((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
	}
}

void updateGpsDateTimeFromSport(Measurement& msr, uint32_t packet)
{

	uint16_t year, sec;
	uint8_t month, day, hour, min;
	if (packet & 0x000000ff) {
		msr.type = Measurement::GPS_DATE;
		msr.data.gpsDate.year = (uint16_t)((packet & 0xff000000) >> 24);
		msr.data.gpsDate.month = (uint8_t)((packet & 0x00ff0000) >> 16);
		msr.data.gpsDate.day = (uint8_t)((packet & 0x0000ff00) >> 8);
	}
	else {
		msr.type = Measurement::GPS_TIME;
		msr.data.gpsTime.hour = (uint8_t)((packet & 0xff000000) >> 24);
		msr.data.gpsTime.min = (uint8_t)((packet & 0x00ff0000) >> 16);
		msr.data.gpsTime.sec = (uint16_t)((packet & 0x0000ff00) >> 8);
	}
}

void updateCurrentFromSport(Measurement& msr, uint32_t packet)
{
	msr.type = Measurement::CURRENT;
	msr.data.genericValue = packet;

}
void updateBatteryCellVoltageFromSport(Measurement& msr, uint32_t packet, uint8_t cellIndex)
{
	msr.type = Measurement::BATTERY_CELL_VOLTAGE;

	msr.data.batteryVoltage.cellCount = (packet & 0xF0) >> 4;
	msr.data.batteryVoltage.cellIndex = (packet & 0x0F);
	if (cellIndex == 0) {
		msr.data.batteryVoltage.cellValue = (((packet & 0x000FFF00) >> 8) / 5);
	}
	else if (cellIndex == 1) {
		msr.data.batteryVoltage.cellIndex += 1;
		msr.data.batteryVoltage.cellValue = (((packet & 0xFFF00000) >> 20) / 5);
	}
	else {
		log_debug("Max two cells in one frame!");
	}
	
}


void updateTemperatureFromSport(Measurement& msr, int32_t packet)
{
	msr.type = Measurement::TEMPERATURE;
	msr.data.genericValue = packet;
}
void updateRpmFromSport(Measurement& msr, uint32_t packet)
{
	msr.type = Measurement::RPM;
	msr.data.genericValue = packet;
}
void updateFuelFromSport(Measurement& msr, uint32_t packet)
{
	msr.type = Measurement::FUEL;
	msr.data.genericValue = packet;
}

void updateBaroAltitudeFromSport(Measurement& msr, int32_t packet)
{
	msr.type = Measurement::BARO_ALTITUDE;
	msr.data.genericValue = packet;
}

void updateAccelXFromSport(Measurement& msr, int32_t packet)
{
	msr.type = Measurement::ACCELERATION_X;
	msr.data.genericValue = packet;
}
void updateAccelYFromSport(Measurement& msr, int32_t packet)
{
	msr.type = Measurement::ACCELERATION_Y;
	msr.data.genericValue = packet;
}
void updateAccelZFromSport(Measurement& msr, int32_t packet)
{
	msr.type = Measurement::ACCELERATION_Z;
	msr.data.genericValue = packet;
}

void updateAirSpeedFromSport(Measurement& msr, uint32_t packet)
{
	msr.type = Measurement::AIR_SPEED;
	msr.data.genericValue = packet;
}
void updateGpsSpeedFromSport(Measurement& msr, uint32_t packet)
{
	msr.type = Measurement::GPS_SPEED;
	msr.data.genericValue = packet;
}
void updateGpsCourseFromSport(Measurement& msr, uint32_t packet)
{
	msr.type = Measurement::GPS_COURSE;
	msr.data.genericValue = packet;
}