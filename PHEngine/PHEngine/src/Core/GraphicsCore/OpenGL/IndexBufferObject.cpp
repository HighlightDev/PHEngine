#include "IndexBufferObject.h"

#include <GL/glew.h>

namespace Graphics
{
	namespace OpenGL
	{
		IndexBufferObject::IndexBufferObject(std::shared_ptr<std::vector<uint32_t>> data, DataCarryFlag dataCarryFlag)
			: VertexBufferObjectBase(GL_ELEMENT_ARRAY_BUFFER)
			, m_data(data)
			, m_dataCarryFlag(dataCarryFlag)
		{
		}

		IndexBufferObject::~IndexBufferObject()
		{
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
			size_t bufferSize = sizeof(size_t) * GetTotalLengthOfData();
			glBufferData(m_bufferTarget, bufferSize, m_data->data(), GL_STATIC_DRAW);

			if (m_dataCarryFlag == DataCarryFlag::Invalidate)
			{
				m_data.~shared_ptr();
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

		size_t IndexBufferObject::GetCountOfIndices() const
		{
			return m_data->size();
		}

		size_t IndexBufferObject::GetVectorSize() const
		{
			return 1;
		}

		size_t IndexBufferObject::GetTotalLengthOfData() const
		{
			return m_data->size();
		}

		size_t IndexBufferObject::GetVertexAttribIndex() const 
		{
			return (size_t)-1;
		}
	}
}
