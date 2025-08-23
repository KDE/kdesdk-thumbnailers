cmake_minimum_required(VERSION 3.16)

# Release service version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "25")
set(RELEASE_SERVICE_VERSION_MINOR "11")
set(RELEASE_SERVICE_VERSION_MICRO "70")

set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kdesdk-thumbnailers VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMDeprecationSettings)
include(ECMOptionalAddSubdirectory)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS KIO)

option(BUILD_FUZZERS "Whether to the thumbnail build fuzzers" OFF)
option(FUZZERS_USE_QT_MINIMAL_INTEGRATION_PLUGIN "Whether to use the Qt minimal integration plugin for fuzzers" OFF)

find_package(GettextPO)
set_package_properties("gettext-po"    PROPERTIES
                       DESCRIPTION "A library for processing PO files"
                       URL         "www.gnu.org/software/gettext"
                       TYPE        OPTIONAL
                       PURPOSE     "Required to build the PO thumbnailer"
)

ecm_set_disabled_deprecation_versions(
    QT 5.15.2
    KF 5.91
)

if(GETTEXTPO_FOUND)
    ecm_optional_add_subdirectory(po_thumbnailer)
endif()

if(BUILD_FUZZERS)
    if(BUILD_SHARED_LIBS)
        message(FATAL_ERROR "Fuzzers can only be built with static libraries")
    else()
        add_subdirectory(autotests/ossfuzz)
    endif()
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
