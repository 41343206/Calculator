#include "calculator.h"
#include <QGridLayout>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), value(0.0), waitingForOperand(true)
{
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setMinimumHeight(50);
    display->setAlignment(Qt::AlignRight);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(display, 0, 0, 1, 4);

    QPushButton *btnBackspace = new QPushButton("←");
    QPushButton *btnDiv = new QPushButton("/");
    QPushButton *btnMul = new QPushButton("*");
    QPushButton *btnSub = new QPushButton("-");

    btnBackspace->setMinimumSize(60,60);
    btnDiv->setMinimumSize(60,60);
    btnMul->setMinimumSize(60,60);
    btnSub->setMinimumSize(60,60);

    layout->addWidget(btnBackspace, 1, 0);
    layout->addWidget(btnDiv,       1, 1);
    layout->addWidget(btnMul,       1, 2);
    layout->addWidget(btnSub,       1, 3);

    connect(btnBackspace, &QPushButton::clicked, this, &Calculator::onBackspaceClicked);
    connect(btnDiv, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(btnMul, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(btnSub, &QPushButton::clicked, this, &Calculator::onOperatorClicked);

    QStringList digits = { "7","8","9","4","5","6","1","2","3" };
    int index = 0;
    for(int row = 2; row <= 4; row++){
        for(int col = 0; col < 3; col++){
            QPushButton *btn = new QPushButton(digits[index]);
            btn->setMinimumSize(60,60);
            layout->addWidget(btn, row, col);
            connect(btn, &QPushButton::clicked, this, &Calculator::onDigitClicked);
            index++;
        }
    }

    QPushButton *btnAdd = new QPushButton("+");
    btnAdd->setMinimumSize(60,120);
    layout->addWidget(btnAdd, 2, 3, 2, 1);
    connect(btnAdd, &QPushButton::clicked, this, &Calculator::onOperatorClicked);

    QPushButton *btnZero = new QPushButton("0");
    QPushButton *btnDot = new QPushButton(".");
    QPushButton *btnEqual = new QPushButton("=");

    btnZero->setMinimumHeight(60);
    btnDot->setMinimumSize(60,60);
    btnEqual->setMinimumHeight(120);

    layout->addWidget(btnZero, 5, 0, 1, 2);
    layout->addWidget(btnDot,  5, 2);
    layout->addWidget(btnEqual, 4, 3, 2, 1);

    connect(btnZero, &QPushButton::clicked, this, &Calculator::onDigitClicked);
    connect(btnDot, &QPushButton::clicked, this, &Calculator::onDotClicked);
    connect(btnEqual, &QPushButton::clicked, this, &Calculator::onEqualClicked);
}

Calculator::~Calculator()
{
}

void Calculator::onDigitClicked()
{
    QPushButton *clickedBtn = qobject_cast<QPushButton *>(sender());
    QString digit = clickedBtn->text();

    if (display->text() == "0" || waitingForOperand)
        display->clear();

    display->setText(display->text() + digit);
    waitingForOperand = false;
}

void Calculator::onDotClicked()
{
    if (waitingForOperand)
        display->setText("0");

    if (!display->text().contains("."))
        display->setText(display->text() + ".");

    waitingForOperand = false;
}

void Calculator::onOperatorClicked()
{
    QPushButton *clickedBtn = qobject_cast<QPushButton *>(sender());
    QString op = clickedBtn->text();

    double operand = display->text().toDouble();

    if (!pendingOperator.isEmpty())
        calculate(operand, pendingOperator);
    else
        value = operand;

    pendingOperator = op;
    waitingForOperand = true;
}

void Calculator::onEqualClicked()
{
    double operand = display->text().toDouble();
    calculate(operand, pendingOperator);
    pendingOperator.clear();
    waitingForOperand = true;
}

void Calculator::onClearClicked()
{
    value = 0.0;
    pendingOperator.clear();
    display->setText("0");
    waitingForOperand = true;
}

void Calculator::onBackspaceClicked()
{
    QString text = display->text();
    text.chop(1);
    if (text.isEmpty()) text = "0";
    display->setText(text);
}

void Calculator::calculate(double rhs, const QString &op)
{
    if (op == "+") value = value + rhs;
    else if (op == "-") value = value - rhs;
    else if (op == "*") value = value * rhs;
    else if (op == "/") {
        if (rhs == 0){
            display->setText("Error");
            return;
        }
        value = value / rhs;
    }

    display->setText(QString::number(value));
}
