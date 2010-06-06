#ifndef LOCALPLAYBACK_H
#define LOCALPLAYBACK_H

#include "serverinfo.h"

#include <QObject>
#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>

class LocalPlayback : public QObject
{
    Q_OBJECT

public:
    static LocalPlayback* instance();

    LocalPlayback(QObject* parent = 0);

    bool isEnabled() const { return m_enabled; } ;

public slots:
    void setEnabled(bool);
    void play();
    void stop();

    void serverChanged(ServerInfo);
    void songChanged();

    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
private:
    static LocalPlayback* m_instance;

    QString m_serverURL;
    bool m_enabled;

    Phonon::MediaObject* m_player;
    Phonon::AudioOutput* m_output;
};

#endif // LOCALPLAYBACK_H
