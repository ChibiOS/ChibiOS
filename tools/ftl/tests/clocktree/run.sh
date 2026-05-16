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
}

mkdir -p "$tmp_root"

for fixture in "$script_dir"/valid/*.xml; do
  run_valid_fixture "$fixture"
done

printf '%s\n' "clocktree test suite passed"
