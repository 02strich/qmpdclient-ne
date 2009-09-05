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

#include "debug.h"
#include "notifications.h"
#include "richtext.h"

#include <QApplication>
#include <QDesktopWidget>

Notifications::Notifications(QObject* parent) : QObject(parent) {
	setObjectName("notifications");
	connect(MPD::instance(), SIGNAL(playingSongUpdated(const MPDSong &)), this, SLOT(setSong(const MPDSong &)));
}



void Notifications::notify(const QString &text) {
	NotificationModule* notifyModule = module((Type)Config::instance()->notifier());
	if(notifyModule != 0) {
		if(notifyModule->notify(text)) return;
		qWarning((notifyModule->name() + " notify failed.").toLatin1());
	}
}

QString Notifications::makeTitle(const MPDSong &s) {
	int desktopWidth = QApplication::desktop()->width();
	QString title = elideRichText("", s.title().isEmpty() ? s.filename() : s.title()  , "", desktopWidth / 2) + "\n";
	QString artist = elideRichText("", s.artist(), "", desktopWidth / 4);
	QString album = elideRichText("", s.album(), "", desktopWidth / 4);

	if (!artist.isEmpty())
		title += artist + "\n";
	if (!album.isEmpty())
		title += album;
	return title;
}

void Notifications::setSong(const MPDSong &s) {
	if (m_previousSong.isNull() || m_previousSong == s || !Config::instance()->notificationsEnabled() || Config::instance()->notificationsTimeout() < 1) {
		m_previousSong = s;
		return;
	}
	if(!s.isNull())
	{
		notify(makeTitle(s));
	}
	m_previousSong = s;
}

QList<NotificationModule*>* Notifications::moduleList() {
	static QList<NotificationModule*> modules;

	if(modules.isEmpty()) {
		setupModulesList(&modules);
	}
	return &modules;
}

void Notifications::setupModulesList(QList<NotificationModule*>* list) {
	list->append(new NotificationModuleCustom(qApp));
	list->append(new NotificationModuleNative(qApp));

#ifdef DBUS_NOTIFICATIONS
	list->append(new NotificationModuleDbus(qApp));
#endif

#ifdef Q_WS_WIN
	list->append(new NotificationModuleGfW(qApp));
#endif

}

QList<Notifications::Type> Notifications::notifiers() {
	QList<Type> result = QList<Type>();
	for (int i = 0; i < moduleList()->size(); ++i)
		  result << moduleList()->at(i)->type();

	return result;
}

QString Notifications::name(Type t) {
	for(int i = 0; i < moduleList()->size(); ++i) {
		NotificationModule* current = moduleList()->at(i);
		if(current->type() == t)
			return current->name();
	}

	return "";
}

NotificationModule* Notifications::module(Type t) {
	for(int i = 0; i < moduleList()->size(); ++i) {
		NotificationModule* current = moduleList()->at(i);
		if(current->type() == t)
			return current;
	}
	return 0;
}


