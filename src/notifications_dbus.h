/*
 * QMPDClient - An MPD client written in Qt 4.
 * Copyright (C) 2005-2008 Håvard Tautra Knutsen <havtknut@tihlde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QApplication>
#include <QDBusInterface>
#include <QDBusReply>
#include <QStringList>

class NotificationModuleDbus : public NotificationModule {

public:
	NotificationModuleDbus(QObject* parent) : NotificationModule(parent),
		m_interface(new QDBusInterface("org.freedesktop.Notifications", "/org/freedesktop/Notifications"))
	{ }

	virtual QString name() const { return "DBus"; }
	virtual Notifications::Type type() const { return Notifications::FREEDESKTOP; }

	virtual bool notify(const QString &text) {
		Q_ASSERT(m_interface);

		if (!m_interface->isValid()) return false;

		QString member = "Notify";
		QVariantList params;
		params << qApp->applicationName() << static_cast<unsigned int>(0);
		params << "/home/h/Projects/qmpdclient/qmpdclient/icons/qmpdclient22.png";
		params << qApp->applicationName() << text;
		params << QStringList() << QMap<QString, QVariant>() << Config::instance()->notificationsTimeout()*1000;
		QDBusMessage reply = m_interface->callWithArgumentList(QDBus::Block, member, params);

		if (reply.type() == QDBusMessage::ErrorMessage) {
			QDBusError err = reply;
			qWarning("Error: %s\n%s", qPrintable(err.name()), qPrintable(err.message()));
			return false;
		} else if (reply.type() != QDBusMessage::ReplyMessage) {
			qWarning("Invalid reply type %d", int(reply.type()));
			return false;
		}

		return true;
	}

private:
	QDBusInterface* m_interface;
};
