#!/usr/bin/env python3
#
# ChibiOS ROMFS image generator.
#

from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import re
import sys
from dataclasses import dataclass
from typing import NoReturn


C_KEYWORDS = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof",
    "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
    "_Static_assert", "_Thread_local",
}

IDENTIFIER_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
MANIFEST_FILENAME = "romfs.dynamic.json"


@dataclass(frozen=True)
class FileEntry:
    name: str
    mode: str
    flags: str
    size: str
    data: bytes | None = None
    ops: str | None = None
    arg: str | None = None


@dataclass(frozen=True)
class DirEntry:
    path: str
    files: tuple[FileEntry, ...]


@dataclass(frozen=True)
class DynamicManifest:
    includes: tuple[str, ...]
    externs: tuple[str, ...]
    files: tuple[tuple[str, FileEntry], ...]


def fail(message: str) -> NoReturn:
    raise SystemExit(f"mkromfs: error: {message}")


def validate_namespace(path: Path) -> str:
    namespace = path.name
    if not namespace:
        fail("input directory must have a non-empty leaf name")
    if not IDENTIFIER_RE.match(namespace):
        fail(
            f"directory name '{namespace}' is not a valid C identifier, "
            "allowed form is [A-Za-z_][A-Za-z0-9_]*"
        )
    if namespace in C_KEYWORDS:
        fail(f"directory name '{namespace}' is a reserved C keyword")
    return namespace


def escape_c_string(text: str) -> str:
    out: list[str] = []
    for b in text.encode("utf-8"):
        if 32 <= b <= 126 and b not in (34, 92):
            out.append(chr(b))
        elif b == 34:
            out.append(r"\"")
        elif b == 92:
            out.append(r"\\")
        elif b == 9:
            out.append(r"\t")
        elif b == 10:
            out.append(r"\n")
        elif b == 13:
            out.append(r"\r")
        else:
            out.append(f"\\x{b:02x}")
    return '"' + "".join(out) + '"'


def format_data_array(data: bytes) -> str:
    lines: list[str] = []
    for offset in range(0, len(data), 12):
        chunk = data[offset:offset + 12]
        lines.append("  " + ", ".join(f"0x{byte:02x}" for byte in chunk))
    return ",\n".join(lines)


def posix_path(root: Path, entry: Path) -> str:
    relative = entry.relative_to(root)
    if str(relative) == ".":
        return "/"
    return "/" + relative.as_posix()


def join_path(dir_path: str, name: str) -> str:
    if dir_path == "/":
        return "/" + name
    return dir_path + "/" + name


def split_path(path: str) -> tuple[str, str]:
    if path == "/" or not path.startswith("/"):
        fail(f"invalid dynamic file path: {path!r}")
    dir_path, _, name = path.rpartition("/")
    if not name:
        fail(f"invalid dynamic file path: {path!r}")
    if not dir_path:
        dir_path = "/"
    return dir_path, name


def normalize_dynamic_path(raw_path: str) -> str:
    if not isinstance(raw_path, str):
        fail("dynamic entry path must be a string")
    if not raw_path.startswith("/"):
        fail(f"dynamic entry path must be absolute: {raw_path!r}")
    if "\\" in raw_path:
        fail(f"dynamic entry path must use '/' separators: {raw_path!r}")

    parts = raw_path.split("/")
    normalized: list[str] = []
    for part in parts[1:]:
        if part in ("", ".", ".."):
            fail(f"invalid dynamic entry path component in {raw_path!r}")
        if part == MANIFEST_FILENAME:
            fail(f"dynamic entry path cannot use reserved name {MANIFEST_FILENAME!r}")
        normalized.append(part)

    if not normalized:
        fail("dynamic entry path cannot refer to the root directory")

    return "/" + "/".join(normalized)


def normalize_c_expr(value: object, field_name: str, *, default: str | None = None) -> str:
    if value is None:
        if default is None:
            fail(f"missing required field {field_name!r}")
        return default
    if isinstance(value, int):
        return str(value)
    if not isinstance(value, str):
        fail(f"field {field_name!r} must be a string or integer")
    expr = value.strip()
    if not expr:
        fail(f"field {field_name!r} cannot be empty")
    return expr


def normalize_string_list(values: object, field_name: str) -> tuple[str, ...]:
    if values is None:
        return ()
    if not isinstance(values, list):
        fail(f"field {field_name!r} must be a list")

    normalized: list[str] = []
    for index, value in enumerate(values):
        if not isinstance(value, str):
            fail(f"field {field_name!r}[{index}] must be a string")
        text = value.strip()
        if not text:
            fail(f"field {field_name!r}[{index}] cannot be empty")
        normalized.append(text)
    return tuple(normalized)


def file_mode(entry: Path) -> str:
    mode = "VFS_MODE_S_IRUSR"
    if os.access(entry, os.X_OK):
        mode += " | VFS_MODE_S_IXUSR"
    return mode


def render_file_flags(file_entry: FileEntry) -> str:
    if file_entry.ops is None:
        return file_entry.flags
    return f"({file_entry.flags}) | VFS_ROMFS_FILE_TYPE_DYNAMIC"


def scan_dir(root: Path, current: Path) -> list[DirEntry]:
    subdirs: list[Path] = []
    files: list[FileEntry] = []

    for entry in sorted(current.iterdir(), key=lambda item: item.name):
        if current == root and entry.name == MANIFEST_FILENAME:
            continue
        if entry.is_symlink():
            fail(f"symbolic links are not supported: {entry}")
        if entry.is_dir():
            subdirs.append(entry)
        elif entry.is_file():
            data = entry.read_bytes()
            files.append(
                FileEntry(
                    name=entry.name,
                    mode=file_mode(entry),
                    flags="0U",
                    size=str(len(data)),
                    data=data,
                    ops=None,
                    arg="NULL",
                )
            )
        else:
            fail(f"unsupported filesystem object: {entry}")

    entries = [
        DirEntry(path=posix_path(root, current), files=tuple(files))
    ]
    for subdir in subdirs:
        entries.extend(scan_dir(root, subdir))
    return entries


def load_dynamic_manifest(root: Path) -> DynamicManifest:
    manifest_path = root / MANIFEST_FILENAME
    if not manifest_path.exists():
        return DynamicManifest(includes=(), externs=(), files=())
    if not manifest_path.is_file():
        fail(f"manifest path is not a regular file: {manifest_path}")

    try:
        raw = json.loads(manifest_path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        fail(f"invalid JSON in {manifest_path}: {exc}")

    if not isinstance(raw, dict):
        fail(f"{manifest_path} must contain a JSON object")

    includes = normalize_string_list(raw.get("includes"), "includes")
    externs = normalize_string_list(raw.get("externs"), "externs")

    raw_dynamic = raw.get("dynamic", [])
    if not isinstance(raw_dynamic, list):
        fail("field 'dynamic' must be a list")

    files: list[tuple[str, FileEntry]] = []
    for index, item in enumerate(raw_dynamic):
        if not isinstance(item, dict):
            fail(f"field 'dynamic'[{index}] must be an object")

        path = normalize_dynamic_path(item.get("path"))
        _, name = split_path(path)
        files.append(
            (
                path,
                FileEntry(
                    name=name,
                    mode=normalize_c_expr(item.get("mode"), "mode", default="VFS_MODE_S_IRUSR"),
                    flags=normalize_c_expr(item.get("flags"), "flags", default="0U"),
                    size=normalize_c_expr(item.get("size"), "size", default="0"),
                    data=None,
                    ops=normalize_c_expr(item.get("ops"), "ops"),
                    arg=normalize_c_expr(item.get("arg"), "arg", default="NULL"),
                ),
            )
        )

    return DynamicManifest(
        includes=includes,
        externs=externs,
        files=tuple(files),
    )


def merge_dirs(static_dirs: list[DirEntry], manifest: DynamicManifest) -> list[DirEntry]:
    existing_dirs = {directory.path for directory in static_dirs}
    static_file_paths = {
        join_path(directory.path, file_entry.name)
        for directory in static_dirs
        for file_entry in directory.files
    }
    merged: dict[str, list[FileEntry]] = {
        directory.path: list(directory.files)
        for directory in static_dirs
    }
    dynamic_paths: set[str] = set()

    for path, file_entry in manifest.files:
        dir_path, _ = split_path(path)
        if dir_path not in existing_dirs:
            fail(
                f"dynamic entry parent directory does not exist in source tree: "
                f"{path!r}"
            )
        if path in existing_dirs:
            fail(f"dynamic entry collides with directory path: {path!r}")
        if path in static_file_paths:
            fail(f"dynamic entry collides with static file path: {path!r}")
        if path in dynamic_paths:
            fail(f"duplicate dynamic entry path: {path!r}")

        dynamic_paths.add(path)
        merged[dir_path].append(file_entry)

    result: list[DirEntry] = []
    for directory in static_dirs:
        files = tuple(sorted(merged[directory.path], key=lambda item: item.name))
        result.append(DirEntry(path=directory.path, files=files))
    return result


def format_include(header: str) -> str:
    if header.startswith("<") and header.endswith(">"):
        return f"#include {header}"
    return f'#include "{header}"'


def emit_header(namespace: str) -> str:
    guard = f"{namespace.upper()}_ROMFS_H"

    return f"""/*
    Generated by tools/mkromfs/mkromfs.py. Do not edit manually.
*/

#ifndef {guard}
#define {guard}

#include "vfs.h"

#ifdef __cplusplus
extern "C" {{
#endif

extern const vfs_romfs_tree_t {namespace}_romfs;

#ifdef __cplusplus
}}
#endif

#endif /* {guard} */
"""


def emit_source(
    namespace: str,
    header_name: str,
    dirs: list[DirEntry],
    manifest: DynamicManifest,
) -> str:
    lines: list[str] = []

    lines.append("/*")
    lines.append("    Generated by tools/mkromfs/mkromfs.py. Do not edit manually.")
    lines.append("*/")
    lines.append("")
    lines.append(f'#include "{header_name}"')
    for include in manifest.includes:
        lines.append(format_include(include))
    lines.append("")

    dynamic_ops = sorted(
        {
            file_entry.ops
            for directory in dirs
            for file_entry in directory.files
            if file_entry.ops is not None
        }
    )
    for ops in dynamic_ops:
        lines.append(f"extern const vfs_romfs_dynamic_ops_t {ops};")
    for declaration in manifest.externs:
        lines.append(declaration)
    if dynamic_ops or manifest.externs:
        lines.append("")

    for dir_index, directory in enumerate(dirs):
        for file_index, file_entry in enumerate(directory.files):
            if file_entry.data:
                lines.append(
                    f"static const uint8_t {namespace}_file_{dir_index}_{file_index}_data[] = {{"
                )
                lines.append(format_data_array(file_entry.data))
                lines.append("};")
                lines.append("")

        lines.append(
            f"static const vfs_romfs_file_desc_t {namespace}_dir_{dir_index}_files[] = {{"
        )
        if directory.files:
            for file_index, file_entry in enumerate(directory.files):
                data_symbol = "NULL"
                if file_entry.data:
                    data_symbol = f"{namespace}_file_{dir_index}_{file_index}_data"
                flags_expr = render_file_flags(file_entry)

                lines.append("  {")
                lines.append(f"    .name = {escape_c_string(file_entry.name)},")
                lines.append(f"    .mode = {file_entry.mode},")
                lines.append(f"    .flags = {flags_expr},")
                lines.append(f"    .size = (vfs_offset_t){file_entry.size},")
                if file_entry.ops is None:
                    lines.append(f"    .content = {{ .data = {data_symbol} }},")
                else:
                    lines.append("    .content = {")
                    lines.append("      .dynamic = {")
                    lines.append(f"        .ops = &{file_entry.ops},")
                    lines.append(f"        .arg = {file_entry.arg},")
                    lines.append("      },")
                    lines.append("    },")
                lines.append("  },")
        lines.append("};")
        lines.append("")

    lines.append(f"static const vfs_romfs_dir_desc_t {namespace}_dirs[] = {{")
    for dir_index, directory in enumerate(dirs):
        lines.append("  {")
        lines.append(f"    .path = {escape_c_string(directory.path)},")
        lines.append(f"    .files = {namespace}_dir_{dir_index}_files,")
        lines.append(f"    .files_num = {len(directory.files)}U,")
        lines.append("  },")
    lines.append("};")
    lines.append("")
    lines.append(f"const vfs_romfs_tree_t {namespace}_romfs = {{")
    lines.append(f"  .dirs = {namespace}_dirs,")
    lines.append(f"  .dirs_num = {len(dirs)}U,")
    lines.append("};")
    lines.append("")

    return "\n".join(lines)


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate a ChibiOS ROMFS C image from a directory tree."
    )
    parser.add_argument("input_dir", help="Source directory to convert.")
    parser.add_argument(
        "output_dir",
        nargs="?",
        default=".",
        help="Destination directory for the generated .c/.h files.",
    )
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)

    input_dir = Path(args.input_dir).resolve()
    if not input_dir.exists():
        fail(f"input directory does not exist: {input_dir}")
    if not input_dir.is_dir():
        fail(f"input path is not a directory: {input_dir}")

    namespace = validate_namespace(input_dir)
    output_dir = Path(args.output_dir).resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    static_dirs = sorted(scan_dir(input_dir, input_dir), key=lambda item: item.path)
    manifest = load_dynamic_manifest(input_dir)
    dirs = merge_dirs(static_dirs, manifest)
    header_name = f"{namespace}_romfs.h"
    source_name = f"{namespace}_romfs.c"

    header_path = output_dir / header_name
    source_path = output_dir / source_name

    header_path.write_text(emit_header(namespace), encoding="utf-8", newline="\n")
    source_path.write_text(
        emit_source(namespace, header_name, dirs, manifest),
        encoding="utf-8",
        newline="\n",
    )

    print(header_path)
    print(source_path)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
