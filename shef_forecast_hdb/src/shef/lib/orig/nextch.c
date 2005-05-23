/*Translated by FOR_C, v3.4.2 (-), on 06/07/94 at 16:58:22 */
/*FOR_C Options SET: c=2 com=u do=r4 ftn=2ln6k op=iv s=dvn str=l sq=i */

#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "shef_structs_external.h"

/*---------------------------------------------------------------------
  
       NAME
         SUBROUTINE NEXTCH(ICHAR,STATUS)
  
       PURPOSE                                                    
         Read lines into a buffer and provide the next character.
         IP points to the last char provided.
         Anything after and including a colon is ignored.
         Database comments are noted withing double quotes.
  
       VERSION and UPDATES
         1.0    APR 82   Geoffrey M. Bonnin
                Original Version
         1.1    AUG 84
                Allow colon as off/on switch on line scan.
                Modify line output to error log.
         1.2    JUN 89   David G. Brandon
                Adapted to QNX NWS Hydromet
         2.0    MAY 94   David G. Brandon
                Also Translated to 'C' using FOR_C
         2.1    APR 95   David G. Brandon
                Changed some of the I/O created by the FORTRAN to C
                translator to 'C' routines
                Cast input string to all upper case.
         2.2    OCT  95  DGB
                Add iscore to t_codes array.
         2.3    MAY 30 96 DGB
                Add the capability to store the last line read into 
                a char buffer.  This will be used in the option to 
                write out the current line if an error occurs.
                This is refered to as the PARIAL_ERROR option.
         2.4    SEP 10 97 DGB
                Add the use of MAX_SHEF_INPUT so that the input line can
                be longer than 80 characters.
                Add code for database comments.
                Add error checks for database comments.
         2.5    JAN 27 98 DGB
                Add capabilities for preprocessing .E lines.  
                Preprocessing consists of detecting NULL fields, 
                i.e.,  a slash followed by another slash or blanks
                then a slash.  {  e.g.  12.2 /  /  4 }.  Also,
                leading slash marks on continuation lines are 
                replaced by a blank.
	 2.5    JAN 29 1998 DGB
                Change number of consecutive blanks from 15 to
                NUM_BLANKS { set at 50 }
         2.6    SEP 22 98 DGB
                Add -r switch for preprocessing .E messages
         2.7    APR 1 99 DGB
                Change <cr> decimal 13 to blanks.
         2.8    JAN 02 00 DGB
                Reworked input so that a : could be part of a
                retained or database comment.
 *--------------------------------------------------------------------- */


void nextch(ichar, status)
short int *ichar, *status;
{
   static short int _fst, i, j, lchar, itemp;               /* dgb:09/10/97 */   
   char buff_temp[MAX_SHEF_INPUT + 1];                      /* dgb:09/10/97 */
   int DEBUG1;                                              /* dgb:10/19/97 */

   /* OFFSET Vectors w/subscript range: 1 to dimension */
   short *const Ibuf = &buffer_.ibuf[0] - 1;

   *status = 0;

   DEBUG1 = 0;  
 
   /*             Update pointer and check it */

L_10:
	buffer_.ip = buffer_.ip + 1;
	if( buffer_.ip == MAX_SHEF_INPUT + 1 )              /* dgb:09/10/97 */
		goto L_910;
	if( buffer_.ip > MAX_SHEF_INPUT )                   /* dgb:09/10/97 */
		goto L_40;

	/*  Get the character and update last character  */

	lchar = xchar_.ichar;
	*ichar = Ibuf[buffer_.ip];
	xchar_.ichar = *ichar;
        if ( DEBUG1 )
        {
          printf("\nxchar_.ichar = %c %d",xchar_.ichar,xchar_.ichar);
        }
	/*  Test for NUM_BLANKS consecutive blanks, end of line */

	if( *ichar != codes_.iblnk )
		goto L_30;
	if( lchar != codes_.iblnk )
		goto L_20;
	buffer_.nblnk = buffer_.nblnk + 1;
	if( buffer_.nblnk < NUM_BLANKS )                    /* dgb:01/29/98 */
 		goto L_30;
	buffer_.ip = MAX_SHEF_INPUT + 1;                    /* dgb:09/10/97 */
	goto L_910;

L_20:
	buffer_.nblnk = 1;

	/*  Check for a colon */

L_30:
	if( *ichar == codes_.icolon )                       /* dgb:09/10/97 */
        {                                                   /* dgb:09/10/97 */
           comment_.colon_on = 1;                           /* dgb:09/10/97 */
           goto L_35;                                       /* dgb:09/10/97 */
        }                                                   /* dgb:09/10/97 */  

        if ( *ichar == codes_.dquote )                      /* dgb:09/10/97 */
        {                                                   /* dgb:09/10/97 */  
           comment_.dquote_on = 1;                          /* dgb:09/10/97 */
           comment_.pos = 0;                                /* dgb:09/10/97 */
 	   buffer_.nblnk = 0;                               /* dgb:09/10/97 */
           goto L_35;                                       /* dgb:09/10/97 */
        }                                                   /* dgb:09/10/97 */

        i = 0;                                              /* dgb:09/10/97 */   
        itemp = buffer_.ip;                                 /* dgb:09/10/97 */ 
        while( Ibuf[itemp+i] == codes_.iblnk )              /* dgb:09/10/97 */
        {                                                   /* dgb:09/10/97 */
          i++;                                              /* dgb:09/10/97 */
          if ( i > NUM_BLANKS ) break;                      /* dgb:01/29/98 */
        }                                                   /* dgb:09/10/97 */

        if ( Ibuf[itemp+i] == codes_.dquote )               /* dgb:09/10/97 */  
            goto L_10;                                      /* dgb:09/10/97 */ 
  
        if ( Ibuf[itemp+i] == codes_.icolon )               /* dgb:09/10/97 */  
            goto L_10;                                      /* dgb:09/10/97 */

	goto L_900;                                         /* dgb:09/10/97 */  

L_35:
	buffer_.ip = buffer_.ip + 1;
	if( buffer_.ip == MAX_SHEF_INPUT + 1 )              /* dgb:09/10/97 */
		goto L_910;
	*ichar = Ibuf[buffer_.ip];
        if ( comment_.colon_on == 1 )                       /* dgb:01/02/00 */
        {
           if ( *ichar == codes_.dquote )                   /* dgb:01/02/00 */
                goto L_930;                                 /* dgb:01/02/00 */
           if ( *ichar == codes_.icolon )                   /* dgb:01/02/00 */
           {
              comment_.colon_on = 0;                        /* dgb:01/02/00 */
              goto L_10;                                    /* dgb:01/02/00 */
           }
           goto L_35;                                       /* dgb:01/02/00 */
        }

        if ( comment_.dquote_on )                           /* dgb:01/02/00 */
        {
           if ( *ichar == codes_.dquote )                   /* dgb:01/02/00 */
           {
              comment_.dquote_on = 0;                       /* dgb:01/02/00 */
              goto L_10;                                    /* dgb:01/02/00 */
           }
        }

        if ( comment_.dquote_on )                           /* dgb:09/10/97 */  
        {                                                   /* dgb:09/10/97 */  
           if ( *ichar == codes_.iblnk )                    /* dgb:09/10/97 */
           {                                                /* dgb:09/10/97 */
	      buffer_.nblnk = buffer_.nblnk + 1;            /* dgb:09/10/97 */
	      if( buffer_.nblnk > 14 )                      /* dgb:09/10/97 */
	    	  goto L_40;                                /* dgb:09/10/97 */
           }                                                /* dgb:09/10/97 */

           comment_.comment[comment_.pos] = *ichar;         /* dgb:09/10/97 */
           comment_.pos++;                                  /* dgb:09/10/97 */
           if ( comment_.pos > MAX_COMMENT_LENGTH )         /* dgb:09/10/97 */
              goto L_940;                                   /* dgb:09/10/97 */
        }                                                   /* dgb:09/10/97 */

	goto L_35;

	/*             Read the next line */

L_40:


    comment_.colon_on     = 0;
    comment_.dquote_on    = 0;
 
	for( i = 1; i <= MAX_SHEF_INPUT; i++ )                 /* dgb:09/10/97 */
		{
		Ibuf[i] = codes_.iblnk;
		}

        if ( (fgets( buff_temp,sizeof(buff_temp),fp_.lchn)) == NULL )
          goto L_920;
        for ( i = strlen(buff_temp) - 1; i < MAX_SHEF_INPUT + 1; i++) /* dgb:09/10/97 */
          buff_temp[i] =  32;

        for ( i = 0; i < MAX_SHEF_INPUT + 1; i++)              /* dgb:09/10/97 */
        {
          if ( buff_temp[i] == 13 ) 
               buff_temp[i] =  32;                             /* dgb:/04/01/99 */
          Ibuf[i+1] = toupper( buff_temp[i] );
          tempfiles_.last_line_read[i] = Ibuf[i+1];            /* dgb:05/30/96 */
        }

	for( i = MAX_SHEF_INPUT; i >= 1; i-- )                 /* dgb:09/10/97 */
        {
		if( Ibuf[i] != codes_.iblnk )
 			goto L_70;
	}

	goto L_75;

L_70:

    if ( !PARTIAL_ERROR )                                      /* dgb:05/30/96 */
    {
	   for( j = 1, _fst=1; (j <= i) || _fst; j++, _fst=0 )
	    	{
             fprintf( fp_.icher,"%c", Ibuf[j]);
		     }
       if ( fp_.icher )
       fprintf( fp_.icher, "\n");
    }

    if ( DOTEPRE )
       pre_process_dote();                                    /* dgb:01/27/98 */

L_75:
	buffer_.ip = 0;
	goto L_10;

	/*             Got it */

L_900:
	return;

	/*             NUM_BLANKS 		Consecutive blanks, end of line */

L_910:
	buffer_.nblnk = 0;
	*status = 1;
	return;

	/*             File read error */

L_920:

	*status = 2;
	return;

   /*             Embedded comment error */

L_930:                                                     /* dgb:09/10/97 */
   buffer_.nblnk = 0;                                      /* dgb:09/10/97 */ 
   i = 39;
   sherr(&i);                                              /* dgb:09/10/97 */
   *status = 1;                                            /* dgb:09/10/97 */
   return;                                                 /* dgb:09/10/97 */

   /*             Database comment too long */

L_940:                                                     /* dgb:09/10/97 */
   buffer_.nblnk = 0;                                      /* dgb:09/10/97 */ 
   i = 40;
   sherr(&i);                                              /* dgb:09/10/97 */
   *status = 1;                                            /* dgb:09/10/97 */
   return;                                                 /* dgb:09/10/97 */

} 
