
add_executable(
    CPP23.Tests
    "main.cpp"
    "formatting.tests.cpp"
    "move.tests.cpp"
    "pimpl.tests.cpp"
) 

set_compiler_flags(CPP23.Tests)   

target_compile_definitions(
    CPP23.Tests
    PRIVATE
    "-DCATCH_CONFIG_ENABLE_BENCHMARKING"
)

target_link_libraries(
    CPP23.Tests
    PRIVATE 
    CPP23.Core
    Catch2::Catch2 
    Catch2::Catch2WithMain
)
