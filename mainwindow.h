#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generatePasswords();
    void generateRandomNumber();
    void checkPasswordStrength(const QString& password);

private:
    Ui::MainWindow *ui;
    QClipboard *clipboard;

    QString generateRandomPassword(int length, bool useUpper, bool useDigits, bool useSpecial);
    void setupPasswordButtonsLayout();
    void clearPasswordButtons();
    int calculatePasswordEntropy(const QString& password);
    QString calculateCrackAttempts(const QString& password);
    QString getPasswordStrengthDescription(int entropy);
    QString formatLargeNumber(double number) const;
};

#endif // MAINWINDOW_H
