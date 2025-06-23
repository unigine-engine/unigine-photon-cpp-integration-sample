# Copyright (C), UNIGINE. All rights reserved.

set(UNIGINE_PHOTON_VERSION "v5.0.7.2")

set(3rdphotonroot $ENV{UNIGINE_3RDPARTY_DIR}/Photon/CPP/${UNIGINE_PHOTON_VERSION})
set(photonroot ${UNIGINE_SDK_PATH}/source/PhotonSDK)

find_path(PHOTON_CHAT_INCLUDE_DIR
	NAMES
		"Chat-cpp/inc/Client.h"
	PATHS
		${photonroot}
		${3rdphotonroot}
	NO_DEFAULT_PATH
)

find_path(PHOTON_COMMON_INCLUDE_DIR
	NAMES
		"Common-cpp/inc/Base.h"
	PATHS
		${photonroot}
		${3rdphotonroot}
	NO_DEFAULT_PATH
)

find_path(PHOTON_LOADBALANCING_INCLUDE_DIR
	NAMES
		"LoadBalancing-cpp/inc/Client.h"
	PATHS
		${photonroot}
		${3rdphotonroot}
	NO_DEFAULT_PATH
)

find_path(PHOTON_PHOTON_INCLUDE_DIR
	NAMES
		"Photon-cpp/inc/PhotonPeer.h"
	PATHS
		${photonroot}
		${3rdphotonroot}
	NO_DEFAULT_PATH
)

find_library(PHOTON_CHAT_LIBRARY_DEBUG
	NAMES
		ChatDebug64
		Chat-cpp_vc14_debug_windows_md_x64
	PATHS
		${photonroot}/Chat-cpp/lib/
		${photonroot}/Chat-cpp/
		${3rdphotonroot}/Chat-cpp/lib/
	NO_DEFAULT_PATH
)

find_library(PHOTON_COMMON_LIBRARY_DEBUG
	NAMES
		CommonDebug64
		Common-cpp_vc14_debug_windows_md_x64
	PATHS
		${photonroot}/Common-cpp/lib/
		${photonroot}/Common-cpp/
		${3rdphotonroot}/Common-cpp/lib/
	NO_DEFAULT_PATH
)

find_library(PHOTON_LOADBALANCING_LIBRARY_DEBUG
	NAMES
		LoadBalancingDebug64
		LoadBalancing-cpp_vc14_debug_windows_md_x64
	PATHS
		${photonroot}/LoadBalancing-cpp/lib/
		${photonroot}/LoadBalancing-cpp/
		${3rdphotonroot}/LoadBalancing-cpp/lib/
	NO_DEFAULT_PATH
)

find_library(PHOTON_PHOTON_LIBRARY_DEBUG
	NAMES
		PhotonDebug64
		Photon-cpp_vc14_debug_windows_md_x64
	PATHS
		${photonroot}/Photon-cpp/lib/
		${photonroot}/Photon-cpp/
		${3rdphotonroot}/Photon-cpp/lib/
	NO_DEFAULT_PATH
)

find_library(PHOTON_CHAT_LIBRARY_RELEASE
	NAMES
		ChatRelease64
		Chat-cpp_vc14_release_windows_md_x64
	PATHS
		${photonroot}/Chat-cpp/lib/
		${photonroot}/Chat-cpp/
		${3rdphotonroot}/Chat-cpp/lib/
	NO_DEFAULT_PATH
)

find_library(PHOTON_COMMON_LIBRARY_RELEASE
	NAMES
		CommonRelease64
		Common-cpp_vc14_release_windows_md_x64
	PATHS
		${photonroot}/Common-cpp/lib/
		${photonroot}/Common-cpp/
		${3rdphotonroot}/Common-cpp/lib/
	NO_DEFAULT_PATH
)

find_library(PHOTON_LOADBALANCING_LIBRARY_RELEASE
	NAMES
		LoadBalancingRelease64
		LoadBalancing-cpp_vc14_release_windows_md_x64
	PATHS
		${photonroot}/LoadBalancing-cpp/lib/
		${photonroot}/LoadBalancing-cpp/
		${3rdphotonroot}/LoadBalancing-cpp/lib/
	NO_DEFAULT_PATH
)

find_library(PHOTON_PHOTON_LIBRARY_RELEASE
	NAMES
		PhotonRelease64
		Photon-cpp_vc14_release_windows_md_x64
	PATHS
		${photonroot}/Photon-cpp/lib/
		${photonroot}/Photon-cpp/
		${3rdphotonroot}/Photon-cpp/lib/
	NO_DEFAULT_PATH
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Photon
	FOUND_VAR PHOTON_FOUND
	REQUIRED_VARS
		PHOTON_CHAT_INCLUDE_DIR
		PHOTON_CHAT_LIBRARY_RELEASE
		PHOTON_CHAT_LIBRARY_DEBUG

		PHOTON_COMMON_INCLUDE_DIR
		PHOTON_COMMON_LIBRARY_RELEASE
		PHOTON_COMMON_LIBRARY_DEBUG

		PHOTON_LOADBALANCING_INCLUDE_DIR
		PHOTON_LOADBALANCING_LIBRARY_RELEASE
		PHOTON_LOADBALANCING_LIBRARY_DEBUG

		PHOTON_PHOTON_INCLUDE_DIR
		PHOTON_PHOTON_LIBRARY_RELEASE
		PHOTON_PHOTON_LIBRARY_DEBUG
	)

if(PHOTON_FOUND AND NOT TARGET UnigineExt::PHOTON_CHAT)
	add_library(UnigineExt::PHOTON_CHAT UNKNOWN IMPORTED)
	set_target_properties(UnigineExt::PHOTON_CHAT PROPERTIES
		IMPORTED_LOCATION_RELEASE ${PHOTON_CHAT_LIBRARY_RELEASE}
		IMPORTED_LOCATION_MINSIZEREL ${PHOTON_CHAT_LIBRARY_RELEASE}
		IMPORTED_LOCATION_DEBUG ${PHOTON_CHAT_LIBRARY_DEBUG}
		IMPORTED_LOCATION_RELWITHDEBINFO ${PHOTON_CHAT_LIBRARY_RELEASE}
		INTERFACE_INCLUDE_DIRECTORIES ${PHOTON_CHAT_INCLUDE_DIR}
	)

	add_library(UnigineExt::PHOTON_COMMON UNKNOWN IMPORTED)
	set_target_properties(UnigineExt::PHOTON_COMMON PROPERTIES
		IMPORTED_LOCATION_RELEASE ${PHOTON_COMMON_LIBRARY_RELEASE}
		IMPORTED_LOCATION_MINSIZEREL ${PHOTON_COMMON_LIBRARY_RELEASE}
		IMPORTED_LOCATION_DEBUG ${PHOTON_COMMON_LIBRARY_DEBUG}
		IMPORTED_LOCATION_RELWITHDEBINFO ${PHOTON_COMMON_LIBRARY_RELEASE}
		INTERFACE_INCLUDE_DIRECTORIES ${PHOTON_COMMON_INCLUDE_DIR}
	)

	add_library(UnigineExt::PHOTON_LOADBALANCING UNKNOWN IMPORTED)
	set_target_properties(UnigineExt::PHOTON_LOADBALANCING PROPERTIES
		IMPORTED_LOCATION_RELEASE ${PHOTON_LOADBALANCING_LIBRARY_RELEASE}
		IMPORTED_LOCATION_MINSIZEREL ${PHOTON_LOADBALANCING_LIBRARY_RELEASE}
		IMPORTED_LOCATION_DEBUG ${PHOTON_LOADBALANCING_LIBRARY_DEBUG}
		IMPORTED_LOCATION_RELWITHDEBINFO ${PHOTON_LOADBALANCING_LIBRARY_RELEASE}
		INTERFACE_INCLUDE_DIRECTORIES ${PHOTON_LOADBALANCING_INCLUDE_DIR}
	)

	add_library(UnigineExt::PHOTON_PHOTON UNKNOWN IMPORTED)
	set_target_properties(UnigineExt::PHOTON_PHOTON PROPERTIES
		IMPORTED_LOCATION_RELEASE ${PHOTON_PHOTON_LIBRARY_RELEASE}
		IMPORTED_LOCATION_MINSIZEREL ${PHOTON_PHOTON_LIBRARY_RELEASE}
		IMPORTED_LOCATION_DEBUG ${PHOTON_PHOTON_LIBRARY_DEBUG}
		IMPORTED_LOCATION_RELWITHDEBINFO ${PHOTON_PHOTON_LIBRARY_RELEASE}
		INTERFACE_INCLUDE_DIRECTORIES ${PHOTON_PHOTON_INCLUDE_DIR}
	)
endif()


