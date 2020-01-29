#ifndef SETTING_H
#define SETTING_H

#include <QTableWidget>
#include <QString>
#include <QFont>
#include <QColor>

class Setting : public QWidget
{
    Q_OBJECT

public:
    Setting(QWidget *parent);
    virtual ~Setting();

    void Initialize(const QString& iniFilePath);
    bool IsDataReady() const;
    void StartPump();
    void StopPump();

signals:
    void Signal_DisplayText(QString text, QFont font, QColor color);

    // QObject interface
protected:
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void SetupLayout();
    void ReloadSetting(const QString& iniFilePath);
    void ResetLabels();
    void PumpItem(int index);
    void SaveSetting();

private:
    QTableWidget* m_tablePtr;
    QWidget* m_parentPtr;
    QString m_iniFilePath;
    int m_timerId;
    int m_pumpIndex;
};

#endif // SETTING_H
