#include "connectwidget.h"
#include <QHBoxLayout>

namespace dcc {
namespace widgets {

ConnectWidget::ConnectWidget(QWidget *parent) :
    QFrame(parent),
    m_disconnectBtn(new DImageButton),
    m_loading(new LoadingIndicator)
{
    m_connectedBtn = new DImageButton;
    m_connectedBtn->setText(tr("Connected"));

    m_loading->setVisible(false);
    m_loading->setFixedSize(22, 22);

    m_disconnectBtn->setVisible(false);
    m_disconnectBtn->setNormalPic(":/display/themes/dark/icons/disconnect.png");
    m_disconnectBtn->setHoverPic(":/display/themes/dark/icons/disconnect_hover.png");
    m_disconnectBtn->setPressPic(":/display/themes/dark/icons/disconnect_press.png");

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_loading, 0, Qt::AlignRight);
    mainLayout->addWidget(m_disconnectBtn, 0, Qt::AlignRight);
    mainLayout->addWidget(m_connectedBtn, 0, Qt::AlignRight);

    setLayout(mainLayout);

    connect(m_disconnectBtn, &DImageButton::clicked, this, &ConnectWidget::clicked);
}

void ConnectWidget::onConnectChanged(ConnectState state)
{
    m_state = state;

    switch (state) {
    case Connected:
        m_loading->stop();
        m_loading->setVisible(false);
        m_disconnectBtn->setVisible(false);
        m_connectedBtn->setVisible(true);
        break;
    case ConnectFaild:
        m_loading->stop();
        m_loading->setVisible(false);
        m_disconnectBtn->setVisible(false);
        m_connectedBtn->setVisible(false);
        break;
    case Connecting:
        m_loading->play();
        m_loading->setVisible(true);
        m_disconnectBtn->setVisible(false);
        m_connectedBtn->setVisible(false);
        break;
    default:
        break;
    }
}

void ConnectWidget::setMouseEnter(const bool state)
{
    if (m_state == Connected) {
        if (state) {
            m_disconnectBtn->setVisible(true);
            m_connectedBtn->setVisible(false);
        } else {
            m_disconnectBtn->setVisible(false);
            m_connectedBtn->setVisible(true);
        }
    }
}

}
}
