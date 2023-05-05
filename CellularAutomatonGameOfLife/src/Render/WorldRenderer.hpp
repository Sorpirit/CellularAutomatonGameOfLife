#pragma once
#include "../Core/WorldBase.hpp"
#include "Shader.hpp"
#include "ComputeShader.hpp"
#include "Texture2D.hpp"

namespace Render
{
	class WorldRenderer
	{
	public:

		WorldRenderer(Core::WorldBase* world, Shader &shader, ComputeShader &compute);
		~WorldRenderer();

		void Render();

		void EnableGrid(bool enable);
	private:
		Shader _shader;
		ComputeShader _compute;
		Core::WorldBase* _world;
		Texture2D* _outputTexture;
		Texture2D* _readTexture;
		unsigned int _quadVAO;
		bool _gridEnable;

		void initRenderData();
	};
}
