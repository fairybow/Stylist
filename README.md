# Stylist

Stylist is a small, in-progress Qt C++ library adding an in-app system for live-styling QWidgets en masse. It uses JSON-formatted themes compiled against template QSS sheets (variable replacement) and mass-applies the resulting QSS to user-defined widget type-groups.

A program using Stylist can give users a way to choose and customize themes without using QSS (on the user-side).

Stylist is being developed as a component of [Fernanda](https://github.com/fairybow/Fernanda).

<kbd>
    <img src="demo/early-demo.gif" alt=""/>
</kbd>

## Usage

### Note

Stylist API functions follow this general format:

```
m_stylist->setProperty<QWidgetT>("value");
auto value = m_stylist->property<QWidgetT>();

// If group was defined with the optional role parameter, it must be included:
m_stylist->setProperty<QWidgetT>("value", 1);
auto value = m_stylist->property<QWidgetT>(1);

// Or:
// m_stylist->setProperty(typeid(QWidgetT), "value");
// auto value = m_stylist->property(typeid(QWidgetT));
```

The type of widget is specified with `typeid(T)` or as a template parameter. The optional `role` parameter allows the creation of separate groups with the same type. So, a type-group is defined as `{ T, role = 0 }`.

### Include

```cpp
#include "Stylist/Stylist.h"

// To use the controller widget from Stylist's dialog (see below)
// #include "Stylist/Controller.h"
```

### Create Stylist and set it up to handle one or more types of QWidget

```cpp
Stylist* m_stylist = new Stylist(this);
```

(Stylist is a subclass of `QObject`.)

Define a type-group or -groups:

```cpp
m_stylist->setName<QMainWindow>("Windows");
m_stylist->setThemesTemplate<QMainWindow>(":/qrc/Window.stylist_template");
m_stylist->setThemesExtension<QMainWindow>(".window_theme");
m_stylist->setBaseQss<QMainWindow>(":/qrc/WindowBase.qss");

// Or:
// m_stylist->define<QMainWindow>(
//     "Windows",
//     ":/qrc/Window.stylist_template",
//     ".window_theme",
//     ":/qrc/WindowBase.qss");
```

Use `Stylist::setUseTheme<T>(bool)` to toggle theme at runtime. When toggled off, only base QSS (or no QSS) will show.

### Add directories containing themes files with the pre-specified extension(s)

> [!IMPORTANT] 
> Unique extensions are needed for each kind of theme.

```cpp
m_stylist->addThemesDirectory(":/qrc2/");
m_stylist->setCurrentTheme<QMainWindow>(":/qrc2/Dark.window_theme");
```

### Associate widgets with Stylist

"Yoking" (like with oxen) associates widgets with their pre-defined type-group or creates the group if it doesn't already exist.

```cpp
QTextEdit* MyMainWindow::newQTextEdit()
{
    auto text_edit = new QTextEdit(this);
    m_stylist->yoke(text_edit);

    // Setup text edit...

    return text_edit;
}
```

### Controlling Stylist

```cpp
// Open a dialog through Stylist:
m_stylist->showDialog();

// Or use Stylist::Controller to integrate the controls into something else:
// auto controller = new Stylist::Controller(m_stylist, this);
// window->setCentralWidget(controller);
```

A `Stylist::Controller` takes a required `Stylist` pointer on creation (and the usual optional `QWidget` parent pointer).

### Demo

See the [demo project](demo) for a more involved example.

## Adding Stylist to a project

Copy the `lib` and `include` folders to the target repo, and include `lib\Stylist.lib` in the compilation. Ensure `Stylist.dll` (in the `bin` folder) is also copied to the directory of the output `.exe`. Access via the `include\Stylist\Stylist.h` header.

## Limitations

### Qt style sheets

This tool is limited, of course, by any limitations of Qt Stylesheets. Of particular note, QWidgets will inherit style from their parents. For example, setting the following base QSS on a `QMainWindow` will also remove the `QSizeGrip` from its child windows:

```
QSizeGrip
{
    image: url(none);
}
```

> [!NOTE] 
> While I don't have any other specific examples in mind at this moment, if by chance anyone encounters a situation that clearly illustrates any inherent limitations, I would like to include it.
