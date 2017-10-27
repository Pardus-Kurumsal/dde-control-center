#include "defaultappsmodule.h"
#include "defcategorywidget.h"
#include "switchwidget.h"
#include "defappwidget.h"
#include "defappviewer.h"
#include "defappmodel.h"
#include "defappworker.h"

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QDebug>
using namespace dcc;
using namespace dcc::defapp;
DefaultAppsModule::DefaultAppsModule(FrameProxyInterface *frame, QObject *parent)
    : QObject(parent),
      ModuleInterface(frame),
      m_defaultappsWidget(nullptr),
      m_defaultAppsDetail(nullptr)
{

}

DefaultAppsModule::~DefaultAppsModule()
{
    m_defAppModel->deleteLater();
    m_defAppWorker->deleteLater();
}

void DefaultAppsModule::initialize()
{
    m_defAppModel  = new DefAppModel;
    m_defAppWorker = new DefAppWorker(m_defAppModel);
    m_defAppModel->moveToThread(qApp->thread());
    m_defAppWorker->moveToThread(qApp->thread());

}

void DefaultAppsModule::moduleActive()
{
    m_defAppWorker->active();
}

//模块非活动时禁用dbus
void DefaultAppsModule::moduleDeactive()
{
    m_defAppWorker->deactive();
}

void DefaultAppsModule::reset()
{
    emit requestReset();
}

ModuleWidget *DefaultAppsModule::moduleWidget()
{
    if (!m_defaultappsWidget) {
        m_defaultappsWidget = new DefaultAppsWidget;
        connect(m_defaultappsWidget, &DefaultAppsWidget::showDefaultAppsDetail, this, &DefaultAppsModule::showDefaultAppsDetail);
    }
    return m_defaultappsWidget;
}

const QString DefaultAppsModule::name() const
{
    return QStringLiteral("defapp");
}

//show下级菜单
void DefaultAppsModule::showDefaultAppsDetail()
{
    if (!m_defaultAppsDetail) {
        m_defAppWorker->onGetListApps();
        m_defAppWorker->onGetDefaultApp();
        m_defaultAppsDetail = new DefAppViewer();
        m_defaultAppsDetail->setModel(m_defAppModel);
        connect(this, &DefaultAppsModule::requestReset,              m_defAppWorker, &DefAppWorker::onResetTriggered);  //恢复默认
        connect(m_defaultAppsDetail, &DefAppViewer::requestSetDefaultApp,   m_defAppWorker, &DefAppWorker::onSetDefaultApp); //设置默认程序
        connect(m_defaultAppsDetail, &DefAppViewer::requestDelUserApp,      m_defAppWorker, &DefAppWorker::onDelUserApp);
        connect(m_defaultAppsDetail, &DefAppViewer::requestFrameAutoHide, this, &DefaultAppsModule::setFrameAutoHide);
        connect(m_defaultAppsDetail, &DefAppViewer::requestCreateFile, m_defAppWorker, &DefAppWorker::onCreateFile);
    }
    m_frameProxy->pushWidget(this, m_defaultAppsDetail);
}

void DefaultAppsModule::contentPopped(ContentWidget *const w)
{
    if (w == m_defaultAppsDetail) {
        m_defaultAppsDetail = nullptr;
    }

    w->deleteLater();
}

void DefaultAppsModule::setFrameAutoHide(const bool autoHide)
{
    m_frameProxy->setFrameAutoHide(this, autoHide);
}

