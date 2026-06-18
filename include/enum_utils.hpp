#pragma once

#include <string_view>
#include <optional>
#include <array>
#include <type_traits>

template <typename T>
struct EnumTraits;

template <typename T>
concept MappableEnum = std::is_enum_v<T> && requires {
    { EnumTraits<T>::mapping };
};

template <MappableEnum T>
constexpr std::string_view enumToString(T value) {
    for (const auto& [enumVal, strVal] : EnumTraits<T>::mapping) {
        if (enumVal == value) {
            return strVal;
        }
    }
    return "UNKNOWN";
}

template <MappableEnum T>
constexpr std::optional<T> enumFromString(std::string_view str) {
    for (const auto& [enumVal, strVal] : EnumTraits<T>::mapping) {
        if (strVal == str) {
            return enumVal;
        }
    }
    return std::nullopt;
}

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
    static constexpr std::array<std::pair<CropType, std::string_view>, 3> mapping = {{
        {CropType::NONE, "NONE"},
        {CropType::WHEAT, "WHEAT"},
        {CropType::CARROT, "CARROT"}
    }};
};