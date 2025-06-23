#include <UnigineInit.h>
#include <UnigineLogic.h>
#include <UnigineVisualizer.h>
#include <UnigineWidgets.h>
#include <UnigineViewport.h>
#include <UniginePlayers.h>
#include <UnigineGame.h>
#include <UnigineEditor.h>
#include <UnigineConsole.h>

#include "AppSystemLogic.h"
#include "AppWorldLogic.h"

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	// init engine
	Unigine::Engine::InitParameters init_params;
	init_params.window_title = "UNIGINE Engine: Photon C++ SDK integration";

	Unigine::EnginePtr engine(init_params, argc, argv);

	AppSystemLogic system_logic;
	AppWorldLogic world_logic;

	// enter main loop
	engine->main(&system_logic, &world_logic);

	return 0;
}
