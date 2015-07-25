#ifndef __SPORT_MEASUREMENTS_H__
#define __SPORT_MEASUREMENTS_H__

void updateRssiFromSport(Measurement& msr, uint8_t rssi);

void updateGpsPosFromSport(Measurement& msr, uint32_t packet);
void updateGpsAltitudeFromSport(Measurement& msr, int32_t altitude);
void updateGpsDateTimeFromSport(Measurement& msr, uint32_t packet);
void updateCurrentFromSport(Measurement& msr, uint32_t packet);
void updateBatteryCellVoltageFromSport(Measurement& msr, uint32_t packet, uint8_t cellIndex);
void updateTemperatureFromSport(Measurement& msr, int32_t packet);
void updateRpmFromSport(Measurement& msr, uint32_t packet);
void updateFuelFromSport(Measurement& msr, uint32_t packet);

void updateBaroAltitudeFromSport(Measurement& msr, int32_t packet);

void updateAccelXFromSport(Measurement& msr, int32_t packet);
void updateAccelYFromSport(Measurement& msr, int32_t packet);
void updateAccelZFromSport(Measurement& msr, int32_t packet);

void updateAirSpeedFromSport(Measurement& msr, uint32_t packet);
void updateGpsSpeedFromSport(Measurement& msr, uint32_t packet);
void updateGpsCourseFromSport(Measurement& msr, uint32_t packet);

#endif