#include "Simulator.hpp"
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <future>
#include <omp.h>
#include "World.cpp"

using namespace std;

//#define DETERMENISTIC

namespace Core
{
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
        : State(SIMULATION_RUNNING), Keys(), Width(width), Height(height), _world(1000, 1000), _generationTimer(0)
    {
        /*for (int i = 0; i < _world.GetWidth(); ++i)
        {
            for (int j = 0; j < _world.GetHeight(); ++j)
            {
                _world.SetCell(i, j, emptyCell);
            }
        }

        _world.SetCell(1, 2, fullCell);
        _world.SetCell(2, 2, fullCell);
        _world.SetCell(3, 2, fullCell);
        _world.ApplyData();*/

        for (int i = 0; i < _world.GetWidth(); ++i)
        {
	        for (int j = 0; j < _world.GetHeight(); ++j)
	        {
                _world.SetCell(i, j, rand() % 2 == 0 ? fullCell : emptyCell);
	        }
        }
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
        _generationTimer += dt;

        if(_generationTimer <= 0.12)
            return;

        _generationTimer = 0;
        _world.Swap();


#ifndef DETERMENISTIC
#pragma omp parallel for num_threads(8)
#endif
        for (int i = 0; i < _world.GetWidth(); ++i)
        {
#ifndef DETERMENISTIC
#pragma omp parallel for num_threads(8)
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


    }

    void Simulator::ProcessInput(float dt)
    {

    }

    void Simulator::Render()
    {
        _render->Render();
    }
}
