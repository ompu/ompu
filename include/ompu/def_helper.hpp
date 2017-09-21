// #pragma once

#define OMPU_DEF_HELPER_EXTRACTOR_TYPE(t) \
    BOOST_PP_TUPLE_ELEM(0, t)

#define OMPU_DEF_HELPER_EXTRACTOR_NAME(t) \
    BOOST_PP_TUPLE_ELEM(1, t)

#define OMPU_DEF_HELPER_EXTRACTOR_SPACED(r, data, elem) \
    (OMPU_DEF_HELPER_EXTRACTOR_TYPE(elem) OMPU_DEF_HELPER_EXTRACTOR_NAME(elem))

#define OMPU_DEF_HELPER_EXTRACTOR_SPACED_WITH_SEMICOLON(r, data, elem) \
    OMPU_DEF_HELPER_EXTRACTOR_TYPE(elem) OMPU_DEF_HELPER_EXTRACTOR_NAME(elem) ;

#define OMPU_DEF_HELPER_EXTRACTOR_INIT_I(r, data, elem) \
    ( \
        OMPU_DEF_HELPER_EXTRACTOR_NAME(elem) BOOST_PP_LPAREN() \
            std::move BOOST_PP_LPAREN() OMPU_DEF_HELPER_EXTRACTOR_NAME(elem) BOOST_PP_RPAREN() \
        BOOST_PP_RPAREN() \
    )

#define OMPU_DEF_HELPER_EXTRACT_ENUM(extractor, seq) \
    BOOST_PP_SEQ_ENUM( \
        BOOST_PP_SEQ_FOR_EACH(extractor, _, seq) \
    )

#define OMPU_DEF_HELPER_EXTRACT_PARAMS(seq) \
    OMPU_DEF_HELPER_EXTRACT_ENUM(OMPU_DEF_HELPER_EXTRACTOR_SPACED, seq)

#define OMPU_DEF_HELPER_EXTRACT_VARIABLES(seq) \
    BOOST_PP_SEQ_FOR_EACH(OMPU_DEF_HELPER_EXTRACTOR_SPACED_WITH_SEMICOLON, _, seq)

#define OMPU_DEF_HELPER_EXTRACT_INITIALIZER_LIST(seq) \
    : \
    BOOST_PP_SEQ_ENUM( \
        BOOST_PP_SEQ_FOR_EACH(OMPU_DEF_HELPER_EXTRACTOR_INIT_I, _, seq) \
    )


#define OMPU_DEF_HELPER_EXTRACT_CLASS_TRAITS(class_name) \
    ~class_name() = default; \
    \
    class_name(class_name const&) = default; \
    class_name(class_name&&) noexcept = default; \
    \
    class_name& operator=(class_name const&) = default; \
    class_name& operator=(class_name&&) noexcept = default;


#define OMPU_MIDI_MESSAGE_DEF \
    ((ComponentID, src_id)) \
    ((unsigned short, ch))

#define OMPU_MIDI_SYSEX_DEF \
    ((Message, msg))
