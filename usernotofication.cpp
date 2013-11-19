/****************************************************************************
**
** Copyright (C) 2013 UpdatNode UG.
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

#include "usernotofication.h"
#include "ui_usernotofication.h"
#include "config.h"
#include "settings.h"

#include <QDesktopServices>

Q_DECLARE_METATYPE ( UpdateNode::Update )
Q_DECLARE_METATYPE ( UpdateNode::Message)

UserNotofication::UserNotofication(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::UserNotofication)
{
    ui->setupUi(this);

    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView, SIGNAL(linkClicked(const QUrl&)), SLOT(openLink(const QUrl&)));

    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), SLOT(updateSelectedUpdate()));
    connect(ui->pshDetails, SIGNAL(clicked()), SLOT(detailsClicked()));

    ui->treeWidget->hide();
    ui->webView->hide();

    ui->labelLogo->hide();

    adjustSize();
}

UserNotofication::~UserNotofication()
{
    delete ui;
}

void UserNotofication::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UserNotofication::updateView()
{
    UpdateNode::Config* config = UpdateNode::Config::Instance();

    setWindowTitle(config->product().getName() + tr(" - Update Client"));

    if(!config->mainIcon().isEmpty())
        setWindowIcon(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
    else if(!config->product().getIconUrl().isEmpty())
        setWindowIcon(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));

    qApp->setWindowIcon(windowIcon());

    if(!config->product().getIconUrl().isEmpty())
    {
        ui->labelLogo->setPixmap(QPixmap(config->product().getLocalIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        ui->labelLogo->show();
    }
    else if(!config->mainIcon().isEmpty())
    {
        ui->labelLogo->setPixmap(QPixmap(config->mainIcon()).scaledToHeight(64, Qt::SmoothTransformation));
        ui->labelLogo->show();
    }

    if(config->updates().size()>0)
    {
        QList<UpdateNode::Update> update_list = config->updates();
        QTreeWidgetItem* parent = new QTreeWidgetItem(ui->treeWidget);
        parent->setData(0, Qt::UserRole, QVariant::fromValue(update_list.at(0)));
        parent->setText(0, update_list.at(0).getTitle());
        parent->setText(1, update_list.at(0).getTargetVersion().getVersion());
        parent->setText(2, update_list.at(0).getFileSize());
        parent->setSelected(true);
    }

    ui->treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->resizeColumnToContents(1);

    adjustSize();
}

void UserNotofication::openLink(const QUrl& aUrl)
{
    QDesktopServices::openUrl(aUrl);
}

void UserNotofication::updateSelectedUpdate()
{
    if(ui->treeWidget->selectedItems().size()>0)
    {
        QTreeWidgetItem* item = ui->treeWidget->selectedItems().at(0);

        UpdateNode::Update update = item->data(0, Qt::UserRole).value<UpdateNode::Update>();
        ui->webView->setHtml(update.getDescription());
    }
}

void UserNotofication::detailsClicked()
{
    if(ui->treeWidget->isVisible())
    {
        ui->treeWidget->hide();
        ui->webView->setHidden(true);
        ui->pshDetails->setText("Show Details");
        adjustSize();
    }
    else
    {
        ui->treeWidget->show();
        ui->webView->show();
        ui->pshDetails->setText("Hide Details");
    }
    layout()->setSizeConstraint(QLayout::SetMinimumSize);

}
