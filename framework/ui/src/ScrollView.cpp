
#include <bdn/ScrollView.h>

#include <bdn/AppRunnerBase.h>

namespace bdn
{
    namespace detail
    {
        VIEW_CORE_REGISTRY_IMPLEMENTATION(ScrollView)
    }

    ScrollView::ScrollView(std::shared_ptr<ViewCoreFactory> viewCoreFactory)
        : View(std::move(viewCoreFactory)), verticalScrollingEnabled(true)
    {
        detail::VIEW_CORE_REGISTER(ScrollView, View::viewCoreFactory());
        content.onChange() += [=](auto va) { _content.update(shared_from_this(), va->get()); };
    }

    void ScrollView::bindViewCore()
    {
        View::bindViewCore();

        auto scrollCore = core<ScrollView::Core>();
        scrollCore->content.bind(content);
        scrollCore->horizontalScrollingEnabled.bind(horizontalScrollingEnabled);
        scrollCore->verticalScrollingEnabled.bind(verticalScrollingEnabled);
    }

    void ScrollView::scrollClientRectToVisible(const Rect &area)
    {
        AppRunnerBase::assertInMainThread();

        auto scrollCore = core<ScrollView::Core>();
        scrollCore->scrollClientRectToVisible(area);
    }

    std::list<std::shared_ptr<View>> ScrollView::childViews()
    {
        AppRunnerBase::assertInMainThread();

        if (content.get() != nullptr) {
            return {content.get()};
        }

        return {};
    }

    void ScrollView::childViewStolen(const std::shared_ptr<View> &childView)
    {
        AppRunnerBase::assertInMainThread();

        if (childView == content.get()) {
            content = nullptr;
        }
    }
}
