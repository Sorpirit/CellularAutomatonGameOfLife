#pragma once

#include <Core/Cell.hpp>

namespace Core
{
	class WorldSimulator
	{
	public:
		virtual void Init() = 0;
		virtual void UpdateWorld(float dt) = 0;
		virtual void SetCell(int x, int y, Core::Cell cell) = 0;
		virtual void ClearWorld() = 0;
		virtual void GenerateWorld() = 0;
	};

}