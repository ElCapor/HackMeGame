#include <Windows.h>
#include <anticheat/CAnticheat.hpp>
#include <anticheat/Scammer.hpp>
#define PRINT_ENUM(enum) printf(#enum);

void printGrantedAccess(uint32_t ulGrantedAccess)
{
    printf("Handle permissions:\n");
    if (ulGrantedAccess & 0x0)
        PRINT_ENUM("No Access");
    if (ulGrantedAccess & PROCESS_TERMINATE)
        PRINT_ENUM(PROCESS_TERMINATE);
    if (ulGrantedAccess & PROCESS_CREATE_THREAD)
        PRINT_ENUM(PROCESS_CREATE_THREAD);
    if (ulGrantedAccess & PROCESS_SET_SESSIONID)
        PRINT_ENUM(PROCESS_SET_SESSIONID);
    if (ulGrantedAccess & PROCESS_VM_OPERATION)
        PRINT_ENUM(PROCESS_VM_OPERATION);
    if (ulGrantedAccess & PROCESS_VM_READ)
        PRINT_ENUM(PROCESS_VM_READ);
    if (ulGrantedAccess & PROCESS_VM_WRITE)
        PRINT_ENUM(PROCESS_VM_WRITE);
    if (ulGrantedAccess & PROCESS_DUP_HANDLE)
        PRINT_ENUM(PROCESS_DUP_HANDLE);
    if (ulGrantedAccess & PROCESS_CREATE_PROCESS)
        PRINT_ENUM(PROCESS_CREATE_PROCESS);
    if (ulGrantedAccess & PROCESS_SET_QUOTA)
        PRINT_ENUM(PROCESS_SET_QUOTA);
    if (ulGrantedAccess & PROCESS_SET_INFORMATION)
        PRINT_ENUM(PROCESS_SET_INFORMATION);
    if (ulGrantedAccess & PROCESS_QUERY_INFORMATION)
        PRINT_ENUM(PROCESS_QUERY_INFORMATION);
    if (ulGrantedAccess & PROCESS_SUSPEND_RESUME)
        PRINT_ENUM(PROCESS_SUSPEND_RESUME);
    if (ulGrantedAccess & PROCESS_QUERY_LIMITED_INFORMATION)
        PRINT_ENUM(PROCESS_QUERY_LIMITED_INFORMATION);
    if (ulGrantedAccess & PROCESS_SET_LIMITED_INFORMATION)
        PRINT_ENUM(PROCESS_SET_LIMITED_INFORMATION);
    if (ulGrantedAccess & SYNCHRONIZE)
        PRINT_ENUM(SYNCHRONIZE);

    printf("\n");
}
#include <anticheat/CInternals.hpp>
void CAnti::StartAntiCheat()
{
    auto NtDuplicateObject = ntdll::GetFunction<NTSTATUS, HANDLE, HANDLE, HANDLE, PHANDLE, ULONG, ULONG, ULONG>("NtDuplicateObject");
    structs::PSYSTEM_HANDLE_INFORMATION handleInfo;
    ULONG pid = GetCurrentProcessId();
    handleInfo = Internal::QueryHandleInformations().value();
    printf("System handle count, %d\n", handleInfo->ulHandleCount); // this returns the count of all handles in the system
    int real_number = 0;
    for (int i = 0; i < handleInfo->ulHandleCount; i++)
    {
        structs::SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = handleInfo->Handles[i];
        if (handle.usUniqueProcessId != pid)
            continue;
        real_number++;
        // printGrantedAccess(handle.ulGrantedAcess);
        NtDuplicateObject(GetModuleHandle(0), (HANDLE)handle.usHandleValue, 0, 0, 0, 0, 1);
    }
    printf("Process handle count, %d\n", real_number);
    return;
}