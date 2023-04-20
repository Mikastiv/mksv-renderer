#include <mksv/common/types.hpp>
#include <mksv/engine.hpp>
#include <mksv/log.hpp>
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
    (void)hInstance;

    auto engine_result = mksv::new_engine();

    if ( !engine_result.has_value() ) {
        mksv::log_fatal( L"Error creating the engine instance" );
        return -1;
    }

    auto engine = std::move( *engine_result );

    return 0;
}
