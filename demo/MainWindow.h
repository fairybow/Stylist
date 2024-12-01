#pragma once

#include "Stylist/Stylist.h"

#include <QMainWindow>
#include <QPlainTextEdit>

class ChildWindow : public QMainWindow
{
    Q_OBJECT

public:
    using QMainWindow::QMainWindow;

}; // class ChildWindow

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private:
    Stylist* m_stylist = new Stylist(this);
    QPlainTextEdit* m_editor = new QPlainTextEdit(this);

    void _initUi();
    void _initUiMenu();

    void _initChildWindows(const auto& childWindows);
    ChildWindow* _newChildWindow();

private slots:
    void _onOkDialogActionTriggered(bool checked = false);

}; // class MainWindow
