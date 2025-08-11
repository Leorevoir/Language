#include <assert.h>
#include <stdbool.h>

#include <rune/parser.h>
#include <rune/string.h>

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
    "if"
    "elif"
    "else"
    "for"
    "foreach"
    "then"
    "do"
};

static const char  DELIMITERS[] = {' ', ',', ';', '(', ')', '[', ']', '{', '}', '\0'};

static const char  OPERATORS[] = {'+', '-', '*', '/', '>', '<', '=', '%', '\0'};

// clang-format on

/**
* public
*/

static void parser_collect_tokens(Parser *self)
{
    size_t left = 0;
    size_t right = 0;
    const struct _IO *io = &self->_io;

    while (right <= (size_t) self->_io.st.st_size && left <= right) {

        if (!contains_char(io->buffer[right], DELIMITERS)) {
            ++right;
            continue;
        }

        if (left == right) {
            if (contains_char(io->buffer[right], OPERATORS)) {
                //add token TOKEN_OPERATOR
            } else if (contains_char(io->buffer[right], DELIMITERS) && io->buffer[right] != ' ') {
                //add token TOKEN_DELIMITER
            }

            ++right;
            left = right;
            continue;
        }

        auto_free const char *substr = substring(io->buffer, left, right - 1);

        if (contains_any(substr, KEYWORDS)) {
            // add token TOKEN_KEYWORD
        } else if (contains_str(substr, OPERATORS)) {
            // add token TOKEN_NUMBER
        } else if (contains_str(substr, DELIMITERS)) {
            // add token TOKEN_IDENTIFIER
        } else {
            // add token TOKEN_INVALID
        }
        left = right;
    }
}

/**
* private
*/

const_ static inline bool _does_exist(struct _IO *io)
{
    return (stat(io->filename, &io->st) == 0 && S_ISREG(io->st.st_mode));
}

pure_ static inline FILE *_allocate_io(const char *filename)
{
    FILE *file = fopen(filename, "r");

    assert(file != NULL);
    return file;
}

pure_ static inline char *_allocate_buffer(struct _IO *io)
{
    auto_fclose FILE *file = _allocate_io(io->filename);
    char *buffer;

    allocate(buffer, alignof(buffer), (size_t) io->st.st_size + 1);
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
