module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.label;

import skbase.geomath;
import skbase.str.String;
import skbase.graph;
import skbase.ui.uitem;

import std;

export namespace UI{
    class Label : public UItem, public str::String {
    public :
		using str::String::String;

		template <bool in_same_line = false>
        inline bool operator() (geometry::Point2D<int> p) {
            this->set_pos(p);
            return this->operator()<in_same_line>();
        }

		template <bool in_same_line = false>
        inline bool operator() (graph::ColorF text_color) {
            ImGui::TextColored(ImVec4(text_color.r, text_color.g, text_color.b, text_color.a), this->c_str());
			this->NLI<in_same_line>();
            return true;
        }

		template <bool in_same_line = false>
        inline bool operator()(geometry::Point2D<int> p, graph::ColorF text_color) {
            this->set_pos(p);
            return this->operator()<in_same_line>(text_color);
        }

    protected:
		virtual bool in_settup() override {
            ImGui::Text(this->c_str());
            return true;
        }
    };
}
