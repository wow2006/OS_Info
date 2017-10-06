#ifndef SDL_INFO_HPP
#define SDL_INFO_HPP
#include <unordered_map>
#include <string>

struct SdlInfo {
  using map = std::unordered_map<std::string, std::string>;

  static bool read_SDL(map& sdlVersion, map& displayServer, map& glVersionInfo,
                       map& gpu);
};
#endif  //! SDL_INFO_HPP
