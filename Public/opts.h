#pragma once

#include <string>
#include <vector>

constexpr const char* BACKEND_URL = "http://127.0.0.1:3551"; // Set valid ip not 127.0.0.1 for mobile
constexpr bool ENABLE_LOG = false;
const std::vector<std::wstring> epicDomains = {
    L"ol.epicgames.com",
    L"ol.epicgames.net",
    L"on.epicgames.com",
    L"ak.epicgames.com"
};
