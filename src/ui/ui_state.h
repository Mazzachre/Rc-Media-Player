#pragma once

#include <QQuickWindow>
#include <QEvent>
#include <QPointer>
#include <QTimer>

#include "../app/app.h"

namespace rc {
namespace ui {

class UiState : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool showControls READ isControlsShow NOTIFY showControlsUpdate)
	Q_PROPERTY(bool fullScreen READ isFullScreen NOTIFY fullScreenUpdate)
	static QPointer<UiState> m_instance;
	QTimer* m_timer{nullptr};
	QQuickWindow* m_window{nullptr};
	
	bool m_controls = true;
	explicit UiState(QObject *parent = nullptr);
	void init(QQuickWindow* window);
	
    bool eventFilter(QObject* watched, QEvent* event) override;
    Q_SLOT void hideControls();
    Q_SLOT void onWindowStateChange();
    Q_SLOT void onMovieStart();
    Q_SLOT void onMovieStop();

	friend class rc::app::App;
public:
	static UiState* instance();

	bool isControlsShow() const;
	Q_SIGNAL void showControlsUpdate() const;

    Q_SLOT void toggleFullScreen();
	bool isFullScreen() const;
	Q_SIGNAL void fullScreenUpdate() const;
};
}}
