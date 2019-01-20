#include <windows.h>

#include "Game.h"

int
main(int argc, wchar_t* argv[])
{
	city::Game().loop();

	// Manually free console
	FreeConsole();
	return 0;
}
