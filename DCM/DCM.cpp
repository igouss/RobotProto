#include "DCM.h"
/*
void DCM::updateR(double o, double w, double f) {
	DCM::R[0][0] = cos(o) * cos(w);
	DCM::R[0][1] = sin(f) * sin(o) * cos(w) - cos(f) * sin(w);
	DCM::R[0][2] = cos(f) * sin(o) * cos(w) + sin(f) * sin(w);

	DCM::R[1][0] = cos(o) * sin(w);
	DCM::R[1][1] = sin(f) * sin(o) * sin(w) + cos(f) * cos(w);
	DCM::R[1][2] = cos(f) * sin(o) * sin(w) - sin(f) * cos(w);

	DCM::R[2][0] = -sin(o);
	DCM::R[2][1] = sin(f) * cos(o);
	DCM::R[2][2] = cos(f) * cos(o);
}
*/

/**
 * Qg = R*Qp
 */
void DCM::computeQgFromQp() {
	Qg[0] = R[0][0] * Qp[0] + R[0][1] * Qp[1] + R[0][2] * Qp[2]; // Qg_x = r_xx * Qp_x + r_xy*Qp_y + r_xz*Qp_z
	Qg[1] = R[1][0] * Qp[0] + R[1][1] * Qp[1] + R[1][2] * Qp[2]; // Qg_x = r_yx * Qp_x + r_yy*Qp_y + r_yz*Qp_z
	Qg[2] = R[2][0] * Qp[0] + R[2][1] * Qp[1] + R[2][2] * Qp[2]; // Qg_x = r_zx * Qp_x + r_zy*Qp_y + r_zz*Qp_z
}

/**
 * Qp = R^t*Qg
 */
void DCM::computeQpFromQg() {
	Qp[0] = R[0][0] * Qg[0] + R[1][0] * Qg[1] + R[2][0] * Qg[2]; // Qg_x = r_xx * Qg_x + r_yx*Qg_y + r_zx*Qg_z
	Qp[1] = R[0][1] * Qg[0] + R[1][1] * Qg[1] + R[2][1] * Qg[2]; // Qg_x = r_xy * Qg_x + r_yy*Qg_y + r_xy*Qg_z
	Qp[2] = R[0][2] * Qg[0] + R[1][2] * Qg[1] + R[2][2] * Qg[2]; // Qg_x = r_xz * Qg_x + r_yz*Qg_y + r_zz*Qg_z
}
