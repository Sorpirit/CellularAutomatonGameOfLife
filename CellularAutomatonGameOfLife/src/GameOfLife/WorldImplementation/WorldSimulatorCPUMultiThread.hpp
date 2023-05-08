#pragma once

#include "WorldSimulatorCPU.hpp"

namespace GameOfLife
{
	class WorldSimulatorCPUMultiThread : public WorldSimulatorCPU
	{
	public:
		WorldSimulatorCPUMultiThread(unsigned int width, unsigned int height, Render::WorldRenderer* render);

		void UpdateWorld(float dt) override;
	};
	
}
