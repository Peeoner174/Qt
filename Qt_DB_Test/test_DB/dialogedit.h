#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QWidget>
#include <QDataWidgetMapper>


namespace Ui {
class DialogEdit;
}

class DialogEdit : public QWidget
{
    Q_OBJECT

signals:
    void updateSignal();

public:
    explicit DialogEdit(QWidget *parent = 0);
    ~DialogEdit();

    void setModel (QAbstractItemModel *model);

    QDataWidgetMapper *mapper;

    void setAllDate();

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::DialogEdit *ui;

};

#endif // DIALOGEDIT_H
