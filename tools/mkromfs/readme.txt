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

  compression
    Optional compression policy for static files. The policy is evaluated on
    normalized ROMFS paths using deterministic exact path, directory prefix
    and filename suffix selectors.

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
  "compression": {
    "default": {
      "mode": "off"
    },
    "rules": [
      {
        "dir": "/web",
        "mode": "off"
      },
      {
        "suffix": ".html",
        "mode": "auto"
      },
      {
        "path": "/index.html",
        "mode": "force"
      }
    ]
  },
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


Compression Policy
------------------

The optional "compression" object controls how static files are emitted.

Supported fields:

  default
    Optional object containing:

      mode
        Default policy applied when no rule matches.
        Supported values: off, force, auto
        Default: off

  rules
    Optional ordered list of rule objects.

Each rule must contain exactly one selector:

  path
    Exact absolute ROMFS path, for example "/index.html".

  dir
    Absolute ROMFS directory prefix, for example "/assets" or "/".
    A rule with dir "/assets" matches files under "/assets/...".

  suffix
    Filename or path suffix without path separators, for example ".html".

Rule objects may also contain:

  mode
    Policy selected by the rule.
    Supported values: off, force, auto
    Default: compression.default.mode

Rule resolution is deterministic:

  - Exact "path" rules have higher precedence than "dir" and "suffix" rules.
  - Within the same selector class, the last matching rule wins.
  - If no rule matches, compression.default.mode is used.

Rules are evaluated on normalized ROMFS paths only. Matching is
case-sensitive and always uses '/' separators, independent from the host
platform.

Current implementation:

  - The tool currently supports a chunked PackBits stream for compressed
    static files.
  - Compression is applied independently on fixed 256-byte chunks.
  - For each compressed file the generated C source emits:
      - a compressed payload array
      - a chunk offset table
      - a chunk descriptor
      - a file descriptor referencing the ROMFS compressed backend ops table
  - "force" always emits the compressed representation.
  - "auto" emits the compressed representation only if:
      compressed_payload_size + chunk_offset_table_size < raw_file_size
    otherwise the file is emitted as a normal raw ROMFS file.

Notes:

  - Compression selection only applies to static files.
  - Dynamic entries are never compressed by mkromfs.
  - The generated ROMFS image can contain a mix of raw and compressed files.
  - Runtime support for compressed files in the ROMFS driver must be enabled
    through DRV_CFG_ROM_ENABLE_COMPRESSION. If support is disabled, opening a
    compressed file fails cleanly.


Validation Rules
----------------

The tool rejects:

  - Non-absolute dynamic paths.
  - Dynamic paths containing empty components, "." or "..".
  - Duplicate dynamic paths.
  - Dynamic paths colliding with static files.
  - Dynamic paths colliding with directories.
  - Dynamic paths whose parent directory does not exist in the source tree.
  - Compression rules not using exactly one of "path", "dir" or "suffix".
  - Compression modes outside: off, force, auto.

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
