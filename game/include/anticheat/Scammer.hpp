#ifndef SCAMMER_HPP
#define SCAMMER_HPP
/*
* SCAMMER TOOLKIT
*/
#include <stdexcept>
#include <functional>
#include <string>

#define NT_SUCCESS(x) ((x) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

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

namespace structs {
    // https://processhacker.sourceforge.io/doc/ntexapi_8h_source.html
    typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
    {
        USHORT usUniqueProcessId;
        USHORT usCreatorBackTraceIndex;
        UCHAR ucObjectTypeIndex;
        UCHAR ucHandleAttributes;
        USHORT usHandleValue;
        PVOID pvObject;
        ULONG ulGrantedAcess;
    } SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;
    typedef struct _SYSTEM_HANDLE_INFORMATION
    {
        ULONG ulHandleCount;
        SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
    } SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;
};


#endif /* SCAMMER_HPP */
