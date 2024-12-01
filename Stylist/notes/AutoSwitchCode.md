# Auto-Switch draft

## In Controller.h

```cpp
// Whether the tabs auto-switch when interacting with application widgets
bool hasAutoSwitch() const noexcept;
void setHasAutoSwitch(bool has) noexcept;

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
```

## In ControllerPrivate.h

```cpp
// Multiple instances of Controller would have different auto-switch states.
// The only thing that would persist from widget to widget is what's inside
// Stylist. However, multiple instances of Controller would be stupid
bool hasAutoSwitch = false;
QAction* autoSwitchToggle = new QAction("Auto-switch", publicClass());
std::unordered_map<WidgetGroup*, int> groupToIndex{};

void autoSwitch(QButtonGroup* buttonGroup);
void initAutoSwitchToggle();
```

## In Controller.cpp

```cpp
Controller::Controller(Stylist* stylist, QWidget* parent)
    : QWidget(parent)
    , m_private(std::make_unique<Private>(stylist, this))
{
    // ...
    m_private->initAutoSwitchToggle();
}

void Controller::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu context_menu(this);
    context_menu.addAction(m_private->autoSwitchToggle);
    context_menu.exec(event->globalPos());
}

bool Controller::hasAutoSwitch() const noexcept
{
    return m_private->hasAutoSwitch;
}

void Controller::setHasAutoSwitch(bool has) noexcept
{
    m_private->hasAutoSwitch = has;
}

void Controller::Private::autoSwitch(QButtonGroup* buttonGroup)
{
    if (!hasAutoSwitch) return;

    if (auto current_group = stylist->m_private->lastInteractedGroup())
    {
        auto index = groupToIndex[current_group];
        stackedWidget->setCurrentIndex(index);
        buttonGroup->button(index)->setChecked(true);
    }
}

void Controller::Private::createMultiGroupUi()
{
    //...

    for (auto& group : stylistGroups())
    {
        // ...

        // For auto-switching look-up
        groupToIndex[&group] = gbox_index;

        //...

    } // for (auto& group : stylistGroups())

    //...

    pub->connect
    (
        stylist,
        &Stylist::currentYokedChanged,
        pub,
        [=]
        {
            autoSwitch(button_group);
        }
    );
}

void Controller::Private::initAutoSwitchToggle()
{
    autoSwitchToggle->setCheckable(true);
    autoSwitchToggle->setChecked(hasAutoSwitch);

    auto pub = publicClass();
    pub->connect
    (
        autoSwitchToggle,
        &QAction::toggled,
        pub,
        [&](bool checked)
        {
            hasAutoSwitch = checked;
        }
    );
}
```

## In StylistPrivate.h

```cpp
//------------------------------------------------------------
// ThemesWidget auto-switch
//------------------------------------------------------------

QPointer<QWidget> currentYoked{};
QTimer* currentYokedChangedSignalChokeTimer = new QTimer(publicClass());
bool currentYokedChangedSignalChokeInit = false;

bool isYoked(QWidget* const& widget) const;
void currentYokedChangedSignalChoke();
bool isAnyMouseClick(QEvent* event) const;
bool isYokedOrChildInteraction(QEvent* event) const;
void handleYokedOrChildInteractionEvent(QObject* watched);
WidgetGroup* lastInteractedGroup() const;

void installEventFilterRecursively(QWidget* widget);
void handleChildAddedEvent(QObject* watched);
```

## In Stylist.cpp

```cpp
#define toQWidget(QObjectPtr) qobject_cast<QWidget*>(QObjectPtr)

bool Stylist::eventFilter(QObject* watched, QEvent* event)
{
    if (m_private->isYokedOrChildInteraction(event))
    {
        m_private->handleYokedOrChildInteractionEvent(watched);
        return false;
    }

    if (event->type() == QEvent::ChildAdded)
    {
        m_private->handleChildAddedEvent(watched);
        return false;
    }

    //...
}

void Stylist::yoke(QWidget* widget, int role)
{
    //...

    for (auto& group : m_private->widgetGroups)
    {
        //...
    }

    // Add event filters and place the widget in the appropriate group, creating
    // the group if necessary
    m_private->installEventFilterRecursively(widget);

    //...
}

void Stylist::Private::handleChildAddedEvent(QObject* watched)
{
    if (auto widget = toQWidget(watched))
        installEventFilterRecursively(widget);
}

// In future need a recursive uninstall, too
void Stylist::Private::installEventFilterRecursively(QWidget* widget)
{
    auto pub = publicClass();
    widget->installEventFilter(pub);

    for (auto& child : widget->findChildren<QWidget*>())
        child->installEventFilter(pub);
}

bool Stylist::Private::isAnyMouseClick(QEvent* event) const
{
    return event->type() == QEvent::MouseButtonPress
        || event->type() == QEvent::MouseButtonDblClick;
}

bool Stylist::Private::isYoked(QWidget* const& widget) const
{
    return find(widget) != nullptr;
}

bool Stylist::Private::isYokedOrChildInteraction(QEvent* event) const
{
    return isAnyMouseClick(event)
        || event->type() == QEvent::FocusIn
        || event->type() == QEvent::WindowActivate;
}

WidgetGroup* Stylist::Private::lastInteractedGroup() const
{
    return find(currentYoked);
}

// Small delay to prevent jitters. If auto-switch is on, and we choose, say, a
// yoked text editor imbedded in a yoked window, the window will be switched to
// first, before the editor
void Stylist::Private::currentYokedChangedSignalChoke()
{
    if (!currentYokedChangedSignalChokeInit)
    {
        auto pub = publicClass();
        currentYokedChangedSignalChokeTimer->setSingleShot(true);

        pub->connect
        (
            currentYokedChangedSignalChokeTimer,
            &QTimer::timeout,
            pub,
            [=]
            {
                // Don't let it crash if current group is nullptr (which it
                // shouldn't ever be--famous last words)

                // For switching, we emit the current yoked changed, but how
                // will any active theme pickers know?

                // Coco function to get address and added name?
                QTextStream oss;
                QString address{};
                oss.setString(&address);
                oss << currentYoked;

                constexpr auto format = \
                    "Last interacted-with yoked widget: \"%1\" in \"%2\"";

                qDebug(D::log) << dUtf8Format
                (
                    format,
                    address,
                    find(currentYoked)->name()
                );

                //auto q = reinterpret_cast<quintptr>(currentYoked.get());
                //auto address = QString("0x%1").arg(q, QT_POINTER_SIZE * 2, 16, QChar('0'));

                emit pub->currentYokedChanged();
            }
        );

        currentYokedChangedSignalChokeInit = true;
    }

    currentYokedChangedSignalChokeTimer->start(100);
}

/// TODO: Prevent switching if the yoked lineage is using a Controller widget
/// somewhere? We otherwise may have someone trying to edit something, touching
/// the controller and then switching to controller...

// For auto-switching to the active yoked widget. Interaction with child widgets
// may indicate a desire to interact with the parent yoked widget type. For
// instance, clicking a yoked text edit field switches to its group. If not
// yoked, it checks all parents up the ancestral line until it find a yoked
// widget, enabling the controller to recognize the correct group. Yoked
// QMainWindow installs its event filter on all children to relay interactions.
void Stylist::Private::handleYokedOrChildInteractionEvent(QObject* watched)
{
    auto widget = toQWidget(watched);

    // Run up the widget's ancestral line to find the widget that was yoked into
    // one of our type catgories
    while (widget && !isYoked(widget))
        widget = widget->parentWidget();

    // In theory, this event filter was only installed on yoked widgets, so we
    // should have hit one, but we check again (and also prevent signalling for
    // same widget)
    if (widget && isYoked(widget)
        && widget != currentYoked)
    {
        currentYoked = widget;
        currentYokedChangedSignalChoke();
    }
}

#undef toQWidget
```

## In Stylist.h

```cpp
signals:
    void currentYokedChanged();
```
