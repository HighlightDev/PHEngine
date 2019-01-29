#include "DeferredRenderer.h"

#include <gl/glew.h>
#include <iostream>

namespace Graphics
{
	namespace Renderer
	{

		DeferredRenderer::DeferredRenderer(int32_t windowWidth, int32_t windowHeight)
			: m_windowWidth(windowWidth)
			, m_windowHeight(windowHeight)
			, m_depthBuffer(0)
			, m_positionBuffer(0)
			, m_normalBuffer(0)
			, m_albedoWithSpecularBuffer(0)
			, m_gBufferFBO(0)
		{
			Init();
		}


		DeferredRenderer::~DeferredRenderer()
		{
			DestroyFramebuffer();
		}

		void DeferredRenderer::Init()
		{
			// Create all empty images and attach them to gBuffer Framebuffer Object

			glGenFramebuffers(1, &m_gBufferFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFBO);

			// Depth texture
			glGenTextures(1, &m_depthBuffer);
			glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_windowWidth, m_windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);

			// Position texture
			glGenTextures(1, &m_positionBuffer);
			glBindTexture(GL_TEXTURE_2D, m_positionBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_windowWidth, m_windowHeight, 0, GL_RGB, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positionBuffer, 0);

			// Normal texture
			glGenTextures(1, &m_normalBuffer);
			glBindTexture(GL_TEXTURE_2D, m_normalBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_windowWidth, m_windowHeight, 0, GL_RGB, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalBuffer, 0);
			
			// Albedo + Specular component texture
			glGenTextures(1, &m_albedoWithSpecularBuffer);
			glBindTexture(GL_TEXTURE_2D, m_albedoWithSpecularBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_windowWidth, m_windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_albedoWithSpecularBuffer, 0);

         if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void DeferredRenderer::PrepareDR()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_gBufferFBO);
			uint32_t attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

         glViewport(0, 0, m_windowWidth, m_windowHeight);
         glClearColor(0, 0, 0, 0);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void DeferredRenderer::StopDR()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void DeferredRenderer::DestroyFramebuffer()
		{
			StopDR();
			glDeleteFramebuffers(1, &m_gBufferFBO);
			glDeleteTextures(1, &m_depthBuffer);
			glDeleteTextures(1, &m_positionBuffer);
			glDeleteTextures(1, &m_normalBuffer);
			glDeleteTextures(1, &m_albedoWithSpecularBuffer);
		}

		void DeferredRenderer::BindDepthTexture(int32_t slot) {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
		}

		void DeferredRenderer::BindPositionTexture(int32_t slot) {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_positionBuffer);
		}

		void DeferredRenderer::BindNormalTexture(int32_t slot) {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_normalBuffer);
		}

		void DeferredRenderer::BindAlbedoWithSpecularTexture(int32_t slot) {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_albedoWithSpecularBuffer);
		}

	}
}