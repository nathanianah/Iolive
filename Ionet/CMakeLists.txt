add_library(Ionet)

add_executable(IonetServer Source/IonetServer.cpp)

set(IONET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Include/ CACHE INTERNAL "")
set(IONET_DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies CACHE INTERNAL "")

target_sources(Ionet
PRIVATE
	Source/IonetCommon.hpp
	Source/IonetClient.cpp
	Source/IonetServer.cpp
	Source/olcPGEX_Network.h

	${IONET_INCLUDE_DIR}/Ionet/IonetClient.hpp
)

target_include_directories(Ionet
PRIVATE
	${IONET_INCLUDE_DIR}
	${IONET_DEPENDENCIES_PATH}/asio/include
)

target_include_directories(IonetServer
PRIVATE
	${IONET_INCLUDE_DIR}
	${IONET_DEPENDENCIES_PATH}/asio/include
)