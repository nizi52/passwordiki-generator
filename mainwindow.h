#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include <QPushButton>
#include <QLabel>

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

private:
    Ui::MainWindow *ui;
    QClipboard *clipboard;
    QString generateRandomPassword(int length, bool useUpper, bool useDigits, bool useSpecial);
    void clearPasswordButtons();
    void setupPasswordButtonsLayout();
};


#endif // MAINWINDOW_H
