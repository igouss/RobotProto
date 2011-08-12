#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

namespace naxsoft {
	typedef struct {
		int x; // pitch
		int y; // roll
		int z; // yaw
	} gyroscope_data;

	class Gyroscope {
	public:
		int setupL3G4200D(int scale);
		void getGyroValues(gyroscope_data* data);
	};

	extern Gyroscope gyroscope;
} // namespace

#endif /* GYROSCOPE_H_ */
