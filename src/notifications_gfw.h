#ifndef NOTIFICATIONS_GFW_H
#define NOTIFICATIONS_GFW_H

class NotificationModuleGfW : public NotificationModule {

public:
	NotificationModuleGfW(QObject* parent) : NotificationModule(parent) {}

	virtual QString name() const { return "Growl for Windows"; }
	virtual Notifications::Type type() const { return Notifications::GfW; }

	virtual bool notify(const QString& /* text */) { return false; }
};

#endif // NOTIFICATIONS_GFW_H
