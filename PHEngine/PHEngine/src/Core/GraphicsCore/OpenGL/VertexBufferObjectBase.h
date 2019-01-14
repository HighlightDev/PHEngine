#pragma once
#include <stdint.h>
#include <gl/glew.h>

namespace Graphics
{
	namespace OpenGL
	{
		class VertexBufferObjectBase
		{
		protected:

			uint32_t m_descriptor;
			int32_t m_bufferTarget;

		public:
			VertexBufferObjectBase(int32_t bufferTarget = GL_ARRAY_BUFFER);
			virtual ~VertexBufferObjectBase();

			void GenBuffer();

			void BindVBO();

			static void UnbindAttribBuffer();

			virtual void SendDataToGPU() = 0;

			virtual int32_t GetCountOfIndices() const = 0;

			virtual int32_t GetTotalLengthOfData() const = 0;

			virtual int32_t GetVectorSize() const = 0;

			virtual int32_t GetVertexAttribIndex() = 0;

			virtual void CleanUp() = 0;
		};
	}
}

