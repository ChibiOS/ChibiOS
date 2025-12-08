#!/usr/bin/env python3
import re
import sys

# Port of the original Perl style checker; keeps the same heuristics and output.
# Desired indentation (kept for parity with the original script).
indentation = 2


def main():
  if len(sys.argv) != 2:
    print("\nUsage: stylecheck.py source\n")
    return

  source = sys.argv[1]

  try:
    with open(source, "r", newline="") as infile:
      c_source = infile.readlines()
  except OSError as exc:
    raise SystemExit(f"Can't open source: {exc}")

  cr = "\r"
  lf = "\n"
  tab = "\t"

  lineno = 0
  emptycnt = 0
  c_comment_complete = False
  c_comment = ""
  state = "start"

  def style(desc: str) -> None:
    print(f'style: {desc} at line {lineno} in "{source}"')

  def error(desc: str) -> None:
    print(f'error: {desc} at line {lineno} in "{source}"')

  # Run style checks that apply to a completed C-style comment block.
  def check_comment(comment: str) -> None:
    if re.match(r"\s*/\*lint", comment):
      return

    if re.match(r"\s*/\*\*<[^\s]", comment):
      style("detected glued doxygen back-comment start")

    if re.match(r"\s*/\*\*[^\s<]", comment):
      style("detected glued doxygen comment start")

    if re.match(r"\s*/\*[^\s\*=]", comment):
      style("detected glued comment start")

    if re.match(r"\s*/\*\s*[a-z]", comment):
      style("detected lower case comment start")

  for raw_line in c_source:
    lineno += 1

    if c_comment_complete:
      check_comment(c_comment)
      c_comment_complete = False

    line = raw_line

    if not line.endswith(lf):
      error("detected malformed EOL")

    line = line.replace(cr, "")
    line = line.replace(lf, "")

    if re.search(r"\s$", line):
      style("detected trailing spaces")

    # Tabs are reported and the first one is expanded to align with prior logic.
    if tab in line:
      style("detected TAB")
      line = line.replace(tab, "    ", 1)

    tmp = re.sub(r"\s", "", line)
    if len(tmp) == 0:
      emptycnt += 1
      if emptycnt == 2:
        style("detected multiple empty lines")
      continue
    else:
      emptycnt = 0

    line = line.replace('\\"', "", 1)
    line = re.sub(r"\"[^\"]*\"", "_string_", line, count=1)

    # Minimal state machine to track comment parsing for follow-up checks.
    if state == "start":

      if "/*" in line:
        if "*/" in line:
          match = re.search(r"/\*.*\*/", line)
          if match:
            c_comment = match.group(0)
            c_comment_complete = True
        else:
          match = re.search(r"/\*.*", line)
          if match:
            c_comment = match.group(0)
            state = "incomment"

      else:

        if "//" in line:
          style("detected // comment")

        if re.search(r",\S", line):
          style("detected comma not followed by space")

        if re.search(r"\S\s;", line):
          style("detected loose semicolon")

        if re.search(r"\sif\(", line):
          style('detected glued "if"')
        if re.search(r"\sfor\(", line):
          style('detected glued "for"')
        if re.search(r"\swhile\(", line):
          style('detected glued "while"')
        if re.search(r"\)while", line):
          style('detected glued "while"')
        if re.search(r"\sswitch\(", line):
          style('detected glued "switch"')
        if re.search(r"\sdo\{", line):
          style('detected glued "do"')

        if re.search(r"\(\s+", line):
          style('detected loose "("')
        if re.search(r"\S\s+\)", line):
          style('detected loose ")"')

        if re.search(r"\)\{", line):
          style("detected glued left brace")
        if re.search(r"\w\{", line):
          style("detected glued left brace")
        if re.search(r"\}\w", line):
          style("detected glued right brace")

        if re.search(r"(\(\S<<=?|\S>>=?|[^\s<]<=|[^\s>]>=|\S[=!+\-*\/%&|^]=)", line):
          style("detected glued operator (1)")
        elif re.search(r"=[^\s=]", line):
          style("detected glued assignment/comparison operator (2)")
        elif re.search(r"[^\s\=\!\+\-\*\/\%\&\|\^\<\>]=", line):
          style("detected glued assignment/comparison operator (3)")
        elif re.search(r"(<<|>>)[^\s=]", line):
          style("detected glued assignment/comparison operator (4)")
        elif re.search(r"\S(&&|\|\||\^\^)", line):
          style("detected glued logical operator (1)")
        elif re.search(r"(&&|\|\||\^\^)\S", line):
          style("detected glued logical operator (2)")

    elif state == "incomment":
      line = re.sub(r"^\s+", "", line)
      if re.match(r"^\s*\*/\s*$", line):
        c_comment += "*/"
        c_comment_complete = True
        state = "start"
      elif re.search(r"\*/\s*$", line):
        match = re.search(r".*\*/", line)
        if match:
          c_comment += " " + match.group(0)
          c_comment_complete = True
          state = "start"
      else:
        line = re.sub(r"^\*?\s*", "", line)
        c_comment += " " + line


if __name__ == "__main__":
  main()
