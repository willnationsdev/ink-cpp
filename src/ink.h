#ifndef INK_H
#define INK_H

#include "thirdparty/cpp-peglib/peglib.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <variant>
#include <sstream>

#define string_t std::string

namespace ink {

    enum Error {
        OK,
        ERR_INVALID_COMMAND,
        ERR_INVALID_CHOICE,
    };

    enum DataType {
        TEXT,
        PRETEXT,
        TAG,
        LABEL,
        EXPRESSION,
        SINGLE_CHOICE,
        MULTI_CHOICE,
        GATHER,
        KNOT,
        STITCH
    };

    class Ink {

        const uint8_t MAX_LABEL_LAYERS = 8;                             // How many levels label namespaces can have

        typedef std::pair<DataType, string_t *> data_t;                 // the type of the data and the string content it might have
        typedef size_t content_id_t;                                    // an index into the data vector
        typedef std::variant<int, float, const char *> expv_t;          // an expression's allowed value type

        std::vector<data_t> data;                                       // all story data
        std::vector<size_t> lines;                                      // The line in which the data exists. vector index == data index. 
        std::unordered_set<string_t> files;                             // files that source the data
        std::unordered_map<size_t, size_t> file_map;                    // The file in which the data exists. key == data index, value == file index.
        std::vector<size_t> texts;                                      // index for TEXT data
        std::vector<size_t> tags;                                       // index for TAG data
        std::vector<size_t> expressions;                                // index for EXPRESSiON data
        std::unordered_map<size_t, expv_t> values;                      // the values of expressions
        std::unordered_map<std::vector<size_t>, size_t> visits;         // the number of times a label has been visited
        std::vector<string_t *> labels;                                 // label hierarchy for namespaces

        std::stringstream stream;
        string_t file;
        size_t line;

    public:
        // parsing
        Error parse_text(const string_t &p_text);
        Error parse_file(const string_t &p_path);

        // generation
        const string_t &generate_text(Error *r_err = nullptr) const;

        // status operations
        void set_file(const string_t &p_path);
        const string_t &get_file() const;

        // load operations
        Error add_data(DataType p_type, const string_t &p_data, size_t line = -1);

        // story operations
        Error start();
        Error quit();
        Error pause();
        Error resume();
        Error reset();

        // navigation
        Error next();
        Error previous();
        bool is_at_choice() const;
        Error choose(uint8_t p_option);

        const string_t &get_text() const;

        Ink();
        ~Ink();
    };

}
#endif // INK_H