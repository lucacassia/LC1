################################################################################
#
# Makefile to compile and link C programs
#
# Version valid for Linux machines
#
# "make" compiles and links the specified main programs and modules
# using the specified libraries (if any), and produces the executables
# 
# "make clean" removes all files created by "make"
#
# Dependencies on included files are automatically taken care of
#
################################################################################

all: rmxeq mkdep mkxeq rmpmk
.PHONY: all


# main programs and required modules 

MAIN = graph test

MODULES = shader_utils


# search path for modules

MDIR = ../../modules/extras

VPATH = $(MDIR)


# additional include directories

INCPATH = ../../include


# additional libraries to be included 
 
LIBS = m glut GL GLEW GLU

LIBPATH = 


# scheduling and optimization options (such as -DSSE -DSSE2 -DP4)
 
CFLAGS = -Wall -O -g -DSSE2
 

############################## do not change ###################################

SHELL=/bin/bash

CC=gcc

PGMS= $(MAIN) $(MODULES)

INCDIRS = $(addprefix -I,$(INCPATH))

OBJECTS = $(addsuffix .o,$(MODULES))

LDFLAGS = $(addprefix -L,$(LIBPATH)) $(addprefix -l,$(LIBS))

# Mac OS alternate link options
ifeq "$(OSTYPE)" "Darwin"
	LDFLAGS	= -framework Carbon -framework OpenGL -framework GLUT -lm -lGLEW
endif

-include $(addsuffix .d,$(PGMS))


# rule to make dependencies

$(addsuffix .d,$(PGMS)): %.d: %.c Makefile
	@ $(CC) -MM -ansi $(INCDIRS) $< -o $@


# rule to compile source programs

$(addsuffix .o,$(PGMS)): %.o: %.c Makefile
	$(CC) $< -c $(CFLAGS) $(INCDIRS) -o $@


# rule to link object files

$(MAIN): %: %.o $(OBJECTS) Makefile
	$(CC) $< $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $@


# produce executables

mkxeq: $(MAIN)


# remove old executables and old error log file

rmxeq:
	@ -rm -f $(MAIN); \
        echo "delete old executables"		


# make dependencies

mkdep:  $(addsuffix .d,$(PGMS))
	@ echo "generate tables of dependencies"


# clean post-compilation 

rmpmk:
	@ -rm -rf *.d *.o .tmp


# clean directory 

clean:
	@ -rm -rf *.d *.o .tmp *~ $(MAIN)
.PHONY: clean

################################################################################
