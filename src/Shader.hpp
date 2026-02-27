#pragma once

#include <string>

namespace Ripview {

struct Shader {
  Shader(const std::string &filepath, unsigned int type);

  int mId;
  int mType;
  const std::string &mFilepath;
};

} // namespace Ripview
