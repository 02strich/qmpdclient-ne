#ifndef LOCALPLAYBACK_H
#define LOCALPLAYBACK_H

#include "serverinfo.h"

#include <QObject>
#include <bass.h>

class LocalPlayback : public QObject
{
    Q_OBJECT

public:
    static LocalPlayback* instance();

    LocalPlayback(QObject* parent = 0);
    ~LocalPlayback();

    bool isEnabled() const { return m_enabled; } ;

public slots:
    void setEnabled(bool);
    void play();
    void stop();

    void serverChanged(ServerInfo);
    void songChanged();

private:
    static LocalPlayback* m_instance;

    QString m_serverURL;
    bool m_enabled;

    HSTREAM m_streamHandle;
};

#endif // LOCALPLAYBACK_H
