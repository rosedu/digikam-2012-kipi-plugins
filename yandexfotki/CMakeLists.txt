SUBDIRS(icons)

SET(kipiplugin_yandexfotki_PART_SRCS
    plugin_yandexfotki.cpp
    logindialog.cpp
    yfalbum.cpp
    yfphoto.cpp
    yftalker.cpp
    yfwindow.cpp
    yfalbumdialog.cpp
    yandexrsa.cpp
    yandexauth.cpp
   )

KDE4_ADD_PLUGIN(kipiplugin_yandexfotki ${kipiplugin_yandexfotki_PART_SRCS})

TARGET_LINK_LIBRARIES(kipiplugin_yandexfotki
                      ${KDCRAW_LIBRARIES}
                      ${KIPI_LIBRARIES}
                      ${KEXIV2_LIBRARIES}
                      ${QCA2_LIBRARIES}
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KIO_LIBS}
                      kipiplugins
                     )

INCLUDE_DIRECTORIES(
    ${QCA2_INCLUDE_DIR}
    )

ADD_DEFINITIONS(
    ${QCA2_DEFINITIONS}
    )

INSTALL(TARGETS kipiplugin_yandexfotki
        DESTINATION ${PLUGIN_INSTALL_DIR})

INSTALL(FILES kipiplugin_yandexfotki.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
