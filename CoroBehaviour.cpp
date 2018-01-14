#include "CoroBehaviour.h"

CoroBehaviour::~CoroBehaviour()
{
	StopAllCoroutines();
	ResumeCoroutines();
}

Coroutine* CoroBehaviour::StartCoroutine(Enumerator InEnumerator)
{
	CoroPull Pull(InEnumerator);
	if (Pull)
	{
		return PushCoroutine(new Coroutine(Pull));
	}
	return nullptr;
}

void CoroBehaviour::StopCoroutine(Coroutine* CoroutinePtr)
{
	YieldInstruction* YieldReturnPtr = nullptr;
	while (CoroutinePtr != nullptr)
	{
		YieldReturnPtr = CoroutinePtr->GetYieldReturn();
		CoroutinePtr->End();
		CoroutinePtr = CastAsCoroutine(YieldReturnPtr);
	}

	if (YieldReturnPtr != nullptr)
	{
		YieldReturnPtr->End();
	}
}

void CoroBehaviour::StopAllCoroutines()
{
	for (Coroutine* CoroutinePtr : Coroutines)
	{
		StopCoroutine(CoroutinePtr);
	}
}

bool CoroBehaviour::HasCoroutines()
{
	return !Coroutines.empty();
}

Coroutine* CoroBehaviour::CastAsCoroutine(YieldInstruction* YieldInstructionPtr)
{
	if (YieldInstructionPtr == nullptr || YieldInstructionPtr->Type != TYPEID(Coroutine))
	{
		return nullptr;
	}
	return static_cast<Coroutine*>(YieldInstructionPtr);
}

bool CoroBehaviour::YieldCoroutine(Coroutine* CoroutinePtr)
{
	YieldInstruction* YieldReturnPtr = CoroutinePtr->GetYieldReturn();
	if (YieldReturnPtr != nullptr)
	{
		if (YieldReturnPtr->IsEnded())
		{
			delete YieldReturnPtr;
		}
		else
		{
			Coroutines.insert(CoroutinePtr);
			return true;
		}
	}
	return false;
}

Coroutine* CoroBehaviour::PushCoroutine(Coroutine* CoroutinePtr)
{
	Coroutine* YieldReturnPtr = CastAsCoroutine(CoroutinePtr->GetYieldReturn());
	if (YieldReturnPtr == nullptr)
	{
		Coroutines.insert(CoroutinePtr);
	}
	else
	{
		YieldReturnPtr->NextPtr = CoroutinePtr;
	}
	return CoroutinePtr;
}

Coroutine* CoroBehaviour::PopCoroutine(Coroutine* CoroutinePtr)
{
	Coroutine* NextPtr = CoroutinePtr->NextPtr;
	delete CoroutinePtr;
	return NextPtr;
}

void CoroBehaviour::ResumeCoroutines()
{
	std::set<Coroutine*> CoroutinesCopy;
	CoroutinesCopy.swap(Coroutines);

	for (Coroutine* CoroutinePtr : CoroutinesCopy)
	{
		if (YieldCoroutine(CoroutinePtr))
		{
			continue;
		}

		while (CoroutinePtr != nullptr)
		{
			CoroutinePtr->Resume();
			if (!CoroutinePtr->IsEnded())
			{
				PushCoroutine(CoroutinePtr);
				break;
			}
			CoroutinePtr = PopCoroutine(CoroutinePtr);
		}
	}
}
