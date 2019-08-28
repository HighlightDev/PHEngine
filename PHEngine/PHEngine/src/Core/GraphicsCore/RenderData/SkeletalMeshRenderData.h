#pragma once

#include <memory>
#include <vector>

#include "Core/GraphicsCore/Mesh/Skin.h"
#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/GraphicsCore/Texture/ITexture.h"
#include "Core/GraphicsCore/Animation/AnimationSequence.h"
#include "Core/GraphicsCore/OpenGL/Shader/CompositeShader.h"

using namespace Graphics::Mesh;
using namespace Graphics::OpenGL;
using namespace Graphics::Texture;
using namespace Graphics::Animation;

namespace Graphics
{
   namespace Data
   {

      struct SkeletalMeshRenderData
      {
         std::shared_ptr<Skin> m_skin;
         std::shared_ptr<std::vector<AnimationSequence>> m_animations;

         std::shared_ptr<ICompositeShader> m_materialShader;
         std::shared_ptr<IMaterial> m_material;

         std::shared_ptr<ShaderBase> m_shader;
         std::shared_ptr<ITexture> m_albedo;
         std::shared_ptr<ITexture> m_normalMapTex;
         std::shared_ptr<ITexture> m_specularMapTex;

         SkeletalMeshRenderData(std::shared_ptr<Skin> mesh, std::shared_ptr<std::vector<AnimationSequence>> animations, std::shared_ptr<ICompositeShader> materialShader, std::shared_ptr<IMaterial> material,
            std::shared_ptr<ShaderBase> staticMeshShader, std::shared_ptr<ITexture> albedo,
            std::shared_ptr<ITexture> normalMapTex, std::shared_ptr<ITexture> specularMapTex)
            : m_skin(mesh)
            , m_animations(animations)
            , m_materialShader(materialShader)
            , m_material(material)
            , m_shader(staticMeshShader)
            , m_albedo(albedo)
            , m_normalMapTex(normalMapTex)
            , m_specularMapTex(specularMapTex)
         {
         }

         ~SkeletalMeshRenderData() { }
      };
   }
}
