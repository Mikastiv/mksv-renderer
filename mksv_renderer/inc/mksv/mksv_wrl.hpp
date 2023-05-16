#pragma once

#pragma warning( push )
#pragma warning( disable : 4265 )

#include <wrl.h>

#pragma warning( pop )

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
