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
#include <QtMath>
#include <QRegularExpression>
#include <QLocale>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icon.png"));
    clipboard = QGuiApplication::clipboard();
    setupPasswordButtonsLayout();


    // Настройка диапазона чисел до 1 миллиона
    ui->minNumberSpinBox->setMaximum(1000000);
    ui->maxNumberSpinBox->setMaximum(1000000);

    // Анимация плавного появления окна
    QPropertyAnimation *anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->start();

    // Стиль кнопки генерации
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

    // Тот же стиль для кнопки генерации чисел
    ui->generateNumberButton->setStyleSheet(
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

QString MainWindow::formatLargeNumber(double number) const
{
    if (number < 1e3) return QString::number(number, 'f', 0);

    int exponent = static_cast<int>(log10(number));
    double base = number / pow(10, exponent);

    // Для очень больших чисел используем экспоненциальную запись
    if (exponent >= 6) {
        return QString("%1×10<sup>%2</sup>").arg(base, 0, 'f', 2).arg(exponent);
    }

    return QString("10<sup>%1</sup>").arg(exponent);
}

QString MainWindow::calculateCrackAttempts(const QString& password)
{
    const qint64 GUESSES_PER_SECOND = 1e9; // 1 млрд попыток в секунду
    const qint64 SECONDS_IN_YEAR = 31536000; // 60*60*24*365

    int entropy = calculatePasswordEntropy(password);
    double totalCombinations = pow(2, entropy);

    // Форматируем число комбинаций
    QString attempts = formatLargeNumber(totalCombinations);

    // Рассчитываем время взлома
    double yearsToCrack = totalCombinations / (GUESSES_PER_SECOND * SECONDS_IN_YEAR);
    QString timeDescription;

    if (yearsToCrack < 1.0/SECONDS_IN_YEAR) {
        timeDescription = "мгновенно";
    }
    else if (yearsToCrack < 1.0) {
        timeDescription = QString("%1 дней").arg(yearsToCrack * 365, 0, 'f', 0);
    }
    else if (yearsToCrack < 1e6) {
        timeDescription = QString("%1 лет").arg(yearsToCrack, 0, 'f', 1);
    }
    else {
        timeDescription = QString("%1 лет").arg(formatLargeNumber(yearsToCrack));
    }

    return QString("1 к %1\n(Время взлома: %2)").arg(attempts).arg(timeDescription);
}

int MainWindow::calculatePasswordEntropy(const QString& password)
{
    int charsetSize = 0;

    if (password.contains(QRegularExpression("[a-z]"))) charsetSize += 26;
    if (password.contains(QRegularExpression("[A-Z]"))) charsetSize += 26;
    if (password.contains(QRegularExpression("[0-9]"))) charsetSize += 10;
    if (password.contains(QRegularExpression("[^a-zA-Z0-9]"))) charsetSize += 32;

    return qRound(password.length() * log2(charsetSize));
}

QString MainWindow::getPasswordStrengthDescription(int entropy)
{
    if (entropy < 28) return "Очень слабый";
    if (entropy < 36) return "Слабый";
    if (entropy < 60) return "Средний";
    if (entropy < 128) return "Сильный";
    return "Очень сильный";
}

void MainWindow::checkPasswordStrength(const QString& password)
{
    int entropy = calculatePasswordEntropy(password);
    QString attempts = calculateCrackAttempts(password);
    QString strength = getPasswordStrengthDescription(entropy);

    QString result = QString("<html><style>sup {font-size: smaller}</style>"
                             "Надежность: <b>%1</b><br>"
                             "Энтропия: %2 бит<br>"
                             "Шанс угадать: %3</html>")
                         .arg(strength)
                         .arg(entropy)
                         .arg(attempts);

    ui->passwordStrengthLabel->setText(result);
    ui->passwordStrengthLabel->setTextFormat(Qt::RichText);
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

    checkPasswordStrength(password);
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
            checkPasswordStrength(password);
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


