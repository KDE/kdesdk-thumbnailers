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

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
