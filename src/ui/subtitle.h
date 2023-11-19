#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QString>
#include <QPointer>
#include <QUrl>

#include "../app/app.h"

namespace rc {
namespace ui {

class Subtitle : public QAbstractListModel
{
	Q_OBJECT
    Q_PROPERTY(QUrl directory READ getDirectory NOTIFY directoryUpdate)
    Q_PROPERTY(QUrl fileName READ getFileName NOTIFY fileNameUpdate)
    Q_PROPERTY(bool external READ isExternal NOTIFY externalUpdate)
	static QPointer<Subtitle> m_instance;
	QVector<QHash<int, QVariant>> m_backing;
    QVariant m_selected;
	QUrl m_directory;
	QString m_file_name;
	bool m_external;
	explicit Subtitle(QObject *parent = nullptr);

	Q_SLOT void setTracks(const QList<QVariant>& tracks);
    Q_SLOT void setSelected(const QVariant& trackId);
    Q_SLOT void movieStarted(const QString& file);
    Q_SIGNAL void removeSubtitleTrack(const QVariant& trackId);

	friend class rc::app::App;

public:
	static Subtitle* instance();

	enum SubtitleTrackRoles {
		IdRole = Qt::UserRole + 1,
		SelectedRole,
		LabelRole,
		ExternalRole,
		FilenameRole
	};

	int rowCount(const QModelIndex&) const override;
	QVariant data(const QModelIndex &index, int role) const override;

	QHash<int, QByteArray> roleNames() const override;
	
	QUrl getDirectory() const;
	Q_SIGNAL void directoryUpdate() const;

	QString getFileName() const;
	Q_SIGNAL void fileNameUpdate() const;
	
	bool isExternal() const;
	Q_SIGNAL void externalUpdate() const;

	Q_SLOT void removeSubtitle();
};
}}
