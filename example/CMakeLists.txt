
add_executable(test2 main2.cpp ../TextEditor.cpp ../LanguageDefinitions.cpp ../ImGuiDebugPanel.cpp ../UnitTests.cpp)
target_link_libraries(test2 PRIVATE ${SDL2_LIBRARIES} ${gl3w_LIBRARIES} ImGui::imgui GL)
target_compile_definitions(test2 PRIVATE IMGUI_IMPL_OPENGL_LOADER_GL3W)
target_compile_features(test2 PRIVATE cxx_constexpr)
