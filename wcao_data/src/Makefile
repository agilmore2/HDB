#  The version for use with C and INGRES follows:

include ../../Environ
include make_include

######################### Directives ###############################

TARGET = $(HDB_APP_BIN)/wcao_data
OBJS = \
		$(OBJDIR)/wcao_data.o

main: $(TARGET)

clean: 
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

$(OBJDIR)/wcao_data.o: \
					wcao_data.pc 
	proc iname=wcao_data.pc include=$(HDB_APP_INCLUDE)
	$(CC) $(CFLAGS) -c wcao_data.c -o $@
