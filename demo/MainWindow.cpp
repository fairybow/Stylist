#include "MainWindow.h"

#include <QAction>
#include <QCoreApplication>
#include <QDialog>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QRect>
#include <QStatusBar>
#include <QTimer>
#include <QVBoxLayout>

#include <array>
#include <filesystem>
#include <string>

/// Note: Right now, QSS-related API only accept a std::filesystem::path
/// argument, not inline QSS
///
/// Note: WidgetGroups (internal class name for type-groups) can't be removed
/// yet. Unsure if we'd ever need to
/// 
/// Note: There is no "unstyle" option or path of logic for removing, reversing,
/// etc. Related, there's no way to remove a themes directory yet
/// 
/// Note: Controller will display with the current number of groups and won't
/// update if groups are created after. It must be recreated for now

constexpr auto CHILD_COUNT = 8;
constexpr auto GROUP_MOD = 1;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    //------------------------------------------------------------
    // Setup
    //------------------------------------------------------------

    _initUi();

    // Create child windows
    std::array<ChildWindow*, CHILD_COUNT> child_windows{};
    for (auto& window : child_windows)
        window = _newChildWindow();

    _initChildWindows(child_windows);

    //------------------------------------------------------------
    // Step 1: Set default directories
    //------------------------------------------------------------

    // (You can add more directories by calling this again.)
    m_stylist->addThemesDirectory(":/sample/");

    //------------------------------------------------------------
    // Step 2: Configure "type-groups" BEFORE yoking
    //------------------------------------------------------------

    // The basic idea: define an arbitrary number of groups of widgets based on
    // type and an optional role (to force a new group of existing type) if
    // needed

    if (GROUP_MOD > 0) // 3 groups:
    {
        // Set names for the type-groups (this will display as the tab label in
        // View)
        m_stylist->setName<MainWindow>("MainWindows [1]");
        m_stylist->setName<QPlainTextEdit>("QPlainTextEdits [2]");

        // Set a base ("unremovable"*) style for type-group 2 (* see
        // limitations)
        m_stylist->setBaseQss<QPlainTextEdit>(":/sample/EditorBase.qss");

        // Set themes template file
        m_stylist->setThemesTemplate<MainWindow>(":/sample/MainWindow.stylist_template");
        m_stylist->setThemesTemplate<QPlainTextEdit>(":/sample/Editor.stylist_template");

        // Set theme file extensions
        m_stylist->setThemesExtension<MainWindow>(".window_theme");
        m_stylist->setThemesExtension<QPlainTextEdit>(".editor_theme");

        // Let's define 2 more groups for ChildWindows:
        m_stylist->define<ChildWindow>
            (
                "ChildWindows [3]",
                ":/sample/ChildWindow.stylist_template",
                ".window_theme"
            );

        m_stylist->define<ChildWindow>
            (
                "ChildWindows (Alt) [4]",
                ":/sample/ChildWindow.stylist_template",
                ".window_theme",
                ":/sample/AltWindowBase.qss",
                1
            );

        // Set the starting themes
        m_stylist->setCurrentTheme<MainWindow>(":/sample/Theme2.window_theme");
        m_stylist->setCurrentTheme<QPlainTextEdit>(":/sample/Theme2.editor_theme");
        m_stylist->setCurrentTheme<ChildWindow>(":/sample/Theme1.window_theme");
        m_stylist->setCurrentTheme<ChildWindow>(":/sample/Theme2.window_theme", 1);

        // Toggle the theme off (showing only base style) for group 4 on
        // start-up
        m_stylist->setUseTheme<ChildWindow>(false, 1);

        //------------------------------------------------------------
        // Step 3: Yoke widgets
        //------------------------------------------------------------

        // Yoke the widgets we want stylable
        m_stylist->yoke(this);                              // type-group 1 (MainWindow, role = 0)
        m_stylist->yoke(m_editor);                          // type-group 2 (QPlainTextEdit)

        // Yoke the child windows into two separate type-groups for demonstration
        for (auto i = 0; i < CHILD_COUNT; ++i)
        {
            (i % 2 == 0)
                ? m_stylist->yoke(child_windows[i])         // type-group 3 (ChildWindow)
                : m_stylist->yoke(child_windows[i], 1);     // type-group 4 (ChildWindow, 1)
        }
    }
    else if (GROUP_MOD == 0) // 1 group:
    {
        m_stylist->define<MainWindow>
            (
                "MainWindows",
                ":/sample/MainWindow.stylist_template",
                ".window_theme"
            );

        m_stylist->setCurrentTheme<MainWindow>(":/sample/Theme2.window_theme");
        m_stylist->yoke(this);
    }
    else // (GROUP_MOD < 0) // No groups
        ;

    //------------------------------------------------------------
    // Finish-up
    //------------------------------------------------------------

    for (auto& window : child_windows)
        window->show();

    QTimer::singleShot(500, this, [&] { m_stylist->showDialog(); });

    //------------------------------------------------------------
    // Debug
    //------------------------------------------------------------

    m_stylist->dump();
    m_stylist->printGroupInfo<MainWindow>();
    m_stylist->printTemplate<MainWindow>();
}

void MainWindow::_initChildWindows(const auto& childWindows)
{
    QRect geometry(1000, 25, 200, 150);
    auto factor = 75;

    for (auto& child : childWindows)
    {
        geometry.moveTo(geometry.x() - factor, geometry.y() + factor);
        child->setGeometry(geometry);
        child->setAttribute(Qt::WA_DeleteOnClose);
    }
}

void MainWindow::_initUi()
{
    _initUiMenu();

    setStatusBar(new QStatusBar(this));
    setCentralWidget(m_editor);
    m_editor->setPlainText("Stylist demo.");
}

void MainWindow::_initUiMenu()
{
    setMenuBar(new QMenuBar(this));

    auto dialog = new QAction("Dialog", this);
    auto quit = new QAction("Quit", this);
    auto open = new QAction("Open", this);

    connect
    (
        dialog,
        &QAction::triggered,
        this,
        &MainWindow::_onOkDialogActionTriggered
    );

    connect
    (
        quit,
        &QAction::triggered,
        QCoreApplication::instance(),
        &QCoreApplication::quit,
        Qt::QueuedConnection
    );

    connect
    (
        open,
        &QAction::triggered,
        this,
        [&](bool) { m_stylist->showDialog(); }
    );

    auto file = menuBar()->addMenu("File");
    file->addAction(dialog);
    file->addSeparator();
    file->addAction(quit);

    auto stylist = menuBar()->addMenu("Stylist");
    stylist->addAction(open);
}

ChildWindow* MainWindow::_newChildWindow()
{
    auto child_window = new ChildWindow(this);
    child_window->setMenuBar(new QMenuBar(child_window));
    child_window->setStatusBar(new QStatusBar(child_window));

    auto close = new QAction("Close", child_window);

    connect
    (
        close,
        &QAction::triggered,
        child_window,
        [=] { child_window->close(); }
    );

    auto file = child_window->menuBar()->addMenu("File");
    file->addAction(close);

    return child_window;
}

// Just a random test dialog, not Stylist dialog
void MainWindow::_onOkDialogActionTriggered(bool checked)
{
    auto dialog = new QDialog(this);
    dialog->setFixedSize(300, 150);

    constexpr auto ok_dialog_text = \
        "This is a dialog with an OK button.";
    auto label = new QLabel(ok_dialog_text, dialog);
    auto ok = new QPushButton("OK", dialog);

    auto layout = new QVBoxLayout(dialog);
    layout->addWidget(label);
    layout->addWidget(ok);

    connect
    (
        ok,
        &QPushButton::clicked,
        dialog,
        &QDialog::accept
    );

    connect
    (
        dialog,
        &QDialog::finished,
        dialog,
        &QDialog::deleteLater
    );

    dialog->open();
}
