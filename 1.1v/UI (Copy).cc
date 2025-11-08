module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module __sk_lib_UIOI__;

import __sk_lib_base__;
import __sk_lib_Arrays__;
import __sk_lib_math__;
import __sk_lib_String__;
import __sk_lib_Files__;
import __sk_lib_Input__;
import __sk_lib_graph__;

import std;

#include "defines.h++"

export namespace UI{



    class SK_CheckBox : public SK_Label<false> {
    public : bool is_checked = false;
        SK_CheckBox(const char* text = "title", bool is_checked = false) : SK_Label<false>(text), is_checked(is_checked) {}
        SK_CheckBox(std::string text, bool is_checked = false) : SK_Label<false>(text), is_checked(is_checked) {}

        bool operator()(bool in_same_line = false) override { return this->operator()({ -1, -1, -1, -1 }, in_same_line); }

        sget_addition(bool, is_checked)

        bool operator()(geometry::SK_Rect2D<float> size, bool in_same_line = false) override {
            bool ret = false;
            if (size.x > -1 and size.y > -1) ImGui::SetCursorPos(ImVec2(size.x, size.y));
            if (ImGui::Checkbox(this->text, &is_checked)) ret = true;
            in_same_line ? ImGui::SameLine() : ImGui::Separator();
            return ret;
        }
    };



    template<> class SK_ColorChooser<3> : public SK_Label<false> {
    public : graph::SK_RGBColorF selected_color;
        SK_ColorChooser(const char* text = "title", graph::SK_RGBColorF selected_color = {}) : SK_Label<false>(text), selected_color(selected_color) {}
        SK_ColorChooser(std::string text, graph::SK_RGBColorF selected_color = {}) : SK_Label<false>(text), selected_color(selected_color) {}

        bool operator()(bool in_same_line = false) override { return this->operator()({ -1, -1, -1, -1 }, in_same_line); }

        sget_addition(graph::SK_RGBColorF, selected_color)

        bool operator()(geometry::SK_Rect2D<float> size, bool in_same_line = false) override {
            bool ret = false;
            if (size.x > -1 and size.y > -1) ImGui::SetCursorPos(ImVec2(size.x, size.y));
            if (ImGui::ColorEdit3(this->text, selected_color.data)) ret = true;
            in_same_line ? ImGui::SameLine() : ImGui::Separator();
            return ret;
        }
    };

    template<> class SK_ColorChooser<4> : public SK_Label<false> {
    public : graph::SK_ColorF selected_color;
        SK_ColorChooser(const char* text = "title", graph::SK_ColorF selected_color = {}) : SK_Label<false>(text), selected_color(selected_color) {}
        SK_ColorChooser(std::string text, graph::SK_ColorF selected_color = {}) : SK_Label<false>(text), selected_color(selected_color) {}

        bool operator()(bool in_same_line = false) override { return this->operator()({ -1, -1, -1, -1 }, in_same_line); }

        sget_addition(graph::SK_ColorF, selected_color)

        bool operator()(geometry::SK_Rect2D<float> size, bool in_same_line = false) override {
            bool ret = false;
            if (size.x > -1 and size.y > -1) ImGui::SetCursorPos(ImVec2(size.x, size.y));
            if (ImGui::ColorEdit4(this->text, selected_color.data)) ret = true;
            in_same_line ? ImGui::SameLine() : ImGui::Separator();
            return ret;
        }
    };



    template<typename num_t, int Size> class SK_Slider : public SK_Label<false> {
    public : num_t data[Size];
        num_t min_value, max_value;
        SK_Slider(const char* text = "title", num_t min_value = -10, num_t max_value = 10, num_t data[Size] = {})
            : SK_Label<false>(text), min_value(min_value), max_value(max_value) { for (int i = 0; i < Size; i++) this->data[i] = data[i]; }
        SK_Slider(std::string text, num_t min_value = -10, num_t max_value = 10, num_t data[Size] = {})
            : SK_Label<false>(text), min_value(min_value), max_value(max_value) { for (int i = 0; i < Size; i++) this->data[i] = data[i]; }
        SK_Slider(const char* text, num_t min_value = -10, num_t max_value = 10, std::vector<num_t> data = {})
            : SK_Label<false>(text), min_value(min_value), max_value(max_value) { for (int i = 0; i < Size; i++) this->data[i] = data[i]; }
        SK_Slider(std::string text, num_t min_value = -10, num_t max_value = 10, std::vector<num_t> data = {})
            : SK_Label<false>(text), min_value(min_value), max_value(max_value) { for (int i = 0; i < Size; i++) this->data[i] = data[i]; }

        bool operator()(bool in_same_line = false) override { return this->operator()({ -1, -1, -1, -1 }, in_same_line); }

        bool operator()(geometry::SK_Rect2D<float> size, bool in_same_line = false) override {
            bool ret = false;
            if (size.x > -1 and size.y > -1) ImGui::SetCursorPos(ImVec2(size.x, size.y));

            if constexpr (std::is_same_v<num_t, float>) {
                if constexpr (Size == 1) { if (ImGui::SliderFloat(this->text, data, min_value, max_value)) ret = true; }
                else if constexpr (Size == 2) { if (ImGui::SliderFloat2(this->text, data, min_value, max_value)) ret = true; }
                else if constexpr (Size == 3) { if (ImGui::SliderFloat3(this->text, data, min_value, max_value)) ret = true; }
                else if constexpr (Size == 4) { if (ImGui::SliderFloat4(this->text, data, min_value, max_value)) ret = true; }
            } else if constexpr (std::is_same_v<num_t, int>) {
                if constexpr (Size == 1) { if (ImGui::SliderInt(this->text, data, min_value, max_value)) ret = true; }
                else if constexpr (Size == 2) { if (ImGui::SliderInt2(this->text, data, min_value, max_value)) ret = true; }
                else if constexpr (Size == 3) { if (ImGui::SliderInt3(this->text, data, min_value, max_value)) ret = true; }
                else if constexpr (Size == 4) { if (ImGui::SliderInt4(this->text, data, min_value, max_value)) ret = true; }
            }

            in_same_line ? ImGui::SameLine() : ImGui::Separator();
            return ret;
        }
        inline num_t operator[] (int i) { return this->data[i]; }
    };

    using SK_SliderF =  SK_Slider<float, 1>;
    using SK_SliderF2 = SK_Slider<float, 2>;
    using SK_SliderF3 = SK_Slider<float, 3>;
    using SK_SliderF4 = SK_Slider<float, 4>;
    using SK_SliderI =  SK_Slider<int, 1>;
    using SK_SliderI2 = SK_Slider<int, 2>;
    using SK_SliderI3 = SK_Slider<int, 3>;
    using SK_SliderI4 = SK_Slider<int, 4>;



    template<bool is_lined, int Size> class SK_InputText : public SK_Label<false> {
    public : str::String text = str::String("");
        const char* label_code = "##inputtext";
        SK_InputText(std::string text = "title", const char* label_code = "##inputtext") : text(text), label_code(label_code) {}
        SK_InputText(const char* text, const char* label_code = "##inputtext") : text(text), label_code(label_code) {}

        bool operator()(bool in_same_line = false) override { return operator()({ -1, -1, -1, -1 }, in_same_line); }

        bool operator()(geometry::SK_Rect2D<float> size, bool in_same_line = false) override {
            bool ret = false;
            if (size.x > -1 and size.y > -1) ImGui::SetCursorPos(ImVec2(size.x, size.y));
            if constexpr  (Size <= 0) {
                if constexpr  (is_lined) {
                    ImVec2 rect_size = { (size.width == -1) ? 0 : size.width, (size.height == -1) ? 0 : size.height };
                    if (ImGui::InputTextMultiline(label_code, (char*)this->text.c_str(), static_cast<int>(this->text.size()) + 1, rect_size)) ret = true;
                } else { if (ImGui::InputText(label_code, (char*)this->text.c_str(), static_cast<int>(this->text.size()) + 1)) ret = true; }
            } else { 
                if constexpr  (is_lined) {
                    ImVec2 rect_size = { (size.width == -1) ? 0 : size.width, (size.height == -1) ? 0 : size.height };
                    if (ImGui::InputTextMultiline(label_code, (char*)this->text.c_str(), Size, rect_size)) ret = true;
                } else { if (ImGui::InputText(label_code, (char*)this->text.c_str(), Size)) ret = true; }
            } in_same_line ? ImGui::SameLine() : ImGui::Separator();
            if (text.size() > Size) text = text.substring(0, Size-1);
            return ret;
        }
    };

    template<int Size = -1> using SK_TxtlnInput = SK_InputText<true, Size>;
    template<int Size = -1> using SK_TxtInput = SK_InputText<false, Size>;



    template<bool Enable = false> class SK_RadioBtn_Selection_extention { public : bool is_active = false; };
    template<> class SK_RadioBtn_Selection_extention<true> {
    public : static int selected;
        int index;
        SK_RadioBtn_Selection_extention(int index) :index(index) {}
        SK_RadioBtn_Selection_extention() {}
        inline bool is_seleceted() { return index == selected; }
    };

    template<bool Enable = true> class SK_RadioButton : public SK_RadioBtn_Selection_extention<Enable>, public SK_Label<false> {
    public : SK_RadioButton(const char* title) :SK_Label(title) {}
        SK_RadioButton(const char* title, int index) requires(Enable)
            :SK_Label(title), SK_RadioBtn_Selection_extention<Enable>(index) {}

        bool operator()(geometry::SK_Rect2D<float> size, bool in_same_line = false) override {
            if (size.x > -1 and size.y > -1) ImGui::SetCursorPos(ImVec2(size.x, size.y));
            if constexpr  (Enable) { if (ImGui::RadioButton(this->text, this->is_seleceted()))
                SK_RadioBtn_Selection_extention<true>::selected = this->index; }
            else ImGui::RadioButton(this->text, this->is_active);
            in_same_line ? ImGui::SameLine() : ImGui::Separator();
            return true;
        }
    };

    using SK_RadioBtn = SK_RadioButton<false>;
    using SK_SRadioBtn = SK_RadioButton<true>;



    class SK_MMenuBar : public SK_UItem<false> {
    private: bool is_active;
    public : SK_MMenuBar() {}
        ~SK_MMenuBar() { if (is_active) ImGui::EndMainMenuBar(); }

        void render(graph::SK_Graph g = graph::SK_Graph()) override {}

        bool operator()(bool in_same_line = false) override { return is_active = ImGui::BeginMainMenuBar(); }
        bool operator()(geometry::SK_Rect2D<float>, bool = false) override { return is_active = ImGui::BeginMainMenuBar(); }
        void operator~() { this->~SK_MMenuBar(); }
    };



    class SK_MenuBar : public SK_UItem<false> {
    private: bool is_active;
    public : SK_MenuBar() {}
        ~SK_MenuBar() { if (is_active) ImGui::EndMenuBar(); }

        void render(graph::SK_Graph g = graph::SK_Graph()) override {}

        bool operator()(bool in_same_line = false) override { return is_active = ImGui::BeginMenuBar(); }
        bool operator()(geometry::SK_Rect2D<float>, bool = false) override { return is_active = ImGui::BeginMenuBar(); }
        void operator~() { this->~SK_MenuBar(); }
    };



    class SK_Menu: public SK_Label<false> {
    private: bool is_active = false;
    public : SK_Menu(const char* text) : SK_Label<false>(text) {}
        ~SK_Menu() { if (is_active) ImGui::EndMenu(); }

        bool operator()(bool enable = true) override { return is_active = ImGui::BeginMenu(this->text, enable); }
        void operator~() { this->~SK_Menu(); }
    };



    class SK_MenuItem : public SK_Label<false> {
    public : SK_MenuItem(const char* text) : SK_Label<false>(text) {}
        inline bool operator()(const char* keybord_shortcut, bool selcted, bool enable) {  return ImGui::MenuItem(this->text, keybord_shortcut, selcted, enable); }
        inline bool operator()(const char* keybord_shortcut, bool enable = true) { return ImGui::MenuItem(this->text, keybord_shortcut, false, enable); }
        inline bool operator()(bool selcted, bool enable) { return ImGui::MenuItem(this->text, nullptr, selcted, enable); }
        inline bool operator()(bool enable = true) override { return ImGui::MenuItem(this->text, nullptr, false, enable); }
        using SK_Label<false>::operator();
    };
}
