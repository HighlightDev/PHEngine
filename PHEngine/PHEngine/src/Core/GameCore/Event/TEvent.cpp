#include "TEvent.h"

namespace Event
{

   template class TEvent<glm::vec3>;

   template <typename DataType>
   TEvent<DataType>* TEvent<DataType>::m_instance = nullptr;

   template <typename DataType>
   TEvent<DataType>::TEvent()
   {
   }

   template <typename DataType>
   TEvent<DataType>::~TEvent()
   {
   }

   template <typename DataType>
   void TEvent<DataType>::SendEvent(const DataType& data) const
   {
      for (auto& listener : m_listeners)
      {
         listener->ProcessEvent(data);
      }
   }

   template <typename DataType>
   void TEvent<DataType>::ProcessEvent(const DataType& data)
   {

   }

   template <typename DataType>
   void TEvent<DataType>::AddListener(TEvent<DataType>* eventListener)
   {
      m_listeners.push_back(eventListener);
   }

   template <typename DataType>
   void TEvent<DataType>::RemoveListener(TEvent<DataType>* eventListener)
   {
      auto it = std::find(m_listeners.begin(), m_listeners.end(), eventListener);
      if (it != m_listeners.end())
         m_listeners.erase(it);
   }

}