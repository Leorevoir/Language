#ifndef C_STRING_H_
#define C_STRING_H_

#include <rune/macros.h>
#include <rune/memory.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

const_ static inline_ bool _is_digits_impl(const char *str, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }
    return len > 0;
}

const_ static inline_ bool _contains_char_impl(const char item, const char *container)
{
    for (size_t i = 0; container[i] != '\0'; ++i) {
        if (item == container[i]) {
            return true;
        }
    }
    return false;
}

const_ static inline_ bool _contains_string_impl(const char *item, const char *container)
{
    const size_t item_len = strlen(item);

    for (size_t i = 0; container[i] != '\0'; ++i) {
        if (strncmp(item, container + i, item_len) == 0) {
            return true;
        }
    }
    return false;
}

const_ static inline_ bool _contains_any_impl(const char *item, size_t len, const char **keywords)
{
    for (size_t i = 0; keywords[i] != NULL; ++i) {
        if (strlen(keywords[i]) == len && strncmp(item, keywords[i], len) == 0) {
            return true;
        }
    }
    return false;
}

#define contains_char(item, container) _contains_char_impl(item, container)
#define contains_str(item, container) _contains_string_impl(item, container)
#define contains_any(substr, len, keywords) _contains_any_impl(substr, len, keywords)

#define is_digits(str, len) _is_digits_impl(str, len)

static pure_ inline_ const char *_substring_impl(const char *str, const size_t start,
    const size_t end)
{
    const size_t len = (end) - (start) + 1;
    char *sub_str;

    allocate(sub_str, alignof(char), len + 1);
    strncpy(sub_str, (str) + (start), len);
    sub_str[len] = '\0';
    return sub_str;
}

#define substring(str, start, end) _substring_impl(str, start, end);

#endif /* C_STRING_H_ */
