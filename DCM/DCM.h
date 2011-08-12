#ifndef DCM_H_
#define DCM_H_


namespace naxsoft {

class DCM {
public:
	/**
	 * direction, velocity, acceleration, measured in the frame of reference of the moving object
	 *
	 * Qp = | Qp_x |
	 *      | Qp_y |
	 *      | Qp_z |
	 */
	static double Qp[3];

	/**
	 * direction, velocity, acceleration, measured in the frame of reference of the ground
	 *
	 * Qg = | Qg_x |
	 *      | Qg_y |
	 *      | Qg_z |
	 */
	static double Qg[3];

	/**
	 * Rotation Matrix.
	 * Direction Cosine Matrix.
	 *
	 *  R = | r_xx r_xy r_xz |
	 *      | r_yx r_yy r_yz |
	 *      | r_zx r_zy r_zz |
	 */
	static double R[3][3];

public:
	/**
	 * Qg = R*Qp
	 */
	static void computeQgFromQp();
	/**
	 * Qp = R^t*Qg
	 */
	static void computeQpFromQg();
};

} // namespace
#endif /* DCM_H_ */
