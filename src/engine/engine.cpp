#include "engine.h"
#include "../renderer/renderer.h"
#include "../inputSystem/inputSystem.h"
#include "../uiSystem/uiSystem.h"
#include "../entity/entity.h"
#include "../renderer/font.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <fstream>
#include <rapidjson/document.h>
#include "../components/components.h"
#include "../fileSystem/levelLoader.h"
namespace psx {


	Engine::Engine(): 
		m_engineState(sGameplay), 
		m_updatingEntities(false),
		m_ticksCount(0), 
		m_renderer(nullptr), 
		m_inputSystem(nullptr),
		m_uiSystem(nullptr)
	{

	}
	
	bool Engine::Init(){
		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}

		m_renderer = new Renderer(this);
		if(!m_renderer->Initialize(1024, 768)){
			SDL_Log("Failed to initialize renderer\n");
			delete m_renderer;
			m_renderer = nullptr;
			return false;
		}

		m_inputSystem = new InputSystem();
		if(!m_inputSystem->Initialize()){
			SDL_Log("Failed to initialize input system\n");
			delete m_inputSystem;
			m_inputSystem = nullptr;
			return false;
		}

		m_uiSystem = new UISystem();
		m_uiSystem->Initialize(m_renderer->GetWindow(), m_renderer->GetContext());

		if(TTF_Init() != 0){
			SDL_Log("Failed to initialize SDL_ttf\n");
			return false;
		}

		LoadData();
		m_ticksCount = SDL_GetTicks();
		return true;
	}
	
	void Engine::RunLoop(){
		while(m_engineState != State::sQuit){
			ProcessInput();
			UpdateGame();
			GenerateOutput();
		}
	}
			
	void Engine::Shutdown(){
		UnloadData();
		TTF_Quit();
		if(m_renderer){
			m_renderer->Shutdown();
		}
		if(m_inputSystem){
			m_inputSystem->Shutdown();
		}
		if(m_uiSystem){
			m_uiSystem->Shutdown();
		}
		SDL_Quit();
	}
			
	void Engine::ProcessInput(){
		m_inputSystem->PrepareForUpdate();
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			m_uiSystem->ProcessEvent(&event);
			switch (event.type) {
				case SDL_QUIT:
					m_engineState = State::sQuit;
					break;
				case SDL_KEYDOWN:
					if(!event.key.repeat){
						if(m_engineState == State::sGameplay){
							HandleKeyPress(event.key.keysym.sym);
						}
					}
					break;
				case SDL_MOUSEWHEEL:
					m_inputSystem->ProcessEvent(event);
					break;

				case SDL_MOUSEBUTTONDOWN:	
					if(!event.key.repeat){
						if(m_engineState == State::sGameplay){
							HandleKeyPress(event.button.button);
						}
					}
					break;
				default:
					break;
			}
		}
		m_inputSystem->Update();
		const auto& state = m_inputSystem->GetState();

		if(m_engineState == State::sGameplay){
			m_updatingEntities = true;
			for(auto entity: m_entities){
				entity->ProcessInput(state);
			}
			m_updatingEntities = false;
		}
	}
	

	void Engine::UpdateGame(){
		
		m_uiSystem->StartFrame();
		ImGui::ShowDemoWindow();
		while(!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16));
		float deltaTime = (SDL_GetTicks() - m_ticksCount)/1000.f;
		if(deltaTime > 0.05f){
			deltaTime = 0.05f;
		}
		m_ticksCount = SDL_GetTicks();
		
		if(m_engineState == State::sGameplay){
			m_updatingEntities = true;
			for(auto entity: m_entities){
				entity->Update(deltaTime);
			}
			m_updatingEntities = false;

			for(auto entity: m_pausedEntities){
				entity->ComputeWorldTransform();
				m_entities.push_back(entity);
			}

			m_pausedEntities.clear();

			std::vector<Entity*> dead;
			for(auto entity: m_entities){
				if(entity->GetState() == Entity::State::sDead){
					dead.push_back(entity);
				}
			}

			for(auto entity: dead){
				delete entity;
			}
		}

	}

	void Engine::GenerateOutput(){
		m_renderer->Draw();
	}

	void Engine::AddEntity(class Entity *entity){
		if(m_updatingEntities){
			m_pausedEntities.push_back(entity);
		}
		else{
			m_entities.push_back(entity);
		}
	}

	void Engine::RemoveEntity(class Entity *entity){
		auto iter = std::find(m_pausedEntities.begin(), m_pausedEntities.end(), entity);
		if(iter != m_pausedEntities.end()){	
			// pendingActors.erase(iter); inefficient if actor in the middle
			std::iter_swap(iter, m_pausedEntities.end() -1);
			m_pausedEntities.pop_back();
			return ; // i don't think there would be a copy of the same actor in the other vector
		}

		iter = std::find(m_entities.begin(), m_entities.end(), entity);
		if(iter != m_entities.end()){
			std::iter_swap(iter, m_entities.end() - 1);
			m_entities.pop_back();
		}
	}

	void Engine::LoadData(){
		LoadText("assets/English.gptext");
		/* auto tex = m_renderer->LoadTexture("assets/skeleton-sprite.png"); */
		/* auto tex2 = m_renderer->LoadTexture("assets/bungo.png"); */
		/* auto e = new Entity(this); */
		/* e->SetPosition(Vec2f(200, 30)); */
		/* e->SetScale(10); */
		/* e->SetRotation(M_PI/2); */
		/* auto s = new SpriteComponent(e); */
		/* s->SetTexture(tex2); */

		/* e = new Entity(this); */
		/* e->SetPosition(Vec2f(200, 30)); */
		/* e->SetScale(10); */
		/* s = new SpriteComponent(e); */
		/* s->SetTexture(tex); */
		
		/* e = new Entity(this); */
		/* e->SetPosition(Vec2f (200, 40)); */
		/* auto c = new CameraComponent(e); */
		/* s = new SpriteComponent(e); */
		/* s->SetTexture(tex2); */
		LevelLoader::LoadLevel(this, "assets/level.glevel");
		//ImGui::ShowDemoWindow();
	}

	void Engine::UnloadData(){
		while(!m_entities.empty()){
			delete m_entities.back();
		}
		if(m_renderer){
			m_renderer->UnloadData();
		}
	}
	

	class Font* Engine::GetFont(const std::string &fileName){
		auto iter = m_fonts.find(fileName);
		if(iter != m_fonts.end()){
			return iter->second;
		}
		else{
			auto font = new Font(this);
			if(font->Load(fileName)){
				m_fonts.emplace(fileName, font);
			}
			else{
				font->Unload();
				delete font;
				font = nullptr;
			}
			return font;
		}
	}

	void Engine::HandleKeyPress(i32 key){
		switch(key){
			case SDLK_ESCAPE:
				m_engineState = State::sQuit;
				break;
			case SDLK_l:
				LevelLoader::SaveLevel(this, "assets/level.glevel");	
			default:
				break;
		}
	}


	void Engine::LoadText(const std::string& fileName){
		m_text.clear();
		std::ifstream file(fileName);
		if(!file.is_open()){
			SDL_Log("Text file %s not found", fileName.c_str());
			return;
		}

		std::stringstream fileStream;
		fileStream << file.rdbuf();
		auto contents = fileStream.str();

		rapidjson::StringStream jsonStr(contents.c_str());
		rapidjson::Document doc;
		doc.ParseStream(jsonStr);
		if(!doc.IsObject()){
			SDL_Log("Text file %s is not valid JSON", fileName.c_str());
			return;
		}

		const auto& actions = doc["TextMap"];
		for(auto itr = actions.MemberBegin(); itr != actions.MemberEnd(); ++itr){
			if(itr->name.IsString() && itr->value.IsString()){
				m_text.emplace(itr->name.GetString(), itr->value.GetString());
			}
		}
	}

	const std::string& Engine::GetText(const std::string& key){
		static std::string errorMsg("**KEY NOT FOUND**");

		auto iter = m_text.find(key);
		if(iter != m_text.end()){
			return iter->second;
		}
		else{
			return errorMsg;
		}
	}
}
