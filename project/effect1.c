#include "Utils.h"

const unsigned short colours[256]=
{
	0x089F,0x089F,0x08BF,0x0CDF,0x0CFF,0x0D1F,0x091F,0x093F,
	0x095F,0x0D7F,0x0D9F,0x09BF,0x09BF,0x09DF,0x0DFF,0x0A1F,
	0x0A1F,0x0E3F,0x0E5F,0x065F,0x067F,0x0A7F,0x0E9F,0x0E9F,
	0x06BF,0x0ABF,0x0EDF,0x0EFF,0x06FF,0x06FF,0x0B1F,0x0F3F,
	0x033F,0x075F,0x0B5F,0x0F7F,0x037F,0x079F,0x0BBF,0x0FBF,
	0x03BF,0x07DF,0x0FFF,0x03FF,0x03FF,0x03FF,0x0FFE,0x0BFE,
	0x0BFD,0x07FC,0x03FB,0x03FA,0x0FFA,0x0FF9,0x0BF8,0x07F7,
	0x07F6,0x03F5,0x03F4,0x0FF4,0x0FF3,0x0BF2,0x0BF0,0x07EF,

	0x03EE,0x03EC,0x03EB,0x0FEA,0x0FE8,0x0BE5,0x0FE8,0x1BE3,
	0x13E0,0x23E2,0x1BE0,0x2FE3,0x27E0,0x2BE0,0x33E1,0x3BE4,
	0x37E0,0x3FE1,0x3BE0,0x47E3,0x43E0,0x4BE4,0x47E0,0x47E0,
	0x53E5,0x4BE0,0x4FE2,0x57E2,0x53E0,0x5BE3,0x5BE0,0x5FE0,
	0x63E3,0x67E5,0x67E0,0x6BE2,0x6FE5,0x6FE0,0x6FE0,0x73E4,
	0x73E0,0x77E0,0x7BE2,0x7FE0,0x7FE4,0x7FE0,0x7FE5,0x7FE0,
	0x7FC4,0x7FC0,0x7FA4,0x7FA0,0x7F83,0x7F80,0x7F62,0x7F83,
	0x7F62,0x7F61,0x7F40,0x7F46,0x7F20,0x7F25,0x7F00,0x7F24,

	0x7F00,0x7F03,0x7EE1,0x7EE2,0x7EC0,0x7EC0,0x7EA0,0x7EC5,
	0x7E80,0x7EA3,0x7EA2,0x7E60,0x7E85,0x7E41,0x7E20,0x7E64,
	0x7E63,0x7E20,0x7E00,0x7E00,0x7E23,0x7E44,0x7DE0,0x7DC0,
	0x7DE3,0x7E04,0x7DA0,0x7DA2,0x7D80,0x7DC4,0x7D60,0x7D83,
	0x7D40,0x7D41,0x7DA5,0x7D20,0x7D64,0x7D22,0x7CE0,0x7D44,
	0x7CC0,0x7D02,0x7D25,0x7C80,0x7CE3,0x7CE5,0x7CA3,0x7CC5,
	0x7C63,0x7C64,0x7C64,0x7C85,0x7C46,0x7C49,0x7C49,0x7C49,
	0x7C6B,0x7C4B,0x7C6C,0x7C6D,0x7C4E,0x7C4F,0x7C50,0x7C70,

	0x7C71,0x7C52,0x7C52,0x7C74,0x7C53,0x7C75,0x7C35,0x7C56,
	0x7C57,0x7C77,0x7C38,0x7C38,0x7C59,0x7C7A,0x7C3B,0x7C5B,
	0x7C7C,0x7C7C,0x7C3D,0x7C5E,0x7C7E,0x7C1F,0x7C1F,0x7C7F,
	0x785F,0x743F,0x701F,0x707F,0x6C5F,0x683F,0x681F,0x687F,
	0x647F,0x605F,0x5C3F,0x581F,0x587F,0x545F,0x505F,0x4C3F,
	0x4C3F,0x481F,0x487F,0x447F,0x405F,0x405F,0x3C3F,0x383F,
	0x383F,0x347F,0x307F,0x2C7F,0x2C5F,0x285F,0x245F,0x245F,
	0x205F,0x1C3F,0x183F,0x183F,0x143F,0x101F,0x107F,0x0C7F,

};

s16 colpos = 0;

void hblank() {
	dmaCopyHalfWords( 0, colours + colpos, PALRAM_B+197, 2 );
	colpos = (colpos + 1) % 256;
}

void effect1_init() {
	irqSet( IRQ_HBLANK, hblank );
	irqEnable( IRQ_HBLANK );
	
	DISPCNT_B = DISPCNT_MODE_5 | DISPCNT_BG3_ON | DISPCNT_BG2_ON | DISPCNT_ON;
	VRAMCNT_C = VRAMCNT_C_BG_VRAM_B;

	BG2CNT_B = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_SCREEN_BASE(0);
	BG2CNT_B = (BG2CNT_B&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_1;
	BG2PA_B = (1 << 8);
	BG2PB_B = 0;
	BG2PC_B = 0;
	BG2PD_B = (1 << 8);
	BG2X_B = 0;
	BG2Y_B = 0;
	
	BG3CNT_B = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_BASE_64K;
	BG3CNT_B = (BG3CNT_B&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_0;
	BG3PA_B = (1 << 8);
	BG3PB_B = 0;
	BG3PC_B = 0;
	BG3PD_B = (1 << 8);
	BG3X_B = 0;
	BG3Y_B = 0;
	
	u16* sub_bg = (u16*)VRAM_B_OFFS_0K;
	u16* sub_hand = (u16*)VRAM_B_OFFS_64K;

	load8bVRAMIndirect( "nitro:/gfx/useful.img.bin", sub_bg, 256*192 );
	load8bVRAMIndirect( "nitro:/gfx/hand.img.bin", sub_hand, 256*192 );
	loadVRAMIndirect( "nitro:/gfx/useful.pal.bin", PALRAM_B, 256 * 2 );

	DISPCNT_A = DISPCNT_MODE_5  | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_ON;
	VRAMCNT_D = VRAMCNT_D_BG_VRAM_A_OFFS_0K;
	VRAMCNT_B = VRAMCNT_B_BG_VRAM_A_OFFS_128K;
	
	BG3CNT_A = BGxCNT_EXTENDED_BITMAP_16 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_BASE_0K;
	BG3CNT_A = (BG3CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_1;
	BG3PA_A = (1 << 8);
	BG3PB_A = 0;
	BG3PC_A = 0;
	BG3PD_A = (1 << 8);
	BG3X_A = 0;
	BG3Y_A = 0;

	BG2CNT_A = BGxCNT_EXTENDED_BITMAP_16 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_TRANSPARENT | BGxCNT_BITMAP_BASE_128K;
	BG2CNT_A = (BG2CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_0;
	BG2PA_A = icos(600)>>4;
	BG2PB_A = isin(600)>>4;
	BG2PC_A = -isin(600)>>4;
	BG2PD_A = icos(600)>>4;
	BG2X_A = 0;
	BG2Y_A = 0;

	u16* bg = (u16*)VRAM_A_OFFS_0K;
	for( int x = 0; x < 256; x++ ) {
		for( int y = 0; y < 256; y++ ) {
			if( y%16 == 0 || (y+1)%16 == 0  ) {
				bg[y*256+x] = RGB15(0,0,0) | BIT(15);
			}
			else {
				bg[y*256+x] = RGB15(28,28,28) | BIT(15);
			}
		}
	}

	VRAMCNT_A = VRAMCNT_A_OBJ_VRAM_A;

	oamInit(&oamMain, SpriteMapping_1D_128, false);
	loadVRAMIndirect("nitro:/gfx/labels_l.pal.bin", SPRITE_PALETTE, 512);
	u16* labels_l = loadSpriteA( "nitro:/gfx/labels_l.img.bin" );
	u16* labels_r = loadSpriteA( "nitro:/gfx/labels_r.img.bin" );
	
	oamSet(
		&oamMain, 1,
		121, 10,
		1, 0,
		SpriteSize_64x64,
		SpriteColorFormat_256Color,
		labels_l,
		-1, false, false, false, false, false
	);
	oamSet(
		&oamMain, 2,
		185, 10,
		1, 0,
		SpriteSize_64x64,
		SpriteColorFormat_256Color,
		labels_r,
		-1, false, false, false, false, false
	);
	
	oamUpdate(&oamMain);
}

int arra_m = 2;
int arrb_m = -2;
int arrc_m = 2;
int arra_s = 0;
int arrb_s = 160;
int arrc_s = 100;

void drawbars() {
	u16* bg = (u16*)(VRAM_A_OFFS_0K+0x10000);

	arra_s += arra_m;
	arrb_s += arrb_m;
	arrc_s += arrc_m;

	if( arra_m == 2 ) {
		if( arra_s > 128 + Random()%128 ) {
			arra_m = -2;
		}
	}
	else {
		if( arra_s < 128 - Random()%128 ) {
			arra_m = 2;
		}
	}

	if( arrb_m == 2 ) {
		if( arrb_s > 128 + Random()%128 ) {
			arrb_m = -2;
		}
	}
	else {
		if( arrb_s < 128 - Random()%128 ) {
			arrb_m = 2;
		}
	}

	if( arrc_m == 2 ) {
		if( arrc_s > 128 + Random()%128 ) {
			arrc_m = -2;
		}
	}
	else {
		if( arrc_s < 128 - Random()%128 ) {
			arrc_m = 2;
		}
	}

	for( int x = 0; x < 256; x++ ) {
		if( x > arra_s && x < arra_s + 20 ) {
			bg[x] = RGB15(31,0,0) | BIT(15);
		}
		else if( x > arrb_s && x < arrb_s + 20 ) {
			bg[x] = RGB15(0,0,31) | BIT(15);
		}
		else if( x > arrc_s && x < arrc_s + 20 ) {
			bg[x] = RGB15(31,21,0) | BIT(15);
		} else {
			bg[x] = ~BIT(15);
		}
	}

	for( int y = 192; y > 0; y-- ) {
		dmaCopy( &bg[(y-1)*256], &bg[y*256], 512 );
	}
}

u8 effect1_update( u32 t ) {
	BG3X_B = isin((t*20)<<3);
	BG3Y_B = isin((t*20))-4096;
	BG3Y_A = -t*800;

	drawbars();
	drawbars();	
	
	swiWaitForVBlank();
	return( 0 );
}


void effect1_destroy() {
	irqDisable( IRQ_HBLANK );
}
