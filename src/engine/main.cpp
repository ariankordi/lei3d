/*
 * This file is a part of lei3d. 
 * 
 * Author: Kevin Sadi
 */

#include "engine/Engine.hpp"
#include "util/Log.hpp"

using namespace lei3d;

/*
 * The main entrypoint for lei3d
 * 
 * Creates window, loads textures, loads vertices, has main render loop
 */
int main() {
	Log::Init();
	LEI_TRACE("Initialized Logger");
	Engine* engine = new Engine();
	// loops until render loop ends
	engine->Start();
	delete engine;

	return 0;
}
