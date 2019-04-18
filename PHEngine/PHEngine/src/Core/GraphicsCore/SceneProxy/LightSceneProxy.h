#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Core/GraphicsCore/Shadow/ProjectedShadowInfo.h"

using namespace Graphics;

namespace Graphics
{
   namespace Proxy
   {

      enum class LightSceneProxyType
      {
         DIR_LIGHT,
         SPOT_LIGHT,
         POINT_LIGHT
      };

      class LightSceneProxy
      {
      protected:

         glm::mat4 m_relativeMatrix;

         ProjectedShadowInfo* m_shadowInfo = nullptr;
        
      public:

         glm::vec3 AmbientColor;
         glm::vec3 DiffuseColor;
         glm::vec3 SpecularColor;

         LightSceneProxy(glm::mat4 relativeMatrix, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, ProjectedShadowInfo* shadowInfo = nullptr);
         virtual ~LightSceneProxy();

         virtual LightSceneProxyType GetLightProxyType() const = 0;

         void SetTransformationMatrix(const glm::mat4& relativeMatrix);

         virtual ProjectedShadowInfo* GetShadowInfo() const {

            return m_shadowInfo;
         }

      };

   }
}

