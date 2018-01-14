# CoroBehaviour

This is the coroutine class using C++ and [boost coroutine2](https://github.com/boostorg/coroutine2).  
Usage of `CoroBehaviour` is very similar with `MonoBehaviour` [Unity](https://unity3d.com/).  
You may try to introduce this to the project of [Unreal Engine](https://www.unrealengine.com/).  

## Requirements

- C++ 11
- Boost Coroutine2

## CoroBehaviour vs. MonoBehaviour

MonoBehaviour in Unity:
```c#
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExampleClass : MonoBehaviour
{
    void Start()
    {
        StartCoroutine(coroutineA());
    }

    IEnumerator coroutineA()
    {
        // wait for 1 second
        Debug.Log("coroutineA created");
        yield return new WaitForSeconds(1.0f);
        yield return StartCoroutine(coroutineB());
        Debug.Log("coroutineA running again");
    }

    IEnumerator coroutineB()
    {
        Debug.Log("coroutineB created");
        yield return new WaitForSeconds(2.5f);
        Debug.Log("coroutineB enables coroutineA to run");
    }
}
```

CoroBehaviour in C++:
```c++
#include "CoroBehaviour.h"
#include <iostream>

class ExampleClass : public CoroBehaviour
{
    void Start()
    {
        StartCoroutine(coroutineA());
    }

    Enumerator coroutineA()
    {
        return [=](CoroPush& yield_return)
        {
            // wait for 1 second
            std::cout << "coroutineA created";
            yield_return(new WaitForSeconds(1.0f));
            yield_return(StartCoroutine(coroutineB()));
            std::cout << "coroutineA running again";
        };
    }

    Enumerator coroutineB()
    {
        return [=](CoroPush& yield_return)
        {
            std::cout << "coroutineB created";
            yield_return(new WaitForSeconds(2.5f));
            std::cout << "coroutineB enables coroutineA to run";
        };
    }
}
```

## Disclaimer

I didn't consider about performace, stability and security.  
Please use this at your own risk.

And I have no plan to make this move forward.  
Please do it yourself if you need more features.

## Contact

exawon@gmail.com
