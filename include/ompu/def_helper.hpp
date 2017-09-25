// #pragma once

#define OMPU_DEF_HELPER_EXTRACTOR_TYPE(t) \
    BOOST_PP_TUPLE_ELEM(0, t)

#define OMPU_DEF_HELPER_EXTRACTOR_NAME(t) \
    BOOST_PP_TUPLE_ELEM(1, t)


#define OMPU_DEF_HELPER_EXTRACTOR_SINGLE_COMMA(r, data, elem) \
    elem ,

#define OMPU_DEF_HELPER_EXTRACTOR_STRINGIZED_ENUM_OSTREAM_TABLE(r, class_name, enum_elem) \
    BOOST_PP_STRINGIZE(class_name) "::" BOOST_PP_STRINGIZE(enum_elem) ,


#define OMPU_DEF_HELPER_EXTRACTOR_SPACED(r, data, elem) \
    (OMPU_DEF_HELPER_EXTRACTOR_TYPE(elem) OMPU_DEF_HELPER_EXTRACTOR_NAME(elem))

#define OMPU_DEF_HELPER_EXTRACTOR_SPACED_WITH_SEMICOLON(r, data, elem) \
    OMPU_DEF_HELPER_EXTRACTOR_TYPE(elem) OMPU_DEF_HELPER_EXTRACTOR_NAME(elem) ;


#define OMPU_DEF_HELPER_EXTRACT_ENUM(extractor, seq) \
    BOOST_PP_SEQ_ENUM( \
        BOOST_PP_SEQ_FOR_EACH(extractor, _, seq) \
    )

#define OMPU_DEF_HELPER_EXTRACT_VARIABLES(seq) \
    BOOST_PP_SEQ_FOR_EACH(OMPU_DEF_HELPER_EXTRACTOR_SPACED_WITH_SEMICOLON, _, seq)


#define OMPU_DEF_HELPER_EXTRACT_CLASS_TRAITS(class_name) \
    class_name() = default; \
    ~class_name() = default; \
    \
    class_name(class_name const&) = default; \
    class_name(class_name&&) noexcept = default; \
    \
    class_name& operator=(class_name const&) = default; \
    class_name& operator=(class_name&&) noexcept = default;

#define OMPU_DEF_HELPER_EXTRACT_ENUM_CLASS(class_name, def) \
    enum class class_name : unsigned \
    { \
        Invalid = 0, \
        BOOST_PP_SEQ_FOR_EACH(OMPU_DEF_HELPER_EXTRACTOR_SINGLE_COMMA, _, def) \
    }; \
    \
    std::ostream& operator<<(std::ostream& os, class_name const& e);

#define OMPU_DEF_HELPER_EXTRACT_ENUM_CLASS_IMPL(class_name, def) \
    std::ostream& operator<<(std::ostream& os, class_name const& e) \
    { \
        static constexpr char const* TABLE[] = { \
            BOOST_PP_STRINGIZE(class_name) "::" "Invalid", \
            BOOST_PP_SEQ_FOR_EACH(OMPU_DEF_HELPER_EXTRACTOR_STRINGIZED_ENUM_OSTREAM_TABLE, class_name, def) \
        }; \
        return os << TABLE[static_cast<std::underlying_type_t<class_name>>(e)]; \
    }


#define OMPU_MIDI_MESSAGE_DEF \
    ((MessageType, type)) \
    ((ComponentID, src_id)) \
    ((unsigned short, ch)) \
    ((unsigned short, note_num))

#define OMPU_MIDI_MESSAGE_TYPE_DEF \
    (NoteOn)(NoteOff)

#define OMPU_MIDI_SYSEX_DEF \
    ((Message, msg))
