#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

#include <boost/current_function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#include "core/utils/assert/assert.h"
#include "core/utils/log/log.h"
#include "core/utils/maths/maths.h"

