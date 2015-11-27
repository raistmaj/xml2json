cmake_minimum_required(VERSION 2.8)
project(umison)

include_directories(pugixml)
include_directories(pugixml/src)
include_directories(rapidjson/include)

if (EXISTS "${PROJECT_SOURCE_DIR}/pugixml/pugixml.hpp")
    set(PUGIFILES pugixml/pugixml.hpp pugixml/pugixml.cpp)
    message(STATUS "Pugi found at pugixml")
elseif (EXISTS "${PROJECT_SOURCE_DIR}/pugixml/src/pugixml.hpp")
    set(PUGIFILES pugixml/src/pugixml.hpp pugixml/src/pugixml.cpp)
    message(STATUS "Pugi found at pugixml/src")
else (EXISTS "${PROJECT_SOURCE_DIR}/pugixml/pugixml.hpp")
    message(STATUS "Pugi not found")
endif (EXISTS "${PROJECT_SOURCE_DIR}/pugixml/pugixml.hpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp
        umison.cpp umison.h
        umixml.cpp umixml.h
        umixmltype.h umixmltypestring.h
        umixmltypeinteger.h umixmltypefloat.h
        umixmltypelist.h umixmltypeclass.h
        umixmltypeboolean.h umixmltypes.h
        umisonoutputengine.h
        umixmltags.h umixmltyperefclass.h
        ${PUGIFILES}
        umixmltypeinteger32.h umixmltypemap.h)
add_executable(umison ${SOURCE_FILES})

target_link_libraries(umison boost_program_options)