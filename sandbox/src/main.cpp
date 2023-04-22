#include <mksv/common/types.hpp>
#include <mksv/engine.hpp>
#include <mksv/log.hpp>
#include <mksv/window.hpp>
#include <mksv/windows_helpers.hpp>
#include <mksv/wrapper/windows.h>

#include <crtdbg.h>

auto WINAPI wWinMain(
    _In_ HINSTANCE                      hInstance,
    [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
    [[maybe_unused]] _In_ LPWSTR        lpCmdLine,
    [[maybe_unused]] _In_ i32           nShowCmd
) -> i32
{
#if _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    auto engine_result = mksv::new_engine( hInstance );

    if ( !engine_result.has_value() ) {
        mksv::log_fatal( L"Error creating the engine instance" );
        return -1;
    }

    const auto window_props =
        mksv::WindowProps{ .width = 800, .height = 600, .title = L"MKSV Engine", .h_instance = hInstance };
    auto window_result = mksv::new_window( window_props );

    if ( !window_result.has_value() ) {
        mksv::log_fatal( L"Error creating the main window" );
        return -2;
    }

    auto engine = std::move( *engine_result );
    auto main_window = std::move( *window_result );

    if ( !main_window.show() ) {
        mksv::log_last_window_error();
    }

    MSG msg{};
    while ( msg.message != WM_QUIT ) {
        if ( PeekMessageW( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
            TranslateMessage( &msg );
            DispatchMessageW( &msg );
        }
    }

    return 0;
}
