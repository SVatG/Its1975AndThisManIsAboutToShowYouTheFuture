#include "Utils.h"
#include "DS3D.h"

static int flip;
static uint32_t whitetexture;

void effect2_init() {
	DISPCNT_A=DISPCNT_MODE_4|DISPCNT_3D|DISPCNT_BG0_ON|DISPCNT_BG3_ON|DISPCNT_ON;
	BG0CNT_A=BGxCNT_PRIORITY_2;

	BG3CNT_A = BGxCNT_EXTENDED_BITMAP_8 | BGxCNT_BITMAP_SIZE_256x256 | BGxCNT_OVERFLOW_WRAP | BGxCNT_BITMAP_BASE_0K;
	BG3CNT_A = (BG3CNT_A&~BGxCNT_PRIORITY_MASK)|BGxCNT_PRIORITY_1;
	BG3PA_A = (1 << 8);
	BG3PB_A = 0;
	BG3PC_A = 0;
	BG3PD_A = (1 << 8);
	BG3X_A = 0;
	BG3Y_A = 0;
	
	DISPCNT_B=DISPCNT_MODE_4|DISPCNT_BG3_ON|DISPCNT_ON;
	BG3CNT_B=BGxCNT_BITMAP_BASE_0K|BGxCNT_EXTENDED_BITMAP_8
			|BGxCNT_BITMAP_SIZE_256x256|BGxCNT_PRIORITY_1;
	BG3PA_B=0x100;
	BG3PB_B=0;
	BG3PC_B=0;
	BG3PD_B=0x100;
	BG3HOFS_B=0;
	BG3VOFS_B=0;
	BG3X_B = 0;
	BG3Y_B = 0;
	
	VRAMCNT_A=VRAMCNT_A_LCDC;
	VRAMCNT_B=VRAMCNT_B_LCDC;
	VRAMCNT_C=VRAMCNT_C_BG_VRAM_B;
	VRAMCNT_D=VRAMCNT_D_BG_VRAM_A_OFFS_0K;
	
	DSInit3D();
	DSViewport(0,0,255,191);

	DSSetControl(DS_TEXTURING|DS_ALPHA_BLEND);
	DSClearParams(0,0,0,31,63);

	for(int i=0;i<256*256;i++)
	{
		VRAM_LCDC_A[i]=0x8000;
		VRAM_LCDC_B[i]=0x8000;
	}

//	DSCopyColorTexture(DS_TEX_ADDRESS(VRAM_LCDC_A+256*204),0x7fff);
//	DSCopyColorTexture(DS_TEX_ADDRESS(VRAM_LCDC_B+256*204),0x7fff);
memset(VRAM_LCDC_A,0x00,256*192*2);
memset(VRAM_LCDC_B,0x00,256*192*2);
memset(VRAM_LCDC_A+256*204,0xff,8*8*2);
memset(VRAM_LCDC_B+256*204,0xff,8*8*2);
	whitetexture=DS_TEX_ADDRESS(VRAM_LCDC_A+256*204);

	flip=0;
	//whitetexture=DSMakeWhiteTexture();
//DSTranslatef32(128,96,0);
//DSScalef(100.0/4096,100.0/4096,1);
}

void LoadOverlay(int n)
{
	switch(n)
	{
		case 1:
			load8bVRAMIndirect( "nitro:/gfx/bolder1.img.bin",VRAM_A,256*192);
			loadVRAMIndirect( "nitro:/gfx/bolder1.pal.bin", PALRAM_A,256*2);
			load8bVRAMIndirect( "nitro:/gfx/onlinedb1.img.bin",VRAM_B,256*192);
			loadVRAMIndirect( "nitro:/gfx/onlinedb1.pal.bin", PALRAM_B,256*2);
		break;

		case 2:
			load8bVRAMIndirect( "nitro:/gfx/onlinedb2.img.bin",VRAM_B,256*192);
			loadVRAMIndirect( "nitro:/gfx/onlinedb2.pal.bin", PALRAM_B,256*2);
		break;

		case 3:
			load8bVRAMIndirect( "nitro:/gfx/bolder2.img.bin",VRAM_A,256*192);
			loadVRAMIndirect( "nitro:/gfx/bolder2.pal.bin", PALRAM_A,256*2);
		break;

		case 4:
			load8bVRAMIndirect( "nitro:/gfx/onlinedb3.img.bin",VRAM_B,256*192);
			loadVRAMIndirect( "nitro:/gfx/onlinedb3.pal.bin", PALRAM_B,256*2);
		break;

		case 5:
			load8bVRAMIndirect( "nitro:/gfx/bolder3.img.bin",VRAM_A,256*192);
			loadVRAMIndirect( "nitro:/gfx/bolder3.pal.bin", PALRAM_A,256*2);
		break;
		
		case 6:
			load8bVRAMIndirect( "nitro:/gfx/onlinedb4.img.bin",VRAM_B,256*192);
			loadVRAMIndirect( "nitro:/gfx/onlinedb4.pal.bin", PALRAM_B,256*2);
		break;

		case 7:
			load8bVRAMIndirect( "nitro:/gfx/bolder4.img.bin",VRAM_A,256*192);
			loadVRAMIndirect( "nitro:/gfx/bolder4.pal.bin", PALRAM_A,256*2);
		break;
	}
}

static int type=1;

u8 effect2_update( u32 t ) {
	int capsrc;

	if(t==0) { LoadOverlay(1); type = 1; }
	else if(t==90) { LoadOverlay(2); }
	else if(t==90*2) { LoadOverlay(3); type = 2; }
	else if(t==90*3) { LoadOverlay(4); }
	else if(t==90*4) { LoadOverlay(5); type = 3; }
	else if(t==90*5) { LoadOverlay(6); }
	else if(t==90*6) { LoadOverlay(7); type = 4; }
	
/*	uint16_t *ptr;
	if(flip) ptr=VRAM_LCDC_A;
	else ptr=VRAM_LCDC_B;
	for(int i=0;i<256*10;i++)
	{
		ptr[256*192+i]=ptr[256*191+i];
	}*/
//	DISPCNT_A=DISPCNT_MODE_4|DISPCNT_3D|DISPCNT_BG0_ON|DISPCNT_BG3_ON|DISPCNT_ON;

	capsrc=DISPCAPCNT_SRC_A_3D;

	if(flip)
	{
		VRAMCNT_A=VRAMCNT_A_TEXTURE_OFFS_0K;
		VRAMCNT_B=VRAMCNT_B_LCDC;
		DISPCAPCNT=DISPCAPCNT_WRITE_VRAM_B|DISPCAPCNT_SIZE_256x192
		|capsrc|DISPCAPCNT_SRC_A|DISPCAPCNT_ENABLE;
	}
	else
	{
		VRAMCNT_A=VRAMCNT_A_LCDC;
		VRAMCNT_B=VRAMCNT_B_TEXTURE_OFFS_0K;
		if(t!=0) DISPCAPCNT=DISPCAPCNT_WRITE_VRAM_A|DISPCAPCNT_SIZE_256x192
		|capsrc|DISPCAPCNT_SRC_A|DISPCAPCNT_ENABLE;
	}
	flip^=1;

	DSMatrixMode(DS_PROJECTION);
	DSLoadIdentity();
	DS2DProjection(0);

	DSMatrixMode(DS_MODELVIEW);
	DSLoadIdentity();

	DSSetTexture(0|DS_TEX_SIZE_S_256|DS_TEX_SIZE_T_256|DS_TEX_FORMAT_RGB|DS_TEX_GEN_TEXCOORD);

	DSColor(0x7fff);

	DSMatrixMode(DS_TEXTURE);
	for(int i=1;i<=2;i++)
	{
//		int rx=rand()%127-63;
//		int ry=rand()%127-63;
//		int x=(0)*16+rx,y=(96+15)*16+ry;
		DSLoadIdentity();
//		DSTranslatef(x,y,0);
//		DSScalef((1-(float)i/40),(1-(float)i/40),0);
//		DSTranslatef(-x,-y,0);

		float f;
		switch(type)
		{
			case 1:
				DSTranslatef(0*16,192*16,0);
				DSRotateZi(-10*i);
				DSTranslatef(-0*16,-192*16,0);
			break;

			case 2:
				f=1-(float)i/20;
				DSTranslatef(128*16,0*16,0);
				DSScalef(f,f,0);
				DSTranslatef(-128*16,-0*16,0);
			break; 

			case 3:
				f=1-(float)i/15;
				DSTranslatef(256*16,192*16,0);
				DSScalef(f,f,0);
				DSTranslatef(-256*16,-192*16,0);
			break; 

			case 4:
			{
				f=1+(float)i/20;
				int x=Random()%65-32;
				int y=Random()%65-32;
				DSTranslatef(x+128*16,y+96*16,0);
				DSScalef(f,f,0);
				DSTranslatef(-x-128*16,-y-96*16,0);

/*				DSTranslatef(56*16,80*16,0);
				DSRotateZi(-20*i);
				DSTranslatef(-56*16,-80*16,0);*/
			}
			break; 
		}

		if(i==1)
//		DSPolygonAttributes(DS_POLY_CULL_NONE|DS_POLY_ALPHA(16));
		DSPolygonAttributes(DS_POLY_CULL_NONE|DS_POLY_ALPHA(31/i));
		else
		DSPolygonAttributes(DS_POLY_MODE_DECAL|DS_POLY_CULL_NONE|DS_POLY_ALPHA(31/i)|DS_POLY_DEPTH_TEST_EQUAL);

		DSBegin(DS_QUADS);
		DSTexCoord2f(0,0); DSVertex3v16(0,0,DSf32(-1));
		DSTexCoord2f(256,0); DSVertex3v16(256,0,DSf32(-1));
		DSTexCoord2f(256,192); DSVertex3v16(256,192,DSf32(-1));
		DSTexCoord2f(0,192); DSVertex3v16(0,192,DSf32(-1));
		DSEnd();
	}

	DSLoadIdentity();

	DSPolygonAttributes(DS_POLY_MODE_DECAL|DS_POLY_CULL_NONE|DS_POLY_ALPHA(31));

	DSSetTexture(whitetexture);

	DSBegin(DS_TRIANGLES);
	switch(type)
	{
		case 1:
			for(int i=0;i<100;i++)
			{
				switch(Random()%2)
				{
					case 0: DSColor3b(Random()%16+16,Random()%4,Random()%4); break;
					case 1: DSColor3b(Random()%4,Random()%4,Random()%16+16); break;
				}

				int x,y;
				if(Random()%2) { x=4; y=Random()%192; }
				else { x=Random()%256; y=4; }

				DSVertex3v16(x+Random()%9-4,y+Random()%9-4,DSf32(-0.5));
				DSVertex3v16(x+Random()%9-4,y+Random()%9-4,DSf32(-0.5));
				DSVertex3v16(x+Random()%9-4,y+Random()%9-4,DSf32(-0.5));
			}
		break;

		case 2:
			for(int i=0;i<100;i++)
			{
				switch(Random()%2)
				{
					case 0: DSColor3b(Random()%16+16,Random()%4,Random()%4); break;
					case 1: DSColor3b(Random()%4,Random()%16+16,Random()%4); break;
				}

				int rx=rand()%257-128;
				DSVertex3v16(rx+Random()%33-16+128-10,Random()%16,DSf32(-0.5));
				DSVertex3v16(rx+Random()%33-16+128-10,Random()%16,DSf32(-0.5));
				DSVertex3v16(rx+Random()%33-16+128-10,Random()%16,DSf32(-0.5));
			}
		break;

		case 3:
			for(int i=0;i<100;i++)
			{
				switch(Random()%2)
				{
					case 0: DSColor3b(Random()%4,Random()%4,Random()%16+16); break;
					case 1: DSColor3b(Random()%4,Random()%16+16,Random()%4); break;
				}

				DSVertex3v16(Random()%33-32+256,Random()%33-32+192,DSf32(-0.5));
				DSVertex3v16(Random()%33-32+256,Random()%33-32+192,DSf32(-0.5));
				DSVertex3v16(Random()%33-32+256,Random()%33-32+192,DSf32(-0.5));
			}
		break;

		case 4:
			for(int i=0;i<400;i++)
			{
				switch(Random()%3)
				{
					case 0: DSColor3b(Random()%4,Random()%4,Random()%16+16); break;
					case 1: DSColor3b(Random()%4,Random()%16+16,Random()%4); break;
					case 2: DSColor3b(Random()%16+16,Random()%4,Random()%4); break;
				}

				int x,y;
				switch(Random()%4)
				{
					case 0: x=4; y=Random()%192; break;
					case 1: x=251; y=Random()%192; break;
					case 2: y=4; x=Random()%256; break;
					case 3: y=187; x=Random()%256; break;
				}

				DSVertex3v16(x+Random()%9-4,y+Random()%9-4,DSf32(-0.5));
				DSVertex3v16(x+Random()%9-4,y+Random()%9-4,DSf32(-0.5));
				DSVertex3v16(x+Random()%9-4,y+Random()%9-4,DSf32(-0.5));
			}
		break;
	}

	DSEnd();

	DSSwapBuffers(DS_SWAP_NO_SORTING);

	if( t == 720 ) {
		return( 1 );
	}

	u16* master_bright_sub = (u16*)(0x400106C);
	u16* master_bright = (u16*)(0x400006C);
	memset( master_bright, (1<<6) | 0, 2 );
	memset( master_bright_sub, (1<<6) | 0, 2 );
	
	return 0;
}


void effect2_destroy() {
	DISPCAPCNT=0;
}
