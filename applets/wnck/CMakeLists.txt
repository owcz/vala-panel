option(ENABLE_WNCK "Enables wnck-related plugins, like taskbar and pager" ON)

if (ENABLE_WNCK)
    pkg_check_modules(WNCK libwnck-3.0)
    if (NOT(WNCK_FOUND))
        set(ENABLE_WNCK OFF)
    endif()
endif()

if (ENABLE_WNCK)
    add_subdirectory(tasklist)
    add_subdirectory(pager)
    add_subdirectory(icontasks)
    add_subdirectory(deskno)
    add_subdirectory(wincmd)
    add_subdirectory(buttons)
endif()

option(ENABLE_X11 "Enables X11 system tray" ON)

if (ENABLE_X11)
    pkg_check_modules(X11 x11 gdk-x11-3.0)
    if (NOT(X11_FOUND))
        set(ENABLE_X11 OFF)
    endif()
endif()

if (ENABLE_X11)
    add_subdirectory(xembed)
endif()
