#  The version for use with C and Oracle follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/inflow_24mo
OBJS = \
		$(OBJDIR)/get_monthly_model_data.o \
		$(OBJDIR)/checkforModelrunid.o \
		$(OBJDIR)/inflow_24mo.o

main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

$(OBJDIR)/get_monthly_model_data.o: \
					get_monthly_model_data.pc \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	proc iname=get_monthly_model_data.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c get_monthly_model_data.c -o $@

$(OBJDIR)/checkforModelrunid.o: \
					../../input_forecast/src/checkforModelrunid.pc \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	proc iname=../../input_forecast/src/checkforModelrunid.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c ../../input_forecast/src/checkforModelrunid.c -o $@

$(OBJDIR)/inflow_24mo.o: \
					inflow_24mo.c \
					inflow_24mo.h \
					$(HDB_APP_LIB)/libhdb.a \
					$(HDB_APP_LIB)/libutils.a
	$(CC) $(CFLAGS) -c inflow_24mo.c -o $@

clean:
	rm -f $(OBJDIR)/get_monthly_model_data.o $(OBJDIR)/checkforModelrunid.o $(OBJDIR)/inflow_24mo.o $(TARGET)
