/*
 * TDistribution.h
 *
 *  Created on: 19. 1. 2016
 *      Author: Dominik Skoda <skoda@d3s.mff.cuni.cz>
 */

#ifndef TDISTRIBUTION_H_
#define TDISTRIBUTION_H_

class TDistribution {
public:
	TDistribution();
	double tnc (double t, double df, double delta, int *ifault);

private:
	double betain ( double x, double p, double q, double beta, int *ifault );
	double alnorm ( double x, int upper );
};

#endif /* TDISTRIBUTION_H_ */
