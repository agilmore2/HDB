/*Translated by FOR_C, v3.4.2 (-), on 04/06/95 at 20:26:05 */
/*FOR_C Options SET: c=2 com=u do=r4 ftn=2ln6kk op=iv s=dvn str=l sq=i */

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


pre_process_dote()
{

    int i, k, j, ii, iii, notfound, nullit;
    int l; 
    int DEBUG4;  
 short *const Ibuf = &buffer_.ibuf[0] -1 ;                                     
    memset(tempfiles_.temp1,0,sizeof(tempfiles_.temp1));                          
    memset(tempfiles_.temp2,0,sizeof(tempfiles_.temp2));                         
    DEBUG4 = 0;

    notfound = 1;

    ii = 0;

    if ( DEBUG4 )
    {
          for ( i=0; i< MAX_SHEF_INPUT; i++) 
           printf("%c", Ibuf[i+1]);
  
    }

          /* determine if first line or .E with not continuation */
          if ( ( Ibuf[1] == codes_.idot ) &&
               ( Ibuf[2] == codes_.iche ) &&
               ( Ibuf[3] == codes_.iblnk )
             ) 
          { 

                efound = 0;
                econtinuation = 0;
                find_dotend();
          }
          else
          {

             if ( 
                  ( Ibuf[1] == codes_.idot ) &&
                  ( Ibuf[2] == codes_.iche ) &&
                  ( Ibuf[3] >= codes_.ich0 && Ibuf[3] <= codes_.ich9 )
                )
             {

                if ( !efound )
                { 

                  efound_it();
                  remove_slash();
                  econtinuation = 1;
                }
                else
                {
 
                   find_dotbegin();
                   if ( !dotend && !dotbegin )
                   {
                   }
                   else
                   if ( dotend && !dotbegin )
                   {
                   } 
                   else 
                   if ( !dotend && dotbegin && econtinuation==0 )
                   {
                      /* remove leading slash */

                      remove_slash();
                      econtinuation = 1;
 
                      for ( i=0; i< MAX_SHEF_INPUT; i++)
                            printf("%c", Ibuf[i+1]);

                   }            

                   find_dotend();
                }
             }
 
          } 
           
          for ( i=0; i< MAX_SHEF_INPUT; i++)
               tempfiles_.temp1[i] = Ibuf[i+1];                        /* dgb:10/05/98 */
          null_it();

}

find_dotend()
{
int DEBUG1, i;
    short *const Ibuf = &buffer_.ibuf[0] - 1;

            DEBUG1 = 0;
            dotend = 0;



            i = MAX_SHEF_INPUT;

            while ( Ibuf[i] == ' ' )
               i--;
               

            if ( Ibuf[i] == '/' )
                dotend = 1;

}

find_dotbegin()
{
int i, notfound, ii,iii;
  short *const Ibuf = &buffer_.ibuf[0] - 1;




             /* check for beginning slash */
             dotbegin = 0;
             i = 4;
             while( notfound )
             {
                if ( Ibuf[i] == codes_.iblnk )
                {  
                }
                else
                if ( Ibuf[i] == codes_.islash )
                {
                     dotbegin = 1;
                     /*  Ibuf[i] = ' ';   */                       
                     notfound = 0; 
                }
                else
                if ( Ibuf[i] == codes_.icolon )
                {
                   i++;
                   iii = 0;
                    {
                       i++;
                       if ( iii > 5000 )
                            notfound = 0;
                       iii++;
                    }
                
                }
                else
                {
                    notfound = 0;
                }
                i++;
                ii++;
                if ( ii > 5000 ) 
                {
                    notfound = 0;
                }
             }
             

}

efound_it()
{
int i, notfound, ii,iii;
  short *const Ibuf = &buffer_.ibuf[0] - 1;



           
  /* check for first valid record */
             i = 4;
             while( notfound )
             {
                if ( Ibuf[i] == codes_.iblnk )
                {  
                }
                else
                if ( Ibuf[i] == codes_.islash )
                {
                     efound  = 1;
                     notfound = 0; 
                }
                else
                if ( Ibuf[i] == codes_.icolon )
                {
                   i++;
                   iii = 0;
                    while( Ibuf[i] != codes_.icolon )
                    {
                       i++;
                       if ( iii > 5000 )
                            notfound = 0;
                       iii++;
                    }
                
                }
                else
                {
                    efound = 1;
                    notfound = 0;
                }
                i++;
                ii++;
                if ( ii > 5000 ) 
                {
                    notfound = 0;
                }
             }
}



remove_slash()
{
int i, notfound, ii, iii;
   short *const Ibuf = &buffer_.ibuf[0] - 1; 



             notfound = 1;
             i = 4;
             while( notfound )
             {

                if ( Ibuf[i] == codes_.iblnk )
                {  
                }
                else
                if ( Ibuf[i] == codes_.islash )
                {
                     Ibuf[i] = ' ';                       
                     notfound = 0; 
                }
                else
                if ( Ibuf[i] == codes_.icolon )
                {
                   i++;
                   iii = 0;
                    while( Ibuf[i] != codes_.icolon )
                    {
                       i++;
                       if ( iii > 5000 )
                            notfound = 0;
                       iii++;
                    }
                  
                }
                else
                {
                    notfound = 0;
                }
                i++;
                ii++;
                if ( ii > 5000 ) 
                {
                    notfound = 0;
                }
             }

}

null_it()
{

int i,k,kk,ii,nullit;
  short *const Ibuf = &buffer_.ibuf[0] - 1;


    i      = 0;
    k      = 0;
    ii     = 0;
    nullit = 0;

    kk = strlen(tempfiles_.temp1);

    while ( i < kk ) 
    {
       if ( tempfiles_.temp1[i] == ' ' )
       {
            tempfiles_.temp2[k] = ' ' ;
            k++;
            i++;
       }
       else     
       if ( tempfiles_.temp1[i] != '/' )
       {

            tempfiles_.temp2[k] = tempfiles_.temp1[i];
            nullit = 0;
            k++;
            i++;
       }
       else
       if ( tempfiles_.temp1[i] == '/' && nullit == 0 )
       {
           tempfiles_.temp2[k] = tempfiles_.temp1[i];
           k++;
           i++;
           nullit = 1;
       }
       else
       if ( tempfiles_.temp1[i] == '/' && nullit == 1 )
       {
              tempfiles_.temp2[k] = 'N';
              k++;
              tempfiles_.temp2[k] = '/';
              k++;
              i++;
              nullit = 1;
       }
       ii++;

       if ( ii > 5000 )    /* check for endless loop */
       {
          i = 10000;
       }

      if ( i == kk)
          i   = 10000;
       if ( k == kk )
          i = 10000;

     }

    for ( i = 0; i < MAX_SHEF_INPUT; i++)        
          Ibuf[i+1] =  tempfiles_.temp2[i];
   

}
