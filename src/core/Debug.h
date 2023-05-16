#pragma once

#ifdef _DEBUG

#define DEBUG_BREAK     __debugbreak()

#else

#define DEBUG_BREAK

#endif