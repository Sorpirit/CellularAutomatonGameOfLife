#include "World.hpp"

#include <cstring>

namespace Core
{
	template<typename T>
	World<T>::World(unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
		_stride = sizeof(T);
		_writeBuffer = new T[width * height];
		_readBuffer = new T[width * height];
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
		x = x % _width;
		y = y % _height;

		if (x < 0)
			x = _width + x;

		if (y < 0)
			y = _height + y;

		int offset = (x + y * _width);
		return *(_readBuffer + offset);
	}

	template <typename T>
	void World<T>::Swap()
	{
		auto tmp = _readBuffer;
		_readBuffer = _writeBuffer;
		_writeBuffer = tmp;
	}

	template <typename T>
	void World<T>::ApplyData()
	{
		memcpy(_readBuffer, _writeBuffer, _width * _height * _stride);
	}

	template <typename T>
	unsigned char* World<T>::GetCurrentBuffer()
	{
		return reinterpret_cast<unsigned char*>(_writeBuffer);
	}
}
