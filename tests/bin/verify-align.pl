#!/usr/bin/perl -w

if (defined($ENV{"CAULDRON"})) {
  @ARGV = (
    "-a",
    "4096",
    "/bin/ls",
  );
}

use bigint qw/hex/;
use Getopt::Std;
use POSIX qw(sysconf _SC_PAGESIZE);

exit(&main());

sub load_commands {
  my ($path) = @_;
  my ($out);
  my ($cmd) = "xcrun otool-classic -lv '$path'";
  
  $out = `$cmd`;
  
  if ($?) {
    die "can't run command: $cmd\n" if ($? == -1);
    
    my ($status) = $? >> 8;
    die "command failed with status $status: $cmd\n";
  }
  
  return $out;
}

sub escape {
  my ($x) = @_;
  $x =~ s#([\.\^\*\+\?\(\)\[\{\|\$])#\\$1#g;
  return $x;
}

sub check_alignment {
  my ($pagealign, $load_commands, $inpath) = @_;
  my ($result) = 0;
  
  # split the otool command into lines
  my (@lines) = split '\n', $load_commands;

  # state for the state machine, data for the data throne.  
  my ($cur_path);
  my ($cur_lc);
  my ($cur_segment);
  my ($cur_segment_name);
  
  # walk the otool output, pull out state, and check the segment offsets.
  # all segment offsets (except __LINKEDIT sizes) must be page aligned.
  foreach my $line (@lines) {
    my ($pattern) = &escape($inpath);
    if ($line =~ /^(${pattern}).*:/) {
      ($cur_path = $line) =~ s/://;
      $cur_lc = undef;
      $cur_segment = undef;
    }
    elsif ($line =~ /^Load command/) {
      $cur_lc = $line;
      $cur_segment = undef;
    }
    elsif ($line =~ /cmd LC_SEGMENT/) {
      die "no load command?" unless defined $cur_lc;
      ($cur_segment = $line) =~ s/.*cmd //;
    }
    elsif ($line =~ /segname/) {
      ($cur_segment_name = $line) =~ s/.* segname //;
    }
    elsif ($line =~ /(vmaddr|vmsize|fileoff|filesize)/) 
    {
      my ($field) = $1;
      unless (($field eq "filesize" 
#       or $field eq "vmsize"
      ) and 
	      $cur_segment_name eq "__LINKEDIT")
      {
	(my $vstr = $line) =~ s/.*$field\s//;
	my ($value) = $vstr =~ /^0x/ ? 
		      hex($vstr) :
		      int($vstr);
	my ($align) = $value ? 
		      $value + ($pagealign - (($value - 1) % $pagealign) - 1) :
		      0;
	if ($value != $align) {
	  printf "$cur_path segment $cur_segment_name $field not aligned: " . 
		"0x%016llx should be 0x%016llx\n", $value, $align;
	  $result = 1;
	}
      }
    }
  }
  
  return $result;
}

sub main {
  
  return &usage() unless (@ARGV);
  return &usage() unless getopts('a:');
  
  my $segalgn = 0;
  
  if ($opt_a) {
    $segalign = $opt_a =~ /0(x|X)/ ? hex($opt_a) : int($opt_a);
  }
  unless ($segalign) {
    $segalign = sysconf(_SC_PAGESIZE);
  }
  
  foreach my $path (@ARGV) {
    my $lc = &load_commands($path);
    &check_alignment($segalign, $lc, $path);
  }

  return 0;
}

sub usage {
  (my $progname = $0) =~ s|.*/||;
  print "usage: $progname [-a=align] file...\n";
  return 1;
}
