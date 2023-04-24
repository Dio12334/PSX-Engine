#ifndef ENGINE_H
#define ENGINE_H

#include "../utils/psxint.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace psx {

	class Engine{
		public:

			Engine();
			bool Init();
			void RunLoop();
			void Shutdown();

			class Renderer* GetRenderer() const { return m_renderer; }
			class InputSystem* GetInputSystem() const { return m_inputSystem; }

			void AddEntity(class Entity* entity);
			void RemoveEntity(class Entity* entity);

			class Font* GetFont(const std::string& fileName);

			enum State{
				sGameplay,
				sPause,
				sQuit
			};

			State GetState() const { return m_engineState; }
			void SetState(State state) { m_engineState = state; }
			void LoadText(const std::string& fileName);
			const std::string& GetText(const std::string& key);

			const std::vector<class Entity*>& GetEntities() const { return m_entities; }

		private:
	
			void HandleKeyPress(i32 key);
			void ProcessInput();
			void UpdateGame();
			void GenerateOutput();
			
			void LoadData();
			void UnloadData();

			State m_engineState;
			bool m_updatingEntities;
			u32 m_ticksCount;

			class Renderer* m_renderer;
			class InputSystem* m_inputSystem;

			std::vector<class Entity*> m_entities;
			std::vector<class Entity*> m_pausedEntities;

			std::unordered_map<std::string, class Font*> m_fonts;
			std::unordered_map<std::string, std::string> m_text;
	};

}

#endif

