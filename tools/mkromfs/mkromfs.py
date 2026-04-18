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
MANIFEST_FILENAME = "romfs_settings.json"


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
    compression: "CompressionSettings"


@dataclass(frozen=True)
class CompressionRule:
    selector_kind: str
    selector_value: str
    mode: str


@dataclass(frozen=True)
class CompressionSettings:
    default_mode: str
    rules: tuple[CompressionRule, ...]


@dataclass(frozen=True)
class StaticPayload:
    kind: str
    flags_expr: str
    size_expr: str
    raw_data: bytes | None = None
    compressed_data: bytes | None = None
    chunk_offsets: tuple[int, ...] | None = None
    chunk_size: int = 0


COMPRESSION_MODES = {"off", "force", "auto"}
COMPRESSED_KIND_RAW = "raw"
COMPRESSED_KIND_PACKBITS = "packbits"
PACKBITS_CHUNK_SIZE = 256
PACKBITS_OPS_SYMBOL = "vfs_romfs_chunked_packbits_ops"


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


def normalize_dir_path(raw_path: object, field_name: str) -> str:
    if not isinstance(raw_path, str):
        fail(f"field {field_name!r} must be a string")
    if not raw_path.startswith("/"):
        fail(f"field {field_name!r} must be absolute: {raw_path!r}")
    if "\\" in raw_path:
        fail(f"field {field_name!r} must use '/' separators: {raw_path!r}")

    parts = raw_path.split("/")
    normalized: list[str] = []
    for part in parts[1:]:
        if part in ("", "."):
            continue
        if part == "..":
            fail(f"invalid directory path component in {raw_path!r}")
        if part == MANIFEST_FILENAME:
            fail(f"directory path cannot use reserved name {MANIFEST_FILENAME!r}")
        normalized.append(part)

    if not normalized:
        return "/"
    return "/" + "/".join(normalized)


def normalize_suffix(value: object, field_name: str) -> str:
    if not isinstance(value, str):
        fail(f"field {field_name!r} must be a string")
    suffix = value.strip()
    if not suffix:
        fail(f"field {field_name!r} cannot be empty")
    if "/" in suffix or "\\" in suffix:
        fail(f"field {field_name!r} must not contain path separators")
    return suffix


def normalize_compression_mode(value: object, field_name: str, *, default: str) -> str:
    if value is None:
        return default
    if not isinstance(value, str):
        fail(f"field {field_name!r} must be a string")
    mode = value.strip().lower()
    if mode not in COMPRESSION_MODES:
        fail(
            f"field {field_name!r} must be one of: "
            + ", ".join(sorted(COMPRESSION_MODES))
        )
    return mode


def load_compression_settings(raw: object) -> CompressionSettings:
    if raw is None:
        return CompressionSettings(default_mode="off", rules=())
    if not isinstance(raw, dict):
        fail("field 'compression' must be an object")

    raw_default = raw.get("default")
    if raw_default is None:
        default_mode = "off"
    else:
        if not isinstance(raw_default, dict):
            fail("field 'compression.default' must be an object")
        default_mode = normalize_compression_mode(
            raw_default.get("mode"),
            "compression.default.mode",
            default="off",
        )

    raw_rules = raw.get("rules", [])
    if not isinstance(raw_rules, list):
        fail("field 'compression.rules' must be a list")

    rules: list[CompressionRule] = []
    for index, item in enumerate(raw_rules):
        if not isinstance(item, dict):
            fail(f"field 'compression.rules'[{index}] must be an object")

        selectors: list[tuple[str, str]] = []
        if "path" in item:
            selectors.append(
                ("path", normalize_dynamic_path(item.get("path")))
            )
        if "dir" in item:
            selectors.append(
                ("dir", normalize_dir_path(item.get("dir"), f"compression.rules[{index}].dir"))
            )
        if "suffix" in item:
            selectors.append(
                ("suffix", normalize_suffix(item.get("suffix"), f"compression.rules[{index}].suffix"))
            )
        if len(selectors) != 1:
            fail(
                f"field 'compression.rules'[{index}] must contain exactly one of "
                "'path', 'dir' or 'suffix'"
            )

        selector_kind, selector_value = selectors[0]
        rules.append(
            CompressionRule(
                selector_kind=selector_kind,
                selector_value=selector_value,
                mode=normalize_compression_mode(
                    item.get("mode"),
                    f"compression.rules[{index}].mode",
                    default=default_mode,
                ),
            )
        )

    return CompressionSettings(default_mode=default_mode, rules=tuple(rules))


def compression_rule_matches(rule: CompressionRule, path: str) -> bool:
    if rule.selector_kind == "path":
        return path == rule.selector_value
    if rule.selector_kind == "dir":
        if rule.selector_value == "/":
            return True
        return path.startswith(rule.selector_value + "/")
    if rule.selector_kind == "suffix":
        return path.endswith(rule.selector_value)
    fail(f"unsupported compression selector kind: {rule.selector_kind}")


def resolve_compression_mode(path: str, settings: CompressionSettings) -> str:
    path_mode: str | None = None
    general_mode: str | None = None

    for rule in settings.rules:
        if not compression_rule_matches(rule, path):
            continue
        if rule.selector_kind == "path":
            path_mode = rule.mode
        else:
            general_mode = rule.mode

    if path_mode is not None:
        return path_mode
    if general_mode is not None:
        return general_mode
    return settings.default_mode


def get_repeated_run_size(data: bytes, offset: int) -> int:
    run_size = 1

    while ((offset + run_size) < len(data)) and (run_size < 128):
        if data[offset + run_size] != data[offset]:
            break
        run_size += 1

    return run_size


def encode_packbits_chunk(data: bytes) -> bytes:
    encoded = bytearray()
    offset = 0

    while offset < len(data):
        run_size = get_repeated_run_size(data, offset)
        if run_size >= 3:
            encoded.append(257 - run_size)
            encoded.append(data[offset])
            offset += run_size
            continue

        literal_start = offset
        literal_size = 0
        while offset < len(data):
            run_size = get_repeated_run_size(data, offset)
            if run_size >= 3:
                break
            if literal_size + run_size > 128:
                break
            offset += run_size
            literal_size += run_size

        encoded.append(literal_size - 1)
        encoded.extend(data[literal_start:literal_start + literal_size])

    return bytes(encoded)


def encode_packbits_file(data: bytes, chunk_size: int) -> tuple[bytes, tuple[int, ...]]:
    encoded = bytearray()
    offsets = [0]

    for offset in range(0, len(data), chunk_size):
        chunk = data[offset:offset + chunk_size]
        encoded.extend(encode_packbits_chunk(chunk))
        offsets.append(len(encoded))

    return bytes(encoded), tuple(offsets)


def get_effective_compressed_size(compressed_data: bytes, chunk_offsets: tuple[int, ...]) -> int:
    return len(compressed_data) + len(chunk_offsets) * 4


def build_static_payload(path: str,
                         file_entry: FileEntry,
                         settings: CompressionSettings) -> StaticPayload:
    mode = resolve_compression_mode(path, settings)

    if mode == "off":
        return StaticPayload(
            kind=COMPRESSED_KIND_RAW,
            flags_expr=file_entry.flags,
            size_expr=f"(vfs_offset_t){file_entry.size}",
            raw_data=file_entry.data,
        )

    if file_entry.data is None:
        fail(f"static compression requested for non-static file: {path!r}")

    compressed_data, chunk_offsets = encode_packbits_file(file_entry.data,
                                                          PACKBITS_CHUNK_SIZE)
    if mode == "auto":
        if get_effective_compressed_size(compressed_data, chunk_offsets) >= len(file_entry.data):
            return StaticPayload(
                kind=COMPRESSED_KIND_RAW,
                flags_expr=file_entry.flags,
                size_expr=f"(vfs_offset_t){file_entry.size}",
                raw_data=file_entry.data,
            )

    return StaticPayload(
        kind=COMPRESSED_KIND_PACKBITS,
        flags_expr=f"({file_entry.flags}) | VFS_ROMFS_FILE_TYPE_COMPRESSED",
        size_expr=f"(vfs_offset_t){file_entry.size}",
        compressed_data=compressed_data,
        chunk_offsets=chunk_offsets,
        chunk_size=PACKBITS_CHUNK_SIZE,
    )


def build_static_payloads(dirs: list[DirEntry],
                          settings: CompressionSettings) -> dict[tuple[int, int], StaticPayload]:
    payloads: dict[tuple[int, int], StaticPayload] = {}

    for dir_index, directory in enumerate(dirs):
        for file_index, file_entry in enumerate(directory.files):
            path = join_path(directory.path, file_entry.name)
            if file_entry.ops is not None:
                continue
            payloads[(dir_index, file_index)] = build_static_payload(path, file_entry, settings)

    return payloads


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
        return DynamicManifest(
            includes=(),
            externs=(),
            files=(),
            compression=CompressionSettings(default_mode="off", rules=()),
        )
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
    compression = load_compression_settings(raw.get("compression"))

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
        compression=compression,
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
    static_payloads = build_static_payloads(dirs, manifest.compression)

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
            payload = static_payloads.get((dir_index, file_index))
            if payload is None:
                continue

            if payload.kind == COMPRESSED_KIND_RAW:
                lines.append(
                    f"static const uint8_t {namespace}_file_{dir_index}_{file_index}_data[] = {{"
                )
                lines.append(format_data_array(payload.raw_data or b""))
                lines.append("};")
                lines.append("")
            elif payload.kind == COMPRESSED_KIND_PACKBITS:
                offsets_values = payload.chunk_offsets or ()

                lines.append(
                    f"static const uint8_t {namespace}_file_{dir_index}_{file_index}_packbits_data[] = {{"
                )
                lines.append(format_data_array(payload.compressed_data or b""))
                lines.append("};")
                lines.append("")
                lines.append(
                    f"static const uint32_t {namespace}_file_{dir_index}_{file_index}_packbits_offsets[] = {{"
                )
                lines.append(
                    "  "
                    + ", ".join(f"{value}U" for value in offsets_values)
                )
                lines.append("};")
                lines.append("")
                lines.append(
                    f"static const vfs_romfs_chunked_desc_t {namespace}_file_{dir_index}_{file_index}_packbits_desc = {{"
                )
                lines.append(f"  .size = {payload.size_expr},")
                lines.append(f"  .chunk_size = {payload.chunk_size}U,")
                lines.append(f"  .chunks_num = {len(offsets_values) - 1}U,")
                lines.append(
                    f"  .offsets = {namespace}_file_{dir_index}_{file_index}_packbits_offsets,"
                )
                lines.append(
                    f"  .data = {namespace}_file_{dir_index}_{file_index}_packbits_data,"
                )
                lines.append("};")
                lines.append("")
            else:
                fail(f"unsupported static payload kind: {payload.kind}")

        lines.append(
            f"static const vfs_romfs_file_desc_t {namespace}_dir_{dir_index}_files[] = {{"
        )
        if directory.files:
            for file_index, file_entry in enumerate(directory.files):
                lines.append("  {")
                lines.append(f"    .name = {escape_c_string(file_entry.name)},")
                lines.append(f"    .mode = {file_entry.mode},")
                if file_entry.ops is None:
                    payload = static_payloads[(dir_index, file_index)]

                    lines.append(f"    .flags = {payload.flags_expr},")
                    lines.append(f"    .size = {payload.size_expr},")
                    if payload.kind == COMPRESSED_KIND_RAW:
                        lines.append(
                            f"    .content = {{ .data = {namespace}_file_{dir_index}_{file_index}_data }},"
                        )
                    elif payload.kind == COMPRESSED_KIND_PACKBITS:
                        lines.append("    .content = {")
                        lines.append("      .compressed = {")
                        lines.append(f"        .ops = &{PACKBITS_OPS_SYMBOL},")
                        lines.append(
                            f"        .arg = &{namespace}_file_{dir_index}_{file_index}_packbits_desc,"
                        )
                        lines.append("      },")
                        lines.append("    },")
                    else:
                        fail(f"unsupported static payload kind: {payload.kind}")
                else:
                    flags_expr = render_file_flags(file_entry)

                    lines.append(f"    .flags = {flags_expr},")
                    lines.append(f"    .size = (vfs_offset_t){file_entry.size},")
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
