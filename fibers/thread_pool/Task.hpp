#pragma once

#include <functional>

namespace fibers {

using Task = std::function<void()>;

}  // namespace fibers
