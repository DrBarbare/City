#ifdef _WIN32
# include <windows.h>
using char_type = wchar_t;
#else
using char_type = char;
#endif

#include "Game.h"

int
main(int argc, char_type* argv[])
{
	city::Game().loop();

#ifdef _WIN32
	// Manually free console
	FreeConsole();
#endif
	return 0;
}
