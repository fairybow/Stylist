/*
* Stylist: Theme.cpp  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2024-11-30
*/

#include "Coco/Io.h"
#include "Debug.h"
#include "Theme.h"

#include <QJsonObject>
#include <QRegularExpression>

//------------------------------------------------------------
// Internal
//------------------------------------------------------------

constexpr auto NAME_KEY = "name";
constexpr auto VARS_KEY = "variables";

static QRegularExpression variableRegex(const QString& key)
{
    constexpr auto variable_format = "@+%1\\b";
    return QRegularExpression(QString(variable_format).arg(key));
}

//------------------------------------------------------------
// Theme definitions
//------------------------------------------------------------

Theme::Theme() = default;
Theme::Theme(const Theme& theme) = default;
//Theme::Theme(Theme&& theme) noexcept = default;

Theme::Theme(const Coco::Path& path)
    : m_path(path)
{
    auto theme_file_text = Coco::Io::readTxt(path);
    m_jsonDoc = QJsonDocument::fromJson(theme_file_text.toUtf8());
}

Theme::~Theme() = default;

//Theme& Theme::operator=(const Theme& other) = default;
//Theme& Theme::operator=(Theme&& other) noexcept = default;
bool Theme::operator==(const Theme& other) const = default;
bool Theme::operator!=(const Theme& other) const = default;

// A-Z
//------------------------------------------------------------

QString Theme::compileQss(const QString& themeTemplate) const
{
    QString result = themeTemplate;
    auto vars = m_jsonDoc
        .object()
        .value(VARS_KEY)
        .toObject();

    for (
        auto it = vars.begin();
        it != vars.end();
        ++it
        )
    {
        result.replace
        (
            variableRegex(it.key()),
            it.value().toString()
        );
    }

    return result;
}

bool Theme::isEmpty() const
{
    return m_jsonDoc.isEmpty();
}

bool Theme::isNull() const
{
    return m_jsonDoc.isNull();
}

QString Theme::name() const
{
    return m_jsonDoc.object()
        .value(NAME_KEY)
        .toString();
}

Coco::Path Theme::path() const
{
    return m_path;
}

void Theme::setName(const QString& name)
{
    auto json_object = m_jsonDoc.object();
    json_object[NAME_KEY] = name;
    m_jsonDoc.setObject(json_object);
}

void Theme::setPath(const Coco::Path& path)
{
    m_path = path;
}

QByteArray Theme::toJson() const
{
    return m_jsonDoc.toJson();
}

QVariant Theme::toQVariant() const
{
    return QVariant::fromValue(*this);
}
