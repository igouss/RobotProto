#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

namespace naxsoft {
	typedef struct {
		int x;
		int y;
		int z;
	} gyroscope_data;

	class Gyroscope {
	public:
		int setupL3G4200D(int scale);
		void getGyroValues(gyroscope_data* data);
	private:
		int readRegister(int deviceAddress, byte address);
		void writeRegister(int deviceAddress, byte address, byte val);
	};

	extern Gyroscope gyroscope;
} // namespace

#endif /* GYROSCOPE_H_ */