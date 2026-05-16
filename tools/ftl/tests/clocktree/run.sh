#!/bin/sh
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
repo_root=$(CDPATH= cd -- "$script_dir/../../../.." && pwd)
schema="$repo_root/tools/ftl/schema/clocks/clocktree.xsd"
processor="$repo_root/tools/ftl/processors/clocktree"
libs="$repo_root/tools/ftl/libs"
tmp_root="${TMPDIR:-/tmp}/clocktree-tests.$$"

cleanup() {
  rm -rf "$tmp_root"
}
trap cleanup EXIT HUP INT TERM

run_valid_fixture() {
  fixture="$1"
  name=$(basename "$fixture" .xml)
  work="$tmp_root/valid/$name"

  printf '%s\n' "== valid/$name"
  xmllint --noout --schema "$schema" "$fixture"

  mkdir -p "$work/out"
  cp "$fixture" "$work/clocktree.xml"
  cat > "$work/config.fmpp" <<EOF
sourceRoot: $processor
outputRoot: out
dataRoot: .

modes: [execute(clocktree.h.ftl), ignore(**)]

freemarkerLinks: {
    lib: $libs
}

data : {
  doc1:xml (
    clocktree.xml
    {
    }
  )
}
EOF

  (cd "$work" && fmpp -C config.fmpp >/dev/null)
  test -s "$work/out/clocktree.h"

  checks="$fixture.checks"
  if [ -f "$checks" ]; then
    while IFS= read -r pattern || [ -n "$pattern" ]; do
      case "$pattern" in
        ''|\#*) continue ;;
      esac
      if ! grep -F -- "$pattern" "$work/out/clocktree.h" >/dev/null; then
        printf '%s\n' "generated output for $fixture is missing: $pattern" >&2
        return 1
      fi
    done < "$checks"
  fi
}

run_schema_invalid_fixture() {
  fixture="$1"
  name=$(basename "$fixture" .xml)

  printf '%s\n' "== invalid/schema/$name"
  if xmllint --noout --schema "$schema" "$fixture" >/dev/null 2>&1; then
    printf '%s\n' "schema-invalid fixture unexpectedly validated: $fixture" >&2
    return 1
  fi
}

run_generator_invalid_fixture() {
  fixture="$1"
  name=$(basename "$fixture" .xml)
  work="$tmp_root/invalid-generator/$name"

  printf '%s\n' "== invalid/generator/$name"
  xmllint --noout --schema "$schema" "$fixture"

  mkdir -p "$work/out"
  cp "$fixture" "$work/clocktree.xml"
  cat > "$work/config.fmpp" <<EOF
sourceRoot: $processor
outputRoot: out
dataRoot: .

modes: [execute(clocktree.h.ftl), ignore(**)]

freemarkerLinks: {
    lib: $libs
}

data : {
  doc1:xml (
    clocktree.xml
    {
    }
  )
}
EOF

  if (cd "$work" && fmpp -C config.fmpp >/dev/null 2>&1); then
    printf '%s\n' "generator-invalid fixture unexpectedly generated: $fixture" >&2
    return 1
  fi
}

mkdir -p "$tmp_root"

for fixture in "$script_dir"/valid/*.xml; do
  run_valid_fixture "$fixture"
done

for fixture in "$script_dir"/invalid/schema/*.xml; do
  run_schema_invalid_fixture "$fixture"
done

for fixture in "$script_dir"/invalid/generator/*.xml; do
  run_generator_invalid_fixture "$fixture"
done

printf '%s\n' "clocktree test suite passed"
