#pragma once
#include "SimulatorState.h"
#include "KeyPress.hpp"
#include "../Render/WorldRenderer.hpp"
#include "../Render/ResourceManager.hpp"
#include "Cell.hpp"
#include "World.hpp"

namespace Core
{
    class Simulator
    {
    public:
        // game state
        GameState    State;
        KeyPress     Keys[1024];
        double cursorWindowX, cursorWindowY;
        KeyPress leftMouseButton, rightMouseButton;
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
        World<Cell> _world;

        float _generationTimer;

        bool _generateMap;
        bool _clearMap;

        bool _showDebugGrid;

        bool _cursorPlacePlaceCell;
        bool _cursorRemovePlaceCell;
        int _cursorWorldX, _cursorWorldY;

        void GenerateRandomMap();
        void ClearMap();
    };
}
