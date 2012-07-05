dofile("./glsdk/links.lua")

solution "EngineTest"
	configurations {"Debug", "Release"}

project "EngineTest"
	kind "ConsoleApp"
	language "c++"
	files {"*.cpp", "*.h"}

	UseLibs {"glload", "glimage", "glm", "glutil", "glmesh", "freeglut"}

	configuration "windows"
		defines "WIN32"
		links {"glu32", "opengl32", "gdi32", "winmm", "user32"}

	configuration "Debug"
		targetsuffix "D"
		defines "_DEBUG"

	configuration "Release"
		defines "NDEBUG"
		