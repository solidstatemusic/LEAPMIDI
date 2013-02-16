package.loadlib("./libleapmidi.so", "luaopen_leapmidi")()
io.write("Hello from lua!\n");
leapmidi.play_arpeggio()
