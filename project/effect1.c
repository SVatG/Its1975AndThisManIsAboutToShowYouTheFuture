#include "Utils.h"

void effect1_init() {
	// Set A video mode to display a static background
	DISPCNT_A = DISPCNT_MODE_5 | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_ON;
	VRAMCNT_B = VRAMCNT_B_BG_VRAM_A_OFFS_0K;

	// Set up static background
	BG3CNT_A = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_BASE_0K;
	BG3CNT_A = (BG3CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_2;
	BG3PA_A = (1 << 8);
	BG3PB_A = 0;
	BG3PC_A = 0;
	BG3PD_A = (1 << 8);

#ifndef DEBUG
	// Set B video mode to display a static background
	DISPCNT_B = DISPCNT_MODE_5 | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_ON;
	VRAMCNT_C = VRAMCNT_C_BG_VRAM_B;

	// Set up static background
	BG3CNT_B = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_BASE_0K;
	BG3CNT_B = (BG3CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_2;
	BG3PA_B = (1 << 8);
	BG3PB_B = 0;
	BG3PC_B = 0;
	BG3PD_B = (1 << 8);
#endif

	// Set up palettes
	loadData( "nitro:/gfx/palette.pal.bin", (u8*)PALRAM_A, 512 );
	
#ifndef DEBUG
	loadData( "nitro:/gfx/palette.pal.bin", (u8*)PALRAM_B, 512 );
#endif

	// Copy in some data
	load8bVRAMIndirect( "nitro:/gfx/online.img.bin", VRAM_A_OFFS_0K, 256*192 );
#ifndef DEBUG
	load8bVRAMIndirect( "nitro:/gfx/online.img.bin", VRAM_B_OFFS_0K, 256*192 );
#endif

	// Set up OBJ memory for flags, buttons and such, A/B 128k each.
	VRAMCNT_A = VRAMCNT_A_OBJ_VRAM_A;
	VRAMCNT_D = VRAMCNT_D_OBJ_VRAM_B;

	// Init OAM
	oamInit(&oamMain, SpriteMapping_1D_128, false);
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	loadVRAMIndirect("nitro:/gfx/palette.pal.bin", SPRITE_PALETTE, 512);
	loadVRAMIndirect("nitro:/gfx/palette.pal.bin", SPRITE_PALETTE_SUB, 512);
}

u8 effect1_update( u32 t ) {
#ifdef DEBUG	
	iprintf( "t = %d\n", t );
#endif
	return( 0 );
}


void effect1_destroy() {
	/* Nada */
}
