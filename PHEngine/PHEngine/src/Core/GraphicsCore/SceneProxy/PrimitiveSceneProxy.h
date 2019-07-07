#pragma once

#include <memory>

#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/Texture/ITexture.h"
#include "Core/GameCore/Components/ComponentType.h"
#include "Core/GameCore/GameObject.h"

using namespace Graphics::OpenGL;
using namespace Graphics::Mesh;
using namespace Graphics::Texture;
using namespace Game;

namespace Graphics
{
   namespace Proxy
   {

      class PrimitiveSceneProxy
         : public GameObject
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

         virtual void PostConstructorInitialize();

         virtual std::shared_ptr<ShaderBase> GetShader() const = 0;

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) = 0;

         virtual glm::mat4 GetMatrix() const;

         void SetTransformationMatrix(const glm::mat4& relativeMatrix);

         virtual std::shared_ptr<Skin> GetSkin() const;

         virtual std::shared_ptr<ITexture> GetAlbedo() const;

         virtual std::shared_ptr<ITexture> GetNormalMap() const;

         virtual std::shared_ptr<ITexture> GetSpecularMap() const;

         virtual uint64_t GetComponentType() const;

         inline bool IsDeferred() const
         {
            return m_IsDeferred;
         }
         
      };

   }
}

