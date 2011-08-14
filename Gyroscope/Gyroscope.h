#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

namespace naxsoft {

	/*
	 * L3G4200D gyroscope data
	 * brief structure containing gyroscope values for yaw, pitch and roll in
	 * signed short
	 */
	typedef struct {
		int16_t y; // pitch data. Range -2048 to 2047.
		int16_t p; // roll data. Range -2048 to 2047.
		int16_t r; // yaw data. Range -2048 to 2047.
	} gyroscope_data;

	class Gyroscope {
	public:
		int setupL3G4200D(int scale);
		void getGyroValues(gyroscope_data* data);
		int16_t getTemperature();
	};

	extern Gyroscope gyroscope;
} // namespace

#endif /* GYROSCOPE_H_ */
