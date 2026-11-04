TARGET = GodOfStrength
OBJS = main.o
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LIBDIR =
LDFLAGS =
LIBS = -lpspdebug -lpspdisplay -lpspge -lpspctrl -lpspsdk -lc -lpspnet -lpspnet_inet -lpsputility -lpspuser -lpspkernel
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = God Of Strength
PSP_EBOOT_ICON = IMG-20261104-WA1729.jpg
PSP_EBOOT_PIC1 = IMG-20261104-WA5109.jpg
PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
