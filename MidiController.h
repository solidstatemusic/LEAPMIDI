#ifndef MIDICONTROLLER_H
#define MIDICONTROLLER_H
#define MESSAGESIZE 3             /* byte count for MIDI note messages   */
#include <CoreMIDI/CoreMIDI.h>
#include <unistd.h>               /* for sleep() function                */
#include <mach/mach_time.h>       /* for mach_absolute_time()            */
#include <CoreAudio/HostTime.h> /* for AudioGetCurrentHostTime */

class MidiController {
  public:
    MidiController();
    void play_arpeggio();
    void handle();
  private:
    MIDIClientRef   theMidiClient;
    MIDIEndpointRef midiOut;
    char pktBuffer[1024];
    MIDIPacketList* pktList;
    MIDIPacket     *pkt;
    void _note_on(int channel, Byte note, Byte velocity, MIDITimeStamp timestamp);
    void _note_off(int channel, Byte note, MIDITimeStamp timestamp);
};

#endif
