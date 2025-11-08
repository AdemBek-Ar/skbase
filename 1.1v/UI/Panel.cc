module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.panel;

import skbase.geomath;
import skbase.str.String;
import skbase.graph;
import skbase.ui.label;

import std;

export namespace UI{
	class Panel : public Label {
	public :
		using UI::Label::Label;

		~Panel() { ImGui::End(); }

		bool operator()()
			{ return ImGui::Begin(this->c_str(), nullptr, 0); }

		bool operator()(geometry::Rect2D<float> rect, ImGuiWindowFlags flags = ImGuiConfigFlags_None) {
			ImGui::SetNextWindowPos  (ImVec2{ rect.x	, rect.y	  });
			ImGui::SetNextWindowSize (ImVec2{ rect.xb, rect.yb });
			if(fixps and fixsz) {
				flags |= ImGuiWindowFlags_NoMove;
				flags |= ImGuiWindowFlags_NoResize;
				flags |= ImGuiWindowFlags_NoCollapse;
			} return this->operator()();
		}

		template<bool fixsz=true> inline void fix_size() { this->fixsz = fixsz; }
		template<bool fixps=true> inline void fix_pos () { this->fixps = fixps; }
		template<bool fixst=true> inline void fix     () {
			this->fixps = fixst;
			this->fixsz = fixst;
		}

	private:
		bool
			fixps     : 1 = false,
			fixsz     : 1 = false;
	};
}
