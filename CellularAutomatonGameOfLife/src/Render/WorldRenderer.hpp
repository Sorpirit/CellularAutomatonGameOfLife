#pragma once
#include "../Core/WorldBase.hpp"
#include "Shader.hpp"
#include "Texture2D.hpp"

namespace Render
{
	class WorldRenderer
	{
	public:

		WorldRenderer(Core::WorldBase* world, Shader &shader);
		~WorldRenderer();

		void Render();
	private:
		Shader _shader;
		Core::WorldBase* _world;
		Texture2D _outputTexture;
		unsigned int _quadVAO;

		void initRenderData();
	};
}
