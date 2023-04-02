#include "Expression.h"

#include <iostream>

namespace xpr {

class Element
{
  public:
    virtual ~Element() = default;
    virtual std::string Type() const = 0;
};

class A : public Element
{
  public:
    std::string Type() const override { return "Type A"; }
};

class B : public Element
{
  public:
    std::string Type() const override { return "Type B"; }
};

template <typename T>
struct AnyOf
{
    AnyOf(const std::string & type_name)
        : m_TypeName(type_name)
    {}

    bool Evaluate(const std::vector<Element *> & ents) const
    {
        return std::any_of(ents.begin(), ents.end(), [](Element * e) { return (dynamic_cast<T *>(e) != nullptr); });
    }

    std::string Description() const
    {
        return "has any " + m_TypeName;
    }

    std::string m_TypeName;
};

template <typename T>
struct FirstIs
{
    FirstIs(const std::string & type_name)
        : m_TypeName(type_name)
    {}

    bool Evaluate(const std::vector<Element *> & ents) const
    {
        return ents.size() >= 1 && (dynamic_cast<T *>(ents.front()) != nullptr);
    }

    std::string Description() const
    {
        return "first is " + m_TypeName;
    }

    std::string m_TypeName;
};

} // namespace xpr

int main(int argc, char ** argv)
{
    using namespace xpr;
    using X = xpr::Expression;

    std::vector<Element *> ents;
    ents.push_back(new A);
    ents.push_back(new A);
    ents.push_back(new B);

    auto ex = X(AnyOf<A>("A")) || X(AnyOf<B>("B"));
    std::cout << ex.Description() + ": " << ex.Evaluate(ents) << " != " << (!ex).Evaluate(ents) << std::endl;

    auto first_is_B = X(FirstIs<B>("B"));
    std::cout << first_is_B.Description() + ": " << first_is_B.Evaluate(ents) << std::endl;

    auto ex2 = first_is_B && !first_is_B;
    std::cout << ex2.Description() + ": " << ex2.Evaluate(ents) << std::endl;
    std::cout << first_is_B.Description() + ": " << first_is_B.Evaluate(ents) << std::endl;

    auto ex3 = X(AnyOf<A>("A")) || ex2;
    std::cout << ex3.Description() + ": " << ex3.Evaluate(ents) << std::endl;
    return 0;
}