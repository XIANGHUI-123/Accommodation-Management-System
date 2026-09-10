#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;


// ============================================================
// CONSTANTS
// ============================================================

const int MAX_STUDENTS = 100;
const int MAX_OWNERS = 100;
const int MAX_PROPERTIES = 100;
const int MAX_SHORTLISTS = 100;
const int MAX_ENQUIRIES = 100;
const int MAX_REVIEWS = 100;
const int MAX_APPOINTMENTS = 100;


// ============================================================
// STRUCTURES
// ============================================================

struct Student
{
    int studentid;
    string name;
    string gender;
    string contact;
    string email;

    double budget;

    string propertytype;
    string roomtype;
    string roomsharing;
    string unitgender;

    double distancetocampus;

    string facilities;
    string requirements;
    string moveindate;
};


struct Owner
{
    int ownerid;
    string ownername;
    string ownertype;
    string contact;
    string address;
};


struct Property
{
    int propertyid;
    int ownerid;

    string name;
    string type;
    string roomtype;

    double rent;
    double deposit;
    double distance;

    string facilities;
    string utilitiesincluded;
    string cleaningservices;
    string availablefrom;

    bool active;
};


struct Shortlist
{
    int studentid;
    int propertyid;
};


struct Enquiry
{
    int enquiryid;
    int studentid;
    int propertyid;

    string message;
    string reply;
};


struct Review
{
    int reviewid;
    int studentid;
    int propertyid;

    int rating;
    string comment;
};


struct Appointment
{
    int appointmentid;
    int studentid;
    int propertyid;

    string date;
    string timeslot;
    string status;
};


// ============================================================
// ARRAYS OF STRUCTURES
// ============================================================

Student studentlist[MAX_STUDENTS];
Owner ownerlist[MAX_OWNERS];
Property propertieslist[MAX_PROPERTIES];

Shortlist shortlistlist[MAX_SHORTLISTS];
Enquiry enquirylist[MAX_ENQUIRIES];
Review reviewslist[MAX_REVIEWS];
Appointment appointmentslist[MAX_APPOINTMENTS];


// ============================================================
// COUNTERS
// ============================================================

int currentuserid = 0;

int studentcount = 0;
int ownercount = 0;
int propertycount = 0;

int shortlistcount = 0;
int enquirycount = 0;
int reviewcount = 0;
int appointmentcount = 0;


// ============================================================
// FUNCTION PROTOTYPES
// ============================================================

// Student
void loadStudents();
bool saveStudents();

bool addStudent(Student newstudent);
bool updateStudent(Student newstudent);

int findStudent(int studentid);


// Owner
void loadOwners();
void saveOwners();

bool addOwner(Owner newowner);
bool updateOwner(Owner newowner);

int findOwner(int ownerid);


// Property
void loadProperties();
void saveProperties();

bool addProperty(Property newproperty);
bool updateProperty(Property newproperty);
bool deleteProperty(int propertyid);

int findProperty(int propertyid);


// Shortlist
void loadShortlists();
void saveShortlists();

bool addShortlist(Shortlist newshortlist);
bool deleteShortlist(int studentid, int propertyid);
bool shortlistExists(int studentid, int propertyid);


// Enquiry
void loadEnquiries();
void saveEnquiries();

bool addEnquiry(Enquiry newenquiry);
bool updateEnquiryReply(int enquiryid, string reply);

int findEnquiry(int enquiryid);


// Review
void loadReviews();
void saveReviews();

bool addReview(Review newreview);

int findReview(int reviewid);


// Appointment
void loadAppointments();
void saveAppointments();

bool addAppointment(Appointment newappointment);
bool updateAppointmentStatus(int appointmentid, string status);

int findAppointment(int appointmentid);


// Menus
void mainmenu();
void userlogin();
void userregister();

void studentmenu();
void studentsearchproperty();
void studentenquiry();
void studentreview();
void studentviewreview();
void studentscheduleappt();
void studentviewappt();
void studentshowanalytics();

void ownermenu();
void ownermanagelistings();
void ownerreplyenquiries();
void ownermanageappts();
void ownershowanalytics();


// Validation
bool validDate(string date);
bool validTime(string time);

int stringToInt(string text);
double stringToDouble(string text);


// Property generation
void generatePropertiesFromOwners();
bool propertyFileNeedsRegeneration();
string normalizePropertyType(string propertytype);
string normalizeRoomType(string roomtype);
bool studentHasMatchingPreference(string propertytype, string roomtype);


// ============================================================
// MAIN
// ============================================================

int main()
{
    loadStudents();
    loadOwners();

    /*
        properties.txt is generated from owners.txt.

        This means the property listing follows the actual
        owner information provided in owners.txt.
    */
    generatePropertiesFromOwners();
    loadProperties();

    loadShortlists();
    loadEnquiries();
    loadReviews();
    loadAppointments();

    mainmenu();

    return 0;
}


// ============================================================
// MAIN MENU
// ============================================================

void mainmenu()
{
    int choice;

    do
    {
        cout << "\n==================================================\n";
        cout << "UTAR SUNGAI LONG ACCOMMODATION MANAGEMENT SYSTEM\n";
        cout << "==================================================\n";

        cout << "\nWelcome! What would you like to do?\n";

        cout << "1. Login\n";
        cout << "2. Register Student or Owner\n";
        cout << "3. Exit\n";

        cout << "Please enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            userlogin();
        }
        else if (choice == 2)
        {
            userregister();
        }
        else if (choice == 3)
        {
            cout << "\nThank you for using the system.\n";
        }
        else
        {
            cout << "\nInvalid selection. Please choose 1, 2, or 3.\n";
        }

    } while (choice != 3);
}


// ============================================================
// STUDENT / OWNER LOGIN
// ============================================================

void userlogin()
{
    int role;
    int id;
    string contact;

    cout << "\n===== LOGIN PORTAL =====\n";
    cout << "1. Student\n";
    cout << "2. Owner/Agent\n";
    cout << "Choose your role: ";
    cin >> role;

    cout << "Enter your ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter your contact number: ";
    getline(cin, contact);

    if (role == 1)
    {
        int position = findStudent(id);

        if (position != -1 && studentlist[position].contact == contact)
        {
            currentuserid = studentlist[position].studentid;
            cout << "\nLogin successful!\n";
            studentmenu();
            return;
        }
    }
    else if (role == 2)
    {
        int position = findOwner(id);

        if (position != -1 && ownerlist[position].contact == contact)
        {
            currentuserid = ownerlist[position].ownerid;
            cout << "\nLogin successful!\n";
            ownermenu();
            return;
        }
    }

    cout << "\nInvalid role, ID, or contact number.\n";
}


// ============================================================
// STUDENT / OWNER REGISTRATION
// ============================================================

void userregister()
{
    cout << "\n===== REGISTRATION PORTAL =====\n";

    int role;

    do
    {
        cout << "\nChoose your role\n";
        cout << "1. Student\n";
        cout << "2. Owner/Agent\n";

        cout << "Enter your choice: ";
        cin >> role;

        if (role != 1 && role != 2)
        {
            cout << "Invalid choice. Please select 1 or 2.\n";
        }

    } while (role != 1 && role != 2);


    cin.ignore();

    if (role == 1)
    {
        Student newstudent;

        cout << "Student ID: ";
        cin >> newstudent.studentid;
        cin.ignore();

        cout << "Full name: ";
        getline(cin, newstudent.name);

        cout << "Gender: ";
        getline(cin, newstudent.gender);

        cout << "Contact number: ";
        getline(cin, newstudent.contact);

        cout << "Email: ";
        getline(cin, newstudent.email);

        cout << "Budget (RM): ";
        cin >> newstudent.budget;
        cin.ignore();

        cout << "Property type (Condo/Landed): ";
        getline(cin, newstudent.propertytype);

        cout << "Room type (Single Room/Medium Room/Master Room): ";
        getline(cin, newstudent.roomtype);

        cout << "Room sharing (Yes/No): ";
        getline(cin, newstudent.roomsharing);

        cout << "Unit gender: ";
        getline(cin, newstudent.unitgender);

        cout << "Distance to campus (m): ";
        cin >> newstudent.distancetocampus;
        cin.ignore();

        cout << "Facilities: ";
        getline(cin, newstudent.facilities);

        cout << "Requirements: ";
        getline(cin, newstudent.requirements);

        cout << "Move-in date: ";
        getline(cin, newstudent.moveindate);

        if (addStudent(newstudent))
        {
            cout << "\nStudent registration successful!\n";
            cout << "Your Student ID is: "
                 << newstudent.studentid << endl;
        }
        else
        {
            cout << "\nStudent registration failed.\n";
        }
    }
    else
    {
        Owner newowner;

        newowner.ownerid = ownercount + 1;

        cout << "Owner/Agent name: ";
        getline(cin, newowner.ownername);

        cout << "Owner type (Owner/Agent): ";
        getline(cin, newowner.ownertype);

        cout << "Contact number: ";
        getline(cin, newowner.contact);

        cout << "Address: ";
        getline(cin, newowner.address);

        if (addOwner(newowner))
        {
            cout << "\nOwner registration successful!\n";
            cout << "Your Owner ID is: "
                 << newowner.ownerid << endl;
        }
        else
        {
            cout << "\nOwner registration failed.\n";
        }
    }
}


// ============================================================
// STUDENT MENU
// ============================================================

void studentmenu()
{
    int choice;

    do
    {
        cout << "\n===== STUDENT DASHBOARD =====\n";

        cout << "Welcome! What would you like to do?\n";

        cout << "1. Search & Shortlist Properties\n";
        cout << "2. Send Enquiry to Owner\n";
        cout << "3. Leave A Property Review\n";
        cout << "4. View Property Reviews\n";
        cout << "5. Schedule Viewing Appointments\n";
        cout << "6. View Scheduled Appointments\n";
        cout << "7. View Market Analytics\n";
        cout << "8. Logout\n";

        cout << "Please enter your choice: ";
        cin >> choice;


        switch (choice)
        {
            case 1:
                studentsearchproperty();
                break;

            case 2:
                studentenquiry();
                break;

            case 3:
                studentreview();
                break;

            case 4:
                studentviewreview();
                break;

            case 5:
                studentscheduleappt();
                break;

            case 6:
                studentviewappt();
                break;

            case 7:
                studentshowanalytics();
                break;

            case 8:
                cout << "\nLogging out...\n";
                break;

            default:
                cout << "\nInvalid choice. Please select 1-8.\n";
        }

    } while (choice != 8);
}


// ============================================================
// STUDENT SEARCH PROPERTY
// ============================================================

void studentsearchproperty()
{
    int choice;

    string propertytype = "0";
    string roomtype = "0";

    double maxrent = 0;
    double maxdistance = 0;


    cout << "\n===== SEARCH PROPERTIES =====\n";

    cout << "1. Search by property type\n";
    cout << "2. Search by room type\n";
    cout << "3. Search by maximum rent\n";
    cout << "4. Search by maximum distance\n";
    cout << "5. Search using all criteria\n";

    cout << "Enter your choice: ";
    cin >> choice;


    // --------------------------------------------------------
    // PROPERTY TYPE
    // --------------------------------------------------------

    if (choice == 1 || choice == 5)
    {
        int typechoice;

        cout << "\nProperty Type\n";
        cout << "1. Condo\n";
        cout << "2. Landed\n";

        if (choice == 5)
        {
            cout << "3. Any\n";
        }

        cout << "Enter your choice: ";
        cin >> typechoice;


        if (typechoice == 1)
        {
            propertytype = "Condo";
        }
        else if (typechoice == 2)
        {
            propertytype = "Landed";
        }
        else if (typechoice == 3 && choice == 5)
        {
            propertytype = "0";
        }
        else
        {
            cout << "Invalid choice.\n";
            return;
        }
    }


    // --------------------------------------------------------
    // ROOM TYPE
    // --------------------------------------------------------

    if (choice == 2 || choice == 5)
    {
        int roomchoice;

        cout << "\nRoom Type\n";
        cout << "1. Single Room\n";
        cout << "2. Medium Room\n";
        cout << "3. Master Room\n";

        if (choice == 5)
        {
            cout << "4. Any\n";
        }

        cout << "Enter your choice: ";
        cin >> roomchoice;


        if (roomchoice == 1)
        {
            roomtype = "Single Room";
        }
        else if (roomchoice == 2)
        {
            roomtype = "Medium Room";
        }
        else if (roomchoice == 3)
        {
            roomtype = "Master Room";
        }
        else if (roomchoice == 4 && choice == 5)
        {
            roomtype = "0";
        }
        else
        {
            cout << "Invalid choice.\n";
            return;
        }
    }


    // --------------------------------------------------------
    // MAXIMUM RENT
    // --------------------------------------------------------

    if (choice == 3 || choice == 5)
    {
        cout << "\nEnter maximum monthly rent (RM)";

        if (choice == 5)
        {
            cout << " (0 for Any)";
        }

        cout << ": ";

        cin >> maxrent;


        if (maxrent < 0)
        {
            cout << "Invalid rent.\n";
            return;
        }
    }


    // --------------------------------------------------------
    // MAXIMUM DISTANCE
    // --------------------------------------------------------

    if (choice == 4 || choice == 5)
    {
        cout << "Enter maximum distance to UTAR (km)";

        if (choice == 5)
        {
            cout << " (0 for Any)";
        }

        cout << ": ";

        cin >> maxdistance;


        if (maxdistance < 0)
        {
            cout << "Invalid distance.\n";
            return;
        }
    }


    if (choice < 1 || choice > 5)
    {
        cout << "Invalid choice.\n";
        return;
    }


    // --------------------------------------------------------
    // DISPLAY RESULTS
    // --------------------------------------------------------

    bool found = false;

    cout << "\n===== SEARCH RESULTS =====\n";


    for (int i = 0; i < propertycount; i++)
    {
        if (propertieslist[i].active == false)
        {
            continue;
        }


        bool match = true;


        if (propertytype != "0" &&
            propertieslist[i].type != propertytype)
        {
            match = false;
        }


        if (roomtype != "0" &&
            propertieslist[i].roomtype != roomtype)
        {
            match = false;
        }


        if (maxrent > 0 &&
            propertieslist[i].rent > maxrent)
        {
            match = false;
        }


        if (maxdistance > 0 &&
            propertieslist[i].distance > maxdistance)
        {
            match = false;
        }


        if (match == true)
        {
            cout << "\n----------------------------------------\n";

            cout << "Property ID: "
                 << propertieslist[i].propertyid << endl;

            cout << "Property Name: "
                 << propertieslist[i].name << endl;

            cout << "Property Type: "
                 << propertieslist[i].type << endl;

            cout << "Room Type: "
                 << propertieslist[i].roomtype << endl;

            cout << "Monthly Rent: RM "
                 << propertieslist[i].rent << endl;

            cout << "Deposit: RM "
                 << propertieslist[i].deposit << endl;

            cout << "Distance to UTAR: "
                 << propertieslist[i].distance
                 << " m" << endl;

            cout << "Facilities: "
                 << propertieslist[i].facilities << endl;

            cout << "Utilities Included: "
                 << propertieslist[i].utilitiesincluded
                 << endl;

            cout << "Cleaning Services: "
                 << propertieslist[i].cleaningservices
                 << endl;

            cout << "Available From: "
                 << propertieslist[i].availablefrom
                 << endl;

            cout << "----------------------------------------\n";

            found = true;
        }
    }


    if (found == false)
    {
        cout << "\nNo properties match your search criteria.\n";
        return;
    }


    // --------------------------------------------------------
    // SHORTLIST
    // --------------------------------------------------------

    char action;

    cout << "\nWould you like to shortlist a property?";
    cout << " (Y - Yes, N - No): ";

    cin >> action;


    if (action == 'Y' || action == 'y')
    {
        int propertyid;

        cout << "Enter property ID: ";
        cin >> propertyid;


        int position = findProperty(propertyid);


        if (position != -1 &&
            propertieslist[position].active == true)
        {
            Shortlist s;

            s.studentid = currentuserid;
            s.propertyid = propertyid;


            if (addShortlist(s))
            {
                cout << "Property added to shortlist!\n";
            }
            else
            {
                cout << "Property is already in your shortlist.\n";
            }
        }
        else
        {
            cout << "Invalid property ID.\n";
        }
    }
    else if (action == 'N' || action == 'n')
    {
        cout << "No property was shortlisted.\n";
    }
    else
    {
        cout << "Invalid choice.\n";
    }
}


// ============================================================
// STUDENT ENQUIRY
// ============================================================

void studentenquiry()
{
    Enquiry e;

    cout << "\n===== SEND ENQUIRY =====\n";


    cout << "Enter the property ID you would like to enquire about: ";
    cin >> e.propertyid;


    int position = findProperty(e.propertyid);


    if (position == -1 ||
        propertieslist[position].active == false)
    {
        cout << "Property ID not found.\n";
        return;
    }


    cin.ignore();


    cout << "Enter your enquiry: ";
    getline(cin, e.message);


    e.studentid = currentuserid;
    e.enquiryid = enquirycount + 1;
    e.reply = "Pending";


    if (addEnquiry(e))
    {
        cout << "Enquiry sent to property owner!\n";
    }
    else
    {
        cout << "Failed to send enquiry.\n";
    }
}


// ============================================================
// STUDENT REVIEW
// ============================================================

void studentreview()
{
    Review r;

    cout << "\n===== ADD REVIEW =====\n";


    cout << "Enter the property ID you would like to review: ";
    cin >> r.propertyid;


    int position = findProperty(r.propertyid);


    if (position == -1 ||
        propertieslist[position].active == false)
    {
        cout << "Property ID not found.\n";
        return;
    }


    cout << "Enter a rating (1-5): ";
    cin >> r.rating;


    if (r.rating < 1 || r.rating > 5)
    {
        cout << "Invalid rating. Please enter 1-5.\n";
        return;
    }


    cin.ignore();


    cout << "Enter a comment: ";
    getline(cin, r.comment);


    r.reviewid = reviewcount + 1;
    r.studentid = currentuserid;


    if (addReview(r))
    {
        cout << "Review submitted successfully!\n";
    }
    else
    {
        cout << "Failed to submit review.\n";
    }
}


// ============================================================
// VIEW REVIEWS
// ============================================================

void studentviewreview()
{
    int propertyid;

    bool found = false;


    cout << "\n===== VIEW PROPERTY REVIEWS =====\n";

    cout << "Enter the property ID: ";
    cin >> propertyid;


    if (findProperty(propertyid) == -1)
    {
        cout << "Property ID not found.\n";
        return;
    }


    for (int i = 0; i < reviewcount; i++)
    {
        if (reviewslist[i].propertyid == propertyid)
        {
            cout << "\n-----------------------------------\n";

            cout << "Review ID: "
                 << reviewslist[i].reviewid << endl;

            cout << "Student ID: "
                 << reviewslist[i].studentid << endl;

            cout << "Rating: "
                 << reviewslist[i].rating
                 << "/5" << endl;

            cout << "Comment: "
                 << reviewslist[i].comment << endl;

            found = true;
        }
    }


    if (found == false)
    {
        cout << "No reviews found for this property.\n";
    }
}


// ============================================================
// VALIDATE DATE
// ============================================================

bool validDate(string date)
{
    /*
        Required format:

        YYYY-MM-DD

        Example:

        2026-10-09
    */

    if (date.length() != 10)
    {
        return false;
    }


    if (date[4] != '-' ||
        date[7] != '-')
    {
        return false;
    }


    for (int i = 0; i < 10; i++)
    {
        if (i != 4 && i != 7)
        {
            if (date[i] < '0' ||
                date[i] > '9')
            {
                return false;
            }
        }
    }


    return true;
}


// ============================================================
// VALIDATE TIME
// ============================================================

bool validTime(string time)
{
    /*
        Required format:

        HH:MM

        Example:

        10:00
        14:30
        18:45
    */

    if (time.length() != 5)
    {
        return false;
    }


    if (time[2] != ':')
    {
        return false;
    }


    if (time[0] < '0' ||
        time[0] > '2')
    {
        return false;
    }


    if (time[1] < '0' ||
        time[1] > '9')
    {
        return false;
    }


    if (time[3] < '0' ||
        time[3] > '5')
    {
        return false;
    }


    if (time[4] < '0' ||
        time[4] > '9')
    {
        return false;
    }


    return true;
}


// ============================================================
// SCHEDULE APPOINTMENT
// ============================================================

void studentscheduleappt()
{
    Appointment a;


    cout << "\n===== SCHEDULE APPOINTMENT =====\n";


    cout << "Enter the property ID you would like to schedule a viewing for: ";
    cin >> a.propertyid;


    int position = findProperty(a.propertyid);


    if (position == -1 ||
        propertieslist[position].active == false)
    {
        cout << "Property ID not found.\n";
        return;
    }


    // --------------------------------------------------------
    // DATE
    // --------------------------------------------------------

    do
    {
        cout << "Enter your preferred date (YYYY-MM-DD): ";
        cin >> a.date;


        if (!validDate(a.date))
        {
            cout << "Invalid date. Please use YYYY-MM-DD format.\n";
        }

    } while (!validDate(a.date));


    // --------------------------------------------------------
    // TIME
    // --------------------------------------------------------

    do
    {
        cout << "Enter your preferred time slot (HH:MM, 24-hour): ";
        cin >> a.timeslot;


        if (!validTime(a.timeslot))
        {
            cout << "Invalid time. Please use HH:MM format.\n";
        }

    } while (!validTime(a.timeslot));


    a.studentid = currentuserid;

    a.appointmentid = appointmentcount + 1;

    a.status = "Pending";


    if (addAppointment(a))
    {
        cout << "\nAppointment request submitted!\n";
    }
    else
    {
        cout << "\nFailed to submit appointment.\n";
    }
}


// ============================================================
// VIEW STUDENT APPOINTMENTS
// ============================================================

void studentviewappt()
{
    bool found = false;


    cout << "\n===== MY APPOINTMENTS =====\n";


    for (int i = 0; i < appointmentcount; i++)
    {
        if (appointmentslist[i].studentid == currentuserid)
        {
            cout << "\n-----------------------------------\n";

            cout << "Appointment ID: "
                 << appointmentslist[i].appointmentid
                 << endl;

            cout << "Property ID: "
                 << appointmentslist[i].propertyid
                 << endl;

            cout << "Date: "
                 << appointmentslist[i].date
                 << endl;

            cout << "Time: "
                 << appointmentslist[i].timeslot
                 << endl;

            cout << "Status: "
                 << appointmentslist[i].status
                 << endl;

            found = true;
        }
    }


    if (found == false)
    {
        cout << "No scheduled appointments.\n";
    }
}


// ============================================================
// STUDENT ANALYTICS
// ============================================================

void studentshowanalytics()
{
    int totalproperties = 0;

    int condoCount = 0;
    int landedCount = 0;

    double totalRent = 0;


    cout << "\n===== UTAR SUNGAI LONG MARKET ANALYTICS =====\n";


    for (int i = 0; i < propertycount; i++)
    {
        if (propertieslist[i].active == true)
        {
            totalproperties++;

            totalRent =
                totalRent + propertieslist[i].rent;


            if (propertieslist[i].type == "Condo")
            {
                condoCount++;
            }
            else if (propertieslist[i].type == "Landed")
            {
                landedCount++;
            }
        }
    }


    cout << "\nTotal active listings: "
         << totalproperties << endl;


    cout << "Condo listings: "
         << condoCount << endl;


    cout << "Landed listings: "
         << landedCount << endl;


    if (totalproperties > 0)
    {
        cout << fixed << setprecision(2);

        cout << "Average monthly rent: RM "
             << totalRent / totalproperties
             << endl;
    }
    else
    {
        cout << "No active properties available.\n";
    }


    // --------------------------------------------------------
    // DISTANCE ANALYSIS
    // --------------------------------------------------------

    int within500 = 0;
    int between500and1000 = 0;
    int above1000 = 0;


    for (int i = 0; i < propertycount; i++)
    {
        if (propertieslist[i].active == true)
        {
            if (propertieslist[i].distance <= 500)
            {
                within500++;
            }
            else if (propertieslist[i].distance <= 1000)
            {
                between500and1000++;
            }
            else
            {
                above1000++;
            }
        }
    }


    cout << "\nDistance from UTAR:\n";

    cout << "Within 500m: "
         << within500 << " listings\n";

    cout << "500m - 1km: "
         << between500and1000 << " listings\n";

    cout << "Above 1km: "
         << above1000 << " listings\n";
}


// ============================================================
// OWNER MENU
// ============================================================

void ownermenu()
{
    int choice;


    do
    {
        cout << "\n===== OWNER / AGENT DASHBOARD =====\n";

        cout << "1. Manage Property Listings\n";
        cout << "2. Reply to Student Enquiries\n";
        cout << "3. Manage Viewing Appointments\n";
        cout << "4. View Performance Analytics\n";
        cout << "5. Logout\n";


        cout << "Enter your choice: ";
        cin >> choice;


        switch (choice)
        {
            case 1:
                ownermanagelistings();
                break;

            case 2:
                ownerreplyenquiries();
                break;

            case 3:
                ownermanageappts();
                break;

            case 4:
                ownershowanalytics();
                break;

            case 5:
                cout << "\nLogging out...\n";
                break;

            default:
                cout << "\nInvalid choice. Please select 1-5.\n";
        }

    } while (choice != 5);
}


// ============================================================
// OWNER MANAGE LISTINGS
// ============================================================

void ownermanagelistings()
{
    int choice;


    do
    {
        cout << "\n===== PROPERTY LISTING MANAGEMENT =====\n";

        cout << "1. Add New Property Listing\n";
        cout << "2. View Active Listings\n";
        cout << "3. Update Property\n";
        cout << "4. Delete Property\n";
        cout << "5. Back\n";


        cout << "Enter your choice: ";
        cin >> choice;


        // ====================================================
        // ADD PROPERTY
        // ====================================================

        if (choice == 1)
        {
            Property p;


            p.propertyid = propertycount + 1;
            p.ownerid = currentuserid;


            cin.ignore();


            cout << "\nEnter property name: ";
            getline(cin, p.name);


            // ------------------------------------------------
            // PROPERTY TYPE
            // ------------------------------------------------

            int typechoice;


            do
            {
                cout << "\nProperty Type\n";
                cout << "1. Condo\n";
                cout << "2. Landed\n";

                cout << "Enter your choice: ";
                cin >> typechoice;


                if (typechoice == 1)
                {
                    p.type = "Condo";
                }
                else if (typechoice == 2)
                {
                    p.type = "Landed";
                }
                else
                {
                    cout << "Invalid choice. Please select 1 or 2.\n";
                }

            } while (typechoice != 1 &&
                     typechoice != 2);


            // ------------------------------------------------
            // ROOM TYPE
            // ------------------------------------------------

            int roomchoice;


            do
            {
                cout << "\nRoom Type\n";

                cout << "1. Single Room\n";
                cout << "2. Medium Room\n";
                cout << "3. Master Room\n";

                cout << "Enter your choice: ";
                cin >> roomchoice;


                if (roomchoice == 1)
                {
                    p.roomtype = "Single Room";
                }
                else if (roomchoice == 2)
                {
                    p.roomtype = "Medium Room";
                }
                else if (roomchoice == 3)
                {
                    p.roomtype = "Master Room";
                }
                else
                {
                    cout << "Invalid choice. Please select 1-3.\n";
                }

            } while (roomchoice < 1 ||
                     roomchoice > 3);


            // ------------------------------------------------
            // PROPERTY DETAILS
            // ------------------------------------------------

            cout << "\nMonthly Rent (RM): ";
            cin >> p.rent;


            cout << "Deposit (RM): ";
            cin >> p.deposit;
            cout << "Distance to UTAR (m): ";
            cin >> p.distance;


            cin.ignore();


            cout << "Facilities: ";
            getline(cin, p.facilities);


            cout << "Utilities Included: ";
            getline(cin, p.utilitiesincluded);


            cout << "Cleaning Services: ";
            getline(cin, p.cleaningservices);


            // ------------------------------------------------
            // AVAILABLE DATE
            // ------------------------------------------------

            do
            {
                cout << "Available From (YYYY-MM-DD): ";
                getline(cin, p.availablefrom);


                if (!validDate(p.availablefrom))
                {
                    cout << "Invalid date. Please use YYYY-MM-DD format.\n";
                }

            } while (!validDate(p.availablefrom));


            p.active = true;


            if (addProperty(p))
            {
                cout << "\nProperty added successfully!\n";
            }
            else
            {
                cout << "\nUnable to add property.\n";
            }
        }


        // ====================================================
        // VIEW LISTINGS
        // ====================================================

        else if (choice == 2)
        {
            bool found = false;


            cout << "\n===== ACTIVE LISTINGS =====\n";


            for (int i = 0; i < propertycount; i++)
            {
                if (propertieslist[i].active == true &&
                    propertieslist[i].ownerid == currentuserid)
                {
                    cout << "\n-----------------------------------\n";

                    cout << "Property ID: "
                         << propertieslist[i].propertyid
                         << endl;

                    cout << "Property Name: "
                         << propertieslist[i].name
                         << endl;

                    cout << "Property Type: "
                         << propertieslist[i].type
                         << endl;

                    cout << "Room Type: "
                         << propertieslist[i].roomtype
                         << endl;

                    cout << "Monthly Rent: RM "
                         << propertieslist[i].rent
                         << endl;

                    cout << "Deposit: RM "
                         << propertieslist[i].deposit
                         << endl;

                    cout << "Distance: "
                         << propertieslist[i].distance
                         << " m"
                         << endl;

                    cout << "Facilities: "
                         << propertieslist[i].facilities
                         << endl;

                    cout << "Utilities Included: "
                         << propertieslist[i].utilitiesincluded
                         << endl;

                    cout << "Cleaning Services: "
                         << propertieslist[i].cleaningservices
                         << endl;

                    cout << "Available From: "
                         << propertieslist[i].availablefrom
                         << endl;

                    found = true;
                }
            }


            if (found == false)
            {
                cout << "No active listings found.\n";
            }
        }


        // ====================================================
        // UPDATE PROPERTY
        // ====================================================

        else if (choice == 3)
        {
            int propertyid;


            cout << "\nEnter property ID to update: ";
            cin >> propertyid;


            int position = findProperty(propertyid);


            if (position == -1 ||
                propertieslist[position].ownerid != currentuserid ||
                propertieslist[position].active == false)
            {
                cout << "Property not found.\n";
            }
            else
            {
                cin.ignore();


                cout << "Enter new property name: ";
                getline(cin, propertieslist[position].name);


                cout << "Enter new monthly rent: ";
                cin >> propertieslist[position].rent;


                cout << "Enter new deposit: ";
                cin >> propertieslist[position].deposit;



                cout << "Enter new distance to UTAR (m): ";
                cin >> propertieslist[position].distance;


                cin.ignore();


                cout << "Enter new facilities: ";
                getline(cin, propertieslist[position].facilities);


                cout << "Enter new utilities included: ";
                getline(cin, propertieslist[position].utilitiesincluded);


                cout << "Enter new cleaning services: ";
                getline(cin, propertieslist[position].cleaningservices);


                do
                {
                    cout << "Enter new available date (YYYY-MM-DD): ";

                    getline(cin,
                            propertieslist[position].availablefrom);


                    if (!validDate(
                        propertieslist[position].availablefrom))
                    {
                        cout << "Invalid date. Please use YYYY-MM-DD format.\n";
                    }

                } while (!validDate(
                    propertieslist[position].availablefrom));


                saveProperties();


                cout << "\nProperty updated successfully!\n";
            }
        }


        // ====================================================
        // DELETE PROPERTY
        // ====================================================

        else if (choice == 4)
        {
            int propertyid;


            cout << "\nEnter property ID to delete: ";
            cin >> propertyid;


            int position = findProperty(propertyid);


            if (position != -1 &&
                propertieslist[position].ownerid == currentuserid &&
                propertieslist[position].active == true)
            {
                deleteProperty(propertyid);

                cout << "Property deleted successfully!\n";
            }
            else
            {
                cout << "Property not found.\n";
            }
        }


        else if (choice != 5)
        {
            cout << "Invalid choice. Please select 1-5.\n";
        }


    } while (choice != 5);
}


// ============================================================
// OWNER REPLY ENQUIRIES
// ============================================================

void ownerreplyenquiries()
{
    bool found = false;


    cout << "\n===== STUDENT ENQUIRIES =====\n";


    for (int i = 0; i < enquirycount; i++)
    {
        int position =
            findProperty(enquirylist[i].propertyid);


        if (position != -1 &&
            propertieslist[position].ownerid == currentuserid)
        {
            cout << "\n-----------------------------------\n";

            cout << "Enquiry ID: "
                 << enquirylist[i].enquiryid
                 << endl;

            cout << "Student ID: "
                 << enquirylist[i].studentid
                 << endl;

            cout << "Property ID: "
                 << enquirylist[i].propertyid
                 << endl;

            cout << "Message: "
                 << enquirylist[i].message
                 << endl;

            cout << "Reply: "
                 << enquirylist[i].reply
                 << endl;

            found = true;
        }
    }


    if (found == false)
    {
        cout << "No enquiries found.\n";
        return;
    }


    int enquiryid;
    string reply;


    cout << "\nEnter enquiry ID to reply: ";
    cin >> enquiryid;


    int position = findEnquiry(enquiryid);


    if (position == -1)
    {
        cout << "Enquiry not found.\n";
        return;
    }


    int propertyposition =
        findProperty(enquirylist[position].propertyid);


    if (propertyposition == -1 ||
        propertieslist[propertyposition].ownerid != currentuserid)
    {
        cout << "You cannot reply to this enquiry.\n";
        return;
    }


    cin.ignore();


    cout << "Enter your reply: ";
    getline(cin, reply);


    if (updateEnquiryReply(enquiryid, reply))
    {
        cout << "Reply sent successfully!\n";
    }
    else
    {
        cout << "Unable to send reply.\n";
    }
}


// ============================================================
// OWNER MANAGE APPOINTMENTS
// ============================================================

void ownermanageappts()
{
    int choice;


    do
    {
        cout << "\n===== MANAGE APPOINTMENTS =====\n";

        cout << "1. View Appointments\n";
        cout << "2. Approve Appointment\n";
        cout << "3. Reject Appointment\n";
        cout << "4. Back\n";


        cout << "Enter your choice: ";
        cin >> choice;


        // ----------------------------------------------------
        // VIEW
        // ----------------------------------------------------

        if (choice == 1)
        {
            bool found = false;


            for (int i = 0; i < appointmentcount; i++)
            {
                int position =
                    findProperty(
                        appointmentslist[i].propertyid);


                if (position != -1 &&
                    propertieslist[position].ownerid == currentuserid)
                {
                    cout << "\n-----------------------------------\n";

                    cout << "Appointment ID: "
                         << appointmentslist[i].appointmentid
                         << endl;

                    cout << "Student ID: "
                         << appointmentslist[i].studentid
                         << endl;

                    cout << "Property ID: "
                         << appointmentslist[i].propertyid
                         << endl;

                    cout << "Date: "
                         << appointmentslist[i].date
                         << endl;

                    cout << "Time: "
                         << appointmentslist[i].timeslot
                         << endl;

                    cout << "Status: "
                         << appointmentslist[i].status
                         << endl;

                    found = true;
                }
            }


            if (found == false)
            {
                cout << "No appointments found.\n";
            }
        }


        // ----------------------------------------------------
        // APPROVE / REJECT
        // ----------------------------------------------------

        else if (choice == 2 || choice == 3)
        {
            int appointmentid;


            cout << "Enter appointment ID: ";
            cin >> appointmentid;


            int position =
                findAppointment(appointmentid);


            if (position == -1)
            {
                cout << "Appointment not found.\n";
                continue;
            }


            int propertyposition =
                findProperty(
                    appointmentslist[position].propertyid);


            if (propertyposition == -1 ||
                propertieslist[propertyposition].ownerid != currentuserid)
            {
                cout << "You cannot manage this appointment.\n";
                continue;
            }


            if (choice == 2)
            {
                updateAppointmentStatus(
                    appointmentid,
                    "Confirmed");

                cout << "Appointment confirmed!\n";
            }
            else
            {
                updateAppointmentStatus(
                    appointmentid,
                    "Cancelled");

                cout << "Appointment cancelled.\n";
            }
        }


        else if (choice != 4)
        {
            cout << "Invalid choice. Please select 1-4.\n";
        }


    } while (choice != 4);
}


// ============================================================
// OWNER ANALYTICS
// ============================================================

void ownershowanalytics()
{
    int totalproperties = 0;
    int totalshortlists = 0;


    double totalrent = 0;


    cout << "\n===== OWNER PERFORMANCE ANALYTICS =====\n";


    for (int i = 0; i < propertycount; i++)
    {
        if (propertieslist[i].ownerid == currentuserid &&
            propertieslist[i].active == true)
        {
            totalproperties++;


            totalrent =
                totalrent +
                propertieslist[i].rent;


            for (int j = 0; j < shortlistcount; j++)
            {
                if (shortlistlist[j].propertyid ==
                    propertieslist[i].propertyid)
                {
                    totalshortlists++;
                }
            }
        }
    }


    cout << "Active listings: "
         << totalproperties
         << endl;


    cout << "Total student shortlists: "
         << totalshortlists
         << endl;


    if (totalproperties > 0)
    {
        cout << fixed << setprecision(2);

        cout << "Average rental price of your listings: RM "
             << totalrent / totalproperties
             << endl;
    }
}


// ============================================================
// GET FIELD FROM SEMICOLON FILE
// ============================================================

int stringToInt(string text)
{
    int number = 0;
    int sign = 1;
    int i = 0;

    if (text.length() > 0 && text[0] == '-')
    {
        sign = -1;
        i = 1;
    }

    while (i < (int)text.length())
    {
        if (text[i] >= '0' && text[i] <= '9')
        {
            number = number * 10 + (text[i] - '0');
        }
        i++;
    }

    return number * sign;
}


double stringToDouble(string text)
{
    double number = 0;
    double decimal = 0.1;
    bool afterDecimal = false;
    int sign = 1;
    int i = 0;

    if (text.length() > 0 && text[0] == '-')
    {
        sign = -1;
        i = 1;
    }

    while (i < (int)text.length())
    {
        if (text[i] >= '0' && text[i] <= '9')
        {
            if (afterDecimal == false)
            {
                number = number * 10 + (text[i] - '0');
            }
            else
            {
                number = number + (text[i] - '0') * decimal;
                decimal = decimal / 10;
            }
        }
        else if (text[i] == '.')
        {
            afterDecimal = true;
        }

        i++;
    }

    return number * sign;
}


string getField(string line, int field)
{
    int start = 0;
    int end = -1;
    char delimiter = ';';

    if (line.find('|') != string::npos)
    {
        delimiter = '|';
    }


    for (int i = 0; i <= field; i++)
    {
        start = end + 1;

        end = line.find(delimiter, start);


        if (i == field)
        {
            if (end == -1)
            {
                return line.substr(start);
            }

            return line.substr(start, end - start);
        }
    }


    return "";
}


// ============================================================
// LOAD STUDENTS
// ============================================================

void loadStudents()
{
    ifstream in_file("students.txt");


    if (!in_file)
    {
        studentcount = 0;
        return;
    }


    studentcount = 0;


    string line;


    /*
        students.txt uses:

        studentID;
        studentName;
        gender;
        contact;
        email;
        budget;
        propertyType;
        roomType;
        roomSharing;
        unitGender;
        distance;
        facilities;
        requirements;
        moveInDate
    */


    while (studentcount < MAX_STUDENTS &&
           getline(in_file, line))
    {
        if (line == "" || getField(line, 0) == "Student ID")
        {
            continue;
        }


        studentlist[studentcount].studentid =
            stringToInt(getField(line, 0));


        studentlist[studentcount].name =
            getField(line, 1);


        studentlist[studentcount].gender =
            getField(line, 2);


        studentlist[studentcount].contact =
            getField(line, 3);


        studentlist[studentcount].email =
            getField(line, 4);


        studentlist[studentcount].budget =
            stringToDouble(getField(line, 5));


        studentlist[studentcount].propertytype =
            getField(line, 6);


        studentlist[studentcount].roomtype =
            getField(line, 7);


        studentlist[studentcount].roomsharing =
            getField(line, 8);


        studentlist[studentcount].unitgender =
            getField(line, 9);


        studentlist[studentcount].distancetocampus =
            stringToDouble(getField(line, 10));


        studentlist[studentcount].facilities =
            getField(line, 11);


        studentlist[studentcount].requirements =
            getField(line, 12);


        studentlist[studentcount].moveindate =
            getField(line, 13);


        studentcount++;
    }


    in_file.close();
}


// ============================================================
// SAVE STUDENTS
// ============================================================

bool saveStudents()
{
    ofstream out_file("students.txt");


    if (!out_file)
    {
        return false;
    }


    for (int i = 0; i < studentcount; i++)
    {
        out_file
            << studentlist[i].studentid << ";"
            << studentlist[i].name << ";"
            << studentlist[i].gender << ";"
            << studentlist[i].contact << ";"
            << studentlist[i].email << ";"
            << studentlist[i].budget << ";"
            << studentlist[i].propertytype << ";"
            << studentlist[i].roomtype << ";"
            << studentlist[i].roomsharing << ";"
            << studentlist[i].unitgender << ";"
            << studentlist[i].distancetocampus << ";"
            << studentlist[i].facilities << ";"
            << studentlist[i].requirements << ";"
            << studentlist[i].moveindate
            << endl;
    }


    out_file.close();

    return !out_file.fail();
}


// ============================================================
// LOAD OWNERS
// ============================================================

void loadOwners()
{
    ifstream in_file("owners.txt");


    if (!in_file)
    {
        ownercount = 0;
        return;
    }


    ownercount = 0;


    string line;


    while (ownercount < MAX_OWNERS &&
           getline(in_file, line))
    {
        if (line == "" || getField(line, 0) == "Owner Name")
        {
            continue;
        }


        /*
            Owner file contains:

            Owner Name;
            Owner Type;
            Contact;
            Address;
            Property Type;
            Room Type;
            Rental Price;
            Deposit;
            Distance;
            Facilities;
            Utilities;
            Cleaning;
            Available From
        */


        string ownername =
            getField(line, 0);

        string ownertype =
            getField(line, 1);

        string contact =
            getField(line, 2);

        string address =
            getField(line, 3);


        int position = -1;


        for (int i = 0; i < ownercount; i++)
        {
            if (ownerlist[i].ownername ==
                    ownername &&
                ownerlist[i].contact ==
                    contact)
            {
                position = i;
                break;
            }
        }


        if (position == -1)
        {
            ownerlist[ownercount].ownerid =
                ownercount + 1;

            ownerlist[ownercount].ownername =
                ownername;

            ownerlist[ownercount].ownertype =
                ownertype;

            ownerlist[ownercount].contact =
                contact;

            ownerlist[ownercount].address =
                address;

            ownercount++;
        }
    }


    in_file.close();
}


// ============================================================
// SAVE OWNERS
// ============================================================

void saveOwners()
{
    ofstream out_file("owners.txt");


    if (!out_file)
    {
        return;
    }


    /*
        Only owner information is stored here.

        Property information is stored separately
        in properties.txt.
    */


    for (int i = 0; i < ownercount; i++)
    {
        out_file
            << ownerlist[i].ownername << ";"
            << ownerlist[i].ownertype << ";"
            << ownerlist[i].contact << ";"
            << ownerlist[i].address
            << endl;
    }


    out_file.close();
}


// ============================================================
// CHECK WHETHER PROPERTY FILE NEEDS REGENERATION
// ============================================================




// ============================================================
// GENERATE PROPERTIES FROM OWNERS.TXT AND STUDENTS.TXT
// ============================================================

void generatePropertiesFromOwners()
{
    ifstream in_file("owners.txt");

    if (!in_file)
    {
        return;
    }

    ofstream out_file("properties.txt");

    if (!out_file)
    {
        in_file.close();
        return;
    }

    string line;
    int propertyid = 1;

    while (getline(in_file, line))
    {
        if (line == "" || getField(line, 0) == "Owner Name")
        {
            continue;
        }

        string ownername = getField(line, 0);
        string contact = getField(line, 2);
        string address = getField(line, 3);
        string propertytype = normalizePropertyType(getField(line, 4));
        string roomtype = normalizeRoomType(getField(line, 5));
        string rent = getField(line, 6);
        string deposit = getField(line, 7);
        string distance = getField(line, 8);
        string facilities = getField(line, 9);
        string utilities = getField(line, 10);
        string cleaning = getField(line, 11);
        string available = getField(line, 12);

        int ownerid = -1;

        for (int i = 0; i < ownercount; i++)
        {
            if (ownerlist[i].ownername == ownername &&
                ownerlist[i].contact == contact)
            {
                ownerid = ownerlist[i].ownerid;
                break;
            }
        }

        if (ownerid == -1 || propertytype == "" || roomtype == "" ||
            !studentHasMatchingPreference(propertytype, roomtype))
        {
            continue;
        }

        // Copy the actual property listing from owners.txt.
        // Student preferences decide which categories are relevant.
        out_file
            << propertyid << "|"
            << ownerid << "|"
            << address << "|"
            << propertytype << "|"
            << roomtype << "|"
            << rent << "|"
            << deposit << "|"
            << distance << "|"
            << facilities << "|"
            << utilities << "|"
            << cleaning << "|"
            << available << "|"
            << "1"
            << endl;

        propertyid++;
    }

    in_file.close();
    out_file.close();
}


string normalizePropertyType(string propertytype)
{
    if (propertytype == "Condo" || propertytype == "Condominium")
    {
        return "Condo";
    }

    if (propertytype == "Landed" || propertytype == "Landed House")
    {
        return "Landed";
    }

    return "";
}


string normalizeRoomType(string roomtype)
{
    if (roomtype == "Single" || roomtype == "Single Room" ||
        roomtype == "Single Bedroom")
    {
        return "Single Room";
    }

    if (roomtype == "Medium" || roomtype == "Medium Room" ||
        roomtype == "Middle Room" || roomtype == "Middle Bedroom")
    {
        return "Medium Room";
    }

    if (roomtype == "Master" || roomtype == "Master Room" ||
        roomtype == "Master Bedroom")
    {
        return "Master Room";
    }

    return "";
}


bool studentHasMatchingPreference(string propertytype, string roomtype)
{
    for (int i = 0; i < studentcount; i++)
    {
        if (normalizePropertyType(studentlist[i].propertytype) == propertytype &&
            normalizeRoomType(studentlist[i].roomtype) == roomtype)
        {
            return true;
        }
    }

    return false;
}


// ============================================================
// LOAD PROPERTIES
// ============================================================

void loadProperties()
{
    ifstream in_file("properties.txt");

    if (!in_file)
    {
        propertycount = 0;
        return;
    }

    propertycount = 0;
    string line;

    while (propertycount < MAX_PROPERTIES && getline(in_file, line))
    {
        if (line == "")
        {
            continue;
        }

        propertieslist[propertycount].propertyid = stringToInt(getField(line, 0));
        propertieslist[propertycount].ownerid = stringToInt(getField(line, 1));
        propertieslist[propertycount].name = getField(line, 2);
        propertieslist[propertycount].type = getField(line, 3);
        propertieslist[propertycount].roomtype = getField(line, 4);
        propertieslist[propertycount].rent = stringToDouble(getField(line, 5));
        propertieslist[propertycount].deposit = stringToDouble(getField(line, 6));
        propertieslist[propertycount].distance = stringToDouble(getField(line, 7));
        propertieslist[propertycount].facilities = getField(line, 8);
        propertieslist[propertycount].utilitiesincluded = getField(line, 9);
        propertieslist[propertycount].cleaningservices = getField(line, 10);
        propertieslist[propertycount].availablefrom = getField(line, 11);
        propertieslist[propertycount].active = stringToInt(getField(line, 12)) != 0;

        propertycount++;
    }

    in_file.close();
}


// ============================================================
// SAVE PROPERTIES
// ============================================================

void saveProperties()
{
    ofstream out_file("properties.txt");


    if (!out_file)
    {
        return;
    }


    for (int i = 0; i < propertycount; i++)
    {
        out_file
            << propertieslist[i].propertyid << "|"
            << propertieslist[i].ownerid << "|"
            << propertieslist[i].name << "|"
            << propertieslist[i].type << "|"
            << propertieslist[i].roomtype << "|"
            << propertieslist[i].rent << "|"
            << propertieslist[i].deposit << "|"
            << propertieslist[i].distance << "|"
            << propertieslist[i].facilities << "|"
            << propertieslist[i].utilitiesincluded << "|"
            << propertieslist[i].cleaningservices << "|"
            << propertieslist[i].availablefrom << "|"
            << propertieslist[i].active
            << endl;
    }


    out_file.close();
}


// ============================================================
// LOAD SHORTLISTS
// ============================================================

void loadShortlists()
{
    ifstream in_file("shortlists.txt");


    if (!in_file)
    {
        shortlistcount = 0;
        return;
    }


    shortlistcount = 0;


    while (shortlistcount < MAX_SHORTLISTS &&
           in_file >>
           shortlistlist[shortlistcount].studentid >>
           shortlistlist[shortlistcount].propertyid)
    {
        shortlistcount++;
    }


    in_file.close();
}


// ============================================================
// SAVE SHORTLISTS
// ============================================================

void saveShortlists()
{
    ofstream out_file("shortlists.txt");


    if (!out_file)
    {
        return;
    }


    for (int i = 0; i < shortlistcount; i++)
    {
        out_file
            << shortlistlist[i].studentid
            << " "
            << shortlistlist[i].propertyid
            << endl;
    }


    out_file.close();
}


// ============================================================
// LOAD ENQUIRIES
// ============================================================

void loadEnquiries()
{
    ifstream in_file("enquiries.txt");


    if (!in_file)
    {
        enquirycount = 0;
        return;
    }


    enquirycount = 0;


    string line;


    while (enquirycount < MAX_ENQUIRIES &&
           getline(in_file, line))
    {
        if (line == "")
        {
            continue;
        }


        enquirylist[enquirycount].enquiryid =
            stringToInt(getField(line, 0));


        enquirylist[enquirycount].studentid =
            stringToInt(getField(line, 1));


        enquirylist[enquirycount].propertyid =
            stringToInt(getField(line, 2));


        enquirylist[enquirycount].message =
            getField(line, 3);


        enquirylist[enquirycount].reply =
            getField(line, 4);


        enquirycount++;
    }


    in_file.close();
}


// ============================================================
// SAVE ENQUIRIES
// ============================================================

void saveEnquiries()
{
    ofstream out_file("enquiries.txt");


    if (!out_file)
    {
        return;
    }


    for (int i = 0; i < enquirycount; i++)
    {
        out_file
            << enquirylist[i].enquiryid << "|"
            << enquirylist[i].studentid << "|"
            << enquirylist[i].propertyid << "|"
            << enquirylist[i].message << "|"
            << enquirylist[i].reply
            << endl;
    }


    out_file.close();
}


// ============================================================
// LOAD REVIEWS
// ============================================================

void loadReviews()
{
    ifstream in_file("reviews.txt");


    if (!in_file)
    {
        reviewcount = 0;
        return;
    }


    reviewcount = 0;


    string line;


    while (reviewcount < MAX_REVIEWS &&
           getline(in_file, line))
    {
        if (line == "")
        {
            continue;
        }


        reviewslist[reviewcount].reviewid =
            stringToInt(getField(line, 0));


        reviewslist[reviewcount].studentid =
            stringToInt(getField(line, 1));


        reviewslist[reviewcount].propertyid =
            stringToInt(getField(line, 2));


        reviewslist[reviewcount].rating =
            stringToInt(getField(line, 3));


        reviewslist[reviewcount].comment =
            getField(line, 4);


        reviewcount++;
    }


    in_file.close();
}


// ============================================================
// SAVE REVIEWS
// ============================================================

void saveReviews()
{
    ofstream out_file("reviews.txt");


    if (!out_file)
    {
        return;
    }


    for (int i = 0; i < reviewcount; i++)
    {
        out_file
            << reviewslist[i].reviewid << "|"
            << reviewslist[i].studentid << "|"
            << reviewslist[i].propertyid << "|"
            << reviewslist[i].rating << "|"
            << reviewslist[i].comment
            << endl;
    }


    out_file.close();
}


// ============================================================
// LOAD APPOINTMENTS
// ============================================================

void loadAppointments()
{
    ifstream in_file("appointments.txt");


    if (!in_file)
    {
        appointmentcount = 0;
        return;
    }


    appointmentcount = 0;


    string line;


    while (appointmentcount < MAX_APPOINTMENTS &&
           getline(in_file, line))
    {
        if (line == "")
        {
            continue;
        }


        appointmentslist[appointmentcount].appointmentid =
            stringToInt(getField(line, 0));


        appointmentslist[appointmentcount].studentid =
            stringToInt(getField(line, 1));


        appointmentslist[appointmentcount].propertyid =
            stringToInt(getField(line, 2));


        appointmentslist[appointmentcount].date =
            getField(line, 3);


        appointmentslist[appointmentcount].timeslot =
            getField(line, 4);


        appointmentslist[appointmentcount].status =
            getField(line, 5);


        appointmentcount++;
    }


    in_file.close();
}


// ============================================================
// SAVE APPOINTMENTS
// ============================================================

void saveAppointments()
{
    ofstream out_file("appointments.txt");


    if (!out_file)
    {
        return;
    }


    for (int i = 0; i < appointmentcount; i++)
    {
        out_file
            << appointmentslist[i].appointmentid << "|"
            << appointmentslist[i].studentid << "|"
            << appointmentslist[i].propertyid << "|"
            << appointmentslist[i].date << "|"
            << appointmentslist[i].timeslot << "|"
            << appointmentslist[i].status
            << endl;
    }


    out_file.close();
}


// ============================================================
// ADD STUDENT
// ============================================================

bool addStudent(Student newstudent)
{
    if (studentcount >= MAX_STUDENTS)
    {
        return false;
    }


    studentlist[studentcount] =
        newstudent;


    studentcount++;


    if (!saveStudents())
    {
        studentcount--;
        return false;
    }


    return true;
}


// ============================================================
// UPDATE STUDENT
// ============================================================

bool updateStudent(Student newstudent)
{
    int position =
        findStudent(newstudent.studentid);


    if (position == -1)
    {
        return false;
    }


    studentlist[position] =
        newstudent;


    if (!saveStudents())
    {
        return false;
    }


    return true;
}


// ============================================================
// FIND STUDENT
// ============================================================

int findStudent(int studentid)
{
    for (int i = 0; i < studentcount; i++)
    {
        if (studentlist[i].studentid ==
            studentid)
        {
            return i;
        }
    }


    return -1;
}


// ============================================================
// ADD OWNER
// ============================================================

bool addOwner(Owner newowner)
{
    if (ownercount >= MAX_OWNERS)
    {
        return false;
    }


    ownerlist[ownercount] =
        newowner;


    ownercount++;


    saveOwners();


    return true;
}


// ============================================================
// UPDATE OWNER
// ============================================================

bool updateOwner(Owner newowner)
{
    int position =
        findOwner(newowner.ownerid);


    if (position == -1)
    {
        return false;
    }


    ownerlist[position] =
        newowner;


    saveOwners();


    return true;
}


// ============================================================
// FIND OWNER
// ============================================================

int findOwner(int ownerid)
{
    for (int i = 0; i < ownercount; i++)
    {
        if (ownerlist[i].ownerid ==
            ownerid)
        {
            return i;
        }
    }


    return -1;
}


// ============================================================
// ADD PROPERTY
// ============================================================

bool addProperty(Property newproperty)
{
    if (propertycount >= MAX_PROPERTIES)
    {
        return false;
    }


    propertieslist[propertycount] =
        newproperty;


    propertycount++;


    saveProperties();


    return true;
}


// ============================================================
// UPDATE PROPERTY
// ============================================================

bool updateProperty(Property newproperty)
{
    int position =
        findProperty(
            newproperty.propertyid);


    if (position == -1)
    {
        return false;
    }


    propertieslist[position] =
        newproperty;


    saveProperties();


    return true;
}


// ============================================================
// DELETE PROPERTY
// ============================================================

bool deleteProperty(int propertyid)
{
    int position =
        findProperty(propertyid);


    if (position == -1)
    {
        return false;
    }


    /*
        We do not physically remove the
        structure.

        Instead, active becomes false.
    */

    propertieslist[position].active =
        false;


    saveProperties();


    return true;
}


// ============================================================
// FIND PROPERTY
// ============================================================

int findProperty(int propertyid)
{
    for (int i = 0; i < propertycount; i++)
    {
        if (propertieslist[i].propertyid ==
            propertyid)
        {
            return i;
        }
    }


    return -1;
}


// ============================================================
// CHECK SHORTLIST EXISTS
// ============================================================

bool shortlistExists(int studentid,
                     int propertyid)
{
    for (int i = 0; i < shortlistcount; i++)
    {
        if (shortlistlist[i].studentid ==
                studentid &&
            shortlistlist[i].propertyid ==
                propertyid)
        {
            return true;
        }
    }


    return false;
}


// ============================================================
// ADD SHORTLIST
// ============================================================

bool addShortlist(Shortlist newshortlist)
{
    if (shortlistcount >=
        MAX_SHORTLISTS)
    {
        return false;
    }


    if (shortlistExists(
            newshortlist.studentid,
            newshortlist.propertyid))
    {
        return false;
    }


    shortlistlist[shortlistcount] =
        newshortlist;


    shortlistcount++;


    saveShortlists();


    return true;
}


// ============================================================
// DELETE SHORTLIST
// ============================================================

bool deleteShortlist(int studentid,
                     int propertyid)
{
    for (int i = 0;
         i < shortlistcount;
         i++)
    {
        if (shortlistlist[i].studentid ==
                studentid &&
            shortlistlist[i].propertyid ==
                propertyid)
        {
            for (int j = i;
                 j < shortlistcount - 1;
                 j++)
            {
                shortlistlist[j] =
                    shortlistlist[j + 1];
            }


            shortlistcount--;


            saveShortlists();


            return true;
        }
    }


    return false;
}


// ============================================================
// ADD ENQUIRY
// ============================================================

bool addEnquiry(Enquiry newenquiry)
{
    if (enquirycount >=
        MAX_ENQUIRIES)
    {
        return false;
    }


    enquirylist[enquirycount] =
        newenquiry;


    enquirycount++;


    saveEnquiries();


    return true;
}


// ============================================================
// UPDATE ENQUIRY REPLY
// ============================================================

bool updateEnquiryReply(int enquiryid,
                        string reply)
{
    int position =
        findEnquiry(enquiryid);


    if (position == -1)
    {
        return false;
    }


    enquirylist[position].reply =
        reply;


    saveEnquiries();


    return true;
}


// ============================================================
// FIND ENQUIRY
// ============================================================

int findEnquiry(int enquiryid)
{
    for (int i = 0;
         i < enquirycount;
         i++)
    {
        if (enquirylist[i].enquiryid ==
            enquiryid)
        {
            return i;
        }
    }


    return -1;
}


// ============================================================
// ADD REVIEW
// ============================================================

bool addReview(Review newreview)
{
    if (reviewcount >=
        MAX_REVIEWS)
    {
        return false;
    }


    reviewslist[reviewcount] =
        newreview;


    reviewcount++;


    saveReviews();


    return true;
}


// ============================================================
// FIND REVIEW
// ============================================================

int findReview(int reviewid)
{
    for (int i = 0;
         i < reviewcount;
         i++)
    {
        if (reviewslist[i].reviewid ==
            reviewid)
        {
            return i;
        }
    }


    return -1;
}


// ============================================================
// ADD APPOINTMENT
// ============================================================

bool addAppointment(
    Appointment newappointment)
{
    if (appointmentcount >=
        MAX_APPOINTMENTS)
    {
        return false;
    }


    appointmentslist[appointmentcount] =
        newappointment;


    appointmentcount++;


    saveAppointments();


    return true;
}


// ============================================================
// UPDATE APPOINTMENT STATUS
// ============================================================

bool updateAppointmentStatus(
    int appointmentid,
    string status)
{
    int position =
        findAppointment(appointmentid);


    if (position == -1)
    {
        return false;
    }


    appointmentslist[position].status =
        status;


    saveAppointments();


    return true;
}


// ============================================================
// FIND APPOINTMENT
// ============================================================

int findAppointment(int appointmentid)
{
    for (int i = 0;
         i < appointmentcount;
         i++)
    {
        if (appointmentslist[i].appointmentid ==
            appointmentid)
        {
            return i;
        }
    }


    return -1;
}

