#include "localplayback.h"

#include "mpd.h"
#include "mpdconnection.h"

#include <QUrl>
#include <QMessageBox>

LocalPlayback *LocalPlayback::m_instance = 0;

LocalPlayback::LocalPlayback(QObject* parent) : QObject(parent), m_enabled(false), m_streamHandle(0)
{
    connect(MPDConnection::instance(), SIGNAL(connected(ServerInfo)), this, SLOT(serverChanged(ServerInfo)));
    connect(MPDConnection::instance(), SIGNAL(disconnected(QString)), this, SLOT(stop()));

    connect(MPD::instance(), SIGNAL(playingSongUpdated(MPDSong)), this, SLOT(songChanged()));

    BASS_Init(-1, 44100, 0, 0, NULL);
}

LocalPlayback::~LocalPlayback()
{
    BASS_Free();
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

    m_streamHandle = BASS_StreamCreateURL(m_serverURL.toLatin1().data(), 0, BASS_STREAM_AUTOFREE, NULL, NULL);
    BASS_ChannelPlay(m_streamHandle, false);
}

void LocalPlayback::stop()
{
    if(m_serverURL.isEmpty()) return;
    if(m_streamHandle == 0) return;

    BASS_ChannelStop(m_streamHandle);
    BASS_StreamFree(m_streamHandle);
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

/*void LocalPlayback::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    //restart playback
    if(newstate == 1 && oldstate == 2 && m_enabled)
    {
        m_player->play();
    }
}*/

void LocalPlayback::songChanged()
{
    // not needed with MP3 stream
    /*m_player->stop();
    m_player->play();*/
}
