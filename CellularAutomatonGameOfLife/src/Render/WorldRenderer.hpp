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

		void EnableGrid(bool enable);
	private:
		Shader _shader;
		Core::WorldBase* _world;
		Texture2D _outputTexture;
		unsigned int _quadVAO;
		bool _gridEnable;

		void initRenderData();
	};
}
