/* --------------------------------------------------------------------
  
       FUNCTION
         strpat

       PURPOSE  
         Search for string patterns, using template characters,
         for numbers and/or alpha characters.

       VERSION and UPDATES
         1.0    DEC  95  David G. Brandon
                Original Version
 *--------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>



			/* *********************** */
			/* pattern search function */
			/* *********************** */

strpat(char buffer[], char token[] , int numtemp, int chartemp )
{

int token_len, buffer_len, cutoff_len;
int i,j,k;

token_len  = strlen(token);
buffer_len = strlen(buffer);
cutoff_len = buffer_len - token_len;

i = 0;
   while ( i < cutoff_len +1 )
   {
      j = 0;
      k = i;

      while ( j < token_len )
      {

         if (  token[j] == numtemp  )
         {

            if ( buffer[k] != '-' )
            {
               if ( buffer[k] != '.' )
               {
                  if ( !isdigit(buffer[k]) )
                  {

                     if (  token[j] != buffer[k]  )
                        break;
                  }
               }
            }

         }
         else
         if (  token[j] == chartemp  )
         {

               if ( !isalpha(buffer[k]) )
               {

                  if (  token[j] != buffer[k]  )
                     break;
               }

         }
         else
         {
         if (  token[j] != buffer[k]  )
               break;
         }

         j++;
         k++;

         if ( j == token_len )
         {
            return i; 
         }

      }   

      i++;
   }

   return -1;

}
