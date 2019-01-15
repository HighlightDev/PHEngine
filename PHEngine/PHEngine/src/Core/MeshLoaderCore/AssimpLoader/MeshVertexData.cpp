#include "MeshVertexData.h"
#include "SkeletonBoneLOADER.h"

#include <tuple>

namespace MeshLoader
{
	namespace Assimp
	{

		template struct MeshVertexData<3>;

		template <int32_t count_bones_influence_vertex>
		MeshVertexData<count_bones_influence_vertex>::MeshVertexData(const aiScene* scene) 
			: m_scene(scene)
			, m_meshes(m_scene->mMeshes)
			, SkeletonRoot(nullptr)
		{
			Indices = std::make_shared<std::vector<uint32_t>>(std::vector<uint32_t>());
			Verts = std::make_shared<std::vector<float>>(std::vector<float>());
			T_Verts = std::make_shared<std::vector<float>>(std::vector<float>());
			N_Verts = std::make_shared<std::vector<float>>(std::vector<float>());
			Tangent_Verts = std::make_shared<std::vector<float>>(std::vector<float>());
			Bitanget_Verts = std::make_shared<std::vector<float>>(std::vector<float>());
			BlendWeights = std::make_shared<std::vector<float>>(std::vector<float>());
			BlendIndices = std::make_shared<std::vector<int32_t>>(std::vector<int32_t>());
		}

		template <int32_t count_bones_influence_vertex>
		MeshVertexData<count_bones_influence_vertex>::~MeshVertexData()
		{
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::GetMeshVertexData()
		{
			size_t meshCount = m_scene->mNumMeshes;

			for (size_t currentMeshIndex = size_t(0); currentMeshIndex < meshCount; currentMeshIndex++)
			{
				if (!bHasIndices)
					m_meshes[currentMeshIndex]->HasFaces();
				if (!bHasNormals)
					m_meshes[currentMeshIndex]->HasNormals();
				if (!bHasTextureCoordinates)
					m_meshes[currentMeshIndex]->HasTextureCoords(0);

				// If we have all necessary info already -> stop passing through array
				if (bHasIndices && bHasNormals && bHasTextureCoordinates)
					break;
			}

			bHasAnimation = m_scene->HasAnimations();

			LoadSkeleton();
			LoadSkin();
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::LoadSkin() {

			size_t countOfVertices = 0;
			size_t meshCount = m_scene->mNumMeshes;
			for (size_t meshIndex = 0; meshIndex < meshCount; meshIndex++)
			{
				aiMesh* mesh = m_scene->mMeshes[meshIndex];
				countOfVertices += mesh->mNumVertices;
			}

			std::vector<uint32_t> countOfIndicesPerMesh;

			// Iterate through all meshes
			for (size_t i = 0; i < m_scene->mNumMeshes; i++)
			{
				aiMesh* mesh = m_meshes[i];
				size_t processedIndices = Indices->size();
				countOfIndicesPerMesh.emplace_back(processedIndices);
				CollectIndices(*mesh, processedIndices);
				TryToCollectSkinInfo(countOfIndicesPerMesh[i], *mesh);
			}

			// collect blend weights and blend ids
			if (bHasAnimation)
			{
				std::vector<VertexLOADER> blendData;
				for (size_t attributeIndex = 0; attributeIndex < countOfVertices; attributeIndex++)
				{
					for (size_t meshIndex = 0; meshIndex < m_scene->mNumMeshes; meshIndex++)
					{
						aiMesh* mesh = m_meshes[meshIndex];
						CollectBlendables(attributeIndex, blendData, *mesh, countOfIndicesPerMesh);
					}
				}

				for (size_t blendableIndex = 0; blendableIndex < blendData.size(); blendableIndex++)
				{
					VertexLOADER& blendVertex = blendData[blendableIndex];
					CollectBlendWeightsAndIndices(blendVertex, blendableIndex);
				}
			}
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::LoadSkeleton() {

			if (bHasAnimation)
			{
				SkeletonRoot = new SkeletonBoneBaseLOADER();
				int32_t boneIdCounter = 0;

				aiNode* rootNode = m_scene->mRootNode->FindNode(GetSkeletonArmatureNodeName(*m_scene->mRootNode).c_str());
				if (rootNode)
				{
					size_t nodesCount = rootNode->mNumChildren;
					for (size_t nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++)
					{
						aiNode* childNode = rootNode->mChildren[nodeIndex];
						aiBone* bone = GetBoneByName(std::string(childNode->mName.C_Str()));
						if (bone)
						{
							SkeletonBoneLOADER* skeletonBone = new SkeletonBoneLOADER(SkeletonRoot);
							skeletonBone->SetBoneId(boneIdCounter++);
							skeletonBone->SetBoneInfo(*bone);
							FillHierarchyRecursive(*childNode, skeletonBone, boneIdCounter);
							SkeletonRoot->AddChildBone(*skeletonBone);
						}
					}
				}
			}
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::FillHierarchyRecursive(aiNode& parentNode, SkeletonBoneLOADER* parentBone, int32_t& boneIdCounter) {

			size_t nodesCount = parentNode.mNumChildren;
			for (size_t nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++)
			{
				aiNode* childNode = parentNode.mChildren[nodeIndex];
				SkeletonBoneLOADER* childBone = new SkeletonBoneLOADER(parentBone);
				parentBone->AddChildBone(*childBone);
				aiBone* boneInfo = GetBoneByName(std::string(childNode->mName.C_Str()));
				childBone->SetBoneInfo(*boneInfo);
				childBone->SetBoneId(boneIdCounter++);
				FillHierarchyRecursive(*childNode, childBone, boneIdCounter);
			}
		}

		template <int32_t count_bones_influence_vertex>
		std::string MeshVertexData<count_bones_influence_vertex>::GetSkeletonArmatureNodeName(aiNode& rootNode) {

			std::string name;

			int32_t maxHierarchySize = 0;
			size_t nodesCount = rootNode.mNumChildren;
			for (size_t nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++)
			{
				aiNode* childNode = rootNode.mChildren[nodeIndex];
				int32_t currentChildHierarchySize = 0;
				IterateHierarchy(*childNode, currentChildHierarchySize);
				if (currentChildHierarchySize > maxHierarchySize)
				{
					maxHierarchySize = currentChildHierarchySize;
					name = std::move(std::string(childNode->mName.C_Str()));
				}
			}

			return name;
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::IterateHierarchy(aiNode& parentNode, int32_t& countChildren) {

			size_t nodesCount = parentNode.mNumChildren;
			for (size_t nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++)
			{
				aiNode& childNode = *parentNode.mChildren[nodeIndex];
				++countChildren;
				IterateHierarchy(childNode, countChildren);
			}
		}

		template <int32_t count_bones_influence_vertex>
		aiBone* MeshVertexData<count_bones_influence_vertex>::GetBoneByName(const std::string& name) {

			aiBone* result = nullptr;

			size_t meshCount = m_scene->mNumMeshes;

			for (size_t currentMeshIndex = size_t(0); (currentMeshIndex < meshCount) && (result == nullptr); currentMeshIndex++)
			{
				aiMesh* mesh = m_meshes[currentMeshIndex];
				size_t bonesCount = mesh->mNumBones;
				for (size_t boneIndex = size_t(0); boneIndex < bonesCount; boneIndex++)
				{
					aiBone* bone = mesh->mBones[boneIndex];
					if (std::string(bone->mName.C_Str()) == name)
					{
						result = bone;
						break;
					}
				}
			}
			return result;
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::CollectIndices(aiMesh& meshBeingCollected, uint32_t lastIndexBeenInterrupted) {

			size_t countOfFaces = meshBeingCollected.mNumFaces;
			for (size_t faceIndex = 0; faceIndex < countOfFaces; faceIndex++)
			{
				aiFace& face = meshBeingCollected.mFaces[faceIndex];
				if (face.mNumIndices == 3) // triangulated face
				{
					Indices->emplace_back(face.mIndices[0] + lastIndexBeenInterrupted);
					Indices->emplace_back(face.mIndices[1] + lastIndexBeenInterrupted);
					Indices->emplace_back(face.mIndices[2] + lastIndexBeenInterrupted);
				}
				else
				{
					throw std::invalid_argument("Face isn't triangulated.");
				}
			}
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::TryToCollectSkinInfo(size_t startIndex, aiMesh& meshBeingCollected) {

			for (size_t attribIndex = 0; attribIndex < meshBeingCollected.mNumVertices; ++attribIndex)
			{
				Verts->emplace_back(meshBeingCollected.mVertices[attribIndex].x);
				Verts->emplace_back(meshBeingCollected.mVertices[attribIndex].y);
				Verts->emplace_back(meshBeingCollected.mVertices[attribIndex].z);

				if (bHasNormals)
				{
					N_Verts->emplace_back(meshBeingCollected.mNormals[attribIndex].x);
					N_Verts->emplace_back(meshBeingCollected.mNormals[attribIndex].y);
					N_Verts->emplace_back(meshBeingCollected.mNormals[attribIndex].z);
				}
				if (bHasTextureCoordinates)
				{
					T_Verts->emplace_back(meshBeingCollected.mTextureCoords[0][attribIndex].x);
					T_Verts->emplace_back(meshBeingCollected.mTextureCoords[0][attribIndex].y);
				}
				if (bHasTangentVertices)
				{
					Tangent_Verts->emplace_back(meshBeingCollected.mTangents[attribIndex].x);
					Tangent_Verts->emplace_back(meshBeingCollected.mTangents[attribIndex].y);
					Tangent_Verts->emplace_back(meshBeingCollected.mTangents[attribIndex].z);

					Bitanget_Verts->emplace_back(meshBeingCollected.mBitangents[attribIndex].x);
					Bitanget_Verts->emplace_back(meshBeingCollected.mBitangents[attribIndex].y);
					Bitanget_Verts->emplace_back(meshBeingCollected.mBitangents[attribIndex].z);
				}
			}
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::CollectBlendables(int32_t vertexId, std::vector<VertexLOADER>& blendData, aiMesh& meshBeingCollected, std::vector<uint32_t>& countOfIndicesPerMesh) {

			VertexLOADER vertex(vertexId);
			blendData.push_back(vertex);

			aiBone** bonesInMesh = meshBeingCollected.mBones;
			size_t bonesCount = meshBeingCollected.mNumBones;
		
			for (size_t boneIndex = 0; boneIndex < bonesCount; boneIndex++)
			{
				aiBone* bone = bonesInMesh[boneIndex];
				size_t weightsCount = bone->mNumWeights;

				for (size_t weightIndex = 0; weightIndex < weightsCount; weightIndex++)
				{
					aiVertexWeight& weight = bone->mWeights[weightIndex];
					int32_t i = 0;
					while (&meshBeingCollected != m_meshes[i])
					{
						i++;
					}
					if (weight.mVertexId == (vertexId + countOfIndicesPerMesh[i]))
					{
						int32_t boneId = SkeletonRoot->GetIdByBoneInHierarchy(*bone);
						if (boneId >= 0)
						{
							std::tuple<aiBone&, int32_t> weightTuple = std::tuple<aiBone&, int32_t>(*bone, boneId);
							//std::pair<std::tuple<aiBone&, int32_t>, float> weightPair = std::make_pair<std::tuple<aiBone&, int32_t>, float>(weightTuple, weight.mWeight);
							//vertex.AddBoneWeight(std::move(weightPair));
						}
						else
						{
							throw std::invalid_argument("Such bone doesn't exist in skeleton!");
						}
					}
				}
			}

			if (vertex.BoneWeightMap.size() == 0)
				blendData.erase(blendData.end() - 1); // remove last inserted element
		}

		template <>
		void MeshVertexData<3>::CollectBlendWeightsAndIndices(VertexLOADER& blendInfoVertex, size_t blendableIndex)
		{
			size_t currentVertexInfluenceCount = blendInfoVertex.BoneWeightMap.size();

			if (currentVertexInfluenceCount == 1)
			{
				BlendWeights->emplace_back(blendInfoVertex.BoneWeightMap[0].second);
				BlendIndices->emplace_back(std::get<int32_t>(blendInfoVertex.BoneWeightMap[0].first));

				BlendWeights->emplace_back(0.0f);
				BlendIndices->emplace_back(-1);  // this provides assurance that skin matrix will not do anything in shader.

				BlendWeights->emplace_back(0.0f);
				BlendIndices->emplace_back(-1);  // this provides assurance that skin matrix will not do anything in shader.
			}
			else if (currentVertexInfluenceCount == 2)
			{
				BlendWeights->emplace_back(blendInfoVertex.BoneWeightMap[0].second);
				BlendIndices->emplace_back(std::get<int32_t>(blendInfoVertex.BoneWeightMap[0].first));

				BlendWeights->emplace_back(blendInfoVertex.BoneWeightMap[1].second);
				BlendIndices->emplace_back(std::get<int32_t>(blendInfoVertex.BoneWeightMap[1].first));

				BlendWeights->emplace_back(0.0f);
				BlendIndices->emplace_back(-1);  // this provides assurance that skin matrix will not do anything in shader.
			}
			else if (currentVertexInfluenceCount > 2)
			{
				BlendWeights->emplace_back(blendInfoVertex.BoneWeightMap[0].second);
				BlendIndices->emplace_back(std::get<int32_t>(blendInfoVertex.BoneWeightMap[0].first));

				BlendWeights->emplace_back(blendInfoVertex.BoneWeightMap[1].second);
				BlendIndices->emplace_back(std::get<int32_t>(blendInfoVertex.BoneWeightMap[1].first));

				BlendWeights->emplace_back(blendInfoVertex.BoneWeightMap[2].second);
				BlendIndices->emplace_back(std::get<int32_t>(blendInfoVertex.BoneWeightMap[2].first));
			}
		}

		template <int32_t count_bones_influence_vertex>
		void MeshVertexData<count_bones_influence_vertex>::CleanUp()
		{

		}
	}
}