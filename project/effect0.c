#include "Utils.h"
#include <nds/registers_alt.h>

int prevtext;
int loadflip2 = 0;
void effect0_init() {
	prevtext = -1;

	DISPCNT_B = DISPCNT_MODE_5 | DISPCNT_BG2_ON | DISPCNT_ON;
	VRAMCNT_C = VRAMCNT_C_BG_VRAM_B;

	BG2CNT_B = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_SCREEN_BASE(0);
	BG2CNT_B = (BG2CNT_B&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_0;
	BG2PA_B = (1 << 8);
	BG2PB_B = 0;
	BG2PC_B = 0;
	BG2PD_B = (1 << 8);
	BG2X_B = 0;
	BG2Y_B = 0;

	DISPCNT_A = DISPCNT_MODE_5 | DISPCNT_BG2_ON | DISPCNT_ON;
	VRAMCNT_A = VRAMCNT_A_BG_VRAM_A_OFFS_0K;
	
	BG2CNT_A = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_BITMAP_BASE_0K;
	BG2CNT_A = (BG2CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_1;
	BG2PA_A = (1 << 8);
	BG2PB_A = 0;
	BG2PC_A = 0;
	BG2PD_A = (1 << 8);
	BG2X_A = 0;
	BG2Y_A = 0;
}

void loadText(int n)
{
	loadflip2 = ~loadflip2;
	u16* laddr;
	if( !loadflip2 ) {
		laddr = VRAM_B;
	}
	else {
		laddr = VRAM_B_OFFS_64K;
	}
	
	switch(n)
	{
		case 0:
			load8bVRAMIndirect( "nitro:/gfx/its.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/its.pal.bin", PALRAM_B,256*2);
		break;

		case 1:
			load8bVRAMIndirect( "nitro:/gfx/1975.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/1975.pal.bin", PALRAM_B,256*2);
		break;

		case 2:
			load8bVRAMIndirect( "nitro:/gfx/and.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/and.pal.bin", PALRAM_B,256*2);
		break;

		case 3:
			load8bVRAMIndirect( "nitro:/gfx/thisman.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/thisman.pal.bin", PALRAM_B,256*2);
		break;

		case 4:
			load8bVRAMIndirect( "nitro:/gfx/isaboutto.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/isaboutto.pal.bin", PALRAM_B,256*2);
		break;

		case 5:
			load8bVRAMIndirect( "nitro:/gfx/showyou.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/showyou.pal.bin", PALRAM_B,256*2);
		break;

		case 6:
			load8bVRAMIndirect( "nitro:/gfx/thefuture.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/thefuture.pal.bin", PALRAM_B,256*2);
		break;
	}
	BG2CNT_B = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_SCREEN_BASE(4*(loadflip2?1:0));
}


u8 effect0_update( u32 t ) {
	int text;
	if( t > 0 ) {
		text = 0;
	}
	if( t > 40 ) {
		text = 1;
	}
	if( t > 180 ) {
		text = 2;
	}
	if( t > 190 ) {
		text = 3;
	}
	if( t > 240 ) {
		text = 4;
	}
	if( t > 290 ) {
		text = 5;
	}
	if( t > 370 ) {
		text = 6;
	}
	if( text != prevtext ) {
		prevtext = text;
		loadText( text );
	}
	if( t == 0 ) {
		load8bVRAMIndirect( "nitro:/gfx/thismana.img.bin",VRAM_A,256*192);
		loadVRAMIndirect( "nitro:/gfx/thismana.pal.bin", PALRAM_A,256*2);
	}

	if( t < 310 ) {
		BLEND_CR = BLEND_FADE_WHITE | BLEND_SRC_BG2;
		BLEND_Y = (310 - t)/10;
	}

	if( t == 370 ) {
		load8bVRAMIndirect( "nitro:/gfx/thismanb.img.bin",VRAM_A,256*192);
		loadVRAMIndirect( "nitro:/gfx/thismanb.pal.bin", PALRAM_A,256*2);
	}

	if( t > 605 ) {
		BLEND_CR = BLEND_FADE_BLACK | BLEND_SRC_BG2;
		BLEND_Y = (t-605)/5;
		SUB_BLEND_CR = BLEND_FADE_BLACK | BLEND_SRC_BG2;
		SUB_BLEND_Y = (t-605)/5;
	}

	if( t == 730 ) {
		return 1;
	}
	
	return( 0 );
}


void effect0_destroy() {
	BLEND_CR = BLEND_NONE;
	SUB_BLEND_CR = BLEND_NONE;
}
