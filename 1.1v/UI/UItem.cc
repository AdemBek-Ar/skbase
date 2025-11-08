module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.uitem;

import skbase.geomath;
import skbase.str.String;
import skbase.graph;

import std;

export namespace UI{
	class UItem {
    public :
    		UItem() = default;
		static void set_pos(geometry::Point2D<int> p)
			{ if (p.x > 0 and p.y > 0) ImGui::SetCursorPos(ImVec2(p.x, p.y)); }

		template <bool in_same_line = false>
        bool operator()() {
			bool ret = this->in_settup();
			this->NLI<in_same_line>();
            return ret;
        }
	protected:
		virtual bool in_settup() = 0;

		template <bool in_same_line = false>
		void NLI() {
            if constexpr (in_same_line)
				ImGui::SameLine();
			else 
				ImGui::Separator();
		}
    };
}
