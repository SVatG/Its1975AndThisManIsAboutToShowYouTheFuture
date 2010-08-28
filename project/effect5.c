#include "Utils.h"
#include <nds/registers_alt.h>

float inf[256][256];


u16* b1s;
u16* b2s;
u16* b3s;
int prevtext;
int loadflip = 0;
void effect5_init() {
	prevtext = -1;

	u16* master_bright = (u16*)(0x400006C);
	memset( master_bright, (1<<6) | 16, 2 );
		
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

	DISPCNT_A = DISPCNT_MODE_5 | DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_ON;
	VRAMCNT_D = VRAMCNT_D_BG_VRAM_A_OFFS_0K;
	
	BG2CNT_A = BGxCNT_EXTENDED_BITMAP_16 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_BITMAP_BASE_0K;
	BG2CNT_A = (BG2CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_1;
	BG2PA_A = (1 << 8);
	BG2PB_A = 0;
	BG2PC_A = 0;
	BG2PD_A = (1 << 8);
	BG2X_A = 0;
	BG2Y_A = 0;

	for( int x = -128; x < 128; x++ ) {
		for( int y = -128; y < 128; y++ ) {
			inf[x+128][y+128] = 1.0/(x*x+y*y);
		}
	}

	VRAMCNT_A = VRAMCNT_A_OBJ_VRAM_A;

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	b1s = loadSpriteA( "nitro:/gfx/b1r.img.bin" );
	b2s = loadSpriteA( "nitro:/gfx/b2r.img.bin" );
	b3s = loadSpriteA( "nitro:/gfx/b3r.img.bin" );
	loadVRAMIndirect("nitro:/gfx/balpal_reduced.pal.bin", SPRITE_PALETTE, 512);
}

void loadText3(int n)
{
	loadflip = ~loadflip;
	u16* laddr;
	if( !loadflip ) {
		laddr = VRAM_B;
	}
	else {
		laddr = VRAM_B_OFFS_64K;
	}
	
	switch(n)
	{
		case 0:
			load8bVRAMIndirect( "nitro:/gfx/prog1.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/prog1.pal.bin", PALRAM_B,256*2);
		break;

		case 1:
			load8bVRAMIndirect( "nitro:/gfx/prog2.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/prog2.pal.bin", PALRAM_B,256*2);
		break;

		case 2:
			load8bVRAMIndirect( "nitro:/gfx/prog3.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/prog3.pal.bin", PALRAM_B,256*2);
		break;

		case 3:
			load8bVRAMIndirect( "nitro:/gfx/prog4.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/prog4.pal.bin", PALRAM_B,256*2);
		break;

		case 4:
			load8bVRAMIndirect( "nitro:/gfx/prog5.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/prog5.pal.bin", PALRAM_B,256*2);
		break;

		case 5:
			load8bVRAMIndirect( "nitro:/gfx/prog6.img.bin",laddr,256*192);
			loadVRAMIndirect( "nitro:/gfx/prog6.pal.bin", PALRAM_B,256*2);
		break;
	}
	BG2CNT_B = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_SCREEN_BASE(4*(loadflip?1:0));
}

int fx = 0;
int fy = 0;
int fs = 0;
u8 effect5_update( u32 t ) {
	int text;
	text = (t / 30) % 6;
	if( text != prevtext ) {
		prevtext = text;
		loadText3( text );
	}

	u16* bg = (u16*)(VRAM_A_OFFS_0K);

	int p1x = 100 + icos(t<<4)>>4;
	int p1y = 100 + isin(t<<4)>>4;
	int p2x = 120 - icos(t<<4)>>4;
	int p2y = 200 - isin(t<<4)>>4;
	int p3x = 150 + icos(t<<4)>>4;
	int p3y = 100 + icos(t<<4)>>4;
	
	float m1inf = 0;
	float m2inf = 0;
	float m3inf = 0;
	int xn;
	int yn;
	int red;
	int green;
	int blue;
	
	for( int x = fx; x < 256; x+=4 ) {
		for( int y = fy; y < 256; y+=4 ) {
			xn = x-p1x; xn = abs( xn % 256 );
			yn = y-p1y; yn = abs( yn % 256 );
			m1inf = inf[xn][yn];
			
			xn = x-p2x; xn = abs( xn % 256 );
			yn = y-p2y; yn = abs( yn % 256 );
			m2inf = inf[xn][yn];

			xn = x-p3x; xn = abs( xn % 256 );
			yn = y-p3y; yn = abs( yn % 256 );
			m3inf = inf[xn][yn];

			#define ballsize 0.0011
			if( m1inf + m2inf + m3inf > ballsize ) {
				red = m1inf/ballsize * 31 + m2inf/ballsize * 31 + m3inf/ballsize * 21;
				red = red > 31 ? 31 : red;

				green = m1inf/ballsize * 13 + m2inf/ballsize * 16 + m3inf/ballsize * 1;
				green = green > 31 ? 31 : green;

				blue = m1inf/ballsize * 22 + m2inf/ballsize * 8 + m3inf/ballsize * 5;
				blue = blue > 31 ? 31 : blue;
				
				bg[256*y+x] = RGB15(red,green,blue) | BIT(15);
				bg[256*(y+1)+x] = RGB15(red,green,blue) | BIT(15);
				bg[256*y+x+1] = RGB15(red,green,blue) | BIT(15);
				bg[256*(y+1)+x+1] = RGB15(red,green,blue) | BIT(15);
			}
			else {
				bg[256*y+x] = ~BIT(15);
			}
		}
	}

	if( fx == 0 ) {
		if( fy == 0 ) {
			fy = 1;
		}
		else {
			fy = 0;
			fx = 1;
		}
	}
	else {
		if( fy == 0 ) {
			fy = 1;
		}
		else {
			fy = 0;
			fx = 0;
		}
	}

	int sx =  abs(isin(t<<4)<<5);
	int sy = 0;
	BG2X_A = sx;
// 	BG2Y_A = sy;
	sx = -sx >> 8;
	sy = -sy >> 8;
	
	oamSet(
		&oamMain, 3,
		p1x-128-32+sx, p1y-128-32+sy,
		1, 0,
		SpriteSize_64x64,
		SpriteColorFormat_256Color,
		b1s,
		-1, false, false, false, false, false
	);
	oamSet(
		&oamMain, 4,
		p2x-128-32+sx, p2y-128-32+sy,
		1, 0,
		SpriteSize_64x64,
		SpriteColorFormat_256Color,
		b2s,
		-1, false, false, false, false, false
	);
	oamSet(
		&oamMain, 5,
		p3x-128-32+sx, p3y-128-32+sy,
		1, 0,
		SpriteSize_64x64,
		SpriteColorFormat_256Color,
		b3s,
		-1, false, false, false, false, false
	);

	oamUpdate(&oamMain);

	if( t < 32 ) {
		u16* master_bright = (u16*)(0x400006C);
		memset( master_bright, (1<<6) | 16-(t/2), 2 );
	}
	
	return( 0 );
}


void effect5_destroy() {
	
}
