#pragma once

#include "wrapper/windows.h"

#include <expected>

namespace mksv
{
class Engine;

auto new_engine() -> std::expected<Engine, HRESULT>;

class Engine
{
public:
    friend auto new_engine() -> std::expected<Engine, HRESULT>;

public:
    Engine( const Engine& ) = delete;
    auto operator=( const Engine& ) -> Engine& = delete;
    Engine( Engine&& ) = default;
    auto operator=( Engine&& ) -> Engine& = default;
    ~Engine() = default;

private:
    Engine() = default;
};

} // namespace mksv
