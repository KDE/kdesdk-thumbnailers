add_definitions(-DTRANSLATION_DOMAIN="pothumbnail")

add_library(pothumbnail MODULE)

target_sources(pothumbnail PRIVATE
    pocreator.cpp
)

ki18n_wrap_ui(pothumbnail pocreatorform.ui)

kconfig_add_kcfg_files(pothumbnail pocreatorsettings.kcfgc)

target_link_libraries(pothumbnail
    Qt5::Widgets
    KF5::I18n
    KF5::ConfigGui
    KF5::KIOWidgets
    ${GETTEXTPO_LIBRARY}
)

install(TARGETS pothumbnail DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(FILES pocreatorsettings.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
install(FILES pothumbnail.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
