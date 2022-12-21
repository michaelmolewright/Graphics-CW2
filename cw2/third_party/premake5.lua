-- Third party builds

includedirs( "stb/include" );
includedirs( "glad/include" );
includedirs( "glfw/include" );
includedirs( "rapidobj/include" );
includedirs( "imgui/include" );

project( "x-stb" )
	kind "StaticLib"

	location "."

	filter "toolset:msc-*"
		-- This is a third party project. We don't fix "upstreams" warnings, as
		-- the upstreams project may have different standards w.r.t. warning
		-- levels. Instead, we disable them specifically for this subproject.
		buildoptions {
			"/wd4204", -- nonstandard extension: non-constant aggregate
		}
	filter "*"

	files( "stb/src/*.c" )

project( "x-glad" )
	kind "StaticLib"

	location "."

	files( "glad/src/*.c" )

project( "x-glfw" )
	kind "StaticLib"

	location "."

	filter "system:linux"
		defines { "_GLFW_X11=1" }

	filter "system:windows"
		defines { "_GLFW_WIN32=1" }

	filter "*"

	filter "toolset:msc-*"
		-- See comment on warnings above.
		buildoptions {
			"/wd4100", -- unreferenced formal parameter
			"/wd4201", -- nonstandard extension: nameless struct/union
			"/wd4204", -- nonstandard extension: non-constant aggregate
			"/wd4244", -- conversion from X to Y, possible loss of data
			"/wd4706", -- assignment within condition expression
		}
	filter "*"

	files {
		"glfw/src/context.c",
		"glfw/src/egl_context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/internal.h",
		"glfw/src/mappings.h",
		"glfw/src/monitor.c",
		"glfw/src/null_init.c",
		"glfw/src/null_joystick.c",
		"glfw/src/null_joystick.h",
		"glfw/src/null_monitor.c",
		"glfw/src/null_platform.h",
		"glfw/src/null_window.c",
		"glfw/src/platform.c",
		"glfw/src/platform.h",
		"glfw/src/vulkan.c",
		"glfw/src/window.c",
		"glfw/src/osmesa_context.c"
	};

	filter "system:linux"
		files {
			"glfw/src/posix_*",
			"glfw/src/x11_*", 
			"glfw/src/xkb_*",
			"glfw/src/glx_*",
			"glfw/src/linux_*",
		};
	filter "system:windows"
		files {
			"glfw/src/win32_*",
			"glfw/src/wgl_*", 
		};

	filter "*"

project( "x-rapidobj" )
	kind "Utility"

	location "."

	files( "rapidobj/include/**.h*" )


project( "x-imgui" )
	kind "StaticLib"

	location "."

	files {
		"imgui/include/imconfig.h",
		"imgui/include/imgui_demo.cpp",
		"imgui/include/imgui_draw.cpp",
		"imgui/include/imgui_impl_glfw.cpp",
		"imgui/include/imgui_impl_glfw.h",
		"imgui/include/imgui_impl_opengl3.cpp",
		"imgui/include/imgui_impl_opengl3.h",
		"imgui/include/imgui_internal.h",
		"imgui/include/imgui_tables.cpp",
		"imgui/include/imgui_widgets.cpp",
		"imgui/include/imgui.cpp",
		"imgui/include/imgui.h",
		"imgui/include/imstb_rectpack.h",
		"imgui/include/imstb_textedit.h",
		"imgui/include/imstb_truetype.h",
		"imgui/include/imgui_impl_opengl3_loader.h"
	};

--EOF
