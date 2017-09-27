[![Build Status](https://travis-ci.org/wow2006/OS_Info.svg?branch=master)](https://travis-ci.org/wow2006/OS_Info)

### TODO

- [x] Kernel and CPU architecture info, that's `uname()`
- [x] CPU model name, `/proc/cpuinfo` "model name" line **FIXME** number of cores 
- [x] Memory Ram, `/proc/meminfo`
- [x] Distro info, that lsb-release or os-release
- [x] Possibly libc version, for glibc check preprocessor variable `__GNU_LIBRARY__` and use gnu_get_glibc_version
- [x] Display server, use `SDL_GetWindowWMInfo`
- [x] Windowing/input toolkit version, use `SDL_GetVersion`
- [x] GL version and driver vendor, use OpenGL version call info*

