#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGuiApplication>
#include <QRandomGenerator>
#include <QToolTip>

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clipboard = QGuiApplication::clipboard();
    setupPasswordButtonsLayout();

    // Настройка диапазона чисел до 1 миллиона
    ui->minNumberSpinBox->setMaximum(1000000);
    ui->maxNumberSpinBox->setMaximum(1000000);

    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->start();


    // Настраиваем стиль кнопки генерации
    ui->generatePasswordsButton->setStyleSheet(
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "               stop:0 #7b337e, stop:1 #3e0866);"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 24px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "               stop:0 #3e0866, stop:1 #7b337e);"
        "}"
        );

    connect(ui->generatePasswordsButton, &QPushButton::clicked,
            this, &MainWindow::generatePasswords);
    connect(ui->generateNumberButton, &QPushButton::clicked,
            this, &MainWindow::generateRandomNumber);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPasswordButtonsLayout()
{
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setSpacing(5);
    layout->setContentsMargins(2, 2, 2, 2);
    container->setLayout(layout);
    ui->scrollArea->setWidget(container);
    ui->scrollArea->setWidgetResizable(true);
}

void MainWindow::clearPasswordButtons()
{
    QWidget *container = ui->scrollArea->widget();
    if (container) {
        QLayout *layout = container->layout();
        if (layout) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0))) {
                if (item->widget()) {
                    delete item->widget();
                }
                delete item;
            }
        }
    }
}

QString MainWindow::generateRandomPassword(int length, bool useUpper, bool useDigits, bool useSpecial)
{
    const QString lowerChars = "abcdefghijklmnopqrstuvwxyz";
    const QString upperChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const QString digits = "0123456789";
    const QString specialChars = "!@#$%^&*()_+-=[]{}|;:,.<>?";

    QString allChars = lowerChars;
    if (useUpper) allChars += upperChars;
    if (useDigits) allChars += digits;
    if (useSpecial) allChars += specialChars;

    QString password;
    password.reserve(length);
    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(allChars.length());
        password.append(allChars.at(index));
    }
    return password;
}

void MainWindow::generatePasswords()
{
    clearPasswordButtons();

    int length = ui->passwordLengthSpinBox->value();
    bool useUpper = ui->uppercaseCheckBox->isChecked();
    bool useDigits = ui->digitsCheckBox->isChecked();
    bool useSpecial = ui->specialCharsCheckBox->isChecked();

    QWidget *container = ui->scrollArea->widget();
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(container->layout());

    for (int i = 0; i < 5; ++i) {
        QString password = generateRandomPassword(length, useUpper, useDigits, useSpecial);
        QPushButton *btn = new QPushButton(password, container);

        // Стиль с анимацией и градиентом
        btn->setStyleSheet(
            "QPushButton {"
            "   padding: 10px;"
            "   font-size: 14px;"
            "   text-align: center;"
            "   border: none;"
            "   border-radius: 8px;"
            "   min-width: 220px;"
            "   color: white;"
            "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
            "               stop:0 #4b0082, stop:1 #6600ff);"
            "   box-shadow: 0 2px 4px rgba(0,0,0,0.2);"
            "   transition: all 0.3s ease;"
            "}"
            "QPushButton:hover {"
            "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
            "               stop:0 #6600ff, stop:1 #4b0082);"
            "   transform: translateY(-2px);"
            "   box-shadow: 0 4px 8px rgba(0,0,0,0.3);"
            "}"
            "QPushButton:pressed {"
            "   transform: translateY(1px);"
            "   box-shadow: 0 1px 2px rgba(0,0,0,0.2);"
            "}"
            );

        connect(btn, &QPushButton::clicked, [this, password]() {
            clipboard->setText(password);
            QToolTip::showText(QCursor::pos(), "✓ Скопировано в буфер обмена", nullptr, {}, 1500);
        });

        layout->addWidget(btn);
    }
    layout->addStretch();

}


void MainWindow::generateRandomNumber()
{
    int min = ui->minNumberSpinBox->value();
    int max = ui->maxNumberSpinBox->value();

    if (min >= max) {
        QMessageBox::warning(this, "Ошибка",
                             "Минимальное значение должно быть меньше максимального!");
        return;
    }

    int randomNum = QRandomGenerator::global()->bounded(min, max + 1);
    ui->randomNumberLabel->setText(QString::number(randomNum));


}

