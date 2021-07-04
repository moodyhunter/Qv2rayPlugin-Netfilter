#pragma once

#include "QvGUIPluginInterface.hpp"
#include "QvPlugin/PluginInterface.hpp"
#include "core/Settings.hpp"

#include <QObject>
#include <QtPlugin>

using namespace Qv2rayPlugin;

class NetfilterPlugin
    : public QObject
    , Qv2rayInterface
{
    Q_INTERFACES(Qv2rayPlugin::Qv2rayInterface)
    Q_PLUGIN_METADATA(IID Qv2rayInterface_IID)
    Q_OBJECT
    friend class NetfilterPluginEventHandler;

  public:
    const QvPluginMetadata GetMetadata() const override
    {
        return { "Windows Transparent Proxy",                               //
                 "Community",                                               //
                 PluginId{ "qvplugin_winnetfilter" },                       //
                 "Transparent Proxy Plugin on Windows, using NetfilterSDK", //
                 "",                                                        //
                 {
                     Qv2rayPlugin::COMPONENT_EVENT_HANDLER, //
                     Qv2rayPlugin::COMPONENT_GUI            //
                 } };
    }
    ~NetfilterPlugin(){};
    bool InitializePlugin() override;
    void SettingsUpdated() override{};

  signals:
    void PluginLog(QString) override;
    void PluginErrorMessageBox(QString, QString) override;

  private:
    PluginOptions options;
};
