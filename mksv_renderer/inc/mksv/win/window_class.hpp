#pragma once

#include "mksv/mksv_win.hpp"
#include "mksv/utils/helpers.hpp"

#include <memory>
#include <string_view>

namespace mksv
{
class WindowClass
{
public:
    static auto create( std::wstring name ) -> std::unique_ptr<WindowClass>;

public:
    WindowClass( const WindowClass& ) = delete;
    WindowClass( WindowClass&& );
    auto operator=( const WindowClass& ) = delete;
    auto operator=( WindowClass&& ) -> WindowClass&;
    ~WindowClass();

public:
    auto get_atom() const -> ATOM;
    auto get_name() const -> std::wstring_view;

private:
    WindowClass( const ATOM atom, std::wstring name );

private:
    ATOM         atom_;
    std::wstring name_;
};

} // namespace mksv
