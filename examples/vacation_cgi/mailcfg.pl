#!/usr/bin/perl

#
# sample view/mailcfg script
#

use warnings;
use strict;
use CGI::Pretty qw(:standard);

my $cgi 		= 	new CGI;

#
# path and mailcfg.cgi path
#
our $VACATION		=	'/usr/bin/vacation';
our $MAILCFG		=	'/cgi-bin/mailcfg.cgi';

our $STATEFILENAME	=	'.cgipaf_state';
our $VACATIONS_TXT	= 	'vacations.txt';

#
# html messages
#

our $TITLE		=	'Change your mail configuration';
our $BODY_TITLE		=	'Change your mailbox settings';
our $TXT_LOGIN		=	'Login name:';
our $TXT_FORWARD	=	'Forward mailbox:';
our $TXT_FORWARD_TO	=	'Forward to:';
our $TXT_KEEP_MSGS	=	'Keep messages:';
our $TXT_AUTOREPLY	=	'Enable autoreply:';
our $TXT_AUTOREPLY_MSG	=	'Autoreplay message:';
our $TXT_SUBJECT	=	'Subject:';
our $TXT_YES		=	'yes';
our $TXT_NO		=	'no';
our @YES_NO 		= 	[ $TXT_YES , $TXT_NO ];
our @NO_YES 		= 	[ $TXT_NO  , $TXT_YES ];

#
# print an error message and exit
#
sub print_error ($) {

	print h1($_[0]);
	print end_html;
	die($_[0]);

}

#
# read the current mailcfg
#
sub read_mailcfg () {

	our ($statefile,$vacationstxtfile);

	#
	# default settings
	#

	my %ret = (

		'forward'	=> 0,
		'forwardto'	=> "",
		'keepmsg'	=> 0,
		'autoreply'	=> 0,
		'vacationstxt'  => "",
		'subject'       => ""

	);

	#
	# open statefile
	#

	if (!open (FH,$statefile)) {
	
		return(%ret);

	}

	#
	# parse statefile
	#

	while ( my $line = <FH> ) {
		
		chomp($line);

		#
		# ignore comments
		#

		$line =~ s/(.*)#(.*)/$1/;

		if($line) {

			#
			# split line into key and value
			#

			my ($key,$value) = split(/\s+/,$line,2);

			$value =~ s/\"([^\"]*)\"/$1/;

			#
			# save only valid keys into ret
			#

			foreach (keys %ret) {

				if($key eq $_) {

				$ret{$_}=$value;

				}

			}

		}

	}
	
	close FH;

	#
	# read vacations.txt
	#
	
	if (!open (FH,$vacationstxtfile)) {

		return(%ret);

	}

	my $vacations_text="";

	while ( my $line = <FH> ) {

		$vacations_text .= $line;

	}

	#
	# store vacations_text in $ret
	#

	$ret{'vacationstxt'}=$vacations_text;

	close FH;

	return %ret;

}

#
# save mail settings to $statefile
#
sub save_statefile ($) {

	our $statefile;

	my $mail_settings=$_[0];

	open (FH_STATEFILE,">$statefile") or print_error("Unable to create $statefile: $!");

	print FH_STATEFILE "# CGIPAF state file\n";
	print FH_STATEFILE "# Please don't edit!!!!!!!\n";

	foreach (('forward','forwardto','keepmsg','autoreply','subject')) {
	        
		my $value=$$mail_settings{$_};

		$value = "\"$value\"" if ($value =~ /\s+[^s]+/);

		print FH_STATEFILE $_ . "     " . $value . "\n";

	}

}

#
# update the mailcfg
#
sub save_mailcfg ($) {

	our ($cgipaf_name,$cgipaf_domain,$homedir,$statefile,$vacationstxtfile);

	my $mail_settings=$_[0];

	#
	# set the .forward and .vacation.msg path
	#

	my   $DOTFORWARD="${homedir}/.forward";
	my   $DOTVACATIONMSG="${homedir}/.vacation.msg";

	#
	# delete the user's .forward if forward and autoreply is disabled
	#

	if(!$$mail_settings{'forward'} and ! $$mail_settings{'autoreply'} ) {

		if ( -r "$DOTFORWARD") {

  		   unlink("$DOTFORWARD") or print_error("Unable to delete $DOTFORWARD");
		}

  	}
	else {

	     #
	     # create a new .forward
	     #

	     open(FH,">$DOTFORWARD") or print_error("Unable to create $DOTFORWARD");

	     #
	     # if forwarding is enabled, copy the forward addresses to .forward
	     #

	     if ($$mail_settings{'forward'}) {

   		print FH $$mail_settings{'forwardto'};

   		if($$mail_settings{'keepmsg'}) {
      			print FH " , \\$cgipaf_name";
   		}
	     }

	     #
	     # autoreply configuration
	     #

	     if ($$mail_settings{'autoreply'} ) {

	        if ( (not -r "$homedir/.vacation.db") and ( not -r "$homedir/.vacation.pag") )   {
	   	   system ("$VACATION","-i") == 0 or print_error("$VACATION -i failed");
	        }

   	        if ($$mail_settings{'forward'} ) {
      		   print FH ',';                  # if forwarding enable add ','
	        }
   	        else {
      		       print FH "\\$cgipaf_name,";    # no forwarding, send the msg to the user.
   	        }
    
   	        #
   	        # add vacation to .forward
   	        #
    
   	        print FH "\"|$VACATION $cgipaf_name\"";
   
   	        #
   	        # create the user's .vacation.msg
   	        #
    
   	        open(FH_MSG,">$DOTVACATIONMSG") or print_error("Unable to create $DOTVACATIONMSG");

   	        print FH_MSG "From: $cgipaf_name\@$cgipaf_domain\n";
   	        print FH_MSG "Subject: I am on vacation\n";
   	        print FH_MSG "Delivered-By-The-Graces-Of: The Vacation program\n";
   	        print FH_MSG "Precedence: bulk\n\n";
   	        print FH_MSG "$$mail_settings{'vacationstxt'}";
   	        close FH_MSG;

	     }

	close FH;
	
	}

	#
	# save vacations.txt
	#

	open(FH_VACTXT,">$vacationstxtfile") or print_error("Unable to create $vacationstxtfile");

	print FH_VACTXT "$$mail_settings{'vacationstxt'}";

	close FH_VACTXT;

	save_statefile(\%$mail_settings);

}

#
# returns a 'yes','no' or 'no','yes' array
#
sub yesno ($$) {

	my ($myhash,$key) = @_;

	if ($$myhash{$key} != 0) {
		
		return @YES_NO;

	}
		
	return @NO_YES;

}

#
# set value of key in hash to 1 if current value ="yes"
# else set value to 0
#
sub yesno_to_bool ($$) {

	my ($myhash,$key) = @_;

	if ($$myhash{$key} =~ /yes/i) {

		$$myhash{$key} = 1;

	}
	else {
		$$myhash{$key} = 0;

	}

}

#
# returns true  if emailaddres is valid
#         false                   invalid
#
sub check_emailaddress($) {

	$_ = $_[0];

	return 1 if( /^[^@]+@[^@^.]+\.[^@^.]+$/ );

	return 0;

}

#
# set value to "" if undefined
#
sub clear_undefined ($) {

	my $myhash = $_[0];

	foreach ( keys %$myhash ) {

		if(not defined $$myhash{$_}) {

			$$myhash{$_} = "";

		}

	}

}


	
#
# shows the current mail settings
#
sub display ($) {

	my $check=$_[0];

	our ($cgipaf_name,$homedir);

	my  %mail_settings=read_mailcfg();
	my $error = 0;
	
	my  %error_messages = (
		'forwardto'	=>	'&nbsp',
		'autoreply'	=>	'&nbsp',
		'subject'	=>	'&nbsp'
	);

	if ($check) {

	   #
	   # get the HTTP param
	   #

	   
	   $mail_settings{'forward'} 		= param('forward');
	   $mail_settings{'forwardto'} 		= param('forward_to');
	   $mail_settings{'keepmsg'} 		= param('keep_msg');
	   $mail_settings{'autoreply'} 		= param('autoreply');
	   $mail_settings{'subject'} 		= param('subject');


	   if ( ! ( $mail_settings{'vacationstxt'} = param('autoreply_msg') ) ) {

	   	$mail_settings{'vacationstxt'} ="";

	   }

	   clear_undefined(\%mail_settings);

	   yesno_to_bool(\%mail_settings,'forward');
	   yesno_to_bool(\%mail_settings,'keepmsg');
	   yesno_to_bool(\%mail_settings,'autoreply');


	   #
	   # if forward is enabled we need a forwardto address
	   #

	   if ($mail_settings{'forward'}) {
			
	      if (!check_emailaddress($mail_settings{'forwardto'})) {

		    $error=1;
		    $error_messages{'forwardto'}=
			"<b><font color=\"red\">Invalid forward to email address</font></b>";

	      }

	   }

	   # 
	   # if autoreply is enabled we need a message
	   #

	   if ($mail_settings{'autoreply'}) {

	   	if ( $mail_settings{'subject'} =~ /^\s*$/) {
		     
		     $error=1;
		     $error_messages{'subject'}="<b><font color=\"red\">Subject is required</font></b>";

		}

	   	if ( $mail_settings{'vacationstxt'} eq "" ) {

		     $error=1;
		     $error_messages{'autoreply'}="<b><font color=\"red\">No autoreply message</font></b>";

		}

	   }

	   #
	   # if no error save mailcfg and exit
	   #

	   if(!$error) {

	   	save_mailcfg(\%mail_settings);
		print h1("Mail configuration updated succesfully.");
		print end_html;
		exit(0);

	   }

	}



	print h1("$BODY_TITLE");
	print "<hr />\n";

	print start_form('POST',"$MAILCFG");

	print table(Tr({ -align => 'left' },  [
	               td( [ 
		             "$TXT_LOGIN", 
			     "$cgipaf_name",
			     hidden('name',$cgipaf_name)
			   ] )
		         ]
		      )
		    ); 

	print "<hr />\n";

	print table(Tr({ -align => 'left'}, 
                 [ td( [ "$TXT_FORWARD", 
	                 popup_menu('forward',yesno(\%mail_settings,'forward')),
		         "$TXT_FORWARD_TO" ,
		         textfield('forward_to',$mail_settings{'forwardto'},30,60),
			 $error_messages{'forwardto'}
		       ]
		     ),
		   td( [ "$TXT_KEEP_MSGS",
		         popup_menu('keep_msg',yesno(\%mail_settings,'keepmsg'))
		       ]

		     )
	         ] 
	      )
	   );

	print "<hr />\n";

	print table(Tr({ -align => 'left'}, [
		 td( [ "$TXT_AUTOREPLY",
		 	 popup_menu('autoreply',yesno(\%mail_settings,'autoreply')),
		       ]
		     ),
		 td( [ 
		       "$TXT_SUBJECT",
		       textfield('subject',$mail_settings{'subject'},20,30),
		       $error_messages{'subject'}
		     ]
		    )
		 ]
	       )
	    );

	
	print "$TXT_AUTOREPLY_MSG&nbsp;$error_messages{'autoreply'}";
	print br;

	print textarea('autoreply_msg',$mail_settings{'vacationstxt'},'10','60');
	print "\n<center>\n";
	print p(
         	table(Tr({ -align => 'center'},
                 	[ td( [ submit('submit','Submit'),reset('reset','Clear') ] ) ]
			)
		     )
	       );
	print "\n</center>\n";


	print end_html;

}




our $homedir="";
our $statefile="";
our $cgipaf_name="";
our $vacationstxtfile="";
my @pw_field=();

print header;
print start_html(
	-title 		=> "$TITLE",
	-bgcolor	=> '#B0B0B0',
	-text		=> '#000000');

if ($cgipaf_name = $cgi->param('name')) {

	@pw_field = getpwnam($cgipaf_name);
	if (!@pw_field ) {
		print_error("getpwname failed");
	}
	$homedir=$pw_field[7];
	$statefile="$homedir/$STATEFILENAME";
	$vacationstxtfile="$homedir/$VACATIONS_TXT";

}
else {
	print_error("Can't read loginname");
}

if (@ARGV == 2) {

   if ($ARGV[0] eq "update") {
		
		our $cgipaf_domain=$ARGV[1];

		display(1);
		exit(0);

   }

}

display(0);

