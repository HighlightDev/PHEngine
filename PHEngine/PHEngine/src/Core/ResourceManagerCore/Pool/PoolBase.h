#pragma once

#include <unordered_map>
#include <stdint.h>
#include <algorithm>
#include <memory>

namespace Resources
{

	template <typename ValueType, typename KeyType, typename AllocationPolicyType>
	class PoolBase
	{
	public:

		using value_t = ValueType;
		using key_t = KeyType;
		using policy_t = AllocationPolicyType;
		using sharedValue_t = std::shared_ptr<ValueType>;

	protected:

		std::unordered_map<KeyType, sharedValue_t> resourceMap;
		std::unordered_map<KeyType, int32_t> referenceMap;

	private:

		void IncreaseRefCounter(KeyType& key)
		{
			auto it = referenceMap.find(key);
			if (it != referenceMap.end())
			{
				++it->second;
			}
			else
			{
				referenceMap.insert(std::make_pair(key, 1));
			}
		}

		sharedValue_t GetResource(KeyType& key)
		{
			sharedValue_t value;
			auto it = resourceMap.find(key);
			if (it != resourceMap.end())
			{
				value = it->second;
			}

			return value;
		}

		KeyType GetKey(sharedValue_t value)
		{
			KeyType key;
			auto predicate = [&value](auto& keyvalue)
			{
				return (keyvalue.second == value);
			};
			auto it = std::find_if(resourceMap.begin(), resourceMap.end(), predicate);

			if (it != resourceMap.end())
				key = it->first;

			return key;
		}

		void FreeResource(KeyType& key)
		{
			auto it = referenceMap.find(key);
			if (it != referenceMap.end())
			{
				--it->second;
				if (it->second == 0)
				{
					policy_t::DeallocateMemory(resourceMap[key]);
					resourceMap.erase(key);
					referenceMap.erase(key);
				}
			}

		}

		void CleanUp()
		{
			for (auto it = resourceMap.begin(); it != resourceMap.end(); ++it)
			{
				 auto key = it->first;
				 policy_t::DeallocateMemory(resourceMap[key]);
			}

			resourceMap.clear();
			referenceMap.clear();
		}

	public:

		PoolBase() {}

		virtual ~PoolBase() 
		{
			CleanUp();
		}

		int32_t GetReferenceCount(KeyType& key)
		{
			int32_t referenceCount = 0;
			auto it = referenceMap.find(key);
			if (it != referenceMap.end())
				referenceCount = it->second;

			return referenceCount;
		}

		int32_t GetResourcesCount()
		{
			int32_t resourceCount = resourceMap.size();

			return resourceCount;
		}

		sharedValue_t GetOrAllocateResource(KeyType& key)
		{
			sharedValue_t resource = GetResource(key);
			if (!resource)
			{
				resource = policy_t::AllocateMemory(key);
				std::pair<KeyType, sharedValue_t> pair = std::make_pair(key, resource);
				resourceMap.insert(pair);
			}

			if (resource)
			{
				IncreaseRefCounter(key);
			}

			return resource;
		}

		bool TryToFreeMemory(KeyType& key)
		{
			bool bMemoryFreed = false;
			sharedValue_t resource = GetResource(key);

			if (resource)
			{
				bMemoryFreed = true;
				FreeResource(key);
			}

			return bMemoryFreed;
		}

		bool TryToFreeMemory(sharedValue_t value)
		{
			bool bMemoryFreed = false;
			KeyType key = GetKey(value);

			if (&(key) != nullptr)
			{
				bMemoryFreed = true;
				FreeResource(key);
			}

			return bMemoryFreed;
		}
	};

}

