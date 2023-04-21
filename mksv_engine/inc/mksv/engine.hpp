#pragma once

#include "wrapper/windows.h"

#include <expected>

namespace mksv
{
class Engine
{
    friend auto new_engine() -> std::expected<Engine, HRESULT>;

public:
    Engine( const Engine& ) = delete;
    Engine( Engine&& ) = default;
    auto operator=( const Engine& ) -> Engine& = delete;
    auto operator=( Engine&& ) -> Engine& = default;
    ~Engine() = default;

private:
    Engine() = default;
};

auto new_engine() -> std::expected<Engine, HRESULT>;

} // namespace mksv
