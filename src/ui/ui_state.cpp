#include "ui_state.h"

#include <QDebug>

QPointer<rc::ui::UiState> rc::ui::UiState::m_instance = nullptr;

rc::ui::UiState* rc::ui::UiState::instance() {
	if (!m_instance) m_instance = new rc::ui::UiState();
	return m_instance;
}

rc::ui::UiState::UiState(QObject *parent)
	: QObject(parent)
	, m_timer{new QTimer} {
	m_timer->start(5000);
	connect(m_timer, &QTimer::timeout, this, &UiState::hideControls);
}

void rc::ui::UiState::init(QQuickWindow* window) {
	m_window = window;
	m_window->installEventFilter(this);	
	connect(m_window, &QWindow::windowStateChanged, this, &UiState::onWindowStateChange);
}

void rc::ui::UiState::hideControls() {
	m_controls = false;
	m_timer->stop();
	Q_EMIT showControlsUpdate();
}

void rc::ui::UiState::onWindowStateChange() {
	Q_EMIT fullScreenUpdate();
}

void rc::ui::UiState::onMovieStart() {
	if (m_window) {
		m_window->show();
	}
}

void rc::ui::UiState::onMovieStop() {
	if (m_window) {
		m_window->hide();
	}	
}

bool rc::ui::UiState::eventFilter(QObject* watched, QEvent* event) {
	Q_UNUSED(watched);
	if (event->type() == QEvent::MouseMove) {
		m_timer->start();
		if (!m_controls) {
			m_controls = true;
			Q_EMIT showControlsUpdate();
		}
	}
		
	return false;
}

void rc::ui::UiState::toggleFullScreen() {
	if (m_window != nullptr) {
		if (m_window->windowState() == Qt::WindowNoState) {
			m_window->setWindowState(Qt::WindowFullScreen);
		} else {
			m_window->setWindowState(Qt::WindowNoState);
		}
	}
}

bool rc::ui::UiState::isControlsShow() const {
	return m_controls;
}

bool rc::ui::UiState::isFullScreen() const {
	if (m_window != nullptr) {
		return m_window->windowState() == Qt::WindowFullScreen;
	}
	return false;
}

void rc::ui::UiState::setFullScreen(bool fullScreen) {
	m_window->setWindowState(fullScreen ? Qt::WindowFullScreen : Qt::WindowNoState);
}
