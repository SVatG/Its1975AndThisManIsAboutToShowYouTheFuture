/**
 * JBDS main entry point
 */

// Utilities to make development easier
#include "Utils.h"

// NitroFS access
#include "nitrofs.h"

// Effects!
#include "effects.h"

u8 soundBuffer[44100*2*40];

int main()
{	
	// Turn on everything.
	POWCNT1 = POWCNT1_ALL;
	irqEnable( IRQ_VBLANK );
	
	// Init NitroFS for data loading.
	nitroFSInitAdv( BINARY_NAME );

	#ifdef DEBUG
	consoleDemoInit();
	iprintf( "Debug mode.\n" );
	#endif

	loadData( "nitro:/zik/music_32.raw", soundBuffer, 44100*2*40 );
	for( int i = 0; i < 44100*2*40; i+=2 ) {
		soundBuffer[i] = soundBuffer[i] & 0xC0;
	}
	soundEnable();
	soundPlaySample(
		soundBuffer,
		SoundFormat_16Bit,
		44100*2*40,
		32000,
		127,
		64,
		false,
		0
	);
	

	// Main loop
	u32 t = 0;
	u32 t_switch = 0;
	u8 effect = 0;
	u8 result = 0;
	effect1_init();
	effect1_update( t );
	while( 1 ) {
		t++;
		t_switch++;
		switch( effect ) {
			// Single effect
			case 0:
				result = effect1_update( t );
				if( result != 0 ) {
					t = 0;
					effect++;
					effect2_init();
					effect2_update( t );
					effect1_destroy();
				}
			break;

			default:
				/* Nada */
			break;
		}
		
		// 60fps, or less if in DOUBLETHREEDEE MODE, or doing fancy
		// things like that, the effect in question should know.
 		swiWaitForVBlank();
	}
	
	return 0;
}
