#pragma once

#include <string>

namespace Ripview {

struct Shader {
  Shader(const std::string &filepath);

  int mId;
  int mType;
  const std::string &mFilepath;
};

} // namespace Ripview
