solution "euchre"
  configurations { "debug", "release" }
  architecture "x64"

  filter "debug"
    defines "_DEBUG"
    warnings "Extra"
    buildoptions { "-g" }

  filter "release"
    warnings "Default"
    optimize "On"

  filter "system:linux"
    toolset "clang"
    buildoptions { "--std=c++1y"}
    linkoptions { "`pkg-config --libs sdl2`", "`pkg-config --libs glew`" }
    links { "SOIL" }

project "euchre"
  kind "ConsoleApp"
  language "C++"
  files { "EuchreCode/**.h", "EuchreCode/**.cpp" }
  includedirs { "/usr/include/SOIL" }
  objdir "build/"
  targetname "euchre"

  filter "debug"
    targetsuffix "-d"
