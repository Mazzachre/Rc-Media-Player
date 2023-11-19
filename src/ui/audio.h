#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QString>
#include <QPointer>

#include "../app/app.h"

namespace rc {
namespace ui {

class Audio : public QAbstractListModel
{
    Q_OBJECT
	Q_PROPERTY(bool muted READ getMuted NOTIFY mutedUpdate)
    Q_PROPERTY(uint volume READ getVolume NOTIFY volumeUpdate)
	
    QVector<QHash<int, QVariant>> m_backing;
    QVariant m_selected;
	uint m_volume = 100;
	bool m_muted;
	static QPointer<Audio> m_instance;

    explicit Audio(QObject *parent = 0);
    
    Q_SLOT void setTracks(const QList<QVariant>& tracks);
    Q_SLOT void setSelected(const QVariant& trackId);
	Q_SLOT void muted(bool muted);
	Q_SLOT void volumeChanged(uint volume);
	Q_SIGNAL void volumeSet(uint volume) const;

	friend class rc::app::App;

public:
	static Audio* instance();

    enum AudioTrackRoles {
        IdRole = Qt::UserRole + 1,
        SelectedRole,
        LabelRole,
        CodecRole,
        ChannelsRole,
    };

    int rowCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

	bool getMuted() const;
	Q_SIGNAL void mutedUpdate() const;
	uint getVolume() const;
    Q_SIGNAL void volumeUpdate() const;

	Q_SLOT void setVolume(uint volume) const;
};
}}
