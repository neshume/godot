/*************************************************************************/
/*  macros.h                                                             */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2019 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2019 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef UTIL_MACROS_H
#define UTIL_MACROS_H

#ifndef __has_cpp_attribute
#define __has_cpp_attribute(attr_token) 0
#endif

#define _GD_VARNAME_CONCAT_B_(m_ignore, m_name) m_name
#define _GD_VARNAME_CONCAT_A_(m_a, m_b, m_c) _GD_VARNAME_CONCAT_B_(hello there, m_a##m_b##m_c)
#define _GD_VARNAME_CONCAT_(m_a, m_b, m_c) _GD_VARNAME_CONCAT_A_(m_a, m_b, m_c)
#define GD_UNIQUE_NAME(m_name) _GD_VARNAME_CONCAT_(m_name, _, __COUNTER__)

// static assert
// TODO: Get rid of this macro once we upgrade to C++11

#ifdef __cpp_static_assert
#define GD_STATIC_ASSERT(m_cond) static_assert((m_cond), "Condition '" #m_cond "' failed")
#else
#define GD_STATIC_ASSERT(m_cond) typedef int GD_UNIQUE_NAME(godot_static_assert)[((m_cond) ? 1 : -1)]
#endif

// final
// TODO: Get rid of this macro once we upgrade to C++11

#if (__cplusplus >= 201103L)
#define GD_FINAL final
#else
#define GD_FINAL
#endif

// noreturn
// TODO: Get rid of this macro once we upgrade to C++11

#if __has_cpp_attribute(deprecated)
#define GD_NORETURN [[noreturn]]
#elif defined(__GNUC__)
#define GD_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define GD_NORETURN __declspec(noreturn)
#else
#define GD_NORETURN
#pragma message "Macro GD_NORETURN will have no effect"
#endif

// unreachable

#if defined(_MSC_VER)
#define GD_UNREACHABLE() __assume(0)
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__) >= 405
#define GD_UNREACHABLE() __builtin_unreachable()
#else
#define GD_UNREACHABLE() \
	CRASH_NOW();         \
	do {                 \
	} while (true);
#endif

#endif // UTIL_MACROS_H
