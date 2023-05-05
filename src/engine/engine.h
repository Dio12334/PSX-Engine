#ifndef ENGINE_H
#define ENGINE_H

#include "../utils/psxint.h"
#include <string>
#include <unordered_map>

namespace psx {

	class Engine{
		public:

			Engine();
			bool Init();
			void RunLoop();
			void Shutdown();

			class Font* GetFont(const std::string& fileName);

            enum State{
                sGameplay,
                sQuit
            };

			State GetState() const { return m_engineState; }
			void SetState(State state) { m_engineState = state; }
			void LoadText(const std::string& fileName);
			const std::string& GetText(const std::string& key);


		private:
	
			void ProcessInput();
			void UpdateGame();
			
			void LoadData();
			void UnloadData();

			State m_engineState;
			u32 m_ticksCount;
			
			class Scene* m_activeScene;

			std::unordered_map<std::string, class Font*> m_fonts;
			std::unordered_map<std::string, std::string> m_text;
	};

}

#endif

