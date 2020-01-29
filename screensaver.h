#ifndef SCREEN_SAVER_H
#define SCREEN_SAVER_H

#include <QWidget>

class Setting;

class ScreenSaver : public QWidget
{
    Q_OBJECT
public:
	ScreenSaver(QWidget* parent);
	virtual ~ScreenSaver();

    // QWidget interface
protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void Slot_DisplayText(QString text, QFont font, QColor color);

private:
    void Stop();
    void Play();

private:
    Setting* m_settingPtr;
    QString m_text;
    QFont m_font;
    QColor m_color;
};

#endif // SCREEN_SAVER_H
