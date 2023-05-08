#include "WorldFactory.hpp"

#include <GameOfLife/WorldImplementation/WorldSimulatorCPU.hpp>
#include <GameOfLife/WorldImplementation/WorldSimulatorCPUMultiThread.hpp>
#include <GameOfLife/WorldImplementation/WorldSimulatorGPU.hpp>

namespace GameOfLife
{
	WorldTextureProvider* WorldFactory::CreateWorld(ImplementationType type, unsigned int width, unsigned int height)
	{
		switch (type)
		{
		case SingleThreadedCPU:
			return new WorldSimulatorCPU(width, height, _render);

		case MultiThreadedCPU:
			return new WorldSimulatorCPUMultiThread(width, height, _render);

		case MultiThreadedGPU:
			return new WorldSimulatorGPU(width, height, _render);
		}

		return new WorldSimulatorCPU(width, height, _render);
	}
}