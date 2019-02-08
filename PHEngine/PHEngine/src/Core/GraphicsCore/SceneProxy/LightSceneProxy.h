#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

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
        
      public:

         glm::vec3 AmbientColor;
         glm::vec3 DiffuseColor;
         glm::vec3 SpecularColor;


         LightSceneProxy(glm::mat4 relativeMatrix, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor);
         virtual ~LightSceneProxy();

         virtual LightSceneProxyType GetLightProxyType() const = 0;

         void SetTransformationMatrix(glm::mat4& relativeMatrix)
         {
            m_relativeMatrix = relativeMatrix;
         }
      };

   }
}

