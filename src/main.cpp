#include <iostream>
#include <thread>

#include "windows/api.h"
#include "hook/hook.h"

static const char *help_msg =
"===DISPERSE KEY BINDINGS==="
"\nALT+CTRL+2+(UP|DOWN|LEFT|RIGHT) \n\t1/2 of monitor\n"
"\nALT+CTRL+3+(UP|DOWN|LEFT|RIGHT) \n\t1/3 or 2/3 (alternating)\n"
"\nALT+CTRL+4+(UP|DOWN|LEFT|RIGHT) \n\t1/4 or 3/4 (alternating)\n"
"\nALT+CTRL+1+(2|3|4)+(UP|DOWN|LEFT|RIGHT) \n\tAction of those above but locked to one dimension (ie UP|DOWN does not affect width)\n"
"\nALT+CTRL+F \n\tFullscreen\n"
"\nALT+CTRL+C \n\tCentered (no resizing)\n"
"\nALT+CTRL+(UP|DOWN|LEFT|RIGHT) \n\tMove window by same pixels as length/width, or to edge of monitor, whichever is closer (no resizing)";

int main() {
	std::cout << help_msg << std::endl;
    Hook::init();
    int exitCode = API::startLoop();
    Hook::cleanup();
    return exitCode;
}
