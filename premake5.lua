workspace "OpenGL Template"
	configurations { "Debug", "Release" }
	architecture "x86_64"

project "opengl-app"
    kind "ConsoleApp"
    language "C"
    targetdir "%{wks.location}/build/bin"
    objdir "%{wks.location}/build/bin-int"
    location "%{wks.location}/build/scripts"

	files {
		"src/**.c",
		"./build/dependencies/src/glad.c",
		"./build/dependencies/include/GLFW/glfw3.h",
		"./build/dependencies/include/GLFW/glfw3native.h",
		"./build/dependencies/src/internal.h",
		"./build/dependencies/src/platform.h",
		"./build/dependencies/src/mappings.h",
		"./build/dependencies/src/context.c",
		"./build/dependencies/src/init.c",
		"./build/dependencies/src/input.c",
		"./build/dependencies/src/monitor.c",
		"./build/dependencies/src/platform.c",
		"./build/dependencies/src/vulkan.c",
		"./build/dependencies/src/window.c",
		"./build/dependencies/src/egl_context.c",
		"./build/dependencies/src/osmesa_context.c",
		"./build/dependencies/src/null_platform.h",
		"./build/dependencies/src/null_joystick.h",
		"./build/dependencies/src/null_joystick.c",
		"./build/dependencies/src/null_init.c",
		"./build/dependencies/src/null_monitor.c",
		"./build/dependencies/src/null_window.c",
    }

	includedirs {
		"include",
		"src",
		"./build/dependencies/include",
        "./build/dependencies/include/glad",
		"./build/dependencies/include/GLFW",
		"./build/dependencies/include/KHR",
	}

	filter "system:windows"
		files {
			"./build/dependencies/src/win32_init.c",
			"./build/dependencies/src/win32_joystick.c",
			"./build/dependencies/src/win32_module.c",
			"./build/dependencies/src/win32_monitor.c",
			"./build/dependencies/src/win32_time.c",
			"./build/dependencies/src/win32_thread.c",
			"./build/dependencies/src/win32_window.c",
			"./build/dependencies/src/wgl_context.c",
			"./build/dependencies/src/egl_context.c",
			"./build/dependencies/src/osmesa_context.c"
		}

		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links {
			"Dwmapi",
			"gdi32",
			"opengl32",
		}

	filter "system:linux"
		files {
			"./build/dependencies/src/x11_init.c",
			"./build/dependencies/src/x11_monitor.c",
			"./build/dependencies/src/x11_window.c",
			"./build/dependencies/src/xkb_unicode.c",
			"./build/dependencies/src/posix_time.c",
			"./build/dependencies/src/posix_thread.c",
			"./build/dependencies/src/posix_module.c",
			"./build/dependencies/src/posix_poll.c",
			"./build/dependencies/src/glx_context.c",
			"./build/dependencies/src/egl_context.c",
			"./build/dependencies/src/osmesa_context.c",
			"./build/dependencies/src/linux_joystick.c"
		}
		defines {
			"_GLFW_X11"
		}
		links {
			"X11",
			"GL",
			"m"
		}
    