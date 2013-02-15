//gcc -o testmidi testmidi.c -framework CoreMIDI -framework CoreServices
#include <CoreMIDI/CoreMIDI.h>
#include <unistd.h>               /* for sleep() function                */
#include <mach/mach_time.h>       /* for mach_absolute_time()            */
#include <CoreAudio/HostTime.h> /* for AudioGetCurrentHostTime */

#define MESSAGESIZE 3             /* byte count for MIDI note messages   */

void play_arpeggio(MIDIPacketList *pktList, MIDIPacket *pkt, char *pktBuffer, uint64_t bufferSize);

int main(int argc, char *args[])
{
	MIDIClientRef   theMidiClient;
	MIDIEndpointRef midiOut;
	char pktBuffer[1024];
	MIDIPacketList* pktList;
	pktList = (MIDIPacketList*) pktBuffer;
	MIDIPacket     *pkt;
	int             i;

	MIDIClientCreate(CFSTR("Magical MIDI"), NULL, NULL,
			&theMidiClient);
	MIDISourceCreate(theMidiClient, CFSTR("Magical MIDI Source"),
			&midiOut);

	pkt = MIDIPacketListInit(pktList);

	//Byte notemessage[MESSAGESIZE];
	for (i = 0; i < 100; i++) {
		play_arpeggio(pktList, pkt, pktBuffer, sizeof(pktBuffer));

		if (pkt == NULL || MIDIReceived(midiOut, pktList)) {
			printf("failed to send the midi.\n");
		} else {
			printf("sent!\n");
		}
		sleep(10);
	}

	return 0;
}

void play_arpeggio(MIDIPacketList *pktList, MIDIPacket *pkt, char *pktBuffer, uint64_t bufferSize)
{
		MIDITimeStamp timestamp = AudioGetCurrentHostTime();

		// play note on
		Byte notemessage[MESSAGESIZE] = {0x90, 0x3F, 0x7F};
		pkt = MIDIPacketListAdd(pktList, bufferSize,
				pkt, timestamp, MESSAGESIZE, notemessage);

		// play note off
		notemessage[1] = 0x3F;            // pitch = C4
		notemessage[2] = 0x00;             // turn off the note
		timestamp += 1000000000;        // one billion nanoseconds later

		pkt = MIDIPacketListAdd(pktList, bufferSize,
				pkt, timestamp, MESSAGESIZE, notemessage);

		// play note on
		notemessage[1] = 0x3F + 0x04; // 2 whole steps, major third
		notemessage[2] = 0x7F;
		pkt = MIDIPacketListAdd(pktList, bufferSize,
				pkt, timestamp, MESSAGESIZE, notemessage);

		// play note off
		notemessage[1] = 0x3F + 0x04;            // pitch = C4
		notemessage[2] = 0x00;             // turn off the note
		timestamp += 1000000000;        // one billion nanoseconds later

		pkt = MIDIPacketListAdd(pktList, bufferSize,
				pkt, timestamp, MESSAGESIZE, notemessage);

		// play note on
		notemessage[1] = 0x3F + 0x07; // 7 half steps, perfect fifth
		notemessage[2] = 0x7F;
		pkt = MIDIPacketListAdd(pktList, bufferSize,
				pkt, timestamp, MESSAGESIZE, notemessage);

		// play note off
		notemessage[1] = 0x3F + 0x07;           
		notemessage[2] = 0x00;             // turn off the note
		timestamp += 1000000000;        // one billion nanoseconds later

		pkt = MIDIPacketListAdd(pktList, bufferSize,
				pkt, timestamp, MESSAGESIZE, notemessage);
}

