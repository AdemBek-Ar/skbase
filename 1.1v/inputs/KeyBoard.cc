module;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#undef __DEPRECATED

export module skbase.input.keyboard;

import skbase.memory.array;
import skbase.base;
import skbase.json;
import std;

export namespace Input{
	#include "./keys.h++"

	class KeyStates {
	public:
		KeyStates() = default;
		KeyStates(bool press, bool hold, bool release, bool spam, uint un_used_space=0)
			:press(press), hold(hold), release(release), spam(spam),
			un_used_space(un_used_space) {}
		bool
			press   : 1 = false,
			hold    : 1 = false,
			release : 1 = false,
			spam    : 1 = false;
		uint un_used_space : 4 = 0;
		inline operator bool() const { return press; }
		inline json tojson() {
			json j;
			j["press"  ] = (bool)press;
			j["hold"   ] = (bool)hold;
			j["release"] = (bool)release;
			j["spam"   ] = (bool)spam;
			return j;
		}
		inline void fromjson(json j) {
			press   = j["press"  ];
			hold    = j["hold"   ];
			release = j["release"];
			spam    = j["spam"   ];
		}
	};



    class KeyBoard {
    public :
        KeyBoard() {}

        inline bool is_key(GLFWwindow* windowHandle, int key, int action=key::Pressed) { return glfwGetKey(windowHandle, key) == action; }
        inline bool is_pressed(GLFWwindow* windowHandle, int key) { return glfwGetKey(windowHandle, key) == key::Pressed; }
        inline bool is_held(int key) { return keyStates[key].hold; }

		template <std::size_t size=100, const std::array<int, size>& arr= key::default_keys_list>
		inline void update(GLFWwindow* windowHandle) {
			for (int i : arr) keyStates[i] = KeyStates(
				is_key(windowHandle, i, key::Pressed),
				keyStates[i].press and is_key(windowHandle, i),
				is_key(windowHandle, i, key::Released),
				is_key(windowHandle, i, key::Spamed)
			);
		}

		template <std::size_t size=100, const std::array<int, size>& arr= key::default_keys_list>
		inline Arrays::Array<json> update_state(GLFWwindow* windowHandle) {
			Arrays::Array<json> ret;
			for (int i : arr) ret.add((keyStates[i] = KeyStates(
				is_key(windowHandle, i, key::Pressed),
				keyStates[i].press and is_key(windowHandle, i),
				is_key(windowHandle, i, key::Released),
				is_key(windowHandle, i, key::Spamed)
			)).tojson());
			return ret;
		}

    protected:
        std::unordered_map<int, KeyStates> keyStates;
    };
}
