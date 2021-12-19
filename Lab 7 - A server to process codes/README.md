# cs140 (Spring 2020)

# Lab Description
You and your friends Rad and Banjolina decide to go into business providing web and cell phone support for reward programs like mycokerewards.com. Users can set up accounts with you that will maintain points. Users can accumulate points by collecting codes from various products (such as bottlecaps and 12-packs, as in mycokerewards.com), and then they can spend the points on various prizes.

Users can enter codes via a web site, or they can register one or more cell phones with their account, and then they can text codes to a given phone number, which will register the points.

Rad is handling the business and marketing end of this endeavor, and Banjolina is doing all of the web programming. Your job is to write the server that maintains information about users, prizes and codes, and talks with Banjolina's web front-end. Since you haven't taken CS360 yet, your server won't do any real networking. Instead, it will communicate via files and standard input.

As with many of our labs, I give you a header file that defines a class, and you have to implement the methods. I have a driver program that you compile with your code, and that will be the final product.

Here's the header, in code_processor.h:

```
#include <set>
#include <deque>
#include <map>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

class User {
  public:
     string username;
     string realname;
     int points;
     set <string> phone_numbers;
};

class Prize {
  public:
     string id;
     string description;
     int points;
     int quantity;
};

class Code_Processor {
  public:
    int New_Prize(string id, string description, int points, int quantity);
    int New_User(string username, string realname, int starting_points);
    int Delete_User(string username);

    int Add_Phone(string username, string phone);
    int Remove_Phone(string username, string phone);
    string Show_Phones(string username);

    int Enter_Code(string username, string code);
    int Text_Code(string phone, string code);
    int Mark_Code_Used(string code);

    int Balance(string username);
    int Redeem_Prize(string username, string prize);
    
    ~Code_Processor();
    int Write(const char *file);

    void Double_Check_Internals();   /* You don't write this */

  protected:
    map <string, User *> Names;
    map <string, User *> Phones;
    set <string> Codes;
    map <string, Prize *> Prizes;
};
```

While this looks like a mouthful, it's not too bad. Users store the following data:
* A username, which is a one-word name.
* A realname, which is the users's full name. For example, my username might be "jimbvz" and my real name "Jim Plank".
* The total number of points that the user has.
* A set containing the phone numbers registered to the user. Phone numbers are stored simply as strings. When you manipulate this set, either by adding or removing strings, make sure you do not create extra copies of the set. That means if you have a pointer to a User record named user, you are not allowed to write:
```
phoneNums = user->phone_numbers;
phoneNums.insert(...);   // bug!!!--inserts into the copy rather than into user's phone_numbers
```
Instead you must write:
```
user->phone_numbers.insert(...)
```
The TAs will check your code and if they find you are making copies of phone_numbers, you will receive a deduction.
Prizes store the following data:

  * An id. This is a unique string for each prize.
  * A description. This is a longer description of each prize.
  * The number of points that it takes to get one prize.
  * The quantity of prizes.

A Code_Processor keeps track of Users, Codes and Prizes. Users are stored in the map Names, which is keyed on their usernames. Phone numbers are stored in the map Phones, which is keyed on the phone numbers, and whose second field points to the user that has registered the cell phone.

There is a set Codes, which stores the codes that have been entered by all users. This set exists so that users can't enter a code more than once. Finally, there is a map Prizes, keyed on the id of each prize.

You'll note that both Names and Phones point to users. In other words, each user has just one User instance, and that is pointed to both in Names and in Phones. If the user has multiple phones, then there will be multiple entries in Phones that point to that user. Moreover, there are two data structures that hold phones -- Phones, which is keyed on the phone number, and the set phone_numbers which is part of the User's data.

Now, you are to write the following methods:

* New_Prize(string id, string description, int points, int quantity): This creates a new prize and puts it into Prizes. You should return zero if all is ok. You should return -1 from the following errors without creating anything new:
  * There is already a prize with the given id in prizes.
  * Points is less than or equal to zero.
  * Quantity is less than or equal to zero.
* New_User(string username, string realname, int starting_points): This creates a new user with the given information, and puts it into Names. The user will start with no registered phone numbers. You should return zero if all is ok. You should return -1 from the following errors without creating anything new:
  * There is already a user with that username.
  * Starting_points is less than zero.
* Delete_User(string username): This should erase the user from Names, and it should erase all of the user's phone numbers from Phones. After that, it should call delete on the user's pointer. You should return zero if all is ok. You should return -1 if the username is not in Names.
* Add_Phone(string username, string phone): This should register the given phone string with the user. That means putting the phone on both the Phones map, and on the user's phone_numbers set. You should return zero if all is ok. You should return -1 from the following errors without creating anything new:
  * There is no user with that username.
  * The phone number is already registered, either with that user or with another.
* Remove_Phone(string username, string phone): This should remove the phone from the system -- both from Phones and from the user's phone_numbers set. You should return zero if all is ok. You should return -1 from the following errors without performing any system modifications:
  * There is no user with that username.
  * There is no phone string with that phone.
  * The phone number is registered to a different user.
* Show_Phones(string username): This should return a string containing all of that user's phone numbers, in lexicographic order, each separated by a newline. There should be a newline at the end of the string too. If the user doesn't exist, return the string "BAD USER". If the user has no phones, simply return an empty string.
* Enter_Code(string username, string code): This is called when a user enters a code. You need to first check the Codes set to see if the code has been entered before. If it has, or if the user doesn't exist, return -1. Otherwise, you need to check to see if the code is valid: A valid code's djbhash() must either be divisible by 17 or 13. If divisible by 17, then it is worth ten points. Otherwise, if it is divisible by 13, then it is worth three points. If the code is valid, then add it to Codes, increment the user's account and return the number of points added. If the code is not valid, simply return zero.
* Text_Code(string phone, string code): This should work just like Enter_Code(), except the user's account is identified by the phone number. If the phone number doesn't exist, return -1. Otherwise, this should work just like Enter_Code().
* Mark_Code_Used(string code): This is called to mark a code as used, even though no user is entering it. This is used to help rebuild the server from a saved state (see Write() below). If the code is not valid or it is already in Codes, return -1. Otherwise, add it to Codes and return 0.
* Balance(string username): This should return the user's points. If the user doesn't exist, return -1.
* Redeem_Prize(string username, string prize): This is called when a user wants to redeem a prize. The prize is identified by its id. If the user or prize don't exist, or if the user doesn't have enough points, return -1. Otherwise, decrement the points from the user's account, and decrement the prize's quantity by one. If the prize's quantity is zero, remove the prize from the system (which should involve a delete call).
* ~Code_Processor(): Since new is called to create users and prizes, you need to write a destructor that calls delete on all the users and prizes. The destructor doesn't have to clear the maps or sets -- that will be done automatically for you when the Code_Processor goes away. If you don't understand this point, please ask about it in class.
* I'll describe Write() later.
* You don't write Double_Check_Internals(). I have written it, and it is in double_checker.cpp, which you must include when you compile your program. My makefile does this for you. The intent of Double_Check_Internals() is to make sure that your treatment of phone numbers is consistent, and that you have closed any open files.

# cp_tester.cpp  
The program cp_tester.cpp is a front end for code_processor.cpp. You call it with filenames on the command line argument, and it reads files that have commands to execute on a Code_Processor. If a filename is "-", it reads the commands from standard input. The commands are specified on separate lines -- blank lines are ok, and lines that begin with a pound sign are ignored. Lines may not have more than 20 words on them.
Otherwise, the following commands are supported:

* "PRIZE id points quantity description": Calls New_Prize() with the given arguments. Id is a single word. Description may be multiple words.
* "ADD_USER username starting_points realname": Calls New_User() with the given arguments. Username must be one word. Realname can contain any number of words.
* "DELETE_USER username": Calls Delete_User with the given username.
* "ADD_PHONE username phone-number": Makes the appropriate Add_Phone() call. Both username and phone-number must be one word.
* "REMOVE_PHONE username phone-number": Makes the appropriate Remove_Phone() call.
* "SHOW_PHONES username": Makes the appropriate Show_Phones() call.
* "ENTER_CODE username code": Makes the appropriate Enter_Code() call. The code should be one word.
* "TEXT_CODE phone code": Makes the appropriate Text_Code() call.
* "MARK_USED code ...": You can specify up to 19 codes on a line. It will call Mark_Code_Used() on each of these codes.
* "BALANCE username": calls Balance() and prints the output.
* "REDEEM username prize": calls Redeem().
* "DOUBLE_CHECK": calls Double_Check_Internals().
* "WRITE filename": calls Write() on the given filename. Explanation below.
* "QUIT": stops reading. You can simply end input too, and that will stop reading.

# Write()
The Write() method is very important. Whenever you write a server like this one, you should make it fault-tolerant. In other words, you should make it so that it can save its state so that you can terminate the server and start it up again later. The Write() method should save the Code_Processor's state to the given file and return 0. Many students forget to return 0 and the C++ compiler won't complain about it. Should you forget to return 0, then the C++ compiler will return an integer that the cp_tester program will interpret as a bad file write and you will get a "cannot write to file ..." message. Write should return -1 if it can't open/create the file.
The format of Write() should be as a file that cp_tester can use as input to recreate the state of the Code_Processor. It should only consist of ADD_USER, PRIZE, ADD_PHONE and MARK_USED lines, and when cp_tester is run with the file as input, it should recreate the state of the Code_Processor.

I don't care about the order or format of the lines, as long as they create the proper Code_Processor when they are fed to cp_tester. My grading program will test your files by using them as input to my cp_tester and looking at the output of my Write() call.

# Some examples
Let's start with a very simple example:
```
UNIX> cp_tester -
CP_Tester> ADD_USER tigerwoods 0 Tiger Woods
ADD_USER successful
CP_Tester> ADD_USER the-donald 100 Donald Trump
ADD_USER successful
CP_Tester> PRIZE mp3 40 5000 Free MP3 download from Bapster
PRIZE successful
CP_Tester> PRIZE cancun 10000 1 All expense-paid vacation to Cancun
PRIZE successful
CP_Tester> WRITE cp1.txt
WRITE successful
CP_Tester> QUIT
UNIX> cat cp1.txt
PRIZE     cancun     10000      1 All expense-paid vacation to Cancun
PRIZE     mp3           40   5000 Free MP3 download from Bapster
ADD_USER  the-donald   100 Donald Trump
ADD_USER  tigerwoods     0 Tiger Woods
UNIX> 
```
I've added two prizes and two users, and then written the server's state to cp1.txt. You'll note that the order of cp1.txt is different from my input. That's fine -- if you use it as input to cp_tester, it will create the same server state. For example:
```
UNIX> cp_tester cp1.txt -
CP_Tester> BALANCE tigerwoods
0 Points
CP_Tester> BALANCE the-donald
100 Points
CP_Tester> WRITE cp2.txt
WRITE successful
CP_Tester> QUIT
UNIX> cat cp2.txt
PRIZE     cancun     10000      1 All expense-paid vacation to Cancun
PRIZE     mp3           40   5000 Free MP3 download from Bapster
ADD_USER  the-donald   100 Donald Trump
ADD_USER  tigerwoods     0 Tiger Woods
UNIX> 
```
When I called cp_tester, I gave it two command line arguments: cp1.txt and -. So, it first read commands from cp1.txt, which recreated the same state as when I created cp1.txt, and then it read from standard input. When I entered WRITE cp2.txt, it created cp2.txt, which is identical to cp1.txt, since they have the same state.
Suppose I call cp_tester with cp1.txt and cp2.txt on the command line. I should expect four error messages, since the users and prizes already exist when it tries to process cp2.txt:
```
UNIX> cp_tester cp1.txt cp2.txt
Prize cancun couldn't be added
Prize mp3 couldn't be added
ADD_USER the-donald unsuccessful
ADD_USER tigerwoods unsuccessful
UNIX> 
```
This is because cp_tester checks the return values of the New_Prize() and New_User() calls.
Let's add a few phone numbers and enter some codes. If you check the hashes using djbhash.cpp from the hashing lecture notes, you'll see that they are each divisible by 13 and not by 17, so they are each worth three points:
```
UNIX> /home/plank/cs140/Notes/Hashing/djbhash | awk '{ print $1%17, $1%13 }'
Df18ly81CO1mo4
11 0
IDWNZJ20ENkAxP
2 0
h0yuKnVD6DvRUu
11 0
UNIX> cp_tester cp1.txt -
CP_Tester> ADD_PHONE tigerwoods 865-974-4400       
ADD_PHONE successful
CP_Tester> ADD_PHONE tigerwoods 1-800-Big-Putt
ADD_PHONE successful
CP_Tester> SHOW_PHONES tigerwoods
1-800-Big-Putt
865-974-4400
CP_Tester> ENTER_CODE tigerwoods Df18ly81CO1mo4
ENTER_CODE: Added 3 points to tigerwoods.
CP_Tester> TEXT_CODE 865-974-4400 IDWNZJ20ENkAxP
TEXT_CODE: Added 3 points.
CP_Tester> TEXT_CODE 1-800-Big-Putt h0yuKnVD6DvRUu
TEXT_CODE: Added 3 points.
CP_Tester> BALANCE tigerwoods
9 Points
CP_Tester> WRITE cp3.txt
WRITE successful
CP_Tester> QUIT
UNIX> 
```

Each ENTER_CODE and TEXT_CODE call adds three points to tigerwoods' account, giving him 9 points in all. After the WRITE call, cp3.txt looks as follows:
```
PRIZE     cancun     10000      1 All expense-paid vacation to Cancun
PRIZE     mp3           40   5000 Free MP3 download from Bapster
ADD_USER  the-donald   100 Donald Trump
ADD_USER  tigerwoods     9 Tiger Woods
ADD_PHONE tigerwoods 1-800-Big-Putt
ADD_PHONE tigerwoods 865-974-4400
MARK_USED Df18ly81CO1mo4 IDWNZJ20ENkAxP h0yuKnVD6DvRUu
```

The phones have been registered to tigerwoods, his point total has been updated, and the codes have been marked as used. Although I put multiple codes on a MARK_USED line, you don't have to. Just remember the 20-word limit on a line.

And again, your output does not have to match mine -- it simply needs to create the same Code_Processor.

Let's take a look at an example where some prizes are redeemed:
```
UNIX> cp_tester cp3.txt -
CP_Tester> ADD_USER billgates 500000 Bill Gates
ADD_USER successful
CP_Tester> REDEEM tigerwoods mp3
REDEEM:       either the user doesn't exist,
              or the prize doesn't exist,
              or the user can't afford the prize.
CP_Tester> REDEEM the-donald mp3
REDEEM successful
CP_Tester> REDEEM billgates cancun
REDEEM successful
CP_Tester> WRITE cp4.txt
WRITE successful
CP_Tester> QUIT
UNIX> cat cp4.txt
PRIZE     mp3           40   4999 Free MP3 download from Bapster
ADD_USER  billgates  490000 Bill Gates
ADD_USER  the-donald    60 Donald Trump
ADD_USER  tigerwoods     9 Tiger Woods
ADD_PHONE tigerwoods 1-800-Big-Putt
ADD_PHONE tigerwoods 865-974-4400
MARK_USED Df18ly81CO1mo4 IDWNZJ20ENkAxP h0yuKnVD6DvRUu
UNIX> 
```
Since tigerwoods only has 9 points, he can't even afford an MP3 from Bapster. the-donald has no such problem, and billgates can easily afford the Cancun vacation (like he needs it). The updated points for the users and the updated quantities for the prizes have been reflected in the file. Since the quantity of cancun went to zero, it has been removed from the system.
