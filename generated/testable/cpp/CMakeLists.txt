cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)


project (testable-ipc
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
add_library(testable-ipc-server ${SERVER_SOURCE_FILES})
add_library(TestableIpc::Server ALIAS testable-ipc-server)
target_include_directories(testable-ipc-server
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(testable-ipc-server
	PUBLIC
		Stinger::Utils
)

# client lib
add_library(testable-ipc-client ${CLIENT_SOURCE_FILES})
add_library(TestableIpc::Client ALIAS testable-ipc-client)
target_include_directories(testable-ipc-client
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(testable-ipc-client
	PUBLIC
		Stinger::Utils
)

# example client
add_executable(testable-ipc-client-demo examples/client_main.cpp)
target_link_libraries(testable-ipc-client-demo
	PRIVATE
		TestableIpc::Client
		Threads::Threads
)

add_executable(testable-ipc-server-demo examples/server_main.cpp)
target_link_libraries(testable-ipc-server-demo
	PRIVATE
		TestableIpc::Server
		Threads::Threads
)

add_executable(testable-ipc-discovery-example examples/discovery_example.cpp)
target_link_libraries(testable-ipc-discovery-example
	PRIVATE
		TestableIpc::Client
		Threads::Threads
)

add_executable(testable-ipc-discovery-publisher examples/discovery_publisher.cpp)
target_link_libraries(testable-ipc-discovery-publisher
	PRIVATE
		TestableIpc::Client
		Threads::Threads
)

set_target_properties(
	testable-ipc-client
	testable-ipc-server
	testable-ipc-client-demo
	testable-ipc-server-demo
	testable-ipc-discovery-publisher
	testable-ipc-discovery-example
	PROPERTIES
		CXX_STANDARD 17 # use 17, if you can
		CXX_STANDARD_REQUIRED ON # use 17, I insist
		CXX_EXTENSIONS OFF # use c++17 not gnu++17
)

# help cmake find this
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/TestableIpc-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/TestableIpc
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

# help cmake enforce version rules
write_basic_package_version_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/TestableIpc-config-version.cmake
	VERSION 0.0.1
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/TestableIpc-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/TestableIpc-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/TestableIpc
)

install(DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/include/TestableIpc
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS
	testable-ipc-server
	testable-ipc-client
	testable-ipc-server-demo
	testable-ipc-client-demo
	testable-ipc-discovery-example
	testable-ipc-discovery-publisher
	stinger_utils
	EXPORT TestableIpcTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # .a files
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # .so files
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # executables
)

install(EXPORT
	TestableIpcTargets
	FILE TestableIpc-targets.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/TestableIpc
	NAMESPACE TestableIpc::
)

export (EXPORT TestableIpcTargets
	FILE ${CMAKE_CURRENT_BINARY_DIR}/TestableIpc-targets.cmake
	NAMESPACE TestableIpc::
)