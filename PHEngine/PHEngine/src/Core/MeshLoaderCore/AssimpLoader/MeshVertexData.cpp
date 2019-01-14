#include "MeshVertexData.h"
#include "SkeletonBoneLOADER.h"

namespace MeshLoader
{
	namespace Assimp
	{

		template struct MeshVertexData<3>;

		template <int32_t count_bones_influence_vertex>
		MeshVertexData<count_bones_influence_vertex>::MeshVertexData(aiScene& scene) 
			: m_scene(scene)
			, m_meshes(m_scene.mMeshes)
		{
		}

		template <int32_t count_bones_influence_vertex>
		MeshVertexData<count_bones_influence_vertex>::~MeshVertexData()
		{
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::GetMeshVertexData()
		{

		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::LoadSkin() {

		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::LoadSkeleton() {

		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::FillHierarchyRecursive(aiNode& parentNode, SkeletonBoneLOADER& parentBone, int32_t& boneIdCounter) {

		}

		template <int32_t count_bones_influence_vertex>
		std::string MeshVertexData<count_bones_influence_vertex>::GetSkeletonArmatureNodeName(aiNode& rootNode) {

			return "";
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::IterateHierarchy(aiNode& parentNode, int32_t& countChildren) {

		}

		template <int32_t count_bones_influence_vertex>
		aiBone& MeshVertexData<count_bones_influence_vertex>::GetBoneByName(std::string name) {

			return m_meshes[0][0].mBones[0][0];
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::CollectIndices(std::vector<uint32_t>& indices, aiMesh& meshBeingCollected, uint32_t lastIndexBeenInterrupted) {

		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::TryToCollectSkinInfo(int32_t startIndex, float*& vertices, float*& normals, float*& texCoords,
			float*& tangents, float*& bitangents, aiMesh& meshBeingCollected) {

		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::CollectBlendables(int32_t vertexId, std::vector<VertexLOADER>& blendData, aiMesh& meshBeingCollected, std::vector<uint32_t>& countOfIndicesPerMesh) {

		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::CleanUp()
		{

		}
	}
}