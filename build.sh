# windres -v -I src res/resource.rc obj/resource.o
g++ $1 -v src/main.cpp src/windows/api.cpp src/worker/worker.cpp src/hook/hook.cpp src/hotkeys/hotkeys.cpp src/tailor/tailor.cpp obj/*.o -I src -lgdi32 -o bin/disperse.exe
echo "Finished, press Enter to exit."
read