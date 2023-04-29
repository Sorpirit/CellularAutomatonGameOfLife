#include "World.hpp"

#include <iostream>
#include <cstring>

namespace Core
{
	template<typename T>
	World<T>::World(int width, int height)
	{
		_width = width;
		_height = height;
		_stride = sizeof(T);
		_writeBuffer = new T[width * height];
		_readBuffer = new T[width * height];

		//todo default init of the buffers
	}

	template<typename T>
	World<T>::~World()
	{
		delete[] _writeBuffer;
		delete[] _readBuffer;
	}

	template <typename T>
	void World<T>::SetCell(int x, int y, const T data)
	{
		int offset = (x + y * _width);
		*(_writeBuffer + offset) = data;
	}

	template <typename T>
	T World<T>::GetCell(int x, int y)
	{
		int offset = (x + y * _width);
		return *(_writeBuffer + offset);
	}

	template <typename T>
	void World<T>::Swap()
	{
		memcpy(_readBuffer, _writeBuffer, sizeof(_writeBuffer));
		unsigned char* tmp = _readBuffer;
		_readBuffer = _writeBuffer;
		_writeBuffer = tmp;
	}

	template <typename T>
	unsigned char* World<T>::GetCurrentBuffer()
	{
		return reinterpret_cast<unsigned char*>(_writeBuffer);
	}
}
