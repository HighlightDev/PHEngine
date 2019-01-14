#include "SkeletonBoneLOADER.h"

namespace MeshLoader
{
	namespace Assimp
	{

		/********* SkeletonBoneBaseLOADER ***************/
		SkeletonBoneBaseLOADER::SkeletonBoneBaseLOADER()
		{

		}

		SkeletonBoneBaseLOADER::~SkeletonBoneBaseLOADER()
		{

		}

		void SkeletonBoneBaseLOADER::AddChildBone(SkeletonBoneLOADER& child)
		{
			m_children.push_back(child);
		}

		std::vector<SkeletonBoneLOADER>& SkeletonBoneBaseLOADER::GetChildren()
		{
			return m_children;
		}

		int32_t SkeletonBoneBaseLOADER::GetIdByBoneInHierarchy(aiBone& seekBone)
		{
			int32_t id = -1;

			for (auto& child : m_children)
			{
				id = child.GetIdByBone(seekBone, child);
				if (id >= 0)
					break;
			}

			return id;
		}

		int32_t SkeletonBoneBaseLOADER::GetIdByBone(aiBone& seekBone, SkeletonBoneLOADER& currentSkeletonBone)
		{
			int32_t id = -1;

			if (currentSkeletonBone.GetBoneInfo().mName == seekBone.mName)
			{
				id = currentSkeletonBone.GetBoneId();
			}

			if (id < 0)
			{
				for (auto& skeletonBone : currentSkeletonBone.GetChildren())
				{
					id = skeletonBone.GetIdByBone(seekBone, skeletonBone);
					if (id > 0)
						return id;
				}
			}

			return id;
		}

		/********* SkeletonBoneLOADER ***************/

		SkeletonBoneLOADER::SkeletonBoneLOADER(SkeletonBoneBaseLOADER* const parent)
		{
			m_parent = parent;
		}


		SkeletonBoneLOADER::~SkeletonBoneLOADER()
		{
		}

		void SkeletonBoneLOADER::SetBoneInfo(aiBone& bone)
		{
			m_boneInfo = bone;
		}

		void SkeletonBoneLOADER::SetBoneId(int32_t id)
		{
			m_boneId = id;
		}

		aiBone& SkeletonBoneLOADER::GetBoneInfo()
		{
			return m_boneInfo;
		}

		int32_t SkeletonBoneLOADER::GetBoneId()
		{
			return m_boneId;
		}

		SkeletonBoneBaseLOADER* SkeletonBoneLOADER::GetParent()
		{
			return m_parent;
		}

		void SkeletonBoneLOADER::CleanUp()
		{
			for (auto& child : m_children)
			{
				child.CleanUp();
			}
			m_children.clear();
		}

	}
}