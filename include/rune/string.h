#ifndef C_STRING_H_
#define C_STRING_H_

#include <rune/macros.h>
#include <rune/memory.h>

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static const_ inline_ bool _is_digits_impl(const char *str)
{
    while (*str) {
        if (*str < '0' || *str > '9') {
            return false;
        }
        str++;
    }
    return true;
}

static const_ inline_ bool _contains_char_impl(const char item, const char *container)
{
    for (size_t i = 0; container[i] != '\0'; ++i) {
        if (item == container[i]) {
            return true;
        }
    }
    return false;
}

static const_ inline_ bool _contains_string_impl(const char *item, const char *container)
{
    const size_t item_len = strlen(item);

    for (size_t i = 0; container[i] != '\0'; ++i) {
        if (strncmp(item, container + i, item_len) == 0) {
            return true;
        }
    }
    return false;
}

static const_ inline_ bool _contains_any_impl(const char *item, const char **container)
{
    for (size_t i = 0; container[i] != NULL; ++i) {
        if (_contains_string_impl(item, container[i])) {
            return true;
        }
    }
    return false;
}

#define contains_char(item, container) _contains_char_impl(item, container)
#define contains_str(item, container) _contains_string_impl(item, container)
#define contains_any(item, container) _contains_any_impl(item, container)

#define is_digits(str) _is_digits_impl(str)

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
