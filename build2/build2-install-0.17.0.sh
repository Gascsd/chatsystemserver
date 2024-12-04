#!/bin/sh

# file      : build2-install.sh
# license   : MIT; see the build2-toolchain/LICENSE file for details

usage="Usage: $0 [-h|--help] [<options>] [<install-dir>]"

ver="0.17.0"
type="public"

build2_ver="0.17.0"
bpkg_ver="0.17.0"
bdep_ver="0.17.0"

# Standard modules comma-separated list and versions.
#
# NOTE: we currently print the list as a single line and will need to somehow
# change that when it becomes too long.
#
standard_modules="autoconf, kconfig"
autoconf_ver="0.3.0"
kconfig_ver="0.3.0"

url="https://download.build2.org"
repo="https://pkg.cppget.org/1/alpha"

toolchain="build2-toolchain-0.17.0"
toolchain_sum_xz="3722a89ea86df742539d0f91bb4429fd46bbf668553a350780a63411b648bf5d"
toolchain_sum_gz="5c7d4a797c252e4516d1e913e50d6cf4d461d340d1e4fa295c878667e6b8bab0"

tdir="$toolchain"

cver="0.17"
cdir="build2-toolchain-$cver"

pcver="0.16"   # Empty if no upgrade is possible.
pcdir="build2-toolchain-$pcver"

manifest="toolchain.sha256"
stem="build2-install"

owd="$(pwd)"
prog="$0"

fail ()
{
  cd "$owd"
  exit 1
}

diag ()
{
  echo "$*" 1>&2
}

error ()
{
  diag "error: $*"
  fail
}

# Note that this function will execute a command with arguments that contain
# spaces but it will not print them as quoted (and neither does set -x).
#
run ()
{
  diag "+ $@"
  "$@"
  if test "$?" -ne "0"; then
    fail
  fi
}

# Check whether the specified command exists.
#
check_cmd () # <cmd> [<hint>]
{
  if ! command -v "$1" >/dev/null 2>&1; then
    diag "error: unable to execute $1: command not found"
    if test -n "$2"; then
      diag "  info: $2"
    fi
    fail
  fi
}

mode=install

yes=
local=
bpkg_install=true
bdep_install=true
modules="$standard_modules"
system=
exe_prefix=
exe_suffix=
stage_suffix="-stage"
private=
cxx=
cxx_name="system-default C++ compiler"
debug=
debug_optimized=
sudo=
jobs=
trust=
check=true
timeout=600
connect_timeout=60

idir=

while test $# -ne 0; do
  case "$1" in
    -h|--help)
      diag
      diag "$usage"
      diag "Options:"
      diag "  --yes                 Do not ask for confirmation before starting."
      diag "  --local               Don't build from packages, only from local source."
      diag "  --no-bpkg             Don't install bpkg nor bdep (requires --local)."
      diag "  --no-bdep             Don't install bdep."
      diag "  --no-modules          Don't install standard build system modules."
      diag "  --modules <list>      Install only specified standard build system modules."
      diag "  --system <list>       Use system-installed versions of specified dependencies."
      diag "  --exe-prefix <pfx>    Toolchain executables name prefix."
      diag "  --exe-suffix <sfx>    Toolchain executables name suffix."
      diag "  --stage-suffix <sfx>  Staged executables name suffix ('-stage' by default)."
      diag "  --cxx <prog>          Alternative C++ compiler to use."
      diag "  --debug               Build with debug information."
      diag "  --debug-optimized     Build with debug information and optimization."
      diag "  --sudo <prog>         Alternative sudo program to use (pass false to disable)."
      diag "  --private             Install shared libraries into private subdirectory."
      diag "  --jobs|-j <num>       Number of jobs to perform in parallel."
      diag "  --trust <fp>          Repository certificate fingerprint to trust."
      diag "  --timeout <sec>       Network operations timeout in seconds (600 by default)."
      diag "  --no-check            Do not check for a new script version."
      diag "  --upgrade             Upgrade previously installed toolchain."
      diag "  --uninstall           Uninstall previously installed toolchain."
      diag
      diag "By default this script will use /usr/local as the installation directory"
      diag "and the current working directory as the build directory."
      diag
      diag "If the installation directory requires root permissions, sudo(1) will"
      diag "be used by default."
      diag
      diag "If --jobs|-j is unspecified, then the number of available hardware"
      diag "threads is used."
      diag
      diag "The --trust option recognizes two special values: 'yes' (trust"
      diag "everything) and 'no' (trust nothing)."
      diag
      diag "Use --system <list> to specify a comma-separated list of dependencies"
      diag "to use from the system rather than building them from source. Currently,"
      diag "only libsqlite3 and libpkgconf can be specified with this option and"
      diag "using either from the system will likely result in limited functionality."
      diag "For example:"
      diag
      diag "$0 --system libsqlite3,libpkgconf"
      diag
      diag "The script by default installs the following standard build system"
      diag "modules:"
      diag
      diag "$standard_modules"
      diag
      diag "Use --no-modules to suppress installing build system modules or"
      diag "--modules <list> to specify a comma-separated subset to install."
      diag
      diag "Note also that <options> must come before the <install-dir> argument."
      diag
      exit 0
      ;;
    --upgrade)
      mode=upgrade
      shift
      ;;
    --uninstall)
      mode=uninstall
      shift
      ;;
    --yes)
      yes=true
      shift
      ;;
    --local)
      local=true
      shift
      ;;
    --no-bpkg)
      bpkg_install=
      shift
      ;;
    --no-bdep)
      bdep_install=
      shift
      ;;
    --no-modules)
      modules=
      shift
      ;;
    --modules)
      shift
      modules="$1"
      shift
      ;;
    --system)
      shift
      system="$1"
      shift
      ;;
    --private)
      private=true
      shift
      ;;
    --exe-prefix)
      shift
      if test $# -eq 0; then
        error "executables name prefix expected after --exe-prefix; run $prog -h for details"
      fi
      exe_prefix="$1"
      shift
      ;;
    --exe-suffix)
      shift
      if test $# -eq 0; then
        error "executables name suffix expected after --exe-suffix; run $prog -h for details"
      fi
      exe_suffix="$1"
      shift
      ;;
    --stage-suffix)
      shift
      if test $# -eq 0; then
        error "staged executables name suffix expected after --stage-suffix; run $prog -h for details"
      fi
      stage_suffix="$1"
      shift
      ;;
    --cxx)
      shift
      if test $# -eq 0; then
        error "C++ compiler expected after --cxx; run $prog -h for details"
      fi
      cxx="$1"
      cxx_name="$1"
      shift
      ;;
    --debug)
      debug=true
      shift
      ;;
    --debug-optimized)
      debug_optimized=true
      shift
      ;;
    --sudo)
      shift
      if test $# -eq 0; then
        error "sudo program expected after --sudo; run $prog -h for details"
      fi
      sudo="$1"
      shift
      ;;
    -j|--jobs)
      shift
      if test $# -eq 0; then
        error "number of jobs expected after --jobs|-j; run $prog -h for details"
      fi
      jobs="$1"
      shift
      ;;
    --trust)
      shift
      if test $# -eq 0; then
        error "certificate fingerprint expected after --trust; run $prog -h for details"
      fi
      trust="$1"
      shift
      ;;
    --timeout)
      shift
      if test $# -eq 0; then
        error "value in seconds expected after --timeout; run $prog -h for details"
      fi
      timeout="$1"
      shift
      ;;
    --no-check)
      check=
      shift
      ;;
    -*)
      # It's a lot more likely for someone to misspell an option than to want
      # an installation directory starting with '-'.
      #
      diag "error: unknown option '$1'"
      diag "  info: run 'sh $prog -h' for usage"
      fail
      ;;
    *)
      idir="$1"
      shift
      if test $# -ne 0; then
        diag "error: unexpected argument '$@'"
        diag "  info: options must come before the <install-dir> argument"
        fail
      fi
      break
      ;;
  esac
done

if test "$debug" = true -a "$debug_optimized" = true; then
  diag "error: both --debug and --debug-optimized specified"
  fail
fi

# If --no-bpkg is specified, then we require --local to also be specified
# since it won't be possible to build things from packages without bpkg. Also
# imply --no-bdep in this case, since bdep is pretty much useless without
# bpkg.
#
if test -z "$bpkg_install"; then
  if test -z "$local"; then
    diag "error: --no-bpkg can only be used for local installation"
    diag "  info: additionally specify --local"
    fail
  fi

  bdep_install=
fi

module_version () # <name>
{
  eval "echo \$$1_ver"
}

# Convert the comma-separated modules list into a space-separated list.
#
module_list="$(echo "$modules" | sed 's/,/ /g')"

for m in $module_list; do
  if test -z "$(module_version "$m")"; then
    diag "error: unknown standard build system module '$m'"
    diag "  info: available standard modules: $standard_modules"
    fail
  fi
done

# Make sure --system is only specified with --local.
#
if test -n "$system" -a -z "$local"; then
  diag "error: --system can only be used for local installation"
  diag "  info: additionally specify --local"
  exit 1
fi

# If the installation directory is specified as a relative path then convert
# it to an absolute path, unless the realpath program is not present on the
# system or doesn't recognize any of the options we pass, in which case fail,
# advising to specify an absolute installation directory.
#
if test -n "$(echo "$idir" | sed -n 's#^[^/].*$#true#p')"; then
  check_cmd realpath "specify absolute installation directory path"

  # Don't resolve symlinks and allow non-existent path components.
  #
  if ! idir="$(realpath -s -m "$idir" 2>/dev/null)"; then
    diag "error: realpath does not recognize -s -m"
    diag "  info: specify absolute installation directory path"
    fail
  fi
fi

# Suppress loading of default options files.
#
BUILD2_DEF_OPT="0"
export BUILD2_DEF_OPT

BPKG_DEF_OPT="0"
export BPKG_DEF_OPT

BDEP_DEF_OPT="0"
export BDEP_DEF_OPT

# Unless --yes was specified, ask the user whether to continue.
#
prompt_continue ()
{
  while test -z "$yes"; do
    printf "Continue? [y/n] " 1>&2
    read yes
    case "$yes" in
      y|Y) yes=true ;;
      n|N) fail     ;;
        *) yes=     ;;
    esac
  done
}

# Extract the config.install.(prefix|suffix) value from config.build in the
# specified configuration directory (assume it exists). Set the
# exe_(prefix|suffix) variable to that value unless the user explicitly
# specified this prefix/suffix, in which case verify they match.
#
set_exe_affix () # (prefix|suffix) (<cdir>|<tdir>)
{
  local cv
  cv="$(sed -n "s/^config.bin.exe.$1 = \(.*\)\$/\1/p" "$2/build/config.build")"

  local ov
  eval "ov=\$exe_$1"

  if test -z "$ov"; then
    eval "exe_$1=$cv"
  elif test "$ov" != "$cv"; then
    error "detected executables name $1 '$cv' does not match specified '$ov'"
  fi
}

# Derive the to be installed executables names based on --exe-{prefix,suffix}
# or config.bin.exe.{prefix,suffix}. Unless <installed_only> is true, also
# derive the staged executables names based on --stage-suffix and verify that
# they don't clash with existing filesystem entries as well the executables
# being installed. Assume that the install root (idir) is already set.
#
set_exe_names () # <installed_only>
{
  b="${exe_prefix}b$exe_suffix"
  bpkg="${exe_prefix}bpkg$exe_suffix"
  bdep="${exe_prefix}bdep$exe_suffix"

  if test -z "$1"; then
    b_stage="b$stage_suffix"
    bpkg_stage="bpkg$stage_suffix"

    if test -e "$idir/bin/$b_stage"; then
      diag "error: staged executable name '$b_stage' clashes with existing $idir/bin/$b_stage"
      diag "  info: specify alternative staged executables name suffix with --stage-suffix"
      fail
    fi

    if test -e "$idir/bin/$bpkg_stage"; then
      diag "error: staged executable name '$bpkg_stage' clashes with existing $idir/bin/$bpkg_stage"
      diag "  info: specify alternative staged executables name suffix with --stage-suffix"
      fail
    fi

    if test "$stage_suffix" = "$exe_suffix" -a -z "$exe_prefix"; then
      diag "error: suffix '$exe_suffix' is used for both final and staged executables"
      diag "  info: specify alternative staged executables name suffix with --stage-suffix"
      fail
    fi
  fi
}

# Calculate the SHA256 checksum of the specified file. Note that the csum
# variable should be set to a suitable checksum program (see checksum_init()).
#
checksum () # <file>
{
  local r
  r="$(run $csum "$1" | cut -d ' ' -f 1)"
  echo "$r"
}

checksum_init ()
{
  # See which SHA256 checksum program we will be using today. The options are:
  #
  #  sha256sum (Linux coreutils)
  #  sha256    (FreeBSD, newer NetBSD)
  #  shasum    (Perl tool, Mac OS; also usually available everywhere else)
  #  chsum     (older NetBSD)
  #
  # Note that with these options all three tools output the sum as the first
  # word.
  #
  if command -v sha256sum > /dev/null 2>&1; then
    csum="sha256sum -b"
  elif command -v sha256 > /dev/null 2>&1; then
    csum="sha256 -q"
  elif command -v shasum > /dev/null 2>&1; then
    csum="shasum -a 256 -b"
  elif command -v chsum > /dev/null 2>&1; then
    csum="chsum -a SHA256 -b"
  else
    error "unable to execute sha256sum, sha256, shasum, or chsum: command not found"
  fi
}

# Download the specified file. Common options are --progress-bar or -sS.
#
download () # [<curl-options>] <url>
{
  run curl -fLO --connect-timeout "$connect_timeout" --max-time "$timeout" "$@"
}

# Check if the script is out of date. Prints diagnostics that normally goes
# as a prefix to the prompt.
#
# Note: must be called after checksum_init().
#
check_script ()
{
  # Using $0 as a script path should be good enough for our needs (we don't
  # expect anyone to run it via PATH or to source it).
  #
  prog_sum="$(checksum "$prog")"

  f="$manifest"
  download -sS "$url/$f"

  # Find our checksum line.
  #
  l="$(sed -n "s#^\([^ ]* \*.*/$stem-.*\.sh\)\$#\1#p" "$f")"

  if test -z "$l"; then
    error "unable to extract checksum for $stem.sh from $f"
  fi

  rm -f "$f"

  # Extract the checksum.
  #
  r="$(echo "$l" | sed -n 's#^\([^ ]*\) .*$#\1#p')"

  if test "$r" != "$prog_sum"; then

    # We can have two cases here: a new version is available but this script
    # is (presumably) still valid (would normally happen on public) or the
    # script is out of date for the same (snapshot) version (would normally
    # happen on stage). To find out which case it is we extract and compare
    # the versions.
    #
    v="$(echo "$l" | sed -n 's#^[^ ]* \*\([^/]*\)/.*$#\1#p')"
    f="$(echo "$l" | sed -n 's#^[^ ]* \*\(.*\)$#\1#p')"

    if test "$v" != "$ver"; then

      # Make it the prefix of the plan that follows.
      #
      diag
      diag "Install script for version $v is now available, download from:"
      diag
      diag "  $url/$f"
    else
      diag
      diag "Install script $prog is out of date:"
      diag
      diag "Old checksum: $prog_sum"
      diag "New checksum: $r"
      diag
      diag "Re-download from:"
      diag
      diag "  $url/$f"
      diag
      diag "Or use the --no-check option to suppress this check."
      diag
      fail
    fi
  fi
}

# Calculate an approximate build2 toolchain build and installation time (in
# seconds) if built with the specified number of cores. See build2-times.txt
# for details.
#
install_time () # <cores>
{
  local f
  f=$(($1 <  7 ? 2529 : \
       $1 <  9 ? 1912 : \
       $1 < 17 ? 2511 : \
       $1 < 33 ? 5964 : \
                 8661))

  local r
  r=$(($f / $1))

  # Serial bootstrap fudge.
  #
  if test -z "$make"; then
    r=$(($r * 12 / 10))
  fi

  # Local installation fudge.
  #
  if test "$local" = true; then
    r=$(($r * 7 / 10))
  fi

  echo "$r"
}

install ()
{
  if test -z "$idir"; then
    idir=/usr/local
  fi

  # While we don't use the staged executables directly, we still want to fail
  # early if they clash with anything.
  #
  set_exe_names "$local"

  if test -z "$local" -o "$check" = true; then
    check_cmd curl
  fi

  check_cmd tar

  checksum_init

  # Check if the script is out of date.
  #
  if test "$check" = true; then
    check_script
  fi

  # Toolchain archive type: use .xz if we can, .gz otherwise.
  #
  ez=
  if command -v xz >/dev/null 2>&1; then
    ez=xz
    toolchain_sum="$toolchain_sum_xz"
  else
    check_cmd gzip
    ez=gz
    toolchain_sum="$toolchain_sum_gz"
  fi

  # Figure out if we need sudo. Unless the user made the choice for us, we
  # probe the installation directory to handle cases like Mac OS allowing
  # non-root users to modify /usr/local.
  #
  sudo_hint=
  if test -z "$sudo"; then

    if test -d "$idir"; then
      if ! touch "$idir/build2-write-probe" >/dev/null 2>&1; then
        sudo=sudo
        sudo_hint="required to install into $idir"
      else
        rm -f "$idir/build2-write-probe"
      fi
    else
      if ! mkdir -p "$idir" >/dev/null 2>&1; then
        sudo=sudo
        sudo_hint="required to create $idir"
      fi
    fi
  elif test "$sudo" = "false"; then
    sudo=
  fi

  if test -n "$sudo"; then
    check_cmd "$sudo" "$sudo_hint"
  fi

  # Check if what's specified with --cxx looks like a C compiler. This is not
  # bullet-proof so we only warn.
  #
  cxx_c=
  if test -n "$cxx"; then

    # Clang. Some examples (bad on the left, good on the right):
    #
    # clang                 clang++
    # clang-7               clang++-7
    # i686-linux-gnu-clang  i686-linux-gnu-clang++
    # /clang++-7/clang      /clang-7/clang++
    #
    if test -z "$cxx_c"; then
      cxx_c="$(echo "$cxx" | sed -n 's#^\(.*\)clang\([^/+]*\)$#\1clang++\2#p')"
    fi

    # GCC. Some examples (bad on the left, good on the right):
    #
    # gcc                 g++
    # gcc-7               g++-7
    # i686-linux-gnu-gcc  i686-linux-gnu-g++
    # /g++-7/gcc          /gcc-7/g++
    #
    if test -z "$cxx_c"; then
      cxx_c="$(echo "$cxx" | sed -n 's#^\(.*\)gcc\([^/]*\)$#\1g++\2#p')"
    fi
  fi

  # If --jobs|-j is unspecified, then try to guess the number of available
  # hardware threads.
  #
  # Note that we will be able to precisely detect the system we are running on
  # only later, when the toolchain is downloaded and the config.guess script
  # becomes available. Thus, now we are just trying to use the nproc (Linux)
  # and sysctl (Darwin, FreeBSD, NetBSD) programs sequentially, in the hope
  # that one of them will succeed.
  #
  if test -z "$jobs"; then
    if ! jobs="$(nproc 2>/dev/null)" && \
       ! jobs="$(sysctl -n hw.ncpu 2>/dev/null)"; then
      jobs=
    fi
  fi

  # Estimate the available cores count based on the hardware threads count.
  # Note: only used for the plan printing.
  #
  cores=
  if test -n "$jobs"; then
    cores=$(($jobs < 4 ? $jobs : $jobs / 2))
  fi

  # See if we can find GNU make and bootstrap in parallel.
  #
  make=
  make_ver=
  if command -v gmake > /dev/null 2>&1; then
    make=gmake
  elif command -v make > /dev/null 2>&1; then
    make=make
  fi

  if test -n "$make"; then
    # Note that if this is not a GNU make, then it may not recognize --version
    # (like BSD make) and fail.
    #
    make_ver="$("$make" --version 2>&1 | sed -n 's/^GNU Make \(.*\)$/\1/p')"
    if test -z "$make_ver"; then
      diag "WARNING: $make is not GNU make, performing serial bootstrap using shell script"
      make=
    fi
  else
    diag "WARNING: no GNU make found, performing serial bootstrap using shell script"
  fi

  # If we are using GNU make and the number of jobs is unspecified and cannot
  # be detected, then advise the user to specify --jobs|-j explicitly to
  # bootstrap in parallel.
  #
  if test -n "$make" -a -z "$jobs"; then
    diag "WARNING: unable to determine hardware concurrency, performing serial bootstrap"
    diag "WARNING: consider manually specifying the number of jobs with --jobs|-j"
  fi

  # Print the plan and ask for confirmation.
  #
  diag
  diag "-------------------------------------------------------------------------"
  diag
  diag "About to download, build, and install build2 toolchain $ver ($type)."
  diag
  diag "From:  $url"
  diag "Using: $cxx_name"
  if test -n "$cxx_c"; then
  diag
  diag "WARNING: $cxx looks like a C compiler, did you mean $cxx_c?"
  fi
  diag
  diag "Install directory: $idir/"
  diag "Build directory:   $owd/"
  diag
  if test -z "$local"; then
  diag "Package repository: $repo"
  diag
  fi
  diag "For options (change the installation directory, etc), run:"
  diag
  diag "  sh $prog -h"
  diag
  if test -n "$jobs"; then
  # Calculate the installation time as the average between the jobs and
  # cores count based installation times and round it to minutes.
  #
  jt="$(install_time $jobs)"
  ct="$(install_time $cores)"
  it=$(((($jt + $ct) / 2 + 30) / 60))
  diag "Expected installation time is approximately $it minute(s)."
  if test -z "$local"; then
  diag
  diag "If you are not concerned with incrementally upgrading the toolchain, you"
  diag "may specify the --local option to reduce this time by approximately 30%."
  fi
  diag
  fi
  if test -f "$idir/bin/$b"; then
  diag "WARNING: $idir/ already contains build2, consider uninstalling first."
  diag
  fi
  if test -z "$local" -a -d "$cdir"; then
  diag "WARNING: $cdir/ already exists and will be overwritten."
  diag
  fi
  if test -n "$sudo"; then
  diag "Note: to install into $idir/ will be using $sudo."
  diag
  fi
  prompt_continue

  # Clean up.
  #
  if test -z "$local"; then
    run rm -rf "$cdir"
  fi
  run rm -rf "$tdir"

  # Get the toolchain package so that we can run its config.guess script.
  #
  if ! test -f "$toolchain.tar.$ez"; then
    download --progress-bar "$url/$ver/$toolchain.tar.$ez"
  fi

  # Verify the checksum.
  #
  r="$(checksum "$toolchain.tar.$ez")"
  if test "$r" != "$toolchain_sum"; then
    diag "error: $toolchain.tar.$ez checksum mismatch"
    diag "  info: calculated $r"
    diag "  info: expected   $toolchain_sum"
    diag "  info: remove $toolchain.tar.$ez to force re-download"
    fail
  fi

  diag "info: $toolchain.tar.$ez checksum verified successfully"

  # Unpack.
  #
  if test "$ez" = "xz"; then
    run xz -dk "$toolchain.tar.xz"
  else
    run gzip -dk "$toolchain.tar.gz"
  fi
  run tar -xf "$toolchain.tar"
  run rm -f "$toolchain.tar"

  run cd "$tdir"

  # Determine the target.
  #
  tgt="$(run build2/config.guess)"
  sys="$(echo "$tgt" | sed -n 's/^[^-]*-[^-]*-\(.*\)$/\1/p')"

  diag "info: running on $tgt ($sys)"

  # Figure out which C++ compiler we are going to use.
  #
  cxx_hint="specify alternative C++ compiler to use with --cxx"
  if test -z "$cxx"; then
    case "$sys" in
      linux*|netbsd*)
        cxx="g++"
        ;;
      darwin*)
        cxx="clang++"
        cxx_hint="install Command Line Tools with 'xcode-select --install'"
        ;;
      freebsd*)
        cxx="clang++"
        ;;
      *)
        error "not guessing C++ compiler for $sys$, specify explicitly with --cxx"
        ;;
    esac
  fi
  check_cmd "$cxx" "$cxx_hint"

  # Check if this looks like ccache (see GH issues 86, 300 for background).
  # Note that we have similar diagnostics in build2 itself (in the cc module).
  #
  # Note that we know command will succeed because of the preceding check_cmd.
  #
  cxx_abs="$(command -v "$cxx")"
  if test -n "$(echo "$cxx_abs" | sed -n 's#ccache#true#p')"; then
    diag "error: $cxx@$cxx_abs looks like a ccache wrapper"
    diag "  info: ccache cannot be used as a C++ compiler with build2"
    diag "  info: specify real C++ compiler path with --cxx"
    if test "$cxx_abs" != "/usr/bin/g++"; then
      diag "  info: for example, try --cxx /usr/bin/g++"
    fi
    diag "  info: alternatively, you may want to uninstall ccache"
    fail
  fi

  # We don't have arrays in POSIX shell but we should be ok as long as none of
  # the option values contain spaces. Note also that the expansion must be
  # unquoted.
  #
  uninst_ops="--uninstall"

  if test "$local" = true; then
    ops="--local"
    uninst_ops="$uninst_ops --local"
  else
    ops="--stage-suffix $stage_suffix --timeout $timeout"

    if test -n "$trust"; then
      ops="$ops --trust $trust"
    fi

    upgrade_ops="--upgrade"
  fi

  if test -n "$exe_prefix"; then
    ops="$ops --exe-prefix $exe_prefix"
  fi

  if test -n "$exe_suffix"; then
    ops="$ops --exe-suffix $exe_suffix"
  fi

  if test -z "$bpkg_install"; then
    ops="$ops --no-bpkg"
    upgrade_ops="$upgrade_ops --no-bpkg"
    uninst_ops="$uninst_ops --no-bpkg"
  fi

  if test -z "$bdep_install"; then
    ops="$ops --no-bdep"
    upgrade_ops="$upgrade_ops --no-bdep"
    uninst_ops="$uninst_ops --no-bdep"
  fi

  if test "$modules" != "$standard_modules"; then
    if test -z "$modules"; then
      ops="$ops --no-modules"
      upgrade_ops="$upgrade_ops --no-modules"
      uninst_ops="$uninst_ops --no-modules"
    else
      ops="$ops --modules $modules"
      upgrade_ops="$upgrade_ops --modules $modules"
      uninst_ops="$uninst_ops --modules $modules"
    fi
  fi

  if test -n "$system"; then
    ops="$ops --system $system"
  fi

  if test -n "$jobs"; then
    ops="$ops -j $jobs"
  fi

  if test -n "$sudo"; then
    ops="$ops --sudo $sudo"
  fi

  if test "$private" = true; then
    ops="$ops --private"
  fi

  if test -n "$make"; then
    ops="$ops --make $make"
  fi

  args=
  if test "$debug" = true; then
    args="-g"
  elif test "$debug_optimized" = true; then
    args="-g -O3"
  fi

  run ./build.sh $ops --install-dir "$idir" "$cxx" $args

  run cd "$owd"

  if test -z "$local"; then
    run rm -rf "$tdir"
  fi

  # Print the report.
  #
  diag
  diag "-------------------------------------------------------------------------"
  diag
  diag "Successfully installed build2 toolchain $ver ($type)."
  diag
  diag "Install directory:   $idir/"
  if test -z "$local"; then
  diag "Build configuration: $cdir/"
  else
  diag "Build configuration: $tdir/"
  fi
  if test -z "$local"; then
  diag
  diag "To upgrade, change to $owd/ and run:"
  diag
  diag "  sh $prog $upgrade_ops"
  fi
  diag
  diag "To uninstall, change to $owd/ and run:"
  diag
  diag "  sh $prog $uninst_ops"
  diag
  if test "$idir" != "/usr" -a "$idir" != "/usr/local"; then
  diag "Consider adding $idir/bin to the PATH environment variable:"
  diag
  diag '  export "PATH='"$idir/bin"':$PATH"'
  diag
  fi
}

# Extract the config.install.root value from config.build in the specified
# configuration directory. Set idir to that value unless the user explicitly
# specified the installation directory, in which case verify they match.
#
set_install_root () # <cdir>|<tdir>
{
  if ! test -d "$1"; then
    error "build configuration directory $1/ does not exist"
  fi

  local c
  c="$1/build/config.build"

  if ! test -f "$c"; then
    error "directory $1/ does not contain a build configuration"
  fi

  # Note that the value could be quoted.
  #
  local r
  r="$(sed -n "s#^config.install.root = '*\(.*\)/'*\$#\1#p" "$c")"

  if test -z "$r"; then
    error "unable to extract installation directory from $c"
  fi

  if test -z "$idir"; then
    idir="$r"
  elif test "$idir" != "$r"; then
    diag "error: detected installation directory does not match specified"
    diag "  info: detected  $r"
    diag "  info: specified $idir"
    fail
  fi
}

upgrade ()
{
  error "no upgrade is possible for this release, perform the from-scratch installation"

  if test "$local" = true; then
    error "upgrade is not supported for local install"
  fi

  checksum_init

  # Check if the script is out of date.
  #
  if test "$check" = true; then
    check_script
  fi

  # First check if we already have the current version (i.e., patch upgrade).
  # Then previous version, unless empty (no upgrade possible).
  #
  # If this is a patch release, then we do the "dirty" upgrade. Otherwise --
  # staged.
  #
  if test -d "$cdir"; then
    kind=dirty
    ucdir="$cdir"
    ionly=true
  elif test -d "$pcdir"; then
    if test -n "$pcver"; then
      kind=staged
      ucdir="$pcdir"
      ionly=
    else
      error "no upgrade is possible, perform the from-scratch installation"
    fi
  else
    error "no existing build configuration in $cdir/ or $pcdir/"
  fi

  set_install_root "$ucdir"

  set_exe_affix "prefix" "$ucdir"
  set_exe_affix "suffix" "$ucdir"
  set_exe_names "$ionly"

  # Print the plan and ask for confirmation.
  #
  diag
  diag "-------------------------------------------------------------------------"
  diag
  diag "About to perform $kind upgrade of build2 toolchain to $ver ($type)."
  diag
  diag "Install directory:   $idir/"
  diag "Build configuration: $ucdir/"
  if ! test -f "$idir/bin/$b"; then
  diag
  diag "WARNING: $idir/ does not seem to contain a build2 installation."
  fi
  diag
  diag "Package repository: $repo"
  diag
  prompt_continue

  # Add $idir/bin to PATH in case it is not already there.
  #
  PATH="$idir/bin:$PATH"
  export PATH

  # Translate our options to their bpkg versions, same as in build.sh from
  # build2-toolchain.
  #
  if test -n "$jobs"; then
    jobs="-j $jobs"
  fi

  fetch_ops="--fetch-timeout $timeout"
  build_ops="--fetch-timeout $timeout"

  if test "$trust" = "yes"; then
    fetch_ops="$fetch_ops --trust-yes"
  elif test "$trust" = "no"; then
    fetch_ops="$fetch_ops --trust-no"
  elif test -n "$trust"; then
    fetch_ops="$fetch_ops --trust $trust"
  fi

  packages="build2/$build2_ver bpkg/$bpkg_ver"

  if test "$bdep_install" = true; then
    packages="$packages bdep/$bdep_ver"
  fi

  module_packages=

  for m in $module_list; do
    if test -n "$module_packages"; then
      module_packages="$module_packages libbuild2-$m/$(module_version "$m")"
    else
      module_packages="libbuild2-$m/$(module_version "$m")"
    fi
  done

  # Note that we use bpkg-rep-fetch(1) to both add and only fetch this
  # repository if it's not the same as the existing.
  #
  # Also note that we need to make sure that the build system modules are
  # built and installed with the new version of the toolchain. Plus the set
  # of build system modules could change.
  #
  if test "$kind" = dirty; then

    run cd "$cdir"
    run "$bpkg" $jobs uninstall '!config.install.scope=project' --all-pattern=libbuild2-*
    run "$bpkg" drop --yes --all-pattern=libbuild2-*
    run "$bpkg" $fetch_ops fetch "$repo"
    run "$bpkg" $jobs $build_ops build --for install --patch --recursive --yes --plan= $packages
    run "$bpkg" $jobs install --all
    run "$bpkg" $jobs $build_ops build --for install --yes --plan= $module_packages
    run "$bpkg" $jobs install '!config.install.scope=project' --all-pattern=libbuild2-*
    run cd "$owd"

  else

    run cp -rp "$ucdir" "$cdir"

    run cd "$cdir"
    run "$bpkg" drop --yes --all-pattern=libbuild2-*
    run "$bpkg" $fetch_ops fetch "$repo"

    stage_conf="config.bin.suffix=$stage_suffix \
config.bin.exe.prefix=[null] config.bin.exe.suffix=[null] \
config.install.data_root=root/stage"

    # Note: not installing bdep-stage since we don't need it.
    #
    run "$bpkg" $jobs $build_ops build --for install --upgrade --recursive --yes --plan= $packages
    run "$bpkg" $jobs install $stage_conf build2 bpkg

    run command -v "$b_stage"
    run "$b_stage" --version

    run command -v "$bpkg_stage"
    run "$bpkg_stage" --version
    run cd "$owd"

    run cd "$ucdir"
    run "$bpkg" $jobs uninstall --all
    run cd "$owd"

    run cd "$cdir"
    run "$bpkg_stage" $jobs install --all
    run "$bpkg" $jobs $build_ops build --for install --yes --plan= $module_packages
    run "$bpkg" $jobs install '!config.install.scope=project' --all-pattern=libbuild2-*
    run "$bpkg" $jobs uninstall $stage_conf build2 bpkg
    run cd "$owd"
  fi

  run "$b" --version
  run "$bpkg" --version

  if test "$bdep_install" = true; then
    run "$bdep" --version
  fi

  # Print the report. The new configuration is always in cdir.
  #
  diag
  diag "-------------------------------------------------------------------------"
  diag
  diag "Successfully upgraded build2 toolchain to $ver ($type)."
  diag
  diag "Install directory:   $idir/"
  diag "Build configuration: $cdir/"
  if test "$ucdir" != "$cdir"; then
  diag
  diag "Old configuration:   $ucdir/"
  fi
  diag
}

uninstall ()
{
  if test -z "$local"; then
    ucdir="$cdir"
  else
    ucdir="$tdir"
  fi

  set_install_root "$ucdir"

  set_exe_affix "prefix" "$ucdir"
  set_exe_affix "suffix" "$ucdir"
  set_exe_names true

  # Print the plan and ask for confirmation.
  #
  diag
  diag "-------------------------------------------------------------------------"
  diag
  diag "About to uninstall build2 toolchain $ver ($type)."
  diag
  diag "Install directory:   $idir/"
  diag "Build configuration: $ucdir/"
  if ! test -f "$idir/bin/$b"; then
  diag
  diag "WARNING: $idir/ does not seem to contain a build2 installation."
  fi
  diag
  prompt_continue

  # Add $idir/bin to PATH in case it is not already there.
  #
  PATH="$idir/bin:$PATH"
  export PATH

  if test -n "$jobs"; then
    jobs="-j $jobs"
  fi

  # Note that if at some point we change the way build.sh installs things
  # (for example, by installing dependencies recursively), then this might
  # have to be adjusted as well.
  #
  if test -z "$local"; then
    run cd "$cdir"
    run "$bpkg" $jobs uninstall --all
    run cd "$owd"
    run rm -rf "$cdir"
  else
    projects="build2/"

    if test "$bpkg_install" = true; then
      projects="bpkg/ $projects"
    fi

    if test "$bdep_install" = true; then
      projects="bdep/ $projects"
    fi

    for m in $module_list; do
      projects="libbuild2-$m/ $projects"
    done

    run cd "$tdir"
    run "$b" $jobs uninstall: $projects
    run cd "$owd"
    run rm -rf "$tdir"
  fi

  # Print the report.
  #
  diag
  diag "-------------------------------------------------------------------------"
  diag
  diag "Successfully uninstalled build2 toolchain $ver ($type)."
  diag
}

case "$mode" in
  install)   install                 ;;
  upgrade)   upgrade                 ;;
  uninstall) uninstall               ;;
  *)         error "unexpected mode" ;;
esac
