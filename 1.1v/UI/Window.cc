module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#undef __DEPRECATED

export module skbase.ui.window;

import skbase.input.keyboard;
import skbase.memory.array;
import skbase.str.String;
import skbase.geomath;
import skbase.graph;
import skbase.base;
import skbase.json;
import std;

inline void glfwErrorCallback(int error, const char* description)
	{ std::cerr << "\e[1;31mGLFW Error\e[0m:" << error << ": " << description << std::endl; }

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    float aspectRatio = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width >= height)
		glOrtho(
			-aspectRatio, aspectRatio,
			-1.0, 1.0,
			-1.0, 1.0);
    else
		glOrtho(-1.0, 1.0,
			-1.0/aspectRatio, 1.0/aspectRatio,
			-1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

export namespace UI {
    class Window : public geometry::Rect2D<int> {
    public :
		using geometry::Rect2D<int>::Rect2D;
		str_t title;

		std::function<void(graph::Graph g)> draw=[](graph::Graph){};

        Window(std::string title = "no title", int width = 0, int height = 0)
				:title(title) {
			this->height = height;
			this->width = width;
            glfwSetErrorCallback(glfwErrorCallback);
            if (!glfwInit()) {
                std::cerr << "\e[1;31mGLFW Error\e[0m: faild to setup GLFW" << std::endl;
                return;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

            this->impl__Window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
            if (this->impl__Window == NULL) {
                std::cerr << "\e[1;31mGLFW Error\e[0m: faild to create GLFW Window" << std::endl;
                glfwTerminate();
                return;
            }
            glfwMakeContextCurrent(this->impl__Window);
            glfwSwapInterval(1);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                std::cerr << "\e[1;31mGLAD Error\e[0m: Failed to initialize GLAD" << std::endl;
                glfwTerminate();
                return;
            }
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
          //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(this->impl__Window, true);
            ImGui_ImplOpenGL3_Init("#version 460");
        }
        ~Window() {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            if (impl__Window) glfwDestroyWindow(impl__Window);
            glfwTerminate();
        }

		inline void new_ui_frame() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
        }

        inline bool is_close() { return glfwWindowShouldClose(this->impl__Window); }
        inline void close() { glfwSetWindowShouldClose(this->impl__Window, true); }

        inline void render(graph::Graph g = 0) {
			this->get_size();
			new_ui_frame();
			this->draw(g);
			ImGui::Render();
        		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(this->impl__Window);
			glfwPollEvents();
        }

        inline int mainloop(bool is_running = true) {
            while (!is_close() && is_running) render();
            return 0;
        }

        inline geometry::Point2D<int> get_pos() {
            glfwGetWindowPos(impl__Window, &this->x, &this->y);
            return this->posA;
        }

        inline geometry::Point2D<int> get_size() {
            glfwGetWindowSize(impl__Window, &this->width, &this->height);
            return this->posB;
        }

        inline GLFWwindow* get() { return this->impl__Window; }

        inline void set_fullscreen(bool state = true) {
            GLFWmonitor* monitor = glfwGetWindowMonitor(impl__Window);
            if (state == true) {
                monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(impl__Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            } else this->update_this_rect();
        }

        inline void set_pos(geometry::Point2D<int> pos) {
        		this->posA = pos;
			this->update_this_pos();
		}
        inline void set_size(geometry::Point2D<int> size) {
        		this->posB = size;
			this->update_this_size();
		}

        inline void centered_draw_coords() { glfwSetFramebufferSizeCallback(this->impl__Window, framebuffer_size_callback); }
        inline geometry::PointF2D draw_coords(geometry::Point2D<int> p, geometry::Rect2D<int> r) {
            return geometry::PointF2D(
                (2.0f * (p.x-r.x) / r.width) - 1.0f,
                (2.0f * (this->height-p.y-r.y) / r.height) - 1.0f
            );
        }
        inline geometry::PointF2D draw_coords(geometry::Point2D<int> p) {
            return geometry::PointF2D(
                (2.0f * (p.x-this->x) / this->width) - 1.0f,
                (2.0f * (this->height-p.y-this->y) / this->height) - 1.0f
            );
        }
        inline geometry::Point2D<int> coords(geometry::PointF2D p) {
            return geometry::Point2D<int>(
                static_cast<int>((p.x + 1.0f) * this->width / 2.0f + this->x),
                static_cast<int>(this->height - (p.y + 1.0f) * this->height / 2.0f + this->y)
            );
        }

        inline bool is_pressed(int key)
			{ return glfwGetKey(this->impl__Window, key) == Input::key::Pressed; }
        inline bool is_key(int key, int action=Input::key::Pressed)
			{ return glfwGetKey(this->impl__Window, key) == action; }

		inline GLFWwindow* impl__() { return impl__Window; }

    private:
		GLFWwindow* impl__Window;
		inline void update_this_size() { glfwSetWindowSize(impl__Window, this->xb, this->yb);}
		inline void update_this_pos()  { glfwSetWindowPos(impl__Window, this->x, this->y);}
		inline void update_this_rect() { glfwSetWindowMonitor(
			impl__Window, nullptr,
			this->x, this->y, this->xb, this->yb,
		0);}
    };

    class KeyBoard : public Input::KeyBoard {
    public :
        using Input::KeyBoard::KeyBoard;

        inline bool is_pressed(int key) { return this->keyStates[key].press; }
        inline bool is_held(int key) { return this->keyStates[key].hold; }

		template <std::size_t size=100, const std::array<int, size>& arr= Input::key::default_keys_list>
		inline void update(Window* wind) {
			for (int i : arr) this->keyStates[i] = Input::KeyStates(
				wind->is_key(i, Input::key::Pressed),
				this->keyStates[i].press and wind->is_key(i),
				wind->is_key(i, Input::key::Released),
				wind->is_key(i, Input::key::Spamed)
			);
		}

		template <std::size_t size=100, const std::array<int, size>& arr= Input::key::default_keys_list>
		inline Arrays::Array<json> update_state(Window* wind) {
			Arrays::Array<json> ret;
			for (int i : arr) ret.add((this->keyStates[i] = Input::KeyStates(
				wind->is_key(i, Input::key::Pressed),
				this->keyStates[i].press and wind->is_key(i),
				wind->is_key(i, Input::key::Released),
				wind->is_key(i, Input::key::Spamed)
			)).tojson());
			return ret;
		}

		inline Input::KeyStates operator[] (int i)
			{ return this->keyStates[i]; }
    };
}
