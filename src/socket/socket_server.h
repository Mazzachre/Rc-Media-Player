#pragma once

#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>
#include "command_parser.h"

namespace rc {
namespace socket {

class SocketServer : public QObject {
	Q_OBJECT
	
	QWebSocketServer *m_server;
	QWebSocket *m_client;
	QTimer *m_timer;

	Q_SLOT void handleClientConnect();
	Q_SLOT void handleClientDisconnect();
	Q_SLOT void handleClientMessage(const QString &message);

public:
	explicit SocketServer(uint port, QObject *parent = nullptr);
	~SocketServer();

	Q_SIGNAL void playMovie(const QMap<QString, QVariant>& args) const;
	Q_SIGNAL void stopMovie();
	Q_SIGNAL void playlist(bool playlist);
	Q_SIGNAL void setPause(bool pause);
	Q_SIGNAL void setPosition(qulonglong position);
	Q_SIGNAL void setVideoTrack(uint subtitle);
	Q_SIGNAL void setAudioTrack(uint subtitle);
	Q_SIGNAL void setSubtitleTrack(QVariant subtitle);
	Q_SIGNAL void setVolume(uint volume);

	Q_SLOT void videoTrackChanged(const QVariant& track) const;
	Q_SLOT void audioTrackChanged(const QVariant& track) const;
	Q_SLOT void subtitleTrackChanged(const QVariant& subtitle) const;
	Q_SLOT void subtitleTrackRemoved(const QVariant& subtitle) const;
	Q_SLOT void volumeChanged(const uint volume) const;

	Q_SLOT void playingStopped(const QString& reason) const;
	Q_SLOT void playing(const qulonglong position) const;
	
	Q_SLOT void reportError(const QString& error) const;
};
}}
