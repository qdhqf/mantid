#ifndef MANTID_KERNEL_MULTIFILENAMEPARSERTEST_H_
#define MANTID_KERNEL_MULTIFILENAMEPARSERTEST_H_

#include <cxxtest/TestSuite.h>

#include "MantidKernel/MultiFileNameParser.h"

#include <vector>

using namespace Mantid::Kernel;
using namespace Mantid::Kernel::MultiFileNameParsing;

class MultiFileNameParserTest : public CxxTest::TestSuite
{
public:
  typedef std::vector<std::vector<unsigned int> > ParsedRuns;

  void test_single()
  {
    ParsedRuns result = parseMultiRunString("1");

    TS_ASSERT_EQUALS(result[0][0], 1);
  }

  void test_listOfSingles()
  {
    ParsedRuns result = parseMultiRunString("1,2,3,4,5,6,7,8");

    TS_ASSERT_EQUALS(result[0][0], 1);
    TS_ASSERT_EQUALS(result[1][0], 2);
    TS_ASSERT_EQUALS(result[2][0], 3);
    TS_ASSERT_EQUALS(result[3][0], 4);
    TS_ASSERT_EQUALS(result[4][0], 5);
    TS_ASSERT_EQUALS(result[5][0], 6);
    TS_ASSERT_EQUALS(result[6][0], 7);
    TS_ASSERT_EQUALS(result[7][0], 8);
  }

  void test_range()
  {
    ParsedRuns result = parseMultiRunString("1:8");

    TS_ASSERT_EQUALS(result[0][0], 1);
    TS_ASSERT_EQUALS(result[1][0], 2);
    TS_ASSERT_EQUALS(result[2][0], 3);
    TS_ASSERT_EQUALS(result[3][0], 4);
    TS_ASSERT_EQUALS(result[4][0], 5);
    TS_ASSERT_EQUALS(result[5][0], 6);
    TS_ASSERT_EQUALS(result[6][0], 7);
    TS_ASSERT_EQUALS(result[7][0], 8);
  }

  void test_steppedRange()
  {
    ParsedRuns result = parseMultiRunString("1:8:3");

    TS_ASSERT_EQUALS(result[0][0], 1);
    TS_ASSERT_EQUALS(result[1][0], 4);
    TS_ASSERT_EQUALS(result[2][0], 7);
  }

  void test_descendingSteppedRange()
  {
    ParsedRuns result = parseMultiRunString("8:1:3");

    TS_ASSERT_EQUALS(result[0][0], 8);
    TS_ASSERT_EQUALS(result[1][0], 5);
    TS_ASSERT_EQUALS(result[2][0], 2);
  }

  void test_descendingSteppedRange_2()
  {
    ParsedRuns result = parseMultiRunString("8:1:2");
    
    TS_ASSERT_EQUALS(result[0][0], 8);
    TS_ASSERT_EQUALS(result[1][0], 6);
    TS_ASSERT_EQUALS(result[2][0], 4);
    TS_ASSERT_EQUALS(result[3][0], 2);
  }

  void test_addedList()
  {
    ParsedRuns result = parseMultiRunString("1+2+3+4+5+6+7+8");

    TS_ASSERT_EQUALS(result[0][0], 1);
    TS_ASSERT_EQUALS(result[0][1], 2);
    TS_ASSERT_EQUALS(result[0][2], 3);
    TS_ASSERT_EQUALS(result[0][3], 4);
    TS_ASSERT_EQUALS(result[0][4], 5);
    TS_ASSERT_EQUALS(result[0][5], 6);
    TS_ASSERT_EQUALS(result[0][6], 7);
    TS_ASSERT_EQUALS(result[0][7], 8);
  }

  void test_addedRange()
  {
    ParsedRuns result = parseMultiRunString("1-8");

    TS_ASSERT_EQUALS(result[0][0], 1);
    TS_ASSERT_EQUALS(result[0][1], 2);
    TS_ASSERT_EQUALS(result[0][2], 3);
    TS_ASSERT_EQUALS(result[0][3], 4);
    TS_ASSERT_EQUALS(result[0][4], 5);
    TS_ASSERT_EQUALS(result[0][5], 6);
    TS_ASSERT_EQUALS(result[0][6], 7);
    TS_ASSERT_EQUALS(result[0][7], 8);
  }

  void test_addedSteppedRange()
  {
    ParsedRuns result = parseMultiRunString("1-8:3");

    TS_ASSERT_EQUALS(result[0][0], 1);
    TS_ASSERT_EQUALS(result[0][1], 4);
    TS_ASSERT_EQUALS(result[0][2], 7);
  }

  void test_descendingAddedRange()
  {
    ParsedRuns result = parseMultiRunString("8-1");
    
    TS_ASSERT_EQUALS(result[0][0], 8);
    TS_ASSERT_EQUALS(result[0][1], 7);
    TS_ASSERT_EQUALS(result[0][2], 6);
    TS_ASSERT_EQUALS(result[0][3], 5);
    TS_ASSERT_EQUALS(result[0][4], 4);
    TS_ASSERT_EQUALS(result[0][5], 3);
    TS_ASSERT_EQUALS(result[0][6], 2);
    TS_ASSERT_EQUALS(result[0][7], 1);
  }

  void test_descendingAddedSteppedRange()
  {
    ParsedRuns result = parseMultiRunString("8-1:3");
    
    TS_ASSERT_EQUALS(result[0][0], 8);
    TS_ASSERT_EQUALS(result[0][1], 5);
    TS_ASSERT_EQUALS(result[0][2], 2);
  }

  void test_descendingAddedSteppedRange_2()
  {
    ParsedRuns result = parseMultiRunString("8-1:2");
    
    TS_ASSERT_EQUALS(result[0][0], 8);
    TS_ASSERT_EQUALS(result[0][1], 6);
    TS_ASSERT_EQUALS(result[0][2], 4);
    TS_ASSERT_EQUALS(result[0][3], 2);
  }

  void test_complexList()
  {
    ParsedRuns result = parseMultiRunString("1-4,1:4,1+2+3+4,1,2,3,4");
    
    TS_ASSERT_EQUALS(result[0][0], 1);
    TS_ASSERT_EQUALS(result[0][1], 2);
    TS_ASSERT_EQUALS(result[0][2], 3);
    TS_ASSERT_EQUALS(result[0][3], 4);

    TS_ASSERT_EQUALS(result[1][0], 1);
    TS_ASSERT_EQUALS(result[2][0], 2);
    TS_ASSERT_EQUALS(result[3][0], 3);
    TS_ASSERT_EQUALS(result[4][0], 4);
    
    TS_ASSERT_EQUALS(result[5][0], 1);
    TS_ASSERT_EQUALS(result[5][1], 2);
    TS_ASSERT_EQUALS(result[5][2], 3);
    TS_ASSERT_EQUALS(result[5][3], 4);
    
    TS_ASSERT_EQUALS(result[6][0], 1);
    TS_ASSERT_EQUALS(result[7][0], 2);
    TS_ASSERT_EQUALS(result[8][0], 3);
    TS_ASSERT_EQUALS(result[9][0], 4);
  }

  void test_nothingReturnedWhenPassedEmptyString()
  {
    TS_ASSERT_EQUALS(parseMultiRunString("").size(), 0);
  }

  void test_errorThrownWhenPassedUnexpectedChar()
  {
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("#"),
      const std::runtime_error & re,
      std::string(re.what()),
      "Non-numeric or otherwise unaccetable character(s) detected.");
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("a"),
      const std::runtime_error & re,
      std::string(re.what()),
      "Non-numeric or otherwise unaccetable character(s) detected.");
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("Z"),
      const std::runtime_error & re,
      std::string(re.what()),
      "Non-numeric or otherwise unaccetable character(s) detected.");
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("("),
      const std::runtime_error & re,
      std::string(re.what()),
      "Non-numeric or otherwise unaccetable character(s) detected.");
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString(">"),
      const std::runtime_error & re,
      std::string(re.what()),
      "Non-numeric or otherwise unaccetable character(s) detected.");
    
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("1012-n1059:5"),
      const std::runtime_error & re,
      std::string(re.what()),
      "Non-numeric or otherwise unaccetable character(s) detected.");
  }

  void test_errorThrownOnEmptyToken()
  {
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("1,,3"),
      const std::runtime_error & re, 
      std::string(re.what()),
      "A comma-separated token is empty.");
  }

  void test_errorThrownWhenStringDoesNotStartAndEndWithNumeral()
  {
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("+2+3"),
      const std::runtime_error & re,
      std::string(re.what()),
      "The token \"+2+3\" is of an incorrect form.  Does it begin or end with a plus, minus or colon?");
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("2-3:"),
      const std::runtime_error & re,
      std::string(re.what()),
      "The token \"2-3:\" is of an incorrect form.  Does it begin or end with a plus, minus or colon?");
  }

  void test_errorThrownIfStepSizeEqualsZero()
  {
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("1:3:0"),
      const std::runtime_error & re, 
      std::string(re.what()),
      "Unable to generate a range with a step size of zero.");
  }

  void test_errorThrownIfAddedStepSizeEqualsZero()
  {
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("1-3:0"),
      const std::runtime_error & re, 
      std::string(re.what()),
      "Unable to generate a range with a step size of zero.");
  }

  void test_errorThrownIfOfIncorrectForm()
  {
    TS_ASSERT_THROWS_EQUALS(parseMultiRunString("1-3-1"),
      const std::runtime_error & re, 
      std::string(re.what()),
      "The token \"1-3-1\" is of an incorrect form.");
  }
};

#endif /* MANTID_KERNEL_MULTIFILENAMEPARSERTEST_H_ */