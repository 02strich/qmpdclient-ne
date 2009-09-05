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

#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include "config.h"
#include "mpd.h"
#include "mpdsong.h"

#include <QObject>

class QDBusInterface;
class NotificationModule;

class Notifications
	: public QObject
{
	Q_OBJECT

public:
	Notifications(QObject *);
	enum Type { CUSTOM = 0, FREEDESKTOP = 1, NATIVE = 2, GfW = 3 };

	static QList<Type> notifiers();
	static QString name(Type);
	static NotificationModule* module(Type t);

private slots:
	void setSong(const MPDSong &);

private:
	void notify(const QString &);
	QString makeTitle(const MPDSong &);

	static QList<NotificationModule*>* moduleList();
	static void setupModulesList(QList<NotificationModule*>* list);

private:
	MPDSong m_previousSong;
};

class NotificationModule
	: public QObject
{
	Q_OBJECT

public:
	NotificationModule(QObject* parent) : QObject(parent) {}
	virtual QString name() const = 0;
	virtual Notifications::Type type() const = 0;
	virtual bool notify(const QString&) = 0;
};

#include "notifications_custom.h"
#include "notifications_native.h"
#ifdef DBUS_NOTIFICATIONS
	#include "notifications_dbus.h"
#endif
#ifdef Q_WS_WIN
	#include "notifications_gfw.h"
#endif

#endif
