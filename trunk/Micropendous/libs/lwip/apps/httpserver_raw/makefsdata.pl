#!/usr/bin/perl

use strict;

open(OUTPUT, "> httpd-fsdata.c");
print OUTPUT "\#include <avr/pgmspace.h>\n";
print OUTPUT "\#include \"http-strings.h\"\n";

chdir("httpd-fs");

opendir(DIR, ".");
my @files =  grep { !/^\./ && !/(CVS|~)/ } readdir(DIR);
closedir(DIR);

my $file;

foreach $file (@files) {  
   
    if(-d $file && $file !~ /^\./) {
      print "Processing directory $file\n";
      opendir(DIR, $file);
      my @newfiles =  grep { !/^\./ && !/(CVS|~)/ } readdir(DIR);
      closedir(DIR);
      printf "Adding files @newfiles\n";
      @files = (@files, map { $_ = "$file/$_" } @newfiles);
      next;
    }
}

my @fvars;
my @pfiles;
my @mime;


foreach $file (@files) {
  if(-f $file) {
    print "Adding file $file\n";
    
    open(FILE, $file) || die "Could not open file $file\n";
		
    if(($file =~ /\.html$/) || ($file =~ /\.htm$/)) {
			push @mime,"http_content_type_html";
    } elsif(($file =~ /\.shtml$/) || ($file =~ /\.shtm$/) ||
            ($file =~ /\.ssi$/)){
			push @mime,"http_content_type_html";
    } elsif($file =~ /\.gif$/) {
			push @mime,"http_content_type_gif";
    } elsif($file =~ /\.png$/) {
			push @mime,"http_content_type_png";
    } elsif($file =~ /\.jpg$/) {
			push @mime,"http_content_type_jpg";
    } elsif($file =~ /\.class$/) {
			push @mime,"http_content_type_binary";
    } elsif($file =~ /\.ram$/) {
			push @mime,"http_content_type_ra";
    } elsif($file =~ /\.bmp$/) {
			push @mime,"http_content_type_bmp";
    } elsif($file =~ /\.css$/) {
			push @mime,"http_content_type_css";
    } else {
			push @mime,"http_content_type_text";
    }

    $file =~ s-^-/-;
    my $fvar = $file;
    $fvar =~ s-/-_-g;
    $fvar =~ s-\.-_-g;

    # for AVR, add PROGMEM here
    print(OUTPUT "static prog_char data".$fvar."[] = {\n");
    print(OUTPUT "\t/* $file */\n\t");
    my $j;
    for($j = 0; $j < length($file); $j++) {
        printf(OUTPUT "%#02x, ", unpack("C", substr($file, $j, 1)));
    }
    printf(OUTPUT "0,\n");
    
    
    my $i = 0;
    my $data;
    while(read(FILE, $data, 1)) {
        if($i == 0) {
          print(OUTPUT "\t");
        }
        printf(OUTPUT "%#02x, ", unpack("C", $data));
        $i++;
        if($i == 10) {
          print(OUTPUT "\n");
          $i = 0;
        }
    }
    print(OUTPUT "0};\n\n");
    close(FILE);
    push(@fvars, $fvar);
    push(@pfiles, $file);
  }
}

my $i;
#my $prevfile;
my $numfiles=$#fvars+1;
print(OUTPUT "const httpd_fsdata httpd_fsdata_file[$numfiles]={\n");
for($i = 0; $i < $numfiles; $i+=1) {
    my $file  = $pfiles[$i];
    my $fvar  = $fvars[$i];
		my $fmime =$mime[$i];
#    if($i == 0) {
#        $prevfile = "NULL";
#    } else {
#        $prevfile = "file" . $fvars[$i - 1];
#    }
#    print(OUTPUT "const struct httpd_fsdata_file file".$fvar."[] = {{$prevfile, data$fvar, ");
#    print(OUTPUT "data$fvar + ". (length($file) + 1) .", ");
#    print(OUTPUT "sizeof(data$fvar) - ". (length($file) + 1) ."}};\n\n");
    print(OUTPUT "{$fmime,data$fvar, ");
    print(OUTPUT "data$fvar + ". (length($file) + 1) .", ");
    print(OUTPUT "sizeof(data$fvar) - ". (length($file) + 1) ."},\n");
}
print(OUTPUT "};\n");
print(OUTPUT "#define HTTPD_FS_NUMFILES $i\n");
