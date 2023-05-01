#include "WorldRenderer.hpp"
#include "ResourceManager.hpp"

namespace Render
{
	WorldRenderer::WorldRenderer(Core::WorldBase* world, Shader& shader)
		: _world(world)
	{
		_shader = shader;
		initRenderData();
	}

	WorldRenderer::~WorldRenderer()
	{
	}

	void WorldRenderer::Render()
	{
        _outputTexture.Generate(_world->GetWidth(), _world->GetHeight(), _world->GetCurrentBuffer());

        _shader.Use();
        _shader.SetInteger("image", 0);
        _shader.SetVector3f("spriteColor", {1, 1, 1});
        _shader.SetVector2f("spriteDimensions", { _world->GetWidth(), _world->GetHeight()});
        _shader.SetFloat("edgeThickness", 0.05f);
        _shader.SetVector3f("edgeColor", _gridEnable ? glm::vec3(0.4, 0.4, 0.4f) : glm::vec3(0,0,0));
        
        glActiveTexture(GL_TEXTURE0);
        _outputTexture.Bind();
        
		glBindVertexArray(_quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
	}

	void WorldRenderer::EnableGrid(bool enable)
	{
        _gridEnable = enable;
	}

	void WorldRenderer::initRenderData()
	{
        _outputTexture.Wrap_S = GL_CLAMP_TO_BORDER;
        _outputTexture.Wrap_T = GL_CLAMP_TO_BORDER;
        _outputTexture.Filter_Max = GL_NEAREST;
        _outputTexture.Filter_Min = GL_NEAREST;
        _outputTexture.Internal_Format = GL_R8UI;
        _outputTexture.Image_Format = GL_RED_INTEGER;
        //_outputTexture.Generate(_world->GetWidth(), _world->GetHeight(), _world->GetCurrentBuffer());
        

        // configure VAO/VBO
        unsigned int VBO;
        float vertices[] = {
            // pos      // tex
            -1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f, 0.0f,

            -1.0f, -1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &_quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(_quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}
}
