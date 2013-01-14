cmake_minimum_required (VERSION 0.1)
project (zeromqb)

find_package(PkgConfig)

add_executable(zeromqb main.cc)
pkg_check_modules (EXTREMECUNIT ExtremeCUnit)

string(REPLACE ";" " " EXTREMECUNIT_LDFLAGS2 "${EXTREMECUNIT_LDFLAGS}" )
string(REPLACE ";" " " EXTREMECUNIT_CFLAGS2 "${EXTREMECUNIT_CFLAGS}" )
message(STATUS "CFLAGS=" ${EXTREMECUNIT_CFLAGS2})

set(CMAKE_EXE_LINKER_FLAGS "${EXTREMECUNIT_LDFLAGS2} -pie")
set(CMAKE_CXX_FLAGS "-Weffc++ -Wall -ggdb3 -std=c++0x -I./include" )
## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTREMECUNIT_CFLAGS2} ")
endif()

add_executable(test zeromqb_test.cc) 
target_link_libraries(test ${EXTREMECUNIT_LIBRARIES} )

#set_target_properties(test PROPERTIES CMAKE_CXX_FLAGS "${EXTREMECUNIT_CFLAGS} ")
#set_target_properties(test PROPERTIES CMAKE_LINKER_FLAGS "${EXTREMECUNIT_LDFLAGS2} -pie")
#set_target_properties(test PROPERTIES CMAKE_CXX_LINK_FLAGS "${EXTREMECUNIT_LDFLAGS2} -pie")

add_custom_target(run ./test)
add_custom_target(runv ./test -v)
ADD_DEPENDENCIES(run test)
ADD_DEPENDENCIES(runv test)
