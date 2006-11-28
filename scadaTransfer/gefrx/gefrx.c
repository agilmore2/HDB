/******************************************************************************
*******************************************************************************
***                                                                         ***
***                                                                         ***
***          Contains confidential and proprietary information of           ***
***                   GE Energy Management Services, Inc.                   ***
***                                                                         ***
***                    (C) 2005 General Electric Company.                   ***
***                           All Rights Reserved                           ***
***                                                                         ***
*******************************************************************************
******************************************************************************/


#include "stdio.h"
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdarg.h>


/*
** The following macro enables or disables debug.  Please do not remove this 
** macro.  Simply change it from either define or undefine to include or 
** exclude debug from the application output.
*/
#undef DEBUG_ON



/*
** macro used to reverse the byte order
*/
#define ENDIAN(input)			\
			((((input) << 24L) & 0xff000000) | \
			 (((input) <<  8L) & 0x00ff0000) | \
			 (((input) >>  8L) & 0x0000ff00) | \
			 (((input) >> 24L) & 0x000000ff))


#define DEFAULT_BAUD_RATE		38400 

#define	MAX_FILENAME_LENGTH		80
#define MAX_OUTPUT_LENGTH 		500

#define	MAX_DATA_BUFFER_SIZE		512 

#define TEMP_FILE_EXT			"_tmp"


/*
** The following macros are used for debugging purposes.  If the macro
** DEBUG_ON is not defined these commands are compiled out of the code.
*/
#ifdef DEBUG_ON	

#define DEBUG0(fmt) 	 		\
printf(fmt);		

#define DEBUG1(fmt,parm1)  		\
printf(fmt,parm1);

#define DEBUG2(fmt,parm1,parm2) 	\
printf(fmt,parm1,parm2);

#define DEBUG3(fmt,parm1,parm2,parm3) 	\
printf(fmt,parm1,parm2,parm3);

#else

#define DEBUG0(fmt)
#define DEBUG1(fmt,parm1)
#define DEBUG2(fmt,parm1,parm2)
#define DEBUG3(fmt,parm1,parm2,parm3)

#endif



#define	GE_SUCCESS		TRUE
#define	GE_FAIL			FALSE

/*
** CRC32 Polynomial
*/
#define	CRC32_POLYNOMIAL		0x04c11db7



typedef enum 
{
	FALSE = 0,
	TRUE = 1
} bool;


/*
** The enumeration used to define the message type we either received or 
** are sending
*/
typedef enum
{
MIN_CMD,
CONNECT,
DISCONNECT,
SEND_FILE,
SEND_FILE_CONT,
SEND_FILE_COMPLETE,
ACK,
NACK,

MAX_CMD  /* Add new entries above */
} command_et;


typedef struct 
{
command_et		msg_type;
unsigned int	data_size;
unsigned int	file_crc;
char			filename[MAX_FILENAME_LENGTH];
unsigned char	data_buffer[MAX_DATA_BUFFER_SIZE];


}	serial_msg_st;


typedef struct 
{
command_et		msg_type;
}	ack_msg_st;

typedef struct
{
int		user_baud_rate;
int		user_device;
int		baud_rate;
char		dev[MAX_FILENAME_LENGTH];
char		output_dir[MAX_FILENAME_LENGTH];
}	cmd_line_st;


/*----------------------------------------------------------------------------*/

int  process_args(	int		argc, 
			char		*argv[], 
			cmd_line_st	*cmd_parms)

/*
PURPOSE:     The purpose of this function is to process the command line args.

DESCRIPTION: This function makes use of the UNIX getopt() function to parse 
             the command line arguments.

RETURNS:     This function returns the following:

             status - set to one of the following:

             GE_SUCCESS - if function completed successfully.
             GE_FAIL - if function encountered an error.

*/

{
char			char_val = 0;
int			status = GE_SUCCESS;

/*
** Process the arguments
*/
while ((char_val = getopt(argc, argv, "b:d:o:")) != EOF)
    {

    switch (char_val)
        {
	case 'b':
		{
            	DEBUG1("string baud rate =  %s\n", optarg);

		cmd_parms->baud_rate = atoi(optarg);

		if (cmd_parms->baud_rate == 0)
		{
			status = GE_FAIL;	
		}

		cmd_parms->user_baud_rate = TRUE;

            	DEBUG1("int baud rate =  %d\n", cmd_parms->baud_rate);


		break;
		}
	case 'd':
		{
            	DEBUG1("Serial device =  %s\n", optarg);

		/*
		** Get the serial device to listen to
		*/

		strncpy(cmd_parms->dev,optarg,MAX_FILENAME_LENGTH);

		cmd_parms->user_device = TRUE;
			
		
		break;
		}
	case 'o':
		{
            	DEBUG1("output directory = %s\n", optarg);

		/*
		** Check to see if the user gave us an absolute or relative 
		** path.  If the user gave us an implied path then add the 
		** relative notation to what the user gave us.
		*/
		if ((optarg[0] == '/') ||
		    (optarg[0] == '.'))
		{
			strncpy(cmd_parms->output_dir,
				optarg,
				MAX_FILENAME_LENGTH);
			
		}
		else
		{
			strcpy(cmd_parms->output_dir,"./");
			strncat(cmd_parms->output_dir,
				optarg,
				MAX_FILENAME_LENGTH-2);
		}
            	DEBUG1("output directory = %s\n", cmd_parms->output_dir);

		break;
		}
	default:
		{
		DEBUG0("ERROR: Invalid command line parameter\n");
		status = GE_FAIL;	
		break;
		}
        } 

    } /* End of while */

    return status;

}


/*----------------------------------------------------------------------------*/


unsigned int reflect(unsigned int input, unsigned char num_bits)

/*
PURPOSE:     The purpose of this function is to reflect the data as 
             described int the CRC32 specification.

DESCRIPTION: The purpose of this function is to reflect the data as
             described int the CRC32 specification.

RETURNS:     This function returns the following:

             return_value - the reflected unsigned integer based on the input

*/

{
	unsigned int	return_value = 0;
	int		count = 0;

	for (count = 1; count < (num_bits + 1); count++)
	{
		if (input & 1)
			return_value |= 1 << (num_bits - count);
		input >>= 1;
	}
	return return_value;
}



/*----------------------------------------------------------------------------*/

unsigned int calc_file_crc32 (char filename[])

/*
PURPOSE:     Calculates the CRC32 for the given filename.

DESCRIPTION: The purpose of this function is to calculate the CRC32 based on 
             the given file.  This calculation is completed using the look-up 
             table approach.

RETURNS:     This function returns the following:

             crc32 - the calculated crc32 value

*/

{
    int         file_fd;
    unsigned char       temp_var;
    static bool     crc_init_complete = FALSE;
    static unsigned int CRC_TABLE[256];
    unsigned int        crc32 = 0;
    unsigned int        lookup = 0;

    if (strlen(filename) == 0)
    {
        return 0;
    }


    if (!crc_init_complete)
    {
        unsigned int                crc;
        int                 byte_count;
        int                 bit_count;

        for (byte_count = 0; byte_count <= 255; byte_count++)
        {
            crc = reflect(byte_count,8) << 24;

            for (bit_count = 0; bit_count < 8; bit_count++)
            {
                /*if ((crc & (1 << 31)) == (1 << 31)) */
                if ((crc & 0x80000000) == 0x80000000)
                {
                    crc = ((crc << 1) ^ CRC32_POLYNOMIAL);
                }
                else
                {
                    crc = (crc << 1);
                }
            }
            CRC_TABLE[byte_count] = reflect(crc,32);

        }
        crc_init_complete = TRUE;
    }

    file_fd = open(filename, O_RDONLY);

    if (file_fd == -1)
    {
        return GE_FAIL;
    }

    crc32 = 0xffffffff;

    while (read(file_fd,&temp_var,1) == 1)
    {
        lookup = ((crc32 & 0xff) ^ temp_var );
        crc32 = (crc32 >> 8) ^ CRC_TABLE[lookup];

    }


    close(file_fd);

    return crc32 ^ 0xffffffff;
}



/*----------------------------------------------------------------------------*/


void send_ack(int	serial_fd)

/*
PURPOSE:     Sends the file transmit application an acknoledge 

DESCRIPTION: The purpose of this function is to send the transmit application 
             GEFTX an acknoledge response message across the serial port 
             defined by the given file descriptor.

RETURNS:     NONE

*/

{
	ack_msg_st			ack_msg;
	unsigned char		*tempPtr;
	int					count = 0;

	memset(&ack_msg,0,sizeof(ack_msg_st));

	ack_msg.msg_type = ACK;
	ack_msg.msg_type = htonl(ack_msg.msg_type);

	usleep(100000);

	tempPtr = (unsigned char *)&ack_msg;

	for (count = 0; count < sizeof(ack_msg_st) ; count++)
	{
		write(serial_fd,tempPtr,1);
		tempPtr++;
		/*sleep(1); */

	}

	DEBUG0("Sending Ack Msg.\n\n");

	return;
}
	

/*----------------------------------------------------------------------------*/

void send_nack(int	serial_fd)


/*
PURPOSE:     Sends the file transmit application a non-acknoledge

DESCRIPTION: The purpose of this function is to send the transmit application
             GEFTX a non-acknoledge response message across the serial port
             defined by the given file descriptor.

RETURNS:     NONE

*/

{
	ack_msg_st		nack_msg;

	memset(&nack_msg,0,sizeof(ack_msg_st));

	nack_msg.msg_type = NACK;
	nack_msg.msg_type = htonl(nack_msg.msg_type);

	write(serial_fd,&nack_msg,sizeof(ack_msg_st));

	DEBUG0("Sending NAck Msg.\n\n");

	return;
}


/*----------------------------------------------------------------------------*/

char *get_date_time( void )

/*
PURPOSE:     Returns the date and time in s character string.

DESCRIPTION: The purpose of this function is to retrieve the current systems 
             date and time in the format YYYY-MM-DD HH:MM:SS.

RETURNS:     This function returns the following:

             date_time_string - the current date and time in the above format

*/

{
   	time_t current_time;
	static char		date_time_string[80];

	memset(date_time_string,0,80);

	current_time = time(NULL);

	strftime(date_time_string, 
		 80,
		 "%Y-%m-%d %H:%M:%S", 
		 localtime(&current_time));

	return date_time_string;
}

/*----------------------------------------------------------------------------*/
void output ( 	FILE 		*output_fd, 
		char		*params, ...)

/*
PURPOSE:     Send output to the specified file descriptor.

DESCRIPTION: The purpose of this function is to send data to the specified 
             file descriptor.  Usually stdout, stderr, etc.


RETURNS:     NONE

*/

{
   char       output_string[MAX_OUTPUT_LENGTH];
   va_list    arglist;
   int        param_len = 0;


   /* Setup the argument list. */
   va_start(arglist,params);

   /* Write the data to a buffer. */
   param_len = vsprintf(output_string, params, arglist);

   va_end(arglist);

   fprintf(output_fd,"%s - %s",get_date_time(),output_string);

   return;
}


/*----------------------------------------------------------------------------*/

main(	int	argc,
	char	*argv[])

/*
PURPOSE:     This application received files from the transmit application 
             over the system serial port.

DESCRIPTION: The application opens the user specified serial port (typically 
             /dev/ttya or /dev/ttyb for Solaris machines) and awaits commands 
             from the master application GEFTX.  The main purpose of the 
             application is to transfer files from GEFTX with no posibility 
             to send data from this application.  A CRC32 is calculated by 
             GEFTX as well as GEFRX and then compared this way we ensure 
             that the input file is identical to the file we received.  A 
             typical file transfer would consist of the following:

                GEFTX                  GEFRX
                -----                  -----
                CONNECT                ACK
                SEND_FILE              ACK
                SEND_FILE_CONT         ACK
                    .                   .
                    .                   .
                    .                   .
                SEND_FILE_COMPLETE     ACK or NAK based on the CRC32 compare




RETURNS:     NONE

*/


{
	int				serial_fd;
	int				file_fd;
	int				baud_rate = 0;
	int				bytes_read;
	int				frame_bytes_read;
	bool			complete = FALSE;
	serial_msg_st	msg;
   	struct termios 	options;
	unsigned char	*tempPtr;
	unsigned int	local_crc;
	int				status = GE_SUCCESS;
	cmd_line_st		cmd_parms;
	char			temp_filename[MAX_FILENAME_LENGTH * 2];


	DEBUG0("Entering GEFRX\n");

	fprintf(stdout,"\n\n\n");
	output(stdout,"Starting GEFRX\n");


	/*
	** Zero out the default command line parameters
	*/
	memset(&cmd_parms,0,sizeof(cmd_line_st));

	/*
	**  Set the default baud rate
	*/
	cmd_parms.baud_rate = DEFAULT_BAUD_RATE;

	/*
	** If there were arguments passed in
	*/
	if (argc > 1)
	{

		if ( process_args(argc, argv, &cmd_parms) != GE_SUCCESS)
		{
			DEBUG0("ERROR: "
				"Could not process command line arguments\n");
			status = GE_FAIL;

		}

	} /* End of options entered */
	else
	{
		/*
		** No options entered
		*/

		DEBUG0("ERROR: No options entered. \n");


		status = GE_FAIL;

	} /* End of no options entered */



	switch (cmd_parms.baud_rate)
	{
	case 300:
		{
		baud_rate=B300;
		break;
		}
	case 600:
		{
		baud_rate=B600;
		break;
		}
	case 1200:
		{
		baud_rate=B1200;
		break;
		}
	case 1800:
		{
		baud_rate=B1800;
		break;
		}
	case 2400:
		{
		baud_rate=B2400;
		break;
		}
	case 4800:
		{
		baud_rate=B4800;
		break;
		}
	case 9600:
		{
		baud_rate=B9600;
		break;
		}
	case 19200:
		{
		baud_rate=B19200;
		break;
		}
	case 38400:
		{
		baud_rate=B38400;
		break;
		}
	default:
		{
		/*
		**  The following speeds have not been tested: 
		**  B57600 B76800 B115200
		*/
		DEBUG0("Invalid baud rate.\n");
		status = GE_FAIL;
		break;
		}

	}


	/*
	**  We are requiring the user to specify the serial device. If it
	**  is not specified on the command line, it is an error so display 
	**  the user prompt.
	*/
	if (cmd_parms.user_device)
	{
		/*
		** Opening the serial device 
		*/
		output(stdout, "Opening serial device: %s\n", cmd_parms.dev);

		/*serial_fd = open(cmd_parms.dev, O_RDWR | O_NOCTTY | O_NDELAY ); */
		serial_fd = open(cmd_parms.dev, O_RDWR | O_NOCTTY | O_NDELAY | O_RSYNC);

		if (serial_fd == -1)
		{
			/*
			** Could not open the port.
			*/
			output(stderr,
				"open_port: Unable to open %s - ",
				cmd_parms.dev);
			status = GE_FAIL;
		}

	}
	else
	{
		output(stderr,
			"User must specify the serial device.  "
			"See usage below.\n ");
		status = GE_FAIL;
	}

	if ((status != GE_SUCCESS) && (!cmd_parms.user_device))
		{
		fprintf(stderr, "\n\n\nGEFRX Application - Version 1.0.0\n\n\n");

		fprintf(stderr,
			"usage: gefrx -d <serial device> [-o <output dir>] "
			"[-b <baud rate>]\n\n");

		fprintf(stderr,
			"The application gefrx receives data files from the "
			"matting\n"
 			"MS Windows application GEFTX over the specified "
			"serial \n"
			"device.\n\n");

		fprintf(stderr,
			"-d     Serial Device - Specifies the serial device "
			"to open \n"
			"                       which is connected to "
			"GEFTX. \n\n");
		fprintf(stderr,

			"                       Linux Example - /dev/ttyS0\n"
			"                       Solaris Example - "
			"/dev/ttya\n\n");

		fprintf(stderr,
			"-o     Output Directory - Specifies the directory to\n"
			"                          place the received file. "
			"The\n"
                        "                          current directory is the \n"
			"                          default.\n\n");

		fprintf(stderr,
			"-b     Baud Rate - Specifies the communication baud "
			"rate.\n"
			"                   This rate must match the GEFTX \n"
                        "                   application.  The default is "
			"38400.\n\n");

		fprintf(stderr,"            Valid Baud Rates:\n");
		fprintf(stderr,"            -----------------\n");

       		fprintf(stderr,"                    300\n");
       		fprintf(stderr,"                    600\n");
       		fprintf(stderr,"                   1200\n");
       		fprintf(stderr,"                   1800\n");
       		fprintf(stderr,"                   2400\n");
       		fprintf(stderr,"                   4800\n");
       		fprintf(stderr,"                   9600\n");
       		fprintf(stderr,"                  19200\n");
       		fprintf(stderr,"                  38400\n");
		fprintf(stderr, "\n\n\n");

		exit(-1);
		}



	/*fcntl(serial_fd, F_SETFL, 0); */
	fcntl(serial_fd, F_SETFL, O_APPEND);


	/*
	** Get the current options for the port...
	*/
	tcgetattr(serial_fd, &options);

	/*
	** Set the baud rates
	*/
	output(stdout, "Setting Baud Rate to: %d\n", cmd_parms.baud_rate);


	cfsetispeed(&options, baud_rate);
	cfsetospeed(&options, baud_rate);


	/*
	** Enable the receiver and set local mode...
	*/
	options.c_cflag |= (CLOCAL | CREAD);

	/*
	** Set Data Bits 8, Parity None, Stop Bits 1
	*/
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/*
	** Disable software flow control
	*/
	options.c_iflag &= ~(IXON | IXOFF | IXANY); 
	options.c_iflag &= ~(ISTRIP); 

	/*
	** Disable CR mapped to NL
	*/
	options.c_iflag &= ~(ICRNL); 

	options.c_cc[VEOL] = 0;
	options.c_cc[VMIN] = 1;
	options.c_cc[VTIME] = 0;

	/*
	** Enable raw mode transfer
	*/
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);




	/*
	** Set the new options for the port after flushing the input 
	** and output buffers.
	*/

	tcsetattr(serial_fd, TCSAFLUSH, &options);


	while (!complete)
	{
		frame_bytes_read = 0;
		tempPtr = (unsigned char *) &msg;

		DEBUG0("Waiting for serial data.\n");

		while (frame_bytes_read < sizeof(serial_msg_st))
		{
			bytes_read = read(	serial_fd,
						(unsigned char*)tempPtr+
						frame_bytes_read,
						1);

			DEBUG1("%02x ",*(tempPtr+frame_bytes_read));

			frame_bytes_read += bytes_read;

			/*
			** Output debug only in the first serial character
			*/
			if (frame_bytes_read == 1)
			{
				DEBUG0("Received serial data.");
			}

		}

		/*
		** Check to see if the user specified an output directory
		*/
		if (strlen(cmd_parms.output_dir) > 0)
		{
			strncpy(temp_filename,
				cmd_parms.output_dir,
				MAX_FILENAME_LENGTH);
			strcat(temp_filename,"/");
		}
		else
		{
			memset(temp_filename,0,MAX_FILENAME_LENGTH);
		}
		

		strncat(temp_filename,msg.filename,MAX_FILENAME_LENGTH);

		if (strlen(temp_filename) > MAX_FILENAME_LENGTH)
		{
			output(stderr,
				"ERROR: Filename with path too long. "
				"Using default path.\n");
			strcpy(temp_filename,"./");
			strncat(temp_filename,msg.filename,MAX_FILENAME_LENGTH);
		}

		strcat(temp_filename,TEMP_FILE_EXT);

		strncpy(msg.filename,temp_filename,MAX_FILENAME_LENGTH);

		msg.msg_type = htonl(msg.msg_type);
		msg.data_size = htonl(msg.data_size);
		msg.file_crc = htonl(msg.file_crc);

		DEBUG2("Received messssage ID: %d (%d bytes)\n",
			msg.msg_type,frame_bytes_read);

		switch (msg.msg_type)
		{
		case CONNECT:
			{
			DEBUG0("Received CONNECT message.\n");
			output(stdout,"Received CONNECT message.\n");
			send_ack(serial_fd);
			break;
			}
		case DISCONNECT:
			{
			DEBUG0("Received DISCONNECT message.\n");
			output(stdout,"Received DISCONNECT message.\n");
			break;
			}
		case SEND_FILE:
			{
			DEBUG0("Received SEND_FILE message.\n");

			memset(temp_filename,0,MAX_FILENAME_LENGTH);
			strncpy(temp_filename,
				msg.filename,
				(strlen(msg.filename) - strlen(TEMP_FILE_EXT)));
			
			DEBUG3("Received %d (0x%x) Bytes of file: \"%s\".\n",
				msg.data_size,msg.data_size,temp_filename);

			output(stdout,
				"Receiving file: \"%s\"\n", temp_filename);
			
			if (file_fd != 0)
			{
				close(file_fd);
				file_fd = 0;
			}

			file_fd = open(msg.filename,O_CREAT | 
					O_WRONLY | O_TRUNC);

			fchmod(file_fd,
				S_IRUSR | 
				S_IWUSR | 
				S_IRGRP | 
				S_IWGRP | 
				S_IROTH ); 

			write(file_fd,msg.data_buffer,msg.data_size);

			send_ack(serial_fd);
			break;
			}
		case SEND_FILE_CONT:
			{
			DEBUG0("Received SEND_FILE_CONT message.\n");
	
			memset(temp_filename,0,MAX_FILENAME_LENGTH);
			strncpy(temp_filename,
				msg.filename,
				(strlen(msg.filename) - strlen(TEMP_FILE_EXT)));

			DEBUG3("Received %d (0x%x) Bytes of file: \"%s\".\n",
				msg.data_size,msg.data_size,temp_filename);

			/*
			** Write the file data received to disk.
			*/
			write(file_fd,msg.data_buffer,msg.data_size);

			send_ack(serial_fd);
			break;
			}
		case SEND_FILE_COMPLETE:
			{
			DEBUG0("Received SEND_FILE_COMPLETE message.\n");

            DEBUG1("Received file: \"%s\".\n",msg.filename);

			memset(temp_filename,0,MAX_FILENAME_LENGTH);
			strncpy(temp_filename,
				msg.filename,
				(strlen(msg.filename) - strlen(TEMP_FILE_EXT)));

			output(stdout,
				"File receive Complete: \"%s\"\n\n", 
				temp_filename);

			close(file_fd);
			file_fd = 0;

			local_crc = calc_file_crc32(msg.filename);

			DEBUG2("Local CRC32 = 0x%x : Windows CRC32 = 0x%x\n",
				local_crc,msg.file_crc);

			if (msg.file_crc == local_crc)
			{
				DEBUG1("strlen(msg_filename) = %d\n",
					strlen(msg.filename));
				DEBUG1("strlen(TEMP_FILE_EXT) = %d\n",
					strlen(TEMP_FILE_EXT));
				DEBUG2("Renaming \"%s\" to \"%s\"\n",
					msg.filename,temp_filename);

				rename(msg.filename,temp_filename);
				send_ack(serial_fd);
			}
			else
			{
				send_nack(serial_fd);
				output(stderr,
					"ERROR: Invalid checksum for "
					"file: \"%s\".\n", 
					msg.filename);
			}
			break;
			}
		case ACK:
			{
			DEBUG0("Received ACK message.\n");
			break;
			}
		case NACK:
			{
			DEBUG0("Received NACK message.\n");
			break;
			}
		default:
			{
			send_nack(serial_fd); 
			DEBUG0("ERROR: Received invalid message from sender\n");
			break;
			}
		}
	}
	


	DEBUG0("Exiting gefrx\n");



}






