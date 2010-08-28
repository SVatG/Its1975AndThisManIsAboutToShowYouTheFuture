/**
 * JBDS main entry point
 */

// Utilities to make development easier
#include "Utils.h"

// NitroFS access
#include "nitrofs.h"

// Effects!
#include "effects.h"

// Sound!
#include <maxmod9.h>
#include "music.h"

int main()
{
	// Turn on everything.
	POWCNT1 = POWCNT1_ALL;
	irqEnable( IRQ_VBLANK );
	
	// Init NitroFS for data loading.
	nitroFSInitAdv( BINARY_NAME );

	mmInitDefault( "nitro:/zik/music.bin" );
	mmLoad( MOD_LIGHTMUSIC3 );
	mmStart( MOD_LIGHTMUSIC3, MM_PLAY_ONCE );
	
	#ifdef DEBUG
	consoleDemoInit();
	iprintf( "Debug mode.\n" );
	#endif

	// Main loop
	u32 t = 0;
	u32 t_switch = 0;
	u8 effect = 0;
	u8 result = 0;
	effect0_init();
	effect0_update( t );

	while( 1 ) {
		t++;
		t_switch++;
		switch( effect ) {
			// Single effect
			case 0:
				result = effect0_update( t );
				if( result != 0 ) {
					t = 0;
					effect++;
					effect0_destroy();
					effect1_init();
					effect1_update( t );
				}
			break;

			case 1:
				result = effect1_update( t );
				if( result != 0 ) {
					t = 0;
					effect++;
					effect1_destroy();
					effect2_init();
					effect2_update( t );
				}
			break;

			case 2:
				result = effect2_update( t );
				if( result != 0 ) {
					t = 0;
					effect++;
					effect2_destroy();					
					effect3_init();
					effect3_update( t );
				}
			break;

			case 3:
				result = effect3_update( t );
// 				if( result != 0 ) {
// 					t = 0;
// 					effect++;
// 					effect2_destroy();
// 					effect3_init();
// 					effect3_update( t );
// 				}
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
