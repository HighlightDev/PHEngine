#pragma once
#include "PrimitiveSceneProxy.h"
#include "Core/GameCore/Components/WaterPlaneComponent.h"
#include "Core/GameCore/FramebufferImplementation/WaterPlaneFramebuffer.h"
#include "Core/GameCore/ShaderImplementation/WaterPlaneShader.h"

using namespace Game;
using namespace Game::FramebufferImpl;
using namespace Game::ShaderImpl;

namespace Graphics
{
   namespace Proxy
   {

      class WaterPlaneSceneProxy :
         public PrimitiveSceneProxy
      {

         std::unique_ptr<WaterPlaneFramebuffer> m_waterPlaneFramebuffer;
         std::shared_ptr<ITexture> m_waterDistortionMap;
         std::shared_ptr<ITexture> m_waterNormalMap;
         std::shared_ptr<Skin> m_skin;
         std::shared_ptr<WaterPlaneShader> m_shader;

         float m_moveFactor;
         float m_waveStrength;
         float m_transparencyDepth;
         float m_nearClipPlane;
         float m_farClipPlane;

      public:

         WaterPlaneSceneProxy(const WaterPlaneComponent* component);

         virtual ~WaterPlaneSceneProxy();

         virtual void Render(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) override;

         virtual std::shared_ptr<ShaderBase> GetShader() override;

         virtual uint64_t GetComponentType() const override;

         void SetMoveFactor(float moveFactor);
         void SetWaveStrength(float waveStr);
         void SetTransparencyDepth(float transparencyDepth);
         void SetNearClipPlane(float nearClipPlane);
         void SetFarClipPlane(float farClipPlane);

      private:

         void Init();
      };
   }
}

