#pragma once

#include <string_view>
#include <optional>
#include <array>
#include <type_traits>

// Załóżmy, że tutaj są zdefiniowane BlockType, CropState i CropType
#include "field.hpp" 

// 1. Deklaracja głównego szablonu (bez definicji)
template <typename T>
struct EnumTraits;

// 2. Concept: Wymaga, aby T było enumem i posiadało statyczne pole `mapping`
template <typename T>
concept MappableEnum = std::is_enum_v<T> && requires {
    { EnumTraits<T>::mapping };
};

// 3. Generyczna funkcja do rzutowania Enum -> String
template <MappableEnum T>
constexpr std::string_view enumToString(T value) {
    for (const auto& [enumVal, strVal] : EnumTraits<T>::mapping) {
        if (enumVal == value) {
            return strVal;
        }
    }
    return "UNKNOWN";
}

// 4. Generyczna funkcja do rzutowania String -> Enum
template <MappableEnum T>
constexpr std::optional<T> enumFromString(std::string_view str) {
    for (const auto& [enumVal, strVal] : EnumTraits<T>::mapping) {
        if (strVal == str) {
            return enumVal;
        }
    }
    return std::nullopt;
}

// ============================================================================
// SPECJALIZACJE TRAITS (Tutaj dodajesz nowe enumy)
// ============================================================================

template <>
struct EnumTraits<BlockType> {
    static constexpr std::array<std::pair<BlockType, std::string_view>, 6> mapping = {{
        {BlockType::NONE, "NONE"},
        {BlockType::GRASS_BLOCK, "GRASS_BLOCK"},
        {BlockType::DIRT, "DIRT"},
        {BlockType::FARMLAND_DRY, "FARMLAND_DRY"},
        {BlockType::FARMLAND_WET, "FARMLAND_WET"},
        {BlockType::WATER, "WATER"}
    }};
};

template <>
struct EnumTraits<CropState> {
    static constexpr std::array<std::pair<CropState, std::string_view>, 3> mapping = {{
        {CropState::EMPTY, "EMPTY"},
        {CropState::SEED, "SEED"},
        {CropState::GROWN, "GROWN"}
    }};
};

template <>
struct EnumTraits<CropType> {
    static constexpr std::array<std::pair<CropType, std::string_view>, 2> mapping = {{
        {CropType::NONE, "NONE"},
        {CropType::WHEAT, "WHEAT"}
    }};
};