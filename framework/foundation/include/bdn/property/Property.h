#pragma once

#include <bdn/String.h>
#include <bdn/property/IValueAccessor.h>

#include <bdn/property/Compare.h>
#include <bdn/property/GetterSetter.h>
#include <bdn/property/GetterSetterBacking.h>
#include <bdn/property/InternalValueBacking.h>
#include <bdn/property/Setter.h>
#include <bdn/property/SetterBacking.h>
#include <bdn/property/Streaming.h>
#include <bdn/property/Transform.h>
#include <bdn/property/property_forward_decl.h>

using namespace std::string_literals;

namespace bdn
{
    enum class BindMode
    {
        unidirectional,
        bidirectional
    };

    template <class ValType> class Property : virtual public IValueAccessor<ValType>
    {
      private:
        template <typename T> class overloadsArrowOperator
        {
            template <typename C = T>
            static uint8_t _test(int dummy, decltype(&(*((C *)nullptr)->operator->())) pDummy = nullptr);

            template <typename C = T> static uint16_t _test(...);

            static_assert(sizeof(_test<T>(0)) != 0, "This should never trigger");

          public:
            enum
            {
                value = sizeof(_test<T>(0)) == sizeof(uint8_t) ? 1 : 0
            };
        };

        using internal_backing_t = InternalValueBacking<ValType>;
        using gs_backing_t = GetterSetterBacking<ValType>;
        using setter_backing_t = SetterBacking<ValType>;

      public:
        using backing_t = Backing<ValType>;
        using value_accessor_t_ptr = typename Backing<ValType>::value_accessor_t_ptr;

        Property() : _backing(std::make_shared<internal_backing_t>()) {}
        Property(const Property &) = delete;
        ~Property() override = default;

        Property(ValType value) : _backing(std::make_shared<internal_backing_t>())
        {
            set(value, false /* do not notify on initial set */);
        }

        Property(const GetterSetter<ValType> &getterSetter)
        {
            _backing =
                std::make_shared<gs_backing_t>(getterSetter.getter(), getterSetter.setter(), getterSetter.member());
        }

        Property(const Setter<ValType> &setter) { _backing = std::make_shared<setter_backing_t>(setter.setter()); }

        Property(Streaming &stream) { _backing = std::make_shared<Streaming::Backing>(stream); }

        template <class U> Property(const Transform<ValType, U> &transform)
        {
            _backing = std::make_shared<typename Transform<ValType, U>::Backing>(transform);
        }

        Property(std::shared_ptr<Backing<ValType>> backing) { _backing = backing; }

        template <class _Rep, class _Period>
        Property(const std::chrono::duration<_Rep, _Period> &duration)
            : _backing(std::make_shared<internal_backing_t>())
        {
            set(std::chrono::duration_cast<ValType>(duration), false);
        }

      public:
        ValType get() const override { return _backing->get(); }
        void set(ValType value, bool notify = true) { _backing->set(value, notify); }

        const auto backing() const { return _backing; }

      public:
        void bind(Property<ValType> &sourceProperty, BindMode bindMode = BindMode::bidirectional)
        {
            if (Compare<ValType>::is_faked && bindMode == BindMode::bidirectional) {
                throw std::logic_error("You cannot bind this type of Property bidirectional, its == operator is faked "
                                       "and therefor would end up in an endless loop.");
            }

            _backing->bind(sourceProperty.backing());
            if (bindMode == BindMode::bidirectional) {
                sourceProperty.backing()->bind(_backing);
            }
        }

      public:
        auto &onChange() const { return _backing->onChange(); }

      public:
        template <typename U = ValType, typename std::enable_if<overloadsArrowOperator<U>::value, int>::type = 0>
        const ValType operator->() const
        {
            return get();
        }

        template <typename U = ValType, typename std::enable_if<!overloadsArrowOperator<U>::value, int>::type = 0>
        const typename backing_t::Proxy operator->() const
        {
            return _backing->proxy();
        }

        Property &operator=(const ValType &value)
        {
            set(value);
            return *this;
        }

        template <class _Rep, class _Period> Property &operator=(const std::chrono::duration<_Rep, _Period> &duration)
        {
            set(std::chrono::duration_cast<ValType>(duration));
            return *this;
        }

        bool operator==(const ValType &value) const { return get() == value; }

        bool operator==(const char *cString) const { return Property<ValType>::operator==(String(cString)); }

        bool operator!=(const ValType &value) const { return get() != value; }

        bool operator!=(const char *cString) const { return Property<ValType>::operator!=(String(cString)); }

        ValType operator*() const { return get(); }

        operator ValType() const { return get(); }

        Property &operator=(const Property &otherProperty)
        {
            if (&otherProperty == this) {
                return *this;
            }

            _backing->set(otherProperty.backing()->get());
            return *this;
        }

        Property operator+() const { return +this->get(); }
        Property operator-() const { return -this->get(); }

        Property operator+(const Property &otherProperty) const { return this->get() + ValType(otherProperty); }

        Property operator-(const Property &otherProperty) const { return this->get() - ValType(otherProperty); }

        Property operator*(const Property &otherProperty) const { return this->get() * ValType(otherProperty); }

        Property operator/(const Property &otherProperty) const { return this->get() / ValType(otherProperty); }

        Property operator%(const Property &otherProperty) const { return this->get() % ValType(otherProperty); }

        Property operator~() const { return ~(this->get()); }

        Property operator&(const Property &otherProperty) const { return this->get() & ValType(otherProperty); }

        Property operator|(const Property &otherProperty) const { return this->get() | ValType(otherProperty); }

        Property operator^(const Property &otherProperty) const { return this->get() ^ ValType(otherProperty); }

        Property operator<<(const Property &otherProperty) const { return this->get() << ValType(otherProperty); }

        Property operator>>(const Property &otherProperty) const { return this->get() >> ValType(otherProperty); }

        ValType operator+(const ValType &other) const { return this->get() + other; }

        ValType operator-(const ValType &other) const { return this->get() - other; }

        ValType operator*(const ValType &other) const { return this->get() * other; }

        ValType operator/(const ValType &other) const { return this->get() / other; }

        ValType operator%(const ValType &other) const { return this->get() % other; }

        ValType operator&(const ValType &other) const { return this->get() & other; }

        ValType operator|(const ValType &other) const { return this->get() | other; }

        ValType operator^(const ValType &other) const { return this->get() ^ other; }

        ValType operator<<(const ValType &other) const { return this->get() << other; }

        ValType operator>>(const ValType &other) const { return this->get() >> other; }

        Property<ValType> &operator++()
        {
            set((ValType)this->get() + 1);
            return *this;
        }
        Property<ValType> &operator--()
        {
            set((ValType)this->get() - 1);
            return *this;
        }

        Property &operator+=(const Property &otherProperty)
        {
            set(this->get() + ValType(otherProperty));
            return *this;
        }

        Property &operator-=(const Property &otherProperty)
        {
            set(this->get() - ValType(otherProperty));
            return *this;
        }

        Property &operator*=(const Property &otherProperty)
        {
            set(this->get() * ValType(otherProperty));
            return *this;
        }

        Property &operator/=(const Property &otherProperty)
        {
            set(this->get() / ValType(otherProperty));
            return *this;
        }

        Property &operator%=(const Property &otherProperty)
        {
            set(this->get() % ValType(otherProperty));
            return *this;
        }

        Property &operator&=(const Property &otherProperty)
        {
            set(this->get() & ValType(otherProperty));
            return *this;
        }

        Property &operator|=(const Property &otherProperty)
        {
            set(this->get() | ValType(otherProperty));
            return *this;
        }

        Property &operator^=(const Property &otherProperty)
        {
            set(this->get() ^ ValType(otherProperty));
            return *this;
        }

        Property &operator<<=(const Property &otherProperty)
        {
            set(this->get() << ValType(otherProperty));
            return *this;
        }

        Property &operator>>=(const Property &otherProperty)
        {
            set(this->get() >> ValType(otherProperty));
            return *this;
        }

      private:
        mutable std::shared_ptr<backing_t> _backing;
        mutable bool _isConnected = false;
    };

    template <typename CHAR_TYPE, class CHAR_TRAITS, typename PROP_VALUE>
    std::basic_ostream<CHAR_TYPE, CHAR_TRAITS> &operator<<(std::basic_ostream<CHAR_TYPE, CHAR_TRAITS> &stream,
                                                           const Property<PROP_VALUE> &s)
    {
        // note that if there is no << operator for PROP_VALUE then that is not
        // a problem. Since this << operator is a template all that does is
        // remove the operator from the list of possible overloads. So it would
        // be as if there was no << operator for Property<PROP_VALUE>, which is
        // exactly what we would want in this case.

        return stream << s.get();
    }
}
