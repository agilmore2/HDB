#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/scadaData
OBJS = \
		$(OBJDIR)/scadadata.o \
		$(OBJDIR)/retrieve_scada_data.o

main: $(TARGET)

clean: 
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

$(OBJDIR)/retrieve_scada_data.o: retrieve_scada_data.pc
	proc iname=retrieve_scada_data.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c retrieve_scada_data.c -o $@


$(OBJDIR)/scadadata.o: scadadata.c
	$(CC) $(CFLAGS) -c $? -o $@

