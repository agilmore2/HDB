#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/col_average
OBJS = \
		$(OBJDIR)/col_average.o

main: $(TARGET)

clean: 
	rm $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

$(OBJDIR)/col_average.o: \
					col_average.pc 
	proc iname=col_average.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c col_average.c -o $@
