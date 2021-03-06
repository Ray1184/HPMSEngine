/**
 * File GLRenderer.h
 */
#pragma once

#include <thirdparty/glad/glad.h>
#include <unordered_map>
#include <core/Renderer.h>

namespace hpms
{
    class GLRenderer : public hpms::Renderer
    {

    public:
        GLRenderer()
        {
            LOG_DEBUG("OpenGL rendering module created.");
        }

        void QuadMeshInit() override;

        void MeshInit(const hpms::Mesh& mesh) override;

        void
        ModelsDraw(const std::unordered_map<const AdvModelItem*, std::vector<Entity*>>& modelToEntitiesMap,
                   Shader* s, Camera* c, Window* w,
                   std::function<void(Entity*, Shader*, Camera*, Window* w)> pipelineCallback) override;

        void QuadsDraw(const std::string& textureName) override;

        void MeshCleanup(const hpms::Mesh& mesh) override;

        void TextureInit(const hpms::Texture& text) override;

        void TextureCleanup(const hpms::Texture& text) override;

        void QuadMeshCleanup() override;

        void ClearAllBuffers() override;

        void ClearDepthBuffer() override;

        inline const std::string Name() const override
        {
            return "GLRenderer";
        }

    private:

        GLuint quadVbo;

        GLuint quadVao;

        std::unordered_map<std::string, GLuint> vaoMap;

        std::unordered_map<std::string, GLuint> texBufferMap;

        std::unordered_map<std::string, std::vector<GLuint>> vbosMap;


    };
}
