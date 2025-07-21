[#ftl]
[#--
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,
              2011,2012,2013,2014,2015,2016,2017,2018,
              2019,2020 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]

[#--
  -- Find and return a code template from the templates archive.
  --]
[#function GetThreadCode name]
  [#list doc_snippets.code.snippets.snippet as snippet]
    [#if (snippet.type[0] == "thread_body") &&
         ((snippet.name[0]!"")?trim?lower_case == name?trim?lower_case)]
      [#return snippet.text[0]!""]
    [/#if]
  [/#list]
  [#return "/* Thread style not found: " + name + " */"]
[/#function]
