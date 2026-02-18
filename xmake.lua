set_project("json-c")
set_version("0.1.0")

set_languages("c89")

add_rules("mode.debug", "mode.release")

target("jsonc")
    set_kind("static")
    add_files("src/jsonc.c")
    add_includedirs("include", {public = true})

target("jsonc_tests")
    set_kind("binary")
    add_files("tests/test_jsonc.c")
    add_includedirs("include")
    add_deps("jsonc")

target("jsonc_basic")
    set_kind("binary")
    add_files("examples/basic.c")
    add_includedirs("include")
    add_deps("jsonc")