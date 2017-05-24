#ifndef QHANDLEEXEVENT_H
#define QHANDLEEXEVENT_H

#include <QEvent>

class QHandleExEvent : public QEvent
{
public:
    QHandleExEvent(QEvent::Type type, DWORD dwEventType, LPVOID lpEventData,  LPVOID lpContext ) : QEvent(type)
    {
        m_type = type;
        m_dwEventType = dwEventType;
        m_lpEventData = lpEventData;
        m_lpContext = lpContext;
    }

    ~QHandleExEvent()
    {

    }

    QEvent::Type type()
    {
        return m_type;
    }

    DWORD dwEventType()
    {
        return m_dwEventType;
    }

    LPVOID lpEventData()
    {
        return m_lpEventData;
    }

    LPVOID lpContext()
    {
        return m_lpContext;
    }

private:
    QEvent::Type m_type;
    DWORD m_dwEventType;
    LPVOID m_lpEventData;
    LPVOID m_lpContext;

};

#endif // QHANDLEEXEVENT_H
