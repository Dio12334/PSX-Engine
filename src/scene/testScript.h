#ifndef TESTSCRIPT_H
#define TESTSCRIPT_H

#include "scriptableEntity.h"
#include "components.h"
#include "scene.h"
#include "../inputSystem/userInput.h"
#include "../uiSystem/uiSystem.h"
#include "../renderer/renderer.h"
#include <iostream>

namespace psx {


    class CharacterScreen;
	class TestScript: public ScriptableEntity{
		public:
            void OnUpdate(float dt) override;
            friend class CharacterScreen;
	};

    class CharacterScreen: public UIScreen{

        public:
            CharacterScreen(TestScript& script): UIScreen("Rotation UI"), m_testScript(script){

            }

        void ShowScreen() override;

        private:
            bool is_open = true;
            TestScript& m_testScript;

    };
}
#endif

