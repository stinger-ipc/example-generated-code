cmake_minimum_required(VERSION 3.16)
cmake_policy(VERSION 3.16)


project (simple-ipc
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
add_library(simple-ipc-server ${SERVER_SOURCE_FILES})
add_library(SimpleIpc::Server ALIAS simple-ipc-server)
target_include_directories(simple-ipc-server
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(simple-ipc-server
	PUBLIC
		Stinger::Utils
)

# client lib
add_library(simple-ipc-client ${CLIENT_SOURCE_FILES})
add_library(SimpleIpc::Client ALIAS simple-ipc-client)
target_include_directories(simple-ipc-client
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
		${rapidJSON_INCLUDE_DIRS}
)
target_link_libraries(simple-ipc-client
	PUBLIC
		Stinger::Utils
)

# example client
add_executable(simple-ipc-client-demo examples/client_main.cpp)
target_link_libraries(simple-ipc-client-demo
	PRIVATE
		SimpleIpc::Client
		Threads::Threads
)

add_executable(simple-ipc-server-demo examples/server_main.cpp)
target_link_libraries(simple-ipc-server-demo
	PRIVATE
		SimpleIpc::Server
		Threads::Threads
)

add_executable(simple-ipc-discovery-example examples/discovery_example.cpp)
target_link_libraries(simple-ipc-discovery-example
	PRIVATE
		SimpleIpc::Client
		Threads::Threads
)

add_executable(simple-ipc-discovery-publisher examples/discovery_publisher.cpp)
target_link_libraries(simple-ipc-discovery-publisher
	PRIVATE
		SimpleIpc::Client
		Threads::Threads
)

set_target_properties(
	simple-ipc-client
	simple-ipc-server
	simple-ipc-client-demo
	simple-ipc-server-demo
	simple-ipc-discovery-publisher
	simple-ipc-discovery-example
	PROPERTIES
		CXX_STANDARD 17 # use 17, if you can
		CXX_STANDARD_REQUIRED ON # use 17, I insist
		CXX_EXTENSIONS OFF # use c++17 not gnu++17
)

# help cmake find this
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/SimpleIpc-config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/SimpleIpc
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

# help cmake enforce version rules
write_basic_package_version_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/SimpleIpc-config-version.cmake
	VERSION 0.0.1
	COMPATIBILITY AnyNewerVersion
)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/SimpleIpc-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/SimpleIpc-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/SimpleIpc
)

install(DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/include/SimpleIpc
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS
	simple-ipc-server
	simple-ipc-client
	simple-ipc-server-demo
	simple-ipc-client-demo
	simple-ipc-discovery-example
	simple-ipc-discovery-publisher
	stinger_utils
	EXPORT SimpleIpcTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # .a files
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # .so files
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # executables
)

install(EXPORT
	SimpleIpcTargets
	FILE SimpleIpc-targets.cmake
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/SimpleIpc
	NAMESPACE SimpleIpc::
)

export (EXPORT SimpleIpcTargets
	FILE ${CMAKE_CURRENT_BINARY_DIR}/SimpleIpc-targets.cmake
	NAMESPACE SimpleIpc::
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

	add_executable(simple_unit_tests
		tests/test_client.cpp
		tests/test_serialization.cpp
		tests/test_server.cpp
		
	)
	target_link_libraries(simple_unit_tests
		PRIVATE
			SimpleIpc::Client
			SimpleIpc::Server
			GTest::gtest_main
			Threads::Threads
	)
	set_target_properties(simple_unit_tests
		PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			CXX_EXTENSIONS OFF
	)

	include(GoogleTest)
	gtest_discover_tests(simple_unit_tests)
endif()