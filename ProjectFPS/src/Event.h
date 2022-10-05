// Based on https://www.codeproject.com/Articles/1256352/CppEvent-How-to-Implement-Events-using-Standard-Cp

#pragma once

template<typename... Args>
class EventHandler
{
public:
	typedef std::function<void(Args...)> HandlerFunctionType;
	typedef unsigned int HandlerIdType;

	EventHandler(const EventHandler& source)
		: handlerFunction(source.handlerFunction), handlerId(source.handlerId)
	{
	}

	EventHandler(const EventHandler&& source)
		: handlerFunction(std::move(source.handlerFunction), handlerId(source.handlerId))
	{
	}

	explicit EventHandler(const HandlerFunctionType& handlerFunction)
		: handlerFunction(handlerFunction)
	{
		handlerId = ++handlerIdCounter;
	}

	bool operator==(const EventHandler& other) const
	{
		return handlerId == other.handlerId;
	}

	void operator()(Args... parameters) const
	{
		if (handlerFunction != nullptr)
			handlerFunction(parameters...);
	}

	EventHandler& operator=(const EventHandler& other)
	{
		handlerFunction = other.handlerFunction;
		handlerId = other.handlerId;

		return *this;
	}

	EventHandler& operator=(EventHandler&& other)
	{
		std::swap(handlerFunction, other.handlerFunction);
		handlerId = other.handlerId;

		return *this;
	}

	HandlerIdType GetId() const { return handlerId; }

private:
	HandlerFunctionType handlerFunction;
	HandlerIdType handlerId;
	static std::atomic_uint handlerIdCounter;
};

template <typename... Args>
std::atomic_uint EventHandler<Args...>::handlerIdCounter(0);

template<typename... Args>
class Event
{
public:
	typedef EventHandler<Args...> HandlerType;

	Event() {}

	Event(const Event& source)
	{
		std::lock_guard<std::mutex> lock(handlersLocker);
		handlers = source.handlers;
	}

	Event(Event&& source)
	{
		std::lock_guard<std::mutex> lock(handlersLocker);
		handlers = std::move(source.handlers);
	}

	// Adding handlers
	typename HandlerType::HandlerIdType Add(const HandlerType& handler)
	{
		std::lock_guard<std::mutex> lock(handlersLocker);

		handlers.push_back(handler);
		return handler.GetId();
	}

	inline typename HandlerType::HandlerIdType Add(const typename HandlerType::HandlerFunctionType& handler)
	{
		return Add(HandlerType(handler));
	}

	// Removing handlers
	bool Remove(const HandlerType& handler)
	{
		std::lock_guard<std::mutex> lock(handlersLocker);

		auto it = std::find(handlers.begin(), handlers.end(), handler);
		if (it != handlers.end())
		{
			handlers.erase(it);
			return true;
		}

		return false;
	}

	bool Remove(const typename HandlerType::HandlerIdType& handlerId)
	{
		std::lock_guard<std::mutex> lock(handlersLocker);

		auto it = std::find_if(handlers.begin(), handlers.end(),
			[handlerId](const HandlerType& handler) -> auto { handler.GetId() == handlerId; });
		if (it != handlers.end())
		{
			handlers.erase(it);
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
		std::lock_guard<std::mutex> lock(handlersLocker);
		std::lock_guard<std::mutex> lock2(other.handlersLocker);

		handlers = other.handlers;
		return *this;
	}

	Event& operator=(const Event&& other)
	{
		std::lock_guard<std::mutex> lock(handlersLocker);
		std::lock_guard<std::mutex> lock2(other.handlersLocker);

		std::swap(handlers, other.handlers);
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
		std::lock_guard<std::mutex> lock(handlersLocker);
		return handlers;
	}

private:
	HandlerCollectionType handlers;
	mutable std::mutex handlersLocker;
};