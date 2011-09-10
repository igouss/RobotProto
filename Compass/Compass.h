#ifndef COMPASS_H_
#define COMPASS_H_

#include <inttypes.h>

namespace naxsoft {
typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} compass_data;

/*
 *  The LSM303DLH is a 5 x 5 x 1 mm with LGA-28L package IC chip that includes a 3D digital
 * linear acceleration and a 3D digital magnetic sensor. It has a selectable linear acceleration
 * full scale range of ±2g / ±4g / ±8g and a selectable magnetic field full scale range of ±1.3 /
 * ±1.9 / ±2.5 / ±4.0 / ±4.7 / ±5.6 / ±8.1 gauss
 */
class Compass {
public:
	void init();
	void read(compass_data* data);
private:
	bool isDataAvailable();
};

extern Compass compass;
} // namespace

#endif /* COMPASS_H_ */
