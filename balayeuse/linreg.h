/*  linreg.h
    Linear Regression calculation class

    by: David C. Swaim II, Ph.D.

    This class implements a standard linear regression on
    experimental data using a least squares fit to a straight
    line graph.  Calculates coefficients a and b of the equation:

                        y = a + b * x

    for data points of x and y.  Also calculates the coefficient of
    determination, the coefficient of correlation, and standard
    error of estimate.

    The value n (number of points) must be greater than 2 to
    calculate the regression.  This is primarily because the
    standard error has a (N-2) in the denominator.

    Check haveData() to see if there is enough data in
    LinearRegression to get values.

    You can think of the x,y pairs as 2 dimensional points.
    The class Point2D is included to allow pairing x and y
    values together to represent a point on a plane.

*/
#ifndef _LINREG_H_
#define _LINREG_H_
//#include <iostream.h>
#include "Vectors.h"

class LinearRegression
{
    public:
        // Constructor using an array of Point2D objects
        // This is also the default constructor
        LinearRegression(Vector3 *p = 0, long size = 0);

        // Constructor using arrays of x values and y values
        LinearRegression(float *x, float *y, long size = 0);

virtual void addXZ(const float& x, const float& z);
        void addPoint(const Vector3& p) { addXZ(p.x, p.z); }

        // Must have at least 3 points to calculate
        // standard error of estimate.  Do we have enough data?
        int haveData() const { return (n > 2 ? 1 : 0); }
        long items() const { return n; }

virtual float getA() const { return a; }
virtual float getB() const { return b; }

        float getCoefDeterm() const  { return coefD; }
        float getCoefCorrel() const { return coefC; }
        float getStdErrorEst() const { return stdError; }
virtual float estimateY(float x) const { return (a + b * x); }

    protected:
        long n;             // number of data points input so far
        float sumX, sumZ;  // sums of x and y
        float sumXsquared, // sum of x squares
               sumZsquared; // sum y squares
        float sumXZ;       // sum of x*y

        float a, b;        // coefficients of f(x) = a + b*x
        float coefD,       // coefficient of determination
               coefC,       // coefficient of correlation
               stdError;    // standard error of estimate

        void Calculate();   // calculate coefficients
};

#endif                      // end of linreg.h
