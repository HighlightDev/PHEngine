#pragma once

#include <memory>

#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

using namespace Graphics::Mesh;
using namespace Graphics::OpenGL;
using namespace Graphics::Texture;

namespace Graphics
{
   namespace Data
   {

      struct StaticMeshRenderData
      {
         std::shared_ptr<Skin> m_skin;
         std::shared_ptr<ShaderBase> m_shader;
         std::shared_ptr<ITexture> m_albedo;
         std::shared_ptr<ITexture> m_normalMapTex;
         std::shared_ptr<ITexture> m_specularMapTex;

         StaticMeshRenderData(std::shared_ptr<Skin> staticMesh,
            std::shared_ptr<ShaderBase> staticMeshShader, std::shared_ptr<ITexture> albedo,
            std::shared_ptr<ITexture> normalMapTex, std::shared_ptr<ITexture> specularMapTex)
            : m_skin(staticMesh)
            , m_shader(staticMeshShader)
            , m_albedo(albedo)
            , m_normalMapTex(normalMapTex)
            , m_specularMapTex(specularMapTex)
         {
         }

         ~StaticMeshRenderData() { }
      };
   }
}
