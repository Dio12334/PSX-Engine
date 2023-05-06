#include "testScript.h"

namespace psx {

    void TestScript::OnUpdate(float dt){
        if(UserInput::GetKeyValue(KeyCode::L)){
            UIScreen::AddToStack<CharacterScreen>(*this);
        }
    }
    void CharacterScreen::ShowScreen() {
		if(ImGui::Begin(m_windowName.c_str(), &is_open)){
			auto& rotation = m_testScript.GetComponent<TransformComponent>().Rotation;
			ImGui::SliderFloat("rotation",&rotation, 0, 2 * M_PI);
			ImGui::End();
			if(!is_open){
				UIScreen::RemoveFromStack(*this);
			}
		}
    }
}
