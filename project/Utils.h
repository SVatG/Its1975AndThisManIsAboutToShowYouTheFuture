#ifndef __UTILS_H__
#define __UTILS_H__

// Debug mode on/off
// Remember to make clean when you switch.
#define DEBUG

// General-purpose includes
#include <nds.h>
#include <dswifi9.h>
#include <nds/fifocommon.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>

// Register definitions
#include "Hardware.h"

// NitroFS access
#include "nitrofs.h"

#define ATTR_ITCM __attribute__((section(".itcm"),long_call))
#define ATTR_DTCM __attribute__((section(".dtcm")))


static inline int MakeRGB15(int r,int g,int b) { return 0x8000|(r<<10)|(g<<5)|b; }


uint32_t Random();


typedef struct DisplayRegisters
{
	uint16_t a[32],b[32];
} DisplayRegisters;

void SaveDisplayRegisters(DisplayRegisters *regs);
void RestoreDisplayRegisters(DisplayRegisters *regs);

typedef struct Palettes
{
	uint16_t a[256],b[256];
} Palettes;

void SavePalettes(Palettes *pals);
void RestorePalettes(Palettes *pals);

uint32_t SaveAndSetMemoryBanks(int bank_a,int bank_b,int bank_c,int bank_d);
void RestoreMemoryBanks(uint32_t savedbanks);





/*static inline int32_t FloatToFixed(float val) { return val*65536.0f; }
static inline int32_t IntToFixed(int val) { return val<<16; }
static inline int32_t IntToFixedPlusHalf(int val) { return (val<<16)+0x8000; }

static inline int FixedToInt(int32_t val) { return val>>16; }
static inline int FixedToRoundedInt(int32_t val) { return (val+0x8000)>>16; }

static inline int32_t FixedMul(int32_t a,int32_t b) { return ((int64_t)a*(int64_t)b)>>16; }*/

static inline int imin(int a,int b) { return a<b?a:b; }
static inline int imax(int a,int b) { return a>b?a:b; }
static inline int iabs(int a) { return a<0?-a:a; }
static inline int isign(int a) { return a>0?1:a<0?-1:0; }

static inline int32_t imul(int32_t a, int32_t b) { return ((int64_t)a*(int64_t)b)>>12; }

static inline int32_t idiv(int32_t num,int32_t den)
{
	DIVCNT=DIVCNT_MODE_64_32;
	DIV_NUMER_64=((int64_t)num)<<12;
	DIV_DENOM_32=den;

	while(DIVCNT&DIVCNT_BUSY);

	return DIV_RESULT_32;
}

static inline int32_t isqrt(int32_t val)
{
	SQRTCNT=SQRTCNT_MODE_64;
	SQRT_PARAM_64=((int64_t)val)<<12;

	while(SQRTCNT&SQRTCNT_BUSY);

	return SQRT_RESULT;
}

int32_t isin(int a);
static inline int32_t icos(int a) { return isin(a+1024); }
static inline int32_t itan(int a) { return idiv(isin(a),icos(a)); } // TODO: use a table
static inline int32_t iangle(int degrees) { return degrees*4096.0f/360.0f; }

static inline void icross(int32_t *a,int32_t *b,int32_t *res)
{
	res[0]=imul(a[1],b[2])-imul(b[1],a[2]);
	res[1]=imul(a[2],b[0])-imul(b[2],a[0]);
	res[2]=imul(a[0],b[1])-imul(b[0],a[1]);
}

static inline int32_t idot(int32_t *a,int32_t *b)
{
	return imul(a[0],b[0])+imul(a[1],b[1])+imul(a[2],b[2]);
}

static inline int32_t imag2(int32_t *v) { return imul(v[0],v[0])+imul(v[1],v[1])+imul(v[2],v[2]); }

static inline int32_t imag(int32_t *v) { return isqrt(imag2(v)); }

static inline void inormalize(int32_t *v)
{
	int32_t mag=imag(v);

	v[0]=idiv(v[0],mag);
	v[1]=idiv(v[1],mag);
	v[2]=idiv(v[2],mag);
}

// Buffer for VRAM image load functions.
u16 tempImage[256*192*2];

// Keep in mind that loadVRAMIndirect and loadImage make sure the alpha bit
// is set.
void loadVRAMIndirect(char* path, u16* vramPos, s32 size);
void loadImage(char* path, u16* buffer, u32 size);
void load8bVRAMIndirect(char* path, u16* vramPos, s32 size);
void loadData(char* path, u8* target, u32 size);
u16* loadSpriteA( char* path );
u16* loadSpriteB( char* path );

#endif
