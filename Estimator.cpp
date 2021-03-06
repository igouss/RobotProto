#include <math.h>
#include <WConstants.h>
#include "Estimator.h"

namespace naxsoft {

void Estimator::estimate(double x_n_plus, double K, double *mu, double *sigma_sqr_n) {
   double mu_n_plus = *mu + K * (x_n_plus - *mu);

/*
   Serial.print(*mu);
   Serial.print(" + ");
   Serial.print(K);
   Serial.print(" * ( ");
   Serial.print(x_n_plus);
   Serial.print(" - ");
   Serial.print(*mu);
   Serial.print(" ) = ");
   Serial.println(mu_n_plus);
*/

   double sigma_sqr_n_prime = *sigma_sqr_n + K * pow((x_n_plus - *mu), 2);
   double sigma_sqr_n_plus = (1 - K) * sigma_sqr_n_prime;

   *mu = mu_n_plus;
   *sigma_sqr_n = sigma_sqr_n_plus;
}

}
