#pragma once

#include <QPointer>

namespace rc {
namespace app {

class Arguments : public QObject {
	Q_OBJECT

	static QPointer<Arguments> m_instance;
	uint m_port;
	bool m_debug;

	Arguments(uint port, bool debug, QObject *parent = nullptr);
	static void init(const QString& port, bool debug, QObject *parent = nullptr);

	friend class App;

public:
	static Arguments* instance();

	bool debug() const;
	uint port() const;
};
	
}}
