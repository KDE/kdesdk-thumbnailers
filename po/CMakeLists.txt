project(kde-thumbnailer-po)
cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
find_package(GettextPO REQUIRED)

include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${QT_INCLUDES} ${KDE4_INCLUDES})

set(pothumbnail_SRCS pocreator.cpp)

kde4_add_ui_files(pothumbnail_SRCS pocreatorform.ui)

kde4_add_kcfg_files(pothumbnail_SRCS pocreatorsettings.kcfgc)

kde4_add_plugin(pothumbnail ${pothumbnail_SRCS})
target_link_libraries(pothumbnail
    ${KDE4_KIO_LIBS}
    ${GETTEXTPO_LIBRARY}
)

install(TARGETS pothumbnail DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES pocreatorsettings.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(FILES pothumbnail.desktop DESTINATION ${SERVICES_INSTALL_DIR})
