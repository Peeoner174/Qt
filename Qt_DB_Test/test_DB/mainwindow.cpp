#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Database
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("test_db");
    db.setUserName("root");
    db.setPassword("123");

    //проверка подключения
    if(!db.open()){
        qDebug() << db.lastError().text();
        return;
    }
    else{
        qDebug() << "Connection to database";
    }

    //первоначальный вывод
    model = new QSqlTableModel(this, db);
    model->setTable("test_table");

    //шапка
    model->setHeaderData(1,Qt::Horizontal, "Дата создания");
    model->setHeaderData(2,Qt::Horizontal, "Текст задачи");
    model->setHeaderData(3,Qt::Horizontal, "Критерий");
    model->setHeaderData(4,Qt::Horizontal, "Срок");
    model->setHeaderData(5,Qt::Horizontal, "Ответственный");
    model->setHeaderData(6,Qt::Horizontal, "Дата выполнения");

    model->select();

    //вид дата в виде dd.MM.yyyy
    myDelegate = new mDelegate;
    ui->tableView->setItemDelegateForColumn(1, myDelegate);
    ui->tableView->setItemDelegateForColumn(4, myDelegate);
    ui->tableView->setItemDelegateForColumn(6, myDelegate);

    ui->tableView->setModel(model);

    //скрыть id
    ui->tableView->setColumnHidden(0, true);

    //ширина столбцов
    ui->tableView->setColumnWidth(1, 90);
    ui->tableView->setColumnWidth(2, 180);
    ui->tableView->setColumnWidth(3, 130);
    ui->tableView->setColumnWidth(4, 90);
    ui->tableView->setColumnWidth(5, 110);

    //даты в фильтра - неделя от сегодня
    ui->dateEdit_min->setDate(QDate::currentDate());
    QDate nowDate = ui->dateEdit_min->date();
    ui->dateEdit_max->setDate(nowDate.addDays(7));

    //окно редактирования
    editForm = new DialogEdit();
    editForm->setParent(this, Qt::Dialog);
    editForm->setModel(model);

    //автоматическое обновление после добавления строк
    connect(editForm, SIGNAL(updateSignal()), this, SLOT(on_pushButton_update_clicked()));

    //конетекстное меню
    m_menu = new QMenu(this);

    QAction *newRowAction = m_menu->addAction("Новая запись");
    QAction *editRowAction = m_menu->addAction("Редактировать запись");
    QAction *delRowAction = m_menu->addAction("Удалить запись");
    m_menu->addSeparator();
    QAction *exitAction = m_menu->addAction("Выход");

    //обработка нажатий меню
    connect(newRowAction, SIGNAL(triggered(bool)), this, SLOT(on_pushButton_add_clicked()));
    connect(editRowAction, SIGNAL(triggered(bool)), this, SLOT(on_pushButton_edit_clicked()));
    connect(delRowAction, SIGNAL(triggered(bool)), this, SLOT(on_pushButton_del_clicked()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(on_pushButton_exit_clicked()));

 }

MainWindow::~MainWindow()
{
    delete ui;
}


//добавить строку
void MainWindow::on_pushButton_add_clicked()
{
    model->insertRow(model->rowCount());
    int newRow = model->rowCount();
    ui->tableView->selectRow(newRow-1);
    on_tableView_doubleClicked(ui->tableView->currentIndex());
}


//удалить строку
void MainWindow::on_pushButton_del_clicked()
{
    int selectedRow = ui->tableView->currentIndex().row();
    if(selectedRow >= 0){
        model->removeRow(selectedRow);
        model->select();
    }
    else{
        qDebug() << "No selected row";
    }
}

//редактировать строку (из меню)
void MainWindow::on_pushButton_edit_clicked()
{
    int selectedRow = ui->tableView->currentIndex().row();
    if(selectedRow >= 0){
    QModelIndex index = ui->tableView->currentIndex();
    editForm->mapper->setCurrentModelIndex(index);
    editForm->show();
    }
    else{
        qDebug() << "No selected row";
    }
}


//закрыть приложение
void MainWindow::on_pushButton_exit_clicked()
{
    close();
}


//запрос с фильтром вида yyyy.MM.dd
void MainWindow::on_pushButton_filter_clicked()
{
    //поле test_ending должно быть в базе!!!

    QString mystr = "test_ending between '" + QString::number(ui->dateEdit_min->date().year()) + "."
                                            + QString::number(ui->dateEdit_min->date().month()) + "."
                                            + QString::number(ui->dateEdit_min->date().day()) + "' and '"
                                            + QString::number(ui->dateEdit_max->date().year()) + "."
                                            + QString::number(ui->dateEdit_max->date().month()) + "."
                                            + QString::number(ui->dateEdit_max->date().day()) + "'";

     model->setFilter(mystr);
     qDebug() << model->filter();

}

//сбросить фильтр
void MainWindow::on_pushButton_filter_no_clicked()
{
    model->setFilter("");
    qDebug() << model->filter();
}


//вывод даты в виде dd.MM.yyyy
QString mDelegate::displayText(const QVariant &value, const QLocale &locale) const{
    return value.toDateTime().toString("dd.MM.yyyy");
    qDebug() << locale; // что бы не выводилось предупреждение о неиспользуемом параметре

}


//вызов окна редактирования (двойной клик)
void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    editForm->mapper->setCurrentModelIndex(index);
    editForm->show();
}

//обновить данные
void MainWindow::on_pushButton_update_clicked()
{
    model->select();
}

//вывод контекстного меню
void MainWindow::contextMenuEvent(QContextMenuEvent *event_menu) {
    if(m_menu) {
        m_menu->exec(event_menu->globalPos());
    }
}
