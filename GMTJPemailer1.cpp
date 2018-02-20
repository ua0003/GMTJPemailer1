///A command line program used for sending emails.

///include statements
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <gmime/gmime.h>
#include <glib-2.0/glib.h>
#include <curl/curl.h>

#include <string>
#include <cstring>

#include <getopt.h>  //used for arg parsing options
#include <cstdlib>
#include <fstream> //used for binary serialization
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <list>
#include <vector>
#include <sstream>
#include <termios.h>
#include <memory>
#include <typeinfo> //used for troubleshooting

///structure to hold smtp information
struct smtpConfig
{
	char name[50];
	char port[5];
	char user_name[50];
	std::string password;
	std::string from_name;
	char from_email_address[60];

    template <class Archive>
    void serialize( Archive & ar )
        {
            ar( name, port, user_name, password, from_name, from_email_address  );
        }
};

//variables
using namespace std;
const char* program_name;
string to;
string cc;
string bcc;
string subject;
string msg;
string internationalCoding;
string attachment;
static smtpConfig smtpObj;

//print usage statement for help or incorrect options
void print_usage(ostream& os, int exit_code)
	{
		cout << "\nUsage : "<<program_name<< "  -t <to_email> -c <cc_email> -b <bcc_email> -s <subject> -m <message> -a <attachment_path>\n\n"
		 <<endl<<program_name<< " -f <config_prompts>\n"<<endl<< program_name << endl<<endl;
		cout << " -h --help Display this usage information." << endl
		<< " -v --version Display the version of the application." << endl
		<< " -t --to Requires an argument, email address of recipient." << endl
		<< " -c --cc Requires an argument, email address to be CC'd." <<endl
		<< " -b --bcc Requires an argument, email address to be blind copied." <<endl
		<< " -s --subject Requires an argument, email subject. (enter in '' )" <<endl
		<< " -m --message Requires an argument, email text//body. (enter in '')" <<endl
		<< " -i --international Requires an argument, choose a UTF-8 character set. " <<endl
//		<< "   TODO ADD LIST OF POSSIBLE CHARACTHER SETS       "
		<< " -f --configure Used to setup initially. Will request SMTP server name, port number, your name, email address, user name, and password." <<endl;

		exit (exit_code);
	}

///function prototypes
smtpConfig configSMTP();
//int mainCurl(_GMimePart*);
int mainCurl(smtpConfig,FILE* tmpf, size_t payload_text_len);
int getch();
string getpass(const char *prompt, bool);
GMimePart* mainGmime(string msg);
static size_t payload_source(char *ptr, size_t size, size_t nmemb, const char *userp);


template<class Archive>
void save(Archive & archive, smtpConfig const & m)
    {
        archive( m.name, m.port, m.user_name, m.from_email_address, m.from_name, m.password );
    }
template<class Archive>
void load(Archive & archive, smtpConfig & m)
    {
        archive( m.name, m.port, m.user_name, m.from_email_address, m.from_name, m.password);
    }
///MAIN

int main(int argc, char* argv[])
{
	int optionCount;
	//setup short options and whether they take arguments
	const char* const short_options= "hvt:c:b:s:m:i:f";
	//setup long options for verbose
	const struct option long_options[] =
	{
		{ "help", 0, 0, 'h' },
		{ "version", 0, 0, 'v' },
		{ "to", 1, NULL, 't' },
		{ "cc", 2, NULL, 'c'},
		{ "bcc", 2, NULL, 'b'},
		{ "subject", 1, NULL, 's'},
		{ "message", 1, NULL, 'm'},
		{ "international",1,NULL, 'i'},
		{ "configure", 0, 0, 'f'},

		{ NULL, 0, NULL, 0 }
	};
	program_name = argv[0];
//begin do - while loop
	do
	{
		optionCount = getopt_long (argc,argv,short_options,long_options,NULL);

		switch (optionCount)
		{
			cout<<optionCount;
			case 'h':
			print_usage(cout, 0);

			case 'v':
			cout<< "This is version 1.0 of GMTJPemailer."<<endl;
			exit (0);

			case 't':
			to = optarg;
			break;

			case 'c':
			cc = optarg;
			break;

			case 'b':
			bcc = optarg;
			break;

			case 's':
			subject = optarg;
			break;

			case 'm':
			msg = optarg;
			break;

			case 'i':
			internationalCoding = optarg;
			break;

			case 'f':
//smtpConfig theSMTPconfig;
            smtpObj = configSMTP();
			break;

			case '?':
			print_usage (cerr, 1);

			case -1:
			break;

			default:
			abort();
		}
	}
	while (optionCount != -1);

///Check for serial data

    if(smtpObj.password.length()>0)

        {
            cout<<"smtpObj pass greater than 0"<<endl;
            std::ofstream os("GMTJPemailer.config", std::ios::binary);
            cereal::PortableBinaryOutputArchive archive( os );
            archive(smtpObj.name, smtpObj.port, smtpObj.user_name, smtpObj.password, smtpObj.from_name, smtpObj.from_email_address  );
            cout<<"Serializing smtp config data complete."<<endl;
        }

    if(smtpObj.password.length()<1)
        {
            try
                {
            ifstream is("GMTJPemailer.config", std::ios::binary);
            cereal::PortableBinaryInputArchive archive(is);
            archive(  smtpObj.name, smtpObj.port, smtpObj.user_name, smtpObj.password, smtpObj.from_name, smtpObj.from_email_address );
            cout<<"SMTP config data retreived."<<endl;
                }
            catch(const std::exception&)
//            cout<<"Something went wrong!"<<endl;
                {
                    cout<<"Something went wrong...let's gather some information."<<endl;
                    smtpObj = configSMTP();
                };
        }
       ///template for email
       //setup to from cc
string TO = string (to.c_str());
string FROM = string (smtpObj.from_email_address);
string CC = string (cc.c_str());

//use GMime to format the date.
 GDateTime *forGmime = g_date_time_new_now_local ();
 char* forlibcurl = g_mime_utils_header_format_date(forGmime);



     const char payload_template[] =
        "Date: %s\r\n"
        "To: %s\r\n"
        "From: %s\r\n"
        "Subject: %s\r\n"
        "\r\n"
        "%s\r\n\r\n";
//declare variable for payload text length used for curl
size_t payload_text_len;
         payload_text_len = strlen(payload_template) +
                                  strlen(forlibcurl) + strlen(subject.c_str()) +
                                  strlen(TO.c_str()) + strlen(FROM.c_str()) +
                                  strlen(msg.c_str()) + 1;

//        void* payload_text = /*(char*)*/ malloc(payload_text_len);
        char* payload_text = (char*) malloc(payload_text_len);
//        char * payload_text = new char[payload_text_len];
//        char* memset(payload_text, 0, payload_text_len);
//         char* payload_text;
//        memset(payload_text,0,payload_text_len);
//        int snprintCheck = snprintf((char*)payload_text, payload_text_len,payload_template,const_cast<const char*>(forlibcurl),TO.c_str()
//        ,FROM.c_str(),subject.c_str(),msg.c_str());
        int sprintCheck = snprintf((char*)payload_text,payload_text_len,payload_template,const_cast<const char*>(forlibcurl),TO.c_str()
        ,FROM.c_str(),subject.c_str(),msg.c_str());
        FILE *tmpf = tmpfile();
        fputs(const_cast<const char*>(payload_text),tmpf);
        rewind(tmpf);

/////////////////////////////////////////////////////////////////////////////



//struct upload_status upload_ctx;

  g_print("Setting the memory needed.");
//  memset(payload_text,0,payload_text_len);
//  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
//    return 0;
//  }


//    delete[]payload_text;

//  if(payload_text)
//    {
//        if (payload_text_len > nmemb * size)
//            {
//                payload_text_len = 0;
//                g_print("Your message is too big!");
//                return payload_text_len;
//            }
//        if (payload_text_len)
//            {
////                memcpy(ptr, payload_text, payload_text_len);
////                upload_ctx->lines_read++;
//                g_print("Data structure loaded.");
//                return payload_text_len;
//            }
//        return payload_text_len;
//    }
char* ptr[payload_text_len] =  {payload_text};
size_t sized = 1;
cout<<"Payload text is:"<<endl;
cout<<*ptr<<endl;
//payload_source(*ptr,sized,payload_text_len,(const char*)payload_text);
///    mainCurl
//check if there is a to address before proceeding.
    cout<<"Payload text length is: "<<payload_text_len<<endl;
    if(strlen(to.c_str())<1)
        {
            cout<<"SMTP configured"<<endl;
            return 0;
        }
    else
        {
            mainCurl(smtpObj,tmpf,payload_text_len);

        }
    free(payload_text);
	return 0;

}
//function definition
struct upload_status {
  int lines_read;
};
smtpConfig configSMTP()
{

	smtpConfig mysmtp {};
	int checker;
	checker = 1;
	GRegex* smtpRegex;
	GMatchInfo *match_info;
	//regex...use posix...no \w character!
	smtpRegex = g_regex_new("((smtp|mail)\\.[a-z.]+\\.(com|net))",G_REGEX_CASELESS,G_REGEX_MATCH_PARTIAL,NULL);
    //while loop to check and see if smtp address entered correctly or with typo. i.e. smtp,gmail.com
	while (checker == 1)
	{
        cout<<"Please enter the SMTP address. i.e. smtp.gmail.com"<<endl;
        cin>> mysmtp.name;
        cout<<endl;
        cout<<"You entered: "<<mysmtp.name<<endl;

        if(TRUE == g_regex_match(smtpRegex,mysmtp.name,G_REGEX_MATCH_ANCHORED,&match_info))
        {
            g_print("SMTP server address accepted.\n");
            checker = 0;
        }
        else
        {
            g_print("That doesn't look like it will work. Let's try that again.\n");
        }
	}

//get rest of data
//check to make sure that the port number is either 2 or 3 digits.
    checker = 1;
    while(checker ==1)
    {
        cout<<endl<<"Please enter the SMTP port number. i.e. 465"<<endl;
        cin >>mysmtp.port;
        cin.get();
        char * x {mysmtp.port};
        int len = strlen(x);

        if(len == 2||
            len == 3)

        {
            g_print("You entered a port number of acceptable length. Adding to SMTP object.\n\n");
            checker = 0;
        }
        else
        {
            g_print("That doesn't look like it will work. Let's try that again.\n\n");

        }
    }
    int arSize = 50;
	cout <<"Please enter your name. i.e. 'John Smith' (Don't forget the ' '. "<<endl;
	cin.getline(mysmtp.user_name,arSize);
	cout<<endl;

    checker = 1;
	GRegex* emailRegex;
	GMatchInfo *match_info2;
	//rege-x...https://www.wired.com/2008/08/four_regular_expressions_to_check_email_addresses/
	emailRegex = g_regex_new("([a-z0-9][-a-z0-9_+.]*[a-z0-9])@([a-z0-9][-a-z0-9.]*[a-z0-9]\\.(arpa|root|aero|biz|cat|com|coop|edu|gov|info|int|jobs|mil|mobi|museum|name|net|org|pro|tel|travel|ac|ad|ae|af|ag|ai|al|am|an|ao|aq|ar|as|at|au|aw|ax|az|ba|bb|bd|be|bf|bg|bh|bi|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|cr|cu|cv|cx|cy|cz|de|dj|dk|dm|do|dz|ec|ee|eg|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gg|gh|gi|gl|gm|gn|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|im|in|io|iq|ir|is|it|je|jm|jo|jp|ke|kg|kh|ki|km|kn|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|me|mg|mh|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|mv|mw|mx|my|mz|na|nc|ne|nf|ng|ni|nl|no|np|nr|nu|nz|om|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|ps|pt|pw|py|qa|re|ro|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|st|su|sv|sy|sz|tc|td|tf|tg|th|tj|tk|tl|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|um|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zw)|([0-9]{1,3}\\.{3}[0-9]{1,3}))",G_REGEX_CASELESS,G_REGEX_MATCH_PARTIAL,NULL);
    //while loop to check and see if email address entered looks legit.
	while (checker == 1)
	{
        cout<<"Please enter the email address you will be sending from."<<endl;
        cin >>mysmtp.from_email_address;
        cout<<endl;
        cout<<"You entered: "<<mysmtp.from_email_address<<endl;

        if(TRUE == g_regex_match(emailRegex,mysmtp.from_email_address,G_REGEX_MATCH_ANCHORED,&match_info2))
        {
            g_print("Your email address looks good.\n");
            checker = 0;
        }
        else
        {
            g_print("That doesn't look like it will work. Let's try that again.\n");
        }
    }
	cout<<"Please enter the username you use to access your email account."<<endl;
	cin>>mysmtp.from_name;
    cout<<endl;

	string prompt ="Please enter the password that you use to access your email account.\n";

	mysmtp.password = getpass(prompt.c_str());
//    mysmtp.password = getpass("Please enter the password: ",true);

	return mysmtp;//mysmtp;
}

//static size_t payload_source(char *ptr, size_t sized, size_t nmemb, const char *userp)
//{
//  struct upload_status *upload_ctx = (struct upload_status *)userp;
//  const char *data;
//
//  if((sized == 0) || (nmemb == 0) || ((sized*nmemb) < 1))
//    {
//        return 0;
//    }
//    cout<<nmemb<<endl;
//
////    cout<<typeid(userp).name<<endl;
//    cout<<sized<<endl;
//    cout<<*ptr<<endl;
////  data = (const char*)&userp[upload_ctx->lines_read];
////    data = **userp[upload_ctx->lines_read];
//  if(data) {
//    size_t len = strlen(data);
//    memcpy(ptr, data, len);
//    upload_ctx->lines_read++;
//
//    return len;
//  }
//
//  return 0;
//}

///int mainCurl
int mainCurl(smtpConfig,FILE* tmpf,size_t payload_text_len)
{
//
//void *ptr;
//size_t size;
//size_t nmemb;
//void *userp;
//  struct upload_status *upload_ctx = (struct upload_status *)payload_text;
//  const char *data = (const char*)payload_text[upload_ctx->lines_read];
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
//  struct upload_status upload_ctx;

//  upload_ctx->lines_read = 0;

  curl = curl_easy_init();
  if(curl) {
    string preformatted = "smtps://";
    string smtpName = string (smtpObj.name);
    string smtpPort = smtpObj.port;
    string formatted =  preformatted+smtpName+":"+smtpPort;
//    char * usrNam = smtpObj.user_name;
//    char * passWord = smtpObj.password;
    string fromEmail = smtpObj.from_email_address;


    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, smtpObj.from_name.c_str());

    curl_easy_setopt(curl, CURLOPT_PASSWORD, smtpObj.password.c_str());
    /* This is the URL for your mailserver. Note the use of port 587 here,
     * instead of the normal SMTP port (25). Port 587 is commonly used for
     * secure mail submission (see RFC4403), but you should use whatever
     * matches your server configuration. */

    curl_easy_setopt(curl,CURLOPT_URL,formatted.c_str());

    /* In this example, we'll start with a plin text connection, and upgrade
     * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
     * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
     * will continue anyway - see the security discussion in the libcurl
     * tutorial for more details. */
    //maybe don't need this
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

    /* Note that this option isn't strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromEmail.c_str());

    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
       if(strlen(to.c_str())>0)
        {
            recipients = curl_slist_append(recipients, to.c_str());
            cout<<"Sending email to: "<<to<<endl;
        }
        if(strlen(cc.c_str())>0)
        {
            recipients = curl_slist_append(recipients, cc.c_str());
            cout<<"CC'ing email to: "<<cc<<endl;
        }
        if(strlen(bcc.c_str())>0)
        {
            recipients = curl_slist_append(recipients, bcc.c_str());
            cout<<"BCC'ing email to: "<<bcc<<endl;
        }
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    /* We're using a callback function to specify the payload (the headers and
     * body of the message). You could just use the CURLOPT_READDATA option to
     * specify a FILE pointer to read from. */
//    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
//    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_READDATA, tmpf);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* Since the traffic will be encrypted, it is very useful to turn on debug
     * information within libcurl to see what is happening during the transfer.
     */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* Send the message */
    std::cout<<"About to send the email!"<<endl;
//    std::cout<<*payload
    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* Free the list of recipients */
    curl_slist_free_all(recipients);

    /* Always cleanup */
    curl_easy_cleanup(curl);
  }

  return (int)res;
}

///mask password
/////https://stackoverflow.com/questions/6856635/hide-password-input-on-terminal
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
