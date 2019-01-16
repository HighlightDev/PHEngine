#pragma once
#include "VertexBufferObjectBase.h"
#include "DataCarryFlag.h"

namespace Graphics
{
	namespace OpenGL
	{
		class IndexBufferObject :
			public VertexBufferObjectBase
		{
		private:

			uint32_t* m_data;
			DataCarryFlag m_dataCarryFlag;
			int32_t m_indicesCount;

		public:

			IndexBufferObject(uint32_t* indicesData, int32_t indicesCount, DataCarryFlag dataCarryFlag = DataCarryFlag::Invalidate);

			virtual ~IndexBufferObject();

			void GenIndexBuffer();

			void BindIndexBuffer();

			void SendDataToGPU() override;

			static void UnbindIndexBuffer();

			void CleanUp() override;

			inline int32_t GetIndicesCount()
			{
				return m_indicesCount;
			}

			int32_t GetVertexAttribIndex() override;
		};
	}
}

