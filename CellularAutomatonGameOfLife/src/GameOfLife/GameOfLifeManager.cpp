#include "GameOfLifeManager.hpp"

#include <GLFW/glfw3.h>
#include <Render/ResourceManager.hpp>
#include "ResourceConstants.hpp"
#include "CellDefaults.hpp"

namespace GameOfLife
{
	static double my_map(double value, double currentRangeStart, double currentRangeEnd, double targetRangeStart, double targetRangeEnd);
	static double my_map(double value, double currentRangeEnd, double targetRangeEnd);

	GameOfLifeManager::GameOfLifeManager(unsigned width, unsigned height) :
        State(Core::SIMULATION_PAUSED), Keys(), WindowWidth(width), WindowHeight(height), _worldWidth(1500), _worldHeight(1500) { }

	GameOfLifeManager::~GameOfLifeManager()
	{
		delete _render;
	}

	void GameOfLifeManager::Init()
	{
		auto vertexShaderPath = R"(C:\Users\daniel.vozovikov\Documents\Projects\Learning\C++\Dev\CellularAutomatonGameOfLife\CellularAutomatonGameOfLife\src\Render\Shaders\vertex.vert)";
		auto fragmentShaderPath = R"(C:\Users\daniel.vozovikov\Documents\Projects\Learning\C++\Dev\CellularAutomatonGameOfLife\CellularAutomatonGameOfLife\src\Render\Shaders\fragment.frag)";
		auto computeShaderPath = R"(C:\Users\daniel.vozovikov\Documents\Projects\Learning\C++\Dev\CellularAutomatonGameOfLife\CellularAutomatonGameOfLife\src\Render\Shaders\compute.comp)";

		Render::ResourceManager::LoadShader(vertexShaderPath, fragmentShaderPath, nullptr, TEXTURE_SHADER);
		Render::ResourceManager::LoadShader(computeShaderPath, WORLD_SIMULATION_SHADER);

		auto textureShader = Render::ResourceManager::GetShader(TEXTURE_SHADER);

		_render = new Render::WorldRenderer(textureShader);
		_factory = new WorldFactory(_render);
		_world = _factory->CreateWorld(SingleThreadedCPU, _worldWidth, _worldHeight);
        _world->Init();
#ifdef TESTING_DETERMINISTIC
        srand(0);
#endif
        _world->GenerateWorld();
	}

	void GameOfLifeManager::ProcessInput(float dt)
	{
        if (Keys[GLFW_KEY_SPACE] == KEY_PRESSED)
        {
            State = State == Core::SIMULATION_PAUSED ? Core::SIMULATION_RUNNING : Core::SIMULATION_PAUSED;
        }
        else if (Keys[GLFW_KEY_D] == KEY_PRESSED)
        {
            State = Core::SIMULATION_NEXT;
        }
        else if (Keys[GLFW_KEY_C] == KEY_PRESSED)
        {
            _clearMap = true;
        }
        else if (Keys[GLFW_KEY_G] == KEY_PRESSED)
        {
            _generateMap = true;
        }

        if (Keys[GLFW_KEY_1] == KEY_PRESSED)
        {
            _showDebugGrid = !_showDebugGrid;
            _render->EnableGrid(_showDebugGrid);
        }


        if (RightMouseButton == KEY_DOWN)
        {
            _cursorRemovePlaceCell = true;
        }
        else if (LeftMouseButton == KEY_DOWN)
        {
            _cursorPlacePlaceCell = true;
        }

        for (int i = 0; i < 1024; ++i)
        {
            if (Keys[i] == KEY_DEFAULT)
                continue;

            switch (Keys[i])
            {
            case KEY_PRESSED:
                Keys[i] = KEY_DOWN;
                break;
            case KEY_RELEASED:
                Keys[i] = KEY_UP;
                break;
            }
        }

        switch (LeftMouseButton)
        {
        case KEY_PRESSED:
            LeftMouseButton = KEY_DOWN;
            break;
        case KEY_RELEASED:
            LeftMouseButton = KEY_UP;
            break;
        }

        switch (RightMouseButton)
        {
        case KEY_PRESSED:
            RightMouseButton = KEY_DOWN;
            break;
        case KEY_RELEASED:
            RightMouseButton = KEY_UP;
            break;
        }
	}

	void GameOfLifeManager::Update(float dt)
	{
        if (_clearMap || _generateMap)
        {
            if (_clearMap)
            {
                _world->ClearWorld();
                _clearMap = false;
            }

            if (_generateMap)
            {
                _world->GenerateWorld();
                _generateMap = false;
            }

            return;
        }

        if (_cursorPlacePlaceCell || _cursorRemovePlaceCell)
        {
            _cursorWorldX = static_cast<int>(my_map(CursorWindowX + .5, WindowWidth, _worldWidth));
            _cursorWorldY = static_cast<int>(my_map(CursorWindowY + .5, WindowHeight, _worldHeight));
            
            _world->SetCell(_cursorWorldX, _cursorWorldY, _cursorPlacePlaceCell ? FullCell : EmptyCell);
            _cursorPlacePlaceCell = false;
            _cursorRemovePlaceCell = false;
            return;
        }

        if (State == Core::SIMULATION_PAUSED)
            return;

        _prefRecord.begin("UpdateWorld");
        _world->UpdateWorld(dt);
        _prefRecord.end("UpdateWorld");

        if (State == Core::SIMULATION_NEXT)
			State = Core::SIMULATION_PAUSED;
	}

	void GameOfLifeManager::Render()
	{
        _render->Render(_world->GetCurrentWorldTexture());
	}

    static double my_map(double value, double currentRangeStart, double currentRangeEnd, double targetRangeStart, double targetRangeEnd)
    {
        return (value - currentRangeStart) / (currentRangeEnd - currentRangeStart) * (targetRangeEnd - targetRangeStart) + targetRangeStart;
    }

    static double my_map(double value, double currentRangeEnd, double targetRangeEnd)
    {
        return value / currentRangeEnd * targetRangeEnd;
    }
}
