//
// Programmer:	Craig Stuart Sapp
// Date:	Tue Jun  9 16:00:00 PDT 2009
// Filename:	testin.c
// Syntax:	C; Apple OSX CoreMIDI
// $Smake:	gcc -o %b %f -framework CoreMIDI -framework CoreServices
//              note: CoreServices needed for GetMacOSSStatusErrorString().
//
// Description:	This program reads in MIDI data and echos notes back
//              out based on the inversion of the MIDI key value.
//

#include <CoreMIDI/CoreMIDI.h>  /* interface to MIDI in Macintosh OS X */
#include <stdio.h>

void playPacketListOnAllDevices (MIDIPortRef midiout, 
                                 const MIDIPacketList* pktlist);
void myReadProc                 (const MIDIPacketList *packetList, 
                                 void* readProcRefCon, 
                                 void* srcConnRefCon);

MIDIPortRef gMidiout = NULL;  // MIDI output has to be global
                              // because initialization function for
                              // MIDI output and the MIDI input callback
                              // function cannot talk to each other
                              // directly.  So the MIDI input callback
                              // should check to see if the output
                              // is valid (not NULL) before it starts
                              // trying to use gMidiout.

/////////////////////////////////////////////////////////////////////////

int main(void) {

   // Prepare MIDI Interface Client/Port for writing MIDI data:
   MIDIClientRef midiclient  = NULL;
   MIDIPortRef   midiin      = NULL;
   OSStatus status;
   if (status = MIDIClientCreate(CFSTR("Magical MIDI"), NULL, NULL, &midiclient)) {
      printf("Error trying to create MIDI Client structure: %d\n", status);
      printf("%s\n", GetMacOSStatusErrorString(status));
      exit(status);
   }
   if (status = MIDIInputPortCreate(midiclient, CFSTR("InPuT"), myReadProc, 
         NULL, &midiin)) {
      printf("Error trying to create MIDI output port: %d\n", status);
      printf("%s\n", GetMacOSStatusErrorString(status));
      exit(status);
   }
   if (status = MIDIOutputPortCreate(midiclient, CFSTR("OuTpUt"), &gMidiout)) {
       printf("Error trying to create MIDI output port: %d\n", status);
       printf("%s\n", GetMacOSStatusErrorString(status));
       exit(status);
   }

   ItemCount nSrcs = MIDIGetNumberOfSources();
   ItemCount iSrc;
   printf("%d sources\n", (int)nSrcs);
   for (iSrc=0; iSrc<nSrcs; iSrc++) {
	   printf("Conecting source!!\n");
      MIDIEndpointRef src = MIDIGetSource(iSrc);
      MIDIPortConnectSource(midiin, src, NULL);
   }

   CFRunLoopRef runLoop;
   runLoop = CFRunLoopGetCurrent();
   CFRunLoopRun();

   return 0;
}


/////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// myReadProc -- What to do when MIDI input packets are received.
//      used as an input parameter to MIDIInputPortCreate() so that 
//      MIDI input knows what to do with the MIDI messages after it
//      receives them.

void myReadProc(const MIDIPacketList *packetList, void* readProcRefCon,
      void* srcConnRefCon) {

   static Byte     packetbuffer[65000] = {0};
   MIDIPacket     *packet              = (MIDIPacket*)packetList->packet;
   MIDIPacketList *outPacketList       = (MIDIPacketList*)packetbuffer;
   MIDIPacket     *outPacket           =  MIDIPacketListInit(outPacketList);
   MIDITimeStamp   timestamp           = 0; // play output data immediatly
   Byte            databuffer[1024];        // storage for MIDI message data
   int i, j;

   printf("got a thing\n");
   int count = packetList->numPackets;
   for (i=0; i<count; i++) {
      if (packet->length < 3) {
         continue;
      }
      if (!(((packet->data[0] & 0xf0) == 0x80) || 
          ((packet->data[0] & 0xf0) == 0x90))) {
         continue;
      }
      for (j=0; j<packet->length; j++) {
         databuffer[j] = packet->data[j];
      }
      databuffer[1] = 127 - databuffer[1];

int z;
for (z = 0; z < 3; z++)
{
    if (z > 0) printf(":");
    printf("%02X", packet->data[z]);
}
printf("\n");

      outPacket = MIDIPacketListAdd(outPacketList, sizeof(packetbuffer), 
         outPacket, timestamp, packet->length, databuffer);
      if (gMidiout != NULL) {
         playPacketListOnAllDevices(gMidiout, outPacketList);
      }
      outPacket = MIDIPacketListInit(outPacketList);
      packet = MIDIPacketNext(packet);
   }
}



//////////////////////////////
//
// playPacketListOnAllDevices -- play the list of MIDI packets
//    on all MIDI output devices which the computer knows about.
//    (Send the MIDI message(s) to all MIDI out ports).
//

void playPacketListOnAllDevices(MIDIPortRef midiout, 
      const MIDIPacketList* pktlist) {
	return;
   // send MIDI message to all MIDI output devices connected to computer:
   ItemCount nDests = MIDIGetNumberOfDestinations();
   ItemCount iDest;
   OSStatus status;
   MIDIEndpointRef dest;
   for(iDest=0; iDest<nDests; iDest++) {
      dest = MIDIGetDestination(iDest);
      if (status = MIDISend(midiout, dest, pktlist)) {
          printf("Problem sendint MIDI data on port %d\n", iDest);
          printf("%s\n", GetMacOSStatusErrorString(status));
          exit(status);
      }
   }
}


