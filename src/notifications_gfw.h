#ifndef NOTIFICATIONS_GFW_H
#define NOTIFICATIONS_GFW_H

#include <QProcess>

class NotificationModuleGfW : public NotificationModule {

public:
	NotificationModuleGfW(QObject* parent) : NotificationModule(parent) {}

	virtual QString name() const { return "Growl for Windows"; }
	virtual Notifications::Type type() const { return Notifications::GfW; }

	virtual bool notify(const QString& text ) {
		QString application = "growlnotify.exe";
		QStringList parameter;
		parameter << "/t:\"QMPDClient\"";
		parameter << "/n:\"Song Update\"";
		parameter << "/a:\"QMPDClient\"";
		parameter << "/r:\"Song Update\"";
		parameter << "/silent:true";
		parameter << "\"" + text + "\"";

		QProcess growl;
		growl.start(application, parameter);
		return growl.waitForFinished();
	}
};

#endif // NOTIFICATIONS_GFW_H
