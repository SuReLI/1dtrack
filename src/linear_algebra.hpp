#ifndef LINEAR_ALGEBRA_HPP_
#define LINEAR_ALGEBRA_HPP_

//#include <Eigen/Dense>

/**
 * @brief 1D Mahalanobis distance
 *
 * Compute the Mahalanobis distance between a scalar and a scalar distribution.
 * An additional parameter is added: the precision with which the variance is considered to be
 * zero.
 * If the variance is zero, a 'big' value is returned.
 * @param {double} v; input scalar
 * @param {double} mean; mean of the distribution or estimator
 * @param {double} var; variance of the distribution or estimator
 * @param {double} precision; precision of the inverse
 * @return Return the 1D Mahalanobis distance.
 */
double mahalanobis_distance_1d(
	double v,
	double mean,
	double var,
	double precision = 1e-30)
{
	if(is_equal_to(v,mean)) {
		return 0.;
	}
	if(is_equal_to(var,0.,precision)) {
		return 9e99;
	} else {
		return sqrt(pow(v-mean,2.) / var);
	}
}

/**
 * @brief 1D Mahalanobis distance
 *
 * Compute the Mahalanobis distance between a scalar and a scalar distribution.
 * Make use of estimators for the mean and standard deviation.
 * An additional parameter is added: the precision with which the variance is considered to be
 * zero.
 * If the variance is zero, a 'big' value is returned.
 * @param {double} v; input scalar
 * @param {std::vector<double> &} data; input scalar data set
 * @return Return the 1D Mahalanobis distance.
 */
double mahalanobis_distance_1d(
	double v,
	std::vector<double> &data,
	double precision = 1e-30)
{/*
	double mean = 1d_mean_estimator(data);
	double var  = 1d_variance_estimator(data,mean);
	return mahalanobis_distance_1d(v,mean,var,precision);*/
}

#endif // LINEAR_ALGEBRA_HPP_
