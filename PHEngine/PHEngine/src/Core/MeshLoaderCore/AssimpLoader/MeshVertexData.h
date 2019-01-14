#pragma once

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <vector>
#include <string>

#include "VertexLOADER.h"

namespace MeshLoader
{
	namespace Assimp
	{

		template <int32_t count_bones_influence_vertex>
		struct MeshVertexData
		{
		private:

			aiScene& m_scene;
			aiMesh** m_meshes;

			void GetMeshVertexData();

			void LoadSkin();

			void LoadSkeleton();

			void FillHierarchyRecursive(aiNode& parentNode, class SkeletonBoneLOADER& parentBone, int32_t& boneIdCounter);

			std::string GetSkeletonArmatureNodeName(aiNode& rootNode);

			void IterateHierarchy(aiNode& parentNode, int32_t& countChildren);

			aiBone& GetBoneByName(std::string name);

			void CollectIndices(std::vector<uint32_t>& indices, aiMesh& meshBeingCollected, uint32_t lastIndexBeenInterrupted);

			void TryToCollectSkinInfo(int32_t startIndex, float*& vertices, float*& normals, float*& texCoords,
				float*& tangents, float*& bitangents, aiMesh& meshBeingCollected);

			void CollectBlendables(int32_t vertexId, std::vector<VertexLOADER>& blendData, aiMesh& meshBeingCollected, std::vector<uint32_t>& countOfIndicesPerMesh);

		public:

			MeshVertexData(aiScene& scene);
			~MeshVertexData();

			bool bHasIndices = false;
			bool bHasNormals = false;
			bool bHasTextureCoordinates = false;
			bool bHasTangentVertices = false;
			bool bHasAnimation = false;

			uint32_t* Indices;
			uint32_t indicesCount;

			float* Verts;
			uint32_t verticesCount;

			float* T_Verts;
			uint32_t texCoordsCount;

			float* N_Verts;
			uint32_t normalsCount;

			float* Tangent_Verts;
			uint32_t tangentsCount;

			float* Bitanget_Verts;
			uint32_t bitangentsCount;

			float* BlendWeights;
			uint32_t blendWeightsCount;

			int32_t* BlendIndices;
			uint32_t blendIndicesCount;

			class SkeletonBoneBaseLOADER* SkeletonRoot;

			void CleanUp();

		};
	}
}

