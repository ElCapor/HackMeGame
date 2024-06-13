#ifndef CINTERNALS_HPP
#define CINTERNALS_HPP
/*
* This should never expose to any header file, internal to anti cheat
* */
#include <anticheat/CAnticheat.hpp>
#include <anticheat/Scammer.hpp>
#include <optional>

namespace Internal
{
    std::optional<structs::PSYSTEM_HANDLE_INFORMATION> QueryHandleInformations();
    std::optional<std::uint32_t> GetCurrentPid();
}

#endif /* CINTERNALS_HPP */
