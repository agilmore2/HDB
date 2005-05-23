#define FONTNEL 0
#define FLAMING 1
#define TAYLOR 2
#define BLUE 3
#define MORROW 4
#define CRYSTAL 5
#define VALLECITO 6
#define NAVAJO 7
#define POWELL 8
#define MEAD 9
#define MOHAVE 10
#define HAVASU 11
#define HOOVER 0
#define DAVIS 1
#define PARKER 2
#define UBPOWER 3
#define MAX_NO_COLS 13
#define N_OF_SITES 12
#define N_OF_POWER 4

#define WAT_FILE "res_file.in"
#define POW_FILE "power.in"
#define WATER "24MoReport.out"
#define POWER "24moPower.out"

#define TIMELENGTH 30
typedef struct   {
			char  column_head[50];
			float values[40];
		} columndata;

typedef struct  {
			int site_id;
			int datatype_id;
			int site_datatype_id;
		} ids;
typedef struct   {
			int unit;
			int scale;
		 } uni_scale;

struct  heading {
			char column[MAX_NO_COLS][20];
	         };

