#ifndef NOTIFICATIONS_CUSTOM_H
#define NOTIFICATIONS_CUSTOM_H

#include "coverartdialog.h"
#include "passivepopup.h"

class NotificationModuleCustom : public NotificationModule {

public:
	NotificationModuleCustom(QObject* parent) : NotificationModule(parent), m_coverArt(new CoverArtDialog(0)) {
		connect(MPD::instance(), SIGNAL(playingSongUpdated(const MPDSong &)), this, SLOT(setSong(const MPDSong &)));
	}

	virtual QString name() const { return "QMDClient"; }
	virtual Notifications::Type type() const { return Notifications::CUSTOM; }

	virtual bool notify(const QString& text) {
		// TODO: height and width set optionaly
		QPixmap icon;
		if (Config::instance()->showCoverArt() && m_coverArt->hasCoverArt()) {
			icon = m_coverArt->coverArt();
			if (icon.height() > 64) icon = icon.scaledToHeight(64, Qt::SmoothTransformation);
			if (icon.width() > 64) icon = icon.scaledToWidth(64, Qt::SmoothTransformation);
		} else {
			icon = QPixmap(":/icons/qmpdclient48.png");
		}

		PassivePopup::Position pos = static_cast<PassivePopup::Position>(Config::instance()->notificationsPosition());
		new PassivePopup("QMPDClient", text, icon, pos, Config::instance()->notificationsTimeout());
		return true;
	}

private slots:
	void setSong(const MPDSong& s) {
		if(!s.isNull()) {
			m_coverArt->setSong(s);
		}
	}

private:
	CoverArtDialog* m_coverArt;
};

#endif // NOTIFICATIONS_CUSTOM_H
