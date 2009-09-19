*** Documentation access ***

Open ./docs/index.html to open the start page or ./docs/html/index.html in
order to access directly the doxigen documentation.

*** Documentation rebuild procedure ***

The following software must be installed:
- Doxygen 1.5.7.1 or later.
- Graphviz 2.21 or later. The ./bin directory must be specified in the path in
  order to make Graphviz accessible by Doxygen.

Build procedure:
- Run Doxywizard.
- Load ./docs/Doxyfile from Doxywizard.
- Start.

*** Doxygen usage rules ***
- The JavaDoc style is recommended (@ instad of \, /** instead of /*!).
- Params descriptions must not begin with a capital letter and must not be
  terminated with a dot unless it is composed of multiple paragraphs.
- Retvals descriptions must not begin with a capital letter but must be
  terminated by a dot.
- Normal paragraphs, return rescriptions and notes must start with a capital
  letter and must be terminated with a dot.
- Multiple return values should be listed using the @retval command.
- AUTOBRIEF is enabled but deprecated, gradually convert to the use of explicit
  @brief command.

Examples:
@note This is a note.
@retval RDY_OK is a possible return value.
@return The pointer to the created thread is returned.
@param n the number of bytes to be written
@param n the number of bytes to be written. This value can be zero.
