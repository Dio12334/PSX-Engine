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


	class TestScript: public ScriptableEntity{

    class CharacterScreen: public UIScreen{

        public:
            CharacterScreen(TestScript& script): UIScreen("Rotation UI"), m_testScript(script){

            }

			void ShowScreen() override {
				if(ImGui::Begin(m_windowName.c_str(), &is_open)){
					auto& rotation = m_testScript.GetComponent<TransformComponent>().Rotation;
					ImGui::SliderFloat("rotation",&rotation, 0, 2 * M_PI);
					ImGui::End();
					if(!is_open){
						UIScreen::RemoveFromStack(*this);
					}
				}
			}

        private:
            bool is_open = true;
            TestScript& m_testScript;

    };

		public:
            void OnUpdate(float dt) override{

				if(UserInput::GetKeyValue(KeyCode::L)){
					UIScreen::AddToStack<CharacterScreen>(*this);
				}
			}
            
	};

}
#endif

