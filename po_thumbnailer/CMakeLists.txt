add_definitions(-DTRANSLATION_DOMAIN="pothumbnail")

kcoreaddons_add_plugin(pothumbnail INSTALL_NAMESPACE "kf6/thumbcreator")

target_sources(pothumbnail PRIVATE
    pocreator.cpp
)

target_link_libraries(pothumbnail
    KF6::KIOGui
    GettextPO::GettextPO
)
