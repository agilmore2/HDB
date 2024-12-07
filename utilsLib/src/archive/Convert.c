#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <defines.h>
#include <typedefs.h> 
#include <utils.h>
#include <dbutils.h>

#define FUNC_NAME "ConvertValue"

/*
<HTML>

<P> <STRONG>Function name:</STRONG> ConvertValue

<P> <STRONG>Function signature:</STRONG> int ConvertValue (VALUE, int, ID, SQL_DATE, VALUE *, int, ID, TIME_INTERVAL_TYPES, long)


<P> <STRONG>Function description:</STRONG> Given a value in a certain unit and scale, convert it to another unit, usually of the same dimension.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> VALUE fromValue:              The value to be converted.
<BR><DD> int fromScale:                The scale in which fromValue is represented.
<BR><DD> ID fromUnit:                  The unit in which fromValue is represented.
<BR><DD> SQL_DATE fromDate:            The date which from value represents (dd-mon-yyyy hh24:mi:ss)
<BR><DD> VALUE *toValue:               The converted value, calculated.
<BR><DD> int toScale:                  The scale in which the converted value must be represented.
<BR><DD> ID toUnit:                    The unit in which the converted value must be represented.
<BR><DD> TIME_INTERVAL_TYPES interval: The interval which the converted value will represent, e.g., BY_MONTH, BY_DAY, etc.
<BR><DD> long seconds:                 The number of seconds in the interval; necessary only if interval is complex range, such as Apr - July; otherwise, use NA.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int ConvertValue (VALUE fromValue, int fromScale, ID fromUnit, 
		  SQL_DATE fromDate, VALUE *toValue, int toScale, ID toUnit,
		  TIME_INTERVAL_TYPES interval, long seconds)

{

  UNIT_INFO fromUnitInfo,
            toUnitInfo;
  
  int result;

  /* Units are same; do scaling if necessary. */
  if (fromUnit == toUnit)
    {
      if (fromScale == toScale)
	{
	  *toValue = fromValue;
	  return (OK);
	}
      else
	{
	  *toValue = fromValue * ((double) fromScale / (double) toScale);
	  return (OK);
	}
    }

  if ((result = SqlGetUnitInfo (fromUnit, &fromUnitInfo)) != OK)
    {
      PrintError ("Can't get info for unit %d\n\tExiting.", fromUnit);
      return (ERROR);
    }

  if ((result = SqlGetUnitInfo (toUnit, &toUnitInfo)) != OK)
    {
      PrintError ("Can't get info for unit %d\n\tExiting.", toUnit);
      return (ERROR);
    }

	
  if (fromUnitInfo.dimension != toUnitInfo.dimension)
    {
      if (((fromUnitInfo.dimension == VOLUME_ID) && 
	   (toUnitInfo.dimension == FLOW_ID)) ||
	  ((fromUnitInfo.dimension == FLOW_ID) && 
	   (toUnitInfo.dimension == VOLUME_ID)))
	{
	  /* Do flow<->volume conversion. */

	  if ((result = ConvertFlowVolume (fromUnitInfo, toUnitInfo, fromValue,
					   fromScale,
					   fromDate, toValue, toScale, 
					   interval, seconds)) != OK)
	    {
	      PrintError ("Problem converting between FLOW and VOLUME.\n\tExiting.");
	      return (ERROR);
	    }
	  return (OK);
	}

      else if (((fromUnitInfo.dimension == POWER_ID) && 
		(toUnitInfo.dimension == ENERGY_ID)) ||
	       ((fromUnitInfo.dimension == ENERGY_ID) && 
		(toUnitInfo.dimension == POWER_ID)))
	{
	  /* Do power<->energy conversion. */
	  if ((result = ConvertPowerEnergy (fromValue, fromScale, fromUnitInfo,
					    fromDate, toValue, toScale, 
					    toUnitInfo, interval, seconds)) 
	      != OK)
	    {
	      PrintError ("Problem converting between POWER and ENERGY.\n\tExiting.");
	      return (ERROR);
	    }
	  return (OK);
	}
      else
	{
	  PrintError ("Cannot convert between units of different dimensions, except FLOW<->VOLUME and POWER<->ENERGY.\n\tExiting.");
	  return (ERROR);
	}
    }

  /* Convert expression values (temperatures) */
  if (!fromUnitInfo.isFactor || !toUnitInfo.isFactor)
    {
      if ((result = ConvertExpressionValue (fromValue, fromScale, fromUnitInfo,
					    fromDate, toValue, toScale, 
					    toUnitInfo, seconds, interval))
	  != OK)
	{
	  PrintError ("Trouble doing expression conversion from %d to %d .\n\tExiting.", fromUnit, toUnit);
	  return (ERROR);
	}
      return (OK);
    }

  /* Do conversion */
  SimpleConvert (fromValue, fromScale, fromUnitInfo, fromDate,
		 toValue, toScale, toUnitInfo, seconds);
    
  return (OK);
}
	

/*********************************************************************/
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SimpleConvert

<P> <STRONG>Function signature:</STRONG> int SimpleConvert (VALUE, int, UNIT_INFO, SQL_DATE, VALUE *, int, UNIT_INFO, long)

<P> <STRONG>Function description:</STRONG> Converts a value to a different unit in the same dimension, using simple factors, not expressions.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> VALUE fromValue:              The value to be converted.
<BR><DD> int fromScale:                The scale in which fromValue is represented.
<BR><DD> UNIT_INFO fromUnitInfo:       The unit in which fromValue is represented.
<BR><DD> SQL_DATE fromDate:            The date which from value represents (dd-mon-yyyy hh24:mi:ss)
<BR><DD> VALUE *toValue:               The converted value, calculated.
<BR><DD> int toScale:                  The scale in which the converted value must be represented.
<BR><DD> UNIT_INFO toUnitInfo:         The unit in which the converted value must be represented.
<BR><DD> TIME_INTERVAL_TYPES interval: The interval which the converted value will represent, e.g., BY_MONTH, BY_DAY, etc.
<BR><DD> long seconds:                 The number of seconds in the interval; necessary only if interval is complex range, such as Apr - July; otherwise, use NA.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
int SimpleConvert (VALUE fromValue, int fromScale, 
		   UNIT_INFO fromUnitInfo, SQL_DATE fromDate,
		   VALUE *toValue, int toScale, UNIT_INFO toUnitInfo, 
		   long seconds)
{
  double coef;
  double stdValue;
  int result;

  /* Convert non-expression values; scale for monthly and yearly values. Note that
     when the unit dimension is TIME, rather than something OVER TIME, the scaling
     is reversed. */
  coef = fromUnitInfo.multFactor;
  if (fromUnitInfo.monthly)
    {
       if (seconds == NA)
       {
	  /* Calculate number of seconds in month, if necessary. */
	  if ((result = SqlGetNumSecondsInMonth (fromDate, &seconds)) != OK)
	  {
	     PrintError ("Problem getting number of seconds in month %s.\n\tExiting.\n", fromDate);
	     return (ERROR);
	  }
       }

      if (fromUnitInfo.overTime)
	coef *= (double) seconds / (double) MONTH_31_DAY;
      else 
	coef *= (double) MONTH_31_DAY / (double) seconds;
    }
  else if (fromUnitInfo.yearly)
    {
       /* Calculate number of seconds in year, if necessary. */
       if (seconds == NA)
       {
	  if ((result = GetNumSecondsInYear (fromDate, &seconds, fromUnitInfo.yearly)) != OK)
	  {
	     PrintError ("Problem getting number of seconds in year %s.\n\tExiting.\n", fromDate);
	     return (ERROR);
	  }
       }

      if (fromUnitInfo.overTime)
	coef *= (double) seconds / (double) YEAR_365_DAY;
      else 
	coef *= (double) YEAR_365_DAY / (double) seconds;
    }

  stdValue = fromValue / coef * fromScale;

  coef = toUnitInfo.multFactor;
  if (toUnitInfo.monthly)
    {
       /* Calculate number of seconds in month, if necessary. */
       if (seconds == NA)
       {
	  if ((result = SqlGetNumSecondsInMonth (fromDate, &seconds)) != OK)
	  {
	     PrintError ("Problem getting number of seconds in month %s.\n\tExiting.\n", fromDate);
	     return (ERROR);
	  }
       }
       
       if (toUnitInfo.overTime)
	  coef *= (double) seconds / (double) MONTH_31_DAY;
       else 
	  coef *= (double) MONTH_31_DAY / (double) seconds;
    }
  else if (toUnitInfo.yearly)
    {
       /* Calculate number of seconds in year, if necessary. */
       if (seconds == NA)
       {
	  if ((result = GetNumSecondsInYear (fromDate, &seconds, toUnitInfo.yearly)) != OK)
	  {
	     PrintError ("Problem getting number of seconds in year %s.\n\tExiting.\n", fromDate);
	     return (ERROR);
	  }
       }

       if (toUnitInfo.overTime)
	  coef *= (double) seconds / (double) YEAR_365_DAY;
       else 
	  coef *= (double) YEAR_365_DAY / (double) seconds;
    }

  *toValue = stdValue * coef / toScale;

  return (OK);
}

/*
<HTML>

/*
<HTML>

<P> <STRONG>Function name:</STRONG> ConvertFlowVolume

<P> <STRONG>Function signature:</STRONG> int ConvertFlowVolume (UNIT_INFO, UNIT_INFO, VALUE, int, SQL_DATE, VALUE *, int, TIME_INTERVAL_TYPES, long)

<P> <STRONG>Function description:</STRONG> Converts a flow value to a volume, or vice-versa.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> UNIT_INFO fromUnitInfo:       The unit in which fromValue is represented.
<BR><DD> UNIT_INFO toUnitInfo:         The unit in which the converted value must be represented.
<BR><DD> VALUE fromValue:              The value to be converted.
<BR><DD> int fromScale:                The scale in which fromValue is represented.
<BR><DD> SQL_DATE fromDate:            The date which from value represents (dd-mon-yyyy hh24:mi:ss)
<BR><DD> VALUE *toValue:               The converted value, calculated.
<BR><DD> int toScale:                  The scale in which the converted value must be represented.
<BR><DD> TIME_INTERVAL_TYPES interval: The interval which the converted value will represent, e.g., BY_MONTH, BY_DAY, etc.
<BR><DD> long seconds:                 The number of seconds in the interval; necessary only if interval is complex range, such as Apr - July; otherwise, use NA.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int ConvertFlowVolume (UNIT_INFO fromUnitInfo, UNIT_INFO toUnitInfo,
		       VALUE fromValue, int fromScale, 
		       SQL_DATE fromDate, VALUE *toValue, int toScale, 
		       TIME_INTERVAL_TYPES interval, long seconds)
{
  double stdValue;
  UNIT_INFO fromStdUnitInfo;
  UNIT_INFO toStdUnitInfo;
  int setFrom = FALSE;
  int setTo = FALSE;
  int result;
  long secondsInterval;

  /* If from unit (and value) are not standard, convert them and set
     the flags. */
  if (fromUnitInfo.multFactor != 1)
    {
      if ((result = SqlGetUnitInfo (fromUnitInfo.storedUnitId, 
				    &fromStdUnitInfo)) != OK)
	{
	  PrintError ("Error getting standard unit for %s.\n\tExiting", 
		      fromUnitInfo.name);
	  return (ERROR);
	}

      /* Convert fromValue into standard units, preserving scale. */
      SimpleConvert (fromValue, fromScale, fromUnitInfo, fromDate,
		     &stdValue, fromScale, fromStdUnitInfo, seconds);

      setFrom = TRUE;
    }
  else
    {
      fromStdUnitInfo = fromUnitInfo;
      stdValue = fromValue;
    }
    
  if (toUnitInfo.multFactor != 1)
    {
      if ((result = SqlGetUnitInfo (toUnitInfo.storedUnitId, 
				    &toStdUnitInfo)) != OK)
	{
	  PrintError ("Error getting standard unit for %s.\n\tExiting", 
		      toUnitInfo.name);
	  return (ERROR);
	}

      setTo = TRUE;
    }
  else
    toStdUnitInfo = toUnitInfo;


  /* Now convert from value in standard units to value in
     standard units. Really all this does here is the scaling and time
     stuff; no factoring is needed since both units are standard. */
  SimpleConvert (stdValue, fromScale, fromStdUnitInfo, fromDate,
		 toValue, toScale, toStdUnitInfo, seconds);
  
  /* NOTE: seems silly: interval overwritten in simpleConvert if month or year,
   and not used otherwise. */
  /* Calculate number of seconds in month or year, if necessary. */
  if (seconds == NA)
    {
      if ((result = GetNumSecondsInInterval (interval, fromDate, &secondsInterval)) != OK)
	{
	  PrintError ("Problem getting number of seconds in interval.\n\tExiting.\n");
	  return (ERROR);
	}
    }
  else
    secondsInterval = seconds;

  /* Multiply or divide by seconds to distribute the time properly, and 
     by the number of cubic feet in an acre foot :)', to get the proper 
     value in standard units. 
     NOTE: RiverWare doesn't need this CF/AF factor because its flow and
     volume are both based on cubic meters. 
     ASSUMPTION: Standard flow unit is based on seconds. 
                 Standard flow is CFS, standard volume is AF. */
  if (fromStdUnitInfo.dimension == FLOW_ID)
    *toValue *= (double)secondsInterval / (double)NUM_CF_IN_AF;
  else
    *toValue /= (double)secondsInterval / (double)NUM_CF_IN_AF;

  /* if toUnit was not standard, convert the toValue (now in
     standard units) to the desired unit. */
  if (setTo)
    {
      fromValue = *toValue;
      SimpleConvert (fromValue, toScale, toStdUnitInfo, fromDate,
		     toValue, toScale, toUnitInfo, seconds);
    }

  return (OK);
}

/*
<HTML>

<P> <STRONG>Function name:</STRONG> ConvertPowerEnergy

<P> <STRONG>Function signature:</STRONG> int ConvertPowerEnergy (VALUE, int, UNIT_INFO, SQL_DATE, VALUE *, int, UNIT_INFO, TIME_INTERVAL_TYPES, long)

<P> <STRONG>Function description:</STRONG> Converts a power value to an energy value, or vice-versa.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> VALUE fromValue:              The value to be converted.
<BR><DD> int fromScale:                The scale in which fromValue is represented.
<BR><DD> UNIT_INFO fromUnitInfo:       The unit in which fromValue is represented.
<BR><DD> SQL_DATE fromDate:            The date which from value represents (dd-mon-yyyy hh24:mi:ss)
<BR><DD> VALUE *toValue:               The converted value, calculated.
<BR><DD> int toScale:                  The scale in which the converted value must be represented.
<BR><DD> UNIT_INFO toUnitInfo:         The unit in which the converted value must be represented.
<BR><DD> TIME_INTERVAL_TYPES interval: The interval which the converted value will represent, e.g., BY_MONTH, BY_DAY, etc.
<BR><DD> long seconds:                 The number of seconds in the interval; necessary only if interval is complex range, such as Apr - July; otherwise, use NA.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int ConvertPowerEnergy (VALUE fromValue, int fromScale, 
			UNIT_INFO fromUnitInfo, SQL_DATE fromDate,
			VALUE *toValue, int toScale, 
			UNIT_INFO toUnitInfo, TIME_INTERVAL_TYPES interval,
			long seconds)
{
  double stdValue;
  UNIT_INFO fromStdUnitInfo;
  UNIT_INFO toStdUnitInfo;
  int setFrom = FALSE;
  int setTo = FALSE;
  int result;
  long secondsInterval;
  long hoursInterval;

  /* If from unit (and value) are not standard, convert them and set
     the flags. */
  if (fromUnitInfo.multFactor != 1)
    {
      if ((result = SqlGetUnitInfo (fromUnitInfo.storedUnitId, 
				    &fromStdUnitInfo)) != OK)
	{
	  PrintError ("Error getting standard unit for %s.\n\tExiting", 
		      fromUnitInfo.name);
	  return (ERROR);
	}

      /* Convert fromValue into standard units, preserving scale. */
      SimpleConvert (fromValue, fromScale, fromUnitInfo, fromDate,
		     &stdValue, fromScale, fromStdUnitInfo, seconds);

      setFrom = TRUE;
    }
  else
    {
      fromStdUnitInfo = fromUnitInfo;
      stdValue = fromValue;
    }
    
  if (toUnitInfo.multFactor != 1)
    {
      if ((result = SqlGetUnitInfo (toUnitInfo.storedUnitId, 
				    &toStdUnitInfo)) != OK)
	{
	  PrintError ("Error getting standard unit for %s.\n\tExiting", 
		      toUnitInfo.name);
	  return (ERROR);
	}

      setTo = TRUE;
    }
  else
    toStdUnitInfo = toUnitInfo;


  /* Now convert from value in standard units to value in
     standard units. Really all this does here is the scaling and time
     stuff; no factoring is needed since both units are standard. */
  SimpleConvert (stdValue, fromScale, fromStdUnitInfo, fromDate,
		 toValue, toScale, toStdUnitInfo, seconds);
  
  /* Calculate number of seconds in the interval, then divide by 3600
     to get number of hours */
  if (seconds == NA)
    {
      if ((result = GetNumSecondsInInterval (interval, fromDate, &secondsInterval)) != OK)
	{
	  PrintError ("Problem getting number of seconds in interval.\n\tExiting.\n");
	  return (ERROR);
	}
    }
  else
    secondsInterval = seconds;
  
  hoursInterval = secondsInterval / 3600;

  /* Multiply or divide by number of hours in destination interval
     to distribute the time properly.
     ASSUMPTION: Standard power unit is based on megawatts.
                 Standard energy unit is based on megawatt hours. 
		 Value passed in is not a SUM of powers, but an avg or
                   a single value */
  if (fromStdUnitInfo.dimension == POWER_ID)
    *toValue *= (double)hoursInterval;
  else
    *toValue /= (double)hoursInterval;

  /* if toUnit was not standard, convert the toValue (now in
     standard units) to the desired unit. */
  if (setTo)
    {
      fromValue = *toValue;
      SimpleConvert (fromValue, toScale, toStdUnitInfo, fromDate,
		     toValue, toScale, toUnitInfo, seconds);
    }

  return (OK);
}

/*
<HTML>

<P> <STRONG>Function name:</STRONG> ConvertExpressionValue

<P> <STRONG>Function signature:</STRONG> int ConvertExpressionValue0 (VALUE, int, UNIT_INFO, SQL_DATE, VALUE *, int, UNIT_INFO, long, TIME_INTERVAL_TYPES)

<P> <STRONG>Function description:</STRONG> Converts a power value to an energy value, or vice-versa.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> VALUE fromValue:              The value to be converted.
<BR><DD> int fromScale:                The scale in which fromValue is represented.
<BR><DD> UNIT_INFO fromUnitInfo:       The unit in which fromValue is represented.
<BR><DD> SQL_DATE fromDate:            The date which from value represents (dd-mon-yyyy hh24:mi:ss)
<BR><DD> VALUE *toValue:               The converted value, calculated.
<BR><DD> int toScale:                  The scale in which the converted value must be represented.
<BR><DD> UNIT_INFO toUnitInfo:         The unit in which the converted value must be represented.
<BR><DD> long seconds:                 The number of seconds in the interval; necessary only if interval is complex range, such as Apr - July; otherwise, use NA.
<BR><DD> TIME_INTERVAL_TYPES interval: The interval which the converted value will represent, e.g., BY_MONTH, BY_DAY, etc.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/
     
int ConvertExpressionValue (VALUE fromValue, int fromScale, 
			    UNIT_INFO fromUnitInfo,
			    SQL_DATE fromDate, VALUE *toValue, int toScale, 
			    UNIT_INFO toUnitInfo, long seconds, 
			    TIME_INTERVAL_TYPES interval)

{
  int result;

  VALUE stdValue;
  UNIT_INFO stdUnitInfo;

  /* If fromUnit has an expression, we know that value must be put into
     standard units first. */
  if (!fromUnitInfo.isFactor)
    {
      /* Get info on the standard unit */
      if ((result = SqlGetUnitInfo (fromUnitInfo.storedUnitId, 
				    &stdUnitInfo)) != OK)
	{
	  PrintError ("Can't get info for unit %d\n\tExiting.", 
		      fromUnitInfo.storedUnitId);
	  return (ERROR);
	}
      
      if ((result = ConvertExpression (fromValue, fromScale, fromDate,
				       fromUnitInfo.toStdExpression,
				       &stdValue, 1, seconds, interval))
	  != OK)
	{
	  PrintError ("Trouble doing expression conversion from %d to stored unit %d.\n\tExiting.", fromUnitInfo.unitId, stdUnitInfo.unitId);
	  return (ERROR);
	}
    }
  else
    stdValue = fromValue;

  /* If stdUnit = toUnit, we're done. Conversion was from non-standard
     to standard. If not equal, must do expression conversion for
     standard unit to toUnit. */
  if (stdUnitInfo.unitId == toUnitInfo.unitId)
    {
      *toValue = stdValue;
      return (OK);
    }

  /* Otherwise, take the value in standard units and convert it to the
     ultimately required unit. */
  if ((result = ConvertExpression (stdValue, 1, fromDate, 
				   toUnitInfo.fromStdExpression,
				   toValue, toScale, seconds, interval))
      != OK)
    {
      PrintError ("Trouble doing expression conversion from %d to stored unit %d.\n\tExiting.", stdUnitInfo.unitId, toUnitInfo.unitId);
      return (ERROR);
    }
  return (OK);
}
      

/*****************************************************/
/*
<HTML>

<P> <STRONG>Function name:</STRONG> ConvertExpression

<P> <STRONG>Function signature:</STRONG> int ConvertExpression (VALUE, int, SQL_DATE, char *, VALUE *, int, long, TIME_INTERVAL_TYPES)

<P> <STRONG>Function description:</STRONG> Converts value from one unit to another where conversion is represented by expression, as in Celcius to Fahrenheit.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> VALUE fromValue:              The value to be converted.
<BR><DD> int fromScale:                The scale in which fromValue is represented.
<BR><DD> SQL_DATE fromDate:            The date which from value represents (dd-mon-yyyy hh24:mi:ss)
<BR><DD> char *expression:             Expression to be evaluated for conversion.
<BR><DD> VALUE *toValue:               The converted value, calculated.
<BR><DD> int toScale:                  The scale in which the converted value must be represented.
<BR><DD> long seconds:                 The number of seconds in the interval; necessary only if interval is complex range, such as Apr - July; otherwise, use NA.
<BR><DD> TIME_INTERVAL_TYPES interval: The interval which the converted value will represent, e.g., BY_MONTH, BY_DAY, etc.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int ConvertExpression (VALUE fromValue, int fromScale, SQL_DATE fromDate, 
		       char *expression, VALUE *toValue, int toScale, 
		       long seconds,  TIME_INTERVAL_TYPES interval)

{
  int result,
      success,
      nDays;
  expression_var_list *varList,
                      *current;

  varList = GetVariableList (expression);

  current = varList;
  while (current)
    {
      switch (current->var_type)
	{
	  case (SOURCE_VALUE):
	    current->value = fromValue; 
	    break;
	  case (N_DAYS):
	    /* Set nDays if it can't be calculated from seconds. */
	    if (seconds == NA)
	      {
		if ((result = GetNumDays (fromDate, interval, &nDays)) != OK)
		  {
		    return (ERROR);
		  }
	      }
	    else
	      nDays = seconds / SECONDS_IN_DAY;
	    current->value = nDays;
	    break;
	  default:
	    PrintError ("%s: Bad variable type in expression %s.\n\tExiting.",
			FUNC_NAME, expression);
	    return (ERROR);
	  }
      current = current->nextItem;
    }

  if (!(success = CalcExpression (toValue, expression, varList)))
    {
      PrintError ("%s: Failure in converting %d using %s.\n\tExiting.",
		  FUNC_NAME, fromValue, expression);
      return (ERROR);
    }

  /* Scale the value */
  *toValue *= fromScale / toScale;

  /* DO I NEED TO HANDLE MONTHLY AND YEARLY HERE?? */
 
  return (OK);
}


/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetNumDays

<P> <STRONG>Function signature:</STRONG> int GetNumDays (SQL_DATE, TIME_INTERVAL_TYPES, int *)

<P> <STRONG>Function description:</STRONG> Gets the number of days in the indicated interval for the given date. For instance, if interval is BY_MONTH and the date is 01-OCT-1998, the number of days is 31.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE timeStamp:           The date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> TIME_INTERVAL_TYPES interval: The interval (BY_DAY, BY_MONTH, etc.).
<BR><DD> int *nDays:                   Number of days in the interval, calculated.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

    
int GetNumDays (SQL_DATE timeStamp, TIME_INTERVAL_TYPES interval, int *nDays)

{
  int month,
      waterYear,
      result;
  SQL_DATE wyFirstDay;

  switch (interval)
    {
      case BY_INSTANT: 
        PrintError ("%s: Instantaneous values should not require a conversion \n\twhere number of days must be known.\n\tCheck your data.\n\tExiting.",
		    FUNC_NAME);
	return (ERROR);
      case BY_SECOND:
	*nDays = 1/86400;
	break;
      case BY_MINUTE:
	*nDays = 1/1440;
	break;
      case BY_HOUR:
	*nDays = 1/24;
	break;
      case BY_DAY:
	*nDays = 1;
	break;
      case BY_MONTH:
	if ((result = SqlExtractDatePart (timeStamp, MONTH, &month)) != OK)
	  {
	    return (ERROR);
	  }
	switch (month)
	  {
	    case 1:
  	    case 3:
  	    case 5:
  	    case 7:
  	    case 8:
  	    case 10:
  	    case 12:
	      *nDays = 31;
	      break;
	    case 4:
	    case 6:
	    case 9:
	    case 11:
	      *nDays = 30;
	      break;
	    case 2:
	      if (IsLeapYear (timeStamp))
		*nDays = 29;
	      else
		*nDays = 28;
	      break;
	    }
	break;
      case BY_YEAR:
	if (IsLeapYear (timeStamp))
	  *nDays = 366;
	else
	  *nDays = 365;
	break;
      case BY_WY:
	if ((result = GetWaterYear (timeStamp, &waterYear)) != OK)
	  {
	    return (ERROR);
	  }
	
	GetFirstDayWaterYear (waterYear + 1, wyFirstDay);
	if (IsLeapYear (wyFirstDay))
	  *nDays = 366;
	else
	  *nDays = 365;
	break;
      }

  return (OK);
}

/***********************************************************************/
#define FUNC_NAME "GetNumSeconds"

/*
<HTML>

<P> <STRONG>Function name:</STRONG> GetNumSeconds

<P> <STRONG>Function signature:</STRONG> int GetNumSeconds (SQL_DATE, TIME_INTERVAL_TYPES, int *)

<P> <STRONG>Function description:</STRONG> Gets the number of seconds in the indicated interval for the given date. For instance, if interval is BY_MONTH and the date is 01-OCT-1998, the number of seconds is 2678400.

<P><DL> <STRONG>Function arguments:</STRONG>
<BR><DD> SQL_DATE timeStamp:           The date (dd-mon-yyyy hh24:mi:ss).
<BR><DD> TIME_INTERVAL_TYPES interval: The interval (BY_DAY, BY_MONTH, etc.).
<BR><DD> int *nDays:                   Number of seconds in the interval, calculated.
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int GetNumSeconds (SQL_DATE timeStamp, TIME_INTERVAL_TYPES interval, 
		   int *nSeconds)

{
  int nDays,
      result;
  
  if ((result = GetNumDays (timeStamp, interval, &nDays)) != OK)
    {
      PrintError ("%s: Could not get nDays (using get) for calculating seconds.\n\tExiting.", FUNC_NAME);
      return (ERROR);
    }

  *nSeconds = nDays * SECONDS_IN_DAY;

  return (OK);
}
