#include "MidiController.h"

MidiController::MidiController() {
    MIDIClientCreate(CFSTR("LEAPMIDI"), NULL, NULL,
        &theMidiClient);
    MIDISourceCreate(theMidiClient, CFSTR("LEAPMIDI Source"),
        &midiOut);

    pktList = new MIDIPacketList;
    pkt = new MIDIPacket;

    pkt = MIDIPacketListInit(pktList);
    if(pkt == NULL){
    	printf("1Packet is null wtf???\n");
    } else {
    	printf("1Nah it's a nice packet... length %d\n", pkt->length);
    }

    // some reason sleep here makes first event trigger
    sleep(1);
}

void MidiController::play_arpeggio()
{
    MIDITimeStamp timestamp = AudioGetCurrentHostTime();

    // maj chord
    //int chordDegrees[3] = {1, 3, 5};
    int chordSteps[3] = {0, 4, 7};
    // C4
    Byte root = 0x48;
    Byte velocity = 0x7F;

    int i = 0;
    for(i=0; i<3; i++){
        _note_on(0, root + (Byte)chordSteps[i], velocity, timestamp);

        timestamp += 1000000000; // one billion nanoseconds later
        _note_off(0, root + (Byte)chordSteps[i], timestamp);
    }
}

void MidiController::handle()
{
	printf("Handling MIDI\n");
      if (pkt == NULL || MIDIReceived(midiOut, pktList)) {
            printf("failed to send the midi.\n");
        } else {
            printf("sent!\n");
        }

}

void MidiController::_note_on(int channel, Byte note, Byte velocity, MIDITimeStamp timestamp)
{
	int bufferSize = sizeof(pktBuffer);
    printf("Note on, value: %02X, velocity: %02X, time: %llu buffer size: %d, packet list size: %d\n", note, velocity, timestamp, bufferSize, sizeof(pktList));
    if(pkt == NULL){
    	printf("Packet is null wtf???\n");
    } else {
    	printf("Nah it's a nice packet... length %d\n", pkt->length);
    }
    Byte notemessage[MESSAGESIZE] = {0x90, note, velocity};
    pkt = MIDIPacketListAdd(pktList, bufferSize,
        pkt, timestamp, MESSAGESIZE, notemessage);
}

void MidiController::_note_off(int channel, Byte note, MIDITimeStamp timestamp)
{
	int bufferSize = sizeof(pktBuffer);
    printf("Note off, value: %02X, time: %llu\n", note, timestamp);
    Byte notemessage[MESSAGESIZE] = {0x80, note, 0x00};
    pkt = MIDIPacketListAdd(pktList, bufferSize,
        pkt, timestamp, MESSAGESIZE, notemessage);
}