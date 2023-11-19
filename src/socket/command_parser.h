#pragma once

#include <QJsonDocument>
#include <QString>
#include <QVariant>
#include <QMap>

namespace rc {
namespace socket {

class CommandParser {
	QVariantMap m_parsed;
	QString m_error;

public:
	explicit CommandParser(const QJsonDocument& doc);
	
	bool hasError();
	QString errorString();
	
	bool hasPlaylist();
	bool isPlaylist();
	
	bool hasMedia();
	QVariantMap media();

	bool hasPause();
	bool isPause();

	bool hasVideoTrack();
	uint videoTrack();

	bool hasAudioTrack();
	uint audioTrack();

	bool hasSubtitleTrack();
	QVariant subtitleTrack();
	
	bool stop();
	
	bool hasPosition();
	qulonglong position();

	bool hasVolume();
	uint volume();
};

}}
