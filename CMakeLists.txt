cmake_minimum_required(VERSION 3.16)

project(thumbnailers)

set(QT_MIN_VERSION "5.15.0")
set(KF_MIN_VERSION "5.83.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMOptionalAddSubdirectory)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets)
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
    -DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055300
    -DKF_DEPRECATED_WARNINGS_SINCE=0x060000
)

if(GETTEXTPO_FOUND)
ecm_optional_add_subdirectory(po_thumbnailer)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
