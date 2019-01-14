#include "VertexArrayObject.h"

#include <xutility>

namespace Graphics
{
	namespace OpenGL
	{
		VertexArrayObject::VertexArrayObject()
		{
			GenVAO();
		}


		VertexArrayObject::~VertexArrayObject()
		{
		}

		bool VertexArrayObject::HasIBO() const
		{
			return m_ibo != nullptr;
		}

		std::vector<std::unique_ptr<VertexBufferObjectBase>>& VertexArrayObject::GetVertexBufferArray()
		{
			return m_vbos;
		}

		void VertexArrayObject::GenVAO()
		{
			glGenVertexArrays(1, &m_descriptor);
		}

		void VertexArrayObject::RenderVAO(int32_t privitiveMode)
		{
			glBindVertexArray(m_descriptor);
			if (HasIBO())
			{
				glDrawElements(privitiveMode, m_ibo->GetIndicesCount(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glDrawArrays(privitiveMode, 0, m_vbos.front()->GetCountOfIndices());
			}
			glBindVertexArray(0);
		}

		void VertexArrayObject::AddIndexBuffer(std::unique_ptr<IndexBufferObject>&& ibo)
		{
			if (ibo != nullptr)
				m_ibo = std::move(ibo);
		}

		void VertexArrayObject::BindBuffersToVao()
		{
			glBindVertexArray(m_descriptor);
			if (m_ibo != nullptr)
				m_ibo->SendDataToGPU();

			for (auto it = m_vbos.begin(); it != m_vbos.end(); ++it)
			{
				(*it)->SendDataToGPU();
			}
			glBindVertexArray(0);
			IndexBufferObject::UnbindIndexBuffer();
			DisableVertexAttribArrays();
		}

		void VertexArrayObject::DisableVertexAttribArrays()
		{
			VertexBufferObjectBase::UnbindAttribBuffer();

			for (auto it = m_vbos.begin(); it != m_vbos.end(); ++it)
			{
				glDisableVertexAttribArray((*it)->GetVertexAttribIndex());
			}
		}

		void VertexArrayObject::CleanUp()
		{
			if (m_ibo != nullptr) m_ibo->CleanUp();

			for (auto it = m_vbos.begin(); it != m_vbos.end(); ++it)
			{
				(*it)->CleanUp();
			}

			glDeleteVertexArrays(1, &m_descriptor);
		}
	}
}
