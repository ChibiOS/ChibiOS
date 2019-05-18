#!/usr/bin/perl
use strict;
use warnings;
use File::Basename;

# Desired indentation.
my $indentation = 2;

if ($#ARGV != 0) {
  print "\nUsage: simplecheck.pl source\n";
  exit;
}

my $source = $ARGV[0];

open(my $in,  "<", $source) or die "Can't open source: $!";

my $lineno = 0;
my @c_source = <$in>;
my $filename = $source;
$filename =~ y/\\/\//;
$filename = basename($filename);

my $cr        = "\r";
my $lf        = "\n";
my $tab       = "\t";
my $c_comment = "";
my $state     = "start";

sub style {
  my $desc = shift;

  print("style: $desc at line $lineno in \"$filename\"\n");
}

sub error {
  my $desc = shift;

  print("error: $desc at line $lineno in \"$filename\"\n");
}

my $emptycnt = 0;
foreach my $line (@c_source) {

  $lineno += 1;

  #****************************************************************************
  # Check on EOL.
  if (not ($line =~ /$cr$lf$/)) {
    error "detected malformed EOL";
  }
  $line =~ s/$cr//;
  $line =~ s/$lf//;

  #****************************************************************************
  # Check on trailing spaces.
  if ($line =~ /\s$/) {
    style "detected trailing spaces";
  }

  #****************************************************************************
  # Check on TABs.
  if ($line =~ /$tab/) {
    style "detected TAB";
    $line =~ s/$tab/    /;
  }

  #****************************************************************************
  # Check on empty lines.
  my $tmp = $line;
  $tmp =~ s/\s//;
  if (length($tmp) == 0) {
    $emptycnt = $emptycnt + 1;
    if ($emptycnt == 2) {
      style "detected multiple empty lines"
    }
    next;
  }
  else {
    $emptycnt = 0;
  }

  #****************************************************************************
  # Stripping strings content for ease of parsing, all strings become _string_.
  $line =~ s/\\\"//;
  if ($line =~ s/(\"[^"]*\")/_string_/) {
#    print "string: $1 replaced by _string_\n";
  }

  #******************************************************************************
  # State machine handling.
  if ($state eq "start") {

    #******************************************************************************
    # Comment start matching.
    if ("$line" =~ /^\s*\/\*/) {
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
    else {

      #****************************************************************************
      # Check on C++ comments.
      if ($line =~ /\/\//) {
        style "detected // comment";
      }

      #****************************************************************************
      # Check on loose semicolons.
      if ($line =~ /\s;/) {
        style "detected loose semicolon";
      }

      #****************************************************************************
      # Check on glued keywords.
      if ($line =~ /\sif\(/) {
        style "detected glued \"if\"";
      }
      if ($line =~ /\sfor\(/) {
        style "detected glued \"for\"";
      }
      if ($line =~ /\swhile\(/) {
        style "detected glued \"while\"";
      }
      if ($line =~ /\)while/) {
        style "detected glued \"while\"";
      }
      if ($line =~ /\sswitch\(/) {
        style "detected glued \"switch\"";
      }
      if ($line =~ /\sdo\{/) {
        style "detected glued \"do\"";
      }

      #****************************************************************************
      # Check on loose parenthesis.
      if ($line =~ /\(\s+/) {
        style "detected loose \"(\"";
      }
      if ($line =~ /\s+\)/) {
        style "detected loose \")\"";
      }

      #****************************************************************************
      # Check on glued braces.
      if ($line =~ /\)\{/) {
        style "detected glued left brace";
      }
      if ($line =~ /\w\{/) {
        style "detected glued left brace";
      }
      if ($line =~ /\}\w/) {
        style "detected glued right brace";
      }

      #****************************************************************************
      # Check function-call-like returns (not perfect so disabled).
      if ($line =~ /return\s*\(/) {
        if ($line =~ /return\s*\([\w\d\s\*]*\)\s*[^;]/) {
        }
        else {
#          style "detected function-call-like return";
        }
      }
    }
  }

  #******************************************************************************
  # Scanning for comment end.
  elsif ($state eq "incomment") {
    if ("$line" =~ /\*\/\s*$/) {
      # End of mult-line comment.
      $line =~ /(.*\*\/)/;
      $c_comment .= $1;
#      print("$c_comment");
      $state = "start";
    }
    else {
      # Add the whole line.
      $c_comment .= $line . " ";
#      print("$c_comment");
    }
  }
}

close $in or die "$in: $!";
