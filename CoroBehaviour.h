#pragma once

#include "CoroDefinition.h"

#include <set>

class CoroBehaviour
{
public:
	virtual ~CoroBehaviour();
	
private:
	std::set<Coroutine*> Coroutines;

public:
	Coroutine* StartCoroutine(Enumerator InEnumerator);
	void StopCoroutine(Coroutine* CoroutinePtr);
	void StopAllCoroutines();
	bool HasCoroutines();

private:
	Coroutine* CastAsCoroutine(YieldInstruction* YieldInstructionPtr);
	bool YieldCoroutine(Coroutine* CoroutinePtr);
	Coroutine* PushCoroutine(Coroutine* CoroutinePtr);
	Coroutine* PopCoroutine(Coroutine* CoroutinePtr);
	
protected:
	void ResumeCoroutines();
};
