#!/usr/bin/perl
use strict;
use warnings;


sub process_header_file
{
  ($module, $header_file_name) = @_;
  
  
  open(HEADER_FILE, "../include/private/stlwrt/" . $header_file_name)
   or die "Could not find file \"" . "../include/private/stlwrt/" .
   $header_file_name . "\".\n";
  
  
  while (<HEADER_FILE>)
  {
    @line = split(//, $_);
    
    $token_encountered = 0;
    $is_preprocessor_directive = 0;
    
    for ($index = 0; $index < @line; $index++)
    {
      # Ignore any spaces on the line, including newlines.
      if ($line[$index] >= 0x00 && $line[$index] <= 0x20)
      {
        continue;
      }
      
      # If this character is not a space, perform further
      # processing steps.
      
      if ($line[$index] == '#' && not $token_encountered)
      {
        # This line contains a preprocessor directive.
        # Treat this line specially; in C, preprocessor
        # directives are not treated anything like
        # normal C code.
        
        for (; $index < @line; $index++)
        {
          # Ignore any spaces on the line, including newlines.
          if ($line[$index] >= 0x00 && $line[$index] <= 0x20)
          {
            continue;
          }
          
          # If this character is not a space, perform further
          # processing steps.
          
          if ($line[$index..$index+7] == 'include')
          {
            
          }
        }
      }
    }
  }
}


my $module;


if (@ARGV != 1 || ($ARGV[0] != 'gtk' && $ARGV[0] != 'gdk'))
{
  die "Specify either \"gtk\" or \"gdk\" on the command line.\n";
}

$module = $ARGV[0];


process_header_file($module, $module . ".h");
