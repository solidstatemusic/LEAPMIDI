UNAME := $(shell uname)

leapmidi: leapmidi.cpp
	$(CXX) -g -shared -o libleapmidi.so SampleListener.cpp ../Leap_SDK/lib/libc++/libLeap.dylib leapmidi.cpp -framework CoreMIDI -framework CoreServices -framework CoreAudio -I/usr/local/Cellar/lua/5.1.5/include/ -L/usr/local/Cellar/lua/5.1.5/lib/ -I../Leap_SDK/include -L../Leap_SDK/lib -llua -Wall -fPIC
#ifeq ($(UNAME), Darwin)
#	install_name_tool -change ../lib/libLeap.dylib @loader_path/libLeap.dylib Sample
#endif

clean:
	rm -rf Sample Sample.dSYM
