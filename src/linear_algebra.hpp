#ifndef LINEAR_ALGEBRA_HPP_
#define LINEAR_ALGEBRA_HPP_

//#include <Eigen/Dense>

/**
 * @brief Scalar mean estimator
 *
 * Compute the mean estimator of the input scalar data set.
 * This method is implemented for 1 dimension.
 * @param {std::vector<double> &} data; input data set
 * @return Return the estimator of the mean scalar of the input data set.
 */
double mean1d_estimator(std::vector<double> &data) {
	assert(data.size() > 0);
	double m = 0.;
	for(auto &v : data) {
		m += v;
	}
	m /= ((double) data.size());
	return m;
}

/**
 * @brief Scalar variance estimator
 *
 * Compute the variance estimator of the input data set.
 * This method is implemented for 1 dimension.
 * Mean vector is given as an argument for faster computation in case it would
 * already have been calculated.
 * @param {std::vector<double> &} data; input scalar data set
 * @param {double} mean; mean of the distribution or estimator
 * @return Return the variance estimator of the input data set.
 */
double var1d_estimator(
	std::vector<double> &data,
	double mean)
{
	assert(data.size() > 0);
	double var = 0.;
	if(data.size() > 1) {
		for(auto &v : data) {
			var += pow(v-mean,2.);
		}
		var /= ((double) data.size() - 1.);
	}
	return var;
}

/**
 * @brief Scalar variance estimator
 *
 * Compute the variance estimator of the input data set.
 * This method is implemented for 1 dimension.
 * @param {std::vector<double> &} data; input data set
 * @return Return the variance estimator of the input data set.
 */
double var1d_estimator(std::vector<double> &data) {
	double mean = mean1d_estimator(data);
	return var1d_estimator(data,mean);
}

/**
 * @brief Scalar Mahalanobis distance
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
double mahalanobis1d_distance(
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
 * @brief Scalar Mahalanobis distance
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
double mahalanobis1d_distance(
	double v,
	std::vector<double> &data,
	double precision = 1e-30)
{
	double mean = mean1d_estimator(data);
	double var  = var1d_estimator(data,mean);
	return mahalanobis1d_distance(v,mean,var,precision);
}

#endif // LINEAR_ALGEBRA_HPP_
