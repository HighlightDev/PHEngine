#pragma once
#include <stdint.h>
#include <GL/glew.h>

#include "DataCarryFlag.h"
#include "VertexBufferObjectBase.h"

namespace Graphics
{
	namespace OpenGL
	{
		template <typename DataType, int32_t vector_size, int32_t gl_type = GL_FLOAT>
		class VertexBufferObject : public VertexBufferObjectBase
		{
		private:

			static constexpr int32_t m_vectorSize = vector_size;
			static constexpr int32_t m_glType = gl_type;

			static void SetVertexAttribPointer(int32_t index, int32_t size, bool normalized, int32_t stride, int32_t pointer_offset)
			{
				if (m_glType == GL_INT ||
					m_glType == GL_UNSIGNED_BYTE ||
					m_glType == GL_UNSIGNED_INT)
				{
					glVertexAttribIPointer(index, size, m_glType, stride, (void*)(pointer_offset));
				}
				else if (m_glType == GL_DOUBLE)
				{
					glVertexAttribLPointer(index, size, m_glType, stride, (void*)(pointer_offset));
				}
				else if (m_glType == GL_FLOAT)
				{
					glVertexAttribPointer(index, size, m_glType, normalized, stride, (void*)(pointer_offset));
				}
			}

			static size_t GetVectorElementByteSize()
			{
				return sizeof(DataType);
			}

			// For current implementation pointer offset is zero
			virtual void SetVertexAttribPointerWithSpecificParams()
			{
				size_t stride = GetVectorElementByteSize() * m_vectorSize;
				SetVertexAttribPointer(m_vertexAttribIndex, m_vectorSize, false, stride, 0);
			}

		protected:

			DataType* m_data;
			int32_t m_totalDataLength;
			int32_t m_countOfIndices;
			int32_t m_vertexAttribIndex;
			DataCarryFlag m_dataCarryFlag;

		public:

			VertexBufferObject(DataType* data, int32_t totalDataLength, int32_t bufferTarget, int32_t vertexAttribIndex, DataCarryFlag flag)
				: VertexBufferObjectBase(bufferTarget)
				, m_data(data)
				, m_totalDataLength(totalDataLength)
				, m_countOfIndices(totalDataLength / m_vectorSize)
				, m_vertexAttribIndex(vertexAttribIndex)
				, m_dataCarryFlag(flag)
			{
				m_bufferTarget = bufferTarget;
				GenBuffer();
			}

			~VertexBufferObject()
			{
				delete m_data;
				m_data = nullptr;
			}

			DataType* GetData() const
			{
				return m_data;
			}

			virtual void SendDataToGPU() override
			{
				const size_t data_size = GetVectorElementByteSize() * m_totalDataLength;

				BindVBO();
				glBufferData(m_bufferTarget, data_size, m_data, GL_STATIC_DRAW);
				glEnableVertexAttribArray(m_vertexAttribIndex);
				this->SetVertexAttribPointerWithSpecificParams();
				glDisableVertexAttribArray(m_vertexAttribIndex);
				VertexBufferObjectBase::UnbindAttribBuffer();

				// If data on CPU is unnecessary - delete it
				if (m_dataCarryFlag == DataCarryFlag::Invalidate)
				{
					delete m_data;
					m_data = nullptr;
				}
			}

			virtual int32_t GetCountOfIndices() const override
			{
				return m_countOfIndices;
			}

			virtual int32_t GetTotalLengthOfData() const override
			{
				return m_totalDataLength;
			}

			virtual int32_t GetVectorSize() const override
			{
				return m_vectorSize;
			}

			virtual int32_t GetVertexAttribIndex() override
			{
				return m_vertexAttribIndex;
			}

			virtual void CleanUp() override
			{
				glDeleteBuffers(1, &m_descriptor);
			}
		};
	}
}


