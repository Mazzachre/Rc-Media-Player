#include "app.h"

#include "../mpv/mpv_player.h"
#include "../ui/ui_state.h"
#include "../ui/audio.h"
#include "../ui/video.h"
#include "../ui/subtitle.h"

#include "arguments.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QThread>

rc::app::App::App(int &argc, char **argv)
	: QApplication(argc, argv)
	, m_engine{new QQmlApplicationEngine} {
	QCommandLineParser parser;
	parser.addOptions({
		{{"p", "port"}, "Listen on <port>", "port"},
		{{"V", "Verbose"}, "Verbose terminal logging"}
	});
	parser.process(*this);

	Arguments::init(parser.value("port"), parser.isSet("Verbose"), this);

	registerQmlTypes();
	m_engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

	QQuickWindow *window = qobject_cast<QQuickWindow*>(m_engine->rootObjects().first());
	if (!window) qFatal("No root window found");
	rc::ui::UiState::instance()->init(window);

	installEventFilter(this);	
}

rc::app::App::~App() {
	m_engine->deleteLater();
}

int rc::app::App::run() {
	if (m_engine->rootObjects().isEmpty()) return -1;
	m_socketServer = new rc::socket::SocketServer(rc::app::Arguments::instance()->port());
	connectSignals();

	return exec();
}

bool rc::app::App::eventFilter(QObject* watched, QEvent* event) {
	Q_UNUSED(watched);
	if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
		return true;
	}
	return false;
}

void rc::app::App::connectSignals() {
	QObject *rootObject = m_engine->rootObjects().first();
	rc::mpv::MpvPlayer *player = rootObject->findChild<rc::mpv::MpvPlayer *>("player");
	if (!player) qFatal("Unable to find player");

	connect(m_socketServer, &rc::socket::SocketServer::playMovie, player, &rc::mpv::MpvPlayer::playMovie);
	connect(m_socketServer, &rc::socket::SocketServer::playlist, player, &rc::mpv::MpvPlayer::onPlaylist);
	connect(m_socketServer, &rc::socket::SocketServer::stopMovie, player, &rc::mpv::MpvPlayer::stopMovie);
	connect(m_socketServer, &rc::socket::SocketServer::setPause, player, &rc::mpv::MpvPlayer::setPause);
	connect(m_socketServer, &rc::socket::SocketServer::setPosition, player, &rc::mpv::MpvPlayer::changePosition);
	connect(m_socketServer, &rc::socket::SocketServer::setVideoTrack, player, &rc::mpv::MpvPlayer::changeVideoTrack);
	connect(m_socketServer, &rc::socket::SocketServer::setAudioTrack, player, &rc::mpv::MpvPlayer::changeAudioTrack);
	connect(m_socketServer, &rc::socket::SocketServer::setSubtitleTrack, player, &rc::mpv::MpvPlayer::changeSubtitleTrack);
	connect(m_socketServer, &rc::socket::SocketServer::setVolume, player, &rc::mpv::MpvPlayer::changeVolume);
	connect(m_socketServer, &rc::socket::SocketServer::setFullScreen, rc::ui::UiState::instance(), &rc::ui::UiState::setFullScreen);

	connect(player, &rc::mpv::MpvPlayer::audioTrackChanged, m_socketServer, &rc::socket::SocketServer::audioTrackChanged);
	connect(player, &rc::mpv::MpvPlayer::videoTrackChanged, m_socketServer, &rc::socket::SocketServer::videoTrackChanged);
	connect(player, &rc::mpv::MpvPlayer::subtitleTrackChangedExt, m_socketServer, &rc::socket::SocketServer::subtitleTrackChanged);
	connect(player, &rc::mpv::MpvPlayer::positionChanged, m_socketServer, &rc::socket::SocketServer::playing);
	connect(player, &rc::mpv::MpvPlayer::volumeChanged, m_socketServer, &rc::socket::SocketServer::volumeChanged);
	connect(player, &rc::mpv::MpvPlayer::movieStopped, m_socketServer, &rc::socket::SocketServer::playingStopped);
	connect(player, &rc::mpv::MpvPlayer::error, m_socketServer, &rc::socket::SocketServer::reportError);

	connect(player, &rc::mpv::MpvPlayer::videoTracksLoaded, rc::ui::Video::instance(), &rc::ui::Video::setTracks);
	connect(player, &rc::mpv::MpvPlayer::videoTrackChanged, rc::ui::Video::instance(), &rc::ui::Video::setSelected);
	connect(player, &rc::mpv::MpvPlayer::paused, rc::ui::Video::instance(), &rc::ui::Video::paused);
	connect(player, &rc::mpv::MpvPlayer::isPlaylist, rc::ui::Video::instance(), &rc::ui::Video::setPlaylist);
	connect(player, &rc::mpv::MpvPlayer::positionChanged, rc::ui::Video::instance(), &rc::ui::Video::positionChanged);
	connect(player, &rc::mpv::MpvPlayer::durationChanged, rc::ui::Video::instance(), &rc::ui::Video::durationChanged);
	connect(rc::ui::Video::instance(), &rc::ui::Video::positionSet, player, &rc::mpv::MpvPlayer::changePosition);

	connect(player, &rc::mpv::MpvPlayer::audioTracksLoaded, rc::ui::Audio::instance(), &rc::ui::Audio::setTracks);
	connect(player, &rc::mpv::MpvPlayer::audioTrackChanged, rc::ui::Audio::instance(), &rc::ui::Audio::setSelected);
	connect(player, &rc::mpv::MpvPlayer::muted, rc::ui::Audio::instance(), &rc::ui::Audio::muted);
	connect(player, &rc::mpv::MpvPlayer::volumeChanged, rc::ui::Audio::instance(), &rc::ui::Audio::volumeChanged);
	connect(rc::ui::Audio::instance(), &rc::ui::Audio::volumeSet, player, &rc::mpv::MpvPlayer::changeVolume);

	connect(player, &rc::mpv::MpvPlayer::subtitleTracksLoaded, rc::ui::Subtitle::instance(), &rc::ui::Subtitle::setTracks);
	connect(player, &rc::mpv::MpvPlayer::subtitleTrackChanged, rc::ui::Subtitle::instance(), &rc::ui::Subtitle::setSelected);
	connect(player, &rc::mpv::MpvPlayer::movieStarted, rc::ui::Subtitle::instance(), &rc::ui::Subtitle::movieStarted);
	connect(rc::ui::Subtitle::instance(), &rc::ui::Subtitle::removeSubtitleTrack, player, &rc::mpv::MpvPlayer::removeSubtitleTrack);
	
	connect(m_socketServer, &rc::socket::SocketServer::playMovie, rc::ui::UiState::instance(), &rc::ui::UiState::onMovieStart);
	connect(player, &rc::mpv::MpvPlayer::movieStopped, rc::ui::UiState::instance(), &rc::ui::UiState::onMovieStop);
}

void rc::app::App::registerQmlTypes() {
	qmlRegisterType<rc::mpv::MpvPlayer>("rc", 1, 0, "MpvPlayer");

	qmlRegisterSingletonType<rc::ui::UiState>("rc", 1, 0, "UiState",
		[](QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> QObject* {
			Q_UNUSED(qmlEngine);
			Q_UNUSED(jsEngine);
			return rc::ui::UiState::instance();
		}
	);

	qmlRegisterSingletonType<rc::ui::Audio>("rc", 1, 0, "Audio",
		[](QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> QObject* {
			Q_UNUSED(qmlEngine);
			Q_UNUSED(jsEngine);
			return rc::ui::Audio::instance();
		}
	);

	qmlRegisterSingletonType<rc::ui::Video>("rc", 1, 0, "Video",
		[](QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> QObject* {
			Q_UNUSED(qmlEngine);
			Q_UNUSED(jsEngine);
			return rc::ui::Video::instance();
		}
	);

	qmlRegisterSingletonType<rc::ui::Subtitle>("rc", 1, 0, "Subtitle",
		[](QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> QObject* {
			Q_UNUSED(qmlEngine);
			Q_UNUSED(jsEngine);
			return rc::ui::Subtitle::instance();
		}
	);
}
