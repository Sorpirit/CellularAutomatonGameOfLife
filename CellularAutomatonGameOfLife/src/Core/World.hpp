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

		/*swaps reading and writing array*/
		void Swap();

		/*copies writing array to reading. Useful when it needed to initialise buffer*/
		void ApplyData();

		unsigned char* GetCurrentBuffer() override;
	private:
		T* _readBuffer;
		T* _writeBuffer;
	};
}
