// file      : bdep/version.hxx.in -*- C++ -*-
// license   : MIT; see accompanying LICENSE file

#ifndef BDEP_VERSION // Note: using the version macro itself.

// The numeric version format is AAAAABBBBBCCCCCDDDE where:
//
// AAAAA - major version number
// BBBBB - minor version number
// CCCCC - bugfix version number
// DDD   - alpha / beta (DDD + 500) version number
// E     - final (0) / snapshot (1)
//
// When DDDE is not 0, 1 is subtracted from AAAAABBBBBCCCCC. For example:
//
// Version      AAAAABBBBBCCCCCDDDE
//
// 0.1.0        0000000001000000000
// 0.1.2        0000000001000020000
// 1.2.3        0000100002000030000
// 2.2.0-a.1    0000200001999990010
// 3.0.0-b.2    0000299999999995020
// 2.2.0-a.1.z  0000200001999990011
//
#define BDEP_VERSION       17000000000ULL
#define BDEP_VERSION_STR   "0.17.0"
#define BDEP_VERSION_ID    "0.17.0"

#define BDEP_VERSION_MAJOR 0
#define BDEP_VERSION_MINOR 17
#define BDEP_VERSION_PATCH 0

#define BDEP_PRE_RELEASE   false

#define BDEP_SNAPSHOT      0ULL
#define BDEP_SNAPSHOT_ID   ""

#include <libbutl/version.hxx>

#ifdef LIBBUTL_VERSION
#  if !(LIBBUTL_VERSION >= 17000000000ULL && LIBBUTL_VERSION < 17999990001ULL)
#    error incompatible libbutl version, libbutl ^0.17.0 is required
#  endif
#endif

#include <libbpkg/version.hxx>

#ifdef LIBBPKG_VERSION
#  if !(LIBBPKG_VERSION >= 17000000000ULL && LIBBPKG_VERSION < 17999990001ULL)
#    error incompatible libbpkg version, libbpkg ^0.17.0 is required
#  endif
#endif

// User agent.
//
#if   defined(_WIN32)
# if defined(__MINGW32__)
#  define BDEP_OS "MinGW"
# else
#  define BDEP_OS "Windows"
# endif
#elif defined(__linux__)
#  define BDEP_OS "GNU/Linux"
#elif defined(__APPLE__)
#  define BDEP_OS "MacOS"
#elif defined(__CYGWIN__)
#  define BDEP_OS "Cygwin"
#elif defined(__FreeBSD__)
#  define BDEP_OS "FreeBSD"
#elif defined(__OpenBSD__)
#  define BDEP_OS "OpenBSD"
#elif defined(__NetBSD__)
#  define BDEP_OS "NetBSD"
#elif defined(__sun)
#  define BDEP_OS "Solaris"
#elif defined(__hpux)
#  define BDEP_OS "HP-UX"
#elif defined(_AIX)
#  define BDEP_OS "AIX"
#elif defined(__unix)
#  define BDEP_OS "Unix"
#elif defined(__posix)
#  define BDEP_OS "Posix"
#else
#  define BDEP_OS "Other"
#endif

#define BDEP_USER_AGENT                                             \
  "bdep/" BDEP_VERSION_ID " (" BDEP_OS "; +https://build2.org)"     \
  " libbpkg/" LIBBPKG_VERSION_ID                                    \
  " libbutl/" LIBBUTL_VERSION_ID

#endif // BDEP_VERSION
