add_library(${LIBRARY_TARGET_NAME} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include/icontroller.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/iusecase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/ipresenter.hpp
)

target_sources(${LIBRARY_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/delegate.hpp
)

target_include_directories(${LIBRARY_TARGET_NAME} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)