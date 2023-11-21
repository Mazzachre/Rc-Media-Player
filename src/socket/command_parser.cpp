#include "command_parser.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

rc::socket::CommandParser::CommandParser(const QJsonDocument& doc) {
	if (doc.isObject()) {
		for (auto& key: doc.object().keys()) {
			if (key.compare("play") == 0) {
				QJsonValue value = doc.object()["play"];
				QMap<QString, QVariant> media;
				if (value.isObject()) {
					QJsonObject obj = value.toObject();
					for (auto& mediaKey: obj.keys()) {
						if (mediaKey.compare("movie") == 0 && obj["movie"].isString()) media["movie"] = obj["movie"].toString();
						else if (mediaKey.compare("videoTrack") == 0 && obj["videoTrack"].isDouble()) media["vid"] = obj["videoTrack"].toInt();
						else if (mediaKey.compare("audioTrack") == 0 && obj["audioTrack"].isDouble()) media["aid"] = obj["audioTrack"].toInt();
						else if (mediaKey.compare("subtitleTrack") == 0 && obj["subtitleTrack"].isDouble()) media["sid"] = obj["subtitleTrack"].toInt();
						else if (mediaKey.compare("subtitleTrack") == 0 && obj["subtitleTrack"].isString()) media["sid"] = obj["subtitleTrack"].toString();
						else if (mediaKey.compare("position") == 0 && obj["position"].isDouble()) media["position"] = obj["position"].toInt();
						else if (mediaKey.compare("volume") == 0 && obj["volume"].isDouble()) media["volume"] = obj["volume"].toInt();
						else if (mediaKey.compare("playlist") == 0 && obj["playlist"].isBool()) media["playlist"] = obj["playlist"].toBool();
					}
				} else if (value.isString()) {
					media["movie"] = value.toString();
				}
				
				if (media.contains("movie")) {
					m_parsed["media"] = media;
				} else {
					m_parsed["error"] = "Invalid type for media";
				}
			} else if (key.compare("playlist") == 0) {
				QJsonValue value = doc.object()["playlist"];
				if (value.isBool()) {
					m_parsed["playlist"] = value.toBool();
				} else {
					m_parsed["error"] = "Invalid type for playlist";
				}
			} else if (key.compare("pause") == 0) {
				QJsonValue value = doc.object()["pause"];
				if (value.isBool()) {
					m_parsed["pause"] = value.toBool();
				} else {
					m_parsed["error"] = "Invalid type for pause";
				}
			} else if (key.compare("fullScreen") == 0) {
				QJsonValue value = doc.object()["fullScreen"];
				if (value.isBool()) {
					m_parsed["fullscreen"] = value.toBool();
				} else {
					m_parsed["error"] = "Invalid type for full screen";
				}
			} else if (key.compare("audioTrack") == 0) {
				QJsonValue value = doc.object()["audioTrack"];
				if (value.isDouble()) {
					m_parsed["audio"] = value.toInt();
				} else {
					m_parsed["error"] = "Invalid type for subtitle";
				}
			} else if (key.compare("videoTrack") == 0) {
				QJsonValue value = doc.object()["videoTrack"];
				if (value.isDouble()) {
					m_parsed["video"] = value.toInt();
				} else {
					m_parsed["error"] = "Invalid type for subtitle";
				}
			} else if (key.compare("subtitleTrack") == 0) {
				QJsonValue value = doc.object()["subtitleTrack"];
				if (value.isDouble()) {
					m_parsed["subtitle"] = value.toInt();
				} else if (value.isString()) {
					m_parsed["subtitle"] = value.toString();
				} else {
					m_parsed["error"] = "Invalid type for subtitle";
				}
			} else if (key.compare("position") == 0) {
				QJsonValue value = doc.object()["position"];
				if (value.isDouble()) {
					m_parsed["position"] = value.toInt();
				} else {
					m_parsed["error"] = "Invalid type for position";
				}
			} else if (key.compare("volume") == 0) {
				QJsonValue value = doc.object()["volume"];
				if (value.isDouble()) {
					m_parsed["volume"] = value.toInt();
				} else {
					m_parsed["error"] = "Invalid type for volume";
				}
			} else if (key.compare("stop") == 0) {
				m_parsed["stop"] = true;
			} else {
				m_parsed["error"] = "Unknown key: '" + key + "'";
			}
		}
	} else {
		m_parsed["error"] = "argument needs to be an object";
	}
}

bool rc::socket::CommandParser::hasError() {
	return m_parsed.contains("error");
}

QString rc::socket::CommandParser::errorString() {
	return m_parsed.value("error").toString();
}

bool rc::socket::CommandParser::hasPlaylist() {
	return m_parsed.contains("playlist");
}

bool rc::socket::CommandParser::isPlaylist() {
	return m_parsed.value("playlist").toBool();
}
	
bool rc::socket::CommandParser::hasMedia() {
	return m_parsed.contains("media");
}

QVariantMap rc::socket::CommandParser::media() {
	return m_parsed.value("media").toMap();
}

bool rc::socket::CommandParser::hasPause() {
	return m_parsed.contains("pause");
}

bool rc::socket::CommandParser::isPause() {
	return m_parsed.value("pause").toBool();
}

bool rc::socket::CommandParser::hasVideoTrack() {
	return m_parsed.contains("video");
}

uint rc::socket::CommandParser::videoTrack() {
	return m_parsed.value("video").toUInt();
}

bool rc::socket::CommandParser::hasAudioTrack() {
	return m_parsed.contains("audio");
}

uint rc::socket::CommandParser::audioTrack() {
	return m_parsed.value("audio").toUInt();
}

bool rc::socket::CommandParser::hasSubtitleTrack() {
	return m_parsed.contains("subtitle");
}

QVariant rc::socket::CommandParser::subtitleTrack() {
	return m_parsed.value("subtitle");
}
	
bool rc::socket::CommandParser::stop() {
	return m_parsed.contains("stop");
}
	
bool rc::socket::CommandParser::hasPosition() {
	return m_parsed.contains("position");
}

qulonglong rc::socket::CommandParser::position() {
	return m_parsed.value("position").toULongLong();
}

bool rc::socket::CommandParser::hasVolume() {
	return m_parsed.contains("volume");
}

uint rc::socket::CommandParser::volume() {
	return m_parsed.value("volume").toUInt();
}

bool rc::socket::CommandParser::hasFullScreen() {
	return m_parsed.contains("fullscreen");	
}

bool rc::socket::CommandParser::fullScreen() {
	return m_parsed.value("fullscreen").toBool();	
}
