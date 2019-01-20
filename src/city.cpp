#include "Game.h"
#include <iostream>

#include "windows.h"

int
main(int argc, wchar_t* argv[])
{
	city::Game().loop();
	std::cerr << "Well done, bye bye\n";

	// Manually free console
	FreeConsole();
	return 0;
}
