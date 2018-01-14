#pragma once

#include <ctime>
#include <functional>
#include <boost/coroutine2/all.hpp>

#include <iostream>

// typeid without rtti
using TYPE_INFO = void*;

template <typename T> TYPE_INFO _TYPEID_()
{
	return reinterpret_cast<TYPE_INFO>(&_TYPEID_<T>);
}

#define TYPEID(T) _TYPEID_<T>()

class YieldInstruction
{
	friend class CoroBehaviour;

private:
	TYPE_INFO Type;

protected:
	YieldInstruction(TYPE_INFO InType)
		: Type(InType)
	{
	}

	virtual ~YieldInstruction()
	{
	}

	virtual void End()
	{
	}

	virtual bool IsEnded()
	{
		return true;
	}
};

class WaitForSeconds : public YieldInstruction
{
private:
	float Time;

	float Now()
	{
		return std::clock() / static_cast<float>(CLOCKS_PER_SEC);
	}
	
public:
	WaitForSeconds(float Seconds)
		: YieldInstruction(TYPEID(WaitForSeconds))
		, Time(Now() + Seconds)
	{
	}

protected:
	virtual void End() override
	{
		Time = Now();
	}

	virtual bool IsEnded() override
	{
		return Time <= Now();
	}
};

class WaitWhile : public YieldInstruction
{
private:
	std::function<bool ()> Condition;

public:
	WaitWhile(std::function<bool ()> InCondition)
		: YieldInstruction(TYPEID(WaitWhile))
		, Condition(InCondition)
	{
	}

protected:
	virtual void End() override
	{
		Condition = []()
		{
			return false;
		};
	}

	virtual bool IsEnded() override
	{
		return !Condition();
	}
};

class WaitUntil : public YieldInstruction
{
private:
	std::function<bool ()> Condition;

public:
	WaitUntil(std::function<bool ()> InCondition)
		: YieldInstruction(TYPEID(WaitWhile))
		, Condition(InCondition)
	{
	}

protected:
	virtual void End() override
	{
		Condition = []()
		{
			return true;
		};
	}

	virtual bool IsEnded() override
	{
		return Condition();
	}
};

using CoroBoost = boost::coroutines2::coroutine<class YieldInstruction*>;
using CoroPull = CoroBoost::pull_type;
using CoroPush = CoroBoost::push_type;
using Enumerator = std::function<void (CoroPush&)>;

class Coroutine : public YieldInstruction
{
	friend class CoroBehaviour;

private:
	CoroPull Pull;
	Coroutine* NextPtr;

private:
	Coroutine(CoroPull& InPull)
		: YieldInstruction(TYPEID(Coroutine))
		, Pull(std::move(InPull))
		, NextPtr(nullptr)
	{
	}

protected:
	virtual void End() override
	{
		Pull = CoroPull(Enumerator());
	}

	virtual bool IsEnded() override
	{
		return !Pull;
	}

	void Resume()
	{
		Pull();
	}

	YieldInstruction* GetYieldReturn()
	{
		return Pull.get();
	}
};
