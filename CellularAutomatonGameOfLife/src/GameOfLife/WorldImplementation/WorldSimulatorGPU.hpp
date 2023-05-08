#pragma once
#include "WorldTextureProvider.hpp"
#include "Render/Shader.hpp"

namespace GameOfLife
{
	class WorldSimulatorGPU : public GameOfLife::WorldTextureProvider
	{
	public:

		WorldSimulatorGPU(unsigned int width, unsigned int height, Render::WorldRenderer* render);
		~WorldSimulatorGPU();
		void Init() override;
		void UpdateWorld(float dt) override;
		void SetCell(int x, int y, Core::Cell cell) override;
		void ClearWorld() override;
		void GenerateWorld() override;
		const Render::Texture2D* GetCurrentWorldTexture() override;

	private:
		Render::Texture2D* _writeTexture;
		Render::Texture2D* _readTexture;
		Render::ComputeShader _computeShader;
		const unsigned int _width, _height;
		
		bool _placeCell;
		bool _removeCell;
		int _cursorCellX, _cursorCellY;

		int getSimulationMode();
	};
};

