module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.combo;

import skbase.geomath;
import skbase.str.String;
import skbase.graph;
import skbase.memory.array;
import skbase.ui.label;

import std;

export namespace UI{
    class Combo: public Label {
    public :
		int selected = 0;
		using UI::Label::Label;

		template <bool in_same_line = false>
        bool operator()(geometry::Point2D<int> p) {
			this->set_pos(p);
            return this->template operator() <in_same_line> ();
        }

		inline void set(Arrays::Array<const char*> arr) { this->arr = arr; }
		inline auto get() { return this->arr; }

		inline auto operator[] (std::size_t i) { return this->arr.at(i); }
		inline void add(std::string item) { this->arr.add(item.c_str()); }

    protected:
		bool in_settup() override {
			int size = int(arr.size());
            if (ImGui::Combo(
					this->c_str(), &selected,
					arr.data(), size, size))
				return true;
            return false;
        }

    private :
	    Arrays::Array<const char*> arr;
    };
}
