module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#undef __DEPRECATED

export module skbase.input.joystick;

import skbase.base;
//import skbase.json;
import skbase.memory.array;
import std;

export namespace Input{
    class Joystick {
    public :
        int joystickId;
        bool isConnected;
        Arrays::Array<float> axes;
        Arrays::Array<byte> buttons;

        Joystick(int jid = 0) : joystickId(jid), isConnected(false) { reconnect(); }

        inline bool is_button_pressed(int buttonIndex)
            { return isConnected and buttonIndex < int(buttons.size()) and buttons[buttonIndex] == GLFW_PRESS; }
        inline float get_axis_value(int axisIndex)
            { return (isConnected and axisIndex < int(axes.size())) ? axes[axisIndex] : 0.0f; }
        inline const char* get_joystick_name()
            { return isConnected ? glfwGetJoystickName(joystickId) : nullptr; }
        inline void reconnect() {
            isConnected = glfwJoystickPresent(joystickId);
            if (isConnected) {
                int axesCount;
                int buttonCount;
                const float* axesData = glfwGetJoystickAxes(joystickId, &axesCount);
                const byte* buttonData = glfwGetJoystickButtons(joystickId, &buttonCount);
                axes = std::vector<float>(axesData, axesData + axesCount);
                buttons = std::vector<byte>(buttonData, buttonData + buttonCount);
            }
        }
        // will add get keys and axes states map as json next
    };
}
