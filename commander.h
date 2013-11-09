#ifndef COMMANDER_H
#define COMMANDER_H

#include <QObject>
#include <QProcess>
#include "update.h"

namespace UpdateNode
{
    class Commander : public QObject
    {
        Q_OBJECT
        public:
            Commander(QObject *parent = 0);
            ~Commander();

            bool run(const UpdateNode::Update& aUpdate);

            QString readStdErr() const;
            QString readStdOut() const;

            void setUpdate(const UpdateNode::Update& aUpdate);
            QString resolve(const QString& aString);

            bool waitForFinished();
            int getReturnCode();

        public:
            static QString resolveGeneral(const QString& aString);
            static bool copy(const QString& aFrom, const QString& aTo);
            static QStringList splitCommandLine(const QString& aString);

        signals:
            void processError();
            void processOutput();
            void updateExit(int aExitCode, QProcess::ExitStatus aExitStatus);
            void progressText(const QString& aStatusText);

        private:
            QString setCommandBasedOnOS() const;


        private:
            QProcess* m_pProcess;
            UpdateNode::Update m_oUpdate;
            bool m_bCopy;
    };
}

#endif // COMMANDER_H
