#ifndef __TELEMETRY_STREAM_PARSER_H__
#define __TELEMETRY_STREAM_PARSER_H__

#include "Measurement.h"


// FrSky telemetry stream parser, currently only supports digital sensors over SPort protocol
// TODO: support for all sensor types, including old hub sensors. Support for old protocol over RS232
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
