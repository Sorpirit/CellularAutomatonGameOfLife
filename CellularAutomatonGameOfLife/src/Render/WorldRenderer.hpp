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

		WorldRenderer(Shader &shader);
		~WorldRenderer();

		void Render(const Texture2D* drawTexture);

		void EnableGrid(bool enable);

	private:
		Shader _shader;
		unsigned int _quadVAO;
		bool _gridEnable;

		void initRenderData();
	};
}
