module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.button;

import skbase.geomath;
import skbase.str.String;
import skbase.graph;
import skbase.ui.label;

import std;

export namespace UI {
    class Button : public Label {
    public :
		using Label::Label;

		template <
			bool in_same_line=false,
			bool is_colored=false,
			bool is_posed=false,
			bool is_sized=false>
        bool operator()(geometry::Point2D<int> p, geometry::Point2D<int> s, graph::Color<float> colors[3] = {}) {
            bool ret = in_settup_prv<is_colored, is_posed, is_sized>(p, s, colors);
			this->NLI<in_same_line>();
            return ret;
        }
        template <bool in_same_line = false>
        bool operator()(geometry::Rect2D<int> r, graph::Color<float> colors[3] = {}) {
            return this->template operator()
            <in_same_line, true, true, true>
            (r.posA, r.posB, colors);
        }

        template <bool in_same_line = false>
        bool operator()(geometry::Point2D<int> p, geometry::Point2D<int> s) {
            return this->template operator()
            <in_same_line, false, true, true>
            (p, s, {});
        }
        template <bool in_same_line = false>
        bool operator()(geometry::Rect2D<int> r) {
            return this->template operator()
            <in_same_line, false, true, true>
            (r.posA, r.posB, {});
        }

        template <bool in_same_line = false>
        bool operator()(graph::Color<float> colors[3] = {}) {
            return this->template operator()
            <in_same_line, true, false, false>
            ({}, {}, colors);
        }

    protected:
		bool in_settup() override
			{ return in_settup_prv<false, false, false>({}, {}, {}); }

    private:
        template <
			bool is_colored=false,
			bool is_posed=false,
			bool is_sized=false>
        bool in_settup_prv(geometry::Point2D<int> p, geometry::Point2D<int> s, graph::Color<float> colors[3] = {}) {
            bool ret = false;
            if constexpr (is_colored)
		        if (colors) {
		            ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(colors[0].r, colors[0].g, colors[0].b, colors[0].a));
		            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(colors[1].r, colors[1].g, colors[1].b, colors[1].a));
		            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(colors[2].r, colors[2].g, colors[2].b, colors[2].a));
		        }
            if constexpr (is_posed)
				this->set_pos(p);
			if constexpr (is_sized) {
				if (ImGui::Button(this->c_str(), ImVec2(s.x, s.y)))
					ret = true;
			} else {
				if (ImGui::Button(this->c_str()))
					ret = true;
			}
            if constexpr (is_colored)
				if (colors)
					ImGui::PopStyleColor(3);
            return ret;
        }
    };
}
