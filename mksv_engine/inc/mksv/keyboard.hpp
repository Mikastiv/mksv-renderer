#pragma once

#include "mksv/keycodes.hpp"

#include <array>

namespace mksv
{
enum class KeyState : bool {
    Down = 0,
    Up = 1,
};

struct KeyEvent {
};

class Keyboard
{
public:
    Keyboard() = default;
    Keyboard( const Keyboard& ) = default;
    Keyboard( Keyboard&& ) = default;
    auto operator=( const Keyboard& ) -> Keyboard& = default;
    auto operator=( Keyboard&& ) -> Keyboard& = default;
    ~Keyboard() = default;

public:
    auto update_key( const Key key, const KeyState key_state ) -> void;

private:
    std::array<bool, 256> key_states_;
};
} // namespace mksv
