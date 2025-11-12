#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void onDigitClicked();
    void onOperatorClicked();
    void onEqualClicked();
    void onClearClicked();
    void onBackspaceClicked();
    void onDotClicked();

private:
    QLineEdit *display;
    double value;
    QString pendingOperator;
    bool waitingForOperand;

    void calculate(double rhs, const QString &op);
};

#endif
