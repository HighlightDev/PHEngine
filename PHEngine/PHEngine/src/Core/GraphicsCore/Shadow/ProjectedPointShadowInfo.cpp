#include "ProjectedPointShadowInfo.h"

namespace Graphics
{
   ProjectedPointShadowInfo::ProjectedPointShadowInfo(const LazyTextureAtlasObtainer& shadowAtlasCellResource)
      : ProjectedShadowInfo(shadowAtlasCellResource)
   {
      m_lightType = LightType::POINT_LIGHT;
   }

   ProjectedPointShadowInfo::~ProjectedPointShadowInfo()
   {
   }

   std::shared_ptr<TextureCubeAtlasHandler> ProjectedPointShadowInfo::GetTextureCubeHandler() const
   {
      std::shared_ptr<TextureCubeAtlasHandler> result;

      auto resourceObtainer = m_shadowAtlasCellResource.GetTextureAtlasCellResource();
      if (resourceObtainer && resourceObtainer->GetTextureType() == TextureType::TEXTURE_CUBE)
      {
         result = std::static_pointer_cast<TextureCubeAtlasHandler>(resourceObtainer);
      }

      return result;
   }

   void ProjectedPointShadowInfo::BindShadowFramebuffer(bool clearDepthBuffer) const 
   {
      ProjectedShadowInfo::BindShadowFramebuffer(clearDepthBuffer);

      auto texAtlas = GetTextureCubeHandler();
      auto rezolution = texAtlas->GetAtlasResource()->GetTextureRezolution();
      const GLbitfield clearDepthFlag = clearDepthBuffer ? GL_DEPTH_BUFFER_BIT : 0;
      m_shadowFramebuffer->RenderToFBO(1, 0, 0, rezolution.x, rezolution.y, clearDepthFlag);
      glDrawBuffer(GL_NONE);
   }

   ProjectedPointShadowInfo::mat4x4tuple_t ProjectedPointShadowInfo::GetShadowViewMatrices() const
   {
      return m_shadowViewMatrix;
   }

   ProjectedPointShadowInfo::mat4x4tuple_t ProjectedPointShadowInfo::GetShadowProjectionMatrices() const
   {
      return m_shadowProjectionMatrix;
   }

   void ProjectedPointShadowInfo::SetShadowViewMatrices(const mat4x4tuple_t& shadowViewMatrices)
   {
      m_shadowViewMatrix = shadowViewMatrices;
   }

   void ProjectedPointShadowInfo::SetShadowProjectionMatrix(const mat4x4tuple_t& shadowProjectionMatrices)
   {
      m_shadowProjectionMatrix = shadowProjectionMatrices;
   }

   ProjectedPointShadowInfo::mat4x4tuple_t ProjectedPointShadowInfo::GetShadowMatrix() const
   {
      ProjectedPointShadowInfo::mat4x4tuple_t result;

      // 0
      glm::mat4 shadowMatrix(1);
      shadowMatrix *= m_shadowBiasMatrix;
      shadowMatrix *= std::get<0>(m_shadowProjectionMatrix);
      shadowMatrix *= std::get<0>(m_shadowViewMatrix);
      std::get<0>(result) = shadowMatrix;
      // 1
      shadowMatrix = glm::mat4(1);
      shadowMatrix *= m_shadowBiasMatrix;
      shadowMatrix *= std::get<1>(m_shadowProjectionMatrix);
      shadowMatrix *= std::get<1>(m_shadowViewMatrix);
      std::get<1>(result) = shadowMatrix;
      // 2
      shadowMatrix = glm::mat4(1);
      shadowMatrix *= m_shadowBiasMatrix;
      shadowMatrix *= std::get<2>(m_shadowProjectionMatrix);
      shadowMatrix *= std::get<2>(m_shadowViewMatrix);
      std::get<2>(result) = shadowMatrix;
      // 3
      shadowMatrix = glm::mat4(1);
      shadowMatrix *= m_shadowBiasMatrix;
      shadowMatrix *= std::get<3>(m_shadowProjectionMatrix);
      shadowMatrix *= std::get<3>(m_shadowViewMatrix);
      std::get<3>(result) = shadowMatrix;
      // 4
      shadowMatrix = glm::mat4(1);
      shadowMatrix *= m_shadowBiasMatrix;
      shadowMatrix *= std::get<4>(m_shadowProjectionMatrix);
      shadowMatrix *= std::get<4>(m_shadowViewMatrix);
      std::get<4>(result) = shadowMatrix;
      // 5
      shadowMatrix = glm::mat4(1);
      shadowMatrix *= m_shadowBiasMatrix;
      shadowMatrix *= std::get<5>(m_shadowProjectionMatrix);
      shadowMatrix *= std::get<5>(m_shadowViewMatrix);
      std::get<5>(result) = shadowMatrix;

      return result;
   }
}
