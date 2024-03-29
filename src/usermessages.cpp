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

#include "usermessages.h"
#include "config.h"
#include "settings.h"
#include "localfile.h"

#ifdef QT_WEBKIT_LIB
#include "qglobal.h"
#if QT_VERSION >= 0x050000
#include <QtWebKitWidgets>
#else
#include <QtWebKit>
#endif
#include "ui_usermessages_ex.h"
#else
#include <QTextBrowser>
#include "ui_usermessages.h"
#endif

#include <QMessageBox>
#include <QNetworkDiskCache>
#include <QDesktopServices>

/*!
\class UserMessages
\brief Message dialog for showing messages defined on UpdateNode
*/

UserMessages::UserMessages(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
#ifdef QT_WEBKIT_LIB
    ui(new Ui::UserMessagesEx)
#else
    ui(new Ui::UserMessages)
#endif
{
    ui->setupUi(this);

	m_bFromRight = false;

    QNetworkDiskCache* cache = new QNetworkDiskCache(this);
    cache->setCacheDirectory(UpdateNode::LocalFile::getCachePath());
#ifdef QT_WEBKIT_LIB
    ui->webView->page()->networkAccessManager()->setCache(cache);
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));
#else
    connect(ui->textBrowser, SIGNAL(anchorClicked(QUrl)), SLOT(openLink(QUrl)));
#endif
    connect(ui->toolLeft, SIGNAL(clicked()), SLOT(onLeft()));
    connect(ui->toolRight, SIGNAL(clicked()), SLOT(onRight()));
    connect(ui->pshRead, SIGNAL(clicked()), SLOT(onRead()));

    connect(this, SIGNAL(rejected()), SLOT(onClose()));
    connect(ui->pshClose, SIGNAL(clicked()), SLOT(onClose()));

    ui->progressBar->hide();
    hide();
}

UserMessages::~UserMessages()
{
#ifdef QT_WEBKIT_LIB
    delete ui->webView->page()->networkAccessManager()->cache();
#endif
    delete ui;
}

void UserMessages::init(UpdateNode::Service* aService)
{
    m_pService = aService;
    connect(m_pService, SIGNAL(done()), SLOT(serviceDone()));

#ifdef QT_WEBKIT_LIB
    connect(ui->webView->page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            m_pService, SLOT(onSslError(QNetworkReply*,QList<QSslError>)));
#endif
}

void UserMessages::serviceDone()
{
    UpdateNode::Settings settings;
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    m_iCurrentIndex = -1;
    m_listMessages.clear();
    ui->retranslateUi(this);

    if(!config->product().getIconUrl().isEmpty())
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    else if(!config->mainIcon().isEmpty())
        setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));

	QList<int> externalMessages;
    QList<UpdateNode::Message> message_list= config->messages();
    for(int i = 0; i < message_list.size(); i++)
    {
        if(!settings.messageShownAndLoaded(message_list.at(i).getCode()))
		{
			m_listMessages << message_list.at(i);
			if(message_list.at(i).isOpenExternal())
				externalMessages << m_listMessages.size()-1;
		}
    }

	if(m_listMessages.size() != externalMessages.size())
	{
		foreach(int ext, externalMessages)
			m_listMessages.takeAt(ext);
	}

    if(m_listMessages.isEmpty())
	{
		qApp->exit(UPDATENODE_PROCERROR_NO_UPDATES);
		return;
	}
    else
    {
        if(m_listMessages.size()==1)
            setWindowTitle(config->product().getName() + tr(" - Message"));
        else
            setWindowTitle(config->product().getName() + tr(" - Messages"));
    }
    layout()->setSizeConstraint(QLayout::SetMinimumSize);
    
    showMessage();

	setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
}


void UserMessages::showMessage()
{
    if(m_listMessages.isEmpty())
    {
        qApp->exit(0);
        return;
    }

    if(m_iCurrentIndex == -1)
        m_iCurrentIndex = 0;

    ui->pshRead->setDisabled(true);

    if(m_listMessages.size()>1)
		ui->label->setText(m_listMessages.at(m_iCurrentIndex).getTitle() +  QString(" (%1/%2)").arg(m_iCurrentIndex+1).arg(m_listMessages.size()));
	else
		ui->label->setText(m_listMessages.at(m_iCurrentIndex).getTitle());
	
    if(m_listMessages.at(m_iCurrentIndex).isOpenExternal() 
		&& !m_listMessages.at(m_iCurrentIndex).getLink().isEmpty())
    {
		hide();

        QMessageBox box(this);

        box.setWindowTitle(UpdateNode::Config::Instance()->product().getName() + tr(" - Message"));
		QPushButton* yesButton = box.addButton(tr("Yes"), QMessageBox::ActionRole);
        box.addButton(tr("Not now"), QMessageBox::RejectRole);
        box.setText(tr("There is a new message available:<br><br>"
                       "<b>%1</b><br><br>"
                       "Do you want to read this message in your standard browser now?").arg(m_listMessages.at(m_iCurrentIndex).getTitle()));

		box.exec();

		if(box.clickedButton() == yesButton)
        {
            openLink(QUrl::fromUserInput(m_listMessages.at(m_iCurrentIndex).getLink()));
			UpdateNode::Settings settings;
			UpdateNode::Message message = m_listMessages.takeAt(m_iCurrentIndex);
			settings.setMessage(message, true, true);
        }
		qApp->exit(0);
		return;
    }
    else
    {
#ifdef QT_WEBKIT_LIB
        if(!m_listMessages.at(m_iCurrentIndex).getMessage().isEmpty())
            ui->webView->setHtml(m_listMessages.at(m_iCurrentIndex).getMessage());
        else
            ui->webView->load(m_listMessages.at(m_iCurrentIndex).getLink());
#else
        if(!m_listMessages.at(m_iCurrentIndex).getMessage().isEmpty())
            ui->textBrowser->setHtml(m_listMessages.at(m_iCurrentIndex).getMessage());
        else
            ui->textBrowser->loadHtml(m_listMessages.at(m_iCurrentIndex).getLink());
        ui->pshRead->setEnabled(true);
#endif
    }
    ui->progressBar->setFormat(tr("Loading %p% ..."));

    if(m_listMessages.size()==m_iCurrentIndex+1)
        ui->toolRight->setDisabled(true);
    else
        ui->toolRight->setEnabled(true);

    if(m_iCurrentIndex == 0)
        ui->toolLeft->setDisabled(true);
    else
        ui->toolLeft->setEnabled(true);

    if(m_listMessages.size()==1)
    {
        ui->toolLeft->hide();
        ui->toolRight->hide();
        ui->pshRead->setText(tr("Mark as read and close"));
    }
    show();
}

void UserMessages::onLeft()
{
    if(m_iCurrentIndex>0)
    {
        m_bFromRight = true;
        m_iCurrentIndex--;
        showMessage();
    }
}

void UserMessages::onRight()
{
    if(m_iCurrentIndex < m_listMessages.size()-1)
    {
        m_bFromRight = false;
        m_iCurrentIndex++;
        showMessage();
    }
}

void UserMessages::onRead()
{
    UpdateNode::Settings settings;
    UpdateNode::Message message = m_listMessages.takeAt(m_iCurrentIndex);

    settings.setMessage(message, true, true);

    m_iCurrentIndex = -1;
    showMessage();
}

void UserMessages::openLink(const QUrl& aUrl)
{
    if(aUrl.toString() == "#onRead")
        QTimer::singleShot(0, this, SLOT(onRead()));
    else if(aUrl.toString() == "#onLeft")
        QTimer::singleShot(0, this, SLOT(onLeft()));
    else if(aUrl.toString() == "#onRight")
        QTimer::singleShot(0, this, SLOT(onRight()));
    else if(aUrl.toString() == "#onClose")
        reject();
    else
        QDesktopServices::openUrl(aUrl);
}

void UserMessages::onClose()
{
    qApp->exit(UPDATENODE_PROCERROR_CANCELED);
}

