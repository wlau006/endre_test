use strict;
use warnings;

sub corrupt ($$$$) {
    # Get parameters, names should be self-explanatory.

    my $filespec = shift;
    my $mincount = shift;
    my $maxcount = shift;
    my $charset = shift;

    # Work out position and size of corruption.

    my @fstat = stat ($filespec);
    my $size = $fstat[7];
    my $count = $mincount + int (rand ($maxcount + 1 - $mincount));
    my $pos = 0;
    if ($count >= $size) {
        $count = $size;
    } else {
        $pos = int (rand ($size - $count));
    }

    # Output for debugging purposes.

    my $last = $pos + $count - 1;
    print "'$filespec', $size bytes, corrupting $pos through $last\n";
 

    # Open file, seek to position, corrupt and close.

    open (my $fh, "+<$filespec") || die "Can't open $filespec: $!";
    seek ($fh, $pos, 0);
    while ($count-- > 0) {
        my $newval = substr ($charset, int (rand (length ($charset) + 1)), 1);
        print $fh $newval;
    }
    close ($fh);
}

# Test harness.

system ("echo =========="); #DEBUG
#system ("cat oldtext.txt"); #DEBUG
#system ("cp oldtext.txt newtext.txt"); #DEBUG
system ("echo =========="); #DEBUG

my @a = (1..200);
for(@a){
    corrupt ("modtext.txt", 1, 1000, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ   ");
}

system ("echo =========="); #DEBUG
#system ("cat newtext.txt"); #DEBUG
#system ("cp newtext.txt oldtext.txt"); #DEBUG
system ("echo =========="); #DEBUG
