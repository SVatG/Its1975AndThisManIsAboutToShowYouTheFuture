/**
 * JBDS main entry point
 */

// Utilities to make development easier
#include "Utils.h"

// NitroFS access
#include "nitrofs.h"

// Effects!
#include "effects.h"

#include <nds/arm9/sound.h>
#include <nds/fifocommon.h>
#include <nds/fifomessages.h>

int soundFd;
u16* soundBuffer[4096];
void fillSoundBuffer(int bytes, void* user_data){
	FifoMessage msg;
	fifoGetDatamsg(FIFO_SOUND, bytes, (u8*)&msg);
	if(msg.type == 777) {
		iprintf( "Lol %x %d\n", msg.MicBufferFull.buffer, msg.MicBufferFull.length );
		read( soundFd, msg.MicBufferFull.buffer, msg.MicBufferFull.length );
// 		DC_InvalidateRange(soundBuffer, msg.MicBufferFull.length);
// 		DC_InvalidateRange(msg.MicBufferFull.buffer, msg.MicBufferFull.length);		
// 		dmaCopy(soundBuffer, msg.MicBufferFull.buffer, msg.MicBufferFull.length);
	}
}

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

	// Start the music.
	soundFd = open( "nitro:/zik/music_32.wilt", O_RDONLY | O_BINARY );
	soundEnable();
	fifoSetDatamsgHandler( FIFO_SOUND, fillSoundBuffer, 0 );

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
