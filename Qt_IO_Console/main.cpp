#include <QTextStream>
#include <QString>

int main()
{
    QTextStream cin(stdin);
    QTextStream cout(stdout);

    QString str;

    cout << "Input Name: ";
    cout.flush();

    cin >> str;

    cout << "Hello, " << str << endl;

    return 0;
}
