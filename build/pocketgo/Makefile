FLAVOR := release
# FLAVOR := debug
#DEVICE := a320
DEVICE := gcw0

#CHAINPREFIX := /opt/mipsel-linux-uclibc
#CROSS_COMPILE := $(CHAINPREFIX)/usr/bin/mipsel-linux-

#CHAINPREFIX=/opt/buildroot-bittboy/output/host
#CROSS_COMPILE=$(CHAINPREFIX)/bin/arm-linux-

CHAINPREFIX=/opt/miyoo
CROSS_COMPILE=$(CHAINPREFIX)/usr/bin/arm-linux-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
STRIP = $(CROSS_COMPILE)strip

SYSROOT     := $(shell $(CC) --print-sysroot)
SDL_CFLAGS  := $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
SDL_LIBS    := $(shell $(SYSROOT)/usr/bin/sdl-config --libs)

$(info Building for $(DEVICE) in $(FLAVOR) mode...)

SRC_PATH := ../..
OUTPUT_BASE := $(DEVICE)-$(FLAVOR)

PROJECT_NAME := pocketSpeccy
SRCFOLDERS = . z80 tools snapshot ui platform platform/custom_ui platform/linux \
		platform/sdl devices devices/fdd devices/input devices/sound \
		3rdparty/tinyxml2 3rdparty/zlib 3rdparty/minizip 3rdparty/libpng
CXXSRCS = $(foreach dir, $(SRCFOLDERS), $(wildcard $(SRC_PATH)/$(dir)/*.cpp))
CSRCS = $(foreach dir, $(SRCFOLDERS), $(wildcard $(SRC_PATH)/$(dir)/*.c))

DIST_PATH := $(OUTPUT_BASE)/dist
OBJ_PATH := $(OUTPUT_BASE)/obj
CXX_OBJS := $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(CXXSRCS))
CXX_DEPS := $(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.d,$(CXXSRCS))
C_OBJS := $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o,$(CSRCS))
C_DEPS := $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.d,$(CSRCS))
BIN_TARGET := speccy/$(PROJECT_NAME)
OPK_TARGET := speccy/speccy.opk

DESKTOP_ENTRY := speccy/default.$(DEVICE).desktop
ICON := speccy/speccy.png

#matholton changed this:
STD_OPTS := -Wall -c  -fno-builtin -fno-exceptions -fno-pic
#STD_OPTS := -G0 -Wall -c  -fno-builtin -fno-exceptions -fno-pic
ifeq ($(FLAVOR),debug)
STD_OPTS += -g -O0 -DDEBUG
else
#matholton changed this:
STD_OPTS += -O3 -fomit-frame-pointer -ffunction-sections -fsingle-precision-constant -finline-functions -DNDEBUG
#STD_OPTS += -O3 -fomit-frame-pointer -ffunction-sections -fsingle-precision-constant -mips32 -finline-functions -DNDEBUG
endif

#CXXFLAGS  = 	$(STD_OPTS) -fno-rtti -fno-threadsafe-statics \
#		-I$(SRC_PATH)/tools/zlib/ -I$(SRC_PATH)/tools/tinyxml/ \
#		-I$(SRC_PATH)/tools/libpng/ -I$(SRC_PATH)/3rdparty/tinyxml2/ \
#		-I$(SRC_PATH)/3rdparty/minizip/ \
#   $(SDL_CFLAGS) \
#	    -DSDL_USE_JOYSTICK -D_LINUX -DUSE_SDL -DSDL_KEYS_DINGOO -DGCWZERO

CXXFLAGS  = 	$(STD_OPTS) -fno-rtti -fno-threadsafe-statics \
		-I$(SRC_PATH)/tools/zlib/ -I$(SRC_PATH)/tools/tinyxml/ \
		-I$(SRC_PATH)/tools/libpng/ -I$(SRC_PATH)/3rdparty/tinyxml2/ \
		-I$(SRC_PATH)/3rdparty/minizip/ \
    $(SDL_CFLAGS) \
		-DSDL_USE_JOYSTICK -D_LINUX -DUSE_SDL -DSDL_KEYS_COMMON -DGCWZERO -DSDL_POCKETGO_KEYS
		
CFLAGS := $(STD_OPTS)
#EXTRA_LDFLAGS = -lasound -Wl,--as-needed -Wl,--gc-sections -flto -s
#matholton changed this
#LFLAGS := $(SDL_LIBS) -s -lz -lSDL -lpng $(EXTRA_LDFLAGS)
#then changed it back
LFLAGS := $(SDL_LIBS) -s -lz -lSDL -lpng 
# LFLAGS := $(SDL_LIBS) -lz -lSDL -lpng 

all: build

$(CXX_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(C_OBJS): $(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $<

#$(BIN_TARGET): $(CXX_OBJS) $(C_OBJS)
#	@mkdir -p $(@D)
#	$(CXX) $(LFLAGS) $^ -o$(BIN_TARGET)

$(BIN_TARGET): $(CXX_OBJS) $(C_OBJS)
	@mkdir -p $(@D)
	$(CXX) $^ -o$(BIN_TARGET) $(LFLAGS)


opk: $(BIN_TARGET) $(DESKTOP_ENTRY) $(ICON)
	mksquashfs $^ $(SRC_PATH)/res speccy/speccy.$@ -noappend -no-exports -no-xattrs -all-root

ipk: $(BIN_TARGET)
	@rm -rf /tmp/.speccy-ipk/ && mkdir -p /tmp/.speccy-ipk/root/home/retrofw/emus/speccy /tmp/.speccy-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators /tmp/.speccy-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems
	@cp -r speccy/speccy.dge speccy/speccy.png ../../res /tmp/.speccy-ipk/root/home/retrofw/emus/speccy
	@cp speccy/speccy.lnk /tmp/.speccy-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators
	@cp speccy/zxspectrum.speccy.lnk /tmp/.speccy-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems
	@sed "s/^Version:.*/Version: $$(date +%Y%m%d)/" speccy/control > /tmp/.speccy-ipk/control
	@cp speccy/conffiles speccy/postinst /tmp/.speccy-ipk/
	@tar --owner=0 --group=0 -czvf /tmp/.speccy-ipk/control.tar.gz -C /tmp/.speccy-ipk/ control conffiles
	@tar --owner=0 --group=0 -czvf /tmp/.speccy-ipk/data.tar.gz -C /tmp/.speccy-ipk/root/ .
	@echo 2.0 > /tmp/.speccy-ipk/debian-binary
	@ar r speccy/speccy.ipk /tmp/.speccy-ipk/control.tar.gz /tmp/.speccy-ipk/data.tar.gz /tmp/.speccy-ipk/debian-binary

build: $(BIN_TARGET)

clean:
	rm -rf $(CXX_OBJS)
	rm -rf $(CXX_DEPS)
	rm -rf $(C_OBJS)
	rm -rf $(C_DEPS)
	rm -rf $(BIN_TARGET)

.PHONY: mkdirs clean build all


