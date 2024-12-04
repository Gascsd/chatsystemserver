// file      : bpkg/version.hxx.in -*- C++ -*-
// license   : MIT; see accompanying LICENSE file

#ifndef BPKG_VERSION // Note: using the version macro itself.

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
#define BPKG_VERSION       17000000000ULL
#define BPKG_VERSION_STR   "0.17.0"
#define BPKG_VERSION_ID    "0.17.0"

#define BPKG_VERSION_MAJOR 0
#define BPKG_VERSION_MINOR 17
#define BPKG_VERSION_PATCH 0

#define BPKG_PRE_RELEASE   false

#define BPKG_SNAPSHOT      0ULL
#define BPKG_SNAPSHOT_ID   ""

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
#  define BPKG_OS "MinGW"
# else
#  define BPKG_OS "Windows"
# endif
#elif defined(__linux__)
#  define BPKG_OS "GNU/Linux"
#elif defined(__APPLE__)
#  define BPKG_OS "MacOS"
#elif defined(__CYGWIN__)
#  define BPKG_OS "Cygwin"
#elif defined(__FreeBSD__)
#  define BPKG_OS "FreeBSD"
#elif defined(__OpenBSD__)
#  define BPKG_OS "OpenBSD"
#elif defined(__NetBSD__)
#  define BPKG_OS "NetBSD"
#elif defined(__sun)
#  define BPKG_OS "Solaris"
#elif defined(__hpux)
#  define BPKG_OS "HP-UX"
#elif defined(_AIX)
#  define BPKG_OS "AIX"
#elif defined(__unix)
#  define BPKG_OS "Unix"
#elif defined(__posix)
#  define BPKG_OS "Posix"
#else
#  define BPKG_OS "Other"
#endif

#define BPKG_USER_AGENT                                             \
  "bpkg/" BPKG_VERSION_ID " (" BPKG_OS "; +https://build2.org)"     \
  " libbpkg/" LIBBPKG_VERSION_ID                                    \
  " libbutl/" LIBBUTL_VERSION_ID

#endif // BPKG_VERSION
