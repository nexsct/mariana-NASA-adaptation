#!/usr/bin/perl -w

$usage = "USAGE:\n\tperl txt2sparse.pl modelfile.model textfile.txt\n\n";

if (@ARGV < 2)
{
	print $usage;
	exit
}


# the terms and thesaurus maps
%terms = ();
%thes = ();


# open the model file to get the terms and thesaurus
open(MOD,"./$ARGV[0]") || die "\nerror: cannot opem model file $ARGV[0]\n";

$findThes = 0;
$findTerm = 0;

foreach $line (<MOD>)
{
	chomp $line;
	$line =~ tr/{/  /;
	$line =~ tr/}/  /;

	if ( $line =~ /^Thesaurus:/ )
	{
		$line =~ s/^Thesaurus://;
		
		@elem = split(/, /,$line);
		$i = 0;
		foreach $el (@elem)
		{
			@kvpair = split(/=/,$el);
			$thes{ $kvpair[0] } = $kvpair[1];
		}
		$findThes = 1;
	}
	
	if ( $line =~ /^Terms:/ )
	{
		$line =~ s/^Terms://;
		@elem = split(/ /,$line);
		$i = 0;
		foreach $el (@elem)
		{
			@kvpair = split(/=/,$el);
			$terms{ $kvpair[0] } = $kvpair[1];
		}
		$findTerm = 1;
	}
	
	if (($findTerm + $findThes) == 2){ last;}
	
}


close(MOD);


#open the text file
open(TXT,"./$ARGV[1]") || die " cannot open the text file $ARGV[1]\n";


foreach $line (<TXT>)
{
	#print "$line";
	chomp $line;

	
	#convert everything to upper case
	$line =~ tr/a-z/A-Z/;
	#convert non-char to spaces
	$line =~ s/[^a-zA-Z]/ /g;
	#remove multiple space and tabs
	$line =~ s/\t/ /g;
	$line =~ s/ +/ /g;
	
	#go through the thesaurus and replace
	while (($key, $value) = each(%thes))
	{
		#print "$key=$value ---- ";
		$line =~ s/\b$key\b/$value/g;
	}
	
	#print the category (0 for now)
	print "0";
	
	#map the terms to the index and print
	%docVec = ();
	while (($key,$value) = each(%terms))
	{
		$cnt = () = $line =~ m/\b$key\b/g;
		if ($cnt!=0)
		{
			$docVec{$value} = $cnt;
			#print " $value:$cnt";
		}
	}
	
	foreach $key (sort keys %docVec)
	{
		print " $key:$docVec{$key}";
	}
	#foreach $key (sort { $hash{$b} <=> $hash{$a} } keys %hash) 
	#{
	#	printf "%4d %s\n", $hash{$key}, $key;
	#}
	
	print "\n";
	#print "$line\n";
	#last;
}
