#pragma once

#include <Core/SimulatorState.h>
#include <Core/KeyPress.hpp>

#include <Render/WorldRenderer.hpp>

#include <GameOfLife/PerformanceTest/PerformanceRecorder.hpp>

#include "WorldFactory.hpp"

namespace GameOfLife
{
	class GameOfLifeManager
	{
	public:
		Core::GameState State;
		KeyPress Keys[1024];
		double CursorWindowX, CursorWindowY;
		KeyPress LeftMouseButton, RightMouseButton;
		const unsigned int WindowWidth, WindowHeight;

		GameOfLifeManager(unsigned int width, unsigned int height);
		~GameOfLifeManager();

		void Init();
		void ProcessInput(float dt);
		void Update(float dt);
		void Render();
	private:
		Render::WorldRenderer* _render;
		WorldFactory* _factory;
		WorldTextureProvider* _world;
		PerformanceTest::PerformanceRecorder _prefRecord;

		bool _generateMap;
		bool _clearMap;

		bool _showDebugGrid;

		bool _cursorPlacePlaceCell;
		bool _cursorRemovePlaceCell;
		int _cursorWorldX, _cursorWorldY;

		const unsigned int _worldWidth, _worldHeight;

		WorldTextureProvider* InitTest(int seed, ImplementationType testType);
		void TestingAlgorithms();
		bool CompareBuffers(const Render::Texture2D* expected, const Render::Texture2D* result);
	};
}
