#pragma once

#include "gtest/gtest.h"

class ExampleTest : public ::testing::Test
{

protected:

	virtual void SetUp();

	virtual void TearDown();

};

