#pragma once

#include <GameOfLife/WorldImplementation/WorldTextureProvider.hpp>

namespace GameOfLife
{
	enum ImplementationType
	{
		SingleThreadedCPU,
		MultiThreadedCPU,
		MultiThreadedGPU
	};

	class WorldFactory
	{
	public:
		WorldFactory(Render::WorldRenderer* render) : _render(render) {}

		WorldTextureProvider* CreateWorld(ImplementationType type, unsigned int width, unsigned int height);

	private:
		Render::WorldRenderer* _render;

	};
}
