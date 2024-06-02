-- the actual cheat to test against
add_rules("mode.release", "mode.debug")

target("cheat")
    set_kind("binary")
    set_languages("cxx23")

    add_files("src/**.cpp")
    add_includedirs("include")