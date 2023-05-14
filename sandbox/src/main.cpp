#include <mksv/common/types.hpp>
#include <mksv/engine.hpp>
#include <mksv/log.hpp>
#include <mksv/mksv_win.hpp>
#include <mksv/utils/helpers.hpp>
#include <mksv/win/window.hpp>

#include <crtdbg.h>

auto WINAPI wWinMain(
    [[maybe_unused]] _In_ HINSTANCE     hInstance,
    [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
    [[maybe_unused]] _In_ LPWSTR        lpCmdLine,
    [[maybe_unused]] _In_ i32           nShowCmd
) -> i32
{
#if _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    auto engine = mksv::Engine::create();

    if ( !engine ) {
        mksv::log_error( L"Error creating the engine instance" );
        return -1;
    }

    if ( !engine->copy_data() ) {
        mksv::log_error( L"Failed to upload data to GPU" );
        return -1;
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
