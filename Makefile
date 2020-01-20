# Binary name.
NAME = its1975andthismanisabouttoshowyouthefuture

# Object files, for the ARM9 binary and the ARM7 binary
OBJS = Main.o DS3D.o Utils.o nitrofs.o effect0.o effect1.o effect2.o effect3.o \
effect4.o effect5.o effect6.o effect7.o VoxelBlock.o DS3Dextra.o

OBJS7 = Main.arm7.o

# Libraries to link into the respective binaries
LIBS = -L$(DEVKITPRO)/libnds/lib -lfat -lnds9 -lm -lmm9
LIBS7 = -L$(DEVKITPRO)/libnds/lib -lnds7 -lmm7

# Bitmaps, to be converted to .bin files by grit and then stored in the
# nitrofs under /gfx.
# This needs work so that the images are not remade every time the binary is
# compiled.
BITMAPS = gfx/online.bin gfx/hand.bin gfx/useful.bin \
	gfx/bolder1.bin gfx/bolder2.bin gfx/bolder3.bin gfx/bolder4.bin \
	gfx/onlinedb1.bin gfx/onlinedb2.bin gfx/onlinedb3.bin gfx/onlinedb4.bin \
	gfx/labels_l.bin gfx/labels_r.bin gfx/arrow_a.bin gfx/arrow_b.bin \
        gfx/arrow_c.bin gfx/kugeln.bin gfx/standing.bin gfx/text.bin gfx/its.bin \
	gfx/1975.bin gfx/and.bin gfx/thisman.bin gfx/isaboutto.bin gfx/showyou.bin \
	gfx/thefuture.bin gfx/thismana.bin gfx/thismanb.bin gfx/greets1.bin \
	gfx/greets2.bin gfx/b1r.bin gfx/b2r.bin gfx/b3r.bin gfx/balpal_reduced.bin \
	gfx/prog1.bin gfx/prog2.bin gfx/prog3.bin gfx/prog4.bin gfx/prog5.bin gfx/prog6.bin \
	gfx/greets_brs.bin gfx/greets_chm.bin gfx/greets_hg.bin gfx/greets_k2.bin \
	gfx/greets_mod.bin gfx/greets_nectarine.bin gfx/greets_rno.bin \
	gfx/empty.bin gfx/credits_a.bin gfx/credits_b.bin gfx/credits.bin \
	gfx/namakubi.bin


# Compiler setup
CC = $(DEVKITARM)/bin/arm-eabi-gcc
AS = $(DEVKITARM)/bin/arm-eabi-as
LD = $(DEVKITARM)/bin/arm-eabi-gcc
CXX = $(DEVKITARM)/bin/arm-eabi-c++

CFLAGS = -std=gnu99 -Os -mcpu=arm9e -mtune=arm9e -ffast-math \
-mthumb -mthumb-interwork -I$(DEVKITPRO)/libnds/include -DARM9 $(DEFINES) \
-D__NITRO__ -DBINARY_NAME=\"$(NAME).nds\"
CXXFLAGS = -Os -mcpu=arm9e -mtune=arm9e -mthumb -mthumb-interwork \
-I$(DEVKITPRO)/libnds/include -DARM9 $(DEFINES) -D__NITRO__ \
-DBINARY_NAME=\"$(NAME).nds\"
CFLAGS7 = -std=gnu99 -O3 -ffast-math -fomit-frame-pointer -mcpu=arm7tdmi \
-mtune=arm7tdmi -mthumb -mthumb-interwork -I$(DEVKITPRO)/libnds/include -DARM7 $(DEFINES)
LDFLAGS = -specs=ds_arm9.specs -mthumb -mthumb-interwork -mno-fpu
LDFLAGS7 = -specs=./ds_arm7_.specs -mthumb -mthumb-interwork -mno-fpu

# Special rules for making png files into .bin files.
# Grit currently appends .img.bin, breaking this a little.
.SUFFIXES: .bin .png
.png.bin :
	$(DEVKITARM)/bin/grit $< -ftb -o$<

# Rules for the final binary
# Builds an NDS binary with a nitrofs file system with the files from /Datafiles
# in it appended.
$(NAME).nds: $(NAME).arm9 $(NAME).arm7
	$(DEVKITARM)/bin/grit gfx_source/pal_images/pal_reduced.png -gB8 -gb -ftb -ogfx/palette.png
	rm gfx/palette.img.bin
	cp gfx/*.bin Datafiles/gfx
	$(DEVKITARM)/bin/ndstool -c $@ -9 $(NAME).arm9 -7  ../devkit/devkitPro/libnds/default.arm7 -d Datafiles

# Arm9 / Arm7 binary rules
$(NAME).arm9: $(NAME).arm9.elf
	$(DEVKITARM)/bin/arm-eabi-objcopy -O binary $< $@

$(NAME).arm7: $(NAME).arm7.elf
	$(DEVKITARM)/bin/arm-eabi-objcopy -O binary $< $@

$(NAME).arm9.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(NAME).arm7.elf: $(OBJS7)
	$(LD) $(LDFLAGS7) -o $@ $(OBJS7) $(LIBS7)
Main.arm7.o: Main.arm7.c
	$(CC) $(CFLAGS7) -c -o $@ $<
../audio/AudioModel.o: ../audio/AudioModel.c
	$(CC) $(CFLAGS7) -c -o $@ $<
../audio/BlockDecoder.o: ../audio/BlockDecoder.c
	$(CC) $(CFLAGS7) -c -o $@ $<
../audio/BlockRangeDecoder.o: ../audio/BlockRangeDecoder.c
	$(CC) $(CFLAGS7) -c -o $@ $<
	
Main.o: Main.c $(BITMAPS)

# To clean, delete all binary objects and converted images
clean:
	rm -f $(NAME).nds $(NAME).arm9 $(NAME).arm7 $(NAME).arm9.elf $(NAME).arm7.elf $(OBJS) $(OBJS7) $(BITMAPS) gfx/*.c gfx/*.h gfx/*.bin *~ Datafiles/gfx/*

# Clean images only
cleanimages:
	rm -f $(BITMAPS) gfx/*.c gfx/*.h gfx/*.bin Datafiles/gfx/*

# Compile if neccesary and run in emulator
test: $(NAME).nds
	/usr/bin/wine $(DEVKITPRO)/nocash/NOCASH.EXE $(NAME).nds

# Send binary to flash card for testing on the system
todisk: $(NAME).nds
	/bin/rm -rf disk
	/bin/mkdir disk
	/usr/bin/sudo mount /dev/sdb1 disk
	/usr/bin/sudo cp $(NAME).nds disk/
	/bin/sleep 1
	/usr/bin/sudo umount disk
	/bin/rm -rf disk
