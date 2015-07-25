#include "TelemetryStreamParser.h"
#include "SPortMeasurements.h"
#include "Utils.h"

#define TELEMETRY_PACKET_SEP  0x7E
#define TELEMETRY_PACKET_STUFF  0x7D
#define TELEMETRY_PACKET_STUFF_XOR 0x20
#define TELEMETRY_PACKET_LENGTH  9

#define DATA_FRAME              0x10

#define ALT_FIRST_ID            0x0100
#define ALT_LAST_ID             0x010f
#define VARIO_FIRST_ID          0x0110
#define VARIO_LAST_ID           0x011f
#define CURR_FIRST_ID           0x0200
#define CURR_LAST_ID            0x020f
#define VFAS_FIRST_ID           0x0210
#define VFAS_LAST_ID            0x021f
#define CELLS_FIRST_ID          0x0300
#define CELLS_LAST_ID           0x030f
#define T1_FIRST_ID             0x0400
#define T1_LAST_ID              0x040f
#define T2_FIRST_ID             0x0410
#define T2_LAST_ID              0x041f
#define RPM_FIRST_ID            0x0500
#define RPM_LAST_ID             0x050f
#define FUEL_FIRST_ID           0x0600
#define FUEL_LAST_ID            0x060f
#define ACCX_FIRST_ID           0x0700
#define ACCX_LAST_ID            0x070f
#define ACCY_FIRST_ID           0x0710
#define ACCY_LAST_ID            0x071f
#define ACCZ_FIRST_ID           0x0720
#define ACCZ_LAST_ID            0x072f
#define GPS_LONG_LATI_FIRST_ID  0x0800
#define GPS_LONG_LATI_LAST_ID   0x080f
#define GPS_ALT_FIRST_ID        0x0820
#define GPS_ALT_LAST_ID         0x082f
#define GPS_SPEED_FIRST_ID      0x0830
#define GPS_SPEED_LAST_ID       0x083f
#define GPS_COURS_FIRST_ID      0x0840
#define GPS_COURS_LAST_ID       0x084f
#define GPS_TIME_DATE_FIRST_ID  0x0850
#define GPS_TIME_DATE_LAST_ID   0x085f
#define A3_FIRST_ID             0x0900
#define A3_LAST_ID              0x090f
#define A4_FIRST_ID             0x0910
#define A4_LAST_ID              0x091f
#define AIR_SPEED_FIRST_ID      0x0a00
#define AIR_SPEED_LAST_ID       0x0a0f
#define RSSI_ID                 0xf101
#define ADC1_ID                 0xf102
#define ADC2_ID                 0xf103
#define SP2UART_A_ID            0xfd00
#define SP2UART_B_ID            0xfd01
#define BATT_ID                 0xf104
#define SWR_ID                  0xf105
#define XJT_VERSION_ID          0xf106
#define FUEL_QTY_FIRST_ID       0x0a10
#define FUEL_QTY_LAST_ID        0x0a1f

#define SPORT_DATA_U8(packet)   (packet[4])
#define SPORT_DATA_S32(packet)  (*((int32_t *)(packet+4)))
#define SPORT_DATA_U32(packet)  (*((uint32_t *)(packet+4)))
#define HUB_DATA_U16(packet)    (*((uint16_t *)(packet+4)))

TelemetryStreamParser::TelemetryStreamParser() : msrReadCallback(0), telemetry_p(telemetry_buffer), telemetry_status(Idle) {}


void TelemetryStreamParser::updateMeasurementFromSportPacket()
{
	uint8_t  dataId = (telemetry_buffer[0] & 0x1F) + 1;
	uint8_t  prim = telemetry_buffer[1];
	uint16_t appId = *((uint16_t *)(telemetry_buffer + 2));
	
	if (!checkSportPacketCRC(telemetry_buffer)) {
		log_debug("Packet CRC failed!");
		return;
	}
	if (prim == DATA_FRAME) {
		log_debug("New frame: sensor: %x, id: %x\n", dataId, appId);
		if (appId == RSSI_ID) {
			updateRssiFromSport(measurement, SPORT_DATA_U8(telemetry_buffer));
		}
		else  if (appId >= GPS_ALT_FIRST_ID && appId <= GPS_ALT_LAST_ID) {
			updateGpsAltitudeFromSport(measurement, SPORT_DATA_S32(telemetry_buffer));

		}
		else if (appId >= GPS_TIME_DATE_FIRST_ID && appId <= GPS_TIME_DATE_LAST_ID) {
			updateGpsDateTimeFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));

		}
		else if (appId >= GPS_LONG_LATI_FIRST_ID && appId <= GPS_LONG_LATI_LAST_ID) {
			updateGpsPosFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= CURR_FIRST_ID && appId <= CURR_LAST_ID) {
			updateCurrentFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= CELLS_FIRST_ID && appId <= CELLS_LAST_ID) {
			updateBatteryCellVoltageFromSport(measurement, SPORT_DATA_U32(telemetry_buffer), 0);
			// Packet might have two cell values:
			if (measurement.data.batteryVoltage.cellIndex + 1 < measurement.data.batteryVoltage.cellCount) {
				measurement.sensorId = dataId;
				if (msrReadCallback) {
					msrReadCallback(measurement);
				}
				updateBatteryCellVoltageFromSport(measurement, SPORT_DATA_U32(telemetry_buffer), 1);
			}
		}
		else if (appId >= T1_FIRST_ID && appId <= T2_LAST_ID) {
			updateTemperatureFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= RPM_FIRST_ID && appId <= RPM_LAST_ID) {
			updateRpmFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= FUEL_FIRST_ID && appId <= FUEL_LAST_ID) {
			updateFuelFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= ALT_FIRST_ID && appId <= ALT_LAST_ID) {
			updateBaroAltitudeFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= ACCX_FIRST_ID && appId <= ACCX_LAST_ID) {
			updateAccelXFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= ACCY_FIRST_ID && appId <= ACCY_LAST_ID) {
			updateAccelYFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= ACCZ_FIRST_ID && appId <= ACCZ_LAST_ID) {
			updateAccelZFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= AIR_SPEED_FIRST_ID && appId <= AIR_SPEED_LAST_ID) {
			updateAirSpeedFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= GPS_SPEED_FIRST_ID && appId <= GPS_SPEED_LAST_ID) {
			updateGpsSpeedFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else if (appId >= GPS_COURS_FIRST_ID && appId <= GPS_COURS_LAST_ID) {
			updateGpsCourseFromSport(measurement, SPORT_DATA_U32(telemetry_buffer));
		}
		else {
			log_debug(" Unkown frame id : %x\n", appId);
			return;
		}

		measurement.sensorId = dataId;

		if (msrReadCallback) {
			msrReadCallback(measurement);
		}
	}
}

bool TelemetryStreamParser::checkSportPacketCRC(uint8_t *packet)
{
	short crc = 0;
	for (int i = 1; i<TELEMETRY_PACKET_LENGTH; ++i) {
		crc += packet[i]; // 0-1FE
		crc += crc >> 8;  // 0-1FF
		crc &= 0x00ff;    // 0-FF
	}
	// TRACE("crc: 0x%02x", crc);
	char buf[32];
	sprintf(buf, "CRC: %x", crc);
	//Serial.print(buf);
	return (crc == 0x00ff);
}

void TelemetryStreamParser::parseByte(uint8_t next)
{
	switch (telemetry_status) {
		case Idle:
			if (next == TELEMETRY_PACKET_SEP) {
				telemetry_status = Parsing;
				telemetry_p = telemetry_buffer;
			}
			break;

		case Start:
			if (next == TELEMETRY_PACKET_SEP) {
				telemetry_status = Parsing;
			}
			else {
				*telemetry_p++ = next;
			}
			break;

		case Parsing:
			if (next == TELEMETRY_PACKET_STUFF) {
				telemetry_status = Stuffing;
				break;
			}
			if (next == TELEMETRY_PACKET_SEP) {
				telemetry_p = telemetry_buffer;
				break;
			}
			else {
				*telemetry_p++ = next;
			}
			break;

		case Stuffing:
			*telemetry_p++ = next^TELEMETRY_PACKET_STUFF_XOR;
			telemetry_status = Parsing;
			break;

	}
	if (telemetry_p - telemetry_buffer >= TELEMETRY_PACKET_LENGTH) {
		updateMeasurementFromSportPacket();
		telemetry_status = Idle;
		telemetry_p = telemetry_buffer;
	}
}