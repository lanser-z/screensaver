#include "setting.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QDebug>
#include "newitem.h"

Setting::Setting(QWidget* parent) : QWidget(parent), m_tablePtr(nullptr)
{
    m_timerId = 0;
    m_pumpIndex = -1;
    m_parentPtr = parent;
}

Setting::~Setting()
{
    StopPump();
}

void Setting::Initialize(const QString& iniFilePath)
{
    m_iniFilePath = iniFilePath;
    if (!m_tablePtr)
    {
        m_tablePtr = new QTableWidget(this);
        m_tablePtr->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_tablePtr->setColumnCount(4);
        m_tablePtr->setHorizontalHeaderLabels(QStringList()<<tr("Text")<<tr("Font")<<tr("Color")<<tr("Elapse"));

        SetupLayout();
        ReloadSetting(iniFilePath);
    }
}

bool Setting::IsDataReady() const
{
    return m_tablePtr != nullptr && m_tablePtr->rowCount() > 0;
}

void Setting::StartPump()
{
    StopPump();

    if (IsDataReady())
    {
        PumpItem(0);
        m_timerId = startTimer(m_tablePtr->item(m_pumpIndex, 3)->text().toInt() * 1000, Qt::VeryCoarseTimer);
    }
}

void Setting::StopPump()
{
    if (m_timerId)
    {
        killTimer(m_timerId);
        m_timerId = 0;
        m_pumpIndex = -1;
    }
}

void Setting::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    killTimer(m_timerId);
    m_pumpIndex = (m_pumpIndex+1) % m_tablePtr->rowCount();
    PumpItem(m_pumpIndex);
    m_timerId = startTimer(m_tablePtr->item(m_pumpIndex, 3)->text().toInt() * 1000, Qt::VeryCoarseTimer);
}

void Setting::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    move((m_parentPtr->width() - width())/2, (m_parentPtr->height() - height())/2);
}

void Setting::resizeEvent(QResizeEvent *event)
{
    move((m_parentPtr->width() - width())/2, (m_parentPtr->height() - height())/2);
    QWidget::resizeEvent(event);
}

void Setting::SetupLayout()
{
    QVBoxLayout* mainLayoutPtr = new QVBoxLayout(this);

    Q_ASSERT(m_tablePtr);
    mainLayoutPtr->addWidget(m_tablePtr);

    QHBoxLayout* btnLayoutPtr = new QHBoxLayout;
    QPushButton* addItem = new QPushButton(tr("&Add"));
    QPushButton* subItem = new QPushButton(tr("&Delete"));
    QPushButton* clearAll = new QPushButton(tr("&Clear All"));
    QPushButton* confirm = new QPushButton(tr("&OK"));
    btnLayoutPtr->addWidget(addItem);
    btnLayoutPtr->addWidget(subItem);
    btnLayoutPtr->addWidget(clearAll);
    btnLayoutPtr->addWidget(confirm);
    mainLayoutPtr->addLayout(btnLayoutPtr);

    QMetaObject::Connection conn;
    conn = connect(addItem, &QPushButton::clicked,
                   [this](bool){
        int row = this->m_tablePtr->currentRow();
        if (!(row >= 0 && row < this->m_tablePtr->rowCount())) {
            row = this->m_tablePtr->rowCount();
        }

        NewItem newItem(this);
        newItem.exec();
        if (newItem.result() == QDialog::Accepted) {
            QString text, font, color, elapse;
            if (newItem.GetNewItem(text, font, color, elapse)) {
                this->m_tablePtr->insertRow(row);
                this->m_tablePtr->setItem(row, 0, new QTableWidgetItem(text));
                this->m_tablePtr->setItem(row, 1, new QTableWidgetItem(font));
                this->m_tablePtr->setItem(row, 2, new QTableWidgetItem(color));
                this->m_tablePtr->setItem(row, 3, new QTableWidgetItem(elapse));

                this->ResetLabels();
            }
        }
    });
    Q_ASSERT(conn);

    conn = connect(subItem, &QPushButton::clicked,
                   [this](bool){
        int row = this->m_tablePtr->currentRow();
        if (row >= 0 && row < this->m_tablePtr->rowCount()) {
            this->m_tablePtr->removeRow(row);
            this->ResetLabels();
        }
    });
    Q_ASSERT(conn);

    conn = connect(clearAll, &QPushButton::clicked,
                   [this](bool){
        this->m_tablePtr->clear();
        this->m_tablePtr->setRowCount(0);
        m_tablePtr->setColumnCount(4);
        m_tablePtr->setHorizontalHeaderLabels(QStringList()<<tr("Text")<<tr("Font")<<tr("Color")<<tr("Elapse"));
    });
    Q_ASSERT(conn);

    conn = connect(confirm, &QPushButton::clicked,
                   [this](bool){
        this->SaveSetting();
        this->hide();

        this->m_parentPtr->setFocus(Qt::PopupFocusReason);
        this->StartPump();
    });
    Q_ASSERT(conn);
}

void Setting::ReloadSetting(const QString& iniFilePath)
{
    QSettings configIniRead(iniFilePath, QSettings::IniFormat);
    configIniRead.setIniCodec("GB18030");
    int rowCount = configIniRead.value("data/count").toInt();
    if (rowCount <= 0)
    {
        return;
    }

    m_tablePtr->setRowCount(rowCount);
    for (int i = 1; i <= rowCount; ++i)
    {
        QString text = configIniRead.value(QString("item%1/text").arg(i)).toString();

        QString fontText = configIniRead.value(QString("item%1/font").arg(i)).toString();
        //QFont font;
        //font.fromString(fontText);

        QString colorText = configIniRead.value(QString("item%1/color").arg(i)).toString();
        //QColor color;
        //color.setNamedColor(colorText);

        QString elapseSec = configIniRead.value(QString("item%1/elapse").arg(i)).toString();

        m_tablePtr->setItem(i-1, 0, new QTableWidgetItem(text));
        m_tablePtr->setItem(i-1, 1, new QTableWidgetItem(fontText));
        m_tablePtr->setItem(i-1, 2, new QTableWidgetItem(colorText));
        m_tablePtr->setItem(i-1, 3, new QTableWidgetItem(elapseSec));
    }
    ResetLabels();
}

void Setting::ResetLabels()
{
    QStringList rowIndexList;
    for (int i = 1; i <= this->m_tablePtr->rowCount(); ++i) {
        rowIndexList << QString::number(i);
    }
    m_tablePtr->setVerticalHeaderLabels(rowIndexList);
}

void Setting::PumpItem(int index)
{
    m_pumpIndex = index;
    Q_ASSERT(m_pumpIndex >= 0 && m_pumpIndex < m_tablePtr->rowCount());

    QString text = m_tablePtr->item(m_pumpIndex, 0)->text();
    QString fontText = m_tablePtr->item(m_pumpIndex, 1)->text();
    QFont font;
    font.fromString(fontText);
    QString colorText = m_tablePtr->item(m_pumpIndex, 2)->text();
    QColor color;
    color.setNamedColor(colorText);

    emit Signal_DisplayText(text, font, color);
}

void Setting::SaveSetting()
{
    QSettings configIniWrite(m_iniFilePath, QSettings::IniFormat);
    configIniWrite.setIniCodec("GB18030");
    int rowCount = m_tablePtr->rowCount();
    configIniWrite.setValue("data/count", rowCount);

    for (int i = 1; i <= rowCount; ++i)
    {
        QString text = m_tablePtr->item(i-1, 0)->text();
        QString fontText = m_tablePtr->item(i-1, 1)->text();
        QString colorText = m_tablePtr->item(i-1, 2)->text();
        QString elapseSec = m_tablePtr->item(i-1, 3)->text();

        configIniWrite.setValue(QString("item%1/text").arg(i), text);
        configIniWrite.setValue(QString("item%1/font").arg(i), fontText);
        configIniWrite.setValue(QString("item%1/color").arg(i), colorText);
        configIniWrite.setValue(QString("item%1/elapse").arg(i), elapseSec);
    }
}
