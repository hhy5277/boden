
#include <bdn/test.h>

#include <bdn/test/TestContainerViewCore.h>

#import <bdn/mac/UIProvider.hh>
#import "TestMacChildViewCoreMixin.hh"

using namespace bdn;

class TestMacContainerViewCore : public bdn::test::TestMacChildViewCoreMixin<bdn::test::TestContainerViewCore>
{
  protected:
    void initCore() override { TestMacChildViewCoreMixin<TestContainerViewCore>::initCore(); }
};

TEST_CASE("mac.ContainerViewCore")
{
    std::shared_ptr<TestMacContainerViewCore> test = std::make_shared<TestMacContainerViewCore>();

    test->runTests();
}
