#include "stdio.h"
#include <stdarg.h>
#include "HardwareSerial.h"

void format_message_to_serial(const char* fmt, ...)
{
	va_list argp;
	va_start(argp, fmt);
	char buffer[64];
	vsnprintf(buffer, 64, fmt, argp);
	Serial.write(buffer);
	va_end(argp);
}