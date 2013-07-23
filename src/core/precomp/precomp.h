#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

#include <boost/current_function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

#define _WIN32_WINNT            0x0501
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "core/utils/assert/assert.h"
#include "core/utils/log/log.h"
