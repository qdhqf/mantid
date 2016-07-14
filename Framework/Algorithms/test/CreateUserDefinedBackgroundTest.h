#ifndef MANTID_ALGORITHMS_CREATEUSERDEFINEDBACKGROUNDTEST_H_
#define MANTID_ALGORITHMS_CREATEUSERDEFINEDBACKGROUNDTEST_H_

#include <cxxtest/TestSuite.h>

#include "MantidAlgorithms/CreateUserDefinedBackground.h"
#include "MantidAPI/AlgorithmFactory.h"
#include "MantidAPI/FrameworkManager.h"
#include "MantidAPI/ITableWorkspace.h"
#include "MantidAPI/MatrixWorkspace.h"
#include "MantidAPI/TableRow.h"
#include "MantidDataObjects/TableWorkspace.h"
#include "MantidTestHelpers/WorkspaceCreationHelper.h"
#include <cmath>

using Mantid::Algorithms::CreateUserDefinedBackground;
using Mantid::API::ITableWorkspace_sptr;
using Mantid::API::MatrixWorkspace_sptr;

namespace {
// Gaussian
double gauss(double x, double height, double centre, double fwhm) {
  const double factor = 2.0 * sqrt(2.0 * log(2.0));
  const double sigma = fwhm / factor;
  return height * exp(-0.5 * (((x - centre) * (x - centre)) / (sigma * sigma)));
}

// Function to generate test background without peaks
double background(double xPoint, int iSpec = 0) {
  UNUSED_ARG(iSpec);
  return gauss(xPoint, 5.0, 0.0, 5.0) + gauss(xPoint, 2.0, 3.0, 2.0);
}

// Function to generate test peaks without background
double peaks(double xPoint) {
  return gauss(xPoint, 1.0, 2.0, 0.1) + gauss(xPoint, 1.0, 4.0, 0.1);
}

// Function to generate test data: a background with some peaks
double dataFunction(double xPoint, int iSpec) {
  return background(xPoint, iSpec) + peaks(xPoint);
}
}

class CreateUserDefinedBackgroundTest : public CxxTest::TestSuite {
public:
  // This pair of boilerplate methods prevent the suite being created statically
  // This means the constructor isn't called when running other tests
  static CreateUserDefinedBackgroundTest *createSuite() {
    return new CreateUserDefinedBackgroundTest();
  }
  static void destroySuite(CreateUserDefinedBackgroundTest *suite) {
    delete suite;
  }

  void test_Init() {
    CreateUserDefinedBackground alg;
    TS_ASSERT_THROWS_NOTHING(alg.initialize())
    TS_ASSERT(alg.isInitialized())
  }

  void test_Properties() {
    CreateUserDefinedBackground alg;
    TS_ASSERT_EQUALS(alg.version(), 1);
    TS_ASSERT_EQUALS(alg.name(), "CreateUserDefinedBackground");
    TS_ASSERT_EQUALS(alg.category(),
                     "CorrectionFunctions\\BackgroundCorrections");
  }

  void test_exec_PointsWS() {
    // Create test input
    const auto inputWS = createTestData(false);
    const auto bgPoints = createTable();

    CreateUserDefinedBackground alg;
    alg.setChild(true);
    TS_ASSERT_THROWS_NOTHING(alg.initialize())
    TS_ASSERT(alg.isInitialized())
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("InputWorkspace", inputWS));
    TS_ASSERT_THROWS_NOTHING(
        alg.setPropertyValue("OutputBackgroundWorkspace", "__NotUsed"));
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("BackgroundPoints", bgPoints))
    TS_ASSERT_THROWS_NOTHING(alg.execute(););
    TS_ASSERT(alg.isExecuted());

    MatrixWorkspace_sptr outputWS =
        alg.getProperty("OutputBackgroundWorkspace");
    TS_ASSERT(outputWS);

    // The expected result
    const auto expected = createExpectedResults(false);
    TS_ASSERT(workspacesEqual(expected, outputWS, 1e-4));
  }

  void test_exec_HistoWS() {
    // Create test input
    const auto inputWS = createTestData(true);
    const auto bgPoints = createTable();

    CreateUserDefinedBackground alg;
    alg.setChild(true);
    TS_ASSERT_THROWS_NOTHING(alg.initialize())
    TS_ASSERT(alg.isInitialized())
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("InputWorkspace", inputWS));
    TS_ASSERT_THROWS_NOTHING(
        alg.setPropertyValue("OutputBackgroundWorkspace", "__NotUsed"));
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("BackgroundPoints", bgPoints))
    TS_ASSERT_THROWS_NOTHING(alg.execute());
    TS_ASSERT(alg.isExecuted());

    MatrixWorkspace_sptr outputWS =
        alg.getProperty("OutputBackgroundWorkspace");
    TS_ASSERT(outputWS);

    // The expected result
    const auto expected = createExpectedResults(true);
    TS_ASSERT(workspacesEqual(expected, outputWS, 5e-2));
  }

  void test_exec_PointsWS_extend() {
    // Create test input
    const auto inputWS = createTestData(false);
    const auto bgPoints = createTable();

    // Remove last row, to make it extend the background to the data
    bgPoints->removeRow(bgPoints->rowCount() - 1);

    CreateUserDefinedBackground alg;
    alg.setChild(true);
    TS_ASSERT_THROWS_NOTHING(alg.initialize())
    TS_ASSERT(alg.isInitialized())
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("InputWorkspace", inputWS));
    TS_ASSERT_THROWS_NOTHING(
        alg.setPropertyValue("OutputBackgroundWorkspace", "__NotUsed"));
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("BackgroundPoints", bgPoints))
    TS_ASSERT_THROWS_NOTHING(alg.execute(););
    TS_ASSERT(alg.isExecuted());

    MatrixWorkspace_sptr outputWS =
        alg.getProperty("OutputBackgroundWorkspace");
    TS_ASSERT(outputWS);

    // The expected result
    const auto expected = createExpectedResults(false);
    TS_ASSERT_DELTA(expected->frequencies(0).back(),
                    outputWS->frequencies(0).back(), 0.001);
  }

  void test_exec_HistoWS_extend() {
    // Create test input
    const auto inputWS = createTestData(true);
    const auto bgPoints = createTable();

    // Remove last row, to make it extend the background to the data
    bgPoints->removeRow(bgPoints->rowCount() - 1);

    CreateUserDefinedBackground alg;
    alg.setChild(true);
    TS_ASSERT_THROWS_NOTHING(alg.initialize())
    TS_ASSERT(alg.isInitialized())
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("InputWorkspace", inputWS));
    TS_ASSERT_THROWS_NOTHING(
        alg.setPropertyValue("OutputBackgroundWorkspace", "__NotUsed"));
    TS_ASSERT_THROWS_NOTHING(alg.setProperty("BackgroundPoints", bgPoints))
    TS_ASSERT_THROWS_NOTHING(alg.execute());
    TS_ASSERT(alg.isExecuted());

    MatrixWorkspace_sptr outputWS =
        alg.getProperty("OutputBackgroundWorkspace");
    TS_ASSERT(outputWS);

    // The expected result
    const auto expected = createExpectedResults(true);
    TS_ASSERT_DELTA(expected->counts(0).back(), outputWS->counts(0).back(),
                    0.001);
  }

private:
  /// Create workspace containing test data
  MatrixWorkspace_sptr createTestData(bool isHisto) {
    return WorkspaceCreationHelper::Create2DWorkspaceFromFunction(
        dataFunction, 1, 0.0, 10.0, 0.1, isHisto);
  }

  /// Create table containing user-selected background points
  ITableWorkspace_sptr createTable() {
    auto table = boost::make_shared<Mantid::DataObjects::TableWorkspace>();
    table->addColumn("double", "X");
    table->addColumn("double", "Y");
    for (int i = 0; i < 100; i++) {
      const auto x = static_cast<double>(i) / 10.0;
      Mantid::API::TableRow row = table->appendRow();
      row << x << background(x);
    }
    return table;
  }

  /// Create expected results
  MatrixWorkspace_sptr createExpectedResults(bool isHisto) {
    std::vector<double> xData, yData, eData;
    constexpr double binWidth = 0.1;
    for (size_t i = 0; i < 100; ++i) {
      const double x = binWidth * static_cast<double>(i);
      xData.push_back(x);
      const double y = background(x);
      yData.push_back(isHisto ? y * binWidth : y);
      eData.push_back(0.0);
    }
    if (isHisto) {
      // add last bin edge
      xData.push_back(10.0);
    } else {
      // add extra point
      xData.push_back(10.0);
      yData.push_back(background(10.0));
      eData.push_back(0.0);
    }
    auto alg =
        Mantid::API::AlgorithmFactory::Instance().create("CreateWorkspace", 1);
    alg->initialize();
    alg->setChild(true);
    alg->setProperty("OutputWorkspace", "__NotUsed");
    alg->setProperty("DataX", xData);
    alg->setProperty("DataY", yData);
    alg->setProperty("DataE", eData);
    alg->execute();
    MatrixWorkspace_sptr output = alg->getProperty("OutputWorkspace");
    return output;
  }

  /// Compare workspaces
  bool workspacesEqual(const MatrixWorkspace_sptr lhs,
                       const MatrixWorkspace_sptr rhs, double tolerance) {
    auto alg = Mantid::API::AlgorithmFactory::Instance().create(
        "CompareWorkspaces", 1);
    alg->setChild(true);
    alg->initialize();
    alg->setProperty<MatrixWorkspace_sptr>("Workspace1", lhs);
    alg->setProperty<MatrixWorkspace_sptr>("Workspace2", rhs);
    alg->setProperty<double>("Tolerance", tolerance);
    alg->setProperty<bool>("CheckAxes", false);
    alg->execute();
    return alg->getProperty("Result");
  }
};

#endif /* MANTID_ALGORITHMS_CREATEUSERDEFINEDBACKGROUNDTEST_H_ */