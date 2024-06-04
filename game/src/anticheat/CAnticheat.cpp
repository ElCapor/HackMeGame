#include <Windows.h>
#include <anticheat/CAnticheat.hpp>
#include <anticheat/Scammer.hpp>

#define NT_SUCCESS(x) ((x) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004
#define PRINT_ENUM(enum) printf(#enum);

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

void printGrantedAccess(uint32_t ulGrantedAccess) {
    printf("Handle permissions:\n");
    if (ulGrantedAccess & 0x0) PRINT_ENUM("No Access");
    if (ulGrantedAccess & PROCESS_TERMINATE) PRINT_ENUM(PROCESS_TERMINATE);
    if (ulGrantedAccess & PROCESS_CREATE_THREAD) PRINT_ENUM(PROCESS_CREATE_THREAD);
    if (ulGrantedAccess & PROCESS_SET_SESSIONID) PRINT_ENUM(PROCESS_SET_SESSIONID);
    if (ulGrantedAccess & PROCESS_VM_OPERATION) PRINT_ENUM(PROCESS_VM_OPERATION);
    if (ulGrantedAccess & PROCESS_VM_READ) PRINT_ENUM(PROCESS_VM_READ);
    if (ulGrantedAccess & PROCESS_VM_WRITE) PRINT_ENUM(PROCESS_VM_WRITE);
    if (ulGrantedAccess & PROCESS_DUP_HANDLE) PRINT_ENUM(PROCESS_DUP_HANDLE);
    if (ulGrantedAccess & PROCESS_CREATE_PROCESS) PRINT_ENUM(PROCESS_CREATE_PROCESS);
    if (ulGrantedAccess & PROCESS_SET_QUOTA) PRINT_ENUM(PROCESS_SET_QUOTA);
    if (ulGrantedAccess & PROCESS_SET_INFORMATION) PRINT_ENUM(PROCESS_SET_INFORMATION);
    if (ulGrantedAccess & PROCESS_QUERY_INFORMATION) PRINT_ENUM(PROCESS_QUERY_INFORMATION);
    if (ulGrantedAccess & PROCESS_SUSPEND_RESUME) PRINT_ENUM(PROCESS_SUSPEND_RESUME);
    if (ulGrantedAccess & PROCESS_QUERY_LIMITED_INFORMATION) PRINT_ENUM(PROCESS_QUERY_LIMITED_INFORMATION);
    if (ulGrantedAccess & PROCESS_SET_LIMITED_INFORMATION) PRINT_ENUM(PROCESS_SET_LIMITED_INFORMATION);
    if (ulGrantedAccess & SYNCHRONIZE) PRINT_ENUM(SYNCHRONIZE);
    
    printf("\n");
}

void CAnti::StartAntiCheat()
{
    auto NtQuerySystemInformation = ntdll::GetFunction<NTSTATUS, ULONG, PVOID, ULONG, PULONG>("NtQuerySystemInformation");
    NTSTATUS status;
    structs::PSYSTEM_HANDLE_INFORMATION handleInfo;
    ULONG handleInfoSize = 0x10000;
    HANDLE processHandle = GetModuleHandle("game.exe");
    ULONG pid = GetCurrentProcessId();
    handleInfo = (structs::PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);
    while ((status = NtQuerySystemInformation(
        0x10, // 0x10
        handleInfo,
        handleInfoSize,
        NULL
        )) == STATUS_INFO_LENGTH_MISMATCH)
        handleInfo = (structs::PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize *= 2);

    /* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
    if (!NT_SUCCESS(status))
    {
        printf("NtQuerySystemInformation failed!\n");
        return;
    }
    printf("System handle count, %d\n", handleInfo->ulHandleCount); // this returns the count of all handles in the system
    int real_number = 0;
    for (int i=0; i < handleInfo->ulHandleCount; i++)
    {
        structs::SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = handleInfo->Handles[i];
        if (handle.usUniqueProcessId != pid)
            continue;
        real_number++;
        printGrantedAccess(handle.ulGrantedAcess);
    }
    printf("Process handle count, %d\n", real_number);
    return;
}