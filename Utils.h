#ifndef __UTILS_H__
#define __UTILS_H__


#ifdef DEBUG
#define log_debug(fmt,...)  format_message_to_serial(fmt, __VA_ARGS__)
#else
#define log_debug(fmt,...)
#endif

#define log_notice(fmt,...) format_message_to_serial(fmt, __VA_ARGS__)

void format_message_to_serial(const char* fmt, ...);

#endif