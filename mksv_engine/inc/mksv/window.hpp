#pragma once

#include "common/types.hpp"
#include "wrapper/windows.h"

#include <optional>
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
    friend auto new_window( const WindowProps props ) -> std::optional<Window>;

public:
    static constexpr const wchar_t CLASS_NAME[] = L"MksvEngineWindowClass";

public:
    Window( const Window& ) = delete;
    Window( Window&& );
    auto operator=( const Window& ) -> Window& = delete;
    auto operator=( Window&& ) -> Window&;
    ~Window();

public:
    auto show() const -> bool;

private:
    Window( const HWND h_wnd, const WindowProps props );

private:
    HWND _h_wnd;
    u32  _width;
    u32  _height;
};

auto new_window( const WindowProps props ) -> std::optional<Window>;

} // namespace mksv
