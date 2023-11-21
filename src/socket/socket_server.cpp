#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>

#include "socket_server.h"

rc::socket::SocketServer::SocketServer(uint port, QObject *parent)
	: QObject(parent)
	, m_server{new QWebSocketServer(QStringLiteral("Media Player"), QWebSocketServer::NonSecureMode, this)}
	, m_client{nullptr}
	, m_timer{new QTimer(this)} {
	if (m_server->listen(QHostAddress::Any, port)) {
		connect(m_server, &QWebSocketServer::newConnection, this, &SocketServer::handleClientConnect);
		connect(m_server, &QWebSocketServer::closed, this, &SocketServer::handleClientDisconnect);
		connect(m_timer, &QTimer::timeout, QCoreApplication::instance(), &QCoreApplication::quit, Qt::QueuedConnection);
	}
}

rc::socket::SocketServer::~SocketServer() {
	if (m_client) m_client->deleteLater();
}

void rc::socket::SocketServer::handleClientConnect() {
	m_server->pauseAccepting();
	m_client = m_server->nextPendingConnection();
	
	connect(m_client, &QWebSocket::textMessageReceived, this, &SocketServer::handleClientMessage);
    connect(m_client, &QWebSocket::disconnected, this, &SocketServer::handleClientDisconnect);
    m_timer->stop();
}

void rc::socket::SocketServer::handleClientDisconnect() {
	if (m_client) m_client->deleteLater();
	m_server->resumeAccepting();
	m_timer->setSingleShot(true);
	m_timer->start(3000);
}

void rc::socket::SocketServer::handleClientMessage(const QString &message) {
	QJsonParseError parse_error;
	QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8(), &parse_error);
	if (parse_error.error == QJsonParseError::NoError) {
		CommandParser parser(doc);
		if (parser.hasError()) {
			QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"error", parser.errorString()}});
			m_client->sendTextMessage(QString{doc.toJson()});
		} else {
			if (parser.hasMedia()) Q_EMIT playMovie(parser.media());
			if (parser.hasPlaylist()) Q_EMIT playlist(parser.isPlaylist());
			if (parser.hasPause()) Q_EMIT setPause(parser.isPause());
			if (parser.hasPosition()) Q_EMIT setPosition(parser.position());
			if (parser.hasAudioTrack()) Q_EMIT setAudioTrack(parser.audioTrack());
			if (parser.hasVideoTrack()) Q_EMIT setVideoTrack(parser.videoTrack());
			if (parser.hasSubtitleTrack()) Q_EMIT setSubtitleTrack(parser.subtitleTrack());
			if (parser.hasVolume()) Q_EMIT setVolume(parser.volume());
			if (parser.hasFullScreen()) Q_EMIT setFullScreen(parser.fullScreen());
			if (parser.stop()) Q_EMIT stopMovie();
		}
	} else {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"error", parse_error.errorString()}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}
}

void rc::socket::SocketServer::videoTrackChanged(const QVariant& track) const {
	if (m_client != nullptr && track.isValid()) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"videoTrack", track}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}
}

void rc::socket::SocketServer::audioTrackChanged(const QVariant& track) const {
	if (m_client != nullptr && track.isValid()) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"audioTrack", track}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}
}

void rc::socket::SocketServer::subtitleTrackChanged(const QVariant& subtitle) const {
	if (m_client != nullptr && subtitle.isValid()) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"subtitleTrack", subtitle}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}
}

void rc::socket::SocketServer::subtitleTrackRemoved(const QVariant& subtitle) const {
	if (m_client != nullptr && subtitle.isValid()) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"subtitleRemoved", subtitle}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}	
}

void rc::socket::SocketServer::volumeChanged(const uint volume) const {
	if (m_client != nullptr) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"volume", volume}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}
}

void rc::socket::SocketServer::playing(const qulonglong position) const {
	if (m_client != nullptr) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"position", position}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}
}

void rc::socket::SocketServer::playingStopped(const QString& reason) const {
	if (m_client != nullptr) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"stopped", reason}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}
}

void rc::socket::SocketServer::reportError(const QString& error) const {
	if (m_client != nullptr) {
		QJsonDocument doc = QJsonDocument::fromVariant(QVariantMap{{"error", error}});
		m_client->sendTextMessage(QString{doc.toJson()});
	}	
}
