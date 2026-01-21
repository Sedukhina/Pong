#pragma once
#include <stdint.h>

constexpr uint32_t FontAtlasWidth = 1024;
constexpr uint32_t FontAtlasHeight = 1024;

constexpr uint32_t CodePointOfFirstChar = 32;
constexpr uint32_t CharsToIncludeInFontAtlas = 95;

constexpr float FontSize = 64.0f;

constexpr auto FontPath = "../Assets/Winter Tosca.ttf";