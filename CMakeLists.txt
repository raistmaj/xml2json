cmake_minimum_required(VERSION 3.3)
project(cudason)

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
        cudason.cpp cudason.h
        cudaxml.cpp cudaxml.h
        cudaxmltype.h cudaxmltypestring.h
        cudaxmltypeinteger.h cudaxmltypefloat.h
        cudaxmltypelist.h cudaxmltypeclass.h
        cudaxmltypeboolean.h cudaxmltypes.h
        cudasonoutputengine.h
        cudaxmltags.h cudaxmltyperefclass.h
        ${PUGIFILES}
        cudaxmltypeinteger32.h)
add_executable(cudason ${SOURCE_FILES})

target_link_libraries(cudason boost_program_options)