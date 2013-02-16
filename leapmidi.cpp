#include <CoreMIDI/CoreMIDI.h>
#include <unistd.h>               /* for sleep() function                */
#include <mach/mach_time.h>       /* for mach_absolute_time()            */
#include <CoreAudio/HostTime.h> /* for AudioGetCurrentHostTime */
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <stdlib.h>
#include <stdio.h>
#include "Leap.h"
#include "SampleListener.h"
using namespace Leap;

#define MESSAGESIZE 3             /* byte count for MIDI note messages   */

void play_arpeggio(MIDIPacketList *pktList, MIDIPacket *pkt, char *pktBuffer, uint64_t bufferSize);
void _note_on(int channel, Byte note, Byte velocity, MIDITimeStamp timestamp, MIDIPacketList *pktList, MIDIPacket **pkt, char *pktBuffer, uint64_t bufferSize);
void _note_off(int channel, Byte note, MIDITimeStamp timestamp, MIDIPacketList *pktList, MIDIPacket **pkt, char *pktBuffer, uint64_t bufferSize);

/* lua api fxns */
static int l_play_arpeggio (lua_State *L);

// register fxns with lua
static const struct luaL_reg leapmidi [] = {
    {"play_arpeggio", l_play_arpeggio},
    {NULL, NULL}  /* sentinel */
};

int main(int argc, char *args[])
{
    MIDIClientRef   theMidiClient;
    MIDIEndpointRef midiOut;
    char pktBuffer[1024];
    MIDIPacketList* pktList;
    pktList = (MIDIPacketList*) pktBuffer;
    MIDIPacket     *pkt;
    int             i;

    MIDIClientCreate(CFSTR("LEAPMIDI"), NULL, NULL,
            &theMidiClient);
    MIDISourceCreate(theMidiClient, CFSTR("LEAPMIDI Source"),
            &midiOut);

    pkt = MIDIPacketListInit(pktList);

      // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);
/*
    //Byte notemessage[MESSAGESIZE];
    for (i = 0; i < 100; i++) {
        //play_arpeggio(pktList, pkt, pktBuffer, sizeof(pktBuffer));

        if (pkt == NULL || MIDIReceived(midiOut, pktList)) {
            printf("failed to send the midi.\n");
        } else {
            printf("sent!\n");
        }
        sleep(10);
    }
*/
    return 0;
}

void _note_on(int channel, Byte note, Byte velocity, MIDITimeStamp timestamp, MIDIPacketList *pktList, MIDIPacket **pkt, char *pktBuffer, uint64_t bufferSize)
{
    printf("Note on, value: %02X, velocity: %02X, time: %llu\n", note, velocity, timestamp);
    Byte notemessage[MESSAGESIZE] = {0x90, note, velocity};
    *pkt = MIDIPacketListAdd(pktList, bufferSize,
            *pkt, timestamp, MESSAGESIZE, notemessage);
}

void _note_off(int channel, Byte note, MIDITimeStamp timestamp, MIDIPacketList *pktList, MIDIPacket **pkt, char *pktBuffer, uint64_t bufferSize)
{
    printf("Note off, value: %02X, time: %llu\n", note, timestamp);
    Byte notemessage[MESSAGESIZE] = {0x80, note, 0x00};
    *pkt = MIDIPacketListAdd(pktList, bufferSize,
            *pkt, timestamp, MESSAGESIZE, notemessage);
}

void play_arpeggio(MIDIPacketList *pktList, MIDIPacket *pkt, char *pktBuffer, uint64_t bufferSize)
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
        _note_on(0, root + (Byte)chordSteps[i], velocity, timestamp, pktList, &pkt, pktBuffer, bufferSize);

        timestamp += 1000000000; // one billion nanoseconds later
        _note_off(0, root + (Byte)chordSteps[i], timestamp, pktList, &pkt, pktBuffer, bufferSize);
    }
}

static int l_play_arpeggio (lua_State *L) {
    printf("Doot doola doot!\n");
    return 0;
}

extern "C" {
    int luaopen_leapmidi (lua_State *L) {
        printf("Opening lib\n");
        luaL_register(L, "leapmidi", leapmidi);
        return 1;
    }
}
