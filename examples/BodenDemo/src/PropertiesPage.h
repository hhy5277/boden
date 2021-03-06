#pragma once

#include <bdn/foundation.h>
#include <bdn/ui.h>
#include <bdn/ui/yoga.h>

#include "Page.h"

namespace bdn
{
    std::shared_ptr<ContainerView> createPropertyDemoPage();

    class Properties : public ContainerView
    {
      public:
        Property<String> _stringProperty = "Hello World"s;
        Property<int> _intProperty = 42;
        Property<String> _stringToIntProperty{
            TransformBacking<String, int>{_intProperty, &Properties::intToString, &Properties::stringToInt}};

        Property<String> _streamProperty{StreamBacking()
                                         << "The string properties value is: \"" << _stringProperty << "\"\n"
                                         << "The int property value is: " << _intProperty << "\n"
                                         << "The stringToInt property value is: \"" << _stringToIntProperty << "\"\n"};

      public:
        Properties() {}

        static String intToString(int v)
        {
            std::ostringstream s;
            s << v;
            return s.str();
        }

        static int stringToInt(String v)
        {
            int result = 0;
            std::istringstream s(v);
            s >> result;
            return result;
        }

        void init()
        {
            stylesheet = FlexJsonStringify(
                {"direction" : "Column", "flexGrow" : 1.0, "flexShrink" : 1.0, "alignItems" : "Stretch"});

            auto stringField = std::make_shared<TextField>();
            stringField->text.bind(_stringProperty);
            stringField->stylesheet = FlexJsonStringify({"minimumSize" : {"width" : 150.0}});
            addChildView(makeRow("String", stringField, 5., 5., 0.8));

            auto intField = std::make_shared<TextField>();
            intField->text.bind(_stringToIntProperty);
            intField->stylesheet = FlexJsonStringify({"minimumSize" : {"width" : 150.0}});
            addChildView(makeRow("Int", intField, 5., 5., 0.8));

            auto btnContainer = std::make_shared<ContainerView>();
            btnContainer->stylesheet = FlexJsonStringify(
                {"direction" : "Row", "flexGrow" : 0.0, "flexShrink" : 0.0, "justifyContent" : "lexEnd"});

            auto plusBtn = std::make_shared<Button>();
            plusBtn->label = "+";
            auto minusBtn = std::make_shared<Button>();
            minusBtn->label = "-";

            plusBtn->onClick() += [=](auto) { ++_intProperty; };
            minusBtn->onClick() += [=](auto) { --_intProperty; };

            btnContainer->addChildView(plusBtn);
            btnContainer->addChildView(minusBtn);

            addChildView(btnContainer);

            auto header = std::make_shared<Label>();
            header->text = "StreamingBacking property:";
            header->stylesheet = FlexJsonStringify({"flexShrink" : 0});
            addChildView(header);

            auto streamLabel = std::make_shared<Label>();
            streamLabel->text.bind(_streamProperty);
            addChildView(streamLabel);
        }

      public:
        const std::type_info &typeInfoForCoreCreation() const override { return typeid(ContainerView); }
    };
}
