#ifndef NOTIFICATIONS_NATIVE_H
#define NOTIFICATIONS_NATIVE_H

#include <QSystemTrayIcon>

class NotificationModuleNative : public NotificationModule {

public:
	NotificationModuleNative(QObject* parent) : NotificationModule(parent) {}

	virtual QString name() const { return "Native"; }
	virtual Notifications::Type type() const { return Notifications::NATIVE; }

	virtual bool notify(const QString & text) {
		QSystemTrayIcon* tray = new QSystemTrayIcon();
		tray->show();
		tray->showMessage("QMPDClient", text);
		return true;
	}
};

#endif
