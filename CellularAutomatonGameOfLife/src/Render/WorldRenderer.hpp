#pragma once
#include "../Core/World.hpp"
#include "Shader.hpp"

namespace Render
{
	class WorldRenderer
	{
	public:

		WorldRenderer(Core::World* world, Shader &shader);
		~WorldRenderer();

		void Render();
	private:
		Shader _shader;
		Core::World* _world;
		Texture2D _outputTexture;
		unsigned int _quadVAO;

		void initRenderData();
	};
}
