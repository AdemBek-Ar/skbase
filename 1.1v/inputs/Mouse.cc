module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#undef __DEPRECATED

export module skbase.input.mouse;

import skbase.base;
import skbase.json;
import skbase.geomath.point;
import skbase.memory.array;
import std;

export namespace Input{
    class Mouse : public geometry::Point2D<double> {
    public :
        std::unordered_map<int, bool> buttonStates;
        double scrollX, scrollY;

        Mouse() : geometry::Point2D<double>(), scrollX(0), scrollY(0) {}

        inline geometry::PointF2D get_pos_as_draw_coords(geometry::Point2D<int> p) { return get_pos_as_draw_coords(p.x, p.y); }
        inline geometry::PointF2D get_pos_as_draw_coords(int width, int height)
            { return geometry::PointF2D((2.0f * this->x / width) - 1.0f, 1.0f - (2.0f * this->y / height)); }
        inline geometry::PointF2D get_pos_as_draw_coords(GLFWwindow* windowHandle) {
            geometry::Point2D<int> p;
            glfwGetWindowSize(windowHandle, &p.x, &p.y);
            return get_pos_as_draw_coords(p.x, p.y);
        }

        inline geometry::PointF2D get_pos_as_drawrect_coords(GLFWwindow* windowHandle, geometry::Rect2D<int> r)
            { return get_pos_as_drawrect_coords(windowHandle, r.x, r.y, r.width, r.height); }
        inline geometry::PointF2D get_pos_as_drawrect_coords(geometry::Rect2D<int> r)
            { return get_pos_as_drawrect_coords(r.x, r.y, r.width, r.height); }
        inline geometry::PointF2D get_pos_as_drawrect_coords(int x, int y, int width, int height)
            { return geometry::PointF2D((2.0f * (this->x-x) / width) - 1.0f, 1.0f - (2.0f * (this->y-y) / height)); }
        inline geometry::PointF2D get_pos_as_drawrect_coords(GLFWwindow* windowHandle, int x, int y, int width, int height) {
            geometry::Point2D<int> p;
            glfwGetWindowSize(windowHandle, &p.x, &p.y);
            return geometry::PointF2D((2.0f * (this->x-x) / width) - 1.0f, (2.0f * (p.y-this->y-y) / height) - 1.0f);
        }
        inline bool is_button_pressed_once(GLFWwindow* windowHandle, int button) { return glfwGetMouseButton(windowHandle, button) == GLFW_PRESS and not buttonStates[button]; }
        inline bool is_button_hold(GLFWwindow* windowHandle, int button) { return glfwGetMouseButton(windowHandle, button) == GLFW_PRESS and buttonStates[button]; }
        inline bool is_button_pressed(GLFWwindow* windowHandle, int button) { return glfwGetMouseButton(windowHandle, button) == GLFW_PRESS; }

        inline void update_btn_startes(GLFWwindow* windowHandle) { for (int button = 0; button < 8; ++button) buttonStates[button] = is_button_pressed(windowHandle, button); }
        inline void update_scroll_offset(double xoffset, double yoffset) { scrollX = xoffset; scrollY = yoffset; }
        inline void update_position(GLFWwindow* windowHandle) { glfwGetCursorPos(windowHandle, &this->x, &this->y); }
        inline geometry::Point2D<double> update(GLFWwindow* windowHandle) {
            update_btn_startes(windowHandle);
            update_position(windowHandle);
            update_scroll_offset(this->x, this->y);
            return static_cast<geometry::Point2D<double>>(*this);
        }

        inline void reset_scroll_offset() { scrollX = scrollY = 0; }
        // will add get keys and position states map as json next
    };
}
