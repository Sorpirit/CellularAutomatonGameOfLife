#pragma once
#include "SimulatorState.h"
#include "../Render/WorldRenderer.hpp"
#include "../Render/ResourceManager.hpp"

namespace Core
{
    class Simulator
    {
    public:
        // game state
        GameState    State;
        bool         Keys[1024];
        unsigned int Width, Height;
        // constructor/destructor
        Simulator(unsigned int width, unsigned int height);
        ~Simulator();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
    private:
        Render::WorldRenderer* _render;
        World _world;
    };
}
