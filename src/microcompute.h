#pragma once

#include <stdbool.h>
#include <stdlib.h>

//============================================================================//
// Types
//============================================================================//

/**
 * Vector types, compatible with glsl vectors.
 */

typedef struct mc_vec2 {
	float x, y;
} mc_vec2;

typedef struct mc_vec3 {
	float x, y, z;
} mc_vec3;

typedef struct mc_vec4 {
	float x, y, z, w;
} mc_vec4;

typedef struct mc_ivec2 {
	int x, y;
} mc_ivec2;

typedef struct mc_ivec3 {
	int x, y, z;
} mc_ivec3;

typedef struct mc_ivec4 {
	int x, y, z, w;
} mc_ivec4;

typedef struct mc_uvec2 {
	unsigned int x, y;
} mc_uvec2;

typedef struct mc_uvec3 {
	unsigned int x, y, z;
} mc_uvec3;

typedef struct mc_uvec4 {
	unsigned int x, y, z, w;
} mc_uvec4;

/**
 * Matrix types, compatible with glsl matrices.
 * If `transpose` is set to true, the matrix values are in row major order,
 * else, the values are in column major order. It is passed as the `transpose`
 * argument to the `glUniformMatrix...()` functions.
 */

typedef struct mc_mat22 {
	float values[4];
	bool transpose;
} mc_mat22;

typedef struct mc_mat33 {
	float values[9];
	bool transpose;
} mc_mat33;

typedef struct mc_mat44 {
	float values[16];
	bool transpose;
} mc_mat44;

typedef struct mc_mat23 {
	float values[6];
	bool transpose;
} mc_mat23;

typedef struct mc_mat32 {
	float values[6];
	bool transpose;
} mc_mat32;

typedef struct mc_mat24 {
	float values[8];
	bool transpose;
} mc_mat24;

typedef struct mc_mat42 {
	float values[8];
	bool transpose;
} mc_mat42;

typedef struct mc_mat34 {
	float values[12];
	bool transpose;
} mc_mat34;

typedef struct mc_mat43 {
	float values[12];
	bool transpose;
} mc_mat43;

/**
 * Debug message severity level.
 * Passed as an argument to the debug callback function.
 */

typedef enum mc_DebugLevel {
	mc_DebugLevel_INFO,
	mc_DebugLevel_LOW,
	mc_DebugLevel_MEDIUM,
	mc_DebugLevel_HIGH,
} mc_DebugLevel;

/**
 * Opaque types for complex objects.
 */

typedef struct mc_Program mc_Program;
typedef struct mc_Buffer mc_Buffer;

//============================================================================//
// core
//============================================================================//

/**
 * Initialize the microcompute library. If any errors occur, it will call the
 * debug callback function, so `mc_set_debug_callback()` should be called before
 * calling `mc_start()`.
 *
 * `renderDevice` should be "/dev/dri/renderD...". If the machine the library is
 * being used on has multiple graphics cards, this value will determine which
 * card is used.
 *
 * - `renderDevice`
 *   The rendering device to be used by the library.
 * - returns
 *   `true` on success, `false` otherwise.
 */
bool mc_start(char *renderDevice);

/**
 * Stops the microcompute library.
 */
void mc_stop();

/**
 * Set the function to be called when a debug message is produced.
 *
 * `mc_set_debug_callback(mc_default_debug_callback, NULL)` can be called to
 * simply have all debug messages printed to `stdout`.
 *
 * - `callback`
 *   The function to be called. The `mc_DebugLevel` parameter indicates the
 *   severity of the message, the `char *` parameter contains the message. This
 *   parameter is managed by microcompute, so do not `free()` it, and the `void`
 *   parameter will contain whatever was passed to `arg`.
 * - `arg`
 *   A pointer to any data to pass onto the debug callback function. If set to
 * `NULL`, all messages will be printed.
 */
void mc_set_debug_callback(
	void (*callback)(mc_DebugLevel, char *, void *),
	void *arg
);

/**
 * The default debug callback function. This will print messages of any greater
 * than the value pointed to by `arg` to `stdout`. See `mc_set_debug_callback()`
 * for more info about the parameters.
 *
 * - `level`
 *   The severity of the debug message.
 * - `message`
 *   The debug message.
 * - `arg`
 *   A pointer to a value indicating the lowest severity message type to print.
 */
void mc_default_debug_callback(mc_DebugLevel level, char *message, void *arg);

//============================================================================//
// program management
//============================================================================//

/**
 * Create a program (compute shader) from a string.
 *
 * - `programCode`
 *   A string containing the program code.
 * - returns
 *   A reference to the program on success, `NULL` otherwise.
 */
mc_Program *mc_program_from_str(const char *programCode);

/**
 * Create a program (compute shader) from a file.
 *
 * args:
 * - `filePath`
 *   The path to the file containing the program code.
 * - returns
 *   A reference to the program on success, `NULL` otherwise.
 */
mc_Program *mc_program_from_file(const char *filePath);

/**
 * Destroy a program.
 *
 * - `program`
 *   The program to destroy.
 */
void mc_program_destroy(mc_Program *program);

/**
 * Dispatch (run) a program.
 *
 * - `program`
 *   The program to run.
 * - `size`
 *   The number of workgroups to be run in each dimension.
 */
void mc_program_dispatch(mc_Program *program, mc_ivec3 size);

//============================================================================//
// uniform value management
//============================================================================//

/**
 * Set the value of uniform value.
 *
 * - `program`
 *   The program in which to set the uniform value.
 * - `name`
 *   The name of the uniform to set.
 * - `value`
 *   The value of the uniform.
 * - returns
 *   `true` on success, `false` if the variable could not be found.
 */

bool mc_program_set_float(mc_Program *program, char *name, float value);
bool mc_program_set_vec2(mc_Program *program, char *name, mc_vec2 value);
bool mc_program_set_vec3(mc_Program *program, char *name, mc_vec3 value);
bool mc_program_set_vec4(mc_Program *program, char *name, mc_vec4 value);

bool mc_program_set_int(mc_Program *program, char *name, int value);
bool mc_program_set_ivec2(mc_Program *program, char *name, mc_ivec2 value);
bool mc_program_set_ivec3(mc_Program *program, char *name, mc_ivec3 value);
bool mc_program_set_ivec4(mc_Program *program, char *name, mc_ivec4 value);

bool mc_program_set_uint(mc_Program *program, char *name, unsigned int value);
bool mc_program_set_uvec2(mc_Program *program, char *name, mc_uvec2 value);
bool mc_program_set_uvec3(mc_Program *program, char *name, mc_uvec3 value);
bool mc_program_set_uvec4(mc_Program *program, char *name, mc_uvec4 value);

bool mc_program_set_mat22(mc_Program *program, char *name, mc_mat22 value);
bool mc_program_set_mat33(mc_Program *program, char *name, mc_mat33 value);
bool mc_program_set_mat44(mc_Program *program, char *name, mc_mat44 value);
bool mc_program_set_mat23(mc_Program *program, char *name, mc_mat23 value);
bool mc_program_set_mat32(mc_Program *program, char *name, mc_mat32 value);
bool mc_program_set_mat24(mc_Program *program, char *name, mc_mat24 value);
bool mc_program_set_mat42(mc_Program *program, char *name, mc_mat42 value);
bool mc_program_set_mat34(mc_Program *program, char *name, mc_mat34 value);
bool mc_program_set_mat43(mc_Program *program, char *name, mc_mat43 value);

//============================================================================//
// buffer management
//============================================================================//

/**
 * Create a buffer (SSBO).
 *
 * - `binding`
 *   The binding in which to store the buffer. Can be changed with
 *   `mc_buffer_rebind()`.
 * - `size`
 *   The size of the buffer. Can be changed with `mc_buffer_resize()`.
 * - returns
 *   A reference to the buffer on success, `NULL` otherwise.
 */
mc_Buffer *mc_buffer_create(int binding, size_t size);

/**
 * Destroy a buffer.
 *
 * - `buffer`
 *   The buffer to destroy.
 */
void mc_buffer_destroy(mc_Buffer *buffer);

/**
 * Rebind a buffer.
 *
 * - `buffer`
 *   The buffer to rebind.
 * - `binding`
 *   The (new) binding in which to store the buffer.
 */
void mc_buffer_rebind(mc_Buffer *buffer, int binding);

/**
 * Resize a buffer.
 *
 * - `buffer`
 *   The buffer to resize.
 * - `binding`
 *   The (new) size of the buffer.
 */
void mc_buffer_resize(mc_Buffer *buffer, size_t size);

/**
 * Get the current size of a buffer.
 *
 * - `buffer`
 *   The buffer to get the size of.
 * - returns
 *   The current size of the buffer.
 */
size_t mc_buffer_get_size(mc_Buffer *buffer);

/**
 * Write data to a buffer. If `off` + `size` is larger than the size of the
 * buffer, the function call will fail.
 *
 * - `buffer`
 *   The buffer to write to.
 * - `off`
 *   The offset at which to start writing data to. Measured in bytes.
 * - `size`
 *   The size (length) of the data to be written. Measured in bytes.
 * - `data`
 *   The data to write.
 * - returns
 *   The number of bytes written. `size` on success, `0` otherwise.
 */
size_t mc_buffer_write(mc_Buffer *buffer, size_t off, size_t size, void *data);

/**
 * Read data from a buffer. If `off` + `size` is larger than the size of the
 * buffer, the function call will fail.
 *
 * - `buffer`
 *   The buffer to read from.
 * - `off`
 *   The offset at which to start reading data from. Measured in bytes.
 * - `size`
 *   The size (length) of the data to be read. Measured in bytes.
 * - `data`
 *   A buffer to write the data into. Must be pre-allocated with enough space.
 * - returns
 *   The number of bytes read. `size` on success, `0` otherwise.
 */
size_t mc_buffer_read(mc_Buffer *buffer, size_t off, size_t size, void *data);