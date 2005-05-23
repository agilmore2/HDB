include ../../Environ
include make_include

TARGET = $(HDB_APP_ROOT)/test/testfile
OBJS = $(OBJDIR)/testfile.o

.PHONY: main
main: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(OBJS)	

#        TARGET DEFINITIONS

$(OBJDIR)/testfile.o: testfile.c\
		$(HDB_APP_INCLUDE)/defines.h \
		$(HDB_APP_INCLUDE)/dbutils.h 
	$(CC) $(CFLAGS) -c testfile.c -o $@
