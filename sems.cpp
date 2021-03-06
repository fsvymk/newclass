#include "sems.h"

void Sems::semsInitDefault()
{
    this->Sem.insert("rotl",        0x01);
    this->Sem.insert("rotr",        0x02);
    this->Sem.insert("ashl",        0x03);
    this->Sem.insert("ashr",        0x04);
    this->Sem.insert("iif",         0x05);
    this->Sem.insert("read",        0x06);
    this->Sem.insert("write",       0x07);
    this->Sem.insert("write_all",	0x08);
    this->Sem.insert("write_sel",	0x09);
    this->Sem.insert("on",          0x0A);
    this->Sem.insert("off",         0x0B);
    this->Sem.insert("local_send_event",	0x0C);
    this->Sem.insert("send_event",	0x0D);
    this->Sem.insert("send_event_sel",	0x0E);
    this->Sem.insert("send_event_sync",	0x0F);
    this->Sem.insert("send_event_sync_sel",	0x10);
    this->Sem.insert("send_event_all",	0x11);
    this->Sem.insert("end_proc",	0x12);
    this->Sem.insert("delay",       0x13);
    this->Sem.insert("make_dialog",	0x14);
    this->Sem.insert("print",       0x15);
    this->Sem.insert("telemetry",	0x16);
    this->Sem.insert("telemetry_reset",	0x17);
    this->Sem.insert("check_hw_connection", 	0x18);
    this->Sem.insert("break",       0x19);
    this->Sem.insert("continue",	0x20);
    this->Sem.insert("transmitt",	0x21);
    this->Sem.insert("receive",     0x22);
    this->Sem.insert("debug_prnt",	0x23);
}

/*
начало круглой скобки		0x01
конец круглой скобки		0x02
начало квадратной скобки (индекс)		0x11
конец квадратной скобки (индекс)		0x12
начало фигурной скобки (начало программного блока)		0x21
конец фигурной скобки (конец программного блока)		0x22
*/

void Sems::initBrakets(){
        this->Brakets.insert("(",   0x01);
        this->Brakets.insert(")",   0x02);
        this->Brakets.insert("[",   0x11);
        this->Brakets.insert("]",   0x12);
        this->Brakets.insert("{",   0x21);
        this->Brakets.insert("}",   0x22);
}

void Sems::initOps(){
    this->Opeartion.insert("&",     0x01);
    this->Opeartion.insert("|",     0x02);
    this->Opeartion.insert("^",     0x03);
    this->Opeartion.insert("*",     0x11);
    this->Opeartion.insert("/",     0x12);
    this->Opeartion.insert("\\",	0x13);
    this->Opeartion.insert("+",     0x21);
    this->Opeartion.insert("-",     0x22);
    this->Opeartion.insert("=",     0x31);
    this->Opeartion.insert("<>",	0x32);
    this->Opeartion.insert("<",     0x33);
    this->Opeartion.insert("<=",	0x34);
    this->Opeartion.insert(">=",	0x35);
    this->Opeartion.insert(">",     0x36);
    this->Opeartion.insert(":=",	0xE1);
    this->Opeartion.insert("pseudo",0xF1); // псевдо - операция
}

void Sems::initErrors(){
    this->errorCode.insert("unknown-command",   0x8001); //	Неизвестная команда
    this->errorCode.insert("command-format",    0x8002); //	Неверный формат команды
    this->errorCode.insert("non-existent",      0x8003); //	Обращение к несуществующему оборудованию
    this->errorCode.insert("type-discrepancy",  0x8004); //	Несоответствие типов оборудования
    this->errorCode.insert("entering-data",     0x8005); //	Программирование - ошибка во входных данных
    this->errorCode.insert("link-module",       0x8006); //	Программирование - ошибка при записи программы в модуль
    this->errorCode.insert("counter-discrepancy", 0x8007); //	Сбой обмена по сети (несовпадение ожидаемого и пришедшего счетчиков в пакетах)
    this->errorCode.insert("var-type",          0x8008); //	Недопустимый тип переменной в данной операции
    this->errorCode.insert("unexpected-end",    0x8009); //	Ошибка в данных для программирования - неожиданный конец
}

void Sems::initPreDefs(){
    this->predefinedVars.insert("shift_bit",    0xF1);
    this->predefinedVars.insert("rg",           0xF0);
    this->predefinedVars.insert("m_block",      0xF2);
    this->predefinedVars.insert("m_pos",        0xF3);
    this->predefinedVars.insert("m_type",       0xF4);
    this->predefinedVars.insert("m_channel",	0xF5);
    this->predefinedVars.insert("__zero",       0xFE);
    this->predefinedVars.insert("__FF",         0xFF);
}

void Sems::initTypes(){
    this->Type.insert("word", 0x00);
    this->Type.insert("dword", 0x01);
    this->Type.insert("uword", 0x02);
    this->Type.insert("udword", 0x03);
    this->Type.insert("array", 0x04);
    this->Type.insert("msg", 0x05);
}

void Sems::initJump(){
    this->Jump.insert("while",  0);
    this->Jump.insert("if",     1);
    this->Jump.insert("else",   2);
}

Sems::Sems()
{
    semsInitDefault();
    initOps();
    initErrors();
    initPreDefs();
    initBrakets();
    initJump();
}


QString Sems::allFunctions(){
    QString result;

    QHash<QString, int>::iterator it;
    for(it = this->Sem.begin(); it != this->Sem.end(); ++it){
        result.append(it.key());
        result.append("|");
    }

    return result;
}

// Hello, china!
QString Sems::allOps(){
    QString result;

    QHash<QString, int>::iterator it;
    for(it = this->Opeartion.begin(); it != this->Opeartion.end(); ++it){
        result.append("\\");
        result.append(it.key());
        result.append("|");
    }

    return result;
}

// Hello, china!
QString Sems::allTypes(){
    QString result;

    QHash<QString, int>::iterator it;
    for(it = this->Type.begin(); it != this->Type.end(); ++it){
        result.append("\\");
        result.append(it.key());
        result.append("|");
    }

    return result;
}
