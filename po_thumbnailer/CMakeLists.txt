add_definitions(-DTRANSLATION_DOMAIN="pothumbnail")

kcoreaddons_add_plugin(pothumbnail INSTALL_NAMESPACE "kf${QT_MAJOR_VERSION}/thumbcreator")

target_sources(pothumbnail PRIVATE
    pocreator.cpp
)

ki18n_wrap_ui(pothumbnail pocreatorform.ui)

kconfig_add_kcfg_files(pothumbnail pocreatorsettings.kcfgc)

target_link_libraries(pothumbnail
    Qt::Widgets
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::ConfigGui
    KF${QT_MAJOR_VERSION}::KIOGui
    KF${QT_MAJOR_VERSION}::WidgetsAddons
    GettextPO::GettextPO
)

install(FILES pocreatorsettings.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
