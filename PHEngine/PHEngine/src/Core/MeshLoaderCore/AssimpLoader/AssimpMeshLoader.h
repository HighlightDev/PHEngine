#pragma once

#include "MeshVertexData.h"

namespace MeshLoader
{
	namespace Assimp
	{
		template <int32_t count_bones_influence_vertex>
		class AssimpMeshLoader
		{
			const struct aiScene* m_scene;
			MeshVertexData<count_bones_influence_vertex>* m_meshData;

		public:

			AssimpMeshLoader(std::string& modelFilePath);

			~AssimpMeshLoader();

			bool GetHasAnimationData() const;
			
			MeshVertexData<count_bones_influence_vertex>& GetMeshData();

			/*MeshAnimationData GetAnimationData()
			{
				if (m_animationData == null)
					m_animationData = new MeshAnimationData(m_scene);

				return m_animationData;
			}*/
		};
	}
}

