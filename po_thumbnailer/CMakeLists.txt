add_definitions(-DTRANSLATION_DOMAIN="pothumbnail")

kcoreaddons_add_plugin(pothumbnail INSTALL_NAMESPACE "kf6/thumbcreator")

target_sources(pothumbnail PRIVATE
    pocreator.cpp
)

ki18n_wrap_ui(pothumbnail pocreatorform.ui)

kconfig_add_kcfg_files(pothumbnail pocreatorsettings.kcfgc)

target_link_libraries(pothumbnail
    Qt::Widgets
    KF6::I18n
    KF6::ConfigGui
    KF6::KIOGui
    KF6::WidgetsAddons
    GettextPO::GettextPO
)

install(FILES pocreatorsettings.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
