#pragma once

#include "Event.h"

#include <iostream>

class EventPackageBase
{
public:
	void exec(Event* EventInfo) {
		call(EventInfo);
	}

private:
	virtual void call(Event* EventInfo) = 0;
};


template<typename T, typename EventStruct>
class EventPackage : public EventPackageBase
{
public:
	typedef void(T::*CallbackFunc)(EventStruct*);

	EventPackage(T* instance, CallbackFunc memberFunction) : m_SubscriberInstance{ instance }, m_Function{ memberFunction } {}

	virtual void call(Event* EventInfo) override
	{
		(m_SubscriberInstance->*m_Function)(static_cast<EventStruct*>(EventInfo));
	}

	inline const T* GetInstance() const { return m_SubscriberInstance; }

private:
	T* m_SubscriberInstance;
	CallbackFunc m_Function;
};

