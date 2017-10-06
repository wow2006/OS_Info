#include "linux/glibcInfo.hpp"
#ifdef GCC_VERSION
#include <gnu/libc-version.h>
#endif

using map = GlibcInfo::map;

bool GlibcInfo::read_glibc(map& info) {
#ifdef GCC_VERSION
  info["version"] = gnu_get_libc_version();
#else
  info["version"] = "Unknow"
#endif
  return !info["version"].empty();
}
