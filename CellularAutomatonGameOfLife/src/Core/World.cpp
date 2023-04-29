#include "World.hpp"

#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstring>

namespace Core
{
	World::World(int width, int height, int stride)
	{
		_width = width;
		_height = height;
		_stride = stride;
		_writeBuffer = new unsigned char[width * height * stride];
		_readBuffer = new unsigned char[width * height * stride];
		memset(_writeBuffer, 0, stride * width * height);
		memset(_readBuffer, 0, stride * width * height);
	}

	World::~World()
	{
		delete[] _writeBuffer;
		delete[] _readBuffer;
	}

	void World::SetPixel(int x, int y, const unsigned char* data)
	{
		int offset = (x + y * _width);
		memcpy(_writeBuffer + offset, data, _stride);
	}

	unsigned char* World::GetPixel(int x, int y)
	{
		auto result = new unsigned char[_stride];

		memcpy(result, _readBuffer + (x + y * _width), _stride);

		return result;
	}

	void World::Swap()
	{
		memcpy(_readBuffer, _writeBuffer, sizeof(_writeBuffer));
		unsigned char* tmp = _readBuffer;
		_readBuffer = _writeBuffer;
		_writeBuffer = tmp;
	}

	unsigned char* World::GetCurrentBuffer()
	{
		return _writeBuffer;
	}
}
