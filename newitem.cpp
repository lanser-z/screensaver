#include "newitem.h"
#include "ui_newitem.h"

#include <QColorDialog>
#include <QFontDialog>

NewItem::NewItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewItem)
{
    ui->setupUi(this);

    QMetaObject::Connection conn;
    conn = connect(ui->toolButton, &QToolButton::clicked,
                   [this](bool){
        bool ok;
        QFont font = QFontDialog::getFont(&ok);
        if (ok) {
            this->ui->lineEdit_2->setText(font.toString());
        }
    });
    Q_ASSERT(conn);

    conn = connect(ui->toolButton_2, &QToolButton::clicked,
                   [this](bool){
        QColor color = QColorDialog::getColor(Qt::white);
        if (color.isValid()) {
            this->ui->lineEdit_3->setText(color.name());
        }
    });
    Q_ASSERT(conn);
}

NewItem::~NewItem()
{
    delete ui;
}

bool NewItem::GetNewItem(QString &text, QString &font, QString &color, QString &elapse)
{
    text = ui->lineEdit->text();
    font = ui->lineEdit_2->text();
    color = ui->lineEdit_3->text();
    elapse = QString::number(ui->spinBox->value());
    if (!text.isEmpty() && !font.isEmpty() && !color.isEmpty() && !elapse.isEmpty())
    {
        return true;
    }
    return false;
}
