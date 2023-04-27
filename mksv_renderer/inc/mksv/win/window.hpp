#pragma once

#include "mksv/common/types.hpp"
#include "mksv/mksv_win.hpp"

#include <memory>
#include <string_view>

namespace mksv
{

struct WindowProps {
    u32          width;
    u32          height;
    std::wstring title;
    std::wstring class_name;
};

class Window
{
    friend auto new_window( WindowProps props ) -> std::unique_ptr<Window>;

public:
    Window( const Window& ) = delete;
    Window( Window&& );
    auto operator=( const Window& ) -> Window& = delete;
    auto operator=( Window&& ) -> Window&;
    ~Window();

public:
    auto show() const -> bool;
    auto handle() const -> HWND;

private:
    Window( const HWND h_wnd, WindowProps props );

private:
    HINSTANCE   h_instance_;
    HWND        h_wnd_;
    WindowProps props_;
};

auto new_window( WindowProps props ) -> std::unique_ptr<Window>;

} // namespace mksv
