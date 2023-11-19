#include "audio.h"

#include <QDebug>

QPointer<rc::ui::Audio> rc::ui::Audio::m_instance = nullptr;

rc::ui::Audio* rc::ui::Audio::instance() {
	if (!m_instance) m_instance = new rc::ui::Audio();
	return m_instance;
}

rc::ui::Audio::Audio(QObject *parent)
    : QAbstractListModel(parent) {}

QHash<int, QByteArray> rc::ui::Audio::roleNames() const {
	QHash<int, QByteArray> roles;
	roles[IdRole] = "id";
	roles[SelectedRole] = "selected";
	roles[LabelRole] = "label";
	roles[CodecRole] = "codec";
	roles[ChannelsRole] = "channels";
	return roles;
}

QString channelLabel(const QVariant& channelCount) {
	if (channelCount.isNull()) return "";
	int channels = channelCount.toInt();
	if (channels == 0) return "";
	if (channels == 1) return "mono";
	if (channels == 2) return "stereo";
	return "surround";
}

QString audioLabel(const QVariant& language, const QVariant& title) {
	if (language.isValid()) return language.toString();
	if (title.isValid()) return title.toString();
	return "unknown";
}

void rc::ui::Audio::setTracks(const QList<QVariant>& tracks) {
	beginResetModel();
	m_backing.clear();
	for (const auto& trackV: tracks) {
		QMap<QString, QVariant> track = trackV.toMap();
		QHash<int, QVariant> item;
		item[IdRole] = track["id"];
		item[LabelRole] = audioLabel(track["lang"], track["title"]);
		item[CodecRole] = track["codec"];
		item[ChannelsRole] = channelLabel(track["demux-channel-count"]);
		m_backing.append(item);
	}
	endResetModel();
}

void rc::ui::Audio::setSelected(const QVariant& trackId) {
	beginResetModel();
	m_selected = trackId;
	endResetModel();
}

int rc::ui::Audio::rowCount(const QModelIndex&) const {
	return m_backing.size();
}

QVariant rc::ui::Audio::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}
    if (role == SelectedRole) {
		return m_backing[index.row()][IdRole] == m_selected;
	}
	return m_backing[index.row()][role];
}

void rc::ui::Audio::muted(bool muted) {
	m_muted = muted;
	Q_EMIT mutedUpdate();
}

void rc::ui::Audio::volumeChanged(uint volume) {
	m_volume = volume;
	Q_EMIT volumeUpdate();
}

bool rc::ui::Audio::getMuted() const {
	return m_muted;
}

uint rc::ui::Audio::getVolume() const {
	return m_volume;
}

void rc::ui::Audio::setVolume(uint volume) const {
	Q_EMIT volumeSet(volume);
}
