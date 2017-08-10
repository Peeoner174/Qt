#include "dialogedit.h"
#include "ui_dialogedit.h"

DialogEdit::DialogEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogEdit)
{
    ui->setupUi(this);


    //установка дат при редактировании
    setAllDate();


    mapper = new QDataWidgetMapper(this);

    //применение после нажатия кнопки
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

}

DialogEdit::~DialogEdit()
{
    delete ui;
}


//связь с таблицей
void DialogEdit::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(ui->dateEdit_create, 1);
    mapper->addMapping(ui->textEdit, 2, "plainText");
    mapper->addMapping(ui->lineEdit_criterion, 3);
    mapper->addMapping(ui->dateEdit_end, 4);
    mapper->addMapping(ui->lineEdit_man, 5);
    mapper->addMapping(ui->dateEdit_finish, 6);
}

//применить изменения и закрыть
void DialogEdit::on_pushButton_ok_clicked()
{
    mapper->submit();
    emit updateSignal();
    close();
    setAllDate();
}

//закрыть без изменений
void DialogEdit::on_pushButton_cancel_clicked()
{
    emit updateSignal();
    close();
    setAllDate();
}

//сброс дат в поле редактирования после закрытия
void DialogEdit::setAllDate(){
    ui->dateEdit_create->setDate(QDate::currentDate());
    QDate nowDate = ui->dateEdit_create->date();
    ui->dateEdit_end->setDate(nowDate.addDays(7));
    ui->dateEdit_finish->setDate(nowDate.addDays(7));
}
