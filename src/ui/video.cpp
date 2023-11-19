#include "video.h"

#include <QDebug>

QPointer<rc::ui::Video> rc::ui::Video::m_instance = nullptr;

rc::ui::Video* rc::ui::Video::instance() {
	if (!m_instance) m_instance = new Video();
	return m_instance;
}

rc::ui::Video::Video(QObject *parent)
    : QAbstractListModel(parent) {}

QHash<int, QByteArray> rc::ui::Video::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[CodecRole] = "codec";
    roles[LabelRole] = "label";
    roles[SelectedRole] = "selected";
    return roles;
}

QString videoLabel(const QVariant& title) {
	if (title.isValid()) return title.toString();
	return "unknown";
}

void rc::ui::Video::setTracks(const QList<QVariant>& tracks) {
	beginResetModel();
	m_backing.clear();
	for (const auto& trackV: tracks) {
		QMap<QString, QVariant> track = trackV.toMap();
		QHash<int, QVariant> item;
		item[IdRole] = track["id"];
		item[CodecRole] = track["codec"];
		item[LabelRole] = videoLabel(track["title"]);
		m_backing.append(item);
	}
	endResetModel();
}

void rc::ui::Video::setSelected(const QVariant& trackId) {
	beginResetModel();
	m_selected = trackId;
	endResetModel();
}

int rc::ui::Video::rowCount(const QModelIndex&) const {
	return m_backing.size();
}

QVariant rc::ui::Video::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}
    if (role == SelectedRole) {
		return m_backing[index.row()][IdRole] == m_selected;
	}
	return m_backing[index.row()][role];
}

void rc::ui::Video::paused(bool paused) {
	m_paused = paused;
	Q_EMIT pausedUpdate();
}

void rc::ui::Video::durationChanged(qulonglong duration) {
	m_duration = duration;
	Q_EMIT durationUpdate();
}

void rc::ui::Video::positionChanged(qulonglong position) {
	m_position = position;
	Q_EMIT positionUpdate();
}

void rc::ui::Video::setPlaylist(bool playlist) {
	m_playlist = playlist;
	Q_EMIT playlistUpdate();
}

bool rc::ui::Video::getPaused() const {
	return m_paused;
}

bool rc::ui::Video::getPlaylist() const {
	return m_playlist;
}

qulonglong rc::ui::Video::getDuration() const {
	return m_duration;
}

qulonglong rc::ui::Video::getPosition() const {
	return m_position;
}

void rc::ui::Video::setPosition(qulonglong position) const {
	Q_EMIT positionSet(position);
}
