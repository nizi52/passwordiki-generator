 #include "mainwindow.h"
#include <QApplication>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создаём палитру
    QPalette palette;

    // 1. Основные цвета фона
    palette.setColor(QPalette::Window, QColor("#1e1c2f"));       // Основной фон окна
    palette.setColor(QPalette::Base, QColor("#2d1e36"));        // Фон текстовых полей
    palette.setColor(QPalette::AlternateBase, QColor("#2d1e36")); // Альтернативный фон

    // 2. Цвета текста
    palette.setColor(QPalette::WindowText, Qt::white);          // Основной текст
    palette.setColor(QPalette::Text, Qt::white);               // Текст в полях ввода
    palette.setColor(QPalette::ButtonText, Qt::white);         // Текст на кнопках

    // 3. Элементы интерфейса
    palette.setColor(QPalette::Button, QColor("#3e0866"));      // Кнопки
    palette.setColor(QPalette::Highlight, QColor("#6667ab"));   // Выделенные элементы
    palette.setColor(QPalette::HighlightedText, Qt::white);     // Текст выделенных элементов

    // 4. Состояния элементов
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#7f7f7f")); // Неактивный текст
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#7f7f7f")); // Неактивные кнопки

    // 5. Границы
    palette.setColor(QPalette::Dark, QColor("#429d4b"));        // Тёмные границы
    palette.setColor(QPalette::Mid, QColor("#2d1e36"));         // Средние границы

    // Устанавливаем палитру для всего приложения
    a.setPalette(palette);

    // Дополняем минимальными QSS-правилами для элементов, которые плохо стилизуются через палитру
    a.setStyleSheet(
        "QPushButton:hover { background-color: #7b337e; }"      // Кнопки при наведении
        "QTabBar::tab:selected { background: #2d1e36; }"        // Выбранные вкладки
        "QScrollBar:vertical { background: #2d1e36; }"          // Вертикальные скроллбары
        );

    MainWindow w;
    w.show();
    return a.exec();
}
