#pragma once

#include "wrapper/windows.h"

#include <expected>

namespace mksv
{
enum class EngineError {
    RegisterClassFailed
};

class Engine
{
    friend auto new_engine( const HINSTANCE h_instance ) -> std::expected<Engine, EngineError>;

public:
    Engine( const Engine& ) = delete;
    Engine( Engine&& );
    auto operator=( const Engine& ) -> Engine& = delete;
    auto operator=( Engine&& ) -> Engine&;
    ~Engine();

private:
    Engine( const HINSTANCE h_instance );

private:
    HINSTANCE _h_instance;
};

auto new_engine( const HINSTANCE h_instance ) -> std::expected<Engine, EngineError>;

} // namespace mksv
