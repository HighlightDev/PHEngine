#include "SimplePrimitiveAllocationPolicy.h"
#include "Core/GraphicsCore/OpenGL/VertexArrayObject.h"
#include "Core/GraphicsCore/OpenGL/IndexBufferObject.h"
#include "Core/GraphicsCore/OpenGL/VertexBufferObject.h"

#include <gl/glew.h>

using namespace Graphics::OpenGL;
using namespace Graphics::Mesh;

namespace Resources
{

   template class SimplePrimitiveAllocationPolicy<int32_t>;

   template <typename Model>
   std::shared_ptr<Skin> SimplePrimitiveAllocationPolicy<Model>::AllocateMemory(int32_t arg)
   {
      auto typeArg = SimplePrimitiveType(arg);

      std::shared_ptr<Skin> resultSkin;

      {
         std::unique_ptr<VertexArrayObject> vao = std::make_unique<VertexArrayObject>();

         std::shared_ptr<std::vector<float>> vertices;

         switch (typeArg)
         {
            case SimplePrimitiveType::POINT:
               vertices = std::make_shared<std::vector<float>>(std::vector<float>({ 0.0f, 0.0f, 0.0f }));
               break;
            case SimplePrimitiveType::CUBE:
               vertices = std::make_shared<std::vector<float>>(std::vector<float>({
                  // back face
                  -1.0f, -1.0f, -1.0f, // bottom-left
                   1.0f,  1.0f, -1.0f, // top-right
                   1.0f, -1.0f, -1.0f, // bottom-right         
                   1.0f,  1.0f, -1.0f, // top-right
                  -1.0f, -1.0f, -1.0f, // bottom-left
                  -1.0f,  1.0f, -1.0f, // top-left
                  // front face
                  -1.0f, -1.0f,  1.0f, // bottom-left
                   1.0f, -1.0f,  1.0f, // bottom-right
                   1.0f,  1.0f,  1.0f, // top-right
                   1.0f,  1.0f,  1.0f, // top-right
                  -1.0f,  1.0f,  1.0f, // top-left
                  -1.0f, -1.0f,  1.0f, // bottom-left
                  // left face
                  -1.0f,  1.0f,  1.0f, // top-right
                  -1.0f,  1.0f, -1.0f, // top-left
                  -1.0f, -1.0f, -1.0f, // bottom-left
                  -1.0f, -1.0f, -1.0f, // bottom-left
                  -1.0f, -1.0f,  1.0f, // bottom-right
                  -1.0f,  1.0f,  1.0f, // top-right
                  // right face
                   1.0f,  1.0f,  1.0f, // top-left
                   1.0f, -1.0f, -1.0f, // bottom-right
                   1.0f,  1.0f, -1.0f, // top-right         
                   1.0f, -1.0f, -1.0f, // bottom-right
                   1.0f,  1.0f,  1.0f, // top-left
                   1.0f, -1.0f,  1.0f, // bottom-left     
                  // bottom face
                  -1.0f, -1.0f, -1.0f, // top-right
                   1.0f, -1.0f, -1.0f, // top-left
                   1.0f, -1.0f,  1.0f, // bottom-left
                   1.0f, -1.0f,  1.0f, // bottom-left
                  -1.0f, -1.0f,  1.0f, // bottom-right
                  -1.0f, -1.0f, -1.0f, // top-right
                  // top face
                  -1.0f,  1.0f, -1.0f, // top-left
                   1.0f,  1.0f , 1.0f, // bottom-right
                   1.0f,  1.0f, -1.0f, // top-right     
                   1.0f,  1.0f,  1.0f, // bottom-right
                  -1.0f,  1.0f, -1.0f, // top-left
                  -1.0f,  1.0f,  1.0f // bottom-left        
               }));
               break;
            default:
               vertices = std::make_shared<std::vector<float>>();
               break;
         }

         VertexBufferObjectBase* vertexVBO;

         vertexVBO = new VertexBufferObject<float, 3, GL_FLOAT>(vertices, GL_ARRAY_BUFFER, 0, DataCarryFlag::Invalidate);

         vao->AddVBO(std::move(std::unique_ptr<VertexBufferObjectBase>(vertexVBO)));

         vao->BindBuffersToVao();

         resultSkin = std::make_shared<Skin>(std::move(vao));
      }

      return resultSkin;
   }

   template <typename Model>
   void SimplePrimitiveAllocationPolicy<Model>::DeallocateMemory(const std::shared_ptr<Skin>& arg)
   {
      arg->CleanUp();
   }

}
