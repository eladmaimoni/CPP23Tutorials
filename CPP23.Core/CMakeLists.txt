

set_source_group(debugging "debugger_trace.hpp" "debugger_trace_impl.hpp" "debugger_trace_impl.cpp")
set_source_group(formatting "formatter_specializations.hpp")
set_source_group(compiler "compiler.hpp")
set_source_group(concepts "basic_concepts.hpp")


add_library(
    CPP23.Core     
    ${debugging}
    ${formatting}
    ${compiler}
    ${concepts}
)

set_compiler_flags(CPP23.Core) 

target_include_directories(
    CPP23.Core
    PUBLIC 
    .
)

target_link_libraries(
    CPP23.Core 
    PUBLIC 
)