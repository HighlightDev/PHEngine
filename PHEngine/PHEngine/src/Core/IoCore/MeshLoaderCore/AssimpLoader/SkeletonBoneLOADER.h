#pragma once

#include <vector>
#include <stdint.h>
#include <assimp/mesh.h>

namespace Io
{
	namespace MeshLoader
	{
		namespace Assimp
		{
			class SkeletonBoneLOADER;

			class SkeletonBoneBaseLOADER
			{
			protected:
				std::vector<SkeletonBoneLOADER*> m_children;
			public:
				SkeletonBoneBaseLOADER();
				virtual ~SkeletonBoneBaseLOADER();

				void AddChildBone(SkeletonBoneLOADER* child);

				inline std::vector<SkeletonBoneLOADER*>& GetChildren();

				int32_t GetIdByBoneInHierarchy(aiBone* seekBone);

				int32_t GetIdByBone(aiBone* seekBone, SkeletonBoneLOADER* currentSkeletonBone);
			};

			class SkeletonBoneLOADER : public SkeletonBoneBaseLOADER
			{
			private:

				SkeletonBoneBaseLOADER* m_parent;
				aiBone* m_boneInfo;
				int32_t m_boneId;

			public:
				SkeletonBoneLOADER(SkeletonBoneBaseLOADER* parent);
				~SkeletonBoneLOADER();

				void SetBoneInfo(aiBone* bone);

				void SetBoneId(int32_t id);

				inline aiBone* GetBoneInfo() const;

				inline int32_t GetBoneId();

				inline SkeletonBoneBaseLOADER* GetParent();

				void CleanUp();
			};

		}
	}
}
