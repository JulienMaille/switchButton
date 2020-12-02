#include "SwitchButton.h"
#include "SwitchButtonPlugin.h"

#include <QtPlugin>

SwitchButtonPlugin::SwitchButtonPlugin(QObject *parent) : QObject(parent)
{
    initialized = false;
}

QString SwitchButtonPlugin::name() const
{
    return QLatin1String("SwitchButton");
}

QString SwitchButtonPlugin::group() const
{
    return QLatin1String("Buttons");
}

QString SwitchButtonPlugin::toolTip() const
{
    return QLatin1String("Switch button");
}

QString SwitchButtonPlugin::whatsThis() const
{
    return QLatin1String("Switch button");
}

QString SwitchButtonPlugin::includeFile() const
{
    return QLatin1String("SwitchButton.h");
}

QIcon SwitchButtonPlugin::icon() const
{
    return QIcon(":/SwitchButton/designericon.png");
}

QWidget *SwitchButtonPlugin::createWidget(QWidget *parent)
{
    return new SwitchButton(parent);
}

bool SwitchButtonPlugin::isInitialized() const
{
    return initialized;
}

void SwitchButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized) return;

    // Add extension registrations, etc. here

    initialized = true;
}

bool SwitchButtonPlugin::isContainer() const
{
    return false;
}

QString SwitchButtonPlugin::domXml() const
{
    return QLatin1String("<widget class=\"SwitchButton\" name=\"switchButton\">\n</widget>\n");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(switchbuttonplugin, SwitchButtonPlugin)
#endif
