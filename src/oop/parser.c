#include "rune/memory.h"
#include <assert.h>
#include <rune/parser.h>
#include <stdbool.h>

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

/**
* public
*/

/**
* private
*/

static inline bool _does_exist(struct _IO *io)
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
    io->filename = va_arg(*args, char *);
    assert(_does_exist(io) == true);

    auto_free const char *buffer = _allocate_buffer(io);

    printf("buffer:\n_______\n%s", buffer);
}

static void parser_dtor(Object unused_ *self_ptr)
{
    /* __nothing__ */
}
