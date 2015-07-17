#ifndef __TELEMETRY_STREAM_PARSER_H__
#define __TELEMETRY_STREAM_PARSER_H__

#include "Measurement.h"


// Discover sensors, warmup period, or even during. Set them in a bitmap. bt characteristic for available sensors
class TelemetryStreamParser {
public:
	typedef void(*MeasurementReadCallback)(const Measurement& msr);
private:
	Measurement measurement;

	MeasurementReadCallback msrReadCallback;

	uint8_t telemetry_buffer[20];
	uint8_t* telemetry_p;

	enum TelemetryParseStatus { Idle, Start, Parsing, Stuffing } telemetry_status;

	bool checkSportPacketCRC(uint8_t *packet);

	void updateMeasurementFromSportPacket();

public:
	TelemetryStreamParser();

	void setMeasurementReadCallback(MeasurementReadCallback msrReadCallback_) { msrReadCallback = msrReadCallback_; }

	void parseByte(uint8_t next);



};

#endif
