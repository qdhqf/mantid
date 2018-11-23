// Mantid Repository : https://github.com/mantidproject/mantid
//
// Copyright &copy; 2018 ISIS Rutherford Appleton Laboratory UKRI,
//     NScD Oak Ridge National Laboratory, European Spallation Source
//     & Institut Laue - Langevin
// SPDX - License - Identifier: GPL - 3.0 +
#include "MantidDataObjects/Histogram1D.h"
#include "MantidKernel/Exception.h"

namespace Mantid {
namespace DataObjects {

/// Construct empty
Histogram1D::Histogram1D(HistogramData::Histogram::XMode xmode,
                         HistogramData::Histogram::YMode ymode)
    : API::ISpectrum(), m_histogram(xmode, ymode) {
  if (ymode == HistogramData::Histogram::YMode::Counts) {
    m_histogram.setCounts(0);
    m_histogram.setCountStandardDeviations(0);
  } else if (ymode == HistogramData::Histogram::YMode::Frequencies) {
    m_histogram.setFrequencies(0);
    m_histogram.setFrequencyStandardDeviations(0);
  } else {
    throw std::logic_error("Histogram1D: YMode must be Counts or Frequencies");
  }
}

/// Construct from ISpectrum.
Histogram1D::Histogram1D(const ISpectrum &other)
    : ISpectrum(other), m_histogram(other.histogram()) {}

/// Assignment from ISpectrum.
Histogram1D &Histogram1D::operator=(const ISpectrum &rhs) {
  ISpectrum::operator=(rhs);
  m_histogram = rhs.histogram();
  return *this;
}

/// Copy data from a Histogram1D or EventList, via ISpectrum reference.
void Histogram1D::copyDataFrom(const ISpectrum &source) {
  source.copyDataInto(*this);
}

/// Used by copyDataFrom for dynamic dispatch for its `source`.
void Histogram1D::copyDataInto(Histogram1D &sink) const {
  sink.m_histogram = m_histogram;
}

void Histogram1D::clearData() {
  MantidVec &yValues = this->dataY();
  std::fill(yValues.begin(), yValues.end(), 0.0);
  MantidVec &eValues = this->dataE();
  std::fill(eValues.begin(), eValues.end(), 0.0);
}

/// Moved from MatrixWorkspace. Separate implementations for Hist1D and
/// EventList. Applies bin weighting for masking workspaces.
///
/// @param binIndex ::  The index of the bin in the spectrum
/// @param weight   ::  How heavily the bin is to be masked. =1 for full
void Histogram1D::applyBinWeight(const size_t &binIndex, const double &weight) {
  if (weight != 0.) {
    double &y = this->mutableY()[binIndex];
    (std::isnan(y) || std::isinf(y)) ? y = 0. : y *= (1 - weight);
    double &e = this->mutableE()[binIndex];
    (std::isnan(e) || std::isinf(e)) ? e = 0. : e *= (1 - weight);
  }
}

/// Deprecated, use setSharedX() instead. Sets the x data.
/// @param X :: vector of X data
void Histogram1D::setX(const Kernel::cow_ptr<HistogramData::HistogramX> &X) {
  m_histogram.setX(X);
}

/// Deprecated, use mutableX() instead. Returns the x data
MantidVec &Histogram1D::dataX() { return m_histogram.dataX(); }

/// Deprecated, use x() instead. Returns the x data const
const MantidVec &Histogram1D::dataX() const { return m_histogram.dataX(); }

/// Deprecated, use x() instead. Returns the x data const
const MantidVec &Histogram1D::readX() const { return m_histogram.readX(); }

/// Deprecated, use sharedX() instead. Returns a pointer to the x data
Kernel::cow_ptr<HistogramData::HistogramX> Histogram1D::ptrX() const {
  return m_histogram.ptrX();
}

/// Deprecated, use mutableDx() instead.
MantidVec &Histogram1D::dataDx() { return m_histogram.dataDx(); }
/// Deprecated, use dx() instead.
const MantidVec &Histogram1D::dataDx() const { return m_histogram.dataDx(); }
/// Deprecated, use dx() instead.
const MantidVec &Histogram1D::readDx() const { return m_histogram.readDx(); }

/**
 * Makes sure a histogram has valid Y and E data.
 * @param histogram A histogram to check.
 * @throw std::invalid_argument if Y or E data is NULL.
 */
void Histogram1D::checkAndSanitizeHistogram(
    HistogramData::Histogram &histogram) {
  if (!histogram.sharedY()) {
    throw std::invalid_argument(
        "Histogram1D: invalid input: Y data set to nullptr");
  }
  if (!histogram.sharedE()) {
    throw std::invalid_argument(
        "Histogram1D: invalid input: E data set to nullptr");
  }
}

} // namespace DataObjects
} // namespace Mantid
