#include "arguments.h"
#include <QLoggingCategory>

QPointer<rc::app::Arguments> rc::app::Arguments::m_instance = nullptr;

rc::app::Arguments::Arguments(uint port, bool debug, QObject *parent)
	: QObject(parent)
	, m_port{port}
	, m_debug{debug} {
	if (!debug) QLoggingCategory::setFilterRules("*.debug=false");
}

void rc::app::Arguments::init(const QString& port, bool debug, QObject *parent) {
	bool ok;
	uint i_port = port.toUInt(&ok);
	if (!ok || i_port < 1025 || i_port > 65635) i_port = 9009;
	m_instance = new Arguments(i_port, debug, parent);
}

rc::app::Arguments* rc::app::Arguments::instance() {
	return m_instance;
}

bool rc::app::Arguments::debug() const {
	return m_debug;
}

uint rc::app::Arguments::port() const {
	return m_port;
}
