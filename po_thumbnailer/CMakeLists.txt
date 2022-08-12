add_definitions(-DTRANSLATION_DOMAIN="pothumbnail")

kcoreaddons_add_plugin(pothumbnail INSTALL_NAMESPACE "kf${QT_MAJOR_VERSION}/thumbcreator")

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
    GettextPO::GettextPO
)

install(FILES pocreatorsettings.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
