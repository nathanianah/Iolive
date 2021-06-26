cmake_minimum_required(VERSION 3.16)

# using c++17
IF (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
ENDIF(NOT DEFINED CMAKE_CXX_STANDARD)

IF (WIN32)
	add_executable(IonetClient Source/IonetClientMain.cpp)
	target_include_directories(IonetClient
	PRIVATE
		${IONET_INCLUDE_DIR}
		${IONET_DEPENDENCIES_PATH}/asio/include
	)
	target_link_libraries(IonetClient
		${CMAKE_THREAD_LIBS_INIT}
		Ionet
	)
ENDIF()

add_library(Ionet)
add_executable(IonetServer Source/IonetServer.cpp)

set(IONET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include/ CACHE INTERNAL "")
set(IONET_DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies CACHE INTERNAL "")

find_package(Threads REQUIRED)

target_sources(Ionet
PRIVATE
	"Include/Ionet/IonetCommon.hpp"
	"Include/Ionet/RoomManager.hpp"
	${IONET_INCLUDE_DIR}/Ionet/IonetClient.hpp
	Source/IonetClient.cpp
	Source/IonetServer.cpp
	"Source/IonetCommon.cpp"
	"Source/RoomManager.cpp"
)

target_include_directories(Ionet
PUBLIC
    ${IONET_INCLUDE_DIR}
    ${IONET_DEPENDENCIES_PATH}/asio/include
)

target_include_directories(IonetServer
PRIVATE
    ${IONET_INCLUDE_DIR}
    ${IONET_DEPENDENCIES_PATH}/asio/include
)


target_link_libraries(Ionet
    ${CMAKE_THREAD_LIBS_INIT}
)
target_link_libraries(IonetServer
    ${CMAKE_THREAD_LIBS_INIT}
    Ionet
)