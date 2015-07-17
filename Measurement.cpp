#include "Measurement.h"
#include "Utils.h"

Measurement::Measurement() :type(LAST), sensorId(-1) {}


void Measurement::setSensorId(uint8_t sensorId_)
{
	sensorId = sensorId_;
}

uint8_t Measurement::getSensorId() const
{
	return sensorId;
}

const Measurement::Data& Measurement::getData() const 
{ 
	return data;
}

void Measurement::updateRssi(uint8_t rssi)
{
	type = RSSI;
	data.rssi = rssi;  
}
void Measurement::updateGpsAltitude(int32_t altitude)
{
	type = GPS_ALTITUDE;
	data.genericValue = altitude;
}

void Measurement::updateGpsPosFromSport(uint32_t packet)
{
	uint32_t gps_long_lati_b1w, gps_long_lati_a1w;
	gps_long_lati_b1w = (packet & 0x3fffffff) / 10000;
	gps_long_lati_a1w = (packet & 0x3fffffff) % 10000;
	switch ((packet & 0xc0000000) >> 30) {
		case 0:
			type = GPS_LATITUDE;
			data.genericValue = (((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
		case 1:
			type = GPS_LATITUDE;
			data.genericValue = -(((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
		case 2:
			type = GPS_LONGITUDE;
			data.genericValue = (((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
		case 3:
			type = GPS_LONGITUDE;
			data.genericValue = -(((gps_long_lati_b1w / 60L * 100L) + (gps_long_lati_b1w % 60L)) * 10000L + gps_long_lati_a1w);
			break;
	}
}

void Measurement::updateGpsDateTimeFromSport(uint32_t packet)
{

	uint16_t year, sec;
	uint8_t month, day, hour, min;
	if (packet & 0x000000ff) {
		type = GPS_DATE;
		data.gpsDate.year = (uint16_t)((packet & 0xff000000) >> 24);
		data.gpsDate.month = (uint8_t)((packet & 0x00ff0000) >> 16);
		data.gpsDate.day = (uint8_t)((packet & 0x0000ff00) >> 8);
	}
	else {
		type = GPS_TIME;
		data.gpsTime.hour = (uint8_t)((packet & 0xff000000) >> 24);
		data.gpsTime.min = (uint8_t)((packet & 0x00ff0000) >> 16);
		data.gpsTime.sec = (uint16_t)((packet & 0x0000ff00) >> 8);
	}
}

void Measurement::updateCurrent(uint32_t packet)
{
	type = CURRENT;
	data.genericValue = packet;

}
void Measurement::updateBatteryCellVoltage(uint32_t packet, uint8_t cellIndex)
{
	type = BATTERY_CELL_VOLTAGE;

	data.batteryVoltage.cellCount = (packet & 0xF0) >> 4;
	data.batteryVoltage.cellIndex = (packet & 0x0F);
	if (cellIndex == 0) {
		data.batteryVoltage.cellValue = (((packet & 0x000FFF00) >> 8) / 5);
	}
	else if (cellIndex == 1) {
		data.batteryVoltage.cellIndex += 1;
		data.batteryVoltage.cellValue = (((packet & 0xFFF00000) >> 20) / 5);
	}
	else {
		log_debug("Max two cells in one frame!");
	}
	
}


void Measurement::updateTemperature(int32_t packet)
{
	type = TEMPERATURE;
	data.genericValue = packet;
}
void Measurement::updateRpm(uint32_t packet)
{
	type = RPM;
	data.genericValue = packet;
}
void Measurement::updateFuel(uint32_t packet)
{
	type = FUEL;
	data.genericValue = packet;
}

void Measurement::updateBaroAltitude(int32_t packet)
{
	type = BARO_ALTITUDE;
	data.genericValue = packet;
}

void Measurement::updateAccelX(int32_t packet)
{
	type = ACCELERATION_X;
	data.genericValue = packet;
}
void Measurement::updateAccelY(int32_t packet)
{
	type = ACCELERATION_Y;
	data.genericValue = packet;
}
void Measurement::updateAccelZ(int32_t packet)
{
	type = ACCELERATION_Z;
	data.genericValue = packet;
}

void Measurement::updateAirSpeed(uint32_t packet)
{
	type = AIR_SPEED;
	data.genericValue = packet;
}
void Measurement::updateGpsSpeed(uint32_t packet)
{
	type = GPS_SPEED;
	data.genericValue = packet;
}
void Measurement::updateGpsCourse(uint32_t packet)
{
	type = GPS_COURSE;
	data.genericValue = packet;
}