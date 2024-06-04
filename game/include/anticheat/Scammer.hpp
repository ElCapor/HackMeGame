#ifndef SCAMMER_HPP
#define SCAMMER_HPP
/*
* SCAMMER TOOLKIT
*/
#include <stdexcept>
#include <functional>
#include <string>

namespace ntdll
{
    template<typename Ret, typename... Args>
    std::function<Ret(Args...)> GetFunction(const std::string& functionName) {
    static HMODULE ntdllModule = LoadLibraryA("ntdll.dll");
    if (!ntdllModule) {
        throw std::runtime_error("Failed to load ntdll.dll");
    }

    FARPROC procAddress = GetProcAddress(ntdllModule, functionName.c_str());
    if (!procAddress) {
        throw std::runtime_error("Failed to get the address of " + functionName);
    }

    return reinterpret_cast<Ret(*)(Args...)>(procAddress);
    }
}


#endif /* SCAMMER_HPP */
