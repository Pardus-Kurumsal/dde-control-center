/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "removeuserdialog.h"

#include <QUrl>
#include <QPainter>
#include <QPainterPath>
#include <QCheckBox>

#include "avatarwidget.h"

using namespace dcc::accounts;

static QPixmap RoundPixmap(const QPixmap &pix) {
    QPixmap ret(pix.size());
    ret.fill(Qt::transparent);

    QPainter painter(&ret);
    painter.setRenderHints(painter.renderHints() | QPainter::Antialiasing);

    QPainterPath path;
    path.addEllipse(ret.rect());
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, pix);

    painter.end();

    return ret;
}

RemoveUserDialog::RemoveUserDialog(const User *user, QWidget *parent) :
    DDialog(parent),
    m_deleteHome(true)
{
    setTitle(tr("Administrator permission required to delete account"));

    const QString iconFile = QUrl(user->currentAvatar()).toLocalFile();
    const QPixmap pix = QPixmap(iconFile).scaled(48, 48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    setIconPixmap(RoundPixmap(pix));

    QCheckBox *box = new QCheckBox(tr("Delete account directory"));
    box->setChecked(true);
    box->setAccessibleName("Delete_Account_Checkbox");
    addContent(box, Qt::AlignTop);

    QStringList buttons;
    buttons << tr("Cancel") << tr("Delete");
    addButtons(buttons);

    connect(box, &QCheckBox::toggled, [this, box] {
        m_deleteHome = box->checkState() == Qt::Checked;
    });
}

bool RemoveUserDialog::deleteHome() const
{
    return m_deleteHome;
}
