#  The version for use with C and INGRES follows:

include /wrg/hdb/HDB_applications/Environ
include make_include

######################### Directives ###############################

TARGET = gentea
OBJS = readdata.o teacup.o gentea.o

main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

readdata.o: \
	readdata.pc 
	proc iname=readdata.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c readdata.c -o $@

gentea.o: gentea.c
	$(CC) $(CFLAGS) -c gentea.c -o $@

teacup.o: teacup.c 
	$(CC) $(CFLAGS) -c teacup.c -o $@

install: $(TARGET)
	cp $(TARGET) ../bin
