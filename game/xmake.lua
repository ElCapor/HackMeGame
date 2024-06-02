add_rules("mode.release", "mode.debug")


add_requires("raylib") -- we gonna use raylib

target("game")
    set_kind("binary")
    set_languages("cxx23")
    add_files("src/**.cpp") -- search for all cpp files in subdirectories recursively
    add_includedirs("include") -- include is a include directory too cuz why not

    add_packages("raylib")

