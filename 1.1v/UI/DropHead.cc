module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.drophead;

import skbase.geomath;
import skbase.str.String;
import skbase.graph;
import skbase.ui.label;
import skbase.ui.uitem;

import std;

export namespace UI{
	class DropHead : public Label {
	public :
		int type : 8 = 0;
		using UI::Label::Label;
		using UI::UItem::operator();

		DropHead(const char* text = "title", int type = 0) : Label(text), type(type) {}
		DropHead(std::string text, int type = 0) : Label(text), type(type) {}

    protected:
		bool in_settup() override
			{ return ImGui::CollapsingHeader(this->c_str(), type); }
	};
}
