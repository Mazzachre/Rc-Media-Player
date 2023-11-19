#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>
#include "../socket/socket_server.h"

namespace rc {
namespace app {

class App : public QApplication {
	Q_OBJECT
	QQmlApplicationEngine *m_engine;
	rc::socket::SocketServer *m_socketServer{nullptr};

	bool eventFilter(QObject* watched, QEvent* event) override;
	void registerQmlTypes();
	void connectSignals();

public:
	App(int &argc, char **argv);
	~App();

	int run();
};
	
}}
