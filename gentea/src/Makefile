#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/gentea
OBJS = \
	$(OBJDIR)/readdata.o \
	$(OBJDIR)/teacup.o \
	$(OBJDIR)/gentea.o

main: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

$(OBJDIR)/readdata.o: \
	readdata.pc 
	proc iname=readdata.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c readdata.c -o $@

$(OBJDIR)/gentea.o: gentea.c
	$(CC) $(CFLAGS) -c gentea.c -o $@

$(OBJDIR)/teacup.o: teacup.c 
	$(CC) $(CFLAGS) -c teacup.c -o $@

