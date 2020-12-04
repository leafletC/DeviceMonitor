#pragma once

#include <map>
#include <stdint.h>
#include <functional>

using namespace std;

class MyModule
{
public:
    MyModule();
    using Fn = function<void *(void *)>;
    void connect(uint32_t msgId, Fn fn);
    void disconnect(uint32_t msgId);
    void *emit(uint32_t msgId, void *arg);
private:
    static uint32_t allocModuleId();
    map<uint32_t, Fn> messageMap;
    const uint32_t moduleId;
    
};