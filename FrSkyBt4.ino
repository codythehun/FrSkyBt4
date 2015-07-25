#include <SPI.h>
#include <RCTelemetry_BLE.h>
#include "Utils.h"
#include <SoftwareSerial.h>
#include "TelemetryStreamParser.h"


SoftwareSerial softSerial(4, 7, true);
TelemetryStreamParser telemParser;
//RCTelemetry_BLE ble(1,2,3);

void newSensorValue(const Measurement& msr)
{
	const Measurement::Data& data = msr.data;
	uint8_t sensorId = msr.sensorId;
	switch (msr.type) {
		case Measurement:: RSSI: log_notice("RSSI[%u]: %d\n", sensorId, data.rssi); break;
		case Measurement::GPS_DATE: log_notice("Gps Date[%u]: %u-%u-%u\n", sensorId, data.gpsDate.year, data.gpsDate.month, data.gpsDate.day); break;
		case Measurement::GPS_TIME: log_notice("Gps Time[%u]: %u:%u:%u\n", sensorId, data.gpsTime.hour, data.gpsTime.min, data.gpsTime.sec); break;
		case Measurement::GPS_LATITUDE: log_notice("Gps Lat[%u]: %ld\n", sensorId, data.genericValue); break;
		case Measurement::GPS_LONGITUDE: log_notice("Gps Lon[%u]: %ld\n", sensorId, data.genericValue); break;
		case Measurement::GPS_ALTITUDE: log_notice("Gps Alt[%u]: %ld\n", sensorId, data.genericValue); break;
		case Measurement::BATTERY_CELL_VOLTAGE:
			log_notice("Batt[%u] Cell_%u/%u: %u Volt\n", sensorId, data.batteryVoltage.cellIndex, data.batteryVoltage.cellCount, data.batteryVoltage.cellValue);
			break;
		case Measurement::BARO_ALTITUDE: log_notice("Altitude[%u]: %ld\n", sensorId, data.genericValue); break;
		case Measurement::CURRENT: log_notice("Current[%u]: %ld\n", sensorId, data.genericValue); break;

	}
}

void setup()
{
	softSerial.begin(57600);
	Serial.begin(57600);
	telemParser.setMeasurementReadCallback(newSensorValue);
	/* add setup code here */

}

void loop()
{
	if (softSerial.available()) {
		uint8_t data = softSerial.read();
		if (data >= 0) {
			telemParser.parseByte(data);

		}
	}

	/* add main program code here */

}
