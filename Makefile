UNAME := $(shell uname)

leapmidi: clean leapmidi.cpp
	# -pie for g++
	$(CXX) -ggdb -o libleapmidi.so -O0 ../Leap_SDK/lib/libc++/libLeap.dylib SampleListener.cpp MidiController.cpp leapmidi.cpp -framework CoreMIDI -framework CoreServices -framework CoreAudio -llua -I../Leap_SDK/include -L../Leap_SDK/lib -Wall -fPIC -pie
#ifeq ($(UNAME), Darwin)
#	install_name_tool -change ../lib/libLeap.dylib @loader_path/libLeap.dylib Sample
#endif

clean:
	rm -rf libleapmidi.so libleapmidi.so.dSYM
