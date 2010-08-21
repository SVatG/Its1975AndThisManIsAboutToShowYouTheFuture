/*---------------------------------------------------------------------------------

	default ARM7 core

		Copyright (C) 2005 - 2010
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.

	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.

	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include <nds.h>
// #include <dswifi7.h>
#include "../audio/BlockDecoder.h"
#include <nds/arm7/audio.h>
#include <nds/fifocommon.h>
#include <nds/fifomessages.h>

//---------------------------------------------------------------------------------
void VblankHandler(void) {
//---------------------------------------------------------------------------------
// 	Wifi_Update();
}


//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

volatile bool exitflag = false;

//---------------------------------------------------------------------------------
void powerButtonHandler() {
//---------------------------------------------------------------------------------
	exitflag = true;
}

// Wilt buffer fill code.
void fillSoundBuffer(uint8_t* buffer, uint32_t size) {
// 	send "more data" request over fifo.
	FifoMessage msg;
	msg.type = 777; // This is now our magic number.
	msg.MicBufferFull.buffer = (void*)buffer; // Abusing these
	msg.MicBufferFull.length = (u32)size;

	fifoSendDatamsg(FIFO_SOUND, sizeof(msg) , (u8*)&msg);
}

u8* soundBuffer[6400];
int main() {
	readUserSettings();

	irqInit();
	// Start the RTC tracking IRQ
	initClockIRQ();

	fifoInit();

	SetYtrigger(80);

// 	installWifiFIFO();
 	installSoundFIFO();
	
	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);
	irqSetAUX(IRQ_POWER, powerButtonHandler);
	irqEnableAUX(IRQ_POWER);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	// Keep the ARM7 decoding.
	int sbCount = 3;
	int bufpos = 0;
	powerOn(POWER_SOUND);
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE ) | PM_SOUND_AMP );
	REG_SOUNDCNT = SOUND_ENABLE;
	REG_MASTER_VOLUME = 127;
	initDecoder(&fillSoundBuffer);
	while (!exitflag) {
		if ( 0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
			exitflag = true;
		}
		if( sbCount < 2 ) {
			sbCount++;
		} else {
			SCHANNEL_SOURCE(0) = (u32)(soundBuffer+bufpos);
			SCHANNEL_REPEAT_POINT(0) = 0;
			SCHANNEL_LENGTH(0) = 3200;
			SCHANNEL_TIMER(0) = SOUND_FREQ(32000);
			SCHANNEL_CR(0) = SCHANNEL_ENABLE | SOUND_VOL(127) | SOUND_PAN(64) | (1 << 29) | SOUND_ONE_SHOT;
			
			bufpos = (bufpos + 3200)%6400;
			getBlock(soundBuffer+bufpos,6400);
			sbCount = 0;
		}
		swiWaitForVBlank();
	}
	return 0;
}

#ifdef __cplusplus
}
#endif
