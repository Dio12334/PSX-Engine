#ifndef TESTSCRIPT_H
#define TESTSCRIPT_H

#include "scriptableEntity.h"
#include "components.h"
#include "scene.h"
#include "../inputSystem/userInput.h"
#include "../renderer/renderer.h"
#include <iostream>

namespace psx {
	class TestScript: public ScriptableEntity{
		public:

			void OnUpdate(float dt) override {
				std::cout << "Testing\n";
			}
	};
}
#endif

