#pragma once
#include "ProjectedShadowInfo.h"

#include <tuple>

namespace Graphics
{

   class ProjectedPointShadowInfo :
      public ProjectedShadowInfo
   {
      using mat4x4tuple_t = std::tuple<glm::mat4x4, glm::mat4x4, glm::mat4x4, glm::mat4x4, glm::mat4x4, glm::mat4x4>;

      mat4x4tuple_t m_shadowViewMatrix;

      mat4x4tuple_t m_shadowProjectionMatrix;

      std::shared_ptr<TextureCubeAtlasHandler>  GetTextureCubeHandler() const;

   public:

      ProjectedPointShadowInfo(const LazyTextureAtlasObtainer& shadowAtlasCellResource);

      virtual ~ProjectedPointShadowInfo();

      virtual void BindShadowFramebuffer(bool clearDepthBuffer) const override;

      mat4x4tuple_t GetShadowViewMatrices() const;

      mat4x4tuple_t GetShadowProjectionMatrices() const;

      void SetShadowViewMatrices(const mat4x4tuple_t& shadowViewMatrices);

      void SetShadowProjectionMatrix(const mat4x4tuple_t& shadowProjectionMatrices);

      glm::mat4 GetShadowMatrix() const;

      glm::vec4 GetPosOffsetShadowMapAtlas() const;
   };
}
