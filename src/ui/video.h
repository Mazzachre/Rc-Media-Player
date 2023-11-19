#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QString>
#include <QPointer>

#include "../app/app.h"

namespace rc {
namespace ui {

class Video : public QAbstractListModel
{
    Q_OBJECT
	Q_PROPERTY(bool paused READ getPaused NOTIFY pausedUpdate)
    Q_PROPERTY(qulonglong duration READ getDuration NOTIFY durationUpdate)
    Q_PROPERTY(qulonglong position READ getPosition NOTIFY positionUpdate)
    Q_PROPERTY(bool playlist READ getPlaylist NOTIFY playlistUpdate)

	qulonglong m_duration = 0;
	qulonglong m_position = 0;
	bool m_paused = false;
	bool m_playlist = false;
	static QPointer<Video> m_instance;
    QVector<QHash<int, QVariant>> m_backing;
    QVariant m_selected;
    
    explicit Video(QObject *parent = 0);
    Q_SLOT void setTracks(const QList<QVariant>& tracks);
    Q_SLOT void setSelected(const QVariant& trackId);
	Q_SLOT void paused(bool paused);
	Q_SLOT void durationChanged(qulonglong duration);
    Q_SLOT void positionChanged(qulonglong position);
    Q_SLOT void setPlaylist(bool playlist);
	Q_SIGNAL void positionSet(qulonglong position) const;

	friend class rc::app::App;
public:
	static Video* instance();

    enum VideoTrackRoles {
        IdRole = Qt::UserRole + 1,
        CodecRole,
        LabelRole,
        SelectedRole,
    };

    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

	bool getPaused() const;
	Q_SIGNAL void pausedUpdate() const;
	qulonglong getDuration() const;
    Q_SIGNAL void durationUpdate() const;
	qulonglong getPosition() const;
    Q_SIGNAL void positionUpdate() const;
	bool getPlaylist() const;
	Q_SIGNAL void playlistUpdate() const;

	Q_SLOT void setPosition(qulonglong position) const;
};
}}
