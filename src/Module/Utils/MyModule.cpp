#include "MyModule.h"
#include <functional>

MyModule::MyModule()
    :moduleId(allocModuleId())
{

}

void MyModule::connect(uint32_t senderId, Fn fn)
{
    senderId += moduleId;
    messageMap.emplace(senderId, fn);
}

void MyModule::disconnect(uint32_t senderId)
{
    senderId += moduleId;
    messageMap.erase(senderId);
}

void *MyModule::emit(uint32_t senderId, void *arg)
{
    senderId += moduleId;
    auto it = messageMap.find(senderId);
    if(it != messageMap.end())
    {
        Fn fn = messageMap[senderId];
        return fn(arg);
    }
    return NULL;
}

uint32_t MyModule::allocModuleId()
{
    static uint32_t newId = 0;
    newId += 1000;
    return newId;
}