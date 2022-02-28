#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <regex>
#include <map>
#include <fstream>
#include <sstream>

// Just testing out the preprocessor directives :P
#define LEFT(x, y, z)   "| " << setw(x) << left << string( z * ceil((x - y.length()) / 2), ' ') + y << " "
#define RIGHT(x, y, z)  "| " << setw(x) << right << y + string( z * ceil((x - y.length()) / 2), ' ') << " "
#define ts(x) to_string(x)

using namespace std;

enum basicCommand {
    RETAIN_SCREEN = 0, CLEAR_SCREEN = 1, HIDE_GRADES = 0, DISPLAY_GRADES = 1, HIDE_INDEX = 0, DISPLAY_INDEX = 1,
    HIDE_PROMPT = 0, PROMPT = 1, HIDE_TIME = 0, DISPLAY_TIME = 1, HIDE_IDENTITY = 0, DISPLAY_IDENTITY = 1,
    POSITION_DEFAULT = 0, POSITION_CENTER = 1
};

int input;
void validateInput(unsigned int maxOption, basicCommand promptForInput) {
    int digit = 0;
    int temp = maxOption;
    while (temp > 0) {
        temp /= 10;
        digit++;
    }
    if (digit > 4) {
        cout << "ERROR: Maximum option exceeded limit.\nTry filtering to lower the options available.\n" << endl;
        return;
    }
    vector<int> digits;
    for (int i = 0; i < 4; i++) {
        digits.push_back((int)(maxOption / pow(10, i)) % 10);
    }
    int ones = digits[0], tens = digits[1], hundreds = digits[2], thousands = digits[3];
    regex r;
    string ONES = "[0-" + ts(ones) + "]";
    string to9 = "[1-9]|", to99 = to9 + "[1-9]\\d|", to999 = to99 + "[1-9]\\d\\d|";
    switch (digit) {
    case 1:
        r = "[1-" + ts(maxOption) + "]";
        break;
    case 2:
        if (tens == 1)
            r = to9 + "1" + ONES;
        else
            r = to9 + "[1-" + ts(tens - 1) + "]\\d|" + ts(tens) + ONES;
        break;
    case 3:
        if (hundreds == 1) {
            if (tens == 0)
                r = to99 + "10" + ONES;
            else
                r = to99 + "1[0-" + ts(tens - 1) + "]\\d|1" + ts(tens) + ONES;
        }
        else {
            if (tens == 0)
                r = to99 + "[1-" + ts(hundreds - 1) + "]\\d\\d|" + ts(hundreds) + "0" + ONES;
            else
                r = to99 + "[1-" + ts(hundreds - 1) + "]\\d\\d|" + ts(hundreds) + "[0-" + ts(tens - 1) + "]\\d|" + ts(hundreds) + ts(tens) + ONES;
        }
        break;
    case 4:
        if (thousands == 1) {
            if (hundreds == 0) {
                if (tens == 0)
                    r = to999 + "100" + ONES;
                else
                    r = to999 + "10[0-" + ts(tens - 1) + "]\\d|10" + ts(tens) + ONES;
            }
            else {
                if (tens == 0)
                    r = to999 + "1[0-" + ts(hundreds - 1) + "]\\d\\d|1" + ts(hundreds) + "0" + ONES;
                else
                    r = to999 + "1[0-" + ts(hundreds - 1) + "]\\d\\d|1" + ts(hundreds) + "[0-" + ts(tens - 1) + "]\\d|1" + ts(hundreds) + ts(tens) + ONES;
            }
        }
        else {
            if (hundreds == 0) {
                if (tens == 0)
                    r = to999 + "[1-" + ts(thousands - 1) + "]\\d\\d\\d|" + ts(thousands) + "00" + ONES;
                else
                    r = to999 + "[1-" + ts(thousands - 1) + "]\\d\\d\\d|" + ts(thousands) + "0[0-" + ts(tens - 1) + "]\\d|" + ts(thousands) + "0" + ts(tens) + ONES;
            }
            else {
                if (tens == 0)
                    r = to999 + "[1-" + ts(thousands - 1) + "]\\d\\d\\d|" + ts(thousands) + "[0-" + ts(hundreds - 1) + "]\\d\\d|" + ts(thousands) + ts(hundreds) + "0" + ONES;
                else
                    r = to999 + "[1-" + ts(thousands - 1) + "]\\d\\d\\d|" + ts(thousands) + "[0-" + ts(hundreds - 1) + "]\\d\\d|" + ts(thousands) + ts(hundreds) + "[0-" + ts(tens - 1) + "]\\d|" + ts(thousands) + ts(hundreds) + ts(tens) + ONES;
            }
        }
        break;
    }
    string choice;
    if (promptForInput)
        cout << "Enter choice: ";
    getline(cin, choice);
    while (!regex_match(choice, r)) {
        cout << "Invalid input. Enter choice: ";
        getline(cin, choice);
    }
    input = stoi(choice);
    cout << endl;
};

struct subject {
    string code, name, type, grade;
    int credit, marks = 0;
    float gradePoint = 0;
    subject(string a, string b, string c, int d) {
        code = a;
        name = b;
        type = c;
        credit = d;
    };
};
vector<subject> coreSubjects = {
    subject("TCYS133", "ALGORITHM AND DATA STRUCTURE", "C", 3),
    subject("TIST223", "DATABASE CONCEPTS", "C", 3),
    subject("TMAT244", "DISCRETE MATHEMATICS", "C", 4),
    subject("TPRJ384", "INDUSTRIAL TRAINING", "C", 4),
    subject("TCYS123", "INTRODUCTION OF FORENSIC AND TECHNIQUES", "C", 3),
    subject("TGEN123", "INTRODUCTION TO INFORMATION TECHNOLOGY", "C", 3),
    subject("TGEN153", "INTRODUCTION TO OPERATING SYSTEMS", "C", 3),
    subject("TCYS224", "INTRODUCTION TO SECURITY TECHNIQUES", "C", 4),
    subject("TGEN164", "IT ETHICS", "C", 4),
    subject("TMAT113", "MATHEMATICS", "C", 3),
    subject("TNWK343", "NETWORK SECURITY", "C", 3),
    subject("TNWK213", "NETWORKING", "C", 3),
    subject("TGEN133", "PC MAINTENANCE", "C", 3),
    subject("TPRG113", "PROGRAMMING CONCEPTS", "C", 3),
    subject("TCPJ212", "PROJECT", "C", 4),
    subject("TCYS113", "SECURITY IN COMPUTING", "C", 3),
    subject("TNWK223", "SYSTEM ADMINISTRATION", "C", 3),
    subject("TIST234", "SYSTEM ANALYSIS AND DESIGN", "C", 4)
};
vector<subject> electives = {
    subject("TMAT133", "ADVANCED MATHEMATICS", "E", 3),
    subject("TGEN273", "CREATIVE THINKING", "E", 3),
    subject("TPRG343", "INTRODUCTION TO ANDROID PROGRAMMING", "E", 3),
    subject("TMAT123", "INTRODUCTION TO STATISTICS", "E", 3),
    subject("TGEN183", "TECHNOPRENEURSHIP", "E", 3),
    subject("TUPJ322", "STUDY SKILL", "E", 2),
    subject("TUPJ312", "UNIVERSITY RELATION PROGRAM", "E", 2)
};
vector<subject> commonSubjects = {
    subject("UCENG2112", "ACADEMIC ENGLISH PROGRAMME (ELEMENTARY) FOR DIPLOMA LEVEL", "CC", 2),
    subject("UCENG2122", "ACADEMIC ENGLISH PROGRAMME (INTERMEDIATE) FOR DIPLOMA LEVEL", "CC", 2),
    subject("MPU2252", "ADVANCED CHINESE LANGUAGE", "CC", 2),
    subject("MPU1322", "ART APPRECIATION", "CC", 2),
    subject("MPU2482", "BADMINTON", "CC", 2),
    subject("MPU2213", "BAHASA KEBANGSAAN A", "CC", 3),
    subject("MPU2462", "BASKETBALL", "CC", 2),
    subject("MPU2262", "CERAMICS APPRECIATION", "CC", 2),
    subject("MPU3282", "CHINESE CALLIGRAPHY", "CC", 2),
    subject("MPU3412", "COMMUNITY SERVICE", "CC", 2),
    subject("MPU3352", "CONFUCIANISM AND MODERN SOCIETY", "CC", 2),
    subject("MPU2332", "FOLKLIFE AND COMMUNITY", "CC", 2),
    subject("MPU2322", "GOVERNMENT AND POLITICS IN MALAYSIA", "CC", 3),
    subject("MPU2312", "INTRODUCTION TO SOCIAL SCIENCES", "CC", 3),
    subject("MPU2422", "KORFBALL", "CC", 2),
    subject("MPU2392", "LIFE EDUCATION AND SOCIAL CONCERN", "CC", 2),
    subject("MPU2163", "MALAYSIAN STUDIES 2", "CC", 3),
    subject("ENGB200", "PUBLIC SPEAKING", "CC", 3),
    subject("ENGB310", "RESEARCH WRITING SKILL", "CC", 3),
    subject("UCENG2133", "WORKPLACE ENGLISH", "CC", 3),
    subject("MPU2432", "TAIJI", "CC", 2)
};
vector<int> randomSubjects(int lowestOption, int highestOption, int size, vector<int> unwantedValues) {
    vector<int> output;
    int r;
    auto rIsIncludedIn = [&](vector<int> v) {
        for (int i = 0; i < v.size(); i++) {
            if (v[i] == r)
                return true;
        }return false;
    };
    while (output.size() < size) {
        r = rand() % (highestOption - lowestOption + 1) + lowestOption;
        if (rIsIncludedIn(unwantedValues))
            continue;
        if (!rIsIncludedIn(output))
            output.push_back(r);
    }
    return output;
};
void print(vector<subject> v, basicCommand displayGrades, basicCommand displayIndex) {
    int maxCodeWidth = 0, maxNameWidth = 0;
    for (int i = 0; i < v.size(); i++) {
        maxCodeWidth += (v[i].code.length() > maxCodeWidth) * (v[i].code.length() - maxCodeWidth);
        maxNameWidth += (v[i].name.length() > maxNameWidth) * (v[i].name.length() - maxNameWidth);
    }
    string code = "Code", name = "Name";
    cout << (displayIndex ? "   " : "")
        << string(maxCodeWidth + maxNameWidth + 23 + displayGrades * 22, '=') << endl;
    //cout << (displayIndex ? "   " : "") << LEFT(maxCodeWidth, code, POSITION_CENTER)
    //    << LEFT(maxNameWidth, name, POSITION_CENTER) << "| Type | Credit |"
    //    << (displayGrades ? " Grade | Grade Point |" : "") << endl;
    cout << (displayIndex ? "   " : "") << RIGHT(maxCodeWidth, code, POSITION_CENTER)
        << RIGHT(maxNameWidth, name, POSITION_CENTER) << "| Type | Credit |"
        << (displayGrades ? " Grade | Grade Point |" : "") << endl;
    cout << (displayIndex ? "   " : "")
        << string(maxCodeWidth + maxNameWidth + 23 + displayGrades * 22, '=') << endl;
    if (displayGrades) {
        for (int i = 0; i < v.size(); i++) {
            //cout << (displayIndex ? ts(i + 1).append(". ") : "") << LEFT(maxCodeWidth, v[i].code, POSITION_CENTER)
            //    << LEFT(maxNameWidth, v[i].name, POSITION_DEFAULT) << LEFT(4, v[i].type, POSITION_CENTER) << "|  "
            //    << v[i].credit << ".0   " << LEFT(5, v[i].grade, POSITION_CENTER);
            cout << (displayIndex ? ts(i + 1).append(". ") : "") << RIGHT(maxCodeWidth, v[i].code, POSITION_CENTER)
                << LEFT(maxNameWidth, v[i].name, POSITION_DEFAULT) << RIGHT(4, v[i].type, POSITION_CENTER) << "|   "
                << v[i].credit << ".0  " << RIGHT(5, v[i].grade, POSITION_CENTER);
            if (v[i].grade == "")
                cout << "|             |" << endl;
            else
                cout << "|     " << fixed << setprecision(2) << v[i].gradePoint << "    |" << endl;
        }
    }
    else {
        for (int i = 0; i < v.size(); i++) {
            //cout << (displayIndex ? ts(i + 1).append(". ") : "") << LEFT(maxCodeWidth, v[i].code, POSITION_CENTER)
            //    << LEFT(maxNameWidth, v[i].name, POSITION_DEFAULT) << LEFT(4, v[i].type, POSITION_CENTER)
            //    << LEFT(6, ts(v[i].credit), POSITION_CENTER) << "|" << endl;
            cout << (displayIndex ? ts(i + 1).append(". ") : "") << RIGHT(maxCodeWidth, v[i].code, POSITION_CENTER)
                << LEFT(maxNameWidth, v[i].name, POSITION_DEFAULT) << RIGHT(4, v[i].type, POSITION_CENTER)
                << RIGHT(6, ts(v[i].credit), POSITION_CENTER) << "|" << endl;
        }
    }
    cout << (displayIndex ? "   " : "") << string(maxCodeWidth + maxNameWidth + 23 + displayGrades * 22, '=') << endl;
};

time_t time_now;
tm now;
string DAYS[7] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};
string MONTHS[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};
string zerosPadded(int input, int digits) {
    string output = ts(input);
    while (output.length() < digits)
        output = "0" + output;
    return output;
};
void currentTime(basicCommand displayTime) {
    time_now = time(0);
    localtime_s(&now, &time_now);
    if (displayTime)
        cout << "Current local time: " << DAYS[now.tm_wday] << ", " << now.tm_mday << " "
        << MONTHS[now.tm_mon] << " " << now.tm_year + 1900 << " " << zerosPadded(now.tm_hour, 2)
        << ":" << zerosPadded(now.tm_min, 2) << ":" << zerosPadded(now.tm_sec, 2) << "\n" << endl;
};

struct gameRecord {
public:
    string studentID, studentName, date, time, elapsedTime;
    int beginningTime, year, month, day, hour, min, sec, elapsedSeconds, score;
    //bool hide = 0;
    gameRecord(string a, string b, int c, int d, int e, int f, int g, int h, int i, int j, int k) {
        studentID = a;
        studentName = b;
        beginningTime = c;
        year = d;
        month = e;
        day = f;
        hour = g;
        min = h;
        sec = i;
        elapsedSeconds = j;
        score = k;
    };

    void finalize() {
        int days = 0, hours = 0, minutes = 0, seconds = 0;
        string Days, Hours, Minutes, Seconds;
        date = zerosPadded(day, 2) + "/" + zerosPadded(month + 1, 2) + "/" + ts(year);  // 10 chars
        time = zerosPadded(hour - (hour > 12) * 12 + (hour == 0) * 12, 2) + ":" + zerosPadded(min, 2)
            + ":" + zerosPadded(sec, 2) + (hour > 12 ? " p.m." : " a.m.");           // 13 chars
        if (elapsedSeconds < 60) {
            seconds = elapsedSeconds + (elapsedSeconds == 0) * 1;
            Seconds = ts(seconds) + " second" + (seconds == 1 ? "" : "s");
            elapsedTime = Seconds;
        }
        else if (elapsedSeconds < 3600) {
            minutes = elapsedSeconds / 60;
            seconds = elapsedSeconds - minutes * 60;
            Minutes = ts(minutes) + " minute" + (minutes == 1 ? "" : "s");
            Seconds = seconds == 0 ? "" : " " + ts(seconds) + " second" + (seconds == 1 ? "" : "s");
            elapsedTime = Minutes + Seconds;
        }
        else if (elapsedSeconds < 86400) {
            hours = elapsedSeconds / 3600;
            minutes = (elapsedSeconds - hours * 3600) / 60;
            seconds = elapsedSeconds - hours * 3600 - minutes * 60;
            Hours = ts(hours) + " hour" + (hours == 1 ? "" : "s");
            Minutes = minutes == 0 ? "" : " " + ts(minutes) + " minute" + (minutes == 1 ? "" : "s");
            Seconds = seconds == 0 ? "" : " " + ts(seconds) + " second" + (seconds == 1 ? "" : "s");
            elapsedTime = Hours + Minutes + Seconds;
        }
        else {
            days = elapsedSeconds / 86400;
            hours = (elapsedSeconds - days * 86400) / 3600;
            minutes = (elapsedSeconds - days * 86400 - hours * 3600) / 60;
            seconds = elapsedSeconds - days * 86400 - hours * 3600 - minutes * 60;
            Days = ts(days) + " day" + (days == 1 ? "" : "s");
            Hours = hours == 0 ? "" : " " + ts(hours) + " hour" + (hours == 1 ? "" : "s");
            Minutes = minutes == 0 ? "" : " " + ts(minutes) + " minute" + (minutes == 1 ? "" : "s");
            Seconds = seconds == 0 ? "" : " " + ts(seconds) + " second" + (seconds == 1 ? "" : "s");
            elapsedTime = Days + Hours + Minutes + Seconds;
        }
    };
};
vector<gameRecord> studentGameHistory;
void updateTreasureGameCSV() {
    sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) {
        return a.studentID < b.studentID; });
    ofstream newCSV("new.csv", ios::out | ios::app);
    for (int i = 0; i < studentGameHistory.size(); i++) {
        gameRecord* gr = &studentGameHistory[i];
        newCSV << gr->studentID << "," << gr->studentName << "," << gr->beginningTime << ","
            << gr->year << "," << gr->month << "," << gr->day << "," << gr->hour << ","
            << gr->min << "," << gr->sec << "," << gr->elapsedSeconds << "," << gr->score;
        if (i < studentGameHistory.size() - 1)
            newCSV << "\n";
    }
    newCSV.close();
    remove("Treasure Game History.csv");
    rename("new.csv", "Treasure Game History.csv");
};
void updateStudentListCSV();    // function prototype

enum STATUS { FULL_TIME, PART_TIME, GRADUATED, PROBATION, WITHDRAWN, DISMISSED };
map<enum STATUS, string> statusList{
    {FULL_TIME, "Full-Time"}, {PART_TIME, "Part-Time"}, {GRADUATED, "Graduated"},
    {PROBATION, "Under Probation"}, {WITHDRAWN, "Withdrawn"}, {DISMISSED, "Dismissed"}
};
map<string, int> studentCount;

class student {
public:
    string ID = "", name = "", icNo = "", gender = "", contactNo = "", email = "", address = "";
    string programme = "Diploma of Computer Science", programmeCode = "DCS";
    STATUS status = FULL_TIME;                              // 2 (10 11 12 1) 5 (2 3 4 5) 9 (6 7 8 9)
    int entryYear = now.tm_year + 1900 + (now.tm_mon > 8);  // 1 (9 10 11 0)  2 (1 2 3 4) 3 (5 6 7 8)
    int batchNum = now.tm_mon == 0 ? 1 : 2 + (now.tm_mon > 4) * 1 - (now.tm_mon > 8) * 2;
    int totalCreditsEarned = 0, trimester = 0;
    vector<int> randomElectives = randomSubjects(0, 6, 4, {});
    vector<int> randomCommonSubjects = randomSubjects(3, 20, 3, { 5, 16 });
    vector<vector<subject>> defaultArrangement = {
        {   // subjects for 1st trimester
            subject("TCYS123", "INTRODUCTION OF FORENSIC AND TECHNIQUES", "C", 3),
            subject("TGEN123", "INTRODUCTION TO INFORMATION TECHNOLOGY", "C", 3),
            subject("TGEN164", "IT ETHICS", "C", 4),
            subject("TMAT113", "MATHEMATICS", "C", 3),
            subject("TPRG113", "PROGRAMMING CONCEPTS", "C", 3),
            subject("UCENG2112", "ACADEMIC ENGLISH PROGRAMME (ELEMENTARY) FOR DIPLOMA LEVEL", "CC", 2),
            subject("MPU2252", "ADVANCED CHINESE LANGUAGE", "CC", 2)
        },
        {   // subjects for 2nd trimester
            subject("MPU2213", "BAHASA KEBANGSAAN A", "CC", 3),
            commonSubjects[randomCommonSubjects[0]],
            subject("MPU2163", "MALAYSIAN STUDIES 2", "CC", 3)
        },
        {   // subjects for 3rd trimester
            subject("TIST223", "DATABASE CONCEPTS", "C", 3),
            subject("TGEN153", "INTRODUCTION TO OPERATING SYSTEMS", "C", 3),
            subject("TNWK213", "NETWORKING", "C", 3),
            subject("TGEN133", "PC MAINTENANCE", "C", 3),
            electives[randomElectives[0]],
            electives[randomElectives[1]],
            subject("UCENG2122", "ACADEMIC ENGLISH PROGRAMME (INTERMEDIATE) FOR DIPLOMA LEVEL", "CC", 2)
        },
        {   // subjects for 4th trimester
            subject("TCYS133", "ALGORITHM AND DATA STRUCTURE", "C", 3),
            subject("TNWK343", "NETWORK SECURITY", "C", 3),
            subject("TCYS113", "SECURITY IN COMPUTING", "C", 3),
            subject("TNWK223", "SYSTEM ADMINISTRATION", "C", 3),
            electives[randomElectives[2]],
            commonSubjects[randomCommonSubjects[1]]
        },
        {   // subjects for 5th trimester
            subject("TPRJ384", "INDUSTRIAL TRAINING", "C", 4)
        },
        {   // subjects for 6th trimester
            subject("TMAT244", "DISCRETE MATHEMATICS", "C", 4),
            subject("TCYS224", "INTRODUCTION TO SECURITY TECHNIQUES", "C", 4),
            subject("TCPJ212", "PROJECT", "C", 4),
            subject("TIST234", "SYSTEM ANALYSIS AND DESIGN", "C", 4),
            electives[randomElectives[3]],
            commonSubjects[randomCommonSubjects[2]]
        }
    };
    vector<gameRecord> gameHistory;
    student() {};
    student(int a, int b, string c, string d, string e) {
        entryYear = a;
        batchNum = b;
        name = c;
        icNo = d;
        gender = e;
    };
    student(int a, int b, string c, string d, int e, int f, string g, string h, string i, string j, string k) {
        entryYear = a;
        batchNum = b;
        ID = c;
        name = d;
        status = STATUS(e);
        trimester = f;
        icNo = g;
        gender = h;
        contactNo = i;
        email = j;
        address = k;
    };

    void randomNum() {
        string s = "01";
        for (int i = 0; i < 8; i++) {
            s += ts(rand() % 10);
        }
        this->contactNo = s;
    };
    void randomEmail(string name, string icNo) {      // generate email using name and IC number
        queue<int> space_pos;
        for (int i = 0; i < name.length(); i++) {
            if (name.at(i) == ' ')
                space_pos.push(i);
        }
        string Email = name.substr(0, space_pos.front());
        while (!space_pos.empty()) {
            Email += name.at(space_pos.front() + 1);
            space_pos.pop();
        }
        int r = rand() % 10;
        Email += icNo.substr(2, 4) + "@" + ((r < 5) ? "gmail" : ((r < 8) ? "outlook" : "qq")) + ".com";
        for (int i = 0; i < Email.length(); i++) {
            Email[i] = tolower(Email[i]);
        }
        this->email = Email;
    };
    void randomAddress() {
        string taman[35] = {
            "Sri Rampai", "Bukit Jaya", "Mewah", "Berlian", "Melati", "Seruling", "Petani Jaya", "Ria", "Ria Jaya",
            "Selayang", "Mutiara", "Indah", "Gemilang", "Raintree", "Permai Jaya", "Pelangi", "Anggun", "Bunga Raya",
            "Panglima", "Teluk Intan", "Seri Setia", "Jaya", "Makmur", "Seri Idaman", "Cahaya", "Bahagia", "Desa Wangsa",
            "Merah", "Kurnia Jaya", "Kemumin", "Bandar Baru", "Angsana", "Damai", "Hijrah", "Pengkalan Indah"
        };
        string state[13] = {
            "Shah Alam, Selangor", "Kuala Terengganu, Terengganu", "Kuching, Sarawak", "Kota Kinabalu, Sabah",
            "Alor Setar, Kedah", "Kota Bahru, Kelantan", "Seremban, Negeri Sembilan", "George Town, Pulau Pinang",
            "Johor Bahru, Johor", "Melaka, Melaka", "Kangar, Perlis", "Ipoh, Perak", "Kuantan, Pahang"
        };
        string posCode[13] = {
            ts(rand() % 8301 + 48000), ts(rand() % 4301 + 20000), ts(rand() % 5860 + 93000), ts(rand() % 3310 + 88000),
            "0" + ts(rand() % 4811 + 5000), ts(rand() % 3501 + 15000), ts(rand() % 3510 + 70000), ts(rand() % 4401 + 10000),
            ts(rand() % 7901 + 79000), ts(rand() % 3310 + 75000), "0" + ts(rand() % 2000 + 1000), ts(rand() % 6811 + 30000),
            ts(rand() % 3801 + 25000)
        };
        int r = rand() % 13;
        string Address = ts(rand() % 99 + 1) + ", Jln " + ts(rand() % 12 + 1) + "/" + ts(rand() % 5 + 3)
            + ", Taman " + taman[rand() % 35] + ", " + posCode[r] + ", " + state[r];
        this->address = Address;
    };
    void setID() {
        string prefix = ts(entryYear % 100) + ts(batchNum);
        ID = prefix + zerosPadded(++studentCount[prefix], 4) + "-" + programmeCode;
    };
    void refreshTrimester() {                           // 2 (2 3 4) 5 (5 6 7 8 9) 9 (10 11 12 1)
        int prefix = entryYear % 100 * 10 + batchNum;   // 1 (1 2 3) 2 (4 5 6 7 8) 3 (9 10 11 0)
        int currentYear = now.tm_year + 1900 + (now.tm_mon == 0) * -1;
        int currentBatch = now.tm_mon == 0 ? 3 : 1 + (now.tm_mon > 3) + (now.tm_mon > 9);
        int currentYearBatch = currentYear % 100 * 10 + currentBatch;
        trimester = status == GRADUATED ? trimester : ((currentYearBatch < prefix) ? 0 :
            1 + (currentYear - entryYear) * 3 + currentBatch - batchNum);
    };
    void homePage() {
        system("CLS");
        currentTime(DISPLAY_TIME);
        cout << "Welcome " << name << ", what would you like to do today?\n" << endl;
        int option = 0;
        cout << ++option << ". View and update student profile" << endl;
        cout << ++option << ". View grades" << endl;
        cout << ++option << ". Play the treasure game" << endl;
        if (!gameHistory.empty())
            cout << ++option << ". View treasure game history" << endl;
        cout << ++option << ". Quit\n" << endl;
        validateInput(option, PROMPT);
        if (option == 4) {
            switch (input) {
            case 1:
                getDetails(CLEAR_SCREEN);
                updateSelection();
                break;
            case 2:
                getResults();
                homePage();
                break;
            case 3:
                playTreasureGame();
                break;
            case 4:
                cout << "Program terminated." << endl;
                exit(0);
            }
        }
        else {
            switch (input) {
            case 1:
                getDetails(CLEAR_SCREEN);
                updateSelection();
                break;
            case 2:
                getResults();
                homePage();
                break;
            case 3:
                playTreasureGame();
                break;
            case 4:
                viewGameHistory();
                break;
            case 5:
                cout << "Program terminated." << endl;
                exit(0);
            }
        }
    };
    void printf(string attributeName, string attributeValue, int width) {
        cout << setw(width) << left << attributeName << ": " << attributeValue << endl;
    };
    void getDetails(basicCommand clearScreen) {
        if (clearScreen)
            system("CLS");
        printf("Student ID", ID, 15);
        printf("Name", name, 15);
        printf("Programme", programme, 15);
        printf("Status", statusList.find(status)->second, 15);
        printf("Trimester", ts(trimester), 15);
        printf("Credits Earned", ts(totalCreditsEarned) + ".0", 15);
        printf("IC Number", icNo, 15);
        printf("Gender", gender, 15);
        printf("Contact Number", contactNo, 15);
        printf("Email", email, 15);
        printf("Address", address, 15);
        cout << endl;
    };
    void getResults() {
        system("CLS");
        int creditsThisSem = 0, totalCredits = 0;
        double GPA = 0, CGPA = 0;
        printf("Student ID", ID, 18);
        printf("Name", name, 18);
        printf("Programme", programme, 18);
        printf("Current trimester", ts(trimester), 18);
        cout << endl;
        if (trimester <= 1) {
            string authorizationFailedWarning = "You are not authorized to view this page";
            cout << string(60, '=') << "\n|" << string(58, ' ') << "|\n"
                << RIGHT(56, authorizationFailedWarning, POSITION_CENTER) << "|"
                << "\n|" << string(58, ' ') << "|\n" << string(60, '=') << endl;
            cout << "\n1.Back" << endl;
            validateInput(1, PROMPT);
            homePage();
            return;
        }
        auto credits = [&](vector<subject> v) {
            int sum = 0;
            for (int i = 0; i < v.size(); i++) {
                sum += (v[i].marks >= 40) * v[i].credit;
            }return sum;
        };
        auto gp = [&](vector<subject> v) {
            float sumProduct = 0;
            for (int i = 0; i < v.size(); i++) {
                sumProduct += v[i].gradePoint * v[i].credit;
            }return sumProduct;
        };
        for (int i = 0; i < trimester - 1; i++) {
            cout << "Trimester " << i + 1 << endl;
            print(defaultArrangement[i], DISPLAY_GRADES, HIDE_INDEX);
            creditsThisSem = credits(defaultArrangement[i]);
            totalCredits += creditsThisSem;
            GPA = gp(defaultArrangement[i]) / creditsThisSem;
            CGPA = (CGPA * (totalCredits - creditsThisSem) + GPA * creditsThisSem) / totalCredits;
            cout << "Credits earned: " << creditsThisSem << ".0\t"
                << "Total credits earned: " << totalCredits << ".0\t"
                << "GPA: " << fixed << setprecision(4) << GPA << "\tCGPA: " << fixed << setprecision(4) << CGPA << endl;
            cout << endl;
        }
        cout << "1. Back" << endl;
        validateInput(1, PROMPT);
    };
    void updateResults(subject* sub, int marks) {
        if (sub->marks >= 40)
            totalCreditsEarned -= sub->credit;
        sub->marks = marks;
        sub->gradePoint = (marks >= 30) * 1.00f + (marks >= 80) * 3.00f + (marks < 80 && marks >= 40) * (float)((marks - 35) / 5) / 3;
        sub->grade = (char)(65 + (marks < 75) + (marks < 60) + (marks < 50) +
            (marks < 40) + (marks < 30));
        if ((marks >= 75 && marks < 80) || (marks >= 60 && marks < 65))
            sub->grade += "-";
        else if (marks >= 85 || (marks >= 70 && marks < 75) || (marks >= 55 && marks < 60) || (marks >= 45 && marks < 50))
            sub->grade += "+";
        totalCreditsEarned += (marks >= 40) * sub->credit;
    };
    void update(string* info, string s, regex r) {
        string newInfo;
        int invalidInput = 0;
        cout << "Enter new " << s << ": ";
        getline(cin, newInfo);
        if (newInfo == *info) {
            cout << "ERROR: Same " << s << "." << endl;
        options:
            cout << "1. Re-enter info\n2. Back\n" << endl;
            validateInput(2, PROMPT);
            switch (input) {
            case 1:
                update(info, s, r);
                break;
            case 2:
                getDetails(CLEAR_SCREEN);
                updateSelection();
                break;
            }
        }
        else if (regex_match(newInfo, r)) {
            *info = newInfo;
            updateStudentListCSV();
            system("CLS");
            cout << "SYSTEM: Info updated successfully\n" << endl;
            getDetails(RETAIN_SCREEN);
            updateSelection();
        }
        else {
            cout << "ERROR: Invalid input. Please check details before re-entering." << endl;
            if (++invalidInput == 3)
                cout << "If the error persists, please contact the Faculty General Office "
                << "to gain technical assistance." << endl;
            goto options;
        }
    };
    void updateSelection() {
        cout << "1. Update info\n2. Back\n" << endl;
        validateInput(2, PROMPT);
        switch (input) {
        case 1:
            cout << "Select info to be updated\n1. Contact number\n2. Email\n3. Address\n4. Back\n" << endl;
            validateInput(4, PROMPT);
            switch (input) {
            case 1:
                update(&contactNo, "contact number", regex("01(\\d){8,9}"));
                break;
            case 2:
                update(&email, "email", regex("[0-9a-zA-Z.-_]+@[0-9a-zA-Z.-_]+.com"));
                break;
            case 3:
                update(&address, "address", regex("[0-9a-zA-Z ,.'-/]+"));
                break;
            case 4:
                homePage();
                break;
            }
            break;
        case 2:
            homePage();
            break;
        }
    };
    void playTreasureGame() {       // I'm adding D&D vibes into this to make it slightly more interesting (only works on first playthrough though)
        string studentID = ID, studentName = name;
        int beginningTime, year, month, day, hour, min, sec, elapsedSeconds, score;
        chrono::steady_clock::time_point begin, end;
        int SEC = 1000, SHORT = 2000, NORMAL = 3000, LONG = 4500;
        string greetings[3] = { "\"Hey!", " Adventurer!\"", "\n\"Are you here to hunt down the legendary treasure of the mystery island?\"" };
        string bestWishes[4] = { "\"Right here, adventurer.", " You'll be teleported to the treasure island via this portal.\"", "\n\"Good luck, brave adventurer.", " You are really going to need it...\"" };
        string loading[3] = { ".", ".", "." };
        auto pause = [&](string message, int pauseInterval) {
            cout << message << flush;
            this_thread::sleep_for(chrono::milliseconds(pauseInterval));
        };
        auto stream = [&](string messages[], int amount, int pauseInterval, string s) {
            for (int i = 0; i < amount; i++) {
                pause(messages[i], pauseInterval);
                cout << s;
            }
        };
        auto gameEnd = [&](int SCORE) {
            end = chrono::steady_clock::now();
            elapsedSeconds = chrono::duration_cast<chrono::seconds>(end - begin).count();
            score = SCORE;
            gameRecord gr = gameRecord(studentID, studentName, beginningTime, year, month, day, hour, min, sec, elapsedSeconds, score);
            gr.finalize();
            gameHistory.push_back(gr);
            studentGameHistory.push_back(gr);
            updateTreasureGameCSV();
            string round = " round" + string((score > 1) * 1, 's');
            cout << "Your score this time is: " << score << round << " passed.\nPlay again next time!\n1. Back\n" << endl;
            validateInput(1, PROMPT);
            homePage();
        };
        auto roundThree = [&]() {
            string action;
            system("CLS");
            pause("\"I would expect nothing less of you!\"", NORMAL);
            pause("\n\"Here\"", SHORT);
            pause("\n*snap*", SEC);
            pause("\nPresented right in front of you,", SHORT);
            pause(" are 3 large metal doors:", SHORT);
            pause(" a red one,", SEC);
            pause(" a yellow one,", SEC);
            pause(" and a blue one.", SEC);
            pause("\n\"While the treasure you seek is RIGHT behind one of the closed doors,", LONG);
            pause("\ndo expect to be greeted by some bloodthirsthy beasts", NORMAL);
            pause(" who were left unfed for days if you open the wrong door,", LONG);
            pause("\nor to be engulfed and vaporized alive by relentless inferno.\"", LONG);
            cout << "\n\"";
            string laughter[6] = { "Heh", "heh", "heh", "ha", "ha", "ha" };
            stream(laughter, 6, SEC, " ");
            pause("ha\"", SEC);
            pause("\nWhat would your choice be, adventurer?", NORMAL);
            pause("\n\n1. Open the red door?", SHORT);
            pause("\n2. Open the yellow door?", SHORT);
            pause("\n3. Open the blue door?", SHORT);
            pause("\n4. Or", SEC);
            stream(loading, 3, SEC, "");
            pause(" Afraid of the possible consequences and choose to end it here?", NORMAL);
            cout << "\n\nYour action: ";
            getline(cin, action);
            while (!regex_match(action, regex("1|2|3|4|.*(touch|knock).*"))) {
                cout << "Invalid input. Please re-enter.\nYour action: ";
                getline(cin, action);
            }
            cout << endl;
            if (action == "1" || action == "3") {
                if (action == "1") {
                    pause("As soon as you opened the red door,", SHORT);
                    pause(" your body was entangled by swirling flames,", SHORT);
                    pause("\nand before you can even let out a squeak,", SHORT);
                    pause(" you body was burned to a crisp and flopped to the ground.", NORMAL);
                }
                else {
                    pause("As soon as you opened the blue door,", SHORT);
                    pause(" you were ambushed and pinned down\nby a canine-looking creature, with resemblance of a hyena.", LONG);
                    pause("\nIt growls and drools directly on your face,", NORMAL);
                    pause(" and before you let out of cry of utter trepidation,", NORMAL);
                    pause("\nyour neck was bitten off.", SHORT);
                    pause(" As your head rolled over,", SHORT);
                    pause(" you saw your limbs torn apart by its pack,", NORMAL);
                    pause("\nyour blood and intestines splattered all over the place,", NORMAL);
                    pause(" before getting cleaned up in a matter of seconds.", NORMAL);
                    pause("\nJust before you lose your last bit of consciousness, you heard...", NORMAL);
                }
                cout << "\n\"";
                for (int i = 0; i < 5; i++) {
                    cout << "HA" << flush;
                    this_thread::sleep_for(chrono::milliseconds(SEC));
                }
                pause(" Learn your lesson, kiddo.\"", SHORT);
                pause("\n\"Relying only to luck is a fool's idea of success.\"", NORMAL);
                pause("\nGAME OVER", NORMAL);
                cout << "\n" << endl;
                gameEnd(2);
            }
            else if (action == "2") {
                pause("As soon as you opened the yellow door,", SHORT);
                pause(" you were dazzled by the glittering radiance", NORMAL);
                pause("\nemitted by various treasures, and jewels", NORMAL);
                pause(" which are just sitting right there in the room!", NORMAL);
                pause("\n\"Congratulations, adventurer.", SHORT);
                pause(" You have discovered the legendary treasures.\"", NORMAL);
                pause("\nChallenge completed.", SHORT);
                cout << "\n" << endl;
                gameEnd(3);
            }
            else if (action == "4") {
                pause("*pfft*", SEC);
                pause(" \"HAHAHAHAHAHAHAHA\"", NORMAL);
                cout << "\nPlayer opted out of the game." << endl;
                gameEnd(2);
            }
            else {
                pause("You carefully approached the doors,", SHORT);
                pause(" cautiously touched and knocked them one by one.", NORMAL);
                pause("\nYou felt that the red metal door is unusually hotter than the other two,", LONG);
                pause("\nwhile also got responses like door slams and roars from the blue metal door.", LONG);
                pause("\nWith that, you confidently opened the yellow door,", NORMAL);
                pause(" and VOILA!", SHORT);
                pause("\nYou discovered the legendary treasures!", NORMAL);
                pause("\n*clap*", SEC);
                pause(" \"Well done.\"", SEC);
                pause(" *clap*", SEC);
                pause("\n*clap*", SEC);
                pause(" \"Well done! Adventurer!\"", SEC);
                pause(" *clap*", SEC);
                pause("\n\"A well-deserved winner of the treasures!\"", NORMAL);
                pause("\n\"You have displayed courage,", SHORT);
                pause(" patience,", SEC);
                pause(" and diligence", SEC);
                pause(" in your quest of the legendary treasures.\"", NORMAL);
                pause("\n\"Remember", SHORT);
                pause(" to always uphold virtues in your life,", NORMAL);
                pause(" even during trying times,", SHORT);
                pause("\nas it will certainly prove to be fruitful over time.\"", LONG);
                pause("\n\"May these virtues guide you to success in life.\"", NORMAL);
                pause("\nGame completed.", SHORT);
                cout << "\n" << endl;
                gameEnd(3);
            }
        };
        auto roundTwo = [&]() {
            system("CLS");
            pause("You looked around in search of a bridge or a raft to cross the river,", NORMAL);
            cout << " yet you spotted nothing of the sort in the horizon" << flush;
            stream(loading, 3, SEC, "");
            pause("\nYou started walking along the riverbank,", SHORT);
            pause(" hoping that you'll come across something useful in due course.", NORMAL);
            cout << "\nMinutes passed" << flush;
            stream(loading, 3, SEC, "");
            pause("\nLooking at the same sight incessantly while in motion", NORMAL);
            pause(" easily creates an illusion where you are not actually making progress.", NORMAL);
            pause("\nYou wiped away your sweat,", SHORT);
            pause(" shook off your self-doubt,", SHORT);
            pause(" and continued your quest for a river-crossing mechanism.", NORMAL);
            cout << "\nHours passed" << flush;
            stream(loading, 3, SEC, "");
            string paragraph_3[9] = { "\nYet the horizon still appears to be as empty as ever,", " as if you have never actually moved from the original spot...", "\nThe loss of sense of direction gradually builds up its toll,", " eventually crushing your hope to cross the river,", "\nas your tiredness and hunger were also reaching their limits.", "\nYou inferred that the Lost Woods is located on an isolated isle,", " and that you were just walking in circles...", "\nThere appears to be no direct way of crossing the river...", "\nWhat will be your next action, adventurer?" };
            stream(paragraph_3, 9, NORMAL, "");
            string decision_2[3] = { "\n\n1. Settle down and see what will happen", "\n2. Swim across the river", "\n3. Exit the game" };
            stream(decision_2, 3, SHORT, "");
            cout << "\n" << endl;
            validateInput(3, PROMPT);
            switch (input) {
            case 1:
                pause("You decided that it's important to preserve your energy and stamina.", NORMAL);
                pause("\nAnd so you sat down,", SEC);
                pause(" washed away your sweat,", SEC);
                pause(" and started to enjoy the breeze.", SEC);
                pause("\nYou collected some branches from the Lost Woods,", SHORT);
                pause(" set up a fire,", SEC);
                pause(" sharpened a few wooden branches,", SHORT);
                pause("\nand you were ready to go fishing!", SHORT);
                cout << endl;
                stream(loading, 3, SEC, " ");
                pause("\nYou caught yourself some trouts,", SHORT);
                pause(" and after having some delicious grilled fish,", NORMAL);
                pause("\nyou lay on the riverbank and fell asleep.", NORMAL);
                cout << endl;
                stream(loading, 3, SEC, "");
                pause("\nYou came to your senses,", SHORT);
                pause(" and were dazed by the evening sky,", SHORT);
                pause(" wondering how long have you been asleep.", NORMAL);
                pause("\nTo your surprise,", SHORT);
                pause(" you realized that the river has receded,", NORMAL);
                pause("\nunveiling a path that leads directly to the other side of the river!", LONG);
                pause("\nWithout hesitation,", SEC);
                pause(" you dashed towards the underwater path,", NORMAL);
                pause(" and sprinted through it before the water rises again.", NORMAL);
                pause("\nJust as you arrived at the other side,", NORMAL);
                pause(" panting in excitement,", SHORT);
                pause(" you hear the same clapping sound again.", NORMAL);
                pause("\n*clap*", SEC);
                pause("\nYou quickly adjust your stance and be on your guard.", NORMAL);
                pause("\n*clap*", SEC);
                pause("\n\"Not too shabby, adventurer.\"", SHORT);
                pause("\n*clap*", SEC);
                pause("\nIt's the same voice as just now.", SHORT);
                pause("\n\"Patience is a virtue that hardly comes by", SEC);
                stream(loading, 3, SEC, "");
                pause("\"\n\"I have been watching over you, adventurer.", NORMAL);
                pause(" Are you ready to take on the final trial?\"", SHORT);
                pause("\n\n1. Yes I am!", SEC);
                pause("\n2. ... I think I'm fine for now", SEC);
                cout << "\n" << endl;
                validateInput(2, PROMPT);
                switch (input) {
                case 1:
                    roundThree();
                    break;
                case 2:
                    cout << "Player opted out of the game." << endl;
                    gameEnd(2);
                    break;
                }
                break;
            case 2:
                pause("Without hesitating for too long,", SHORT);
                pause(" you took off your shirt and started swimming across the river.", NORMAL);
                pause("\nHowever...", SEC);
                pause("\nThe weather is hot, the river is wide,", SHORT);
                pause("\nand it wasn't long before you realize that your swimming speed is no match to how quick your stamina depletes.", LONG);
                pause("\n\"OUCH!!\"", SHORT);
                pause("\nYou felt an acute pain coming from your right foot,", NORMAL);
                pause(" and as you turned around to take a look at it,", SHORT);
                pause("\nyou saw a trout chewing and biting off a piece of meat from your foot.", NORMAL);
                pause("\nEven worse, the scent of your blood attracted more trouts and more of them were swimming swiftly to you.", LONG);
                pause("\nYou eagerly grasped for air,", SHORT);
                pause(" flapped your limbs in terror,", SHORT);
                pause("\nhoping to escape your impending death by reaching the other side of the river.", NORMAL);
                pause("\nBut how can an untrained person outswim a sea creature?", NORMAL);
                pause("\nAs the trouts caught up to you,", SHORT);
                pause(" you were eaten alive, and experienced one of the most painful and gruesome deaths", NORMAL);
                stream(loading, 3, SEC, "");
                pause("\nGAME OVER", NORMAL);
                cout << "\n" << endl;
                gameEnd(1);
                break;
            case 3:
                cout << "Player opted out of the game." << endl;
                gameEnd(1);
                break;
            }
        };
        auto roundOne = [&]() {
            beginningTime = time(0);
            year = now.tm_year + 1900, month = now.tm_mon, day = now.tm_mday, hour = now.tm_hour, min = now.tm_min, sec = now.tm_sec;
            begin = chrono::steady_clock::now();
            system("CLS");
            stream(loading, 3, SEC, " ");
            pause("\nAdventurer...", SEC);
            pause("\nYou have finally come to your senses...", SHORT);
            string paragraph_1[7] = { "\nYou find yourself amidst a lush verdant rainforest.", "\nAs you pat away the dirts and mud from your body,", " you look around to get a better insight of your whereabouts.", "\nHowever, the dense fog in the surroundings obtruct your view heavily,", "\nand all you can really discern is the path underneath your feet.", "\nAfter trying to get a sight of what lies ahead but to no avail,", "\nyou decided to tread deeper into the woods, as there's no other obvious choice." };
            stream(paragraph_1, 7, NORMAL, "");
            cout << "\n";
            stream(loading, 3, SEC, " ");
            string paragraph_2[4] = { "\nNot knowing how long has the time passed, you arrived at a three-way junction.", "\nFrom a broken, dilapidated sign post,", " it appears that one way might bring you out of the woods,", "\nwhile the other leads to a hideaway of some unknown, seemingly-ferocious creatures." };
            stream(paragraph_2, 4, NORMAL, "");
            string decision_1[4] = { "\nMake your move...", " Adventurer...", "\nTo the left?", " Or to the right?" };
            stream(decision_1, 4, SHORT, "");
            pause("\n\n1. To the left, forward!", SEC);
            pause("\n2. To the right, charge!", SEC);
            pause("\n3. Turn back and exit the game", SEC);
            cout << "\n" << endl;
            validateInput(3, PROMPT);
            string clap[5] = { "\n*clap*", "Slow,", "*clap*", "but steady.", "*clap*" };
            switch (input) {
            case 1:
                pause("You took the left path and ventured into the murky woods", SEC);
                stream(loading, 3, SEC, "");
                pause("\nSoon enough, you traversed through the forest and found yourself in front of a river.", LONG);
                pause("\nOut of nowhere, you started to hear several claps.", SHORT);
                stream(clap, 5, SEC, "\n");
                pause("Confused and startled, you looked around warily to locate the source of the sound.", LONG);
                pause("\nA voice spoke from the sky:", SHORT);
                pause(" \"Bravo, adventurer!\"", SHORT);
                pause("\n\"Props to you for getting through the Lost Woods,", NORMAL);
                pause(" your bravery is commendable.\"", SHORT);
                pause("\n\"However the challenge has just begun!\"", SHORT);
                pause("\n\"Would you like to continue to take up the challenge?\"", NORMAL);
                pause("\n\n1. Continue", SEC);
                pause("\n2. Leave", SEC);
                cout << "\n" << endl;
                validateInput(2, PROMPT);
                switch (input) {
                case 1:
                    roundTwo();
                    break;
                case 2:
                    cout << "Player opted out of the game." << endl;
                    gameEnd(1);
                    break;
                }
                break;
            case 2:
                cout << "You took the right path and venture into the murky woods" << flush;
                this_thread::sleep_for(chrono::milliseconds(SEC));
                stream(loading, 3, SEC, "");
                pause("\n\nYou fell into a pitfall trap designed to trap the ferocious beasts.", NORMAL);
                pause("\nGAME OVER", NORMAL);        // This is super anticlimactic but I'm running out of time XD
                cout << "\n" << endl;
                gameEnd(0);
                break;
            case 3:
                cout << "Player opted out of the game." << endl;
                gameEnd(0);
                break;
            }
        };
        cout << "\nSelect text speed:\n1. Slow\n2. Normal\n3. Immediate\n" << endl;
        validateInput(3, PROMPT);
        if (input == 2)
            SHORT = 1500, NORMAL = 2000, LONG = 3500;
        else if (input == 3)
            SEC = 0, SHORT = 0, NORMAL = 0, LONG = 0;
        system("CLS");
        stream(greetings, 3, SEC, "");
        cout << "\n1. Yes\n2. No\n" << endl;
        validateInput(2, PROMPT);
        switch (input) {
        case 1:
            stream(bestWishes, 4, SHORT, "");
            cout << "\nHeh heh heh..." << endl;
            roundOne();
            break;
        case 2:
            cout << "No worries, feel free to join the hunt next time! ^_^\n1. Back\n" << endl;;
            validateInput(1, PROMPT);
            homePage();
            break;
        }
    };
    void viewGameHistory() {
        auto digit = [](unsigned int i) {
            int numberOfDigits = 0;         // returns the number of digits of a given integer
            while (i) {                     // value will be added into maxIndexWidth
                i /= 10;
                numberOfDigits++;
            }return numberOfDigits;
        };
        auto format = [](unsigned int num) {
            string input = ts(num), output = "";
            int i = input.size(), count = 0;
            while (i) {
                output = input.at(--i) + output;
                if ((++count == 3) && i) {
                    output = "." + output;
                    count = 0;
                }
            }return output;
        };
        int currentPage = 1, displayAmount = 20;    // alterable to suit user's preference
        int maxPage = (gameHistory.size() - 1) / displayAmount + 1;
        auto print = [&](vector<gameRecord> v, unsigned int currentPage, unsigned int displayAmount) {
            int maxElapsedTimeWidth = 12;
            int maxPage = (v.size() - 1) / displayAmount + 1;
            int indexInCurrentPage = currentPage * displayAmount;
            int firstDisplay = (currentPage - 1) * displayAmount;
            int pageLimit = (indexInCurrentPage < v.size()) * indexInCurrentPage + (indexInCurrentPage >= v.size()) * v.size();
            int maxIndexWidth = digit(pageLimit) + 2;   // +2 for the 2 characters ". " after the index
            for (int i = firstDisplay; i < pageLimit; i++) {
                gameRecord* gr = &v[i];
                maxElapsedTimeWidth += (gr->elapsedTime.length() > maxElapsedTimeWidth) * (gr->elapsedTime.length() - maxElapsedTimeWidth);
            }
            string name = "Name", date = "Date", time = "Time", elapsedTime = "Elapsed Time";
            string page = "page " + ts(currentPage) + " of " + ts(maxPage);
            int tableWidth = 41 + maxElapsedTimeWidth;
            system("CLS");
            cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
            cout << string(maxIndexWidth, ' ') << RIGHT(10, date, POSITION_CENTER) << RIGHT(13, time, POSITION_CENTER) << RIGHT(maxElapsedTimeWidth, elapsedTime, POSITION_CENTER) << "| SCORE |" << endl;
            cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
            for (int i = firstDisplay; i < pageLimit; i++) {
                gameRecord* gr = &v[i];
                cout << setw(maxIndexWidth) << ts(i + 1).append(". ") << RIGHT(10, gr->date, POSITION_CENTER)
                    << RIGHT(13, gr->time, POSITION_CENTER) << RIGHT(maxElapsedTimeWidth, gr->elapsedTime, POSITION_CENTER)
                    << RIGHT(5, ts(gr->score), POSITION_CENTER) << "|" << endl;
            }
            cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
            string first = format(firstDisplay + 1), last = format(pageLimit), size = format(v.size());
            string entry = v.size() == 1 ? " entry" : " entries";
            cout << string(maxIndexWidth, ' ') << "Current display: " << first << "-" << last << " of " << size << entry  // 22 constant characters
                << setw(tableWidth - 22 - first.length() - last.length() - size.length() - entry.length()) << right << page << endl;
            cout << endl;
        };
        auto filter = [&]() {
            currentTime(HIDE_TIME);
            vector<gameRecord> temp;
            string year, month, day, date;
        requestDate:
            temp.clear();
            cout << "Enter year: ";
            validateInput(now.tm_year + 1900, HIDE_PROMPT);
            year = zerosPadded(input, 4);
            cout << "Enter month: ";
            validateInput(12, HIDE_PROMPT);
            month = zerosPadded(input, 2);
            cout << "Enter day: ";
            validateInput(31, HIDE_PROMPT);
            day = zerosPadded(input, 2);
            date = day + "/" + month + "/" + year;
            for (int i = 0; i < gameHistory.size(); i++) {
                gameRecord* gr = &gameHistory[i];
                if (date == gr->date)
                    temp.push_back(*gr);
            }
            if (temp.empty()) {
                cout << "No record found on " << date << endl;
                cout << "1. Search again\n2. Back\n" << endl;
                validateInput(2, PROMPT);
                switch (input) {
                case 1:
                    goto requestDate;
                    break;
                case 2:
                    viewGameHistory();
                    break;
                }
            }
            else {
                print(temp, 1, temp.size());
                cout << "1. Back\n" << endl;
                validateInput(1, PROMPT);
                viewGameHistory();
            }
        };
        auto jumpToPage = [&]() {
            cout << "Jump to page: ";
            validateInput(maxPage, HIDE_PROMPT);
            currentPage = input;
            print(gameHistory, currentPage, displayAmount);
        };
        auto setEntriesPerPage = [&]() {
            cout << "Select the number of entries per page\n1. 10 lines\n2. 20 lines (default)\n3. 50 lines" << endl;
            validateInput(3, PROMPT);
            currentPage = 1;
            displayAmount = input * 10 + (input == 3) * 20;
            print(gameHistory, currentPage, displayAmount);
        };
        auto runOptions = [&]() {
        provideOptions:
            int option = 0;
            cout << ++option << ". Filter game history based on date" << endl;
            cout << ++option << ". Clear game history" << endl;
            if (maxPage > 1) {
                if (currentPage < maxPage)
                    cout << ++option << ". Next page" << endl;
                if (currentPage > 1)
                    cout << ++option << ". Previous page" << endl;
                cout << ++option << ". Jump to specific page" << endl;
            }
            cout << ++option << ". Adjust entries per page" << endl;
            cout << ++option << ". Sort game history" << endl;
            cout << ++option << ". Back\n" << endl;
            validateInput(option, PROMPT);
            switch (option) {
            case 5:
                switch (input) {
                case 1:
                    filter();
                    break;
                case 2:
                    clearGameHistory();
                    break;
                case 3:
                    setEntriesPerPage();
                    goto provideOptions;
                    break;
                case 4:
                    sortGameHistory();
                    break;
                case 5:
                    homePage();
                    break;
                }
                break;
            case 7:
                switch (input) {
                case 1:
                    filter();
                    break;
                case 2:
                    clearGameHistory();
                    break;
                case 3:
                    if (currentPage > 1)
                        currentPage--;
                    else
                        currentPage++;
                    print(gameHistory, currentPage, displayAmount);
                    goto provideOptions;
                    break;
                case 4:
                    jumpToPage();
                    goto provideOptions;
                    break;
                case 5:
                    setEntriesPerPage();
                    goto provideOptions;
                    break;
                case 6:
                    sortGameHistory();
                    break;
                case 7:
                    homePage();
                    break;
                }
                break;
            case 8:
                switch (input) {
                case 1:
                    filter();
                    break;
                case 2:
                    clearGameHistory();
                    break;
                case 3:
                    currentPage++;
                    print(gameHistory, currentPage, displayAmount);
                    goto provideOptions;
                    break;
                case 4:
                    currentPage--;
                    print(gameHistory, currentPage, displayAmount);
                    goto provideOptions;
                    break;
                case 5:
                    jumpToPage();
                    goto provideOptions;
                    break;
                case 6:
                    setEntriesPerPage();
                    goto provideOptions;
                    break;
                case 7:
                    sortGameHistory();
                    break;
                case 8:
                    homePage();
                    break;
                }
                break;
            }
        };
        print(gameHistory, currentPage, displayAmount);
        runOptions();
    };
    void sortGameHistory() {
        cout << "Select the sorting order\n1. Ascending date/time (default)\n2. Descending date/time\n"
            << "3. Ascending score\n4. Descending score\n5. Back\n" << endl;
        validateInput(5, PROMPT);
        switch (input) {
        case 1:
            sort(gameHistory.begin(), gameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.beginningTime < b.beginningTime; });
            break;
        case 2:
            sort(gameHistory.begin(), gameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.beginningTime > b.beginningTime; });
            break;
        case 3:
            sort(gameHistory.begin(), gameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.score < b.score; });
            break;
        case 4:
            sort(gameHistory.begin(), gameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.score > b.score; });
            break;
        }
        viewGameHistory();
    };
    void clearGameHistory() {
        cout << "Are you sure you want to clear your game history? (Action cannot be undone)\n1. Yes I'm certain\n2. Back\n" << endl;
        validateInput(2, PROMPT);
        vector<gameRecord>::iterator itr;
        switch (input) {
        case 1:
            gameHistory.clear();
            itr = remove_if(studentGameHistory.begin(), studentGameHistory.end(), [&](gameRecord& gr) {return gr.studentID == ID; });
            updateTreasureGameCSV();
            system("CLS");
            cout << "SYSTEM: Data of game history cleared.\n1. Back\n" << endl;
            validateInput(1, PROMPT);
            homePage();
            break;
        case 2:
            viewGameHistory();
            break;
        }
    }
};
vector<student> studentList;
struct selectedStudent {
    student* ptr;
    int position;
};
struct selectedStudent selectedStudent;
void studentLogin() {
    string username, password;
    int invalidLoginCount = 0;
    auto requestCredentials = [&]() {
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);
    };
    auto loginAuth = [&]() {
        for (int i = 0; i < studentList.size(); i++) {
            if (studentList[i].name == username && studentList[i].icNo == password) {
                selectedStudent.ptr = &studentList[i];
                return true;
            }
        }return false;
    };
    requestCredentials();
    while (!loginAuth()) {
        invalidLoginCount++;
        cout << "Invalid login. Please check username or password.";
        if (invalidLoginCount >= 3)
            cout << " [Hint:Full name(case-sensitive) and IC Number]";
        cout << endl;
        requestCredentials();
    }selectedStudent.ptr->homePage();
}
void updateStudentListCSV() {
    ofstream newCSV("new.csv", ios::out | ios::app);
    for (int i = 0; i < studentList.size(); i++) {
        student* s = &studentList[i];
        newCSV << s->entryYear << "|" << s->batchNum << "|" << s->ID << "|" << s->name << "|"
            << s->programme << "|" << (int)s->status << "|" << s->trimester << "|" << s->icNo << "|"
            << s->gender << "|" << s->contactNo << "|" << s->email << "|" << s->address;
        if (i < studentList.size() - 1)
            newCSV << "\n";
    }
    newCSV.close();
    remove("Student List.csv");
    rename("new.csv", "Student List.csv");
};

class admin {
public:             // I think that admin needs to register these info as well that's why I declared these variables
    string ID, name, icNo, gender, email, contactNo, address;
    admin() {};     // Though not used in this assignment, I aim to make it as realistic as possible, so I just include them here
    admin(string a, string b, string c, string d) {
        ID = a;
        name = b;       // username
        icNo = c;       // password
        gender = d;
    };

    void loginPage() {
        system("CLS");
        currentTime(DISPLAY_TIME);
        cout << "Welcome " << name << ", what would you like to do today?\n" << endl;
        cout << "1. View and update students' information\n2. View and update students' grades\n"
            << "3. Insert new student(s) into student list\n4. View students' game history\n5. Quit\n" << endl;
        // remember to implement [1] search function based on date/student info [1] in "view game history"
        validateInput(5, PROMPT);
        switch (input) {
        case 1:
            printStudentList();
            viewUpdateDelete(CLEAR_SCREEN);
            break;
        case 2:
            printStudentList();
            updateStudentGrades(CLEAR_SCREEN);
            break;
        case 3:
            insertStudent();
            break;
        case 4:
            viewStudentGameHistory();
            break;
        case 5:
            cout << "Program terminated." << endl;
            exit(0);
        }
    };
    void selectStudent() {
        if (studentList.size() == 1) {
            selectedStudent.position = 0;
            selectedStudent.ptr = &studentList[0];
            return;
        }
        cout << "Enter the index of the student you wish to inspect: ";
        validateInput(studentList.size(), HIDE_PROMPT);
        selectedStudent.position = input - 1;
        selectedStudent.ptr = &studentList[input - 1];
    };
    void nullStudentPointerWarning() {
        if (!selectedStudent.ptr) {
            cout << "ERROR: NULL student pointer.\n1. Back\n" << endl;
            validateInput(1, PROMPT);
            loginPage();
        }
    };
    void printStudentList() {
        if (studentList.empty()) {
            system("CLS");
            cout << "ERROR: Empty student list.\n1. Back" << endl;
            validateInput(1, PROMPT);
            loginPage();
            return;
        }
        auto digit = [](unsigned int i) {
            int numberOfDigits = 0;         // returns the number of digits of a given integer
            while (i) {                     // value will be added into maxIndexWidth
                i /= 10;
                numberOfDigits++;
            }return numberOfDigits;
        };
        auto format = [](unsigned int num) {
            string input = ts(num), output = "";
            int i = input.size(), count = 0;
            while (i) {
                output = input.at(--i) + output;
                if ((++count == 3) && i) {
                    output = "." + output;
                    count = 0;
                }
            }return output;
        };
        int currentPage = 1, displayAmount = 20;    // alterable to suit user's preference
    startPrinting:
        int maxPage = (studentList.size() - 1) / displayAmount + 1;
        int maxIDWidth = 0, maxNameWidth = 0, maxStatusWidth = 0;
        int indexInCurrentPage = currentPage * displayAmount;
        int firstDisplay = (currentPage - 1) * displayAmount;
        int pageLimit = (indexInCurrentPage < studentList.size()) * indexInCurrentPage + (indexInCurrentPage >= studentList.size()) * studentList.size();
        int maxIndexWidth = digit(pageLimit) + 2;   // +2 for the 2 characters ". " after the index
        for (int i = firstDisplay; i < pageLimit; i++) {
            student* s = &studentList[i];
            maxIDWidth += (s->ID.length() > maxIDWidth) * (s->ID.length() - maxIDWidth);
            maxNameWidth += (s->name.length() > maxNameWidth) * (s->name.length() - maxNameWidth);
            int statusLength = statusList.find(s->status)->second.length();
            maxStatusWidth += (statusLength > maxStatusWidth) * (statusLength - maxStatusWidth);
        }   // compute and initialize variables that affect table's column width
        string name = "Name", status = "Status", page = "page " + ts(currentPage) + " of " + ts(maxPage);
        int tableWidth = maxNameWidth + maxStatusWidth + 33;
        system("CLS");
        cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
        cout << string(maxIndexWidth, ' ') << "|  Student ID " << RIGHT(maxNameWidth, name, POSITION_CENTER)
            << "| Trimester " << RIGHT(maxStatusWidth, status, POSITION_CENTER) << "|" << endl;
        cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
        for (int i = firstDisplay; i < pageLimit; i++) {
            student* s = &studentList[i];
            cout << setw(maxIndexWidth) << ts(i + 1).append(". ") << RIGHT(maxIDWidth, s->ID, POSITION_CENTER)
                << LEFT(maxNameWidth, s->name, POSITION_DEFAULT) << RIGHT(9, ts(s->trimester), POSITION_CENTER)
                << RIGHT(maxStatusWidth, statusList.find(s->status)->second, POSITION_CENTER) << "|" << endl;
        }
        cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
        string first = format(firstDisplay + 1), last = format(pageLimit), size = format(studentList.size());
        string student = " student" + string(studentList.size() > 1, 's');
        cout << string(maxIndexWidth, ' ') << "Current display: " << first << "-" << last << " of " << size << student  // 22 constant characters
            << setw(tableWidth - 22 - first.length() - last.length() - size.length() - student.length()) << right << page << endl;
        cout << endl;
        // provide user options
        auto search = [&]() {
        startOfSearch:
            cout << "1. Search student by ID\n2. Search student by name\n3. Back\n" << endl;
            validateInput(3, PROMPT);
            string identifier;
            switch (input) {
            case 1:
            searchByID:
                cout << "Enter student ID: ";
                getline(cin, identifier);
                for (int i = 0; i < studentList.size(); i++) {
                    if (identifier == studentList[i].ID) {
                        selectedStudent.position = i;
                        selectedStudent.ptr = &studentList[i];
                        return;
                    }
                }cout << "ERROR: No matching student ID found. Please examine spelling and capitalization." << endl;
                cout << "1.Re-enter student ID\n2.Back\n" << endl;
                validateInput(2, PROMPT);
                switch (input) {
                case 1:
                    goto searchByID;
                    break;
                case 2:
                    goto startOfSearch;
                    break;
                }
                break;
            case 2:
            searchByName:
                cout << "Enter student name: ";
                getline(cin, identifier);
                for (int i = 0; i < studentList.size(); i++) {
                    if (identifier == studentList[i].name) {
                        selectedStudent.position = i;
                        selectedStudent.ptr = &studentList[i];
                        return;
                    }
                }cout << "ERROR: No matching student name found. Please examine spelling and capitalization." << endl;
                cout << "1.Re-enter student name\n2.Back\n" << endl;
                validateInput(2, PROMPT);
                switch (input) {
                case 1:
                    goto searchByName;
                    break;
                case 2:
                    goto startOfSearch;
                    break;
                }
                break;
            case 3:
                printStudentList();
                break;
            }
        };
        auto jumpToPage = [&]() {
            cout << "Jump to page: ";
            validateInput(maxPage, HIDE_PROMPT);
            currentPage = input;
        };
        auto setEntriesPerPage = [&]() {
            cout << "Select the number of entries per page\n1. 10 lines\n2. 20 lines (default)\n3. 50 lines" << endl;
            validateInput(3, PROMPT);
            currentPage = 1;
            displayAmount = input * 10 + (input == 3) * 20;
        };
        int option = 0;
        cout << ++option << ". Select student" << endl;
        cout << ++option << ". Search student" << endl;
        if (maxPage > 1) {
            if (currentPage < maxPage)
                cout << ++option << ". Next page" << endl;
            if (currentPage > 1)
                cout << ++option << ". Previous page" << endl;
            cout << ++option << ". Jump to specific page" << endl;
        }
        cout << ++option << ". Adjust entries per page" << endl;
        cout << ++option << ". Sort student list" << endl;
        cout << ++option << ". Back\n" << endl;
        validateInput(option, PROMPT);
        switch (option) {
        case 5:
            switch (input) {
            case 1:
                selectStudent();
                break;
            case 2:
                search();
                break;
            case 3:
                setEntriesPerPage();
                goto startPrinting;
                break;
            case 4:
                sortStudentList();
                break;
            case 5:
                loginPage();
                break;
            }
            break;
        case 7:
            switch (input) {
            case 1:
                selectStudent();
                break;
            case 2:
                search();
                break;
            case 3:
                if (currentPage > 1)
                    currentPage--;
                else
                    currentPage++;
                goto startPrinting;
                break;
            case 4:
                jumpToPage();
                goto startPrinting;
                break;
            case 5:
                setEntriesPerPage();
                goto startPrinting;
                break;
            case 6:
                sortStudentList();
                break;
            case 7:
                loginPage();
                break;
            }
            break;
        case 8:
            switch (input) {
            case 1:
                selectStudent();
                break;
            case 2:
                search();
                break;
            case 3:
                currentPage++;
                goto startPrinting;
                break;
            case 4:
                currentPage--;
                goto startPrinting;
                break;
            case 5:
                jumpToPage();
                goto startPrinting;
                break;
            case 6:
                setEntriesPerPage();
                goto startPrinting;
                break;
            case 7:
                sortStudentList();
                break;
            case 8:
                loginPage();
                break;
            }
            break;
        }
    };
    void sortStudentList() {
        cout << "Select the sorting order\n1. Ascending student ID (default)\n"
            << "2. Descending student ID\n3. Ascending student name\n4. Descending student name\n"
            << "5. Ascending student status\n6. Descending student status\n7. Back\n" << endl;
        validateInput(7, PROMPT);
        switch (input) {
        case 1:
            sort(studentList.begin(), studentList.end(), [](student& a, student& b) { return a.ID < b.ID; });
            updateStudentListCSV();
            break;
        case 2:
            sort(studentList.begin(), studentList.end(), [](student& a, student& b) { return a.ID > b.ID; });
            updateStudentListCSV();
            break;
        case 3:
            sort(studentList.begin(), studentList.end(), [](student& a, student& b) { return a.name < b.name; });
            updateStudentListCSV();
            break;
        case 4:
            sort(studentList.begin(), studentList.end(), [](student& a, student& b) { return a.name > b.name; });
            updateStudentListCSV();
            break;
        case 5:
            sort(studentList.begin(), studentList.end(), [](student& a, student& b) { return a.status < b.status; });
            updateStudentListCSV();
            break;
        case 6:
            sort(studentList.begin(), studentList.end(), [](student& a, student& b) { return a.status > b.status; });
            updateStudentListCSV();
            break;
        }
        printStudentList();
    };
    void viewUpdateDelete(basicCommand clearScreen) {
        nullStudentPointerWarning();
        selectedStudent.ptr->getDetails(clearScreen);
        cout << "1. Update student's information\n2. Update student's status\n"
            << "3. Delete student's record\n4. Back\n" << endl;
        validateInput(4, PROMPT);
        switch (input) {
        case 1:
            updateSelection();
            break;
        case 2:
            updateStudentStatus();
            break;
        case 3:
            deleteStudent();
            break;
        case 4:
            loginPage();
            break;
        }
    };
    void updateSelection() {
        student* s = selectedStudent.ptr;
        cout << "Select info to be updated\n1. Name\n2. IC Number\n3. Gender\n4. Contact Number\n"
            << "5. Email\n6. Address\n7. Back\n" << endl;
        validateInput(7, PROMPT);
        switch (input) {
        case 1:
            updateStudentInfo(&s->name, "student name", regex("[a-zA-Z .'-/]{4,}"));
            break;
        case 2:
            updateStudentInfo(&s->icNo, "student IC number", regex("((\\d{2}(((0[13578]|1[02])(0[1-9]|[12]\\d|3[0-1]))|((0[469]|11)(0[1-9]|[12]\\d|30))|(02(0[1-9]|1\\d|2[0-8]))))|(([02468][048]|[13579][26])0229))(0[1-9]|1[0-6]|2[1-9]|[3-5]\\d)\\d{4}"));
            break;
        case 3:
            updateStudentInfo(&s->gender, "student gender", regex("Male|Female"));
            break;
        case 4:
            updateStudentInfo(&s->contactNo, "student contact number", regex("01(\\d){8,9}"));
            break;
        case 5:
            updateStudentInfo(&s->email, "student email", regex("[0-9a-zA-Z.-_]+@[0-9a-zA-Z.-_]+.com"));
            break;
        case 6:
            updateStudentInfo(&s->address, "student address", regex("[0-9a-zA-Z ,.'-/]+"));
            break;
        case 7:
            viewUpdateDelete(CLEAR_SCREEN);
            break;
        }
    };
    void updateStudentInfo(string* info, string s, regex r) {
        string newInfo;
        int invalidInput = 0;
        cout << "Enter new " << s << ": ";
        getline(cin, newInfo);
        if (newInfo == *info) {
            cout << "ERROR: Same " << s << "." << endl;
        options:
            cout << "1. Re-enter info\n2. Back\n" << endl;
            validateInput(2, PROMPT);
            switch (input) {
            case 1:
                updateStudentInfo(info, s, r);
                break;
            case 2:
                selectedStudent.ptr->getDetails(CLEAR_SCREEN);
                updateSelection();
                break;
            }
        }
        else if (regex_match(newInfo, r)) {
            *info = newInfo;
            updateStudentListCSV();
            system("CLS");
            cout << "SYSTEM: Info updated successfully\n" << endl;
            selectedStudent.ptr->getDetails(RETAIN_SCREEN);
            updateSelection();
        }
        else {
            cout << "ERROR: Invalid input. Please check details before re-entering." << endl;
            if (++invalidInput == 3)
                cout << "Please contact technical support if the error persists." << endl;
            goto options;
        }
    };
    void updateStudentStatus() {
        STATUS* stat = &selectedStudent.ptr->status;
        cout << "Select student's current status:" << endl;
        map<enum STATUS, string>::iterator itr = statusList.begin();
        for (int i = 1; itr != statusList.end(); i++, itr++) {
            cout << i << ". " << itr->second << endl;
        }cout << statusList.size() + 1 << ". Back\n" << endl;
        validateInput(statusList.size() + 1, PROMPT);
        if (input == statusList.size() + 1)
            viewUpdateDelete(CLEAR_SCREEN);
        else if (input - 1 == (int)*stat) {
            cout << "ERROR: Same student status." << endl;
            cout << "1. Re-select\n2. Back\n" << endl;
            validateInput(2, PROMPT);
            switch (input) {
            case 1:
                updateStudentStatus();
                break;
            case 2:
                viewUpdateDelete(CLEAR_SCREEN);
                break;
            }
        }
        else {
            *stat = STATUS(input - 1);
            updateStudentListCSV();
            system("CLS");
            cout << "SYSTEM: Info updated successfully\n" << endl;
            viewUpdateDelete(RETAIN_SCREEN);
        }
    };
    void updateStudentGrades(basicCommand clearScreen) {
        nullStudentPointerWarning();
        student* s = selectedStudent.ptr;
        s->getDetails(clearScreen);
        auto completeProgramme = [&](student* s) {
            for (int i = 0; i < s->defaultArrangement.size(); i++) {
                for (int j = 0; j < s->defaultArrangement[i].size(); j++)
                    if (s->defaultArrangement[i][j].marks < 40)
                        return false;
            }return true;
        };
        auto updateSubject = [&]() {
            string marks;
            int m;
            system("CLS");
        start:
            s->printf("Student ID", s->ID, 18);
            s->printf("Name", s->name, 18);
            s->printf("Programme", s->programme, 18);
            s->printf("Current trimester", ts(s->trimester), 18);
            cout << endl;
            cout << "Updating results for trimester " << s->trimester << "..." << endl;
            print(s->defaultArrangement[s->trimester - 1], DISPLAY_GRADES, DISPLAY_INDEX);
            cout << "\nSelect subject to be updated, or" << endl;
            cout << s->defaultArrangement[s->trimester - 1].size() + 1 << ". Back\n" << endl;
            validateInput(s->defaultArrangement[s->trimester - 1].size() + 1, PROMPT);
            if (input == s->defaultArrangement[s->trimester - 1].size() + 1)
                updateStudentGrades(CLEAR_SCREEN);
            cout << "Enter marks: ";
            getline(cin, marks);
            while (!regex_match(marks, regex("\\d|[1-9]\\d|100"))) {
                cout << "Invalid marks. Please enter again: ";
                getline(cin, marks);
            }
            m = stoi(marks);
            s->updateResults(&s->defaultArrangement[s->trimester - 1][input - 1], m);
            system("CLS");
            if (completeProgramme(s)) {
                s->status = GRADUATED;
                updateStudentListCSV();
            }
            else if (s->status == GRADUATED)
                cout << "WARNING: Credits not fully earned. Revoke student's graduated status immediately." << endl;
            cout << "SYSTEM: Subject grades updated.\n" << endl;
            goto start;
        };
        int option = 0;
        if (s->trimester > 1)
            cout << ++option << ". View student's grades" << endl;
        cout << ++option << ". Update student's grades" << endl;
        cout << ++option << ". Back\n" << endl;
        validateInput(option, PROMPT);
        switch (option) {
        case 2:
            switch (input) {
            case 1:
                updateSubject();
                break;
            case 2:
                loginPage();
                break;
            }
            break;
        case 3:
            switch (input) {
            case 1:
                s->getResults();
                updateStudentGrades(CLEAR_SCREEN);
                break;
            case 2:
                updateSubject();
                break;
            case 3:
                loginPage();
                break;
            }
            break;
        }
    }
    void deleteStudent() {
        nullStudentPointerWarning();
        selectedStudent.ptr->getDetails(CLEAR_SCREEN);
        cout << "WARNING: All records associated with this student will be removed from the system. Are you sure you want to delete?\n1. Delete\n2. Back\n" << endl;
        validateInput(2, PROMPT);
        if (input == 2) {
            viewUpdateDelete(CLEAR_SCREEN);
            return;
        }
        string choice;
        cout << "Confirm delete?\n1. Back\n13781. DELETE\n" << endl;
        cout << "Enter choice: ";
        getline(cin, choice);
        while (!regex_match(choice, regex("1|13781"))) {
            cout << "Invalid input. Enter choice: ";
            getline(cin, choice);
        }
        input = stoi(choice);
        switch (input) {
        case 1:
            viewUpdateDelete(CLEAR_SCREEN);
            break;
        case 13781:
            studentList.erase(studentList.begin() + selectedStudent.position);
            updateStudentListCSV();
            selectedStudent.position = NULL;
            selectedStudent.ptr = NULL;
            cout << "\nSYSTEM: Student data deleted\n1. Back\n" << endl;
            validateInput(1, PROMPT);
            loginPage();
            break;
        }
    }
    void insertStudent() {
        system("CLS");              // if time allows I would really love to write one for auto input(this) and one more for manual input
        student s;                  // that allows manual input of entry year, batch num, student ID
        string name = "<name_placeholder>", icNo = "<icNo_placeholder>", gender = "<gender_placeholder>";
        string contactNo = "<contactNo_placeholder>", email = "<email_placeholder>", address = "<address_placeholder>";
        cout << "Reminder: Enter \"esc\" to quit, or \"skip\" to skip a particular info\n" << endl;
        auto record = [&](string* attribute, string attributeName, regex r) {
            string adminInput;
            int invalidInput = 0;
        start:
            cout << "Enter new student's " << attributeName << ": ";
            getline(cin, adminInput);
            if (regex_match(adminInput, regex("esc|Esc|ESC")))
                loginPage();
            else if (regex_match(adminInput, regex("skip|Skip|SKIP")))
                return;
            else if (regex_match(adminInput, r)) {
                *attribute = adminInput;
                cout << endl;
            }
            else {
                cout << "ERROR: Invalid input. Please check student details before re-entering." << endl;
                if (++invalidInput == 3)
                    cout << "You may enter \"skip\" for now but please contact technical support if the error persists." << endl;
                goto start;
            }
        };
        auto printf = [&](string attributeName, string attribute) {
            cout << setw(15) << left << attributeName << ": " << attribute << endl;
        };
        record(&name, "name", regex("[a-zA-Z .'-/]{4,}"));
        record(&icNo, "IC number", regex("((\\d{2}(((0[13578]|1[02])(0[1-9]|[12]\\d|3[0-1]))|((0[469]|11)(0[1-9]|[12]\\d|30))|(02(0[1-9]|1\\d|2[0-8]))))|(([02468][048]|[13579][26])0229))(0[1-9]|1[0-6]|2[1-9]|[3-5]\\d)\\d{4}"));
        record(&gender, "gender", regex("m|M|male|Male|MALE|f|F|female|Female|FEMALE"));
        if (tolower(gender.at(0)) == 'm')
            gender = "Male";
        else if (tolower(gender.at(0)) == 'f')
            gender = "Female";
        record(&contactNo, "contact number", regex("01(\\d){8,9}"));
        record(&email, "email", regex("[0-9a-zA-Z.-_]+@[0-9a-zA-Z.-_]+.com"));
        record(&address, "address", regex("[0-9a-zA-Z ,.'-/]+"));
    confirmation:
        system("CLS");
        cout << "New student info:\n" << endl;
        printf("Name", name);
        printf("IC Number", icNo);
        printf("Gender", gender);
        printf("Contact Number", contactNo);
        printf("Email", email);
        printf("Address", address);
        cout << "\n1. Amend particular info\n2. Add student into student list\n3. Back\n" << endl;
        validateInput(3, PROMPT);
        switch (input) {
        case 1:
            cout << "Select info to be amended\n1. Name\n2. IC Number\n3. Gender\n4. Contact number\n5. Email\n"
                << "6. Address\n7. Back\n" << endl;
            validateInput(7, PROMPT);
            switch (input) {
            case 1:
                record(&name, "name", regex("[a-zA-Z .'-/]{4,}"));
                break;
            case 2:
                record(&icNo, "IC number", regex("((\\d{2}(((0[13578]|1[02])(0[1-9]|[12]\\d|3[0-1]))|((0[469]|11)(0[1-9]|[12]\\d|30))|(02(0[1-9]|1\\d|2[0-8]))))|(([02468][048]|[13579][26])0229))(0[1-9]|1[0-6]|2[1-9]|[3-5]\\d)\\d{4}"));
                break;
            case 3:
                record(&gender, "gender", regex("m|M|male|Male|MALE|f|F|female|Female|FEMALE"));
                if (tolower(gender.at(0)) == 'm')
                    gender = "Male";
                else if (tolower(gender.at(0)) == 'f')
                    gender = "Female";
                break;
            case 4:
                record(&contactNo, "contact number", regex("01(\\d){8,9}"));
                break;
            case 5:
                record(&email, "email", regex("[0-9a-zA-Z.-_]+@[0-9a-zA-Z.-_]+.com"));
                break;
            case 6:
                record(&address, "address", regex("[0-9a-zA-Z ,.'-/]+"));
                break;
            }
            goto confirmation;
            break;
        case 2:
            s.name = name;
            s.icNo = icNo;
            s.gender = gender;
            s.contactNo = contactNo;
            s.email = email;
            s.address = address;
            s.setID();
            //s.refreshTrimester();
            studentList.push_back(s);
            updateStudentListCSV();
            cout << "SYSTEM: New student added to student list successfully.\n"
                << "1. Continue to add new student\n2. Back\n" << endl;
            validateInput(2, PROMPT);
            switch (input) {
            case 1:
                insertStudent();
                break;
            case 2:
                loginPage();
                break;
            }
            break;
        case 3:
            loginPage();
            break;
        }
    }
    void viewStudentGameHistory() {
        if (studentGameHistory.empty()) {
            system("CLS");
            cout << "ERROR: Empty student game history.\n1. Back" << endl;
            validateInput(1, PROMPT);
            loginPage();
            return;
        }
        auto digit = [](unsigned int i) {
            int numberOfDigits = 0;         // returns the number of digits of a given integer
            while (i) {                     // value will be added into maxIndexWidth
                i /= 10;
                numberOfDigits++;
            }return numberOfDigits;
        };
        auto format = [](unsigned int num) {        // add commas as thousands seperators
            string input = ts(num), output = "";
            int i = input.size(), count = 0;
            while (i) {
                output = input.at(--i) + output;
                if ((++count == 3) && i) {
                    output = "." + output;
                    count = 0;
                }
            }return output;
        };
        auto findStudentWithID = [&](string str) {
            for (int i = 0; i < studentList.size(); i++) {
                student* s = &studentList[i];
                if (s->ID == str)
                    return i;
            }return -1;
        };
        auto findStudentWithName = [&](string str) {
            for (int i = 0; i < studentList.size(); i++) {
                student* s = &studentList[i];
                if (s->name == str)
                    return i;
            }return -1;
        };
        int currentPage = 1, displayAmount = 20, maxPage;    // alterable to suit user's preference
        auto print = [&](vector<gameRecord> v, int currentPage, int displayAmount) {
            maxPage = (v.size() - 1) / displayAmount + 1;
            int maxNameWidth = 0, maxElapsedTimeWidth = 12;      // can be initialized outside of startPrinting to have a fixed width
            int indexInCurrentPage = currentPage * displayAmount;
            int firstDisplay = (currentPage - 1) * displayAmount;
            int pageLimit = (indexInCurrentPage < v.size()) * indexInCurrentPage + (indexInCurrentPage >= v.size()) * v.size();
            int maxIndexWidth = digit(pageLimit) + 2;   // +2 for the 2 characters ". " after the index
            for (int i = firstDisplay; i < pageLimit; i++) {
                gameRecord* gr = &v[i];
                maxNameWidth += (gr->studentName.length() > maxNameWidth) * (gr->studentName.length() - maxNameWidth);
                maxElapsedTimeWidth += (gr->elapsedTime.length() > maxElapsedTimeWidth) * (gr->elapsedTime.length() - maxElapsedTimeWidth);
            }   // compute and initialize variables that affect table's column width
            string name = "Name", date = "Date", time = "Time", elapsedTime = "Elapsed Time";
            string page = "page " + ts(currentPage) + " of " + ts(maxPage);
            int tableWidth = 58 + maxNameWidth + maxElapsedTimeWidth;
            system("CLS");
            cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
            cout << string(maxIndexWidth, ' ') << "|  Student ID " << RIGHT(maxNameWidth, name, POSITION_CENTER) << RIGHT(10, date, POSITION_CENTER)
                << RIGHT(13, time, POSITION_CENTER) << RIGHT(maxElapsedTimeWidth, elapsedTime, POSITION_CENTER) << "| SCORE |" << endl;
            cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
            for (int i = firstDisplay; i < pageLimit; i++) {
                gameRecord* gr = &v[i];
                cout << setw(maxIndexWidth) << ts(i + 1).append(". ") << RIGHT(11, gr->studentID, POSITION_CENTER)
                    << LEFT(maxNameWidth, gr->studentName, POSITION_DEFAULT) << RIGHT(10, gr->date, POSITION_CENTER)
                    << RIGHT(13, gr->time, POSITION_CENTER) << RIGHT(maxElapsedTimeWidth, gr->elapsedTime, POSITION_CENTER)
                    << RIGHT(5, ts(gr->score), POSITION_CENTER) << "|" << endl;
            }
            cout << string(maxIndexWidth, ' ') << string(tableWidth, '=') << endl;
            string first = format(firstDisplay + 1), last = format(pageLimit), size = format(v.size());
            string entry = v.size() == 1 ? " entry" : " entries";
            cout << string(maxIndexWidth, ' ') << "Current display: " << first << "-" << last << " of " << size << entry  // 22 constant characters
                << setw(tableWidth - 22 - first.length() - last.length() - size.length() - entry.length()) << right << page << endl;
            cout << endl;
        };
        auto filter = [&]() {
            currentTime(HIDE_TIME);
            vector<gameRecord> temp;
            string year, month, day, date, studentID, studentName;
            int position;
        startOfFilter:
            cout << "1. Filter by date\n2. Filter by student ID\n3. Filter by student name\n4. Back\n" << endl;
            validateInput(4, PROMPT);
            temp.clear();
            switch (input) {
            case 1:
            requestDate:
                cout << "Enter year: ";
                validateInput(now.tm_year + 1900, HIDE_PROMPT);
                year = zerosPadded(input, 4);
                cout << "Enter month: ";
                validateInput(12, HIDE_PROMPT);
                month = zerosPadded(input, 2);
                cout << "Enter day: ";
                validateInput(31, HIDE_PROMPT);
                day = zerosPadded(input, 2);
                date = day + "/" + month + "/" + year;
                for (int i = 0; i < studentGameHistory.size(); i++) {
                    gameRecord* gr = &studentGameHistory[i];
                    if (date == gr->date)
                        temp.push_back(*gr);
                }
                if (temp.empty()) {
                    cout << "No game record found on " << date << endl;
                    cout << "1. Search again\n2. Change filter method\n3. Back\n" << endl;
                    validateInput(3, PROMPT);
                    switch (input) {
                    case 1:
                        goto requestDate;
                        break;
                    case 2:
                        goto startOfFilter;
                        break;
                    case 3:
                        viewStudentGameHistory();
                        break;
                    }
                }
                else {
                    print(temp, 1, temp.size());
                    cout << "1. Back\n" << endl;
                    validateInput(1, PROMPT);
                    viewStudentGameHistory();
                }
                break;
            case 2:
            requestStudentID:
                cout << "Enter student ID: ";
                getline(cin, studentID);
                while (!regex_match(studentID, regex("\\d{2}[1-3]\\d{4}-DCS"))) {
                    cout << "Invalid input. Please re-enter student ID: ";
                    getline(cin, studentID);
                }
                position = findStudentWithID(studentID);
                if (position == -1) {
                    cout << "No student with student ID \"" << studentID << "\" found in the system" << endl;
                    cout << "1. Search again\n2. Change filter method\n3. Back\n" << endl;
                    validateInput(3, PROMPT);
                    switch (input) {
                    case 1:
                        goto requestStudentID;
                        break;
                    case 2:
                        goto startOfFilter;
                        break;
                    case 3:
                        viewStudentGameHistory();
                        break;
                    }
                }
                else {
                    vector<gameRecord>* v = &studentList[position].gameHistory;
                    if (v->empty()) {
                        cout << "No game history found for the student with student ID \"" << studentID << "\"\n1. Back\n" << endl;
                        validateInput(1, PROMPT);
                        viewStudentGameHistory();
                    }
                    else {
                        print(*v, 1, v->size());
                        cout << "1. Back\n" << endl;
                        validateInput(1, PROMPT);
                        viewStudentGameHistory();
                    }
                }
                break;
            case 3:
            requestStudentName:
                cout << "Enter student name: ";
                getline(cin, studentName);
                position = findStudentWithName(studentName);
                if (position == -1) {
                    cout << "No student with student name \"" << studentID << "\" found in the system" << endl;
                    cout << "1. Search again\n2. Change filter method\n3. Back\n" << endl;
                    validateInput(3, PROMPT);
                    switch (input) {
                    case 1:
                        goto requestStudentName;
                        break;
                    case 2:
                        goto startOfFilter;
                        break;
                    case 3:
                        viewStudentGameHistory();
                        break;
                    }
                }
                else {
                    vector<gameRecord>* v = &studentList[position].gameHistory;
                    if (v->empty()) {
                        cout << "No game history found for the student with student name \"" << studentName << "\"\n1. Back\n" << endl;
                        validateInput(1, PROMPT);
                        viewStudentGameHistory();
                    }
                    else {
                        print(*v, 1, v->size());
                        cout << "1. Back\n" << endl;
                        validateInput(1, PROMPT);
                        viewStudentGameHistory();
                    }
                }
                break;
            case 4:
                viewStudentGameHistory();
                return;
            }
        };
        auto jumpToPage = [&]() {
            cout << "Jump to page: ";
            validateInput(maxPage, HIDE_PROMPT);
            currentPage = input;
            print(studentGameHistory, currentPage, displayAmount);
        };
        auto setEntriesPerPage = [&]() {
            cout << "Select the number of entries per page\n1. 10 lines\n2. 20 lines (default)\n3. 50 lines" << endl;
            validateInput(3, PROMPT);
            currentPage = 1;
            displayAmount = input * 10 + (input == 3) * 20;
            print(studentGameHistory, currentPage, displayAmount);
        };
        auto runOptions = [&]() {
        provideOptions:
            int option = 0;
            cout << ++option << ". Filter game history" << endl;
            if (maxPage > 1) {
                if (currentPage < maxPage)
                    cout << ++option << ". Next page" << endl;
                if (currentPage > 1)
                    cout << ++option << ". Previous page" << endl;
                cout << ++option << ". Jump to specific page" << endl;
            }
            cout << ++option << ". Adjust entries per page" << endl;
            cout << ++option << ". Sort game history" << endl;
            cout << ++option << ". Back\n" << endl;
            validateInput(option, PROMPT);
            switch (option) {
            case 4:
                switch (input) {
                case 1:
                    filter();
                    break;
                case 2:
                    setEntriesPerPage();
                    goto provideOptions;
                    break;
                case 3:
                    sortGameHistory();
                    break;
                case 4:
                    loginPage();
                    break;
                }
                break;
            case 6:
                switch (input) {
                case 1:
                    filter();
                    break;
                case 2:
                    if (currentPage > 1)
                        currentPage--;
                    else
                        currentPage++;
                    print(studentGameHistory, currentPage, displayAmount);
                    goto provideOptions;
                    break;
                case 3:
                    jumpToPage();
                    goto provideOptions;
                    break;
                case 4:
                    setEntriesPerPage();
                    goto provideOptions;
                    break;
                case 5:
                    sortGameHistory();
                    break;
                case 6:
                    loginPage();
                    break;
                }
                break;
            case 7:
                switch (input) {
                case 1:
                    filter();
                    break;
                case 2:
                    currentPage++;
                    print(studentGameHistory, currentPage, displayAmount);
                    goto provideOptions;
                    break;
                case 4:
                    currentPage--;
                    print(studentGameHistory, currentPage, displayAmount);
                    goto provideOptions;
                    break;
                case 5:
                    jumpToPage();
                    goto provideOptions;
                    break;
                case 6:
                    setEntriesPerPage();
                    goto provideOptions;
                    break;
                case 7:
                    sortGameHistory();
                    break;
                case 8:
                    loginPage();
                    break;
                }
                break;
            }
        };
        print(studentGameHistory, currentPage, displayAmount);
        runOptions();
    };
    void sortGameHistory() {
        cout << "Select the sorting order\n1. Ascending student ID\n2. Descending student ID\n3. Ascending student name\n"
            << "4. Descending student name\n5. Ascending date/time\n6. Descending date/time\n7. Ascending score\n"
            << "8. Descending score\n9. Back\n" << endl;
        validateInput(9, PROMPT);
        switch (input) {
        case 1:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.studentID < b.studentID; });
            break;
        case 2:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.studentID > b.studentID; });
            break;
        case 3:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.studentName < b.studentName; });
            break;
        case 4:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.studentName > b.studentName; });
            break;
        case 5:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.beginningTime < b.beginningTime; });
            break;
        case 6:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.beginningTime > b.beginningTime; });
            break;
        case 7:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.score < b.score; });
            break;
        case 8:
            sort(studentGameHistory.begin(), studentGameHistory.end(), [](gameRecord& a, gameRecord& b) { return a.score > b.score; });
            break;
        }
        viewStudentGameHistory();
    };
};
vector<admin> adminList = { admin("1234567-ABC", "a", "b", "Male") };
void adminLogin() {
    admin* a;
    string username, password;
    int invalidLoginCount = 0;
    auto loginAuth = [&]() {
        for (int i = 0; i < adminList.size(); i++) {
            if (adminList[i].name == username && adminList[i].icNo == password) {
                a = &adminList[i];
                return true;
            }
        }return false;
    };
    auto requestCredentials = [&]() {
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);
    };
    requestCredentials();
    while (!loginAuth()) {
        invalidLoginCount++;
        cout << "Invalid login. Please check username or password.";
        if (invalidLoginCount >= 3)
            cout << " [Hint:Full name(case-sensitive) and IC Number]";
        cout << endl;
        requestCredentials();
    }
    a->loginPage();
};

void init() {
    fstream studentListCSV("Student List.csv");
    if (studentListCSV.is_open()) {
        studentListCSV.close();
        studentListCSV.open("Student List.csv", ios::in);
        string currentLine, word;
        vector<string> temp;
        while (!studentListCSV.eof()) {
            temp.clear();
            getline(studentListCSV, currentLine);
            if (currentLine == "")          // this is necessary to handle existing file that's empty
                break;
            stringstream s(currentLine);
            while (getline(s, word, '|'))
                temp.push_back(word);
            studentList.push_back(student(stoi(temp[0]), stoi(temp[1]), temp[2], temp[3], stoi(temp[5]), stoi(temp[6]), temp[7], temp[8], temp[9], temp[10], temp[11]));
            studentCount[temp[0].substr(2, 2) + temp[1]]++;
        }
        studentListCSV.close();
    }
    else {
        studentListCSV.close();
        studentList.push_back(student(2020, 2, "Wong Jee Ching", "020615101987", "Female"));
        studentList.push_back(student(2020, 2, "Lee Kang Boon", "010430109876", "Male"));
        studentList.push_back(student(2020, 2, "Pang Jin Rong", "020819101478", "Female"));
        studentList.push_back(student(2020, 2, "Lee Kai Wen", "010916101774", "Male"));
        studentList.push_back(student(2020, 2, "Alison Soh Kai Xin", "020210149632", "Female"));
        studentList.push_back(student(2020, 2, "Lee Man Jun", "020405078521", "Male"));
        studentList.push_back(student(2020, 2, "Koo Qiao Lin", "020306011456", "Female"));
        studentList.push_back(student(2020, 3, "Law Wen Jun", "010506147348", "Male"));
        studentList.push_back(student(2020, 3, "Ng Xin Yu", "021215101932", "Female"));
        studentList.push_back(student(2020, 3, "Ng Kok Ming", "020214101463", "Male"));
        studentList.push_back(student(2020, 3, "Tan Fatt Kei", "010105140123", "Female"));
        studentList.push_back(student(2020, 3, "Calvin Khor Wei Xiang", "020214101463", "Male"));
        studentList.push_back(student(2020, 3, "Chen Jun Yu", "011114140536", "Male"));
        studentList.push_back(student(2020, 3, "Ashley Heong Jin Yi", "010827109452", "Female"));
        studentList.push_back(student(2020, 3, "Carrie Chang Wei Yi", "010222145674", "Female"));
        studentList.push_back(student(2021, 1, "Chua Yuke Feng", "031230105442", "Female"));
        studentList.push_back(student(2021, 1, "Chua Ee Vern", "020214147635", "Male"));
        studentList.push_back(student(2021, 1, "Low Yan Wen", "020315140925", "Female"));
        studentList.push_back(student(2021, 1, "Liew Kay Lee", "030329105481", "Female"));
        studentList.push_back(student(2021, 1, "Hong Shi Qi", "021127145227", "Female"));
        studentList.push_back(student(2021, 1, "Ang Soon Bing", "031214108542", "Male"));
        studentList.push_back(student(2021, 2, "Tey Zhi Hong", "020219145219", "Male"));
        studentList.push_back(student(2021, 2, "Easther Ku Xin Yan", "020416145887", "Female"));
        studentList.push_back(student(2021, 2, "Wong Jun Yang", "030409101632", "Male"));
        studentList.push_back(student(2021, 2, "Lai Zi Qi", "031015101468", "Female"));
        studentList.push_back(student(2021, 2, "Melvin Wong Zhi Sheng", "031025146170", "Male"));
        studentList.push_back(student(2021, 2, "Yap Yik Min", "021217108542", "Male"));
        studentList.push_back(student(2021, 3, "Lim Ruen Xii", "030111108452", "Female"));
        studentList.push_back(student(2021, 3, "Loo Ying Huan", "020827142335", "Female"));
        studentList.push_back(student(2021, 3, "Adele Leong Jin Yue", "031126148741", "Female"));
        studentList.push_back(student(2021, 3, "Choo En Yi", "031219102095", "Female"));
        studentList.push_back(student(2022, 1, "Lee Ying Hui", "040216102547", "Female"));
        studentList.push_back(student(2022, 1, "Khoo Kai Zhe", "040512146558", "Male"));
        studentList.push_back(student(2022, 1, "Lee Dong Liang", "041215108836", "Male"));

        for (int i = 0; i < studentList.size(); i++) {
            student* s = &studentList[i];
            s->randomNum();
            s->randomEmail(s->name, s->icNo);
            s->randomAddress();
            s->setID();
            s->refreshTrimester();
            //s->status = STATUS(rand() % 6);         // for fun, also to test out sort function
        }
        updateStudentListCSV();
    }
    for (int i = 0; i < studentList.size(); i++) {
        student* s = &studentList[i];
        s->refreshTrimester();
        for (int j = 0; j < s->trimester - 1 && j < s->defaultArrangement.size(); j++) {
            for (int k = 0; k < s->defaultArrangement[j].size(); k++) {
                subject* sub = &s->defaultArrangement[j][k];
                int marks = 50 + rand() % 51;
                s->updateResults(sub, marks);
            }
        }
    }
    int position = 0;
    ifstream TreasureGameCSV("Treasure Game History.csv", ios::in | ios::app);
    string currentLine, word;
    vector<string> temp;
    while (!TreasureGameCSV.eof()) {
        temp.clear();
        getline(TreasureGameCSV, currentLine);
        if (currentLine == "")
            break;
        stringstream s(currentLine);
        while (getline(s, word, ','))
            temp.push_back(word);
        gameRecord gr = gameRecord(temp[0], temp[1], stoi(temp[2]), stoi(temp[3]), stoi(temp[4]), stoi(temp[5]), stoi(temp[6]), stoi(temp[7]), stoi(temp[8]), stoi(temp[9]), stoi(temp[10]));
        gr.finalize();
        studentGameHistory.push_back(gr);
        while (studentList[position].ID != temp[0])
            position++;
        studentList[position].gameHistory.push_back(gr);
    }
};

int main() {
    srand(time(NULL));
    currentTime(DISPLAY_TIME);
    init();
    cout << "Welcome to New Era University College Electronic Learning Portal\n" << endl;
    cout << "1. Admin login\n2. Student login\n3. Exit\n" << endl;
    validateInput(3, PROMPT);
    switch (input) {
    case 1:
        adminLogin();
        break;
    case 2:
        studentLogin();
        break;
    case 3:
        cout << "Program terminated." << endl;
        break;
    }
    return 0;
}