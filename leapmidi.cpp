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
#include "MidiController.h"
using namespace Leap;


/* lua api fxns */
static int l_play_arpeggio (lua_State *L);
void load (const char *filename, int *finger1_note);

// register fxns with lua
static const struct luaL_reg leapmidi [] = {
    {"play_arpeggio", l_play_arpeggio},
    {NULL, NULL}  /* sentinel */
};

int main(int argc, char *args[])
{
      // Create a sample listener and controller
   /* SampleListener listener;
    Controller controller;

  // Have the sample listener receive events from the controller
    controller.addListener(listener);

    int finger1_note = 0;
    load("./testmidi.lua", &finger1_note);
*/
  // Keep this process running until Enter is pressed
    /*
    std::cout << "Press Enter to quit..." << std::endl;
    std::cin.get();
*/
  // Remove the sample listener when done
 //   controller.removeListener(listener);
    MidiController midiController;
    int i=0;
    for (i = 0; i < 100; i++) {
        midiController.play_arpeggio();
        midiController.handle();
        sleep(5);
    }
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

static int l_play_arpeggio (lua_State *L) {
    printf("Doot doola doot!\n");
    return 0;
}

extern "C" {
    void error (lua_State *L, const char *error) {
        lua_pushstring(L, error);
        lua_error(L);
    }

    void load (const char *filename, int *finger1_note) {
        lua_State *L = lua_open();
        luaL_openlibs(L);
        printf("Loading conf from %s\n", filename); 
        if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
        {
            error(L, "cannot run configuration file");
        }

        lua_getglobal(L, "finger1_note");
        if (!lua_isnumber(L, -1))
            error(L, "`finger1_note' should be a number");
        *finger1_note = (int)lua_tonumber(L, -1);

        printf("Finger 1 will play the note %d\n", *finger1_note);
        lua_close(L);
    }

int luaopen_leapmidi (lua_State *L) {
    printf("Opening lib\n");
    luaL_register(L, "leapmidi", leapmidi);
    return 1;
}
}
