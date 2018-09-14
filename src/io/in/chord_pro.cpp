#include "ompu/io/in/chord_pro.hpp"


#include <boost/variant.hpp>
#include <boost/phoenix.hpp>

#pragma warning(push)
#include "ompu/juicy/qi_warnings.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_locals.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#pragma warning(pop)


#include <boost/preprocessor/seq/head.hpp>
#include <boost/preprocessor/seq/tail.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>


#include <vector>

// https://www.chordpro.org/chordpro/ChordPro-File-Format-Specification.html



namespace ompu { namespace io { namespace in {


class ChordPro::Impl
{
public:
    explicit Impl(ChordPro::root_rule_type& root)
    {
        namespace qi = boost::spirit::qi;
        namespace ns = qi::standard;
        namespace phx = boost::phoenix;

        using qi::_1; using qi::_2; using qi::_3; using qi::_4;
        using qi::_a;


        tone_height_.name("tone-height");
        tone_height_ %= ns::char_('A', 'G');


        chord_frag_minor_.name("chord-frag-minor");
        chord_frag_minor_ %= ns::char_('m');

        chord_frag_tension_.name("chord-frag-tension");

        chord_frag_7th_.name("chord-frag-7th");
        chord_frag_7th_ %= qi::as_string [
            qi::
        ];


        chord_.name("chord");
       #define OMPU_DEF_PUSH_CHORD_SPECS(which_spec) [phx::push_back(qi::_a, detail::chord_specs::which_spec)]

        chord_ %= qi::as_string [
              qi::lit('[')
            > tone_height_

            > (
                (
                    (qi::lit("add2") OMPU_DEF_PUSH_CHORD_SPECS(add2)) |
                    (qi::lit("add9") OMPU_DEF_PUSH_CHORD_SPECS(add9))
                ) |
                (
                      (-chord_frag_minor_) OMPU_DEF_PUSH_CHORD_SPECS(minor)

                    > (
                        (
                            (
                                (qi::lit("dim7") OMPU_DEF_PUSH_CHORD_SPECS(dim7)) |
                                (qi::lit("M") OMPU_DEF_PUSH_CHORD_SPECS(M7)) |
                                (qi::lit("7") OMPU_DEF_PUSH_CHORD_SPECS(m7))
                            )

                            > qi::omit[
                                (qi::lit("sus2") OMPU_DEF_PUSH_CHORD_SPECS(sus2)) |
                                (qi::lit("sus4") OMPU_DEF_PUSH_CHORD_SPECS(sus4))
                            ]
                        ) |
                        (
                    qi::omit [
                        (qi::lit("dim") OMPU_DEF_PUSH_CHORD_SPECS(dim)) |
                        (qi::lit("aug") OMPU_DEF_PUSH_CHORD_SPECS(aug))

                    ]
                )
            )
            > qi::lit(']')
        ];
        #undef OMPU_DEF_PUSH_CHORD_SPECS


        blob_.name("blob");
        blob_ %= chord_ | plain_string_;


        directive_.name("directive");

        #define OMPU_DEF(r, data, elem) \
            ( \
                qi::lit(BOOST_PP_STRINGIZE(elem)) >> qi::attr(score_data::DirectiveType::elem) \
            ) OMPU_DEF_COMMA

        directive_ %=
            (
                #define OMPU_DEF_COMMA ,
                BOOST_PP_SEQ_FOR_EACH(OMPU_DEF, _, BOOST_PP_SEQ_TAIL(OMPU_IO_SCORE_DATA_DIRECTIVE_TYPES))

                #undef OMPU_DEF_COMMA
                #define OMPU_DEF_COMMA
                OMPU_DEF(BOOST_PP_SEQ_HEAD(OMPU_IO_SCORE_DATA_DIRECTIVE_TYPES))
                #undef OMPU_DEF_COMMA
            ) [_a = qi::_1]

            > (directive_ | blob_)

            > qi::lit(_a)
        ; // directive

        #undef OMPU_DEF
    }

private:
    template<class... Ts>
    using make_rule = boost::spirit::qi::rule<in_iterator_type, Ts...>;

    using blank_type = boost::spirit::qi::standard::blank_type;


    make_rule<char()>
    tone_height_, chord_frag_minor_;

    make_rule<std::string()>
    chord_frag_7th_, chord_frag_tension_;


    make_rule<music::concrete::Chord(), boost::spirit::qi::locals<std::vector<detail::chord_specs>>, blank_type>
    chord_;

    make_rule<std::string()>
    plain_string_;

    make_rule<score_data::Blob(), blank_type>
    blob_;

    make_rule<score_data::Directive(), blank_type, boost::spirit::qi::locals<std::string>>
    directive_;

    make_rule<score_data::Blob const*(), blank_type>
    blob_ref_;
};


ChordPro::ChordPro()
    : ChordPro::base_type(root_, "ChordPro")
    , impl_(root_)
{}



}}} // ompu
