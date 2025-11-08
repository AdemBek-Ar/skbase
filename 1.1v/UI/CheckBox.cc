module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.checkbox;

import skbase.geomath;
import skbase.str.String;
import skbase.graph;
import skbase.ui.label;
import skbase.ui.uitem;

import std;

#include "../defines.h++"

export namespace UI {
	class CheckBox : public Label {
	public :
		bool checked = false;
		using UI::Label::Label;
		using UI::UItem::operator();

		sget_addition(bool, checked)
	protected:
		bool in_settup() override
			{ return ImGui::Checkbox(this->c_str(), &checked); }
	};
}
