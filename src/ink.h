#ifndef INK_H
#define INK_H

#include "thirdparty/peglib/peglib.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

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

        struct AddressableData {
            std::vector<const char *> addresses;
            std::unordered_map<size_t, size_t> address_map; // address-index to data-index

            void bind_index_to_address(const char *p_address, size_t target_index) {
                addresses.push_back(std::strcpy(p_address));
                address_map[address.size()-1] = target_index;
            }
            size_t get_index_by_address(const char *p_address) {
                return addresses[address_map[p_address]];
            }

            ~AddressableData() {
                for (const char *an_address : addresses) {
                    if (an_address)
                        delete an_address;
                }
            }
        };

        typedef data_t size_t; // an index into the data vector
        typedef expv_t std::variant<int, float, const char *>; // an expression's allowed value type
        typedef labelseq_t std::vector<const char *>; // a sequence of labels

        struct LabelsNode {
            LabelsNode *parent;
            std::vector<LabelsNode *> children;
            std::vector<const char *> labels;
            std::unordered_map<size_t, data_t> labels_map; // label_idx -> data_idx
        };

        std::vector<std::string> data;                          // all story content
        std::vector<size_t> lines;                              // index data_t -> the line in which the data exists
        std::vector<data_t> texts;                              // narrative content
        std::vector<data_t> tags;                               // tag content
        std::vector<data_t> expressions;                        // expression strings
        std::unordered_map<data_t, expv_t> values;              // the values of expressions
        std::unordered_map<std::vector<data_t>, size_t> visits; // the number of times a label has been visited
        LabelsNode label_tree;                                  // 
        std::vector<Range> paragraphs; // glued together, continuous content
        std::vector<Range> statements; // idk

    public:
        Error parse_text(const char *p_text);
        Error parse_file(const char *p_path);
        Error parse_script(const InkScript &p_script);

        Error start();
        Error quit();
        Error advance();
        bool is_at_choice() const;
        Error choose(uint8_t p_option);
        std::string get_paragraph() const;
    };

}
#endif // INK_H