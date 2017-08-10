#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QStyledItemDelegate>
#include <QDateTime>
#include <QMenu>
#include <QContextMenuEvent>

#include "dialogedit.h"


namespace Ui {
class MainWindow;
}

//отображение в таблице даты в нужном формате - Делегат
class mDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    mDelegate(QWidget *parent=0):QStyledItemDelegate(parent){}

    QString displayText(const QVariant &value, const QLocale &locale) const;
};


//основной класс
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_filter_clicked();

    void on_pushButton_filter_no_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_update_clicked();


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    DialogEdit *editForm;
    mDelegate *myDelegate;
    QMenu *m_menu;

protected:
    void contextMenuEvent( QContextMenuEvent *event_menu);
};



#endif // MAINWINDOW_H
