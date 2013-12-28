TARGET = ya2d
OBJS = main.o yapong.o

INCDIR =
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lya2d -ltinyfont -lpspdisplay -lpspgum -ljpeg -lpspgu -lpng -lz -lm -lpspvram -lpspmath

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = yapong

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak


copy: EBOOT.PBP
	mkdir -p /media/$(USER)/disk/PSP/GAME/$(notdir $(CURDIR))
	cp EBOOT.PBP /media/$(USER)/disk/PSP/GAME/$(notdir $(CURDIR))
	sync
	
