#include <Windows.h>
#include <anticheat/CInternals.hpp>
#include <anticheat/CAnticheat.hpp>
#include <anticheat/Scammer.hpp>
std::optional<structs::PSYSTEM_HANDLE_INFORMATION> Internal::QueryHandleInformations()
{
    auto NtQuerySystemInformation = ntdll::GetFunction<NTSTATUS, ULONG, PVOID, ULONG, PULONG>("NtQuerySystemInformation");
    structs::PSYSTEM_HANDLE_INFORMATION handleInfo;
    ULONG handleInfoSize = 0x10000;
    NTSTATUS status;
    handleInfo = (structs::PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);
    while ((status = NtQuerySystemInformation(
                0x10, // 0x10
                handleInfo,
                handleInfoSize,
                NULL)) == STATUS_INFO_LENGTH_MISMATCH)
        handleInfo = (structs::PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize *= 2);
    /* NtQuerySystemInformation stopped giving us STATUS_INFO_LENGTH_MISMATCH. */
    if (!NT_SUCCESS(status))
    {
        printf("NtQuerySystemInformation failed!\n");
        return std::nullopt;
    }
    return handleInfo;
}
std::optional<std::uint32_t> Internal::GetCurrentPid()
{
    return std::optional<std::uint32_t>(GetCurrentProcessId());
}