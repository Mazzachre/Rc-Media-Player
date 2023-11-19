#include "subtitle.h"

#include <QDebug>

QPointer<rc::ui::Subtitle> rc::ui::Subtitle::m_instance = nullptr;

rc::ui::Subtitle* rc::ui::Subtitle::instance() {
	if (!m_instance) m_instance = new rc::ui::Subtitle();
	return m_instance;
}

rc::ui::Subtitle::Subtitle(QObject *parent)
	: QAbstractListModel(parent)
	, m_file_name{"No video"} {}

QHash<int, QByteArray> rc::ui::Subtitle::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[SelectedRole] = "selected";
    roles[LabelRole] = "label";
    roles[ExternalRole] = "external";
    roles[FilenameRole] = "filename";
    return roles;
}

QString subLabel(const QVariant& language, const QVariant& title) {
	if (language.isValid()) return language.toString();
	if (title.isValid()) return title.toString();
	return "unknown";
}

void rc::ui::Subtitle::setTracks(const QList<QVariant>& tracks) {
	beginResetModel();
	m_backing.clear();
	for (const auto& trackV: tracks) {
		QMap<QString, QVariant> track = trackV.toMap();
		QHash<int, QVariant> item;
		item[IdRole] = track["id"];
		item[LabelRole] = subLabel(track["lang"], track["title"]);
		item[ExternalRole] = track["external"];
		item[FilenameRole] = track["external-filename"];
		m_backing.append(item);
	}
	endResetModel();
}

void rc::ui::Subtitle::setSelected(const QVariant& trackId) {
	m_external = false;
	for (const auto& track: m_backing) {
		if (track[IdRole] == trackId) {
			m_external = true;
			break;
		}
	}
	Q_EMIT externalUpdate();

	beginResetModel();
	m_selected = trackId;
	endResetModel();
}

void rc::ui::Subtitle::movieStarted(const QString& file) {
	QUrl url(file);
	m_directory = url.adjusted(QUrl::RemoveFilename);
	Q_EMIT directoryUpdate();
	m_file_name = url.fileName();
	Q_EMIT fileNameUpdate();
}


int rc::ui::Subtitle::rowCount(const QModelIndex&) const {
	return m_backing.size();
}

QVariant rc::ui::Subtitle::data(const QModelIndex &index, int role) const {
	if (!index.isValid()) {
		return QVariant();
	}
    if (role == SelectedRole) {
		return m_backing[index.row()][IdRole] == m_selected;
	}
	return m_backing[index.row()][role];
}

QUrl rc::ui::Subtitle::getDirectory() const {
	return m_directory;
}

QString rc::ui::Subtitle::getFileName() const {
	return m_file_name;
}

bool rc::ui::Subtitle::isExternal() const {
	return m_external;
}

void rc::ui::Subtitle::removeSubtitle() {
	Q_EMIT removeSubtitleTrack(m_selected);
}

