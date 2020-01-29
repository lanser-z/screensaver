#include "screensaver.h"
#include "setting.h"

#include <QPaintEvent>
#include <QPainter>

ScreenSaver::ScreenSaver(QWidget *parent) : QWidget(parent)
{
    m_settingPtr = nullptr;
}

ScreenSaver::~ScreenSaver()
{

}

void ScreenSaver::showEvent(QShowEvent *event)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus(Qt::ActiveWindowFocusReason);

    if (!m_settingPtr)
    {
        m_settingPtr = new Setting(this);
        QMetaObject::Connection conn = connect(m_settingPtr,
            SIGNAL(Signal_DisplayText(QString, QFont, QColor)),
            SLOT(Slot_DisplayText(QString, QFont, QColor)));
        Q_ASSERT(conn);

        m_settingPtr->Initialize("config.ini");
        if (m_settingPtr->IsDataReady())
        {
            m_settingPtr->hide();
            m_settingPtr->StartPump();
        }
        else
        {
            m_settingPtr->show();
        }
    }
    showFullScreen();
    QWidget::showEvent(event);
}

void ScreenSaver::resizeEvent(QResizeEvent *event)
{
    if (m_settingPtr)
    {
        m_settingPtr->resize(width()/2, height()/2);
    }
    QWidget::resizeEvent(event);
}

void ScreenSaver::mousePressEvent(QMouseEvent *event)
{
    Stop();
    QWidget::mousePressEvent(event);
}

void ScreenSaver::mouseMoveEvent(QMouseEvent *event)
{
    Stop();
    QWidget::mouseMoveEvent(event);
}

void ScreenSaver::wheelEvent(QWheelEvent *event)
{
    Stop();
    QWidget::wheelEvent(event);
}

void ScreenSaver::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F5)
    {
        Play();
    }
    else if (event->key() == Qt::Key_F1)
    {
        Stop();
        if (m_settingPtr)
        {
            m_settingPtr->show();
        }
    }
    else if (event->key() == Qt::Key_Escape)
    {
        close();
    }
    QWidget::keyPressEvent(event);
}

void ScreenSaver::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::black);
    if (m_text.isEmpty())
    {
        return;
    }

    painter.setFont(m_font);
    painter.setPen(m_color);
    painter.drawText(rect(), Qt::AlignCenter, m_text);
}

void ScreenSaver::Slot_DisplayText(QString text, QFont font, QColor color)
{
    m_text = text;
    m_font = font;
    m_color = color;
    update();
}

void ScreenSaver::Stop()
{
    if (m_settingPtr)
    {
        m_settingPtr->StopPump();
        m_text = tr("User stoped, press F5 to replay. Press F1 for setting.");
        m_color = Qt::white;
        m_font = QFont();
        update();
    }
}

void ScreenSaver::Play()
{
    if (m_settingPtr)
    {
        m_settingPtr->StartPump();
    }
}
