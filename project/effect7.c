#include "Utils.h"

uint16_t hstate = 0xDEC0;
uint16_t vstate = 0xC0DE;

uint16_t hnext() {
	uint16_t next =
		((hstate & 0x08) >> 3) ^
		((hstate & 0x10) >> 4) ^
		((hstate & 0x20) >> 5) ^
		((hstate & 0x80) >> 7);
	hstate = (hstate >> 1) | (next << 15);
	return( hstate );
}

uint16_t vnext() {
	uint16_t next =
		((vstate & 0x08) >> 3) ^
		((vstate & 0x10) >> 4) ^
		((vstate & 0x20) >> 5) ^
		((vstate & 0x80) >> 7);
	vstate = (vstate >> 1) | (next << 15);
	return( vstate );
}

void effect7_init() {
		
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

	DISPCNT_A = DISPCNT_MODE_5 | DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_ON;
	VRAMCNT_A = VRAMCNT_A_BG_VRAM_A_OFFS_0K;
	VRAMCNT_D = VRAMCNT_D_BG_VRAM_A_OFFS_128K;
	
	BG2CNT_A = BGxCNT_EXTENDED_BITMAP_16 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_BITMAP_BASE_128K;
	BG2CNT_A = (BG2CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_0;
	BG2PA_A = (1 << 8);
	BG2PB_A = 0;
	BG2PC_A = 0;
	BG2PD_A = (1 << 8);
	BG2X_A = 0;
	BG2Y_A = 0;

	load8bVRAMIndirect( "nitro:/gfx/credits.img.bin",VRAM_B,256*192);
	loadVRAMIndirect( "nitro:/gfx/credits.pal.bin", PALRAM_B,256*2);
	loadImage( "nitro:/gfx/credits_a.img.bin",VRAM_A_OFFS_0K+0x10000,256*192*2);

	u16* screen = VRAM_A_OFFS_0K+0x10000;
	for( int i = 0; i < 256*192;i++ ) {
		if( screen[i] == (RGB15(0,31,0)|BIT(15))) {
			screen[i] = (RGB15(0,31,0)|~BIT(15));
		}
	}
	
	BG0CNT_A=BGxCNT_TILE_BASE_0K|BGxCNT_CHAR_BASE_64K
		|BGxCNT_TEXT_256_COLOURS|BGxCNT_TEXT_SIZE_256x256;
	BG0CNT_A = (BG0CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_1;
	for(int i=0;i<256*128;i++)
	{
		VRAM_A_OFFS_64K[i]=hnext();
		vnext() & 0x01 ? hnext() : vnext();
	}

	for(int i=0;i<32*24;i++)
	{
		VRAM_A_OFFS_0K[i]=vnext()&0xc3ff;
	}

	for(int i=0;i<256;i++)
	{
		int c=(hnext())&0x1f;
		PALRAM_A[i]=0x8000|(c<<10)|(c<<5)|c;
	}

}

u8 effect7_update( u32 t ) {
	if( t == 360 ) {
		loadImage( "nitro:/gfx/credits_b.img.bin",VRAM_A_OFFS_0K+0x10000,256*192*2);

		u16* screen = VRAM_A_OFFS_0K+0x10000;
		for( int i = 0; i < 256*192;i++ ) {
			if( screen[i] == (RGB15(0,31,0)|BIT(15))) {
				screen[i] = (RGB15(0,31,0)|~BIT(15));
			}
		}
	}
	if( t == 720 ) {
		VRAMCNT_A=VRAMCNT_A_BG_VRAM_A;
		VRAMCNT_C=VRAMCNT_C_BG_VRAM_B;

		DISPCNT_A=DISPCNT_MODE_0|DISPCNT_ON|DISPCNT_BG0_ON;
		BG0CNT_A=BGxCNT_TILE_BASE_0K|BGxCNT_CHAR_BASE_64K
				|BGxCNT_TEXT_256_COLOURS|BGxCNT_TEXT_SIZE_256x256;

		DISPCNT_B=DISPCNT_MODE_0|DISPCNT_ON|DISPCNT_BG0_ON;
		BG0CNT_B=BGxCNT_TILE_BASE_0K|BGxCNT_CHAR_BASE_64K
				|BGxCNT_TEXT_256_COLOURS|BGxCNT_TEXT_SIZE_256x256;

		for(int i=0;i<126*1024/2;i++)
		{
			VRAM_A_OFFS_64K[i]=hnext();
			VRAM_B_OFFS_64K[i]=hnext();
			vnext() & 0x01 ? hnext() : vnext();
		}

		for(int i=0;i<32*24;i++)
		{
			VRAM_A_OFFS_0K[i]=vnext()&0xc3ff;
			VRAM_B_OFFS_0K[i]=hnext()&0xc3ff;
		}

		for(int i=0;i<256;i++)
		{
			int c=(hnext())&0x1f;
			PALRAM_A[i]=PALRAM_B[256-i]=0x8000|(c<<10)|(c<<5)|c;
		}

		int round = 0;
		touchPosition touch;
		for(;;)
		{
			for(int i=round;i<126*1024/2;i+=16)
			{
				VRAM_A_OFFS_64K[i]=VRAM_B_OFFS_64K[i]=hnext()^vnext();
			}
			round++;
			round = round & 0xF;

			for(int i=0;i<256;i++)
			{
				int c=(hnext())&0x1f;
				PALRAM_A[i]=PALRAM_B[256-i]=0x8000|(c<<10)|(c<<5)|c;
			}
		}
	}

	int round = t*2;
	for(int i=round;i<256*128;i+=16)
	{
		VRAM_A_OFFS_64K[i]=VRAM_B_OFFS_64K[i]=hnext()^vnext();
	}
	round++;
	round = round & 0xF;

	for(int i=0;i<256;i++)
	{
		int c=(hnext())&0x1f;
		PALRAM_A[i]=0x8000|(c<<10)|(c<<5)|c;
	}
		
	return( 0 );
}


void effect7_destroy() {
	
}
