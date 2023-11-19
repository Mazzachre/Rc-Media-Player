add_executable(rc_media_player)

target_sources(rc_media_player
    PRIVATE
    main.cpp
    app/app.cpp
    app/arguments.cpp
    mpv/mpv_controller.cpp
    mpv/mpv_player.cpp
    mpv/mpv_renderer.cpp
    socket/socket_server.cpp
    socket/command_parser.cpp
    ui/ui_state.cpp
    ui/audio.cpp
    ui/video.cpp
    ui/subtitle.cpp
    resources.qrc
)

target_link_libraries(rc_media_player
	Qt5::Quick
	Qt5::Qml
	Qt5::Gui
	Qt5::QuickControls2
	Qt5::Widgets
	Qt5::WebSockets
	mpv
)
