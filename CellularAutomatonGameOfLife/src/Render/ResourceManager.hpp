#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "Texture2D.hpp"
#include "Shader.hpp"
#include "ComputeShader.hpp"

namespace Render
{
    class ResourceManager
    {
    public:
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2D> Textures;
        static std::map<std::string, ComputeShader> ComputeShaders;

    	static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    	static ComputeShader LoadShader(const char* computeShader, std::string name);
        static Shader GetShader(std::string name);
        static ComputeShader GetComputeShader(std::string name);

    	static Texture2D LoadTexture(const char* file, bool alpha, std::string name);
        static Texture2D GetTexture(std::string name);

    	static void Clear();
    private:

    	ResourceManager() { }

    	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        static ComputeShader loadShaderFromFile(const char* computeShaderFile);

    	static Texture2D loadTextureFromFile(const char* file, bool alpha);
    };
}
