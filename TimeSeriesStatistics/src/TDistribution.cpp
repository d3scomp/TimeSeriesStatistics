/*
 * TDistribution.cpp
 *
 *  Created on: 19. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 */

#include "TDistribution.h"
#include <math.h>

TDistribution::TDistribution() {
	// TODO Auto-generated constructor stub

}

/*
  Purpose:

    TNC computes the tail of the noncentral T distribution.

  Discussion:

    This routine computes the cumulative probability at T of the
    non-central T-distribution with DF degrees of freedom (which may
    be fractional) and non-centrality parameter DELTA.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    13 November 2010

  Author:

    Original FORTRAN77 version by Russell Lenth.
    C version by John Burkardt.

  Reference:

    Russell Lenth,
    Algorithm AS 243:
    Cumulative Distribution Function of the Non-Central T Distribution,
    Applied Statistics,
    Volume 38, Number 1, 1989, pages 185-189.

    William Guenther,
    Evaluation of probabilities for the noncentral distributions and
    difference of two T-variables with a desk calculator,
    Journal of Statistical Computation and Simulation,
    Volume 6, Number 3-4, 1978, pages 199-206.

  Parameters:

    Input, double T, the point whose cumulative probability
    is desired.

    Input, double DF, the number of degrees of freedom.

    Input, double DELTA, the noncentrality parameter.

    Output, int *IFAULT, error flag.
    0, no error.
    nonzero, an error occcurred.

    Output, double TNC, the tail of the noncentral
    T distribution.
*/
double TDistribution::tnc (double t, double df, double delta, int *ifault){
	double a;
	double albeta;
	double alnrpi = 0.57236494292470008707;
	double b;
	double del;
	double en;
	double errbd;
	double errmax = 1.0E-10;
	double geven;
	double godd;
//	double half;
	int itrmax = 100;
	double lambda;
	int negdel;
//	double one;
	double p;
	double q;
	double r2pi = 0.79788456080286535588;
	double rxb;
	double s;
	double tt;
//	double two;
	double value;;
	double x;
	double xeven;
	double xodd;
//	double zero;

	value = 0.0;

	if ( df <= 0.0 ){
		*ifault = 2;
		return value;
	}

	*ifault = 0;

	tt = t;
	del = delta;
	negdel = 0;

	if ( t < 0.0 )	{
		negdel = 1;
		tt = - tt;
		del = - del;
	}
	/*
	Initialize twin series.
	*/
	en = 1.0;
	x = t * t / ( t * t + df );

	if ( x <= 0.0 ){
		*ifault = 0;
		value = value + alnorm ( del, 1 );

		if ( negdel ){
		  value = 1.0 - value;
		}
		return value;
	}

	lambda = del * del;
	p = 0.5 * exp ( - 0.5 * lambda );
	q = r2pi * p * del;
	s = 0.5 - p;
	a = 0.5;
	b = 0.5 * df;
	rxb = pow ( 1.0 - x, b );
	albeta = alnrpi + lgamma ( b ) - lgamma ( a + b );
	xodd = betain ( x, a, b, albeta, ifault );
	godd = 2.0 * rxb * exp ( a * log ( x ) - albeta );
	xeven = 1.0 - rxb;
	geven = b * x * rxb;
	value = p * xodd + q * xeven;
	/*
	Repeat until convergence.
	*/
	for ( ; ; ){
		a = a + 1.0;
		xodd = xodd - godd;
		xeven = xeven - geven;
		godd = godd * x * ( a + b - 1.0 ) / a;
		geven = geven * x * ( a + b - 0.5 ) / ( a + 0.5 );
		p = p * lambda / ( 2.0 * en );
		q = q * lambda / ( 2.0 * en + 1.0 );
		s = s - p;
		en = en + 1.0;
		value = value + p * xodd + q * xeven;
		errbd = 2.0 * s * ( xodd - godd );

		if ( errbd <= errmax ){
			*ifault = 0;
			break;
		}

		if ( itrmax < en ){
			*ifault = 1;
			break;
		}
	}

	value = value + alnorm ( del, 1 );

	if ( negdel ){
		value = 1.0 - value;
	}

	return value;
}

/*
  Purpose:

    ALNORM computes the cumulative density of the standard normal distribution.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    13 November 2010

  Author:

    Original FORTRAN77 version by David Hill.
    C++ version by John Burkardt.

  Reference:

    David Hill,
    Algorithm AS 66:
    The Normal Integral,
    Applied Statistics,
    Volume 22, Number 3, 1973, pages 424-427.

  Parameters:

    Input, double X, is one endpoint of the semi-infinite interval
    over which the integration takes place.

    Input, int UPPER, determines whether the upper or lower
    interval is to be integrated:
    1  => integrate from X to + Infinity;
    0 => integrate from - Infinity to X.

    Output, double ALNORM, the integral of the standard normal
    distribution over the desired interval.
*/
double TDistribution::alnorm ( double x, int upper )
{
	double a1 = 5.75885480458;
	double a2 = 2.62433121679;
	double a3 = 5.92885724438;
	double b1 = -29.8213557807;
	double b2 = 48.6959930692;
	double c1 = -0.000000038052;
	double c2 = 0.000398064794;
	double c3 = -0.151679116635;
	double c4 = 4.8385912808;
	double c5 = 0.742380924027;
	double c6 = 3.99019417011;
	double con = 1.28;
	double d1 = 1.00000615302;
	double d2 = 1.98615381364;
	double d3 = 5.29330324926;
	double d4 = -15.1508972451;
	double d5 = 30.789933034;
	double ltone = 7.0;
	double p = 0.398942280444;
	double q = 0.39990348504;
	double r = 0.398942280385;
	int up;
	double utzero = 18.66;
	double value;
	double y;
	double z;

	up = upper;
	z = x;

	if ( z < 0.0 ){
		up = !up;
		z = - z;
	}

	if ( ltone < z && ( ( !up ) || utzero < z ) ){
		if ( up ){
			value = 0.0;
		} else {
		  value = 1.0;
		}
		return value;
	}

	y = 0.5 * z * z;

	if ( z <= con ){
		value = 0.5 - z * ( p - q * y
				/ ( y + a1 + b1
						/ ( y + a2 + b2
								/ ( y + a3 ))));
	} else {
		value = r * exp ( - y )
		  	  / ( z + c1 + d1
		  			  / ( z + c2 + d2
		  					  / ( z + c3 + d3
		  							  / ( z + c4 + d4
		  									  / ( z + c5 + d5
		  											  / ( z + c6 ))))));
	}

	if ( !up ){
		value = 1.0 - value;
	}

	return value;
}

/*
  Purpose:

    BETAIN computes the incomplete Beta function ratio.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    13 November 2010

  Author:

    Original FORTRAN77 version by KL Majumder, GP Bhattacharjee.
    C version by John Burkardt.

  Reference:

    KL Majumder, GP Bhattacharjee,
    Algorithm AS 63:
    The incomplete Beta Integral,
    Applied Statistics,
    Volume 22, Number 3, 1973, pages 409-411.

  Parameters:

    Input, double X, the argument, between 0 and 1.

    Input, double P, Q, the parameters, which
    must be positive.

    Input, double BETA, the logarithm of the complete
    beta function.

    Output, int *IFAULT, error flag.
    0, no error.
    nonzero, an error occurred.

    Output, double BETAIN, the value of the incomplete
    Beta function ratio.
*/
double TDistribution::betain ( double x, double p, double q, double beta, int *ifault )
{
	double acu = 0.1E-14;
	double ai;
	//double betain;
	double cx;
	int indx;
	int ns;
	double pp;
	double psq;
	double qq;
	double rx;
	double temp;
	double term;
	double value;
	double xx;

	value = x;
	*ifault = 0;

	/* Check the input arguments. */
	if ( p <= 0.0 || q <= 0.0 ){
		*ifault = 1;
		return value;
	}

	if ( x < 0.0 || 1.0 < x ){
		*ifault = 2;
		return value;
	}

	/* Special cases. */
	if ( x == 0.0 || x == 1.0 )	{
		return value;
	}

	/* Change tail if necessary and determine S. */
	psq = p + q;
	cx = 1.0 - x;

	if ( p < psq * x ){
		xx = cx;
		cx = x;
		pp = q;
		qq = p;
		indx = 1;
	} else {
		xx = x;
		pp = p;
		qq = q;
		indx = 0;
	}

	term = 1.0;
	ai = 1.0;
	value = 1.0;
	ns = ( int ) ( qq + cx * psq );

	/* Use the Soper reduction formula. */
	rx = xx / cx;
	temp = qq - ai;
	if ( ns == 0 ){
		rx = xx;
	}

	while(true){
		term = term * temp * rx / ( pp + ai );
		value = value + term;
		temp = fabs ( term );

		if ( temp <= acu && temp <= acu * value ){
			value = value * exp ( pp * log ( xx )
				+ ( qq - 1.0 ) * log ( cx ) - beta ) / pp;

			if ( indx ){
				value = 1.0 - value;
			}

			break;
		}

		ai = ai + 1.0;
		ns = ns - 1;

		if ( 0 <= ns ){
			temp = qq - ai;
			if ( ns == 0 ){
				rx = xx;
			}
		} else {
			temp = psq;
			psq = psq + 1.0;
		}
	}

	return value;
}

