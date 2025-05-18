#include "helper.h"

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "../TextEditor.h"


struct AssemblyLine {
    std::string assembly;
    uint64_t addr;
};


// Split a string into lines
std::vector<std::string> SplitLines(const std::string& text) {
    std::istringstream stream(text);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::vector<AssemblyLine> GetMockDisassembly(int lineNumber) {
    // Simulate a few different instructions
    switch (lineNumber % 3) {
        case 0:
            return {};
        case 1:
            return {
                    { "push ebp", 0x00000008 },
                    { "mov ebp, esp", 0x0000000C }
            };
        default:
            return {
                    { "call 0x80483f0 <add>", 0x00000010 },
                    { "pop ebp", 0x00000015 }
            };
    }
}

// Renders a C code editor with optional inline assembly (line-by-line)
void ShowCCodeEditorLineByLine() {
    static TextEditor editor;
    static bool initialized = false;
    if (!initialized) {
        editor.SetLanguageDefinition(TextEditor::LanguageDefinitionId::C);
        editor.SetText(R"(int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3);
    return result;
})");
        initialized = true;
        editor.SetReadOnlyEnabled(true);
    }

    static std::string cCode = R"(int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3);
    return result;
})";

    static bool showDisasm = true;
    {
        ImGui::Begin("Hello, world!");
        int mLine, mColumn;
        editor.GetCursorPosition(mLine, mColumn);
        ImGui::Text("%6d/%-6d lines ", mLine, mColumn);
        ImGui::End();
    }

    ImGui::Begin("C Code Editor with Inline Assembly");
    editor.Render("##CCodeEditor", false, ImVec2(600, 300));  // adjust size as needed


    ImGui::End();
}


int main() {
    auto window = init();
    ImGuiIO& io = ImGui::GetIO();

    // Main loop
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
            ShowCCodeEditorLineByLine();
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    deinit(window);
    return 0;
}
