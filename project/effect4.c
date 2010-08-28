#include "Utils.h"
#include <nds/registers_alt.h>

int prevtext5;
void effect4_init() {
	prevtext5 = -1;

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

	DISPCNT_A = DISPCNT_MODE_5 | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_ON;
	VRAMCNT_A = VRAMCNT_A_BG_VRAM_A_OFFS_0K;
	
	BG2CNT_A = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_BITMAP_BASE_64K;
	BG2CNT_A = (BG2CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_1;
	BG2PA_A = (1 << 8);
	BG2PB_A = 0;
	BG2PC_A = 0;
	BG2PD_A = (1 << 8);
	BG2X_A = 0;
	BG2Y_A = 0;

	BG3CNT_A = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_OVERFLOW_TRANSPARENT | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_BITMAP_BASE_0K;
	BG3CNT_A = (BG3CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_0;
	BG3PA_A = (1 << 8);
	BG3PB_A = 0;
	BG3PC_A = 0;
	BG3PD_A = (1 << 8);
	BG3X_A = 0;
	BG3Y_A = 0;

	load8bVRAMIndirect( "nitro:/gfx/empty.img.bin",VRAM_A,256*256);
}

void loadText2(int n)
{
	switch(n)
	{
		case 0:
			load8bVRAMIndirect( "nitro:/gfx/greets_k2.img.bin",VRAM_A,256*192);
		break;

		case 1:
			load8bVRAMIndirect( "nitro:/gfx/greets_chm.img.bin",VRAM_A,256*192);
		break;

		case 2:
			load8bVRAMIndirect( "nitro:/gfx/greets_hg.img.bin",VRAM_A,256*192);
		break;

		case 3:
			load8bVRAMIndirect( "nitro:/gfx/greets_brs.img.bin",VRAM_A,256*192);
		break;

		case 4:
			load8bVRAMIndirect( "nitro:/gfx/greets_mod.img.bin",VRAM_A,256*192);
		break;

		case 5:
			load8bVRAMIndirect( "nitro:/gfx/greets_nectarine.img.bin",VRAM_A,256*192);
		break;

		case 6:
			load8bVRAMIndirect( "nitro:/gfx/greets_rno.img.bin",VRAM_A,256*192);
		break;
	}
}

int spos;
u8 effect4_update( u32 t ) {
	int text;
	text = t / 102;
	if( text != prevtext5 ) {
		spos = -44000;
		prevtext5 = text;
		loadText2( text );
	}
	if( t == 0 ) {
		load8bVRAMIndirect( "nitro:/gfx/greets1.img.bin",VRAM_A_OFFS_64K,256*192);
		loadVRAMIndirect( "nitro:/gfx/greets1.pal.bin", PALRAM_A,256*2);
		load8bVRAMIndirect( "nitro:/gfx/greets2.img.bin",VRAM_B,256*192);
		loadVRAMIndirect( "nitro:/gfx/greets2.pal.bin", PALRAM_B,256*2);
	}

	BG2X_B = -t*2000;
	spos += 1000;

	BG3Y_A = spos;
	
	if( t == 720 ) {
		return( 1 );
	}
	
	return( 0 );
}


void effect4_destroy() {
	
}
