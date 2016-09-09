#include "MantidQtMantidWidgets/WorkspacePresenter/WorkspacePresenter.h"
#include "MantidKernel/make_unique.h"
#include "MantidQtMantidWidgets/WorkspacePresenter/ADSAdapter.h"
#include "MantidQtMantidWidgets/WorkspacePresenter/IWorkspaceDockView.h"

using namespace Mantid;

namespace MantidQt {
namespace MantidWidgets {

WorkspacePresenter::WorkspacePresenter(DockView_wptr view)
    : m_view(view), m_adapter(Kernel::make_unique<ADSAdapter>()) {}

WorkspacePresenter::~WorkspacePresenter() {}

void WorkspacePresenter::init() {
  m_adapter->registerPresenter(std::move(m_view.lock()->getPresenterWeakPtr()));
}

void WorkspacePresenter::notifyFromWorkspaceProvider(
    WorkspaceProviderNotifiable::Flag flag) {
  switch (flag) {
  case WorkspaceProviderNotifiable::Flag::WorkspaceLoaded:
    workspaceLoaded();
    break;
  case WorkspaceProviderNotifiable::Flag::WorkspaceRenamed:
    workspaceRenamed();
    break;
  case WorkspaceProviderNotifiable::Flag::WorkspacesSaved:
    workspaceSaved();
    break;
  case WorkspaceProviderNotifiable::Flag::WorkspacesGrouped:
    workspacesGrouped();
    break;
  case WorkspaceProviderNotifiable::Flag::WorkspacesSorted:
    workspacesSorted();
    break;
  case WorkspaceProviderNotifiable::Flag::WorkspaceDeleted:
    workspacesDeleted();
    break;
  }
}

void WorkspacePresenter::notifyFromView(ViewNotifiable::Flag flag) {
  switch (flag) {
  case ViewNotifiable::Flag::LoadWorkspace:
    loadWorkspace();
    break;
  case ViewNotifiable::Flag::RenameWorkspace:
    renameWorkspace();
    break;
  case ViewNotifiable::Flag::GroupWorkspaces:
    groupWorkspaces();
    break;
  case ViewNotifiable::Flag::SaveWorkspaces:
    saveWorkspace();
    break;
  case ViewNotifiable::Flag::SortWorkspaces:
    sortWorkspaces();
    break;
  case ViewNotifiable::Flag::DeleteWorkspaces:
    deleteWorkspaces();
    break;
  }
}

void WorkspacePresenter::loadWorkspace() {
  auto view = lockView();
  view->showLoadDialog();
}

void WorkspacePresenter::saveWorkspace() {}

void WorkspacePresenter::renameWorkspace() {
  auto view = lockView();
  auto selected = view->getSelectedWorkspaceNames();
  view->showRenameDialog(selected);
}

void WorkspacePresenter::groupWorkspaces() {}
void WorkspacePresenter::sortWorkspaces() {}

void WorkspacePresenter::deleteWorkspaces() {
  auto view = lockView();

  if (view->deleteConfirmation())
    view->deleteWorkspaces();
}

void WorkspacePresenter::workspaceLoaded() { updateView(); }

void WorkspacePresenter::workspaceSaved() {}

void WorkspacePresenter::workspaceRenamed() { updateView(); }

void WorkspacePresenter::workspacesGrouped() {}
void WorkspacePresenter::workspacesSorted() {}

void WorkspacePresenter::workspacesDeleted() { updateView(); }

DockView_sptr WorkspacePresenter::lockView() {
  auto view_sptr = m_view.lock();

  if (view_sptr == nullptr)
    throw std::runtime_error("Could not obtain pointer to DockView.");

  return std::move(view_sptr);
}

void WorkspacePresenter::updateView() {
  auto view = lockView();
  view->updateTree(m_adapter->topLevelItems());
}

} // namespace MantidQt
} // namespace MantidWidgets