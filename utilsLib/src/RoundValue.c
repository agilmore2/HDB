/****************************************************************************
* DESCRIPTION
* This file contains functions for rounding values.
* 
* AUTHOR - EDITORS
* Julien Chastang
****************************************************************************/

/*
<HTML>

<P> <STRONG>Function name:</STRONG> RoundValue

<P> <STRONG>Function signature:</STRONG> float RoundValue(float value, int round_amount) 

<P> <STRONG>Function description:</STRONG> A function for rounding float values 

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> float value         : The value to be rounded.
<BR><DD> int round_amount    : Amount to round by (1 or a factor of 10)
</DL>

<P> <STRONG>Function return value:</STRONG> Returns the rounded float.

</HTML>
*/

#include <utils.h>

float RoundValue(float value, int round_amount)
{
    int temp_value;
    float difference;
    float rounding_cutoff;

    /* round_amount must be 1 or a factor of ten, or this fails to round */
    if ( (round_amount != 1) && (round_amount % 10 != 0) )
    {
        PrintError("Illegal rounding amount: %d\n", round_amount);
        return(value);
    }

    rounding_cutoff = ((float) round_amount) / 2;

    temp_value = (int) (value / round_amount);
    temp_value = temp_value * round_amount;
    difference = (float) (value - temp_value);

    if (difference <= rounding_cutoff)
    { /* round down */
        return ((float) temp_value);
    }
    else
    {                               /* round up   */
        return ((float) (temp_value + round_amount));
    }
}


