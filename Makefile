RTEMS_MAKEFILE_PATH=/opt/rtems4.11/i386-rtems4.11/pc686/

PGM=${ARCH}/main.exe

# optional managers required
MANAGERS=all

# C source names
CSRCS = main.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

OBJS= $(COBJS) $(CXXOBJS) $(ASOBJS)

all:    ${ARCH} $(PGM)

$(PGM): $(OBJS)
	$(make-exe)

.PHONY: run
run:
	cp o-optimize/main.exe ../run/hda/main.exe
	make -C ../run
