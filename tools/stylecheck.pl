#!/usr/bin/perl
use strict;
use warnings;
use File::Basename;

if ($#ARGV != 0) {
  print "\nUsage: stylecheck.pl source\n";
  exit;
}

my $source = $ARGV[0];

open(my $in,  "<", $source) or die "Can't open source: $!";

my $lineno = 0;
my @c_source = <$in>;
my $filename = $source;
$filename =~ y/\\/\//;
$filename = basename($filename);

my $c_comment = "";
my $state     = "start";
my $cr        = "\r";
my $lf        = "\n";
my $f_lineno  = 0;
my $f_name    = "";
my $t_lineno  = 0;
my $t_type    = "";
my $e_lineno  = 0;
my $e_name    = "";
my $d_lineno  = 0;
my $d_name    = "";
my $d_name2    = "";
my $d_name_app = "";
my $scope     = "";

foreach my $line (@c_source) {

  $lineno += 1;
  $line =~ s/$cr//;
  $line =~ s/$lf//;

  #******************************************************************************
  # State machine handling.
  if ($state eq "start") {
    # Searching for a global code element or a comment start.

    #******************************************************************************
    # Functions matching, triggered by the "(".
    if ($line =~ /^(static|)\s*(struct|union|)\s*([a-zA-Z_][a-zA-Z0-9_]*\s*[*]*)\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*\(/) {
      # $1=scope $2$3=return type $4=name
      $line =~ s/^(static|)\s*(struct|union|)\s*([a-zA-Z_][a-zA-Z0-9_]*\s*[*]*)\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*\(//;
      print "function: " . $1 . " " . $2 . " " . $3 . " " . $4 . "(";

      # Function line number.
      $f_lineno = $lineno;
      
      # Checking if all parameters are on the same line.
      if ($line =~ /.*\)\s*{/) {
        print $line . "\n";
        $state = "infunc";
      }
      else {
        print $line;
        $state = "inparams";
        }
    }
    #******************************************************************************
    # Structures matching.
    elsif ($line =~ /^\s*struct\s+([a-zA-Z_][a-zA-Z0-9_]*)/) {
    }
    #******************************************************************************
    # Single line "typedef" matching.
    elsif ($line =~ /^\s*typedef\s+([\sa-zA-Z0-9_]*\*+)\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*;\s*$/ or
           $line =~ /^\s*typedef\s+([\sa-zA-Z0-9_]*)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*;\s*$/) {
    }
    #******************************************************************************
    # Single line "typedef function" matching.
    elsif ($line =~ /^\s*typedef\s+.*\(\s*\*\s*([a-zA-Z0-9_]*)\s*\)\(/) {
    }
    #******************************************************************************
    # Multi line "typedef struct" matching.
    elsif ($line =~ /^\s*typedef\s*struct\s*([a-zA-Z_][a-zA-Z0-9_]*|\s?)?/) {
      $t_lineno = $lineno;
      $t_type   = "struct " . $1;
      $state = "intypedef";
    }
    #******************************************************************************
    # Multi line "typedef enum" matching.
    elsif ($line =~ /^\s*typedef\s*enum\s*([a-zA-Z_][a-zA-Z0-9_]*|\s?)?/) {
      $t_lineno = $lineno;
      $t_type   = "enum " . $1;
      if ($line =~ /([a-zA-Z_][a-zA-Z0-9_]*)\s*;\s*$/) {
        # Special case of a single-line typedef enum.
      }
      else {
        $state = "intypedef";
      }
    }
    #******************************************************************************
    # Multi line "enum" matching.
    elsif ($line =~ /^\s*enum\s*([a-zA-Z_][a-zA-Z0-9_]*)/) {
      $e_name = $1;
      $e_lineno = $lineno;
      if ($line =~ /;\s*$/) {
        # Special case of a single-line enum.
      }
      else {
        $state = "inenum";
      }
    }
    #******************************************************************************
    # Struct variable matching.
    elsif ($line =~ /^\s*(static|)\s+([\sa-zA-Z0-9_]*)\s+([a-zA-Z_][a-zA-Z0-9_]*\[.*\]|[a-zA-Z_][a-zA-Z0-9_]*)\s*(;\s*$|=)/ or
           $line =~ /^\s*(static|)\s+([\sa-zA-Z0-9_]*\*+)\s*([a-zA-Z_][a-zA-Z0-9_]*\[.*\]|[a-zA-Z_][a-zA-Z0-9_]*)\s*(;\s*$|=)/) {
    }
    #******************************************************************************
    # Variable matching.
    elsif ($line =~ /^\s*(static|).*\s+([a-zA-Z_][a-zA-Z0-9_]*\s*\*+|[a-zA-Z_][a-zA-Z0-9_]*)\s*([a-zA-Z_][a-zA-Z0-9_]*\[.*\]|[a-zA-Z_][a-zA-Z0-9_]*)\s*(;\s*$|=)/) {
      # variable declaration.
    }
    #******************************************************************************
    # #nclude matching.
    elsif ($line =~ /^\s*#include\s+"([^"]+)"/) {
    }
    #******************************************************************************
    # #if matching.
    elsif ($line =~ /^\s*#if\s+(.+)/) {
    }
    #******************************************************************************
    # #ifdef matching.
    elsif ($line =~ /^\s*#ifdef\s+([\w_]+)/) {
    }
    #******************************************************************************
    # #define matching.
    elsif ($line =~ /^\s*#define\s+([\w_]+)\s*(.*)/) {
      $d_lineno = $lineno;
      $d_name   = $1;
      $d_name2   = $2;
      # enum typedef declaration.
      if ($line =~ /[^\\]$/) {
        # Special case of a single-line typedef enum.
      }
      else {
        $state = "indefine";
      }
    }
    #******************************************************************************
    # Comment start matching.
    elsif ("$line" =~ /^\s*\/\*/) {
      if ("$line" =~ /\*\//) {
        # Special case of single line comments.
        $line =~ /^\s*(\/\*.*\*\/)/;
      }
      else {
        # Start of multi-line comment.
        $line =~ /(\/\*.*)/;
        $c_comment = $1 . " ";
        $state = "incomment";
      }
    }
  }
  #******************************************************************************
  # Scanning for params end and function body begin.
  elsif ($state eq "inparams") {
    if ($line =~ /.*\)\s*{/) {
        print $line . "\n";
        $state = "infunc";
    }
    else {
      print $line;
    }
  }
  #******************************************************************************
  # Scanning for function end.
  elsif ($state eq "infunc") {
    if (($line =~ /^\}/) || ($line =~ /^\);/)) {
      # Function end because the final '}' or ');' in case of a prototype.
      $state = "start";
    }
    elsif ($line =~ /(\/\*.*\*\/)/) {
      # Single line comments inside functions.
    }
    elsif ("$line" =~ /(\/\*.*)/) {
      # Start of multi-line comment inside a function.
      $c_comment = $1 . " ";
      $state = "infunccomment";
    }
  }
  #******************************************************************************
  # Scanning for comment end within a function body.
  elsif ($state eq "infunccomment") {
    if ("$line" =~ /\*\/\s*$/) {
      # End of mult-line comment.
      $line =~ /(.*\*\/)/;
      $c_comment .= $1;

      $state = "infunc";
    }
    else {
      # Add the whole line.
      $c_comment .= $line . " ";
    }
  }
  #******************************************************************************
  # Scanning for comment end.
  elsif ($state eq "incomment") {
    if ("$line" =~ /\*\/\s*$/) {
      # End of mult-line comment.
      $line =~ /(.*\*\/)/;
      $c_comment .= $1;

      $state = "start";
    }
    else {
      # Add the whole line.
      $c_comment .= $line . " ";
    }
  }
  #******************************************************************************
  # Scanning for typedef end.
  elsif ($state eq "intypedef") {
    if ($line =~ /^\s*}\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*;\s*$/) {
      # typedef end because the final '} <name>;'.

      $state = "start";
    }
  }
  #******************************************************************************
  # Scanning for enum end.
  elsif ($state eq "inenum") {
    if ($line =~ /^\s*}\s*;\s*$/) {
      # enum end because the final '};'.

      $state = "start";
    }
  }
  #******************************************************************************
  # Scanning for define end.
  elsif ($state eq "indefine") {
    if ($line =~ /[^\\]$/) {
      # define end because the final 'not \'.
      # remove blank from starting of the line
      $line =~ s/^\s+|\s+$//g;
      # Add the whole line.
      $d_name2 .= $line;

      $state = "start";
    }
    else {
      # Add the whole line.
      # $line =~ s/^\s+|\s+$//g;
      $line =~ s/^\s*(.*?)\s*$/$1/;
      $d_name2 .= $line;
    }
  }
}

close $in or die "$in: $!";
