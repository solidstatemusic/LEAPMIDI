//gcc -o testmidi testmidi.c -framework CoreMIDI -framework CoreServices
#include <CoreMIDI/CoreMIDI.h>

int main(int argc, char *args[])
{
    MIDIClientRef   theMidiClient;
    MIDIEndpointRef midiOut;
    char pktBuffer[1024];
    MIDIPacketList* pktList;
	pktList = (MIDIPacketList*) pktBuffer;
    MIDIPacket     *pkt;
    Byte            midiDataToSend[] = {0x91, 0x3c, 0x40};
    int             i;

    MIDIClientCreate(CFSTR("Magical MIDI"), NULL, NULL,
                     &theMidiClient);
    MIDISourceCreate(theMidiClient, CFSTR("Magical MIDI Source"),
                     &midiOut);

    pkt = MIDIPacketListInit(pktList);
    pkt = MIDIPacketListAdd(pktList, 1024, pkt, 0, 3, midiDataToSend);

    for (i = 0; i < 100; i++) {
        if (pkt == NULL || MIDIReceived(midiOut, pktList)) {
            printf("failed to send the midi.\n");
        } else {
            printf("sent!\n");
        }
        sleep(1);
    }

return 0;
}

