#include "engine.h"
#include "../renderer/renderer.h"
#include "../inputSystem/inputSystem.h"
#include "../uiSystem/uiSystem.h"
#include "../inputSystem/userInput.h"
#include "../renderer/font.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_ttf.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <rapidjson/document.h>
#include "../scene/testScene.h"

namespace psx {


	Engine::Engine(): 
		m_engineState(sGameplay),
		m_ticksCount(0),
		m_activeScene(nullptr)
	{

	}
	
	bool Engine::Init(){

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}

		if(!Renderer::Initialize(1024, 768)){
			SDL_Log("Failed to initialize renderer\n");
			return false;
		}

		if(!InputSystem::Initialize()){
			SDL_Log("Failed to initialize input system\n");
			return false;
		}

		UISystem::Initialize(Renderer::GetWindow(), Renderer::GetContext());

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
		}
	}
			
	void Engine::Shutdown(){
		UnloadData();
		TTF_Quit();
		Renderer::Shutdown();
        InputSystem::Shutdown();
        UISystem::Shutdown();

		SDL_Quit();
	}
			

	void Engine::ProcessInput(){
        InputSystem::PrepareForUpdate();
		SDL_Event event;

		while(SDL_PollEvent(&event)){
			UISystem::ProcessEvent(&event);
			switch (event.type) {
				case SDL_QUIT:
					m_engineState = State::sQuit;
					break;
				case SDL_MOUSEWHEEL:
                    InputSystem::ProcessEvent(event);
					break;
				default:
					break;
			}
		}
        InputSystem::Update();

		if(UserInput::GetKeyValue(KeyCode::ESCAPE)){
			m_engineState = State::sQuit;
		}

	}
	

	void Engine::UpdateGame(){

		int width, height;
		SDL_GetWindowSize(Renderer::GetWindow(), &width, &height);
		m_activeScene->OnViewPortResize( width, height);
		UISystem::StartFrame();
		
		//Demo window
		ImGui::ShowDemoWindow(); 
		ImGui::Begin("My Fixed-Size Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
		
		ImGui::Text("Hello, world!");

		if (ImGui::Button("Click me!")) {

			std::cout << "Button clicked!\n";
		}

		ImGui::End();

		while(!SDL_TICKS_PASSED(SDL_GetTicks(), m_ticksCount + 16));
		float deltaTime = (float)(SDL_GetTicks() - m_ticksCount)/1000.f;
		if(deltaTime > 0.05f){
			deltaTime = 0.05f;
		}
		m_ticksCount = SDL_GetTicks();
		
		m_activeScene->OnUpdate(deltaTime);

	}

	void Engine::LoadData(){
		LoadText("assets/English.gptext");
		m_activeScene = new TestScene(this);
		m_activeScene->LoadScene();
	}

	void Engine::UnloadData(){
		
		delete m_activeScene;

	    Renderer::UnloadData();
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
