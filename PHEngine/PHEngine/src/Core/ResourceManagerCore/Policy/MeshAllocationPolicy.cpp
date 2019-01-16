#include "MeshAllocationPolicy.h"
#include "Core/MeshLoaderCore/AssimpLoader/AssimpMeshLoader.h"
#include "Core/MeshLoaderCore/AssimpLoader/MeshVertexData.h"
#include "Core/MeshLoaderCore/AssimpLoader/MeshAnimationData.h"
#include "Core/GraphicsCore/OpenGL/IndexBufferObject.h"
#include "Core/GraphicsCore/OpenGL/VertexBufferObject.h"
#include "Core/UtilityCore/PlatformDependentFunctions.h"

#include <gl/glew.h>

using namespace MeshLoader::Assimp;

namespace Resources
{

	MeshAllocationPolicy::MeshAllocationPolicy()
	{
	}


	MeshAllocationPolicy::~MeshAllocationPolicy()
	{
	}

	std::shared_ptr<VertexArrayObject> MeshAllocationPolicy::AllocateMemory(std::string& arg)
	{
		static const int32_t countOfBonesInfluencingOnVertex = 3;

		std::shared_ptr<VertexArrayObject> vao;

		{
			std::string absolutePath = std::move(EngineUtility::ConvertFromRelativeToAbsolutePath(arg));
			MeshLoader::Assimp::AssimpMeshLoader<countOfBonesInfluencingOnVertex> loader(absolutePath);

			vao = std::make_shared<VertexArrayObject>();

			MeshVertexData<countOfBonesInfluencingOnVertex>& meshData = loader.GetMeshData();

			// explicit null assignment if there is no some of mesh data
			std::shared_ptr<std::vector<float>> vertices = meshData.Verts;
			std::shared_ptr<std::vector<float>> normals = meshData.N_Verts;
			std::shared_ptr<std::vector<float>> texCoords = meshData.T_Verts;
			std::shared_ptr<std::vector<float>> tangents = meshData.Tangent_Verts;
			std::shared_ptr<std::vector<float>> bitangents = meshData.Bitanget_Verts;
			std::shared_ptr<std::vector<float>> blendWeights = meshData.BlendWeights;
			std::shared_ptr<std::vector<int32_t>> blendIndices = meshData.BlendIndices;
			std::shared_ptr<std::vector<uint32_t>> indices = meshData.Indices;

			IndexBufferObject* ibo = nullptr;

			VertexBufferObjectBase* vertexVBO, *normalsVBO = nullptr, *texCoordsVBO = nullptr, *tangentsVBO = nullptr, *bitangentsVBO = nullptr, *blendWeightsVBO = nullptr, *blendIndicesVBO = nullptr;

			if (meshData.bHasIndices)
				ibo = new IndexBufferObject(indices);

			vertexVBO = new VertexBufferObject<float, 3, GL_FLOAT>(vertices, GL_ARRAY_BUFFER, 0, DataCarryFlag::Invalidate);

			if (meshData.bHasNormals)
				normalsVBO = new VertexBufferObject<float, 3, GL_FLOAT>(normals, GL_ARRAY_BUFFER, 1, DataCarryFlag::Invalidate);
			if (meshData.bHasTextureCoordinates)
				texCoordsVBO = new VertexBufferObject<float, 2, GL_FLOAT>(texCoords, GL_ARRAY_BUFFER, 2, DataCarryFlag::Invalidate);
			if (meshData.bHasTangentVertices)
				tangentsVBO = new VertexBufferObject<float, 3, GL_FLOAT>(tangents, GL_ARRAY_BUFFER, 4, DataCarryFlag::Invalidate);
			if (meshData.bHasTangentVertices)
				bitangentsVBO = new VertexBufferObject<float, 3, GL_FLOAT>(bitangents, GL_ARRAY_BUFFER, 5, DataCarryFlag::Invalidate);
			if (meshData.bHasAnimation)
			{
				blendWeightsVBO = new VertexBufferObject<float, countOfBonesInfluencingOnVertex, GL_FLOAT>(blendWeights, GL_ARRAY_BUFFER, 6, DataCarryFlag::Invalidate);
				blendIndicesVBO = new VertexBufferObject<int32_t, countOfBonesInfluencingOnVertex, GL_FLOAT>(blendIndices, GL_ARRAY_BUFFER, 7, DataCarryFlag::Invalidate);
			}

			vao->AddVBO(std::unique_ptr<VertexBufferObjectBase>(vertexVBO),
				std::unique_ptr<VertexBufferObjectBase>(normalsVBO),
				std::unique_ptr<VertexBufferObjectBase>(texCoordsVBO),
				std::unique_ptr<VertexBufferObjectBase>(tangentsVBO),
				std::unique_ptr<VertexBufferObjectBase>(bitangentsVBO),
				std::unique_ptr<VertexBufferObjectBase>(blendWeightsVBO),
				std::unique_ptr<VertexBufferObjectBase>(blendIndicesVBO));

			vao->AddIndexBuffer(std::unique_ptr<IndexBufferObject>(ibo));
			vao->BindBuffersToVao();

			/*if (meshData.bHasAnimation)
			{
				ParentBone rootBone = AssimpConverter.Converter.ConvertAssimpBoneToEngineBone(meshData.SkeletonRoot);
				resultSkin = new AnimatedSkin(vao, rootBone);
			}
			else
			{
				resultSkin = new Skin(vao);
			}*/

		}

		return vao;
	}

	void MeshAllocationPolicy::DeallocateMemory(std::shared_ptr<VertexArrayObject> arg)
	{
		arg->CleanUp();
	}

}
