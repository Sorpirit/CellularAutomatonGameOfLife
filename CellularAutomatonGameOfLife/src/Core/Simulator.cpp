#include "Simulator.hpp"

namespace Core
{
    struct Cell
    {
        unsigned char Data;
    };


    Simulator::Simulator(unsigned int width, unsigned int height)
        : State(SIMULATION_RUNNING), Keys(), Width(width), Height(height), _world(4, 4, 1)
    {
        Cell fullCell;
        fullCell.Data = UCHAR_MAX;
        unsigned char const* fullByteBuffer = reinterpret_cast<unsigned char const*>(&fullCell);

        Cell emptyCell;
        emptyCell.Data = 0;
        unsigned char const* emptyByteBuffer = reinterpret_cast<unsigned char const*>(&emptyCell);

        for (int i = 0; i < _world.GetWidth(); ++i)
        {
	        for (int j = 0; j < _world.GetHeight(); ++j)
	        {
                _world.SetPixel(i, j, (i + j) % 2 == 0 ? fullByteBuffer : emptyByteBuffer);
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
        //_world.Swap();
        /*for (int i = 0; i < _world.GetWidth(); ++i)
        {
            for (int j = 0; j < _world.GetHeight(); ++j)
            {
                _world.SetPixel(i, j, (i + j) % 2 == 0 ? fullByteBuffer : emptyByteBuffer);
            }
        }*/
    }

    void Simulator::ProcessInput(float dt)
    {

    }

    void Simulator::Render()
    {
        _render->Render();
    }
}
