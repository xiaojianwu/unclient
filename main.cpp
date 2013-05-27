#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QSystemTrayIcon>
#include <QDebug>
#include "dialog.h"
#include "singleappdialog.h"
#include "config.h"
#include "settings.h"
#include "sara_service.h"
#include "version.h"
#include "status.h"
#include "usermessages.h"

int printHelp()
{
    QString appName = QString("%1 %2 %3").arg(SARA_COMPANY_STR).arg(SARA_APPLICATION_STR).arg(SARA_CLIENT_VERSION);

    QString message = QString("Command Line Parameters: \n\n%1 <options> mode").arg(QFileInfo(qApp->arguments().at(0)).fileName())
            + "\n\n"
            + "Mode:\n\n"
            + "  check          \tchecks for update\n"
            + "  update         \truns update mode only\n"
            + "  messages       \truns message mode only\n"
            + "  manager        \truns update & message mode\n"
            + "\n\n"
            + "Options:\n\n"
            + "  -k <key>       \tUnique Sara key\n"
            + "  -t <key>       \tUnique Sara test key\n"
            + "  -vc <code>     \tProduct Version Code\n"
            + "  -pc <code>     \tProduct Code\n"
            + "  -v <version>   \tProduct Version\n"
            + "  -i <file>      \tMain Icon\n"
            + "  -l <lang-code> \tLanguage Code\n"
            + "  -s             \tSilent check only\n"
            + "  -q             \tDo not show any question dialog before\n"
            + "  -d             \tDialog when updates available\n"
            + "  -st            \tSystem Tray Icon\n"
            + "  -l <lang-code> \tLanguage Code\n";

    QMessageBox::information(NULL, appName, message);
    return 1;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sara::Config* config = Sara::Config::Instance();
    Sara::Service* service = new Sara::Service(0);

    QString mode;
    QString argument;
    QStringList arguments = QCoreApplication::arguments();
    for (int i = 0; i < arguments.size(); ++i)
    {
        argument = arguments.at(i);
        
        if(argument == "-k")
            config->setKey(arguments.at(i+1));
        else if(argument == "-t")
            config->setTestKey(arguments.at(i+1));
        else if(argument == "-vc")
        {
            config->setSingleMode(true);
            config->setVersionCode(arguments.at(i+1));
        }
        else if(argument == "-pc")
        {
            config->setSingleMode(true);
            config->setProductCode(arguments.at(i+1));
        }
        else if(argument == "-v")
        {
            config->setSingleMode(true);
            config->setVersion(arguments.at(i+1));
        }
        else if(argument == "-s")
            config->setSilent(TRUE);
        else if(argument == "-st")
            config->setSystemTray(true);
        else if(argument == "-i")
            config->setMainIcon(arguments.at(i+1));
        else if(argument == "-n")
            config->setUpdateInterval(arguments.at(i+1).toInt());
        else if(arguments.at(i) == "-l")
            config->setLanguage(arguments.at(i+1));
        else if(arguments.at(i) == "-h" || arguments.at(i) == "--h" || arguments.at(i) == "--help")
            return printHelp();
        else if(argument == "updates" || argument == "messages")
            mode = argument;
    }

    if(config->getKey().isEmpty())
        return SARA_PROCERROR_WRONG_PARAMETER;
    else if(!config->getVersion().isEmpty() && config->getProductCode().isEmpty())
        return SARA_PROCERROR_WRONG_PARAMETER;
    else if(config->getVersion().isEmpty() && !config->getProductCode().isEmpty())
        return SARA_PROCERROR_WRONG_PARAMETER;

    UserMessages messageDialog;
    SingleAppDialog singleDialog;
    Dialog manageDialog;

    if(!config->isSilent())
    {
        if(config->isSingleMode())
        {
            if(mode == "updates")
            {
                singleDialog.init(service);
                singleDialog.hide();
            }
            else if(mode == "messages")
            {
                messageDialog.init(service);
                messageDialog.show();
            }

        }
        else
        {
            manageDialog.init(service);
            manageDialog.hide();
        }
    }
    else
        QObject::connect(service, SIGNAL(done()), &a, SLOT(quit()));

    /*
    if(config->isSystemTray())
    {
        QSystemTrayIcon* tray = new QSystemTrayIcon(0);
        tray->setIcon(QIcon(config->mainIcon()));
        tray->show();
        tray->showMessage("Sara Update Client", "Checking for updates...");
    }
    */

    service->checkForUpdates();

    int result = a.exec();

    if(!config->isSilent())
        return result;
    else
        return service->returnCode();
}
