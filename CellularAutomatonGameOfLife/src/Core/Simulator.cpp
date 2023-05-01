#include "Simulator.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <tuple>
#include <future>
#include <omp.h>
#include <GLFW/glfw3.h>

#include "World.cpp"

using namespace std;

//#define DETERMENISTIC

namespace Core
{
    static double my_map(double value, double currentRangeStart, double currentRangeEnd, double targetRangeStart, double targetRangeEnd);
    static double my_map(double value, double currentRangeEnd, double targetRangeEnd);

    static const tuple<int, int> neighborDirections[] = {
	    make_tuple(-1, 1),
	    make_tuple(0, 1),
	    make_tuple(1, 1),
	    make_tuple(1, 0),
	    make_tuple(1, -1),
	    make_tuple(0, -1),
	    make_tuple(-1, -1),
	    make_tuple(-1, 0),
    };

    static const Cell fullCell{ UCHAR_MAX };
    static const Cell emptyCell{ 0 };

    Simulator::Simulator(unsigned int width, unsigned int height)
        : State(SIMULATION_PAUSED), Keys(), Width(width), Height(height), _world(100, 100), _generationTimer(0)
    {
        GenerateRandomMap();
    }

    Simulator::~Simulator()
    {
        delete _render;
    }

    void Simulator::Init()
    {
        auto vertexShaderPath = R"(C:\Users\daniel.vozovikov\Documents\Projects\Learning\C++\Dev\CellularAutomatonGameOfLife\CellularAutomatonGameOfLife\src\Render\Shaders\vertex.vs)";
        auto fragmentShaderPath = R"(C:\Users\daniel.vozovikov\Documents\Projects\Learning\C++\Dev\CellularAutomatonGameOfLife\CellularAutomatonGameOfLife\src\Render\Shaders\fragment.frag)";
	    Render::ResourceManager::LoadShader(vertexShaderPath, fragmentShaderPath, nullptr, "sprite");
        auto shader = Render::ResourceManager::GetShader("sprite");
    	_render = new Render::WorldRenderer(&_world, shader);
    }

    void Simulator::Update(float dt)
    {
        if (_clearMap || _generateMap)
        {
            if (_clearMap)
            {
                ClearMap();
                _clearMap = false;
            }

            if (_generateMap)
            {
                GenerateRandomMap();
                _generateMap = false;
            }

            return;
        }

        if(_cursorPlacePlaceCell || _cursorRemovePlaceCell)
        {
            _cursorWorldX = static_cast<int>(my_map(cursorWindowX + .5, Width, _world.GetWidth()));
            _cursorWorldY = static_cast<int>(my_map(cursorWindowY + .5, Height, _world.GetHeight()));

            _world.SetCell(_cursorWorldX, _cursorWorldY, _cursorPlacePlaceCell ? fullCell : emptyCell);
            _cursorPlacePlaceCell = false;
            _cursorRemovePlaceCell = false;
            return;
        }

        if(State == SIMULATION_PAUSED)
            return;
                
        _generationTimer += dt;

        if(_generationTimer <= 0.12)
            return;

        _generationTimer = 0;
        _world.Swap();

#ifndef DETERMENISTIC
#pragma omp parallel for num_threads(4)
#endif
        for (int i = 0; i < _world.GetWidth(); ++i)
        {
#ifndef DETERMENISTIC
#pragma omp parallel for num_threads(4)
#endif
            for (int j = 0; j < _world.GetHeight(); ++j)
            {
                int neighborAmount = 0;
                for (const auto& dir : neighborDirections) {
                    int neighborX = i + std::get<0>(dir);
                    int neighborY = j + std::get<1>(dir);
                    const Cell neighbor = _world.GetCell(neighborX, neighborY);
                    if (neighbor.Data == fullCell.Data)
                        neighborAmount++;
                }

                bool isCellFull = _world.GetCell(i, j).Data == fullCell.Data;

                if(isCellFull)
                {
                    if (neighborAmount >= 2 && neighborAmount <= 3)
                    {
                        _world.SetCell(i, j, fullCell);
                    }
                    else
                    {
                        _world.SetCell(i, j, emptyCell);
                    }
                }
            	else
                {
                    if (neighborAmount == 3)
                    {
                        _world.SetCell(i, j, fullCell);
                    }
                    else
                    {
                        _world.SetCell(i, j, emptyCell);
                    }
                }
            }
        }
        
        if (State == SIMULATION_NEXT)
            State = SIMULATION_PAUSED;
    }

    void Simulator::ProcessInput(float dt)
    {
        if(Keys[GLFW_KEY_SPACE] == KEY_PRESSED)
        {
            State = State == SIMULATION_PAUSED ? SIMULATION_RUNNING : SIMULATION_PAUSED;
        }
    	else if (Keys[GLFW_KEY_D] == KEY_PRESSED)
        {
            State = SIMULATION_NEXT;
        }
    	else if(Keys[GLFW_KEY_C] == KEY_PRESSED)
    	{
            _clearMap = true;
    	}
        else if (Keys[GLFW_KEY_G] == KEY_PRESSED)
        {
            _generateMap = true;
        }

        if(rightMouseButton == KEY_DOWN)
        {
            _cursorRemovePlaceCell = true;
        }
        else if(leftMouseButton == KEY_DOWN)
        {
            _cursorPlacePlaceCell = true;
        }

        for (int i = 0; i < 1024; ++i)
        {
            if(Keys[i] == KEY_DEFAULT)
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

        switch (leftMouseButton)
        {
        case KEY_PRESSED:
            leftMouseButton = KEY_DOWN;
            break;
        case KEY_RELEASED:
            leftMouseButton = KEY_UP;
            break;
        }

        switch (rightMouseButton)
        {
        case KEY_PRESSED:
            rightMouseButton = KEY_DOWN;
            break;
        case KEY_RELEASED:
            rightMouseButton = KEY_UP;
            break;
        }
    }

    void Simulator::Render()
    {
        _render->Render();
    }

    void Simulator::GenerateRandomMap()
    {
        for (int i = 0; i < _world.GetWidth(); ++i)
        {
            for (int j = 0; j < _world.GetHeight(); ++j)
            {
                _world.SetCell(i, j, rand() % 2 == 0 ? fullCell : emptyCell);
            }
        }
    }

    void Simulator::ClearMap()
    {
        for (int i = 0; i < _world.GetWidth(); ++i)
        {
            for (int j = 0; j < _world.GetHeight(); ++j)
            {
                _world.SetCell(i, j, emptyCell);
            }
        }
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
