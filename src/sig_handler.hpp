#pragma once

#include <functional>

int register_handler(const std::function<void(int)>&);
