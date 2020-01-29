#ifndef NEWITEM_H
#define NEWITEM_H

#include <QDialog>

namespace Ui {
class NewItem;
}

class NewItem : public QDialog
{
    Q_OBJECT

public:
    explicit NewItem(QWidget *parent = nullptr);
    ~NewItem();

    bool GetNewItem(QString &text, QString &font, QString &color, QString &elapse);

private:
    Ui::NewItem *ui;
};

#endif // NEWITEM_H
