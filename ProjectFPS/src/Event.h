// Based on https://www.codeproject.com/Articles/1256352/CppEvent-How-to-Implement-Events-using-Standard-Cp

#pragma once

template<typename... Args>
class EventHandler
{
public:
	typedef std::function<void(Args...)> HandlerFunctionType;
	typedef unsigned int HandlerIdType;

	EventHandler(const EventHandler& source)
		: m_HandlerFunction(source.m_HandlerFunction), m_HandlerId(source.m_HandlerId)
	{
	}

	EventHandler(const EventHandler&& source)
		: m_HandlerFunction(std::move(source.m_HandlerFunction), m_HandlerId(source.m_HandlerId))
	{
	}

	explicit EventHandler(const HandlerFunctionType& handlerFunction)
		: m_HandlerFunction(handlerFunction)
	{
		m_HandlerId = ++m_HandlerIdCounter;
	}

	bool operator==(const EventHandler& other) const
	{
		return m_HandlerId == other.m_HandlerId;
	}

	void operator()(Args... parameters) const
	{
		if (m_HandlerFunction != nullptr)
			m_HandlerFunction(parameters...);
	}

	EventHandler& operator=(const EventHandler& other)
	{
		m_HandlerFunction = other.m_HandlerFunction;
		m_HandlerId = other.m_HandlerId;

		return *this;
	}

	EventHandler& operator=(EventHandler&& other)
	{
		std::swap(m_HandlerFunction, other.m_HandlerFunction);
		m_HandlerId = other.m_HandlerId;

		return *this;
	}

	HandlerIdType GetId() const { return m_HandlerId; }

private:
	HandlerFunctionType m_HandlerFunction;
	HandlerIdType m_HandlerId;
	static std::atomic_uint m_HandlerIdCounter;
};

template <typename... Args>
std::atomic_uint EventHandler<Args...>::m_HandlerIdCounter(0);

template<typename... Args>
class Event
{
public:
	typedef EventHandler<Args...> HandlerType;

	Event() {}

	Event(const Event& source)
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);
		m_Handlers = source.m_Handlers;
	}

	Event(Event&& source)
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);
		m_Handlers = std::move(source.m_Handlers);
	}

	// Adding handlers
	typename HandlerType::HandlerIdType Add(const HandlerType& handler)
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);

		m_Handlers.push_back(handler);
		return handler.GetId();
	}

	inline typename HandlerType::HandlerIdType Add(const typename HandlerType::HandlerFunctionType& handler)
	{
		return Add(HandlerType(handler));
	}

	// Removing handlers
	bool Remove(const HandlerType& handler)
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);

		auto it = std::find(m_Handlers.begin(), m_Handlers.end(), handler);
		if (it != m_Handlers.end())
		{
			m_Handlers.erase(it);
			return true;
		}

		return false;
	}

	bool Remove(const typename HandlerType::HandlerIdType& handlerId)
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);

		auto it = std::find_if(m_Handlers.begin(), m_Handlers.end(),
			[handlerId](const HandlerType& handler) -> auto { handler.GetId() == handlerId; });
		if (it != m_Handlers.end())
		{
			m_Handlers.erase(it);
			return true;
		}

		return false;
	}

	void Call(Args... parameters) const
	{
		HandlerCollectionType handlersCopy = GetHandlersCopy();
		
		for (const auto& handler : handlersCopy)
		{
			handler(parameters...);
		}
	}

	Event& operator=(const Event& other)
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);
		std::lock_guard<std::mutex> lock2(other.m_HandlersLocker);

		m_Handlers = other.m_Handlers;
		return *this;
	}

	Event& operator=(const Event&& other)
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);
		std::lock_guard<std::mutex> lock2(other.m_HandlersLocker);

		std::swap(m_Handlers, other.m_Handlers);
		return *this;
	}

	inline void operator()(Args... parameters) const { Call(parameters...); }

	inline void operator+=(const HandlerType& handler) { Add(handler); }
	inline void operator-=(const HandlerType& handler) { Add(handler); }

	inline void operator+=(const typename HandlerType::HandlerFunctionType& handler) { Add(handler); }

protected:
	typedef std::list<HandlerType> HandlerCollectionType;

	HandlerCollectionType GetHandlersCopy() const
	{
		std::lock_guard<std::mutex> lock(m_HandlersLocker);
		return m_Handlers;
	}

private:
	HandlerCollectionType m_Handlers;
	mutable std::mutex m_HandlersLocker;
};