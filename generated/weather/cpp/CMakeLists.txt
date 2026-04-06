cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)


project (weather-ipc
	LANGUAGES CXX
	VERSION 0.1.2)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# more secure code
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)
find_package(RapidJSON REQUIRED)

# Fetch stinger-cpp-utils dependency
include(FetchContent)
FetchContent_Declare(
	stinger-cpp-utils
	GIT_REPOSITORY https://github.com/stinger-ipc/stinger-cpp-utils.git
	GIT_TAG main  # or specify a specific tag/commit
)
FetchContent_MakeAvailable(stinger-cpp-utils)

set(CLIENT_SOURCE_FILES
	src/client.cpp
	src/discovery.cpp
	src/method_payloads.cpp
	src/property_structs.cpp
	src/signal_payloads.cpp
	src/structs.cpp
	
)

set(SERVER_SOURCE_FILES
	src/discovery.cpp
	src/method_payloads.cpp
	src/property_structs.cpp
	src/server.cpp
	src/signal_payloads.cpp
	src/structs.cpp
	
)

# server lib
add_library(weather-ipc-server ${SERVER_SOURCE_FILES})
add_library(WeatherIpc::Server ALIAS weather-ipc-server)
target_include_directories(weather-ipc-server
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(weather-ipc-server
	PUBLIC
		Stinger::Utils
)

# client lib
add_library(weather-ipc-client ${CLIENT_SOURCE_FILES})
add_library(WeatherIpc::Client ALIAS weather-ipc-client)
target_include_directories(weather-ipc-client
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(weather-ipc-client
	PUBLIC
		Stinger::Utils
)

# example client
add_executable(weather-ipc-client-demo examples/client_main.cpp)
target_link_libraries(weather-ipc-client-demo
	PRIVATE
		WeatherIpc::Client
		Threads::Threads
)

add_executable(weather-ipc-server-demo examples/server_main.cpp)
target_link_libraries(weather-ipc-server-demo
	PRIVATE
		WeatherIpc::Server
		Threads::Threads
)

add_executable(weather-ipc-discovery-example examples/discovery_example.cpp)
target_link_libraries(weather-ipc-discovery-example
	PRIVATE
		WeatherIpc::Client
		Threads::Threads
)

add_executable(weather-ipc-discovery-publisher examples/discovery_publisher.cpp)
target_link_libraries(weather-ipc-discovery-publisher
	PRIVATE
		WeatherIpc::Client
		Threads::Threads
)

set_target_properties(
	weather-ipc-client
	weather-ipc-server
	weather-ipc-client-demo
	weather-ipc-server-demo
	weather-ipc-discovery-publisher
	weather-ipc-discovery-example
	PROPERTIES
		CXX_STANDARD 17 # use 17, if you can
		CXX_STANDARD_REQUIRED ON # use 17, I insist
		CXX_EXTENSIONS OFF # use c++17 not gnu++17
)

# help cmake find this
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/WeatherIpc-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/WeatherIpc
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

# help cmake enforce version rules
write_basic_package_version_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/WeatherIpc-config-version.cmake
	VERSION 0.1.2
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/WeatherIpc-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/WeatherIpc-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/WeatherIpc
)

install(DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/include/WeatherIpc
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS
	weather-ipc-server
	weather-ipc-client
	weather-ipc-server-demo
	weather-ipc-client-demo
	weather-ipc-discovery-example
	weather-ipc-discovery-publisher
	stinger_utils
	EXPORT WeatherIpcTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # .a files
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # .so files
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # executables
)

install(EXPORT
	WeatherIpcTargets
	FILE WeatherIpc-targets.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/WeatherIpc
	NAMESPACE WeatherIpc::
)

export (EXPORT WeatherIpcTargets
	FILE ${CMAKE_CURRENT_BINARY_DIR}/WeatherIpc-targets.cmake
	NAMESPACE WeatherIpc::
)