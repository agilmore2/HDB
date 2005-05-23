PROGRAM = shef.lib.a
XDIR = /usr/X11
XLIBDIR = $(XDIR)
CFLAGS = -I$(XDIR)/include 

#CC = cc -Aa
CC = gcc -g

AR = ar r

#INSTALLDIR = $(XDIR)

SRC = blockdata.c checkparm.c date_time.c getparm.c  irang.c nextch.c \
      shbfr.c shbfw.c shcal.c shdeca.c shdecb.c shdece.c \
      shdotb.c shdrive.c shdtype.c shefclose.c  sherr.c sherrp.c \
      shfor.c shint.c shleap.c shlocl.c shpcode.c shpos.c shreal.c shtadj.c \
      shtdat.c shefop.c shefcl.c ouptime.c shef_test.c strpat.c \
      suspend_it.c is_file_closed.c stats_utilities.c  \
      dotepre.c pause_it.c error_log.c

OBJ = blockdata.o checkparm.o date_time.o getparm.o irang.o nextch.o \
      shbfr.o shbfw.o shcal.o shdeca.o shdecb.o shdece.o \
      shdotb.o shdrive.o shdtype.o shefclose.o sherr.o sherrp.o \
      shfor.o shint.o shleap.o shlocl.o shpcode.o shpos.o shreal.o shtadj.o \
      shtdat.o shefop.o shefcl.o ouptime.o shef_test.o strpat.o \
      suspend_it.o is_file_closed.o stats_utilities.o  \
      dotepre.o pause_it.o error_log.o

$(PROGRAM): $(OBJ)
	@echo "Making lib"
	$(AR) $(PROGRAM) $(OBJ) $(LIBS) $(LOCAL_LIB)  

clean: 
	rm $(OBJ) $(PROGRAM)


testlib: $(OBJ)
	$(CC) -o test.exe $(OBJ) $(LIBS) $(LOCAL_LIB)
