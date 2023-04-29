#pragma once

#include "../Render/Texture2D.hpp"

namespace Core
{
	class World
	{
	public:
		World(int width, int height, int stride);
		~World();

		inline int GetWidth() { return _width; }
		inline int GetHeight() { return _height; }

		void SetPixel(int x, int y, const unsigned char* data);
		unsigned char* GetPixel(int x, int y);
		void Swap();

		unsigned char* GetCurrentBuffer();
	private:
		unsigned char* _readBuffer;
		unsigned char* _writeBuffer;
		int _width;
		int _height;
		int _stride;
	};
}
