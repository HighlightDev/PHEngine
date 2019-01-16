#include "IndexBufferObject.h"

#include <GL/glew.h>

namespace Graphics
{
	namespace OpenGL
	{
		IndexBufferObject::IndexBufferObject(uint32_t* indicesData, int32_t indicesCount, DataCarryFlag dataCarryFlag)
			: VertexBufferObjectBase(GL_ELEMENT_ARRAY_BUFFER)
		{
			m_data = indicesData;
			m_dataCarryFlag = dataCarryFlag;
			m_indicesCount = indicesCount;
		}

		IndexBufferObject::~IndexBufferObject()
		{
			delete m_data;
			m_data = nullptr;
		}

		void IndexBufferObject::GenIndexBuffer()
		{
			GenBuffer();
		}

		void IndexBufferObject::BindIndexBuffer()
		{
			glBindBuffer(m_bufferTarget, m_descriptor);
		}

		void IndexBufferObject::SendDataToGPU()
		{
			BindIndexBuffer();
			size_t bufferSize = sizeof(uint32_t) * m_indicesCount;
			glBufferData(m_bufferTarget, bufferSize, m_data, GL_STATIC_DRAW);

			if (m_dataCarryFlag == DataCarryFlag::Invalidate)
			{
				delete m_data;
				m_data = nullptr;
			}
		}

		void IndexBufferObject::UnbindIndexBuffer()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void IndexBufferObject::CleanUp()
		{
			glDeleteBuffers(1, &m_descriptor);
		}

		int32_t IndexBufferObject::GetVertexAttribIndex()
		{
			return 0;
		}
	}
}
