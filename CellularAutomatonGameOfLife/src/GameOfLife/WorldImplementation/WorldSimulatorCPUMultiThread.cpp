#include "WorldSimulatorCPUMultiThread.hpp"
#include <GameOfLife/CellDefaults.hpp>
#include <GameOfLife/WorldHelpers.hpp>

namespace GameOfLife
{
	WorldSimulatorCPUMultiThread::WorldSimulatorCPUMultiThread(unsigned width, unsigned height, Render::WorldRenderer* render)
	: WorldSimulatorCPU(width, height, render)
	{
	}

	void WorldSimulatorCPUMultiThread::UpdateWorld(float dt)
	{
		_world.Swap();

#pragma omp parallel for num_threads(4)
		for (int i = 0; i < _world.GetWidth(); ++i)
		{
#pragma omp parallel for num_threads(4)
			for (int j = 0; j < _world.GetHeight(); ++j)
			{
				int neighborAmount = 0;
				for (const auto& dir : NeighborDirections) {
					int neighborX = i + std::get<0>(dir);
					int neighborY = j + std::get<1>(dir);
					const Core::Cell neighbor = _world.GetCell(neighborX, neighborY);
					if (neighbor.Data == FullCell.Data)
						neighborAmount++;
				}

				bool isCellFull = _world.GetCell(i, j).Data == FullCell.Data;

				if (isCellFull)
				{
					if (neighborAmount >= 2 && neighborAmount <= 3)
					{
						_world.SetCell(i, j, FullCell);
					}
					else
					{
						_world.SetCell(i, j, EmptyCell);
					}
				}
				else
				{
					if (neighborAmount == 3)
					{
						_world.SetCell(i, j, FullCell);
					}
					else
					{
						_world.SetCell(i, j, EmptyCell);
					}
				}
			}
		}
	}
	
}
