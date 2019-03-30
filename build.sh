windres -v -I src res/resource.rc obj/resource.o
g++ -v src/main.cpp src/app/app.cpp src/icon/icon.cpp src/hotkeys/hotkeys.cpp src/hotkeys/hotkey.cpp obj/*.o -I src -lgdi32 -o bin/disperse.exe
echo "Finished, press Enter to exit."
read