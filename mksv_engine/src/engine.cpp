#include "engine.hpp"

namespace mksv
{
auto new_engine() -> std::expected<Engine, HRESULT>
{
    return Engine{};
}
} // namespace mksv
