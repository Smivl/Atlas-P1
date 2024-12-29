#pragma once

#include "EventPackage.h"

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <mutex>

class EventDispatcher
{
public:

    template<typename EventStruct>
    void publish(EventStruct* eventData)
    {
        m_MutexLock.lock();

        std::shared_ptr<std::vector<std::shared_ptr<EventPackageBase>>> targets = m_Subscribers[typeid(EventStruct)];

        if (!targets)
        {
            delete eventData;
            m_MutexLock.unlock();
            return;
        }

        for (auto& target : *targets)
        {
            if (target)
            {

                target->exec(eventData);

            }
        }

        delete eventData;
        m_MutexLock.unlock();
    }

    template<typename T, typename EventStruct>
    void subscribe(T* instance, void(T::* CallbackFunction)(EventStruct*)) 
    {
        m_MutexLock.lock();

        std::shared_ptr<std::vector<std::shared_ptr<EventPackageBase>>> targets = m_Subscribers[typeid(EventStruct)];

        if (!targets) 
        {

            targets = std::make_shared<std::vector<std::shared_ptr<EventPackageBase>>> ();
            m_Subscribers[typeid(EventStruct)] = targets;

        }

        targets->push_back(std::make_shared<EventPackage<T, EventStruct>>(instance, CallbackFunction));

        m_MutexLock.unlock();
    }

    template<typename T, typename EventStruct>
    void unsubscribe(T* instance)
    {
        m_MutexLock.lock();

        std::shared_ptr<std::vector<std::shared_ptr<EventPackageBase>>> TargetEventVec = m_Subscribers[typeid(EventStruct)];

        for (auto& VectorInstance : *TargetEventVec)
        {
            if(std::static_pointer_cast<EventPackage<T, EventStruct>>(VectorInstance)->GetInstance() == instance)
            {

                std::swap(VectorInstance, TargetEventVec->back());
                TargetEventVec->pop_back();

                return;

            }
        }

        m_MutexLock.unlock();
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<std::vector<std::shared_ptr<EventPackageBase>>>> m_Subscribers;
    std::mutex m_MutexLock;

};


