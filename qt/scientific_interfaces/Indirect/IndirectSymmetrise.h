// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2013 ISIS Rutherford Appleton Laboratory UKRI,
//     NScD Oak Ridge National Laboratory, European Spallation Source
//     & Institut Laue - Langevin
// SPDX - License - Identifier: GPL - 3.0 +
#ifndef MANTIDQTCUSTOMINTERFACES_INDIRECTSYMMETRISE_H_
#define MANTIDQTCUSTOMINTERFACES_INDIRECTSYMMETRISE_H_

#include "IndirectDataManipulationTab.h"

#include "MantidAPI/MatrixWorkspace_fwd.h"
#include "MantidKernel/System.h"
#include "ui_IndirectSymmetrise.h"

// Suppress a warning coming out of code that isn't ours
#if defined(__INTEL_COMPILER)
#pragma warning disable 1125
#elif defined(__GNUC__)
#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
#include <MantidQtWidgets/Common/QtPropertyBrowser/QtCheckBoxFactory>
#if defined(__INTEL_COMPILER)
#pragma warning enable 1125
#elif defined(__GNUC__)
#if (__GNUC__ >= 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif
#endif

namespace MantidQt {
namespace CustomInterfaces {
namespace IDA {

class DLLExport IndirectSymmetrise : public IndirectDataManipulationTab {
  Q_OBJECT

public:
  IndirectSymmetrise(QWidget *parent = nullptr);
  ~IndirectSymmetrise() override;

  void setup() override;
  void run() override;
  bool validate() override;

private slots:
  void algorithmComplete(bool error);
  void plotRawInput(const QString &workspaceName);
  void updateMiniPlots();
  void replotNewSpectrum(QtProperty *prop, double value);
  void verifyERange(QtProperty *prop, double value);
  void updateRangeSelectors(QtProperty *prop, double value);
  void preview();
  void previewAlgDone(bool error);
  void xRangeMaxChanged(double value);
  void xRangeMinChanged(double value);

  void runClicked();
  void plotClicked();
  void saveClicked();

private:
  void loadSettings(const QSettings &settings) override{};

  void setRunIsRunning(bool running);
  void setPlotIsPlotting(bool plotting);
  void setButtonsEnabled(bool enabled);
  void setRunEnabled(bool enabled);
  void setPlotEnabled(bool enabled);
  void setSaveEnabled(bool enabled);

  Ui::IndirectSymmetrise m_uiForm;
};

} // namespace IDA
} // namespace CustomInterfaces
} // namespace MantidQt

#endif // MANTIDQTCUSTOMINTERFACES_INDIRECTSYMMETRISE_H_
