#ifndef SAMPLELISTENER_H
#define SAMPLELISTENER_H

#include "Leap.h"
class SampleListener : public Leap::Listener {
  public:
    virtual void onInit(const Leap::Controller&);
    virtual void onConnect(const Leap::Controller&);
    virtual void onDisconnect(const Leap::Controller&);
    virtual void onExit(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
  private:
  	int finger1_state;
};


#endif