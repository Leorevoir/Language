#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#include <rune/memory.h>
#include <rune/parser.h>
#include <rune/string.h>
#include <rune/vector.h>

#ifndef C_STRDUP_DEFINED
    #define C_STRDUP_DEFINED
/**
* @brief C standard library signature bc idk why the hell gcc tells me its not defined
*/
char *strdup(const char *s);
char *strndup(const char *s, size_t n);
#endif /* C_STRDUP_DEFINED */

static void parser_ctor(Object *self_ptr, va_list *args);
static void parser_dtor(Object *self_ptr);

// clang-format off
static const Class vector_class = {
    .__size__ = sizeof(Parser),
    .__name__ = "Parser",
    .__ctor__ = parser_ctor,
    .__dtor__ = parser_dtor
};
// clang-format on

const_ const Class *Parser_getClass(void)
{
    return &vector_class;
}

// clang-format off
static const char *KEYWORDS[] = {
    "function",
    "if",
    "elif",
    "else",
    "for",
    "foreach",
    "then",
    "do"
};

static const char DELIMITERS[] = {' ', ',', ';', '(', ')', '[', ']', '{', '}', '\n', '\0'};

static const char OPERATORS[] = {'+', '-', '*', '/', '>', '<', '=', '%', '\0'};
// clang-format on

/*
 * helpers
 */

const_ static inline_ const char *token_type_to_string(enum _TokenType type)
{
    switch (type) {
        case TOKEN_EOF:
            return "TOKEN_EOF";
        case TOKEN_OPERATOR:
            return "TOKEN_OPERATOR";
        case TOKEN_DELIMITER:
            return "TOKEN_DELIMITER";
        case TOKEN_KEYWORD:
            return "TOKEN_KEYWORD";
        case TOKEN_IDENTIFIER:
            return "TOKEN_IDENTIFIER";
        case TOKEN_NUMBER:
            return "TOKEN_NUMBER";
        case TOKEN_INVALID:
            return "TOKEN_INVALID";
        default:
            return "UNKNOWN_TOKEN_TYPE";
    }
}

const_ static inline_ bool is_valid_identifier(const char *str, size_t len)
{
    if (len == 0 || isdigit(str[0]) || contains_char(str[0], DELIMITERS)) {
        return false;
    }
    return true;
}

static inline_ void push_token(Vector **out_tokens, const enum _TokenType type, const char *value,
    const size_t len)
{
    (*out_tokens)
        ->push_back(*out_tokens, &(struct _Token) {.type = type, .value = value, .length = len});
}

/**
 * public
 */

static void parser_collect_tokens(Parser *self)
{
    size_t left = 0;
    size_t right = 0;
    const struct _IO *io = &self->_io;

    while (right <= (size_t) io->st.st_size && left <= right) {

        /**
         * @brief skip over delimiters
         */
        if (!contains_char(io->buffer[right], DELIMITERS)) {
            ++right;
            continue;
        }

        /**
         * @brief if left and right are equal, it means we have a single token
         */
        if (left == right) {

            if (io->buffer[right] == '\0') {
                push_token(&self->_tokens, TOKEN_EOF, io->buffer + right, 0);
                break;
            }
            if (contains_char(io->buffer[right], OPERATORS)) {
                push_token(&self->_tokens, TOKEN_OPERATOR, io->buffer + right, 1);
            } else if (contains_char(io->buffer[right], DELIMITERS) && io->buffer[right] != ' '
                && io->buffer[right] != '\n') {
                push_token(&self->_tokens, TOKEN_DELIMITER, io->buffer + right, 1);
            }

            ++right;
            left = right;
            continue;
        }

        /**
         * @brief otherwise we have a big token (keyword, identifier, number, etc...)
         */
        const char *substr = io->buffer + left;
        const size_t len = right - left;

        if (contains_any(substr, len, KEYWORDS)) {
            push_token(&self->_tokens, TOKEN_KEYWORD, substr, len);
        } else if (is_digits(substr)) {
            push_token(&self->_tokens, TOKEN_NUMBER, substr, len);
        } else if (is_valid_identifier(substr, len)) {
            push_token(&self->_tokens, TOKEN_IDENTIFIER, substr, len);
        } else {
            push_token(&self->_tokens, TOKEN_INVALID, substr, len);
        }
        left = right;
    }

#if defined(DEBUG)
    vector_for_each(self->_tokens, struct _Token, __token, {
        auto_free char *value = strndup(__token->value, __token->length);
        printf("token: %s, type: %s\n", value, token_type_to_string(__token->type));
    });
#endif
}

/**
 * private
 */

const_ static inline_ bool _does_exist(struct _IO *io)
{
    return (stat(io->filename, &io->st) == 0 && S_ISREG(io->st.st_mode));
}

pure_ static inline_ FILE *_allocate_io(const char *filename)
{
    FILE *file = fopen(filename, "r");
    assert(file != NULL);
    return file;
}

pure_ static inline_ const char *_allocate_buffer(struct _IO *io)
{
    auto_fclose FILE *file = _allocate_io(io->filename);
    char *buffer;
    allocate(buffer, alignof(char), (size_t) io->st.st_size + 1);
    const size_t br = fread(buffer, sizeof(char), (size_t) io->st.st_size, file);
    assert(br != 0 && !feof(file));
    buffer[br] = '\0';
    return buffer;
}

static void parser_ctor(Object *self_ptr, va_list *args)
{
    Parser *self = (Parser *) self_ptr;
    struct _IO *io = &self->_io;

    self->class = Parser_getClass();
    self->collect_tokens = parser_collect_tokens;
    io->filename = va_arg(*args, char *);
    assert(_does_exist(io) == true);

    io->buffer = _allocate_buffer(io);
    self->_tokens = (Vector *) new (Vector_getClass(), sizeof(struct _Token), 32, NULL);

    printf("buffer:\n_______\n%s", io->buffer);
}

static void parser_dtor(Object *self_ptr)
{
    auto_free const char *buffer = (const char *) ((Parser *) self_ptr)->_io.buffer;
    delete ((Vector *) ((Parser *) self_ptr)->_tokens);
}
