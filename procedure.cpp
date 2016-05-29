#include "procedure.h"
#include "sems.h"

procedure::procedure()
{

}

void procedure::compileHeader(){

}

QByteArray procedure::compileAtom(QString atom){
    QByteArray result;

    //QRegExp QR("\\\"[\\w\\W]+\\\"|[\\w]+|[\\,\\(\\)]");

    QRegExp QRE_FUNCTION("[\w]+");
    QRegExp QRE_COMMA("\,");
    QRegExp QRE_TEXT("\"([\w\s\d]+)\"");

    // To do:
    /*
     *
     * Check atom, what's this?
     *
     * 1. Function
     * 2. End of function
     * 3. If ..
     * 4. GoTo
     * 5. Separator
     * 6. Comma ','
     * 7. String number
     * 8. Procedure begin
     * 9. Operand
     * 10. End of program 0xFF
     * 11. Const16
     * 12. Const32
     * 13. Variant
     *
     * */

    // 1. Регулярное выражение - список функций, если нет строгого соответствия, значит не функция
    /*
     * 2. Конец функции - ...
     *
     * 3. Регулярка с условным переходом и выражение в аргументе.
     *      Выражение в аргументе....
     *
     * 4. Строгое соответствие шаблону "Безусловный переход в начало цикла"; continue;
     *
     * 5. Это типа любое из ; } {  если я правильно понял
     *
     * 6. Запятая
     *
     * 7. Номер строки - Получим другим путем
     * */

    Sems SEMS;
    QString functions  = SEMS.allFunctions();
    QString operations = SEMS.allOps();

    QString QRSBrakes = "[\\s\\t]*\\([\\w\\W]*\\)";

    QRegExp QR_function(functions);         // 1
      // 2 - pass, eoF will received in other way

    QRegExp QR_if("if[\\s\\t]*\\([\\w\\W]*\\)");  // 3
    QRegExp QR_continue("continue");        // 4
    QRegExp QR_separaror("\\;|\\{|\\}");       // 5
    QRegExp QR_comma("\\,");                 // 6
     // 7 - other way

    QRegExp QR_procedure("procedure"+QRSBrakes); // 8
    QRegExp QR_operand(operations);              // 9

    QRegExp QR_end("");  // 10 end of Program - other way

    QRegExp QR_const16(); // регулярка - все константы
    QRegExp QR_const32();

    QRegExp QR_Variant(); // список всех переменных

    QRegExp QR_expression(); // выражение
    //QRegExp QR_var(SEMS.allTypes+"[\\s\\t]*([w]+)"); // объявление переменной (до знака равенства)

    // Все переменные можно заранее найти и занести
    // в контейнер а из него в регулярку отдельным методом
    /*
     * Пройтись с шаблоном тип переменной - переменная [= выражение]
     *
     * что для этого нужно?
     * шаблон выражений
     * шаблон объявления переменной "тип переменная"

    */

    return result;
}

void procedure::splitStr(QString str, QList<QString> &atoms){
    QByteArray result;
    if(str.length()<1) return;

    //QRegExp QR("\\\"[\\w\\W]+\\\"|[\\w]+|[\\,\\(\\)]", Qt::CaseInsensitive);
    Sems sems;
    QString C = "\\>\\=|\\>|\\<|\\+|\\/|\\-|\\^|\\\\|\\<=|\\||\\=|\\*|\\<\\>|\\:\\=|\\&"; // all Operators.
    QString B = "[\\s\\t]*"; // all spaces and tabs
    QString E = "[\\w]+";     // all functions, keywords, variables types and other that contains alphabet symbols and numbers and _
    QString CLS = "\\[|\\]|\\(|\\)|\\,|\\{|\\}";  // brakes and comma;
    QString M = "\\\"[\\w\\W]*\\\"";    // all in quotes
    QString S = "\\;";          // separator

    QString Z = C + "|" + E + "|" + CLS + "|" + M + "|" + S; // critical mass of uranium

    QRegExp QR(E);
    QR.setMinimal(false);

    //int count = 0;
    int pos = 0;

    while ((pos = QR.indexIn(str, pos)) != -1) {
        //++count;
        pos += QR.matchedLength();
        atoms.append(QR.cap(0));
        compileAtom(QR.cap(0));
    }
}

void procedure::compile(){
    QStringList::iterator it;
    QString stroke;

    for(it = this->code.begin(); it != this->code.end(); ++it){
        stroke = *it;
        splitStr(*it, this->atoms);
    }
    this->code;
}
