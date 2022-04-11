// Copyright 2022 Toyota Research Institute
#pragma once

#include <string>

#include <maliput/common/assertion_error.h>
#include <maliput/common/logger.h>
#include <maliput/common/maliput_abort.h>
#include <maliput/common/maliput_copyable.h>
#include <maliput/common/maliput_throw.h>

/// @def MALIPUT_OBJECT_DEMAND
/// Used to declare a demand. Will quit execution otherwise.
#define MALIPUT_OBJECT_DEMAND(condition) MALIPUT_DEMAND(condition)

/// @def MALIPUT_OBJECT_DEMAND
/// Aborts the program (via \::abort) with a message showing at least the
/// given message (macro argument), function name, file, and line.
#define MALIPUT_OBJECT_ABORT_MSG(msg) MALIPUT_ABORT_MESSAGE(msg)

/// @def MALIPUT_OBJECT_IS_IN_RANGE
/// Throws if `value` is within [`min_value`; `max_value`]. It forwards the call
/// to MALIPUT_OBJECT_VALIDATE() with a customized string stating the error.
#define MALIPUT_OBJECT_IS_IN_RANGE(value, min_value, max_value)                                       \
  do {                                                                                                \
    MALIPUT_OBJECT_VALIDATE(value >= min_value, maliput::common::assertion_error,                     \
                            std::to_string(value) + " is less than " + std::to_string(min_value));    \
    MALIPUT_OBJECT_VALIDATE(value <= max_value, maliput::common::assertion_error,                     \
                            std::to_string(value) + " is greater than " + std::to_string(max_value)); \
  } while (0)

/// @def MALIPUT_OBJECT_THROW_UNLESS
/// Used to declare a conditional throw.
#define MALIPUT_OBJECT_THROW_UNLESS(condition) MALIPUT_THROW_UNLESS(condition)

/// @def MALIPUT_OBJECT_THROW_MESSAGE
/// Throws with `msg`.
#define MALIPUT_OBJECT_THROW_MESSAGE(msg)                                              \
  do {                                                                                 \
    const std::string message(msg);                                                    \
    MALIPUT_OBJECT_VALIDATE(false, maliput::common::assertion_error, message.c_str()); \
  } while (0)

/// @def MALIPUT_OBJECT_NO_COPY_NO_MOVE_NO_ASSIGN
/// Deletes the special member functions for copy-construction, copy-assignment,
/// move-construction, and move-assignment.
/// Invoke this macro in the public section of the class declaration, e.g.:
/// <pre>
/// class Foo {
///  public:
///   MALIPUT_OBJECT_NO_COPY_NO_MOVE_NO_ASSIGN(Foo)
///
///   // ...
/// };
/// </pre>
/// */
#define MALIPUT_OBJECT_NO_COPY_NO_MOVE_NO_ASSIGN(Classname) MALIPUT_NO_COPY_NO_MOVE_NO_ASSIGN(Classname)

/// @def MALIPUT_OBJECT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN
/// Implements default special member functions for copy-construction,
/// copy-assignment, move-construction, and move-assignment.
/// Invoke this macro in the public section of the class declaration, e.g.:
/// <pre>
/// class Foo {
///  public:
///   MALIPUT_OBJECT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Foo)
///
///   // ...
/// };
/// </pre>
/// */
#define MALIPUT_OBJECT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Classname) MALIPUT_DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Classname)

/// @def STR_SIMPLE
/// Internal stringify a token
#define STR_SIMPLE(x) #x

/// @def STR
/// Stringify a token
#define STR(x) STR_SIMPLE(x)

/// @def MALIPUT_OBJECT_VALIDATE
/// Used to validate that an argument passed into a function or method is true;
/// if not, an exception of type exctype is thrown.
#define MALIPUT_OBJECT_VALIDATE(pred, exctype, message)                      \
  do {                                                                       \
    if (!(pred)) {                                                           \
      std::string fullname = std::string(__FILE__);                          \
      size_t found = fullname.find_last_of("/");                             \
      std::string fname = fullname;                                          \
      if (found != std::string::npos) {                                      \
        fname = fullname.substr(found + 1);                                  \
      }                                                                      \
      std::string errmsg(fname);                                             \
      errmsg.append(":").append(__func__).append(":").append(STR(__LINE__)); \
      errmsg.append(": ").append(message);                                   \
      throw exctype(errmsg);                                                 \
    }                                                                        \
  } while (0)

/// @def MALIPUT_OBJECT_TRACE
/// Rely on maliput::log->trace(). Used to trace log `msg` and add
/// information of file, line and function.
#define MALIPUT_OBJECT_TRACE(msg)                                                      \
  do {                                                                                 \
    const std::string message(msg);                                                    \
    maliput::log()->trace("[\"{}\":{}:{}] {}", __FILE__, __LINE__, __func__, message); \
  } while (0)
