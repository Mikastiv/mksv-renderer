#pragma once

#include "common/types.hpp"
#include "wrapper/windows.h"

#include <expected>
#include <string_view>

namespace mksv
{

struct WindowProps {
    u32               width = 800;
    u32               height = 600;
    std::wstring_view title = L"MKSV Engine";
    HINSTANCE         h_instance = nullptr;
};

class Window
{
    friend auto new_window( const u32 width, const u32 height ) -> std::expected<Window, HRESULT>;

public:
    Window( const Window& ) = delete;
    Window( Window&& ) = default;
    auto operator=( const Window& ) -> Window& = delete;
    auto operator=( Window&& ) -> Window& = default;
    ~Window();

private:
    Window( const WindowProps props );
};

auto new_window( const WindowProps props ) -> std::expected<Window, HRESULT>;

} // namespace mksv
