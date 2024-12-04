#include <libbuild2/autoconf/checks.hxx>

const build2::autoconf::check build2::autoconf::checks[206] = {
{
"", "", "",

""
},

{
"BUILD2_AUTOCONF_LIBC_VERSION", "!", "",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#define BUILD2_AUTOCONF_LIBC_VERSION\n"
"\n"
"/* Include headers required to make various libc version number macros\n"
" * available.\n"
" *\n"
" * Note that besides the platform-specific macros listed below, this\n"
" * check also defines the following helper macros:\n"
" *\n"
" * BUILD2_AUTOCONF_GLIBC_PREREQ(maj, min)\n"
" * BUILD2_AUTOCONF_FREEBSD_PREREQ(maj, min)\n"
" * BUILD2_AUTOCONF_OPENBSD_PREREQ(yyyymm)\n"
" * BUILD2_AUTOCONF_NETBSD_PREREQ(maj, min)\n"
" * BUILD2_AUTOCONF_MACOS_PREREQ(maj, min)\n"
" * BUILD2_AUTOCONF_MINGW_PREREQ(maj, min)\n"
" *\n"
" * Note that all of the above macros are always defined and evaluate to false\n"
" * on platforms to which they do not apply. Which means that instead of:\n"
" *\n"
" * #if defined(__GLIBC__) && BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)\n"
" *\n"
" * You can just write:\n"
" *\n"
" * #if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)\n"
" *\n"
" * Note also that for checking just for the specific platform we still use one\n"
" * of the system macros below. For example:\n"
" *\n"
" * #if defined(__GLIBC__)\n"
" * #if defined(__FreeBSD__)\n"
" * #if defined(__OpenBSD__)\n"
" * #if defined(__NetBSD__)\n"
" * #if defined(__MINGW32__)\n"
" *\n"
" * Except for MacOS specifically, which we detect using our own macro (for\n"
" * the sake of simplicity):\n"
" *\n"
" * #if defined(BUILD2_AUTOCONF_MACOS)\n"
" *\n"
" * If, however, you want to cover the entire Apple operation systems family,\n"
" * then use:\n"
" *\n"
" * #if defined(__APPLE__)\n"
" *\n"
" * Macros for detecting platforms and their versions:\n"
" *\n"
" * __GLIBC__: The glibc major version number.\n"
" *\n"
" * __GLIBC_MINOR__: The glibc minor version number.\n"
" *\n"
" * __GLIBC_PREREQ(major,minor): Return 1 if a given version number is greater\n"
" *                              than or equal to the current glibc version\n"
" *                              (__GLIBC__.__GLIBC_MINOR__), or 0 otherwise.\n"
" *\n"
" * __FreeBSD__: Defined if on FreeBSD. The value is the FreeBSD major version\n"
" *              number but it's typically used only for OS detection. This is\n"
" *              a predefined macro so no header needs to be included.\n"
" *\n"
" * __FreeBSD_version: The FreeBSD version in the following format: MMmmXXX\n"
" *                    where `MM` is the major and `mm` the minor version\n"
" *                    numbers. The `XXX` portion can be disregarded for the\n"
" *                    purposes of feature checking (because it's too\n"
" *                    fine-grained).\n"
" *\n"
" * __OpenBSD__: Defined if on OpenBSD. This is a predefined macro so no header\n"
" *              needs to be included.\n"
" *\n"
" * OpenBSD: The OpenBSD version in the following format: YYYYMM (year and\n"
" *          month of release). There are always two releases per year: one\n"
" *          with an odd-numbered minor version earlier in the year and one\n"
" *          with an even-numbered minor version later in the year. OpenBSD 7.0\n"
" *          was released in October 2021 so its OpenBSD macro value is 202110.\n"
" *\n"
" *\n"
" * __NetBSD__: Defined if on NetBSD. This is a predefined macro so no header\n"
" *             needs to be included.\n"
" *\n"
" * __NetBSD_Version__: The NetBSD version in the following format: MMmmrrpp00\n"
" *                     where `MM` is the major and `mm` the minor version\n"
" *                     numbers, `rr` is always 0 after NetBSD version 2.0I,\n"
" *                     and `pp` is the patch level. The `rrpp00` portion can\n"
" *                     be disregarded for the purposes of feature checking\n"
" *                     (because it's too fine-grained).\n"
" *\n"
" * __MAC_OS_X_VERSION_MIN_REQUIRED: The minimum supported Mac OS version. Up\n"
" *                                  to Mac OS X 10.9, formatted as `MMm0`\n"
" *                                  where `m` was the (always single-digit)\n"
" *                                  minor version. Since Mac OS X 10.10 it is\n"
" *                                  formatted as `MMmmpp` where `mm` is the\n"
" *                                  (now potentially double-digit) minor\n"
" *                                  version and `pp` is the patch version.\n"
" *\n"
" * __MINGW32__: Defined if on original Mingw and on Mingw-w64, both 32 and\n"
" *              64-bit. This is a predefined macro so no header needs to be\n"
" *              included. Note that we don't really care about original Mingw.\n"
" *\n"
" * __MINGW64__: Defined if on Mingw-w64, 64-bit only. This is a predefined\n"
" *              macro so no header needs to be included.\n"
" *\n"
" * __MINGW64_VERSION_MAJOR:\n"
" * __MINGW64_VERSION_MINOR: The Mingw-w64 major/minor version numbers. Note\n"
" *                          that these are defined for both 32 and 64-bit.\n"
" */\n"
"#if defined(__linux__)\n"
"#  include <features.h> /* __GLIBC__, __GLIBC_MINOR__, __GLIBC_PREREQ() */\n"
"#elif defined(__FreeBSD__)\n"
"#  include <osreldate.h> /* __FreeBSD_version */\n"
"#elif defined(__OpenBSD__) || defined(__NetBSD__)\n"
"/* Note: sys/param.h defines macros with common names such as ALIGN which may\n"
" * clash with user code (see Qt).\n"
" */\n"
"#  include <sys/param.h> /* OpenBSD, __NetBSD_Version__ */\n"
"#elif defined(__APPLE__)\n"
"#  include <Availability.h> /* __MAC_OS_X_VERSION_MIN_REQUIRED */\n"
"#elif defined(__MINGW32__)\n"
"#  include <_mingw.h> /* __MINGW64_VERSION_{MAJOR,MINOR} */\n"
"#endif\n"
"\n"
"/* BUILD2_AUTOCONF_GLIBC_PREREQ(maj, min)\n"
" *\n"
" * Return 1 if the glibc version is >= the given version number, or 0\n"
" * otherwise.\n"
" *\n"
" * __GLIBC_PREREQ() first appeared in glibc 2.2 (released in 2000) so define\n"
" * BUILD2_AUTOCONF_GLIBC_PREREQ() using its latest glibc implementation if it\n"
" * is not defined.\n"
" *\n"
" * __GLIBC__ and __GLIBC_MINOR__ first appeared in glibc 1.97 on 1996-10-22\n"
" * (but only released in glibc 2.0 on 1997-01-26) with no preceding equivalent\n"
" * so always return 0 if these macros are not defined.\n"
" */\n"
"#if defined(__GLIBC__) && defined(__GLIBC_MINOR__)\n"
"#  if defined(__GLIBC_PREREQ)\n"
"#    define BUILD2_AUTOCONF_GLIBC_PREREQ(maj, min) __GLIBC_PREREQ(maj, min)\n"
"#  else\n"
"#    define BUILD2_AUTOCONF_GLIBC_PREREQ(maj, min) \\\n"
"       ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))\n"
"#  endif\n"
"#else\n"
"#  define BUILD2_AUTOCONF_GLIBC_PREREQ(maj, min) 0\n"
"#endif\n"
"\n"
"/* BUILD2_AUTOCONF_FREEBSD_PREREQ(maj, min)\n"
" *\n"
" * Return 1 if the FreeBSD version is >= the given version number, or 0\n"
" * otherwise.\n"
" */\n"
"#if defined(__FreeBSD__) && defined(__FreeBSD_version)\n"
"#  define BUILD2_AUTOCONF_FREEBSD_PREREQ(maj, min) \\\n"
"     (__FreeBSD_version >= (maj)*100000 + (min)*1000)\n"
"#else\n"
"#  define BUILD2_AUTOCONF_FREEBSD_PREREQ(maj, min) 0\n"
"#endif\n"
"\n"
"/* BUILD2_AUTOCONF_OPENBSD_PREREQ(yyyymm)\n"
" *\n"
" * Return 1 if the release date of the OpenBSD version is >= the given release\n"
" * date, or 0 otherwise.\n"
" */\n"
"#if defined(__OpenBSD__) && defined(OpenBSD)\n"
"#  define BUILD2_AUTOCONF_OPENBSD_PREREQ(yyyymm) (OpenBSD >= (yyyymm))\n"
"#else\n"
"#  define BUILD2_AUTOCONF_OPENBSD_PREREQ(yyyymm) 0\n"
"#endif\n"
"\n"
"/* BUILD2_AUTOCONF_NETBSD_PREREQ(maj, min)\n"
" *\n"
" * Return 1 if the NetBSD version is >= the given version number, or 0\n"
" * otherwise.\n"
" */\n"
"#if defined(__NetBSD__) && defined(__NetBSD_Version__)\n"
"#  define BUILD2_AUTOCONF_NETBSD_PREREQ(maj, min) \\\n"
"     (__NetBSD_Version__ >= (maj)*100000000 + (min)*1000000)\n"
"#else\n"
"#  define BUILD2_AUTOCONF_NETBSD_PREREQ(maj, min) 0\n"
"#endif\n"
"\n"
"/* BUILD2_AUTOCONF_MACOS_PREREQ(maj, min)\n"
" *\n"
" * Return 1 if the Mac OS version is >= the given version number, or 0\n"
" * otherwise.\n"
" *\n"
" * BUILD2_AUTOCONF_MACOS\n"
" *\n"
" * Defined if the current platform is MacOS, or undefined otherwise.\n"
" */\n"
"#if defined(__APPLE__) && defined(__MACH__)\n"
"/* Note that just __APPLE__ covers all the Apple platforms (MacOS, iOS, etc).\n"
" */\n"
"#  define BUILD2_AUTOCONF_MACOS 1\n"
"\n"
"#  if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED)\n"
"#    error __MAC_OS_X_VERSION_MIN_REQUIRED not defined\n"
"#  endif\n"
"#  if __MAC_OS_X_VERSION_MIN_REQUIRED <= 1090\n"
"#    define BUILD2_AUTOCONF_MACOS_PREREQ(maj, min) \\\n"
"       (__MAC_OS_X_VERSION_MIN_REQUIRED >= (maj)*100 + (min)*10)\n"
"#  else\n"
"#    define BUILD2_AUTOCONF_MACOS_PREREQ(maj, min) \\\n"
"       (__MAC_OS_X_VERSION_MIN_REQUIRED >= (maj)*10000 + (min)*100)\n"
"#  endif\n"
"#else\n"
"#  undef BUILD2_AUTOCONF_MACOS\n"
"\n"
"#  define BUILD2_AUTOCONF_MACOS_PREREQ(maj, min) 0\n"
"#endif\n"
"\n"
"/* BUILD2_AUTOCONF_MINGW_PREREQ(maj, min)\n"
" *\n"
" * Return 1 if the Mingw-w64 version is >= the given version number, or 0\n"
" * otherwise.\n"
" */\n"
"#if defined(__MINGW64_VERSION_MAJOR) && defined(__MINGW64_VERSION_MINOR)\n"
"#  define BUILD2_AUTOCONF_MINGW_PREREQ(maj, min)  \\\n"
"            (__MINGW64_VERSION_MAJOR > (maj) ||   \\\n"
"             (__MINGW64_VERSION_MAJOR == (maj) && \\\n"
"              __MINGW64_VERSION_MINOR >= (min)))\n"
"#else\n"
"#  define BUILD2_AUTOCONF_MINGW_PREREQ(maj, min) 0\n"
"#endif\n"
"\n"
"#endif /* BUILD2_AUTOCONF_LIBC_VERSION */\n"
},

{
"BYTE_ORDER", "!", "",

"\n"
"/* Include the endianness header based on platform.\n"
" *\n"
" * Each of these headers should define BYTE_ORDER, LITTLE_ENDIAN, BIG_ENDIAN,\n"
" * AND PDP_ENDIAN but this can be affected by macros like _ANSI_SOURCE,\n"
" * _POSIX_C_SOURCE, _XOPEN_SOURCE and _NETBSD_SOURCE, depending on the\n"
" * platform (in which case most of them define underscored versions only).\n"
" */\n"
"#if defined(__GLIBC__) || defined(__OpenBSD__)\n"
"#  include <endian.h>\n"
"#elif defined(__FreeBSD__) || defined(__NetBSD__)\n"
"#  include <sys/endian.h>\n"
"#elif defined(__APPLE__)\n"
"#  include <machine/endian.h>\n"
"#elif !defined(_WIN32)\n"
"#  include <sys/param.h>\n"
"#endif\n"
"\n"
"/* Try various system- and compiler-specific byte order macro names if the\n"
" * endianness headers did not define BYTE_ORDER.\n"
" */\n"
"#if !defined(BYTE_ORDER)\n"
"#  if defined(__linux__)\n"
"#    if defined(__BYTE_ORDER)\n"
"#      define BYTE_ORDER    __BYTE_ORDER\n"
"#      define BIG_ENDIAN    __BIG_ENDIAN\n"
"#      define LITTLE_ENDIAN __LITTLE_ENDIAN\n"
"#    endif\n"
"#  elif defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)\n"
"#    if defined(_BYTE_ORDER)\n"
"#      define BYTE_ORDER    _BYTE_ORDER\n"
"#      define BIG_ENDIAN    _BIG_ENDIAN\n"
"#      define LITTLE_ENDIAN _LITTLE_ENDIAN\n"
"#    endif\n"
"#  elif defined(__APPLE__)\n"
"#    if defined(__DARWIN_BYTE_ORDER)\n"
"#      define BYTE_ORDER    __DARWIN_BYTE_ORDER\n"
"#      define BIG_ENDIAN    __DARWIN_BIG_ENDIAN\n"
"#      define LITTLE_ENDIAN __DARWIN_LITTLE_ENDIAN\n"
"#    endif\n"
"#  elif defined(_WIN32)\n"
"#    define BIG_ENDIAN    4321\n"
"#    define LITTLE_ENDIAN 1234\n"
"#    define BYTE_ORDER    LITTLE_ENDIAN\n"
"#  elif defined(__BYTE_ORDER__) &&       \\\n"
"        defined(__ORDER_BIG_ENDIAN__) && \\\n"
"        defined(__ORDER_LITTLE_ENDIAN__)\n"
"     /* GCC, Clang (and others, potentially).\n"
"      */\n"
"#    define BYTE_ORDER    __BYTE_ORDER__\n"
"#    define BIG_ENDIAN    __ORDER_BIG_ENDIAN__\n"
"#    define LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef BYTE_ORDER\n"
"#  error no byte order macros defined\n"
"#endif\n"
},

{
"HAVE_ACCEPT4", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ACCEPT4\n"
"/* OpenBSD 5.7, FreeBSD 10.0, NetBSD 8.0\n"
" * Solaris 11.4.0, glibc 2.10\n"
" * NOTE: Non-standard Linux extension\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(201505) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(10, 0)  || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(8, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 10)\n"
"#  define HAVE_ACCEPT4 1\n"
"#endif\n"
},

{
"HAVE_AES", "", "",

"\n"
"#undef HAVE_AES\n"
"\n"
"/* GCC, Clang: -maes\n"
" *\n"
" * MSVC: No controlling compiler option nor indicating macro. The\n"
" *       documentation murkily says it's always enabled on Intel and only\n"
" *       Intel processors; see:\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x64-amd64-intrinsics-list)\n"
" *\n"
" * The AES-NI instructions which are widely supported in CPUs (since 2009 for\n"
" * Intel; also `latest` ARM and SPARC processors) and most modern compilers\n"
" * (according to Wikipedia). First added to GCC 4.4.7 (2012).\n"
" */\n"
"#ifdef __AES__\n"
"#  define HAVE_AES 1\n"
"#endif\n"
},

{
"HAVE_AFUNIX_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_AFUNIX_H\n"
"\n"
"#ifdef _MSC_VER\n"
"#  include <sdkddkver.h>\n"
"#  if defined(WDK_NTDDI_VERSION) && WDK_NTDDI_VERSION >= NTDDI_WIN10_19H1\n"
"#    define HAVE_AFUNIX_H 1\n"
"#  endif\n"
"#elif BUILD2_AUTOCONF_MINGW_PREREQ(11, 0)\n"
"#  define HAVE_AFUNIX_H 1\n"
"#endif\n"
},

{
"HAVE_AI_ADDRCONFIG", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_AI_ADDRCONFIG\n"
"\n"
"/* Since Linux/glibc 2.3.4, OpenBSD >=5.6, FreeBSD >=6.3, Mac OS, Windows.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4)      || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201411)  || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(6, 3)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)          || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_AI_ADDRCONFIG 1\n"
"#endif\n"
},

{
"HAVE_ARC4RANDOM", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ARC4RANDOM\n"
"\n"
"/* Since OpenBSD 2.1, FreeBSD 2.3, NetBSD 2.0, Mac OS X 10.7\n"
" * and glibc 2.36.\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 3)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 7)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 36)\n"
"#  define HAVE_ARC4RANDOM 1\n"
"#endif\n"
},

{
"HAVE_ARC4RANDOM_ADDRANDOM", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ARC4RANDOM_ADDRANDOM\n"
"\n"
"/* Since NetBSD 2.0.\n"
" *\n"
" * It was removed from OpenBSD in version 5.5 because it 'should not be called\n"
" * directly' so treat it as not present in any version.\n"
" */\n"
"#if BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)\n"
"#  define HAVE_ARC4RANDOM_ADDRANDOM 1\n"
"#endif\n"
},

{
"HAVE_ARC4RANDOM_BUF", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ARC4RANDOM_BUF\n"
"\n"
"/* Since OpenBSD 2.1, FreeBSD 2.3, NetBSD 2.0, Mac OS X 10.7\n"
" * and glibc 2.36.\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 3)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 7)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 36)\n"
"#  define HAVE_ARC4RANDOM_BUF 1\n"
"#endif\n"
},

{
"HAVE_ARPA_INET_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ARPA_INET_H\n"
"\n"
"/* Since BSD 4.2 (1983) */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(__GLIBC__)              || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_ARPA_INET_H 1\n"
"#endif\n"
},

{
"HAVE_ATEXIT", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ATEXIT\n"
"\n"
"/* Since glibc 2.2.3, FreeBSD 1.0, OpenBSD 2.2, NetBSD 1.3, Mac OS (from BSD),\n"
" * Windows 95 at the latest.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199712) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 3)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)         || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_ATEXIT 1\n"
"#endif\n"
},

{
"HAVE_AVX", "", "",

"\n"
"#undef HAVE_AVX\n"
"\n"
"/* GCC, Clang: -mavx\n"
" *\n"
" * MSVC: /arch:{AVX,AVX2,AVX512}\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef __AVX__\n"
"#  define HAVE_AVX 1\n"
"#endif\n"
},

{
"HAVE_AVX2", "", "",

"\n"
"#undef HAVE_AVX2\n"
"\n"
"/* GCC, Clang: -mavx2\n"
" *\n"
" * MSVC: /arch:{AVX2,AVX512}\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef __AVX2__\n"
"#  define HAVE_AVX2 1\n"
"#endif\n"
},

{
"HAVE_BIGENDIAN", "", "BYTE_ORDER",

"\n"
"#ifndef BYTE_ORDER\n"
"#  error BYTE_ORDER appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_BIGENDIAN\n"
"\n"
"#if BYTE_ORDER == BIG_ENDIAN\n"
"#  define HAVE_BIGENDIAN 1\n"
"#endif\n"
},

{
"HAVE_BUILTIN_CTZ", "", "",

"\n"
"#undef HAVE_BUILTIN_CTZ\n"
"\n"
"/* __builtin_ctz (count the leading/most-significant zeros in an unsigned int)\n"
" * is available in all versions of GCC and Clang.\n"
" */\n"
"#if defined (__GNUC__) || defined(__clang__)\n"
"#  define HAVE_BUILTIN_CTZ 1\n"
"#endif\n"
},

{
"HAVE_BUILTIN_CTZL", "", "",

"\n"
"#undef HAVE_BUILTIN_CTZL\n"
"\n"
"/* __builtin_ctzl (count the leading/most-significant zeros in an unsigned\n"
" * long) is available in all versions of GCC and Clang.\n"
" */\n"
"#if defined (__GNUC__) || defined(__clang__)\n"
"#  define HAVE_BUILTIN_CTZL 1\n"
"#endif\n"
},

{
"HAVE_BUILTIN_CTZLL", "", "",

"\n"
"#undef HAVE_BUILTIN_CTZLL\n"
"\n"
"/* __builtin_ctzll (count the leading/most-significant zeros in an unsigned\n"
" * long long) is available in all versions of GCC and Clang.\n"
" */\n"
"#if defined (__GNUC__) || defined(__clang__)\n"
"#  define HAVE_BUILTIN_CTZLL 1\n"
"#endif\n"
},

{
"HAVE_CLOCK_GETTIME", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_CLOCK_GETTIME\n"
"\n"
"/* Since Linux 2.6/glibc 2.2 (based on glibc commit history), FreeBSD 3.0,\n"
" * OpenBSD 2.1, NetBSD 1.4, and Mac OSX 10.12.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 2)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 4)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 12)\n"
"#  define HAVE_CLOCK_GETTIME 1\n"
"#endif\n"
},

{
"HAVE_CLOSE_RANGE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_CLOSE_RANGE\n"
"\n"
"/* Since FreeBSD 12.2 (based on their online manual pages) and glibc 2.34.\n"
" */\n"
"#if BUILD2_AUTOCONF_FREEBSD_PREREQ(12, 2) || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 34)\n"
"#  define HAVE_CLOSE_RANGE 1\n"
"#endif\n"
},

{
"HAVE_DECL_CTL_KERN", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_DECL_CTL_KERN\n"
"\n"
"/* Since 4.4BSD (OpenBSD 2.1-6.2, FreeBSD, NetBSD, Mac OS X 10.7),\n"
" * <= glibc 2.31.\n"
" * NOTE: Appears to have been available parallel to sysctl()\n"
" */\n"
"#if (BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) && !BUILD2_AUTOCONF_OPENBSD_PREREQ(201804)) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 3)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 7)    || \\\n"
"    !BUILD2_AUTOCONF_GLIBC_PREREQ(2, 32)\n"
"#  define HAVE_DECL_CTL_KERN 1\n"
"#endif\n"
},

{
"HAVE_DECL_KERN_ARND", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_DECL_KERN_ARND\n"
"\n"
"/* OpenBSD 2.6-6.0.\n"
" * NOTE: Only mentioned in OpenBSD manual.\n"
" */\n"
"#if (BUILD2_AUTOCONF_OPENBSD_PREREQ(199912) && !BUILD2_AUTOCONF_OPENBSD_PREREQ(201609))\n"
"#  define HAVE_DECL_KERN_ARND 1\n"
"#endif\n"
},

{
"HAVE_DEVPOLL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_DEVPOLL\n"
"\n"
"/* Since Solaris version ... (?). No version details in manual.\n"
" */\n"
"#if (defined(__sun) && defined(__SVR4)) || \\\n"
"    defined(__sun__)\n"
"#  define HAVE_DEVPOLL 1\n"
"#endif\n"
},

{
"HAVE_DLOPEN", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_DLOPEN\n"
"\n"
"/* Since Linux/glibc 2.0 and all versions of other supported UNIXes.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    defined(__FreeBSD__)               || \\\n"
"    defined(__OpenBSD__)               || \\\n"
"    defined(__NetBSD__)                || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_DLOPEN 1\n"
"#endif\n"
},

{
"HAVE_EPOLL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_EPOLL\n"
"\n"
"/* Since glibc 2.3.2.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)\n"
"#  define HAVE_EPOLL 1\n"
"#endif\n"
},

{
"HAVE_EPOLL_CREATE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_EPOLL_CREATE\n"
"\n"
"/* Since glibc 2.3.2.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)\n"
"#  define HAVE_EPOLL_CREATE 1\n"
"#endif\n"
},

{
"HAVE_EPOLL_CREATE1", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_EPOLL_CREATE1\n"
"\n"
"/* Since glibc 2.9.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 9)\n"
"#  define HAVE_EPOLL_CREATE1 1\n"
"#endif\n"
},

{
"HAVE_ERRNO_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ERRNO_H\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.0, FreeBSD 1.0, NetBSD 1.0, Mac OS\n"
" * and win32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_ERRNO_H 1\n"
"#endif\n"
},

{
"HAVE_EVENTFD", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_EVENTFD\n"
"\n"
"/* Since Linux/glibc 2.8, FreeBSD 13.\n"
" *\n"
" * @@ TODO eventfd() looks likely to be added to NetBSD 10 which does not yet\n"
" *    have a release date.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 8) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(13, 0)\n"
"#  define HAVE_EVENTFD 1\n"
"#endif\n"
},

{
"HAVE_EXPLICIT_BZERO", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_EXPLICIT_BZERO\n"
"\n"
"/* Since FreeBSD 11, OpenBSD 5.5, and glibc 2.25.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 25)   || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(11, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201405)\n"
"#  define HAVE_EXPLICIT_BZERO 1\n"
"#endif\n"
},

{
"HAVE_EXPLICIT_MEMSET", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_EXPLICIT_MEMSET\n"
"\n"
"/* Since NetBSD 7.0.0.\n"
" */\n"
"#if BUILD2_AUTOCONF_NETBSD_PREREQ(7, 0)\n"
"#  define HAVE_EXPLICIT_MEMSET 1\n"
"#endif\n"
},

{
"HAVE_F16C", "", "",

"\n"
"#undef HAVE_F16C\n"
"\n"
"/* GCC, Clang: -mf16c\n"
" *\n"
" * MSVC: No controlling compiler option nor indicating macro. The\n"
" *       documentation murkily says it's always enabled on Intel and only\n"
" *       Intel processors; see:\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x64-amd64-intrinsics-list)\n"
" *\n"
" *       Instead we make use of the fact that all processors that support AVX2\n"
" *       also support F16C.\n"
" *\n"
" * Supported since Intel's Ivy Bridge (2012) and AMD's Bulldozer (2011). Added\n"
" * to GCC 4.6.4 (2013) at the latest.\n"
" */\n"
"#ifdef _MSC_VER\n"
"#  ifdef __AVX2__\n"
"#    define HAVE_F16C 1\n"
"#  endif\n"
"#else\n"
"#  ifdef __F16C__\n"
"#    define HAVE_F16C 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_FCNTL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_FCNTL\n"
"/* Since BSD 4.2 (1983)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_FCNTL 1\n"
"#endif\n"
},

{
"HAVE_FCNTL_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_FCNTL_H\n"
"\n"
"/* Note that there is <fcntl.h> on Windows (both MSVC and MinGW) though its\n"
" * declarations are not the same as on POSIX. But it does exist.\n"
" */\n"
"\n"
"#if defined(__linux__)              || \\\n"
"    defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_FCNTL_H 1\n"
"#endif\n"
},

{
"HAVE_FD_MASK", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_FD_MASK\n"
"\n"
"/* Extremely little information, other than it being very old... */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(__GLIBC__)              || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_FD_MASK 1\n"
"#endif\n"
},

{
"HAVE_FUTIMENS", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_FUTIMENS\n"
"\n"
"/* Since Linux/glibc 2.6, FreeBSD 10.3, OpenBSD 5.0, NetBSD 6.0, Mac OS 10.6\n"
" * (2009).\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 6)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(10, 3)  || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201111) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(6, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 6)\n"
"#  define HAVE_FUTIMENS 1\n"
"#endif\n"
},

{
"HAVE_FUTIMES", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_FUTIMES\n"
"\n"
"/* Since Linux/glibc 2.3, FreeBSD 3.x, OpenBSD 1.2 (but the OpenBSD macro\n"
" * was only added in 2.0 with a value of 199610), NetBSD 1.2, Mac OS (all\n"
" * versions).\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 2)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_FUTIMES 1\n"
"#endif\n"
},

{
"HAVE_GETADDRINFO", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETADDRINFO\n"
"\n"
"/* Since Linux/glibc 2.4, OpenBSD 2.9, FreeBSD 3.5, NetBSD 1.5, and Mac OS\n"
" * (hard to tell but based on the manpages available online it looks like \n"
" * Tiger at the latest).\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 5)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_GETADDRINFO 1\n"
"#endif\n"
},

{
"HAVE_GETAUXVAL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETAUXVAL\n"
"\n"
"/* Since glibc 2.16.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 16)\n"
"#  define HAVE_GETAUXVAL 1\n"
"#endif\n"
},

{
"HAVE_GETEGID", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETEGID\n"
"\n"
"/* Since Linux/glibc 2.4, OpenBSD 2.0, FreeBSD 1.0, NetBSD 1.0, and Mac OS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_GETEGID 1\n"
"#endif\n"
},

{
"HAVE_GETENTROPY", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETENTROPY\n"
"\n"
"/* Since Linux/glibc 2.25, OpenBSD 5.6, FreeBSD 12, and Mac OS 12.\n"
" *\n"
" * @@ TODO Update when NetBSD releases its support (see\n"
" *    https://mail-index.netbsd.org/tech-userlevel/2020/05/02/msg012333.html).\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 25)    || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(12, 0)  || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201411) || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 12)\n"
"#  define HAVE_GETENTROPY 1\n"
"#endif\n"
},

{
"HAVE_GETENV", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETENV\n"
"\n"
"/* Since Version 7 AT&T UNIX, 4.3BSD-Tahoe (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.09\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 9)\n"
"#  define HAVE_GETENV 1\n"
"#endif\n"
},

{
"HAVE_GETEUID", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETEUID\n"
"\n"
"/* Since Linux/glibc 2.4, OpenBSD 2.0, FreeBSD 1.0, NetBSD 1.0, and Mac OS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_GETEUID 1\n"
"#endif\n"
},

{
"HAVE_GETHOSTBYNAME_R", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETHOSTBYNAME_R\n"
"\n"
"/* Since Linux/glibc 2.1, SunOS, AIX and HPUX.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)       || \\\n"
"    defined(__SunOS) && defined(__SunOS_5_5) || \\\n"
"    defined(_AIX) || \\\n"
"    defined(__hpux)\n"
"#  define HAVE_GETHOSTBYNAME_R 1\n"
"#endif\n"
},

{
"HAVE_GETHOSTBYNAME_R_3_ARG", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETHOSTBYNAME_R_3_ARG\n"
"\n"
"/* AIX4, HPUX 10 uses 3 args\n"
" */\n"
"#if defined(_AIX) || defined(__hpux)\n"
"#  define HAVE_GETHOSTBYNAME_R_3_ARG 1\n"
"#endif\n"
},

{
"HAVE_GETHOSTBYNAME_R_5_ARG", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETHOSTBYNAME_R_5_ARG\n"
"\n"
"/* Solaris uses 5 args starting from at latest 5.5 could not find earlier.\n"
" */\n"
"#if defined(__SunOS) && defined(__SunOS_5_5)\n"
"#  define HAVE_GETHOSTBYNAME_R_5_ARG 1\n"
"#endif\n"
},

{
"HAVE_GETHOSTBYNAME_R_6_ARG", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETHOSTBYNAME_R_6_ARG\n"
"\n"
"/* Since Linux/glibc 2.1\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)\n"
"#  define HAVE_GETHOSTBYNAME_R_6_ARG 1\n"
"#endif\n"
},

{
"HAVE_GETIFADDRS", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETIFADDRS\n"
"\n"
"/* Since Linux/glibc 2.3, OpenBSD 2.9, FreeBSD 4.1, NetBSD 1.5, and Mac OS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 1)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_GETIFADDRS 1\n"
"#endif\n"
},

{
"HAVE_GETNAMEINFO", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETNAMEINFO\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.9, FreeBSD 4.0, NetBSD 1.5, and Mac OS X\n"
" * (hard to tell but based on the manpages available online it looks like\n"
" * Tiger at the latest).\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 0)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_GETNAMEINFO 1\n"
"#endif\n"
},

{
"HAVE_GETPAGESIZE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETPAGESIZE\n"
"\n"
"/* Since glibc 2.0 at the latest (but broken until 2.1), FreeBSD 2.0, OpenBSD\n"
" * 2.2, NetBSD 1.3, Mac OS (from BSD).\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199712) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 3)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_GETPAGESIZE 1\n"
"#endif\n"
},

{
"HAVE_GETPROTOBYNUMBER", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETPROTOBYNUMBER\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.0, FreeBSD 1.0, NetBSD 1.0, Mac OS\n"
" * and WIN32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_GETPROTOBYNUMBER 1\n"
"#endif\n"
},

{
"HAVE_GETRANDOM", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETRANDOM\n"
"\n"
"/* Since FreeBSD 12.0 and glibc 2.25.\n"
" */\n"
"#if BUILD2_AUTOCONF_FREEBSD_PREREQ(12, 0)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 25)\n"
"#  define HAVE_GETRANDOM 1\n"
"#endif\n"
},

{
"HAVE_GETRLIMIT", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETRLIMIT\n"
"\n"
"/* Since 4.2BSD (Linux, OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.90\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 90)\n"
"#  define HAVE_GETRLIMIT 1\n"
"#endif\n"
},

{
"HAVE_GETSERVBYNAME", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETSERVBYNAME\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.0, FreeBSD 1.0, NetBSD 1.0, Mac OS\n"
" * and WIN32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_GETSERVBYNAME 1\n"
"#endif\n"
},

{
"HAVE_GETTIMEOFDAY", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_GETTIMEOFDAY\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.0, FreeBSD 1.0, NetBSD 1.0 and Mac OS.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)|| \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_GETTIMEOFDAY 1\n"
"#endif\n"
},

{
"HAVE_IFADDRS_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_IFADDRS_H\n"
"\n"
"/* Since OpenBSD 2.1, FreeBSD 4.1, NetBSD 1.5, Mac OS X (?)\n"
" * and glibc 2.3.\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(200006) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 1)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)         || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)\n"
"#  define HAVE_IFADDRS_H 1\n"
"#endif\n"
},

{
"HAVE_INET_NTOP", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_INET_NTOP\n"
"/* Since BSD 4.2 (1983)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_INET_NTOP 1\n"
"#endif\n"
},

{
"HAVE_INET_PTON", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_INET_PTON\n"
"/* Since BSD 4.2 (1983)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_INET_PTON 1\n"
"#endif\n"
},

{
"HAVE_INOTIFY", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_INOTIFY\n"
"\n"
"/* Since Linux 2.6.3/glibc 2.4.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4)\n"
"#  define HAVE_INOTIFY 1\n"
"#endif\n"
},

{
"HAVE_INTRIN_H", "", "",

"\n"
"#undef HAVE_INTRIN_H\n"
"\n"
"/* Windows only: MSVC, MinGW-W64, and Clang.\n"
" */\n"
"#if defined(_WIN32) && \\\n"
"    (defined(_MSV_VER) || defined(__MINGW32__) || defined(__clang__))\n"
"#  define HAVE_INTRIN_H 1\n"
"#endif\n"
},

{
"HAVE_INTTYPES_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_INTTYPES_H\n"
"\n"
"/* Since OpenBSD 3.9, FreeBSD 5.0, NetBSD 5.0, Mac OS X 10.3\n"
" * glibc 2.2, Mingw-w64 2.0, Platform SDK 10.\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(200601) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 3)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(5, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 3)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 2)     || \\\n"
"    BUILD2_AUTOCONF_MINGW_PREREQ(2, 0)     || \\\n"
"    defined(_MSC_VER)\n"
"#  define HAVE_INTTYPES_H 1\n"
"#endif\n"
},

{
"HAVE_ISATTY", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ISATTY\n"
"\n"
"/* All versions of glibc, FreeBSD, OpenBSD, NetBSD, Mac OS, Windows.\n"
" */\n"
"#if defined(__GLIBC__)             || \\\n"
"    defined(__FreeBSD__)           || \\\n"
"    defined(__OpenBSD__)           || \\\n"
"    defined(__NetBSD__)            || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_ISATTY 1\n"
"#endif\n"
},

{
"HAVE_ISSETUGID", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_ISSETUGID\n"
"\n"
"/* Since OpenBSD 2.0, FreeBSD 3.0, NetBSD 1.5, Mac OS X, Solaris\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(200601) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 3)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(5, 0)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)         || \\\n"
"    ((defined(__sun) && defined(__SVR4)) || defined(__sun__))\n"
"#  define HAVE_ISSETUGID 1\n"
"#endif\n"
},

{
"HAVE_KQUEUE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_KQUEUE\n"
"\n"
"/* Since OpenBSD 2.9, FreeBSD 4.1, NetBSD 2.0, MacOS 10.0.\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 1)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)\n"
"#  define HAVE_KQUEUE 1\n"
"#endif\n"
},

{
"HAVE_LINKAT", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_LINKAT\n"
"\n"
"/* Since Linux 2.6.16/glibc 2.4, FreeBSD 8.0, OpenBSD 5.0, NetBSD 7.0 (it was\n"
" * only partially implemented in earlier versions according to the manpage),\n"
" * Mac OS (all versions).\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(8, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201111) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(7, 0)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_LINKAT 1\n"
"#endif\n"
},

{
"HAVE_MACH_ABSOLUTE_TIME", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_MACH_ABSOLUTE_TIME\n"
"\n"
"/* Mac OS X 10.0\n"
" */\n"
"#if BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)\n"
"#  define HAVE_MACH_ABSOLUTE_TIME 1\n"
"#endif\n"
},

{
"HAVE_MACH_MACH_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_MACH_MACH_H\n"
"\n"
"/* Mac OS X 10.0\n"
" */\n"
"#if BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)\n"
"#  define HAVE_MACH_MACH_H 1\n"
"#endif\n"
},

{
"HAVE_MACH_MACH_TIME_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_MACH_MACH_TIME_H\n"
"\n"
"/* Mac OS X 10.0\n"
" */\n"
"#if BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)\n"
"#  define HAVE_MACH_MACH_TIME_H 1\n"
"#endif\n"
},

{
"HAVE_MMAP", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_MMAP\n"
"\n"
"/* Note: see also HAVE_SYS_MMAN_H. */\n"
"\n"
"/* All versions of glibc, FreeBSD, OpenBSD, NetBSD, and Mac OS (from BSD).\n"
" */\n"
"#if defined(__GLIBC__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_MMAP 1\n"
"#endif\n"
},

{
"HAVE_MPROTECT", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_MPROTECT\n"
"\n"
"/* Since glibc 2.27 and all versions of FreeBSD, OpenBSD, NetBSD, and Mac OS.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 27) || \\\n"
"    defined(__FreeBSD__)                || \\\n"
"    defined(__OpenBSD__)                || \\\n"
"    defined(__NetBSD__)                 || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_MPROTECT 1\n"
"#endif\n"
},

{
"HAVE_NANOSLEEP", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_NANOSLEEP\n"
"\n"
"/* Since OpenBSD 2.1, FreeBSD 3.0, NetBSD 1.3, Mac OS X < 10.0,\n"
" * glibc 2.1 (?)\n"
" */\n"
"#if defined(__linux__)                     || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 0)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 3)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)\n"
"#  define HAVE_NANOSLEEP 1\n"
"#endif\n"
},

{
"HAVE_NEON", "", "",

"\n"
"#undef HAVE_NEON\n"
"\n"
"/* GCC, Clang: -mfpu={neon,neon-vfpv3,neon-vfpv4,neon-fp16,..}\n"
" *\n"
" * MSVC: Always enabled (and presumably defines at least one of\n"
" *       __ARM_NEON[__]).\n"
" */\n"
"#if defined(__ARM_NEON) || defined(__ARM_NEON__)\n"
"#  define HAVE_NEON 1\n"
"#endif\n"
},

{
"HAVE_NETDB_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_NETDB_H\n"
"\n"
"/* Since BSD 4.2 (1983) */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(__GLIBC__)              || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_NETDB_H 1\n"
"#endif\n"
},

{
"HAVE_NETINET_IN6_H", "", "",

"\n"
"/* Looks like netinet/in6.h only exists on System V-derived OSes like AIX,\n"
" * HP-UX, Solaris, and Illumos.\n"
" *\n"
" * On Linux the IPv6 constructs are defined in netinet/in.h and on FreeBSD,\n"
" * OpenBSD, NetBSD, and MacOS netinet/in.h includes netinet6/in6.h (note: not\n"
" * netinet/) so the latter should not be included directly.\n"
" */\n"
"#undef HAVE_NETINET_IN6_H\n"
},

{
"HAVE_NETINET_IN_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_NETINET_IN_H\n"
"\n"
"/* Since 4.2BSD\n"
"*/\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_NETINET_IN_H 1\n"
"#endif\n"
},

{
"HAVE_NETINET_TCP_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_NETINET_TCP_H\n"
"\n"
"/* Since 4.2BSD\n"
"*/\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_NETINET_TCP_H 1\n"
"#endif\n"
},

{
"HAVE_NEWLOCALE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_NEWLOCALE\n"
"\n"
"/* Note: see also HAVE_USELOCALE. */\n"
"\n"
"/* Since glibc 2.3, FreeBSD 9.1, OpenBSD 6.2, and Mac OS X (hard to tell but\n"
" * based on the manpages available online it looks like Tiger at the latest).\n"
" *\n"
" * @@ TODO Update when NetBSD adds support for newlocale. According to\n"
" *    https://man.netbsd.org/newlocale.3 it's only in CURRENT at the time of\n"
" *    writing.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(9, 1)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201710) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_NEWLOCALE 1\n"
"#endif\n"
},

{
"HAVE_PIPE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_PIPE\n"
"\n"
"/* Since Version 3 AT&T UNIX (older than 1979).\n"
" * Linux, FreeBSD, OpenBSD, NetBSD, Mac OS X, Solaris\n"
" */\n"
"#if defined(__linux__)                    || \\\n"
"    defined(__FreeBSD__)                  || \\\n"
"    defined(__OpenBSD__)                  || \\\n"
"    defined(__NetBSD__)                   || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)        || \\\n"
"    ((defined(__sun) && defined(__SVR4)) || defined(__sun__))\n"
"#  define HAVE_PIPE 1\n"
"#endif\n"
},

{
"HAVE_PIPE2", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_PIPE2\n"
"\n"
"/* Since OpenBSD 5.7, FreeBSD 10.0, NetBSD 6.0,\n"
" * glibc 2.9.\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(201505) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(10, 0)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(6, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 9)\n"
"#  define HAVE_PIPE2 1\n"
"#endif\n"
},

{
"HAVE_POLL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_POLL\n"
"\n"
"/* Since Linux 2.1.23/glibc (all versions; emulated using select() on older\n"
" * kernels), FreeBSD (all versions), OpenBSD (all versions), NetBSD 1.3, Mac\n"
" * OS (all versions).\n"
" */\n"
"#if defined(__GLIBC__)                  || \\\n"
"    defined(__FreeBSD__)                || \\\n"
"    defined(__OpenBSD__)                || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 3) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_POLL 1\n"
"#endif\n"
},

{
"HAVE_POLLTS", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_POLLTS\n"
"\n"
"/* Since NetBSD 3.0.\n"
" */\n"
"#if BUILD2_AUTOCONF_NETBSD_PREREQ(3, 0)\n"
"#  define HAVE_POLLTS 1\n"
"#endif\n"
},

{
"HAVE_POLL_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_POLL_H\n"
"\n"
"/* Since Linux 2.1.23/glibc (all versions; emulated using select() on older\n"
" * kernels), FreeBSD (all versions), OpenBSD (all versions), NetBSD 1.3, Mac\n"
" * OS (all versions).\n"
" */\n"
"#if defined(__GLIBC__)                  || \\\n"
"    defined(__FreeBSD__)                || \\\n"
"    defined(__OpenBSD__)                || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 3) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_POLL_H 1\n"
"#endif\n"
},

{
"HAVE_PORT_CREATE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_PORT_CREATE\n"
"\n"
"/* Since Solaris version ... (?). No version details in manual.\n"
" */\n"
"#if (defined(__sun) && defined(__SVR4)) || \\\n"
"    defined(__sun__)\n"
"#  define HAVE_PORT_CREATE 1\n"
"#endif\n"
},

{
"HAVE_PORT_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_PORT_H\n"
"\n"
"/* Since Solaris version ... (?). No version details in manual.\n"
" */\n"
"#if (defined(__sun) && defined(__SVR4)) || \\\n"
"    defined(__sun__)\n"
"#  define HAVE_PORT_H 1\n"
"#endif\n"
},

{
"HAVE_POSIX_FALLOCATE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_POSIX_FALLOCATE\n"
"\n"
"/* Since Linux/glibc 2.2, FreeBSD 9.0\n"
" *\n"
" * posix_fallocate(3) on Linux says since glibc 2.1.94 but use 2.2 as\n"
" * https://sourceware.org/glibc/wiki/Glibc%20Timeline does not show that\n"
" * version and the glibc macros can't check for patch versions.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 2) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(9, 0)\n"
"#  define HAVE_POSIX_FALLOCATE 1\n"
"#endif\n"
},

{
"HAVE_POSIX_MONOTONIC", "", "",

"\n"
"#undef HAVE_POSIX_MONOTONIC\n"
"\n"
"#if defined(__GLIBC__)             || \\\n"
"    defined(__FreeBSD__)           || \\\n"
"    defined(__OpenBSD__)           || \\\n"
"    defined(__NetBSD__)            || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(__MINGW32__)\n"
"#  include <unistd.h> /* _POSIX_MONOTONIC_CLOCK */\n"
"\n"
"  /* A value of 0 means it's supported for compilation while a value greater\n"
"   * than 0 means it's supported for compilation and at runtime (see\n"
"   * https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html).\n"
"   */\n"
"#  if defined(_POSIX_MONOTONIC_CLOCK) && _POSIX_MONOTONIC_CLOCK >= 0\n"
"#    define HAVE_POSIX_MONOTONIC 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_POSIX_SEM", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"/* If HAVE_POSIX_SEM is defined then all of the POSIX semaphore system calls\n"
" * such as sem_open(), sem_close(), sem_unlink(), etc. are available.\n"
" */\n"
"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_POSIX_SEM\n"
"\n"
"/* Since glibc 2.3.3 (when NPTL was added), FreeBSD 3.0, OpenBSD 3.1 (based on\n"
" * checking the online manpages by version), and NetBSD 2.0.\n"
" *\n"
" * Available on all versions of macOS.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200206) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_POSIX_SEM 1\n"
"#endif\n"
},

{
"HAVE_POSIX_SHM", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"/* If HAVE_POSIX_SHM is defined then both shm_open() and shm_unlink() are\n"
" * available.\n"
" */\n"
"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_POSIX_SHM\n"
"\n"
"/* Since glibc 2.2, FreeBSD 4.3, and NetBSD 7.0.\n"
" *\n"
" * Available on all versions of macOS.\n"
" *\n"
" * Not available on OpenBSD (security concerns).\n"
" *\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 2)   || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 3) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(7, 0)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_POSIX_SHM 1\n"
"#endif\n"
},

{
"HAVE_PPOLL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_PPOLL\n"
"\n"
"/* Since Linux 2.6.16/glibc 2.4, FreeBSD 11, OpenBSD 5.4.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4)    || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(11, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201311)\n"
"#  define HAVE_PPOLL 1\n"
"#endif\n"
},

{
"HAVE_PTHREAD", "", "",

"\n"
"/* Note that MinGW-W64 has pthread in the form of the libwinpthreads wrapper\n"
"   (though GCC may or may not be configured to use pthread). However, it's\n"
"   probably not a good idea to define this macro for MinGW-W64 since this\n"
"   may cause whomever depends on this check to use the wrapper instead of\n"
"   the native Win32 threads support. */\n"
"\n"
"#undef HAVE_PTHREAD\n"
"\n"
"#ifndef _WIN32\n"
"#  define HAVE_PTHREAD 1\n"
"#endif\n"
},

{
"HAVE_PUTENV", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_PUTENV\n"
"\n"
"/* Since AT&T System V Release 2 UNIX, 4.3BSD-Reno (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.09\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 9)\n"
"#  define HAVE_PUTENV 1\n"
"#endif\n"
},

{
"HAVE_RDRND", "", "",

"\n"
"#undef HAVE_RDRND\n"
"\n"
"/* GCC, Clang: -mrdrnd\n"
" *\n"
" * MSVC: No controlling compiler option nor indicating macro. The\n"
" *       documentation murkily says it's always enabled on Intel and only\n"
" *       Intel processors; see:\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x64-amd64-intrinsics-list)\n"
" *\n"
" *       According to Qt, all processors that support AVX2 support RDRND so\n"
" *       follow their lead by assuming it's available if AVX2 is.\n"
" *\n"
" *       Note that RDRND intrinsics were added to Visual C++ 2015 according to\n"
" *       Wikipedia.\n"
" */\n"
"#if defined(__RDRND__) || (defined(_MSC_VER) && defined(__AVX2__))\n"
"#  define HAVE_RDRND 1\n"
"#endif\n"
},

{
"HAVE_RDSEED", "", "",

"\n"
"#undef HAVE_RDSEED\n"
"\n"
"/* GCC, Clang: -mrdseed\n"
" *\n"
" * MSVC: No controlling compiler option nor indicating macro. The\n"
" *       documentation murkily says it's always enabled on Intel and only\n"
" *       Intel processors; see:\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x64-amd64-intrinsics-list)\n"
" */\n"
"#ifdef __RDSEED__\n"
"#  define HAVE_RDSEED 1\n"
"#endif\n"
},

{
"HAVE_RENAMEAT2", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_RENAMEAT2\n"
"\n"
"/* Since Linux 3.15/glibc 2.28. */\n"
"\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 28)\n"
"#  define HAVE_RENAMEAT2 1\n"
"#endif\n"
},

{
"HAVE_SA_FAMILY_T", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SA_FAMILY_T\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD >=2.7, FreeBSD >=4.0, NetBSD >=1.5, Mac OS X 10.0\n"
" * NOTE: POSIX.1g (2000) specifies this type name for the 'sa_family' member.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)      || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200015)  || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 0)    || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5)     || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)\n"
"#  define HAVE_SA_FAMILY_T 1\n"
"#endif\n"
},

{
"HAVE_SCHED_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SCHED_H\n"
"\n"
"/* Since glibc 2.0, FreeBSD 3.0, OpenBSD 4.9, NetBSD 5.0, Mac OS.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201105) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(5, 0)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SCHED_H 1\n"
"#endif\n"
},

{
"HAVE_SELECT", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SELECT\n"
"\n"
"/* Since 4.2BSD (~1983)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SELECT 1\n"
"#endif\n"
},

{
"HAVE_SENDFILE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SENDFILE\n"
"\n"
"/* Since FreeBSD 3.0, Mac OS 10.5, glibc 2.1\n"
" */\n"
"#if BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 0)   || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 5)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)\n"
"#  define HAVE_SENDFILE 1\n"
"#endif\n"
},

{
"HAVE_SETENV", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SETENV\n"
"\n"
"/* Since Version 7 AT&T UNIX, 4.3BSD-Tahoe (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.09\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 9)\n"
"#  define HAVE_SETENV 1\n"
"#endif\n"
},

{
"HAVE_SETFD", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SETFD\n"
"\n"
"/* Since SVr4, 4.3BSD, POSIX.1-2001 (only Linux mentioned but assume all\n"
" * because it's old & standardized),\n"
" * glibc 1.90\n"
" */\n"
"#if defined(__FreeBSD__)                || \\\n"
"    defined(__OpenBSD__)                || \\\n"
"    defined(__NetBSD__)                 || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)      || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 90)\n"
"#  define HAVE_SETFD 1\n"
"#endif\n"
},

{
"HAVE_SETRLIMIT", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SETRLIMIT\n"
"\n"
"/* Since 4.2BSD (Linux, OpenBSD, FreeBSD, NetBSD, Mac OS X)\n"
" * glibc 1.90\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 90)\n"
"#  define HAVE_SETRLIMIT 1\n"
"#endif\n"
},

{
"HAVE_SIGACTION", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SIGACTION\n"
"\n"
"/* Since POSIX.1 (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.09\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 9)\n"
"#  define HAVE_SIGACTION 1\n"
"#endif\n"
},

{
"HAVE_SIGNALING_NAN", "", "",

"\n"
"#undef HAVE_SIGNALING_NAN\n"
"\n"
"/* Whether implementation's double fulfills the requirements of IEC 559 which\n"
" * was released in 2008. See\n"
" * https://en.cppreference.com/w/cpp/types/numeric_limits/is_iec559.\n"
" */\n"
"#define HAVE_SIGNALING_NAN 1\n"
},

{
"HAVE_SIGNAL_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SIGNAL_H\n"
"\n"
"/* Since Version 3 AT&T UNIX (older than 1979).\n"
" * Linux, FreeBSD, OpenBSD, NetBSD, Mac OS X, Solaris\n"
" */\n"
"#if defined(__linux__)                    || \\\n"
"    defined(__FreeBSD__)                  || \\\n"
"    defined(__OpenBSD__)                  || \\\n"
"    defined(__NetBSD__)                   || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)        || \\\n"
"    ((defined(__sun) && defined(__SVR4)) || defined(__sun__))\n"
"#  define HAVE_SIGNAL_H 1\n"
"#endif\n"
},

{
"HAVE_SLEEP", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SLEEP\n"
"\n"
"/* Since Version 4/7 AT&T UNIX.\n"
" * glibc 2.1 (?)\n"
" */\n"
"#if defined(__linux__)                     || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 0)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 3)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)\n"
"#  define HAVE_SLEEP 1\n"
"#endif\n"
},

{
"HAVE_SSE", "", "",

"\n"
"#undef HAVE_SSE\n"
"\n"
"/* GCC, Clang: -msse\n"
" *\n"
" * MSVC (x86):    /arch:{SSE,SSE2,AVX,AVX2,AVX512}\n"
" * MSVC (X86-64): Always enabled\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef _MSC_VER\n"
"#  if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)\n"
"#    define HAVE_SSE 1\n"
"#  endif\n"
"#else\n"
"#  ifdef __SSE__\n"
"#    define HAVE_SSE 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_SSE2", "", "",

"\n"
"#undef HAVE_SSE2\n"
"\n"
"/* GCC, Clang: -msse2\n"
" *\n"
" * MSVC (x86):    /arch:{SSE2,AVX,AVX2,AVX512}\n"
" * MSVC (X86-64): Always enabled\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef _MSC_VER\n"
"#  if defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)\n"
"#    define HAVE_SSE2 1\n"
"#  endif\n"
"#else\n"
"#  ifdef __SSE2__\n"
"#    define HAVE_SSE2 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_SSE3", "", "",

"\n"
"#undef HAVE_SSE3\n"
"\n"
"/* GCC, Clang: -msse3\n"
" *\n"
" * MSVC: /arch:{AVX,AVX2,AVX512}\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef _MSC_VER\n"
"#  ifdef __AVX__\n"
"#    define HAVE_SSE3 1\n"
"#  endif\n"
"#else\n"
"#  ifdef __SSE3__\n"
"#    define HAVE_SSE3 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_SSE4_1", "", "",

"\n"
"#undef HAVE_SSE4_1\n"
"\n"
"/* GCC, Clang: -msse4.1\n"
" *\n"
" * MSVC: /arch:{AVX,AVX2,AVX512}\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef _MSC_VER\n"
"#  ifdef __AVX__\n"
"#    define HAVE_SSE4_1 1\n"
"#  endif\n"
"#else\n"
"#  ifdef __SSE4_1__\n"
"#    define HAVE_SSE4_1 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_SSE4_2", "", "",

"\n"
"#undef HAVE_SSE4_2\n"
"\n"
"/* GCC, Clang: -msse4.2\n"
" *\n"
" * MSVC: /arch:{AVX,AVX2,AVX512}\n"
" *\n"
" * Supported since Intel's Nehalem (2008), AMD's Bulldozer (2011), and GCC 4.3\n"
" * (2008).\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef _MSC_VER\n"
"#  ifdef __AVX__\n"
"#    define HAVE_SSE4_2 1\n"
"#  endif\n"
"#else\n"
"#  ifdef __SSE4_2__\n"
"#    define HAVE_SSE4_2 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_SSSE3", "", "",

"\n"
"#undef HAVE_SSSE3\n"
"\n"
"/* GCC, Clang: -mssse3\n"
" *\n"
" * MSVC: /arch:{AVX,AVX2,AVX512}\n"
" *\n"
" * This code is based on\n"
" * https://code.qt.io/cgit/qt/qtbase.git/tree/src/corelib/global/qsimd.h.\n"
" */\n"
"#ifdef _MSC_VER\n"
"#  ifdef __AVX__\n"
"#    define HAVE_SSSE3 1\n"
"#  endif\n"
"#else\n"
"#  ifdef __SSSE3__\n"
"#    define HAVE_SSSE3 1\n"
"#  endif\n"
"#endif\n"
},

{
"HAVE_STATX", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STATX\n"
"\n"
"/* Since Linux 4.11/glibc 2.28.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 28)\n"
"#  define HAVE_STATX 1\n"
"#endif\n"
},

{
"HAVE_STDARG_H", "", "",

"\n"
"#undef HAVE_STDARG_H\n"
"\n"
"#define HAVE_STDARG_H 1\n"
},

{
"HAVE_STDBOOL_H", "", "",

"\n"
"#undef HAVE_STDBOOL_H\n"
"\n"
"#define HAVE_STDBOOL_H 1\n"
},

{
"HAVE_STDDEF_H", "", "",

"\n"
"#undef HAVE_STDDEF_H\n"
"\n"
"#define HAVE_STDDEF_H 1\n"
},

{
"HAVE_STDINT_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STDINT_H\n"
"\n"
"/* Since Linux/glibc 2.0, OpenBSD 2.0, FreeBSD 5.0, NetBSD 1.6, Mac OS\n"
" * and win32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 6) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_STDINT_H 1\n"
"#endif\n"
},

{
"HAVE_STDLIB_H", "", "",

"\n"
"#undef HAVE_STDLIB_H\n"
"\n"
"#define HAVE_STDLIB_H 1\n"
},

{
"HAVE_STRERROR_R", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRERROR_R\n"
"\n"
"/* Since glibc 2.3.4, FreeBSD 4.4, OpenBSD 3.3, NetBSD 4.0, Mac OS (from BSD)\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 4)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200305) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(4, 0)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_STRERROR_R 1\n"
"#endif\n"
},

{
"HAVE_STRINGS_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRINGS_H\n"
"\n"
"/* Since 4.4BSD (OpenBSD 2.2, FreeBSD 1.0, NetBSD 1.3, Mac OS X 10.0),\n"
" * glibc 2.0. Note that while MinGW also provides it, its contents\n"
" * could be omitted if the NO_OLDNAMES macro is defined. So let's\n"
" * exclude MinGW for now.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199712) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 3)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)\n"
"#  define HAVE_STRINGS_H 1\n"
"#endif\n"
},

{
"HAVE_STRLCAT", "", "",

"\n"
"#undef HAVE_STRLCAT\n"
"\n"
"#if defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(__APPLE__)\n"
"#  define HAVE_STRLCAT 1\n"
"#endif\n"
},

{
"HAVE_STRLCPY", "", "",

"\n"
"#undef HAVE_STRLCPY\n"
"\n"
"#if defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(__APPLE__)\n"
"#  define HAVE_STRLCPY 1\n"
"#endif\n"
},

{
"HAVE_STRSEP", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRSEP\n"
"\n"
"/* Since 4.4BSD (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.09\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 9)\n"
"#  define HAVE_STRSEP 1\n"
"#endif\n"
},

{
"HAVE_STRTOK", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRTOK\n"
"\n"
"/* Since 4.4BSD (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 2.0\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0)\n"
"#  define HAVE_STRTOK 1\n"
"#endif\n"
},

{
"HAVE_STRTOK_R", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRTOK_R\n"
"\n"
"/* Since 4.4BSD (OpenBSD, FreeBSD, NetBSD, Mac OS X), glibc 2.0, and Mingw-w64\n"
" * 3.3.0.\n"
" */\n"
"#if defined(__FreeBSD__)               || \\\n"
"    defined(__OpenBSD__)               || \\\n"
"    defined(__NetBSD__)                || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)     || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    BUILD2_AUTOCONF_MINGW_PREREQ(3, 3)\n"
"#  define HAVE_STRTOK_R 1\n"
"#endif\n"
},

{
"HAVE_STRTOL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRTOL\n"
"\n"
"/* Since 'ISO C90' (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.90\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 90)\n"
"#  define HAVE_STRTOL 1\n"
"#endif\n"
},

{
"HAVE_STRTOLL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRTOLL\n"
"\n"
"/* Since ISO C99 (OpenBSD ~2.7, FreeBSD ~4.0, NetBSD ~1.5, Mac OS X ~10.0),\n"
" * glibc 1.90\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(200015) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 0)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 90)\n"
"#  define HAVE_STRTOLL 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_ADDRINFO", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_ADDRINFO\n"
"\n"
"/* Since Linux/glibc 2.4, OpenBSD 2.9, FreeBSD 3.5, NetBSD 1.5, and Mac OS\n"
" * (hard to tell but based on the manpages available online it looks like\n"
" * Tiger at the latest).\n"
" * NOTE: Assuming same as HAVE_GETADDRINFO\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 4)      || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106)  || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 5)    || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5)     || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_STRUCT_ADDRINFO 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_IN6_ADDR", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_IN6_ADDR\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.9, FreeBSD 3.5, NetBSD 1.5, and Mac OS X, Windows\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)      || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 5)    || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106)  || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5)     || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)          || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_STRUCT_IN6_ADDR 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_LINGER", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_LINGER\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.9, FreeBSD 3.5, NetBSD 1.5, Mac OS X\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)      || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(3, 5)    || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106)  || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5)     || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_STRUCT_LINGER 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_SOCKADDR_IN6", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_SOCKADDR_IN6\n"
"\n"
"/* Since glibc 2.1, OpenBSD ~4.2, FreeBSD ~7.0, NetBSD 2.1, Mac OS X 10.6,\n"
" * Windows\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)      || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200705)  || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(7, 0)    || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 1)     || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 6)     || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_STRUCT_SOCKADDR_IN6 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_SOCKADDR_IN6_SIN6_LEN", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_SOCKADDR_IN6_SIN6_LEN\n"
"\n"
"/* Since OpenBSD ~4.2, FreeBSD ~7.0, NetBSD 2.1, Mac OS X 10.6\n"
" */\n"
"#if BUILD2_AUTOCONF_OPENBSD_PREREQ(200705)  || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(7, 0)    || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 1)     || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 6)\n"
"#  define HAVE_STRUCT_SOCKADDR_IN6_SIN6_LEN 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_SOCKADDR_IN_SIN_LEN", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_SOCKADDR_IN_SIN_LEN\n"
"\n"
"/* Since 4.3BSD (OpenBSD, FreeBSD, NetBSD, Mac OS X)\n"
" *  'The length member, sin_len, was added with 4.3BSD-Reno [..]'\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_STRUCT_SOCKADDR_IN_SIN_LEN 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_SOCKADDR_STORAGE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_SOCKADDR_STORAGE\n"
"\n"
"/* Linux/glibc 2.1.1, Since 4.2BSD (~1983), Windows\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 2)  || \\\n"
"    defined(__FreeBSD__)                || \\\n"
"    defined(__OpenBSD__)                || \\\n"
"    defined(__NetBSD__)                 || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)      || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_STRUCT_SOCKADDR_STORAGE 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY\n"
"\n"
"/* Since Linux/glibc 2.3, OpenBSD >=2.7, FreeBSD >=4.0, NetBSD >=1.5, Mac OS X 10.0,\n"
" * Windows\n"
" *\n"
" * NOTE: RFC2553 (<1999)    - defines __ss_len & __ss_family\n"
" *       XNET draft (>1999) - defines ss_len & ss_family\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)      || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200015)  || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(4, 0)    || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 5)     || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 0)     || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_SOCKADDR_STORAGE___SS_FAMILY", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_SOCKADDR_STORAGE___SS_FAMILY\n"
"\n"
"/* Since Linux/glibc <=2.2, OpenBSD <2.7, FreeBSD <4.0, NetBSD <1.5\n"
" * NOTE: RFC2553 (<1999)    - defines __ss_len & __ss_family\n"
" *       XNET draft (>1999) - defines ss_len & ss_family\n"
" */\n"
"#if !BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)\n"
"#  define HAVE_STRUCT_SOCKADDR_STORAGE___SS_FAMILY 1\n"
"#endif\n"
},

{
"HAVE_STRUCT_SOCKADDR_UN", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_STRUCT_SOCKADDR_UN\n"
"\n"
"/* Linux/glibc 1.09, Since 4.4BSD (~1995)\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(1, 9) || \\\n"
"    defined(__FreeBSD__)                || \\\n"
"    defined(__OpenBSD__)                || \\\n"
"    defined(__NetBSD__)                 || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_STRUCT_SOCKADDR_UN 1\n"
"#endif\n"
},

{
"HAVE_SYSCONF", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYSCONF\n"
"\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYSCONF 1\n"
"#endif\n"
},

{
"HAVE_SYSCTL", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYSCTL\n"
"/* Since 4.4BSD (OpenBSD 2.1-6.2, FreeBSD, NetBSD, Mac OS X 10.7),\n"
" * <= glibc 2.31.\n"
" */\n"
"#if (BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) && !BUILD2_AUTOCONF_OPENBSD_PREREQ(201804)) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 3)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 7)    || \\\n"
"    !BUILD2_AUTOCONF_GLIBC_PREREQ(2, 32)\n"
"#  define HAVE_SYSCTL 1\n"
"#endif\n"
},

{
"HAVE_SYSV_SEM", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"/* If HAVE_SYSV_SEM is defined then all of the SYSV semaphore system calls\n"
" * semget(), semctl(), and semop() are available.\n"
" */\n"
"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYSV_SEM\n"
"\n"
"/* Since glibc 2.0, FreeBSD 2.1.0 (based on checking the online manpages by\n"
" * version), and OpenBSD 2.2 (based on checking the online manpages by\n"
" * version).\n"
" *\n"
" * Available on all versions of NetBSD and macOS.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 1)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199711) || \\\n"
"    defined(__NetBSD__)                    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYSV_SEM 1\n"
"#endif\n"
},

{
"HAVE_SYSV_SHM", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"/*\n"
" * If HAVE_SYSV_SHM is defined then all SYSV shared memory system calls\n"
" * shmget(), shmat(), shmdt(), and shmctl() are available.\n"
" */\n"
"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYSV_SHM\n"
"\n"
"/*\n"
" * Since glibc 2.0, FreeBSD 2.1.0 (based on checking the online manpages by\n"
" * version), and OpenBSD 2.2 (based on checking the online manpages by\n"
" * version).\n"
" *\n"
" * Available on all versions of NetBSD and macOS.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 1)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199711) || \\\n"
"    defined(__NetBSD__)                    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYSV_SHM 1\n"
"#endif\n"
},

{
"HAVE_SYS_EVENTFD_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_EVENTFD_H\n"
"\n"
"/* Since FreeBSD 13.0, NetBSD 10.0, glibc 2.8\n"
" */\n"
"#if BUILD2_AUTOCONF_FREEBSD_PREREQ(13, 0)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(10, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 8)\n"
"#  define HAVE_SYS_EVENTFD_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_IOCTL_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_IOCTL_H\n"
"/* Since Version 7 AT&T UNIX (1979)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_IOCTL_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_MMAN_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_MMAN_H\n"
"\n"
"/* Note: see also HAVE_MMAP. */\n"
"\n"
"/* All versions of glibc, FreeBSD, OpenBSD, NetBSD, and Mac OS (from BSD).\n"
" */\n"
"#if defined(__GLIBC__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_MMAN_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_PARAM_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_PARAM_H\n"
"/* Since 4.4BSD (~1995)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_PARAM_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_POLL_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_POLL_H\n"
"\n"
"/* Since glibc 2.0.\n"
" * FreeBSD, OpenBSD, NetBSD, Mac OS: \n"
" * All use poll.h though sys/poll.h still exists.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    defined(__FreeBSD__)               || \\\n"
"    defined(__OpenBSD__)               || \\\n"
"    defined(__NetBSD__)                || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_POLL_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_RANDOM_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_RANDOM_H\n"
"\n"
"/* FreeBSD 12.0, NetBSD 10.0 and glibc 2.25.\n"
" */\n"
"#if BUILD2_AUTOCONF_FREEBSD_PREREQ(12, 0)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(10, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 25)\n"
"#  define HAVE_SYS_RANDOM_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_RESOURCE_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_RESOURCE_H\n"
"/* Since 4.2BSD (~1983)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_RESOURCE_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_SELECT_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_SELECT_H\n"
"\n"
"/* Since 4.2BSD (~1983)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_RESOURCE_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_SENDFILE_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_SENDFILE_H\n"
"\n"
"/* Since glibc 2.1.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)\n"
"#  define HAVE_SYS_SENDFILE_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_SOCKET_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_SOCKET_H\n"
"/* Since 4.2BSD (~1983)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_SOCKET_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_STAT_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_STAT_H\n"
"/* Since 4.2BSD (~1983)\n"
" */\n"
"#if defined(__linux__)              || \\\n"
"    defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_SYS_STAT_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_SYSCTL_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_SYSCTL_H\n"
"\n"
"/* Since 4.4BSD (OpenBSD 2.1-6.2, FreeBSD, NetBSD, Mac OS X 10.7),\n"
" * <= glibc 2.31.\n"
" */\n"
"#if (BUILD2_AUTOCONF_OPENBSD_PREREQ(199706) && !BUILD2_AUTOCONF_OPENBSD_PREREQ(201804)) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 3)   || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(2, 0)    || \\\n"
"    BUILD2_AUTOCONF_MACOS_PREREQ(10, 7)    || \\\n"
"    !BUILD2_AUTOCONF_GLIBC_PREREQ(2, 32)\n"
"#  define HAVE_SYS_SYSCTL_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_TIMERFD_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_TIMERFD_H\n"
"\n"
"/* Since NetBSD 10.0, glibc 2.8.\n"
" */\n"
"#if BUILD2_AUTOCONF_NETBSD_PREREQ(10, 0)    || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 8)\n"
"#  define HAVE_SYS_TIMERFD_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_TIME_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_TIME_H\n"
"\n"
"/* Since 4.2BSD (OpenBSD, FreeBSD, Mac OS X), NetBSD 6.0\n"
" */\n"
"#if defined(__linux__)                      || \\\n"
"    defined(__FreeBSD__)                    || \\\n"
"    defined(__OpenBSD__)                    || \\\n"
"    defined(__NetBSD__)                     || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)          || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(6, 0)\n"
"#  define HAVE_SYS_TIME_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_TYPES_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_TYPES_H\n"
"/* Linux only (?) */\n"
"#if defined(__linux__)\n"
"#  define HAVE_SYS_TYPES_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_UIO_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_UIO_H\n"
"\n"
"/* Since 4.2BSD (Linux, FreeBSD, OpenBSD, NetBSD, Mac OS X)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_UIO_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_UN_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_UN_H\n"
"\n"
"/* Since 4.3BSD (?)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_UN_H 1\n"
"#endif\n"
},

{
"HAVE_SYS_WAIT_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_SYS_WAIT_H\n"
"/* Since 4.3BSD (~1994)\n"
" */\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_SYS_WAIT_H 1\n"
"#endif\n"
},

{
"HAVE_THREADSAFE_CLOEXEC", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_THREADSAFE_CLOEXEC\n"
"\n"
"/* Define if file descriptor-creating functions taking a *_CLOEXEC flag are\n"
" * available. Setting the FD_CLOEXEC flag in a separate fcntl() call opens a\n"
" * window for another thread to fork/exec.\n"
" *\n"
" * The set of functions covered by this check are pipe2(), dup3(), accept4(),\n"
" * and a version of socket() which takes the SOCK_CLOEXEC flag.\n"
" *\n"
" * Since glibc 2.10, FreeBSD 10, OpenBSD 5.7, NetBSD 6.0.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 10)    || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(10, 0)  || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201507) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(6, 0)\n"
"#  define HAVE_THREADSAFE_CLOEXEC 1\n"
"#endif\n"
},

{
"HAVE_TIMERADD", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_TIMERADD\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.9, FreeBSD 5.0, NetBSD 6.0 and Mac OS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(6, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_TIMERADD 1\n"
"#endif\n"
},

{
"HAVE_TIMERCLEAR", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_TIMERCLEAR\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.9, FreeBSD 5.0, NetBSD 6.0 and Mac OS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(6, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_TIMERCLEAR 1\n"
"#endif\n"
},

{
"HAVE_TIMERFD_CREATE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_TIMERFD_CREATE\n"
"\n"
"/* Since Linux/glibc 2.8 and Mac OS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 8) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_TIMERFD_CREATE 1\n"
"#endif\n"
},

{
"HAVE_TIMERISSET", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_TIMERISSET\n"
"\n"
"/* Since Linux/glibc 2.1, OpenBSD 2.9, FreeBSD 5.0, NetBSD 6.0 and Mac OS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(6, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_TIMERISSET 1\n"
"#endif\n"
},

{
"HAVE_UINT16_T", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UINT16_T\n"
"\n"
"/* Since Linux/glibc 2.0, OpenBSD 2.0, FreeBSD 5.0, NetBSD 1.6, Mac OS\n"
" * and win32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 6) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_UINT16_T 1\n"
"#endif\n"
},

{
"HAVE_UINT32_T", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UINT32_T\n"
"\n"
"/* Since Linux/glibc 2.0, OpenBSD 2.0, FreeBSD 5.0, NetBSD 1.6, Mac OS\n"
" * and win32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 6) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_UINT32_T 1\n"
"#endif\n"
},

{
"HAVE_UINT64_T", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UINT64_T\n"
"\n"
"/* Since Linux/glibc 2.0, OpenBSD 2.0, FreeBSD 5.0, NetBSD 1.6, Mac OS\n"
" * and win32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 6) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_UINT64_T 1\n"
"#endif\n"
},

{
"HAVE_UINT8_T", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UINT8_T\n"
"\n"
"/* Since Linux/glibc 2.0, OpenBSD 2.0, FreeBSD 5.0, NetBSD 1.6, Mac OS\n"
" * and win32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 6) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_UINT8_T 1\n"
"#endif\n"
},

{
"HAVE_UINTPTR_T", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UINTPTR_T\n"
"\n"
"/* Since Linux/glibc 2.0, OpenBSD 2.0, FreeBSD 5.0, NetBSD 1.6, Mac OS\n"
" * and win32.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 0) || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(5, 0) || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 6) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS) || \\\n"
"    defined(_WIN32)\n"
"#  define HAVE_UINTPTR_T 1\n"
"#endif\n"
},

{
"HAVE_UMASK", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UMASK\n"
"\n"
"/*  Since glibc 2.1, FreeBSD 1.0, OpenBSD 2.0, NetBSD 1.0 and MacOS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)    || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(1, 0)  || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(199610) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 0) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_UMASK 1\n"
"#endif\n"
},

{
"HAVE_UNISTD_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UNISTD_H\n"
"\n"
"#if defined(__linux__)   || \\\n"
"    defined(__FreeBSD__) || \\\n"
"    defined(__OpenBSD__) || \\\n"
"    defined(__NetBSD__)  || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_UNISTD_H 1\n"
"#endif\n"
},

{
"HAVE_UNSETENV", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_UNSETENV\n"
"\n"
"/* Since Version 7 AT&T UNIX, 4.3BSD-Tahoe (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 1.90\n"
" */\n"
"#if defined(__FreeBSD__)            || \\\n"
"    defined(__OpenBSD__)            || \\\n"
"    defined(__NetBSD__)             || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)  || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(1, 90)\n"
"#  define HAVE_UNSETENV 1\n"
"#endif\n"
},

{
"HAVE_USELOCALE", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_USELOCALE\n"
"\n"
"/* Note: see also HAVE_NEWLOCALE. */\n"
"\n"
"/* Since glibc 2.3, FreeBSD 9.1, OpenBSD 6.2, and Mac OS X (hard to tell but\n"
" * based on the manpages available online it looks like Tiger at the latest).\n"
" *\n"
" * @@ TODO Update when NetBSD adds support for uselocale. newlocale at least\n"
" *    is in CURRENT already.\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 3)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(9, 1)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(201710) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_USELOCALE 1\n"
"#endif\n"
},

{
"HAVE_USLEEP", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_USLEEP\n"
"\n"
"/* Since 4.3BSD (OpenBSD, FreeBSD, NetBSD, Mac OS X),\n"
" * glibc 2.1 (?)\n"
" */\n"
"#if defined(__FreeBSD__)                || \\\n"
"    defined(__OpenBSD__)                || \\\n"
"    defined(__NetBSD__)                 || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)      || \\\n"
"    BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)\n"
"#  define HAVE_USLEEP 1\n"
"#endif\n"
},

{
"HAVE_VASPRINTF", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_VASPRINTF\n"
"\n"
"/*  Since glibc 2.1, FreeBSD 2.2, OpenBSD 2.9, NetBSD 1.4 and MacOS\n"
" */\n"
"#if BUILD2_AUTOCONF_GLIBC_PREREQ(2, 1)     || \\\n"
"    BUILD2_AUTOCONF_FREEBSD_PREREQ(2, 2)   || \\\n"
"    BUILD2_AUTOCONF_OPENBSD_PREREQ(200106) || \\\n"
"    BUILD2_AUTOCONF_NETBSD_PREREQ(1, 4)    || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_VASPRINTF 1\n"
"#endif\n"
},

{
"HAVE_XLOCALE_H", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE_XLOCALE_H\n"
"\n"
"#if BUILD2_AUTOCONF_FREEBSD_PREREQ(9, 1) || \\\n"
"    defined(BUILD2_AUTOCONF_MACOS)\n"
"#  define HAVE_XLOCALE_H 1\n"
"#endif\n"
},

{
"HAVE_XOP", "", "",

"\n"
"#undef HAVE_XOP\n"
"\n"
"/* GCC, Clang: -mxop\n"
" *\n"
" * MSVC: No controlling compiler option nor indicating macro. The\n"
" *       documentation murkily says it's always enabled on AMD and only AMD\n"
" *       processors; see:\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x86-intrinsics-list\n"
" *       https://docs.microsoft.com/en-us/cpp/intrinsics/x64-amd64-intrinsics-list)\n"
" */\n"
"#ifdef __XOP__\n"
"#  define HAVE_XOP 1\n"
"#endif\n"
},

{
"HAVE__GMTIME", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE__GMTIME\n"
"\n"
"/* Win32\n"
" */\n"
"#if defined(_WIN32)\n"
"#  define HAVE__GMTIME 1\n"
"#endif\n"
},

{
"HAVE__GMTIME32", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE__GMTIME32\n"
"\n"
"/* Win32\n"
" */\n"
"#if defined(_WIN32)\n"
"#  define HAVE__GMTIME32 1\n"
"#endif\n"
},

{
"HAVE__GMTIME32_S", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE__GMTIME32_S\n"
"\n"
"/* Win32\n"
" */\n"
"#if defined(_WIN32)\n"
"#  define HAVE__GMTIME32_S 1\n"
"#endif\n"
},

{
"HAVE__GMTIME64", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE__GMTIME64\n"
"\n"
"/* Win32\n"
" */\n"
"#if defined(_WIN32)\n"
"#  define HAVE__GMTIME64 1\n"
"#endif\n"
},

{
"HAVE__GMTIME64_S", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE__GMTIME64_S\n"
"\n"
"/* Win32\n"
" */\n"
"#if defined(_WIN32)\n"
"#  define HAVE__GMTIME64_S 1\n"
"#endif\n"
},

{
"HAVE__GMTIME_S", "", "BUILD2_AUTOCONF_LIBC_VERSION",

"\n"
"#ifndef BUILD2_AUTOCONF_LIBC_VERSION\n"
"#  error BUILD2_AUTOCONF_LIBC_VERSION appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef HAVE__GMTIME_S\n"
"\n"
"/* Win32\n"
" */\n"
"#if defined(_WIN32)\n"
"#  define HAVE__GMTIME_S 1\n"
"#endif\n"
},

{
"HAVE___FUNCTION__", "", "",

"\n"
"#undef HAVE___FUNCTION__\n"
"\n"
"/* __FUNCTION__ is another name for __func__, provided for backward\n"
" * compatibility with old versions of GCC. Although it is seemingly\n"
" * available everywhere else.\n"
" */\n"
"#define HAVE___FUNCTION__ 1\n"
},

{
"HAVE___func__", "", "",

"\n"
"#undef HAVE___func__\n"
"\n"
"/* __func__ was added to the standard in C99 and C++11. But seemingly exists\n"
" * almost everywhere before that as well.\n"
" */\n"
"\n"
"#define HAVE___func__ 1\n"
},

{
"SIZEOF_INT", "", "",

"\n"
"#undef SIZEOF_INT\n"
"\n"
"#ifdef _MSC_VER\n"
"#  define SIZEOF_INT 4\n"
"#else\n"
"/* Both GCC and Clang (and maybe others) define __SIZEOF_INT__. */\n"
"#  ifdef __SIZEOF_INT__\n"
"#    define SIZEOF_INT __SIZEOF_INT__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef SIZEOF_INT\n"
"#  error unable to determine size of int\n"
"#endif\n"
},

{
"SIZEOF_LONG", "", "",

"\n"
"#undef SIZEOF_LONG\n"
"\n"
"#ifdef _MSC_VER\n"
"#  define SIZEOF_LONG 4\n"
"#else\n"
"/* Both GCC and Clang (and maybe others) define __SIZEOF_LONG__. */\n"
"#  ifdef __SIZEOF_LONG__\n"
"#    define SIZEOF_LONG __SIZEOF_LONG__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef SIZEOF_LONG\n"
"#  error unable to determine size of long int\n"
"#endif\n"
},

{
"SIZEOF_LONG_LONG", "", "",

"\n"
"#undef SIZEOF_LONG_LONG\n"
"\n"
"#ifdef _MSC_VER\n"
"#  define SIZEOF_LONG_LONG 8\n"
"#else\n"
"/* Both GCC and Clang (and maybe others) define __SIZEOF_LONG_LONG__. */\n"
"#  ifdef __SIZEOF_LONG_LONG__\n"
"#    define SIZEOF_LONG_LONG __SIZEOF_LONG_LONG__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef SIZEOF_LONG_LONG\n"
"#  error unable to determine size of long long\n"
"#endif\n"
},

{
"SIZEOF_OFF_T", "", "",

"\n"
"#undef SIZEOF_OFF_T\n"
"\n"
"\n"
"/* Use __SIZEOF_SIZE_T__ (defined by GCC and Clang and maybe others) to detect\n"
" * a 64-bit target.\n"
" *\n"
" * Note that the _FILE_OFFSET_BITS macro is expected to be defined by the\n"
" * user, for example, on the command line.\n"
" *\n"
" * MinGW uses 32-bit off_t both in the 32-bit and 64-bit modes unless forced\n"
" * with _FILE_OFFSET_BITS.\n"
" */\n"
"\n"
"#ifdef _MSC_VER\n"
"#  define SIZEOF_OFF_T 4\n"
"#elif defined(__MINGW32__)\n"
"#  if defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64\n"
"#    define SIZEOF_OFF_T 8\n"
"#  else\n"
"#    define SIZEOF_OFF_T 4\n"
"#  endif\n"
"#elif (defined(__SIZEOF_SIZE_T__) && __SIZEOF_SIZE_T__ == 8) || \\\n"
"      (defined(_FILE_OFFSET_BITS) && _FILE_OFFSET_BITS == 64)\n"
"#  define SIZEOF_OFF_T 8\n"
"#else\n"
"#  define SIZEOF_OFF_T 4\n"
"#endif\n"
},

{
"SIZEOF_POINTER", "", "",

"\n"
"/* See also SIZEOF_SIZE_T (the two may not be the same), SIZEOF_VOID_P. */\n"
"\n"
"#undef SIZEOF_POINTER\n"
"\n"
"#ifdef _MSC_VER\n"
"/* _WIN64 is defined for both x64 (x86_64) and ARM64 (aarch64). */\n"
"#  ifdef _WIN64\n"
"#    define SIZEOF_POINTER 8\n"
"#  else\n"
"#    define SIZEOF_POINTER 4\n"
"#  endif\n"
"#else\n"
"/* Both GCC and Clang (and maybe others) define __SIZEOF_POINTER__. */\n"
"#  ifdef __SIZEOF_POINTER__\n"
"#    define SIZEOF_POINTER __SIZEOF_POINTER__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef SIZEOF_POINTER\n"
"#  error unable to determine size of pointer\n"
"#endif\n"
},

{
"SIZEOF_PTHREAD_T", "", "",

"\n"
"#undef SIZEOF_PTHREAD_T\n"
"\n"
"/* Using the size of pointer appears to be a reasonable heuristics and\n"
" *  __SIZEOF_POINTER__ is availbale everywhere where we have pthread (see\n"
" *  SIZEOF_POINTER.h for details). Note also that we have a test that verifies\n"
" *  this assumption.\n"
" */\n"
"#ifndef _MSC_VER\n"
"#  ifdef __SIZEOF_POINTER__\n"
"#    define SIZEOF_PTHREAD_T __SIZEOF_POINTER__\n"
"#  endif\n"
"\n"
"#  ifndef SIZEOF_PTHREAD_T\n"
"#    error unable to determine size of pthread_t\n"
"#  endif\n"
"#endif\n"
},

{
"SIZEOF_SHORT", "", "",

"\n"
"#undef SIZEOF_SHORT\n"
"\n"
"#ifdef _MSC_VER\n"
"#  define SIZEOF_SHORT 2\n"
"#else\n"
"/* Both GCC and Clang (and maybe others) define __SIZEOF_SHORT__. */\n"
"#  ifdef __SIZEOF_SHORT__\n"
"#    define SIZEOF_SHORT __SIZEOF_SHORT__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef SIZEOF_SHORT\n"
"#  error unable to determine size of short\n"
"#endif\n"
},

{
"SIZEOF_SIZE_T", "", "",

"\n"
"/* See also SIZEOF_POINTER (the two may not be the same). */\n"
"\n"
"#undef SIZEOF_SIZE_T\n"
"\n"
"#ifdef _MSC_VER\n"
"/* _WIN64 is defined for both x64 (x86_64) and ARM64 (aarch64). */\n"
"#  ifdef _WIN64\n"
"#    define SIZEOF_SIZE_T 8\n"
"#  else\n"
"#    define SIZEOF_SIZE_T 4\n"
"#  endif\n"
"#else\n"
"/* Both GCC and Clang (and maybe others) define __SIZEOF_SIZE_T__. */\n"
"#  ifdef __SIZEOF_SIZE_T__\n"
"#    define SIZEOF_SIZE_T __SIZEOF_SIZE_T__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef SIZEOF_SIZE_T\n"
"#  error unable to determine size of size_t\n"
"#endif\n"
},

{
"SIZEOF_VOID_P", "", "",

"\n"
"/* See also SIZEOF_SIZE_T (the two may not be the same), SIZEOF_POINTER. */\n"
"\n"
"#undef SIZEOF_VOID_P\n"
"\n"
"#ifdef _MSC_VER\n"
"/* _WIN64 is defined for both x64 (x86_64) and ARM64 (aarch64). */\n"
"#  ifdef _WIN64\n"
"#    define SIZEOF_VOID_P 8\n"
"#  else\n"
"#    define SIZEOF_VOID_P 4\n"
"#  endif\n"
"#else\n"
"/* Both GCC and Clang (and maybe others) define __SIZEOF_POINTER__. */\n"
"#  ifdef __SIZEOF_POINTER__\n"
"#    define SIZEOF_VOID_P __SIZEOF_POINTER__\n"
"#  endif\n"
"#endif\n"
"\n"
"#ifndef SIZEOF_VOID_P\n"
"#  error unable to determine size of pointer\n"
"#endif\n"
},

{
"STDC_HEADERS", "", "",

"\n"
"#undef STDC_HEADERS\n"
"\n"
"#define STDC_HEADERS 1\n"
},

{
"STRERROR_R_CHAR_P", "", "",

"\n"
"#undef STRERROR_R_CHAR_P\n"
"\n"
"/* GNU-specific.\n"
" *\n"
" * Note that while the HAVE_STRERROR_R_CHAR_P would have been the better\n"
" * name (since we are testing for presence/absence), this name is used by\n"
" * the autoconf's AC_FUNC_STRERROR_R test.\n"
" */\n"
"#ifdef __GLIBC__\n"
"#  if defined(_GNU_SOURCE) || (_POSIX_C_SOURCE < 200112L)\n"
"#    define STRERROR_R_CHAR_P 1\n"
"#  endif\n"
"#endif\n"
},

{
"WORDS_BIGENDIAN", "", "BYTE_ORDER",

"\n"
"#ifndef BYTE_ORDER\n"
"#  error BYTE_ORDER appears to be conditionally included\n"
"#endif\n"
"\n"
"#undef WORDS_BIGENDIAN\n"
"\n"
"#if BYTE_ORDER == BIG_ENDIAN\n"
"#  define WORDS_BIGENDIAN 1\n"
"#endif\n"
},

{
"const", "!", "",

"\n"
"/* This check is unusual in that for a positive answer (the compiler supports\n"
"   const) we undefine const to allow the compiler to recognize it as a keyword\n"
"   and for a negative answer we define it to nothing so that const is\n"
"   effectively removed from the user code. */\n"
"\n"
"#undef const\n"
},

{
"inline", "!", "",

"\n"
"/* This check is unusual in that for a positive answer (the compiler supports\n"
"   inline) we undefine inline to allow the compiler to recognize it as a\n"
"   keyword and for a negative answer we define it to nothing so that inline is\n"
"   effectively removed from the user code. */\n"
"\n"
"#undef inline\n"
},

{
"socklen_t", "!", "",

"\n"
"/* If socklen_t is already defined, assume it's correct and use it as-is (see\n"
" * ssize_t for details).\n"
" */\n"
"#ifndef socklen_t\n"
"\n"
"/* Since 4.xBSD, SunOS\n"
" * The Single UNIX Specification, Version 2\n"
" *   <sys/socket.h> makes available a type, socklen_t, which is\n"
" *   an unsigned opaque integral type of length of at least 32 bits.\n"
" *   To forestall portability problems, it is recommended that\n"
" *   applications should not use values larger than 232 - 1.\n"
" */\n"
"#  if defined(__linux__)                  || \\\n"
"      defined(__FreeBSD__)                || \\\n"
"      defined(__OpenBSD__)                || \\\n"
"      defined(__NetBSD__)                 || \\\n"
"      defined(__APPLE__)                  || \\\n"
"      (defined(__sun) && defined(__SVR4)) || \\\n"
"      defined(__CYGWIN__)\n"
"#     include <sys/socket.h>\n"
"      /* If available, we do nothing. */\n"
"#  elif defined(_WIN32)\n"
"      /* While socklen_t is declared in <ws2tcpip.h>, including this header is\n"
"       * hazardous due to it being sensitive to WIN32_LEAN_AND_MEAN (and who\n"
"       * knows what else). At the same time, in both Platform SDK and MinGW,\n"
"       * this is a simple int typedef and so just doing that feels like the\n"
"       * simplest (if hackish) way to sidestep the whole mess.\n"
"       */\n"
"      typedef int socklen_t;\n"
"#  else\n"
"      /* Else define it to unsigned int (suggested fallback by libevent). */\n"
"#     define socklen_t unsigned int\n"
"#  endif\n"
"#endif\n"
},

{
"ssize_t", "!", "",

"\n"
"/* If ssize_t is already defined, assume it's correct and use it as-is.\n"
" *\n"
" * Note that we may not be able to redefine it because while we can undef the\n"
" * macro, there is no guarantee we will be able to re-include the header (due\n"
" * to include guards).\n"
" */\n"
"#ifndef ssize_t\n"
"  /* POSIX and MinGW (which also has <sys/types.h> that defines ssize_t).\n"
"   */\n"
"#  if defined(__linux__)                  || \\\n"
"      defined(__FreeBSD__)                || \\\n"
"      defined(__OpenBSD__)                || \\\n"
"      defined(__NetBSD__)                 || \\\n"
"      defined(__APPLE__)                  || \\\n"
"      defined(__MINGW32__)                || \\\n"
"      (defined(__sun) && defined(__SVR4)) || \\\n"
"      defined(__CYGWIN__)\n"
"#    include <sys/types.h>\n"
"     /* If available, we do nothing. */\n"
"#  elif defined(_WIN32)\n"
"#    include <basetsd.h>\n"
"     /* Use define instead of typedef to avoid conflicts. */\n"
"#    define ssize_t SSIZE_T\n"
"#  else\n"
"     /* Else define it to int (suggested fallback by libevent). */\n"
"#    define ssize_t int\n"
"#  endif\n"
"#endif\n"
},

{
"volatile", "!", "",

"\n"
"/* This check is unusual in that for a positive answer (the compiler supports\n"
"   volatile) we undefine volatile to allow the compiler to recognize it as a\n"
"   keyword and for a negative answer we define it to nothing so that volatile\n"
"   is effectively removed from the user code. */\n"
"\n"
"#undef volatile\n"
},

{
"zzz_TEST_DUMMY1_H", "", "",

"#define zzz_TEST_DUMMY1_H 1\n"
},

{
"zzz_TEST_DUMMY2_H", "", "",

"#define zzz_TEST_DUMMY2_H 1\n"
},

{
"zzz_TEST_DUMMY3_H", "!", "",

"#define zzz_TEST_DUMMY3_H 1\n"
},

{
"zzz_TEST_DUMMY4_H", "", "zzz_TEST_DUMMY1_H zzz_TEST_DUMMY3_H",

"#ifdef zzz_TEST_DUMMY1_H\n"
"#  define zzz_TEST_DUMMY4_H zzz_TEST_DUMMY3_H\n"
"#endif /*zzz_TEST_DUMMY1_H*/\n"
},

{
"zzz_TEST_DUMMY5_H", "", "zzz_TEST_DUMMY3_H zzz_TEST_DUMMY4_H",

"#ifdef zzz_TEST_DUMMY3_H\n"
"#  define zzz_TEST_DUMMY5_H zzz_TEST_DUMMY4_H\n"
"#endif /*zzz_TEST_DUMMY3_H*/\n"
}
};
