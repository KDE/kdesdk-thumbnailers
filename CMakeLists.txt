cmake_minimum_required(VERSION 3.16)

# Release service version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "23")
set(RELEASE_SERVICE_VERSION_MINOR "04")
set(RELEASE_SERVICE_VERSION_MICRO "3")

set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kdesdk-thumbnailers VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "5.15.0")
set(KF_MIN_VERSION "5.85.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMOptionalAddSubdirectory)
include(FeatureSummary)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets)
find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS Config I18n KIO)

find_package(GettextPO)
set_package_properties("gettext-po"    PROPERTIES
                       DESCRIPTION "A library for processing PO files"
                       URL         "www.gnu.org/software/gettext"
                       TYPE        OPTIONAL
                       PURPOSE     "Required to build the PO thumbnailer"
)

add_definitions(
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050F00
    -DQT_DEPRECATED_WARNINGS_SINCE=0x060000
    -DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055500
    -DKF_DEPRECATED_WARNINGS_SINCE=0x060000
)

if(GETTEXTPO_FOUND)
ecm_optional_add_subdirectory(po_thumbnailer)
endif()

ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
