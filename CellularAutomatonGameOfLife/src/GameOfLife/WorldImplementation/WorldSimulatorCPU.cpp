#include <Core/World.cpp>

#include "WorldSimulatorCPU.hpp"
#include "WorldTextureHelper.hpp"

#include <GameOfLife/WorldHelpers.hpp>

#include <omp.h>

namespace GameOfLife
{
	WorldSimulatorCPU::WorldSimulatorCPU(unsigned int width, unsigned int height, Render::WorldRenderer* render)
	: WorldTextureProvider(render), _world(width, height)
	{
		GenerateWorld();
	}

	WorldSimulatorCPU::~WorldSimulatorCPU()
	{
		
	}

	void WorldSimulatorCPU::Init()
	{
		_outputTexture = DefaultTexture();
	}

	void WorldSimulatorCPU::UpdateWorld(float dt)
	{
        _world.Swap();
        
        for (int i = 0; i < _world.GetWidth(); ++i)
        {
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

	void WorldSimulatorCPU::SetCell(int x, int y, Core::Cell cell)
	{
		_world.SetCell(x, y, cell);
	}

	void WorldSimulatorCPU::ClearWorld()
	{
		ClearMap(_world);
	}

	void WorldSimulatorCPU::GenerateWorld()
	{
		GenerateRandomMap(_world);
	}

	const Render::Texture2D* WorldSimulatorCPU::GetCurrentWorldTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		_outputTexture->Generate(_world.GetWidth(), _world.GetHeight(), _world.GetCurrentBuffer());
        return _outputTexture;
	}
}
