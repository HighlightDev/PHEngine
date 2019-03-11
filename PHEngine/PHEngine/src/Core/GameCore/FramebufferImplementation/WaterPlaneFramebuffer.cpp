#include "WaterPlaneFramebuffer.h"
#include "Core/ResourceManagerCore/Pool/RenderTargetPool.h"

using namespace Resources;

namespace Game
{
   namespace FramebufferImpl
   {

      WaterPlaneFramebuffer::WaterPlaneFramebuffer()
      {
         Init();
      }

      WaterPlaneFramebuffer::~WaterPlaneFramebuffer()
      {

      }

      void WaterPlaneFramebuffer::SetTextures()
      {
         TexParams params;
         ReflectionTexture = RenderTargetPool::GetInstance()->GetOrAllocateResource(params);
         RefractionTexture = RenderTargetPool::GetInstance()->GetOrAllocateResource(params);
         DepthTexture = RenderTargetPool::GetInstance()->GetOrAllocateResource(params);
        /* ReflectionTexture = PoolProxy.GetResource<GetRenderTargetPool, RenderTargetAllocationPolicy, TextureParameters, ITexture>(new TextureParameters(TextureTarget.Texture2D, TextureMagFilter.Nearest, TextureMinFilter.Nearest, 0, PixelInternalFormat.Rgb, (Int32)(EngineStatics.globalSettings.DomainFramebufferRezolution.X / 1.5), (Int32)(EngineStatics.globalSettings.DomainFramebufferRezolution.Y / 1.5), PixelFormat.Rgb, PixelType.UnsignedByte, TextureWrapMode.Repeat));
         RefractionTexture = PoolProxy.GetResource<GetRenderTargetPool, RenderTargetAllocationPolicy, TextureParameters, ITexture>(new TextureParameters(TextureTarget.Texture2D, TextureMagFilter.Nearest, TextureMinFilter.Nearest, 0, PixelInternalFormat.Rgb, (Int32)(EngineStatics.globalSettings.DomainFramebufferRezolution.X / 1.5), (Int32)(EngineStatics.globalSettings.DomainFramebufferRezolution.Y / 1.5), PixelFormat.Rgb, PixelType.UnsignedByte, TextureWrapMode.Repeat));
         DepthTexture = PoolProxy.GetResource<GetRenderTargetPool, RenderTargetAllocationPolicy, TextureParameters, ITexture>(new TextureParameters(TextureTarget.Texture2D, TextureMagFilter.Nearest, TextureMinFilter.Nearest, 0, PixelInternalFormat.Depth24Stencil8,
            (Int32)(EngineStatics.globalSettings.DomainFramebufferRezolution.X / 1.5f), (Int32)(EngineStatics.globalSettings.DomainFramebufferRezolution.Y / 1.5f), PixelFormat.DepthComponent, PixelType.Float, TextureWrapMode.Repeat));*/
      }

      void WaterPlaneFramebuffer::SetFramebuffers()
      {

      }

      void WaterPlaneFramebuffer::SetRenderbuffers()
      {

      }

      void WaterPlaneFramebuffer::CleanUp()
      {

      }

   }
}
