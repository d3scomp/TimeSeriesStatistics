/*
 * TTable.h
 *
 * Header file for quantiles of the Student's t-distribution
 */

#ifndef TTABLE_H_
#define TTABLE_H_

enum ALPHAS {ALPHA_0_025, ALPHA_0_05};

extern const int minor_count;
extern const int boost;
extern const int df_max;

extern const double icdf[2][500];

#endif /* TTABLE_H_ */
