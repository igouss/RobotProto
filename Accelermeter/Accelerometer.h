#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

namespace naxsoft {
	typedef struct {
		int x;
		int y;
		int z;
	} accelerometer_data;

	class Accelerometer {
	public:
		void init();
		void read(accelerometer_data* data);
	};

	extern Accelerometer accelerometer;
} // namespace
#endif /* ACCELEROMETER_H_ */
