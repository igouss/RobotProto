#ifndef COMPASS_H_
#define COMPASS_H_

namespace naxsoft {
	typedef struct {
		int x;
		int y;
		int z;
	} compass_data;

	class Compass {
	public:
		void init();
		void read(compass_data* data);
	};

	extern Compass compass;
} // namespace

#endif /* COMPASS_H_ */
