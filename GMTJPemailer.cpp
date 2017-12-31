//A command line program used for sending emails.

#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

static int verbose_flag; // --maybe don't need this? */
//set name space
using namespace std;	

int main(int argc, char *argv[])
{
	//ArgCount
	int c;
	while(1)
	{
		static struct option long_options[] =
		{
			//flagged options
			//name, requires argument (0 = no, 1 = yes, 2 = optional),flag, value
			
			{"verbose", 0, &verbose_flag, 1},
			{"brief", 0, &verbose_flag, 0},
			//other options
			{"help", 0, 0, 'h'},
			{"version", 0, 0, 'v'},
			{"to", 1, 0, 't'},
//TODO: add other options:
//cc - c
//bcc - b
//subject - s
//message - m
//attachment -a
//configure - c
			{0,0,0,0}
		};
	int option_index = 0;
	
	c = getopt_long(argc,argv,"hvt:", long_options, &option_index);
	
	//Detect the end of options.
	if(c== -1)
		break;
		
	switch(c)
		{
		case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
            break;
        cout << ("option %s", long_options[option_index].name);
        if (optarg)
            cout << (" with arg %s", optarg)<<endl;
            break;
          
        case "h":
			break;
		case "v":
			break;
		case "t":
			break;
		}
	}
	
	cout << "Hello World" <<endl;
	cout << "This is my second line."<<endl;
	cout << "Here's a third line.";
	return 0;
}

