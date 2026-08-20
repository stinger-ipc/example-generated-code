cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)


project (signal-only-ipc
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
# Build the MockConnection so the generated unit tests can drive the client/server
# without a real broker.  Must be set before stinger-cpp-utils is made available.
set(STINGER_UTILS_BUILD_MOCK ON CACHE BOOL "" FORCE)
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
add_library(signal-only-ipc-server ${SERVER_SOURCE_FILES})
add_library(SignalOnlyIpc::Server ALIAS signal-only-ipc-server)
target_include_directories(signal-only-ipc-server
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(signal-only-ipc-server
	PUBLIC
		Stinger::Utils
)

# client lib
add_library(signal-only-ipc-client ${CLIENT_SOURCE_FILES})
add_library(SignalOnlyIpc::Client ALIAS signal-only-ipc-client)
target_include_directories(signal-only-ipc-client
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(signal-only-ipc-client
	PUBLIC
		Stinger::Utils
)

# example client
add_executable(signal-only-ipc-client-demo examples/client_main.cpp)
target_link_libraries(signal-only-ipc-client-demo
	PRIVATE
		SignalOnlyIpc::Client
		Threads::Threads
)

add_executable(signal-only-ipc-server-demo examples/server_main.cpp)
target_link_libraries(signal-only-ipc-server-demo
	PRIVATE
		SignalOnlyIpc::Server
		Threads::Threads
)

add_executable(signal-only-ipc-discovery-example examples/discovery_example.cpp)
target_link_libraries(signal-only-ipc-discovery-example
	PRIVATE
		SignalOnlyIpc::Client
		Threads::Threads
)

add_executable(signal-only-ipc-discovery-publisher examples/discovery_publisher.cpp)
target_link_libraries(signal-only-ipc-discovery-publisher
	PRIVATE
		SignalOnlyIpc::Client
		Threads::Threads
)

set_target_properties(
	signal-only-ipc-client
	signal-only-ipc-server
	signal-only-ipc-client-demo
	signal-only-ipc-server-demo
	signal-only-ipc-discovery-publisher
	signal-only-ipc-discovery-example
	PROPERTIES
		CXX_STANDARD 17 # use 17, if you can
		CXX_STANDARD_REQUIRED ON # use 17, I insist
		CXX_EXTENSIONS OFF # use c++17 not gnu++17
)

# help cmake find this
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/SignalOnlyIpc-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/SignalOnlyIpc
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

# help cmake enforce version rules
write_basic_package_version_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/SignalOnlyIpc-config-version.cmake
	VERSION 0.0.1
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/SignalOnlyIpc-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/SignalOnlyIpc-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/SignalOnlyIpc
)

install(DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/include/SignalOnlyIpc
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS
	signal-only-ipc-server
	signal-only-ipc-client
	signal-only-ipc-server-demo
	signal-only-ipc-client-demo
	signal-only-ipc-discovery-example
	signal-only-ipc-discovery-publisher
	stinger_utils
	EXPORT SignalOnlyIpcTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # .a files
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # .so files
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # executables
)

install(EXPORT
	SignalOnlyIpcTargets
	FILE SignalOnlyIpc-targets.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/SignalOnlyIpc
	NAMESPACE SignalOnlyIpc::
)

export (EXPORT SignalOnlyIpcTargets
	FILE ${CMAKE_CURRENT_BINARY_DIR}/SignalOnlyIpc-targets.cmake
	NAMESPACE SignalOnlyIpc::
)

# ---------------------------- Unit tests ----------------------------
# Tests are built and registered with CTest by default.  Configure with
# -DBUILD_TESTING=OFF to skip them (e.g. when consuming this as a subproject).
include(CTest)
if(BUILD_TESTING)
	# Fetch GoogleTest.  If a dependency (e.g. stinger-cpp-utils) has already
	# made it available, this re-declaration is a no-op and the existing
	# GTest::* targets are reused.
	FetchContent_Declare(
		googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG v1.15.2
	)
	# Prevent GoogleTest from overriding the parent project's settings on Windows.
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	add_executable(signal_only_unit_tests
		tests/test_client.cpp
		tests/test_serialization.cpp
		tests/test_server.cpp
		
	)
	target_link_libraries(signal_only_unit_tests
		PRIVATE
			SignalOnlyIpc::Client
			SignalOnlyIpc::Server
			GTest::gtest_main
			Threads::Threads
	)
	set_target_properties(signal_only_unit_tests
		PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			CXX_EXTENSIONS OFF
	)

	include(GoogleTest)
	gtest_discover_tests(signal_only_unit_tests)
endif()