#include "ompu/io/io_fwd.hpp"

#include "ompu/io/chord_score.hpp"

#pragma warning(push)
#include "ompu/juicy/qi_warnings.hpp"

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/support_unused.hpp>
#pragma warning(pop)

#include <memory>


namespace ompu { namespace io { namespace in {

class ChordPro
    : boost::spirit::qi::grammar<
        in_iterator_type, ChordScore(),
        boost::spirit::qi::standard::blank_type
    >
{
public:
    using root_type = ChordScore;
    using root_rule_type = boost::spirit::qi::rule<
        in_iterator_type,
        root_type(),
        boost::spirit::qi::standard::blank_type
    >;

    explicit ChordPro();

private:
    root_rule_type root_;

    class Impl;
    std::unique_ptr<Impl> impl_;
};

}}} // ompu
