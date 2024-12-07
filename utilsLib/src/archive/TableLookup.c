
/*                                  TableLookup                         */

/*  program that does a linear interpolation.  An array of xvalues, an 
    array of y values, an xvalue, the address for the y value, and the 
    number of valid rows in each of the arrays are passed to the function.  
    The interpolated y value is then computed.  */

/*  if the xvalue is below lowest value in xarray(0), the function returns 
    an error.  If the xvalue is above higheset value in xarray(nrows), 
    the function returns an error as well */

/*  errors give a return value of -1
    successful fuctioning returns a value of 1 */

/*  this function is a modified version of lookup.c */

#include <defines.h>
/*
<HTML>

<P> <STRONG>Function name:</STRONG> TableLookup

<P> <STRONG>Function signature:</STRONG> int  TableLookup(float *,float *,float,float *,int)

<P> <STRONG>Function description:</STRONG> Interpolates a y value given an array of xvalues, an array of yvalues, and the lookup x value. Returns error if the given x value is outside the range of values in the x array.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> float *xarray: Array of x values.
<BR><DD> float *yarray: Array of y values.
<BR><DD> float xval:    X (lookup) value.
<BR><DD> float *yval:   Y value, interpolated.
<BR><DD> int nrows:     Number of rows in X and Y arrays.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/


int  TableLookup(float *xarray,float *yarray,float xval,float *yval,int nrows)
{
  int j,ju,jm;
  *yval = -998877.0;
  j=0;
  ju = nrows + 1;
  while ((ju-j) > 1)
    {
      jm = (ju + j)/2;
      if (xval > xarray[jm-1])
	j=jm;
      else 
	ju=jm;
    }
  /*printf("\n The value of j is %d",j);*/
  /*printf("\n The value of nrows is %d",nrows);*/
  if ((j != 0) && (j != nrows))
    {
      *yval = ((xval-xarray[j-1]) / (xarray[j]-xarray[j-1])) * (yarray[j]-yarray[j-1])
	+ yarray[j-1];
      return (OK);
    }
  return (ERROR);
}
