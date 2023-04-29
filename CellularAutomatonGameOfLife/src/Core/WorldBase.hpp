#pragma once

namespace Core
{
	class WorldBase
	{
	public:

		inline int GetWidth() const { return _width; }
		inline int GetHeight() const { return _height; }
	
		virtual  unsigned char* GetCurrentBuffer() = 0;
	protected:
		int _width;
		int _height;
		int _stride;
	
	};
}
