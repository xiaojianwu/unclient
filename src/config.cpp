/****************************************************************************
**
** Copyright (C) 2014 UpdateNode UG (haftungsbeschränkt)
** Contact: code@updatenode.com
**
** This file is part of the UpdateNode Client.
**
** Commercial License Usage
** Licensees holding valid commercial UpdateNode license may use this file
** under the terms of the the Apache License, Version 2.0
** Full license description file: LICENSE.COM
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation. Please review the following information to ensure the
** GNU General Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
** Full license description file: LICENSE.GPL
**
****************************************************************************/

#include <QLocale>
#include <QSettings>
#include <QCryptographicHash>

#include "config.h"
#include "updatenode_service.h"
#include "binarysettings.h"

using namespace UpdateNode;

#define DEFAULT_TIMEOUT 20

/*!
\class UpdateNode::Config
\brief Main config class which holds all informations about the current requested
product, its version and all information required to retreive and display updates
and messages
*/

/*!
Global instance of the config data - used for program confiuration and single product
updates and messages
*/
Config* Config::m_pInstance = NULL;

/*!
Retrieves the global Config class instance as an pointer. If no instance is present,
a new one will be created in this method
*/
Config* Config::Instance()
{
   if(!m_pInstance)
       m_pInstance = new Config;

   return m_pInstance;
}

/*!
Constructs a Config object
*/
Config::Config()
{
    m_bSilent = false;
    m_bSytemTray = false;
    m_bSingleMode = false;
    m_bRelaunch = false;
    m_bEnforeMessages = false;
    m_iTimeOut = DEFAULT_TIMEOUT;
}

/*!
Sets the API key which has been assigned in UpdateNode.com
\sa Config::getKey
*/
void Config::setKey(const QString& aKey)
{
    m_strKey = aKey;
}

/*!
Returns the API key which has been assigned in UpdateNode.com
\sa Config::setKey
*/
QString Config::getKey() const
{
    return m_strKey;
}

/*!
Returns a MD5 hashed representation of the API key previously set by Config::setkey
\sa Config::setKey
*/
QString Config::getKeyHashed() const
{
    return QString(QCryptographicHash::hash(getKey().toLatin1(), QCryptographicHash::Md5).toHex());
}

/*!
Sets the Test key which has been assigned in UpdateNode.com
\sa Config::getTestKey
*/
void Config::setTestKey(const QString& aTestKey)
{
    m_strTestKey = aTestKey;
}

/*!
Returns the Test key which has been assigned in UpdateNode.com
\sa Config::setTestKey
*/
QString Config::getTestKey() const
{
    return m_strTestKey;
}

/*!
Sets the version code
\sa Config::getVersionCode
*/
void Config::setVersionCode(const QString& aVersionCode)
{
    m_strVersionCode = aVersionCode;
}

/*!
Returns the version code
\sa Config::setVersionCode
*/
QString Config::getVersionCode() const
{
    return m_strVersionCode;
}

/*!
Sets the product code
\sa Config::getProductCode
*/
void Config::setProductCode(const QString& aProductCode)
{
    m_strProductCode = aProductCode;
}

/*!
Returns the product code
\sa Config::setProductCode
*/
QString Config::getProductCode() const
{
    return m_strProductCode;
}

/*!
Sets the product's version as a string
\sa Config::getVersion
*/
void Config::setVersion(const QString& aVersion)
{
    m_strVersion = aVersion;
}

/*!
Returns the product's version as a string
\sa Config::setVersion
*/
QString Config::getVersion() const
{
    return m_strVersion;
}

/*!
Sets the silent mode
\sa Config::isSilent
*/
void Config::setSilent(bool aSilent)
{
    m_bSilent = aSilent;
}

/*!
Returns true if silent mode has been enabled
\sa Config::setSilent
*/
bool Config::isSilent()
{
    return m_bSilent;
}

/*!
Returns the current OS as String
\note Possible values are: Windows, Linux, or Mac
*/
QString Config::getOS() const
{
#ifdef Q_OS_UNIX
    return "Linux";
#else
#ifdef Q_OS_WIN
    return "Windows";
#else
    return "Mac";
#endif
#endif
}

/*!
Returns the Product object
\sa Config::setProduct
*/
UpdateNode::Product Config::product()
{
    return m_oProduct;
}

/*!
Returns the ProductVersion object
\sa Config::setVersion
*/
UpdateNode::ProductVersion Config::version()
{
    return m_oCurrentVersion;
}

/*!
Returns the update object's in a QList
\sa Config::addUpdate
*/
QList<UpdateNode::Update> Config::updates()
{
    return m_listUpdates;
}

/*!
Returns the message object's in a QList
\sa Config::addMessage
*/
QList<UpdateNode::Message> Config::messages()
{
    return m_listMessages;
}

/*!
Adds a new update into the updates list
\sa Config::updates
*/
void Config::addUpdate(const UpdateNode::Update& aUpdate)
{
    m_listUpdates.append(aUpdate);
}

/*!
Adds a new message into the message list
\sa Config::messages
*/
void Config::addMessage(const UpdateNode::Message& aMessage)
{
    m_listMessages.append(aMessage);
}

/*!
Sets the Product object
\sa Config::product
*/
void Config::setProduct(const Product& aProduct)
{
    m_oProduct = aProduct;
}

/*!
Sets the ProductVersion object
\sa Config::version
*/
void Config::setVersion(const ProductVersion& aVersion)
{
    m_oCurrentVersion = aVersion;
}

/*!
Returns the main icon
\sa Config::setMainIcon
*/
QString Config::mainIcon() const
{
    return m_strMainIcon;
}

/*!
Sets the main icon
\sa Config::mainIcon
*/
void Config::setMainIcon(const QString& aIcon)
{
    m_strMainIcon = aIcon;
}

/*!
Sets the langugage. Language needs to be at least two characted language code.
\sa Config::getLanguage
*/
void Config::setLanguage(const QString& aLanguage)
{
    m_strLanguage = aLanguage;
}

/*!
Returns the set langugage, or the systems language if Config::setLanguage was not called before.
\sa Config::setLanguage
*/
QString Config::getLanguage() const
{
    return m_strLanguage.isEmpty() ? QLocale::system().name() : m_strLanguage;
}

/*!
Clears the update, message and configurations list
*/
void Config::clear()
{
    m_listUpdates.clear();
    m_listMessages.clear();
    m_listConfigs.clear();
}

/*!
Enables systems tray icon
\sa Config::isSystemTray
*/
void Config::setSystemTray(bool aTray)
{
    m_bSytemTray = aTray;
}

/*!
Returns true if system tray has been activated
\sa Config::setSystemTray
*/
bool Config::isSystemTray()
{
    return m_bSytemTray;
}

/*!
Enabled the single mode. The single mode indicates that the update manager should be only
operating on on product.
\sa Config::isSingleMode
*/
void Config::setSingleMode(bool aSingleMode)
{
    m_bSingleMode = aSingleMode;
}

/*!
Returns true if singel mode has been activated
\sa Config::setSingleMode
*/
bool Config::isSingleMode()
{
    return m_bSingleMode;
}

/*!
Sets the UpdateNode host
\note this method is called by UpdateNode team internal to test new features in an separated environment
\sa Config::getHost
*/
void Config::setHost(const QString& aHost)
{
    m_strHost = aHost;
}

/*!
Returns the host
\sa Config::setHost
*/
QString Config::getHost() const
{
    return m_strHost;
}

/*!
Adds a configuration to the list of configurations. This is used for the multi-products mode
\sa Config::configurations
*/
void Config::addConfiguration(UpdateNode::Config* aConfig)
{
    m_listConfigs.append(aConfig);
}

/*!
Sets the langugage. Language needs to be at least two characted language code.
\sa Config::addConfiguration
*/
QList<UpdateNode::Config*> Config::configurations()
{
    return m_listConfigs;
}

/*!
Clears the list of configurations
\sa Config::clear
*/
void Config::clearConfigurations()
{
    m_listConfigs.clear();
}

/*!
Sets the logging file name
\sa Config::isLoggingEnabled
\sa Config::getLoggingFile
*/
void Config::setLogging(const QString &aFileName)
{
    m_strLogging = aFileName;
}

/*!
Checks if logging has been enabled or not. Logging can be disabled calling
Config::setLogging with an empty string as parameter
\sa Config::getLoggingFile
\sa Config::setLoggingFile
*/
bool Config::isLoggingEnabled()
{
    return !m_strLogging.isEmpty();
}

/*!
Returns the logging file name
\sa Config::isLoggingEnabled
\sa Config::setLoggingFile
*/
QString Config::getLoggingFile()
{
    return m_strLogging;
}

/*!
Sets executable and its parameters which should be executed once check/installation
has been performed
\sa Config::getExec
*/
void Config::setExec(const QString& aFileName)
{
    m_strExec = aFileName;
}

/*!
Sets the executable
\sa Config::setExec
*/
QString Config::getExec()
{
    return m_strExec;
}

/*!
Using the setRelaunch method, you are able to re-spawn the current unclient
in a save evniroment. Means, to some place from which you can overwrite the original binary.
This mode is mostly useful when you are running on Windows.
\sa Config::isRelaunch
*/
void Config::setRelaunch(bool aEnable)
{
    m_bRelaunch = aEnable;
}

/*!
Returns true if relaunch is enabled
\sa Config::setRelaunch
*/
bool Config::isRelaunch()
{
    return m_bRelaunch;
}

/*!
Sets the splash sceen.
\sa Config::getSplashScreen
*/
void Config::setSplashScreen(const QString& aFileName)
{
    m_strSplashImage = aFileName;
}

/*!
Returns the splash screen
\sa Config::setSplashScreen
*/
QString Config::getSplashScreen()
{
    return m_strSplashImage;
}

/*!
Sets the style sheet file
\sa Config::getStyleSheet
*/
void Config::setStyleSheet(const QString& aFileName)
{
    m_strStyleSheet = aFileName;
}

/*!
Returns the style sheet
\sa Config::setStyleSheet
*/
QString Config::getStyleSheet()
{
    return m_strStyleSheet;
}

/*!
Sets the time out for accessing data on UpdateNode
\sa Config::getTimeOut
*/
void Config::setTimeOut(int aTimeOutInSeconds)
{
    m_iTimeOut = aTimeOutInSeconds;
}

/*!
Returns the time out
\sa Config::setTimeOut
*/
int Config::getTimeOut()
{
    return m_iTimeOut;
}

/*!
Checks if messages are enforced even not running in -messages mode
\sa Config::setEnforceMessages
*/
bool Config::isEnforceMessages()
{
    return m_bEnforeMessages;
}

/*!
Sets messages enforcement mode
\sa Config::isEnforeMessages
*/
void Config::setEnforceMessages(bool aEnforceMessages)
{
    m_bEnforeMessages = aEnforceMessages;
}

/*!
Sets a custom request value
\sa Config::getCustomRequestValue
*/
void Config::setCustomRequestValue(const QString& aValue)
{
    m_strCustomRequestValue = aValue;
}

/*!
Returns the custom request value
\sa Config::setCustomRequestValue
*/
QString Config::getCustomRequestValue()
{
    return m_strCustomRequestValue;
}

/*!
Sets a custom value to identify the customer
\sa Config::getIdentifier
*/
void Config::setIdentifier(const QString& aIdent)
{
    m_strIdentifier = aIdent;
}

/*!
Returns the custom client identify
\sa Config::getIdentifier
*/
QString Config::getIdentifier()
{
    return m_strIdentifier;
}

/*!
Reads commandline parameters from config file
\sa Config::setParametersToFile
*/
void Config::getParametersFromFile(const QString& aFile)
{
    QSettings *settings = NULL;

    if(QFileInfo(aFile).suffix()!="cfg")
    {
        const QSettings::Format BinFormat =
                 QSettings::registerFormat("bin",
                                           UpdateNode::BinarySettings::readBinFile,
                                           UpdateNode::BinarySettings::writeBinFile);
        settings = new QSettings(aFile, BinFormat);
    }
    else
        settings = new QSettings(aFile, QSettings::IniFormat);

    if(settings->contains("key"))
        setKey(settings->value("key").toString());
    if(settings->contains("test_key"))
        setTestKey(settings->value("test_key").toString());
    if(settings->contains("version_code"))
    {
        setSingleMode(true);
        setVersionCode(settings->value("version_code").toString());
    }
    if(settings->contains("product_code"))
    {
        setSingleMode(true);
        setProductCode(settings->value("product_code").toString());
    }
    if(settings->contains("version"))
    {
        setSingleMode(true);
        setVersion(settings->value("version").toString());
    }
    if(settings->contains("icon"))
        setMainIcon(settings->value("icon").toString());
    if(settings->contains("language"))
        setLanguage(settings->value("language").toString());
    if(settings->contains("silent"))
        setSilent(settings->value("silent").toString().toLower()=="true");
    if(settings->contains("http") && settings->value("http").toString().toLower()=="true")
        setHost(QString(UPDATENODE_SERVICE_URL).replace("https", "http"));
    if(settings->contains("systemtray"))
        setSystemTray(settings->value("systemtray").toString().toLower()=="true");
    if(settings->contains("relaunch"))
        setRelaunch(settings->value("relaunch").toString().toLower()=="true");
    if(settings->contains("enforce_messages"))
        setEnforceMessages(settings->value("enforce_messages").toString().toLower()=="true");
    if(settings->contains("log"))
        setLogging(settings->value("log").toString());
    if(settings->contains("exec_command"))
        setExec(settings->value("exec_command").toString());
    if(settings->contains("splash"))
        setSplashScreen(settings->value("splash").toString());
    if(settings->contains("stylesheet"))
        setStyleSheet(settings->value("stylesheet").toString());
    if(settings->contains("timeout"))
        setTimeOut(settings->value("timeout").toInt());
    if(settings->contains("custom"))
        setCustomRequestValue(settings->value("custom").toString());
    if(settings->contains("identifier"))
        setIdentifier(settings->value("identifier").toString());

    delete settings;
}

/*!
Writes current commandline parameters to a config file \n
Parameter aAll ignores some values when set to false
\sa Config::getParametersFromFile
*/
void Config::setParametersToFile(const QString& aFile, bool aAll /* = true */)
{
    QFile::remove(aFile);

    QSettings *settings = NULL;

    if(QFileInfo(aFile).suffix()!="cfg")
    {
        const QSettings::Format BinFormat =
                 QSettings::registerFormat("bin",
                                           UpdateNode::BinarySettings::readBinFile,
                                           UpdateNode::BinarySettings::writeBinFile);
        settings = new QSettings(aFile, BinFormat);
    }
    else
        settings = new QSettings(aFile, QSettings::IniFormat);

    settings->setValue("key", getKey());
    if(!getTestKey().isEmpty())
        settings->setValue("test_key", getTestKey());
    if(!getProductCode().isEmpty())
        settings->setValue("product_code", getProductCode());
    if(!getVersion().isEmpty())
        settings->setValue("version", getVersion());
    if(!getVersionCode().isEmpty())
        settings->setValue("version_code", getVersionCode());
    if(!getCustomRequestValue().isEmpty())
        settings->setValue("custom", getCustomRequestValue());
    if(!getExec().isEmpty() && aAll)
        settings->setValue("exec_command", getExec());
    if(!getIdentifier().isEmpty())
        settings->setValue("identifier", getIdentifier());
    if(!getHost().isEmpty())
        settings->setValue("http", "true");
    if(!m_strLanguage.isEmpty())
        settings->setValue("language", getLanguage());
    if(!getLoggingFile().isEmpty())
        settings->setValue("log", getLoggingFile());
    if(!getSplashScreen().isEmpty() && aAll)
        settings->setValue("splash", getSplashScreen());
    if(!getStyleSheet().isEmpty())
        settings->setValue("stylesheet", getStyleSheet());
    if(getTimeOut()!=DEFAULT_TIMEOUT)
        settings->setValue("timeout", getTimeOut());
    if(!mainIcon().isEmpty())
        settings->setValue("icon", mainIcon());
    if(isEnforceMessages() && aAll)
        settings->setValue("enforce_messages", "true");
    if(isRelaunch() && aAll)
        settings->setValue("relaunch", "true");
    if(isSilent() && aAll)
        settings->setValue("silent", "true");
    if(isSystemTray() && aAll)
        settings->setValue("systemtray", "true");

    delete settings;
}




