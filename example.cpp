#include <iostream>
#include <sstream>
#include <string>

#include "CoroBehaviour.h"

class ExampleBehaviour : public CoroBehaviour
{
private:
	std::stringstream Stream;

public:
	Enumerator TaskA()
	{
		return [=](CoroPush& yield_return)
		{
			Stream << "A1 ";
			yield_return(nullptr);

			Stream << "A2 ";
		};
	}

	Enumerator TaskB(float Param)
	{
		return [=](CoroPush& yield_return)
		{
			Stream << "B1 ";
			yield_return(new WaitForSeconds(Param));

			Stream << "B2 ";
			yield_return(new WaitForSeconds(Param));

			Stream << "B3 ";
		};
	}

	Enumerator TaskC(float Param)
	{
		return [=](CoroPush& yield_return)
		{
			Stream << "C1 ";
			yield_return(StartCoroutine(TaskA()));

			Stream << "C2 ";
			yield_return(StartCoroutine(TaskB(Param)));

			Stream << "C3 ";
		};
	}

	Enumerator TaskD(int Param)
	{
		return [=](CoroPush& yield_return)
		{
			for (int i = 1; i < Param; i++)
			{
				Stream << "D" << i << " ";
				yield_return(new WaitForSeconds(0.5f));
			}
			Stream << "D" << Param << " ";
		};
	}

	void Test()
	{
		std::cout << Stream.str() << std::endl;
		Stream.str("");

		while (HasCoroutines())
		{
			ResumeCoroutines();

			auto str = Stream.str();
			if (str.size() > 0)
			{
				std::cout << str << std::endl;
				Stream.str("");
			}
		}
	}
};

int main()
{
	ExampleBehaviour foo;
	foo.StartCoroutine(foo.TaskA());
	foo.StartCoroutine(foo.TaskB(1.0f));
	foo.StartCoroutine(foo.TaskC(2.0f));
	foo.StartCoroutine(foo.TaskD(10));
	foo.Test();
	return 0;
}
