##==============================================================================
## General.
##==============================================================================
cmake_minimum_required(VERSION 3.19)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/UnigineCMakeVariables.cmake)

project(unigine-photon-cpp-integration-sample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

##==============================================================================
## Environment.
##==============================================================================
set(UNIGINE_DOUBLE False CACHE BOOL "Double coords")
set(UNIGINE_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/../)
set(UNIGINE_BIN_DIR ${UNIGINE_SDK_PATH}bin)
set(UNIGINE_LIB_DIR ${UNIGINE_SDK_PATH}lib)
set(UNIGINE_INCLUDE_DIR ${UNIGINE_SDK_PATH}include)

##==============================================================================
## Dependencies.
##==============================================================================
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# Engine.
find_package(Engine REQUIRED MODULE QUIET)
find_package(Photon MODULE QUIET)
find_package(Threads REQUIRED QUIET)


##==============================================================================
## Target.
##==============================================================================
set(target "unigine-photon-cpp-integration-sample")

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:wmainCRTStartup")
endif()

add_executable(${target}
		${CMAKE_CURRENT_LIST_DIR}/AppSystemLogic.cpp
		${CMAKE_CURRENT_LIST_DIR}/AppSystemLogic.h
		${CMAKE_CURRENT_LIST_DIR}/AppWorldLogic.cpp
		${CMAKE_CURRENT_LIST_DIR}/AppWorldLogic.h
		${CMAKE_CURRENT_LIST_DIR}/main.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/ChatListener.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ChatListener.h
		${CMAKE_CURRENT_SOURCE_DIR}/GameChat.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/GameChat.h
		${CMAKE_CURRENT_SOURCE_DIR}/GameManager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/GameManager.h
		${CMAKE_CURRENT_SOURCE_DIR}/GamePlayer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/GamePlayer.h
		${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.h
		${CMAKE_CURRENT_SOURCE_DIR}/NetworkListener.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/NetworkListener.h
		${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Utils.h
)


target_include_directories(${target}
	PRIVATE
	${UNIGINE_INCLUDE_DIR}
	)

target_link_libraries(${target}
	PRIVATE
	Unigine::Engine
	
	)

target_compile_definitions(${target}
	PRIVATE
	$<$<BOOL:${UNIX}>:_LINUX>
	$<$<CONFIG:Debug>:DEBUG>
	$<$<NOT:$<CONFIG:Debug>>:NDEBUG>
	)

if(PHOTON_FOUND)
	target_link_libraries(${target}
		PRIVATE
		Threads::Threads
		UnigineExt::PHOTON_CHAT
		UnigineExt::PHOTON_LOADBALANCING
		UnigineExt::PHOTON_PHOTON
		UnigineExt::PHOTON_COMMON
		)
else()
	message(STATUS "\n======================================\n PHOTON was configured with errors. (PHOTON not found)\n======================================\n")
endif()

##==============================================================================
## Compiler constants.
##==============================================================================
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(UNIGINE_COMPILER_IS_MSVC TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(UNIGINE_COMPILER_IS_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(UNIGINE_COMPILER_IS_GNU TRUE)
endif()

if (UNIGINE_COMPILER_IS_MSVC)
    target_compile_definitions(${target}
	PRIVATE
	_CRT_SECURE_NO_DEPRECATE
	)
    include(ProcessorCount)
    ProcessorCount(proc_count)
    target_compile_options(${target}
	PRIVATE
	/TP               # Specifies all source files are C++.
	/FS               # Forces writes to the program database (PDB) file to be serialized through MSPDBSRV.EXE.
	/fp:fast          # Specifies floating-point behavior.
	/bigobj           # Increases the number of addressable sections in an .obj file.
	/EHsc             # Specifies the kind of exception handling used by the compiler.
	/MP${proc_count}  # Build with Multiple Processes.
	)
    unset(proc_count)
    target_link_options(${target} INTERFACE "/FIXED:NO")

elseif(UNIGINE_COMPILER_IS_GNU OR UNIGINE_COMPILER_IS_CLANG)

    target_compile_options(${target}
	PRIVATE
	-m64
	-march=athlon64
	-msse4.2
	# Optimizations.
	-ffast-math
	-fno-strict-aliasing
	$<$<NOT:$<CONFIG:Debug>>:-fomit-frame-pointer>
	# Warnings.
	$<$<CONFIG:Debug>:-Wall>
	$<$<CONFIG:Debug>:-Wno-unknown-pragmas>
	$<$<CONFIG:Debug>:-Wno-unused-parameter>
	)
endif ()

set(binary_name ${target})
string(APPEND binary_name "_x64")
set_target_properties(${target} PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${target} PROPERTIES OUTPUT_NAME ${binary_name})

set_target_properties(${target}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${UNIGINE_BIN_DIR}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${UNIGINE_BIN_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${UNIGINE_BIN_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${UNIGINE_BIN_DIR}
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${UNIGINE_BIN_DIR}
	)

unset(UNIGINE_DOUBLE CACHE)