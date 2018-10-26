#ifndef INK_H
#define INK_H

#include "thirdparty/peglib/peglib.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace ink {

    enum Error {
        CALL_OK,
        ERR_INVALID_COMMAND,
        ERR_INVALID_CHOICE,
    };

    struct Range {
        uint64_t start;
        uint64_t end;
    }

    class Ink;

    class InkScript {
        friend class Ink;

        Ink *ink;
    protected:
        Ink *get_ink() const;
    public:
        Error new_paragraph();
        Error append_text(const char *p_text, size_t p_line);
        Error add_tag(const char *p_text, size_t p_line)

        Error load(const char *p_path);
        Error save(const char *p_path);

        InkScript(const Ink &p_ink);
    };

    class Ink {
        std::vector<std::string> data;
        std::vector<size_t> text;
        std::vector<size_t> tags;
        std::vector<size_t> expressions;
        std::vector<size_t> lines;
        std::vector<std::string> addresses; //may need to expand this for different knot, stitch, gather namespaces
        std::unordered_map<size_t, size_t> address_map;
        std::vector<Range> paragraphs;
        std::vector<Range> statements;

    public:
        Error parse_text(const char *p_text);
        Error parse_file(const char *p_path);
        Error parse_script(const InkScript &p_script);

        Error start();
        Error quit();
        Error advance();
        bool is_at_choice() const;
        Error choose(uint8_t p_option);
    };

}
#endif // INK_H