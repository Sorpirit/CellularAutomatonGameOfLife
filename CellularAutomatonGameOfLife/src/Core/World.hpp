#pragma once
#include "WorldBase.hpp"

namespace Core
{
	template<typename T>
	class World : public WorldBase
	{
	public:
		World(int width, int height);
		~World();

		void SetCell(int x, int y, const T data);
		T GetCell(int x, int y);
		void Swap();

		unsigned char* GetCurrentBuffer() override;
	private:
		T* _readBuffer;
		T* _writeBuffer;
	};
}
