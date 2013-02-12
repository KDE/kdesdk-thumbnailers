project(thumbnailers)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

macro_optional_find_package(GettextPO)
macro_log_feature(GETTEXTPO_FOUND "gettext-po" "A library for processing PO files" "www.gnu.org/software/gettext" FALSE "" "Required to build the PO thumbnailer")

if(GETTEXTPO_FOUND)
macro_optional_add_subdirectory(po_thumbnailer)
endif(GETTEXTPO_FOUND)

macro_display_feature_log()
