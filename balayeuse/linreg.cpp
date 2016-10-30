#include <math.h>
#include <float.h>
#include "linreg.h"

LinearRegression::LinearRegression(Vector3 *p, long size)
{
    long i;
    a = b = sumX = sumZ = sumXsquared = sumZsquared = sumXZ = 0.0;
    n = 0L;

    if (size > 0L) // if size greater than zero there are data arrays
        for (n = 0, i = 0L; i < size; i++)
            addPoint(p[i]);
}

LinearRegression::LinearRegression(float *x, float *z, long size)
{
    long i;
    a = b = sumX = sumZ = sumXsquared = sumZsquared = sumXZ = 0.0;
    n = 0L;

    if (size > 0L) // if size greater than zero there are data arrays
        for (n = 0, i = 0L; i < size; i++)
            addXZ(x[i], z[i]);
}

void LinearRegression::addXZ(const float& x, const float& z)
{
    n++;
    sumX += x;
    sumZ += z;
    sumXsquared += x * x;
    sumZsquared += z * z;
    sumXZ += x * z;
    Calculate();
}

void LinearRegression::Calculate()
{
    if (haveData())
    {
        if (fabs( float(n) * sumXsquared - sumX * sumX) > DBL_EPSILON)
        {
            b = ( float(n) * sumXZ - sumZ * sumX) /
                ( float(n) * sumXsquared - sumX * sumX);
            a = (sumZ - b * sumX) / float(n);

            float sx = b * ( sumXZ - sumX * sumZ / float(n) );
            float sz2 = sumZsquared - sumZ * sumZ / float(n);
            float sz = sz2 - sx;

            coefD = sx / sz2;
            coefC = sqrt(coefD);
            stdError = sqrt(sz / float(n - 2));
        }
        else
        {
            a = b = coefD = coefC = stdError = 0.0;
        }
    }
}
