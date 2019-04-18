#pragma once

#include <vector>
#include <glm/vec3.hpp>

namespace Event
{
   template <typename DataType>
   class TEvent
   {
   public:

      using Event_t = TEvent<DataType>;
      using EventData_t = DataType;

   private:

      static Event_t* m_instance;

      std::vector<TEvent<DataType>*> m_listeners;

   protected:

      TEvent();

   public:

      virtual ~TEvent();

      void SendEvent(const DataType& data) const;

      void AddListener(TEvent<DataType>* eventListener);

      void RemoveListener(TEvent<DataType>* eventListener);

      virtual void ProcessEvent(const DataType& data);

      static Event_t* GetInstance() {

         if (!m_instance)
            m_instance = new Event_t();

         return m_instance;
      }

   };

   typedef TEvent<glm::vec3> CameraMovedEvent;
}

