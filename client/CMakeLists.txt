include(findcoredeps)
include(findswigdeps)

if (BUILD_SWIG_LIB)
    set_property(SOURCE ovpncli.i PROPERTY CPLUSPLUS ON)
    if (NOT MSVC)
        # Swig generates code with deprecated python declarations
        set_property(SOURCE ovpncli.i PROPERTY GENERATED_COMPILE_OPTIONS -Wno-deprecated-declarations -Wno-sometimes-uninitialized -Wno-class-memaccess -Wno-unknown-warning-option)
    endif()
	
    add_library(openvpn_plugin SHARED IMPORTED)
    set_target_properties(openvpn_plugin PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/pluginjni/${ANDROID_ABI}/libck-ovpn-plugin.so)
    message(${CMAKE_BINARY_DIR})

    add_library(ovpnclilib SHARED
          ovpncli.cpp
          ovpncli_wrap.cxx
          ovpncli_wrap.h
         )

    add_core_dependencies(ovpnclilib)
    link_directories(${CMAKE_SOURCE_DIR}/pluginjni/${ANDROID_ABI}/)
    target_link_libraries(ovpnclilib ${PYTHON_LIBRARIES} libck-ovpn-plugin.so)
    target_include_directories(ovpnclilib PRIVATE ${PYTHON_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
	
    # Use proper python library name to generate _ovpncli.so/dylib/dll
    set_target_properties(ovpnclilib PROPERTIES OUTPUT_NAME "_ovpncli")
    set_target_properties(ovpnclilib PROPERTIES PREFIX "")

endif ()
