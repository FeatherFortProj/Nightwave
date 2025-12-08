#pragma once

#include <string>
#include <vector>

constexpr const char* BACKEND_URL = "http://192.168.1.69:8080"; // Set valid ip not 127.0.0.1 for mobile
constexpr bool ENABLE_LOG = true;
static const std::wstring epicDomains[] = {
    L"game-social.epicgames.com",
    L"ol.epicgames.com",
    L"ol.epicgames.net", 
    L"on.epicgames.com",
    L"ak.epicgames.com",
    L"epicgames.dev"
};
