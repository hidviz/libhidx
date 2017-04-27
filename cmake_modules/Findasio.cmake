
find_path( ASIO_INCLUDE_DIR asio.hpp HINTS "${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include" /usr/include /usr/local/include )

if( ASIO_INCLUDE_DIR )
    set( ASIO_FOUND TRUE )
    set( ASIO_INCLUDE_DIRS ${ASIO_INCLUDE_DIR} )
endif( ASIO_INCLUDE_DIR )

if( ASIO_FOUND )
    if( NOT ASIO_FIND_QUIETLY )
        message( STATUS "Found ASIO: ${ASIO_INCLUDE_DIR}" )
    endif( NOT ASIO_FIND_QUIETLY )

    add_library(asio INTERFACE)
    target_include_directories(asio INTERFACE "${ASIO_INCLUDE_DIR}")
    target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

    if(WIN32)
        target_link_libraries(asio INTERFACE ws2_32)
    endif(WIN32)

else( ASIO_FOUND )

    if( ASIO_FIND_REQUIRED )
        message( FATAL_ERROR "Could not find Asio-lib" )
    endif( ASIO_FIND_REQUIRED )

endif( ASIO_FOUND )
