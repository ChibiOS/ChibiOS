mkromfs
=======

Purpose
-------

The mkromfs tool converts a host directory tree into a ChibiOS ROMFS image
made of C source and header files.

Static files are taken directly from the input directory tree.

Dynamic files can be declared through a reserved settings manifest named
"romfs_settings.json" placed in the root of the input directory. The manifest
is tool metadata only and is not emitted as a ROMFS file.


Static Files
------------

Every regular file in the source tree is emitted as a static ROMFS file.

Subdirectories are emitted as ROMFS directories. Empty directories are kept in
the generated tree and can be used as parents for dynamic files.

Symbolic links are not supported.


ROMFS Settings Manifest
-----------------------

The reserved manifest filename is:

  romfs_settings.json

The file must contain a JSON object. Supported top-level fields are:

  dynamic
    List of dynamic file entries.

  includes
    Optional list of headers to include in the generated .c file.
    Each item can be either a plain header name such as "proc_dyn.h" or a
    bracket form such as "<my/header.h>".

  externs
    Optional list of raw C extern declarations to copy verbatim into the
    generated .c file.

Example:

{
  "includes": ["proc_dyn.h"],
  "externs": [
    "extern const proc_file_arg_t proc_version_arg;"
  ],
  "dynamic": [
    {
      "path": "/proc/version",
      "ops": "proc_version_ops",
      "arg": "&proc_version_arg",
      "mode": "VFS_MODE_S_IRUSR",
      "flags": "0U",
      "size": "0"
    },
    {
      "path": "/proc/uptime",
      "ops": "proc_uptime_ops"
    }
  ]
}


Dynamic Entry Fields
--------------------

Each element of "dynamic" must be an object with:

  path
    Required. Absolute ROMFS path of the dynamic file.

  ops
    Required. Name of a vfs_romfs_dynamic_ops_t symbol.

Optional fields:

  arg
    C expression used for the descriptor arg field.
    Default: NULL

  mode
    C expression used for the descriptor mode field.
    Default: VFS_MODE_S_IRUSR

  flags
    C expression used for the descriptor flags field.
    Default: 0U

  size
    C expression used for the descriptor size field.
    Default: 0

The generated C source automatically emits:

  extern const vfs_romfs_dynamic_ops_t <ops>;

for every dynamic entry "ops" symbol.

If "arg" references external symbols, add the necessary declarations through
"includes" or "externs".


Validation Rules
----------------

The tool rejects:

  - Non-absolute dynamic paths.
  - Dynamic paths containing empty components, "." or "..".
  - Duplicate dynamic paths.
  - Dynamic paths colliding with static files.
  - Dynamic paths colliding with directories.
  - Dynamic paths whose parent directory does not exist in the source tree.

Parent directories for dynamic files must exist as real directories in the
input tree. For example, a dynamic file "/proc/uptime" requires a "proc/"
directory in the source tree.


Intended Use
------------

Dynamic ROMFS entries are intended for synthetic read-only files such as:

  - /proc-style runtime information
  - sandbox-visible status files
  - generated diagnostics snapshots

They are not intended to carry HTTP transaction state. Request-dependent web
pages should be handled by the HTTP layer, for example through CGI or SSI.
