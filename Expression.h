/// \file Expression.h
///
/// \author Bryn Lloyd
/// \copyright 2023

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace xpr {

class Element;

/**	\brief Self-documenting expression evaluator
*/
class Expression
{
  public:
    template <typename T>
    Expression(T && x)
        : m_Pimpl{new ExpressionModel<T>(std::forward<T>(x))}
    {}
    Expression(Expression const & e) { *this = e; }
    Expression(Expression &&) = default;
    Expression & operator=(Expression const & e);
    Expression & operator=(Expression &&) = default;

    bool Evaluate(const std::vector<Element *> & ents) const { return m_Pimpl->Evaluate(ents); }

    std::string Description() const { return m_Pimpl->Description(); }

    Expression operator&&(const Expression & other) const;
    Expression operator||(const Expression & other) const;
    Expression operator!() const;

  private:
    struct ExpressionConcept
    {
        virtual ~ExpressionConcept() = default;
        virtual bool Evaluate(const std::vector<Element *> &) const = 0;
        virtual std::string Description() const = 0;
        virtual std::unique_ptr<ExpressionConcept> Clone() const = 0;
    };

    template <typename T>
    struct ExpressionModel : ExpressionConcept
    {
        ExpressionModel(T && value)
            : object{std::forward<T>(value)}
        {}

        bool Evaluate(const std::vector<Element *> & ents) const override
        {
            return object.Evaluate(ents);
        }

        std::string Description() const override
        {
            return object.Description();
        }

        std::unique_ptr<ExpressionConcept> Clone() const override
        {
            return std::make_unique<ExpressionModel>(*this);
        }

        T object;
    };

    std::unique_ptr<ExpressionConcept> m_Pimpl;
};

} // namespace xpr