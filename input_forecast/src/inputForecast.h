/* to modify the sites processed you must add element to the valid_sites array and change NUM_SITES */ 

#define NUM_SITES 9

typedef struct stuff {
  ID site_id;
  ID total_datatype_id;
  ID ave_datatype_id;
} STUFF;


STUFF valid_sites[] =
{
  {912,30,29},{913,34,33},{914,34,33},{915,34,33},{916,30,29},{917,34,33},{919,34,33},{920,124,123},{933,30,29}
};

int checkforModelrunid(
#ifdef ANSIPROTO
ID,
SQL_DATE,
int,
char *,
int *
#endif
);

int insertModeldata(
#ifdef ANSIPROTO
int,
int,
char *,
char *,
float,
char
#endif
);

int NumDaysMonth(
#ifdef ANSIPROTO
SQL_DATE,
int *
#endif
);

void modify_to_lower(
#ifdef ANSIPROTO
char *
#endif
);

void modify_to_upper(
#ifdef ANSIPROTO
char *
#endif
);
