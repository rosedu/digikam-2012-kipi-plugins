
SET(PLUGIN_NAME kipiplugin_kioexportimport)

SET(${PLUGIN_NAME}_PART_SRCS
    plugin_kioexportimport.cpp
    KioExportWindow.cpp
    KioExportWidget.cpp
    KioImportWindow.cpp
    KioImportWidget.cpp
)

KDE4_ADD_PLUGIN(${PLUGIN_NAME} ${${PLUGIN_NAME}_PART_SRCS})

TARGET_LINK_LIBRARIES(${PLUGIN_NAME}
    ${KDE4_KDEUI_LIBS}
    ${KIPI_LIBRARIES}
    ${KDE4_KIO_LIBRARY}
    kipiplugins
)

INSTALL(TARGETS ${PLUGIN_NAME} DESTINATION ${PLUGIN_INSTALL_DIR})
INSTALL(FILES ${PLUGIN_NAME}.desktop DESTINATION ${SERVICES_INSTALL_DIR})
