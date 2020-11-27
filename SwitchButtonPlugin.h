#pragma once

#include <QtUiPlugin/QDesignerCustomWidgetInterface>

class SwitchButtonPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QDesignerCustomWidgetInterface")
#endif

public:
    SwitchButtonPlugin(QObject *parent = 0);

    QString name() const;
    QString group() const;
    QString toolTip() const;
    QString whatsThis() const;
    QString includeFile() const;
    QIcon icon() const;
    QWidget *createWidget(QWidget *parent);
    bool isContainer() const;
    bool isInitialized() const;
    void initialize(QDesignerFormEditorInterface *core);
    QString domXml() const;

private:
    bool initialized;
};
