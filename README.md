# GMTJPemailer1

## ABOUT

GMTJPEmailer1 is a command line email program.  It’s meant to be a convenient way to send emails from the command line so you don’t have to open up a browser window while working on the command line.  It’s also beneficial in that you can send a quick email without getting sucked into reading anything in your inbox. 

### Background
GMTJP stands for Give Me The Job Please.  The program was designed to meet the requirements as a technical assessment  of my coding skills for a position in Konica Minolta’s SEC group. Emailer is part of the name as a way to describe what the program does and ‘1’ is because this is the second C/C++ program I have ever written. The first was the obligatory ‘Hello Word’ program.

## USING
GMTJPemailer1 is designed to be simple to use. It uses switches, a list of which that can be brought up using the -h or - - help option.

 -v --version Display the version of the application.
 
 -t --to Requires an argument, email address of recipient. 
 
 -c --cc Requires an argument, the option it’s self is optional, email address to be CC'd.
 
 -b --bcc Requires an argument, the option it’s self is optional, email address to be blind copied.
 
 -s --subject Requires an argument, email subject. (Enter in ' ' )

 -m --message Requires an argument, email text/body. (Enter in ' ')
 
-f --configure Used to setup initially. Configure will request your SMTP server name, port number, your name, email address, user name, and password. This information will be used later to send your emails.  If you do not use this option first the program will take you through the SMTP setup automatically. The configuration process will create a file called GMTJPemailer.config to store the SMTP data.  

You can enter multiple email addresses for the to, cc, and bcc field by entering them in quotes and separating them by a space. 

For Example: 
```
-t ‘myemail@somemail.com otheremail@diffemail.com’ 
```

A full example of a what an email might look like:
```
GMTJPemailer1 -t 'someone@notarealemail.com yetanother@stillnotanemail.com' -c pretendperson@someotheremail.com -b myotheremail@notreally.com -s 'Hello there!' -m 'Hi,

How are you doing?

Thanks,
NotmyName'
```
*IMPORTANT this program assumes a SSL connection will be used.  This means you can use the program with GMAIL.*

### Demo
For demo purposes also make sure that the GMTJPemailer.config is in the same working directory as GMTJPemailer1. This file will allow you to send emails from a per-configured email address.

For demo purposes the following information can be used to test the --configure function:

- **SMTP Server Name:** smtp.gmail.com
- **Port Number:** 465
- **Email Address:** gmtjpemailer@gmail.com
- **Sign in Name:** gmtjpemailer
- **Password:** Konica18

## Requirements

**Operating System:** Linux – Debian based distro used for development.  Unknown whether other Linux environments will work.  
**Libraries:** Glib, GMIME, Cereal, CURL

**GLIB:**  version 2.0,  GNU LGPL,  https://github.com/GNOME/glib (Required for parameters 	of assignment.) A helpful link for installing and building if needed is below. 

http://linuxg.net/how-to-install-glib-2-36-3-on-ubuntu-and-linux-mint/
  
**GMIME:** version 3.2.0, GNU Lesser General Public License 			https://github.com/GNOME/gmime (Required for parameters of assignment.) Used for Date 	formatting. 

**CEREAL:** version 1.2.2  BSD License https://uscilab.github.io/cereal/ Used for serializing 	data.

**CURL:** version 7.57.0  MIT/X derivate license (link to license 	https://curl.haxx.se/docs/copyright.html) https://curl.haxx.se/dev/source.html Used for 	SMTP protocol.

**Compiler:** Minimum g++ 4.7.3, clang++ 3.3.  It needs to be C++11 compliant.

Code::Blocks 13.12 was used for development
  
## Compiling
#### Using Code::Blocks  (Can use similar steps with other programs.)

In compiler setting select “Have g++ follow the comping C++0x ISO C++ language standard [-std=c++0x]

In Linker settings add:

- -L/usr/local/lib -lglib-2.0
- -L/usr/local/lib -gmime-3.0
- -L/usr/local/lib -lcurl
- -L/usr/local/lib -lgmime-3.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0

*locations of your libraries may differ.*

Running the following commands will help identify Include and Linking paths
```
pkg-config --cflags --libs glib-2.0 
pkg-config --cflags --libs gmime-3.0
curl-config –libs
```
*Cereal is a header only library and does not require linking.*

In Search Directories enter:

The include paths provided by the previous statements. Also include the directory that you put Cereal in.

Finally, load the GMTJPemailer1.cpp file into Code::Blocks and press Build.

## DESIGN
### Overview
Conceptually I started working on this project with the thought that C++ was VERY similar to Python...which was ridiculously naive of me.   With that in mind the program was written in one file instead of separating things out into separate source and header files.  I never would have conceived of the idea of putting things in separate files to be compiled together later. Now, however I understand this would have made sense and would have made finding things in my code easier. Additionally, by the end of the project I came to the realization that I had written most of the code in C with a few interjections of C++ and writing more of it in C++ would have probably made my life easier.  

### The Code
#### Organization
The code is organized as outlined below:
 1. Include statements
 2. Custom data structures
 3. Some variable declarations
 4. Usage statement for command line options
 5. Function prototypes
 6. Main
 
  a) Setup command line options
  
  b) do while loop for command line options
  
  c) Check to see if SMTP data needs to be serialized
  
   - Serialize or load data
     
  d) Clean up email addresses to be used in MIME formatted email file.
  
  e) Calculate date/time with GMIME for email.
  
  f) Load email file with command line options (payloadCreate function)
  
  g) Check to make sure a “TO” address was entered
  
  h) Send email
  
 7. Function definitions
 
  a) configSMTP
  
   - Used for collecting the data needed for SMTP.

  b) mainCurl
  
   - Uses SMTP data to talk to and send email.

  c) getch
  
   - Used to mask the password entered by the user.

  d) payloadCreate
  
   - Used to format information from command line for MIME formatted email file.

### Sample Code
The below is links to and sample code I used to model my code along with sources I used for different parts of the code. 

**GETOPT**

For setting up the switches I followed the code at the following site:
  
https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html

**DATA SERIALIZATION**

For serializing the data I used CEREAL both because of the cheesy graphic on their home page and most importantly ease of use.  The other added benefit of using Cereal was that the SMTP data would be stored in a binary file, which makes it slightly more difficult for someone to find the password to your email account.  I was able to follow the below code to serialize the SMTP data, which can be found at:  https://uscilab.github.io/cereal/quickstart.html
```
#include <cereal/archives/binary.hpp>
#include <sstream>

int main()
{
  std::stringstream ss; // any stream can be used

  {
    cereal::BinaryOutputArchive oarchive(ss); // Create an output archive

    MyData m1, m2, m3;
    oarchive(m1, m2, m3); // Write the data to the archive
  } // archive goes out of scope, ensuring all contents are flushed

  {
    cereal::BinaryInputArchive iarchive(ss); // Create an input archive

    MyData m1, m2, m3;
    iarchive(m1, m2, m3); // Read the data from the archive
  }
}
```
**CURL**

For sending the SMTP email I decided to use CURL and follow the example found at:
https://curl.haxx.se/libcurl/c/smtp-ssl.html

This proved to be too rigid and the following proved to be super useful for fixing this. Found at:
https://stackoverflow.com/questions/48383627/how-to-send-email-with-a-variable-declaration
```
const char payload_template[] =
        "Date: %s\r\n"
        "To: %s\r\n"
        "From: %s\r\n"
        "Message-ID: <%s@example.com>\r\n"
        "Subject: Message Test\r\n"
        "\r\n"
        "Day %d Welcome %s\r\n\r\n";
```
Followed by a modification to the sprintf()
```
sprintf(payload_text, payload_template, time_buffer, to,
        smtp_from, message_id, tm->tm_mday, user_name);
```
*An important note while reading about MIME standards I learned that while using most major email providers like Google, Yahoo, Hotmail the message ID is not needed as they take care of this detail.*

**GETCH**

For password masking I used the following code found on: http://www.cplusplus.com/articles/E6vU7k9E/:
```
	int getch() {
    int ch;
    struct termios t_old, t_new;

    tcgetattr(STDIN_FILENO, &t_old);
    t_new = t_old;
    t_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
    return ch;
}
```
**REGEX**

To validate email addresses I used method 3 , with some modifications, found at: https://www.wired.com/2008/08/four_regular_expressions_to_check_email_addresses/

**MULTIPLE EMAIL ADDRESSES**

In order to solve the issue of allowing multiple email addresses to be used for a single option like TO, CC, or BCC I needed a way to easily enter them on the command line and have the program separate out the email addresses to be loaded.  To accomplish this I used the below snippet of code which was found at https://www.quora.com/How-do-I-split-a-string-by-space-into-an-array-in-c++
```
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

int main()
{
    std::string s = "split on    whitespace   ";
    std::vector<std::string> result;

    std::istringstream iss(s);
    for(std::string s; iss >> s; )
        result.push_back(s);

    for(auto& s: result)
        std::cout << std::quoted(s) << '\n';
}
```

## OTHER DECISIONS

**g_print():**  This was used in parts of the code to ensure that I was using some part of GLIB other than for using GMIME.

**GMIME:** GMIME was used to generate a correctly formatted MIME date, which seems like overkill but so was anything GMIME did for this project.  I found that although the documentation was voluminous it was obtuse and there was very little third party documentation that I could find.  Looking back learning more about input/output streams and having GMIME write to a file might have worked but I had already spent far too many hours with it to consider a “might”.  

**CURL:**  Although most examples I found showed using the payload_source function in various forms I found this to be impossible to adapt for my purposes. It was cumbersome and in the end I could never figure out just how it would work without everything being a global variable. Using the temporary file method proved to be infinitely simpler.

**GETCH:**  Implementing a password masking with * would have been preferable but I liked the simplicity of the sample code I found, and most importantly it masks the users password adding a layer of protection from over the shoulder snoops.

## Troubleshooting
- If you are using two step verification be sure to create an app id for this program.  Instructions for how to do this can be found at: https://support.google.com/accounts/answer/185833?hl=en 
- Use the following on the command line ```ldconfig``` after loading new libraries.
- If the program hangs during the first run. Try re-running the command. 
- Don't use Valgrind while running this program.  There seems to be an error with Valgrind and Valgrind will crash without sending your email.
