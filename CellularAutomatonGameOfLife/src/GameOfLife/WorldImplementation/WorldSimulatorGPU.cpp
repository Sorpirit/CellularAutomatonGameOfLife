#include "glad/glad.h"
#include "WorldSimulatorGPU.hpp"
#include "WorldTextureHelper.hpp"

#include <GameOfLife/ResourceConstants.hpp>
#include <GameOfLife/CellDefaults.hpp>

#include <Render/ResourceManager.hpp>

namespace GameOfLife
{

    WorldSimulatorGPU::WorldSimulatorGPU(unsigned width, unsigned height, Render::WorldRenderer* render) :
        WorldTextureProvider(render), _width(width), _height(height)
    {
    }

    WorldSimulatorGPU::~WorldSimulatorGPU()
    {
    }

    void WorldSimulatorGPU::Init()
    {
        _computeShader = Render::ResourceManager::GetComputeShader(WORLD_SIMULATION_SHADER);
        _readTexture = DefaultTexture();
        _writeTexture = DefaultTexture();
    }

    void WorldSimulatorGPU::UpdateWorld(float dt)
    {
        auto tmp = _writeTexture;
        _writeTexture = _readTexture;
        _readTexture = tmp;

        glActiveTexture(GL_TEXTURE0);
        _readTexture->Bind();
        glBindImageTexture(0, _readTexture->ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R8UI);

        glActiveTexture(GL_TEXTURE1);
        _writeTexture->Bind();
        _writeTexture->Generate(_width, _height, nullptr);
        glBindImageTexture(1, _writeTexture->ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R8UI);

        _computeShader.Use();
        _computeShader.SetVector2i("dimensions", _width, _height);
        _computeShader.SetVector2i("placePixel", _cursorCellX, _cursorCellY);
        _computeShader.SetInteger("simulationMode", getSimulationMode());
        glDispatchCompute(_width, _height, 1);

        // make sure writing to image has finished before read
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        //reset vars
        _placeCell = false;
        _removeCell = false;
    }

    void WorldSimulatorGPU::SetCell(int x, int y, Core::Cell cell)
    {
        _cursorCellX = x;
        _cursorCellY = y;
        _placeCell = cell.Data == FullCell.Data;
        _removeCell = cell.Data == EmptyCell.Data;
    }

    void WorldSimulatorGPU::ClearWorld()
    {
        const auto writeBuffer = new Core::Cell[_width * _height];
        for (int i = 0; i < _width; ++i)
        {
            for (int j = 0; j < _height; ++j)
            {
                const int offset = (i + j * _width);
                *(writeBuffer + offset) =  EmptyCell;
            }
        }

        const auto imagePointer = reinterpret_cast<unsigned char*>(writeBuffer);
        glActiveTexture(GL_TEXTURE0);
        _writeTexture->Bind();
        _writeTexture->Generate(_width, _height, imagePointer);
    }

    void WorldSimulatorGPU::GenerateWorld()
    {
	    const auto writeBuffer = new Core::Cell[_width * _height];
        for (int i = 0; i < _width; ++i)
        {
            for (int j = 0; j < _height; ++j)
            {
	            const int offset = (i + j * _width);
                *(writeBuffer + offset) = rand() % 2 == 0 ? FullCell : EmptyCell;
            }
        }

	    const auto imagePointer = reinterpret_cast<unsigned char*>(writeBuffer);
        glActiveTexture(GL_TEXTURE0);
        _writeTexture->Bind();
        _writeTexture->Generate(_width, _height, imagePointer);
    }

    const Render::Texture2D* WorldSimulatorGPU::GetCurrentWorldTexture()
    {
        if(_placeCell ||_removeCell)
        {
            UpdateWorld(0);
        }

        return _writeTexture;
    }

    int WorldSimulatorGPU::getSimulationMode()
    {
        if (_placeCell)
            return 1;

        if (_removeCell)
            return 2;

        return 0;
    }
}
