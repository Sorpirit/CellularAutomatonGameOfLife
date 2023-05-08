#pragma once

#include <Core/WorldSimulator.hpp>

#include <Render/Texture2D.hpp>
#include <Render/WorldRenderer.hpp>

namespace GameOfLife
{
	class WorldTextureProvider : public Core::WorldSimulator
	{
	public:
		WorldTextureProvider(Render::WorldRenderer* render) : _render(render) { }

		virtual const Render::Texture2D* GetCurrentWorldTexture() = 0;
	protected:
		Render::WorldRenderer* _render;
	};
}
