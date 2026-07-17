cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)


project (prop-only-ipc
	LANGUAGES CXX
	VERSION 0.0.1)

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
	GIT_TAG v0.7.5  # or specify a specific tag/commit
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
add_library(prop-only-ipc-server ${SERVER_SOURCE_FILES})
add_library(PropOnlyIpc::Server ALIAS prop-only-ipc-server)
target_include_directories(prop-only-ipc-server
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(prop-only-ipc-server
	PUBLIC
		Stinger::Utils
)

# client lib
add_library(prop-only-ipc-client ${CLIENT_SOURCE_FILES})
add_library(PropOnlyIpc::Client ALIAS prop-only-ipc-client)
target_include_directories(prop-only-ipc-client
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(prop-only-ipc-client
	PUBLIC
		Stinger::Utils
)

# example client
add_executable(prop-only-ipc-client-demo examples/client_main.cpp)
target_link_libraries(prop-only-ipc-client-demo
	PRIVATE
		PropOnlyIpc::Client
		Threads::Threads
)

add_executable(prop-only-ipc-server-demo examples/server_main.cpp)
target_link_libraries(prop-only-ipc-server-demo
	PRIVATE
		PropOnlyIpc::Server
		Threads::Threads
)

add_executable(prop-only-ipc-discovery-example examples/discovery_example.cpp)
target_link_libraries(prop-only-ipc-discovery-example
	PRIVATE
		PropOnlyIpc::Client
		Threads::Threads
)

add_executable(prop-only-ipc-discovery-publisher examples/discovery_publisher.cpp)
target_link_libraries(prop-only-ipc-discovery-publisher
	PRIVATE
		PropOnlyIpc::Client
		Threads::Threads
)

set_target_properties(
	prop-only-ipc-client
	prop-only-ipc-server
	prop-only-ipc-client-demo
	prop-only-ipc-server-demo
	prop-only-ipc-discovery-publisher
	prop-only-ipc-discovery-example
	PROPERTIES
		CXX_STANDARD 17 # use 17, if you can
		CXX_STANDARD_REQUIRED ON # use 17, I insist
		CXX_EXTENSIONS OFF # use c++17 not gnu++17
)

# help cmake find this
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/PropOnlyIpc-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/PropOnlyIpc
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

# help cmake enforce version rules
write_basic_package_version_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/PropOnlyIpc-config-version.cmake
	VERSION 0.0.1
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/PropOnlyIpc-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/PropOnlyIpc-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/PropOnlyIpc
)

install(DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/include/PropOnlyIpc
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS
	prop-only-ipc-server
	prop-only-ipc-client
	prop-only-ipc-server-demo
	prop-only-ipc-client-demo
	prop-only-ipc-discovery-example
	prop-only-ipc-discovery-publisher
	stinger_utils
	EXPORT PropOnlyIpcTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # .a files
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # .so files
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # executables
)

install(EXPORT
	PropOnlyIpcTargets
	FILE PropOnlyIpc-targets.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/PropOnlyIpc
	NAMESPACE PropOnlyIpc::
)

export (EXPORT PropOnlyIpcTargets
	FILE ${CMAKE_CURRENT_BINARY_DIR}/PropOnlyIpc-targets.cmake
	NAMESPACE PropOnlyIpc::
)