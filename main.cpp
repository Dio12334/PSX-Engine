#include "src/engine/engine.h"

int main(int argc, char* argv[]){
	psx::Engine engine;
	if(engine.Init()){
		engine.RunLoop();
	}
	engine.Shutdown();
	return 0;
}
