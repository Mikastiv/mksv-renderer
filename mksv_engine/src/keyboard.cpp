#include "mksv/keyboard.hpp"

#include <utility>

namespace mksv
{

auto Keyboard::update_key( const Key key, const KeyState key_state ) -> void
{
    static_assert( std::is_same_v<bool, std::underlying_type_t<KeyState>> );
    key_states_[std::to_underlying( key )] = key_state;
}

} // namespace mksv
