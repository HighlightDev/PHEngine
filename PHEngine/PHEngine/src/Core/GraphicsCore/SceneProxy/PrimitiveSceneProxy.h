#pragma once

#include <memory>

#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/Texture/ITexture.h"

using namespace Graphics::OpenGL;
using namespace Graphics::Mesh;
using namespace Graphics::Texture;

namespace Graphics
{
   namespace Proxy
   {

      class PrimitiveSceneProxy
      {
      protected:

         glm::mat4 m_relativeMatrix;

         std::shared_ptr<Skin> m_skin;

         std::shared_ptr<ITexture> m_albedoTex;
         std::shared_ptr<ITexture> m_normalMapTex;
         std::shared_ptr<ITexture> m_specularMapTex;

         bool m_IsDeferred = false;

      public:

         PrimitiveSceneProxy(glm::mat4 relativeMatrix, std::shared_ptr<Skin> skin, std::shared_ptr<ITexture> albedoTex,
            std::shared_ptr<ITexture> normalMapTex = std::shared_ptr<ITexture>(),
            std::shared_ptr<ITexture> specularMapTex = std::shared_ptr<ITexture>());

         virtual ~PrimitiveSceneProxy();

         inline bool IsDeferred() const
         {
            return m_IsDeferred;
         }

         virtual std::shared_ptr<ShaderBase> GetShader() = 0;

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) = 0;

         virtual glm::mat4 GetMatrix()
         {
            return m_relativeMatrix;
         }

         void SetTransformationMatrix(const glm::mat4& relativeMatrix)
         {
            m_relativeMatrix = relativeMatrix;
         }

         virtual std::shared_ptr<Skin> GetSkin() const
         {
            return m_skin;
         }

         virtual std::shared_ptr<ITexture> GetAlbedo() const
         {
            return m_albedoTex;
         }

         virtual std::shared_ptr<ITexture> GetNormalMap() const
         {
            return m_normalMapTex;
         }

         virtual std::shared_ptr<ITexture> GetSpecularMap() const
         {
            return m_specularMapTex;
         }
         
      };

   }
}

