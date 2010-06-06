#include "localplayback.h"

#include "mpd.h"
#include "mpdconnection.h"

#include <QUrl>
#include <QMessageBox>

LocalPlayback *LocalPlayback::m_instance = 0;

LocalPlayback::LocalPlayback(QObject* parent) : QObject(parent), m_enabled(false), m_player(new Phonon::MediaObject(this)), m_output(new Phonon::AudioOutput(Phonon::MusicCategory, this))
{
    connect(MPDConnection::instance(), SIGNAL(connected(ServerInfo)), this, SLOT(serverChanged(ServerInfo)));
    connect(MPDConnection::instance(), SIGNAL(disconnected(QString)), this, SLOT(stop()));

    connect(MPD::instance(), SIGNAL(playingSongUpdated(MPDSong)), this, SLOT(songChanged()));

    connect(m_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
}

LocalPlayback *LocalPlayback::instance() {
    if (!m_instance) {
        m_instance = new LocalPlayback();
    }
    return m_instance;
}

void LocalPlayback::serverChanged(ServerInfo info)
{
    stop();
    m_serverURL = info.streamingURL();
    play();
}

void LocalPlayback::play()
{
    if(m_serverURL.isEmpty()) return;
    if(!m_enabled) return;

    Phonon::Path path = Phonon::createPath(m_player, m_output);
    m_player->setCurrentSource(Phonon::MediaSource(m_serverURL));
    m_player->play();
}

void LocalPlayback::stop()
{
    if(m_serverURL.isEmpty()) return;
    if(m_player == 0) return;

    m_player->stop();
}

void LocalPlayback::setEnabled(bool value)
{
    m_enabled = value;
    if(m_enabled)
    {
        play();
    }
    else
    {
        stop();
    }
}

void LocalPlayback::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    //restart playback
    if(newstate == 1 && oldstate == 2 && m_enabled)
    {
        m_player->play();
    }
}

void LocalPlayback::songChanged()
{
    // not needed with MP3 stream
    /*m_player->stop();
    m_player->play();*/
}
