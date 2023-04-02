#include "Expression.h"

namespace xpr {

struct And
{
    And(const Expression & l, const Expression & r)
        : lhs(l)
        , rhs(r)
    {}

    bool Evaluate(const std::vector<Element *> & ents) const
    {
        return lhs.Evaluate(ents) && rhs.Evaluate(ents);
    }

    bool Explain(const std::vector<Element *> & ents, std::string& reason) const
    {
        if (!lhs.Evaluate(ents))
        {
            reason = lhs.Description() + " is false";
            return false;
        }
        if (!rhs.Evaluate(ents))
        {
            reason = rhs.Description() + " is false";
            return false;
        }
        return true;
    }

    std::string Description() const
    {
        return lhs.Description() + " and " + rhs.Description();
    }

    Expression lhs;
    Expression rhs;
};

struct Or
{
    Or(const Expression & l, const Expression & r)
        : lhs(l)
        , rhs(r)
    {}

    bool Evaluate(const std::vector<Element *> & ents) const
    {
        return lhs.Evaluate(ents) || rhs.Evaluate(ents);
    }

    bool Explain(const std::vector<Element *> & ents, std::string& reason) const
    {
        if (!Evaluate(ents))
        {
            reason = "both " + lhs.Description() + " and " + rhs.Description() + " is false";
            return false;
        }
        return true;
    }

    std::string Description() const
    {
        return lhs.Description() + " or " + rhs.Description();
    }

    Expression lhs;
    Expression rhs;
};

struct Not
{
    Not(const Expression & e)
        : expression(e)
    {}

    bool Evaluate(const std::vector<Element *> & ents) const
    {
        return !expression.Evaluate(ents);
    }

    bool Explain(const std::vector<Element *> & ents, std::string& reason) const
    {
        if (!Evaluate(ents))
        {
            reason = expression.Description() + " is not false";
            return false;
        }
        return true;
    }

    std::string Description() const { return "not " + expression.Description(); }

    Expression expression;
};

Expression & Expression::operator=(Expression const & e)
{
    if (this != &e)
    {
        m_Pimpl = e.m_Pimpl->Clone();
    }
    return *this;
}

Expression Expression::operator&&(const Expression & other) const
{
    return Expression(And(*this, other));
}

Expression Expression::operator||(const Expression & other) const
{
    return Expression(Or(*this, other));
}

Expression Expression::operator!() const
{
    return Expression(Not(*this));
}

} // namespace xpr
