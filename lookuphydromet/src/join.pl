#!/usr/bin/perl 

open INPUT, "input";
open DATA, "data";

@input = <INPUT>;
@data = <DATA>;

chomp @input;


for ($i = 0;$i < scalar(@input); $i++) {
print $input[$i] ." " . $data[$i];
}
