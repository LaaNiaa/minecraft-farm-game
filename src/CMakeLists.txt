add_executable(main
        main.cpp
        game.cpp
        field.cpp
        map_loader.cpp
        save_manager.cpp
)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(main PRIVATE SFML::Audio)
target_link_libraries(main PRIVATE SFML::Window)
target_link_libraries(main PRIVATE SFML::System)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)