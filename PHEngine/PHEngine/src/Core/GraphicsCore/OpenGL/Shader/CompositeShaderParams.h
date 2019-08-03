#pragma once

#include "Core/GraphicsCore/OpenGL/Shader/ShaderBase.h"
#include "Core/CommonApi/StringHash.h"

#include <memory>

namespace Graphics
{
   namespace OpenGL
   {
      struct CompositeShaderParams
      {

         std::shared_ptr<ShaderBase> mShader;
         std::string ShaderName;

         const size_t HASH;

         CompositeShaderParams(const size_t hash, const std::string& shaderName, const std::shared_ptr<ShaderBase>& shader)
            : mShader(shader)
            , ShaderName(shaderName)
            , HASH(hash)
         {
         }

         bool operator==(const CompositeShaderParams& other) const {

            return this->ShaderName == other.ShaderName
               && this->HASH == other.HASH;
         }
      };

#define NAME_TO_STR(name) #name
#define COMPOSITE_SHADER_TO_STR(name1, name2, name3) NAME_TO_STR(name1) ## NAME_TO_STR(name2) ## NAME_TO_STR(name3)

      template <typename CompositeShaderType>
      struct TemplatedCompositeShaderParams 
         : public CompositeShaderParams
      {

         TemplatedCompositeShaderParams(const std::string& uniqueName, const ShaderParams& shaderParams)
            : CompositeShaderParams(
               Game::Hash(uniqueName),
               uniqueName,
               std::make_shared<typename CompositeShaderType::shader_t>(shaderParams))
         {

         }
      };
   }
}

namespace std
{
   using namespace Graphics::OpenGL;

   template<>
   struct hash<CompositeShaderParams>
   {
      std::size_t operator()(const CompositeShaderParams& k) const
      {
         return k.HASH;
      }
   };
}


