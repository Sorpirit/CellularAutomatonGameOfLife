#pragma once

#include <Core/World.hpp>
#include <Core/WorldSimulator.hpp>

#include <Render/WorldRenderer.hpp>

#include "WorldTextureProvider.hpp"


namespace GameOfLife
{
	class WorldSimulatorCPU : public WorldTextureProvider
	{
	public:

		WorldSimulatorCPU(unsigned int width, unsigned int height, Render::WorldRenderer* render);
		~WorldSimulatorCPU();
		void Init() override;
		void UpdateWorld(float dt) override;
		void SetCell(int x, int y, Core::Cell cell) override;
		void ClearWorld() override;
		void GenerateWorld() override;
		const Render::Texture2D* GetCurrentWorldTexture() override;

	protected:
		Core::World<Core::Cell> _world;
		Render::Texture2D* _outputTexture;
	};

}
