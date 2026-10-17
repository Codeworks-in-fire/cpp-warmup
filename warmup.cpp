// =====================================================================
//  C++ WARM-UP  ·  African Olympiad Academy
//
//  The question descriptions are in README.md. Read them there.
//  This file holds the data, one worked example, and your answers.
//
//  BUILD AND RUN:   g++ -std=c++17 -Wall -o run warmup.cpp && ./run
//            or:    ./run.sh
//
//  Work on ONE question at a time. Write it, run it, check the output
//  against the README, commit it, move on.
// =====================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

/**
 * THE RECORD
 * One delivery: who brought it, how much, and what grade it was given.
 */
struct Delivery {
    string farmer;
    double kg;
    char   grade;
};
struct Count {
    Delivery del;
    int c;
    };
vector<char> letters = {'a','b','c','A','B','C'};
double payment(double kg, char grade);
bool verid(string id);
string verkg(string kg);

/**
 * THE DATA
 * Do not change these. Every expected answer in the README comes from exactly this data.
 */

// Q1 — one morning's weights, in the order they were weighed
vector<double> morningWeights = {
    45.5, 80.0, 12.0, -3.0, 66.5, 20.0, 31.0, 95.0, 50.0, 18.5, 0.0, 72.0
};

// Q2, Q3, Q4, Q7, Q8 — the day's logbook
vector<Delivery> logbook = {
    {"Keza",    45.5, 'A'},
    {"Bonu",    80.0, 'B'},
    {"Isanda",  20.0, 'A'},
    {"Keza",    12.0, 'C'},
    {"Onyinye", 66.5, 'B'},
    {"Karimi",  31.0, 'A'},
    {"Karimi",  31.0, 'A'},
    {"Keza",    18.5, 'B'},
    {"Bonu",    50.0, 'C'},
    {"Isanda",  20.0, 'A'},
    {"Onyinye", 72.0, 'A'}
};   // yes, one of these rows is in here twice. Q8 is about that.

// Q5 — records as the volunteers typed them, before any checking. Several of these are wrong. That is the point.
struct RawRecord {
    string id;
    string kg;        // text, because a volunteer can type anything
    char   grade;
};

vector<RawRecord> rawRecords = {
    {"F012", "45.5",  'A'},
    {"F7",   "20",    'A'},
    {"F033", "heavy", 'B'},
    {"F021", "0",     'C'},
    {"F044", "80",    'b'},
    {"F055", "",      'A'},
    {"F066", "31.5",  'D'},
    {"F077", "92",    'B'}
};

// Q6 — an Igisoro board. 16 pits, in sowing order.
//      Position 0 is unused so that board[1] really is pit 1.
vector<int> board = {0, 0, 0, 1, 0, 0, 4, 1, 0, 0, 2, 2, 0, 1, 5, 0, 0};


// ---------------------------------------------------------------------
//  A HELPER YOU MAY FIND USEFUL
// ---------------------------------------------------------------------
void printBoard(const vector<int> &b) {
    for (int i = 1; i <= 16; i++) cout << b[i] << " ";
    cout << "\n";
}

int findFarmer(const vector<Delivery> &log, string name);
int findFarmer2(const vector<struct Count> &log, string name);

int main() {

    // =================================================================
    //  WORKED EXAMPLE  ·  not a question
    //
    //  This is the house style: walk the data once, keep what you need
    //  as you go, print at the end. Read it before you start Q1.
    //
    //  It answers: how many deliveries were grade A, and what was the
    //  heaviest single delivery in the whole logbook?
    // =================================================================

    int gradeACount = 0;
    Delivery heaviest = logbook[0];        // start from a REAL entry,
                                           // never from a made-up 0
    for (Delivery d : logbook) {
        if (d.grade == 'A') {
            gradeACount++;
        }
        if (d.kg > heaviest.kg) {
            heaviest = d;                  // keep the WHOLE record,
        }                                  // not just the number
    }

    cout << "=== Worked example ===\n";
    cout << "Grade A deliveries: " << gradeACount << "\n";
    cout << "Heaviest: " << heaviest.farmer << " with "
         << heaviest.kg << " kg\n\n";


    // =================================================================
    //  Q1  ·  THE WEIGHING SESSION          (see README)
    //  Uses: morningWeights
    // =================================================================

    cout << "=== Q1 ===\n";
    double total = 0;
    int valid = 0;
    double heavykg = morningWeights[0];
    int heavypos = 0;
    int lrun = 0;
    int crun = 0;
    for (int i = 0; i < 12; i++) {
        double temp = morningWeights[i];
        total += temp;
        if (0 < temp && temp<=80){
            valid++;
            crun++;
        } else {
            if (lrun < crun){
                lrun = crun;
            }
            crun = 0;
        }
        if (temp > heavykg){
            heavykg = temp;
            heavypos = i;
        }
    }
    cout << "Total: " << total << " kg" << endl;
    cout << "Valid: " << valid << " of 12" << endl;
    cout << "Heaviest: " << heavykg << " kg at position " << heavypos << endl;
    cout << "Longest valid run: " << lrun << endl;
    cout << "\n";


    // =================================================================
    //  Q2  ·  GRADE SUMMARY                 (see README)
    //  Uses: logbook
    // =================================================================

    cout << "=== Q2 ===\n";
    int gradeCount[3] = {0}; // Initialize with 3 elements, all 0
    double totalWeight[3] = {0.0}; // Initialize with 3 elements, all 0.0
    double averageWeight[4] = {0.0}; // Initialize with 3 elements, all 0.0
    char BestGrade;
    for (const Delivery& d : logbook) {
        if (d.grade == 'A') {
            gradeCount[0]++;
            totalWeight[0] += d.kg;
        } else if (d.grade == 'B') {
            gradeCount[1]++;
            totalWeight[1] += d.kg;
        } else if (d.grade == 'C') {
            gradeCount[2]++;
            totalWeight[2] += d.kg;
        }
    }
    const char Grade[] = {'A','B','C'};
    for (int i = 0; i < 3; i++) {
        if (gradeCount[i] > 0) {
            averageWeight[i] = totalWeight[i] / gradeCount[i];
        }
        if (averageWeight[3] < averageWeight[i]) {
            averageWeight[3] = averageWeight[i];
            BestGrade = Grade[i];
        }
    }

    cout << "Grade A: " << gradeCount[0] << " deliveries, " << totalWeight[0] << " kg total, " << averageWeight[0] << " kg average" << endl;
    cout << "Grade B: " << gradeCount[1] << " deliveries, " << totalWeight[1] << " kg total, " << averageWeight[1] << " kg average" << endl;
    cout << "Grade C: " << gradeCount[2] << " deliveries, " << totalWeight[2] << " kg total, " << averageWeight[2] << " kg average" << endl;
    cout << "Best average: grade " << BestGrade << endl;
    cout << "\n";


    // =================================================================
    //  Q3  ·  WHO IS WHO                     (see README)
    //  Uses: logbook, and the findFarmer function you write below main
    // =================================================================

    cout << "=== Q3 ===\n";
    vector<Count> kilogram = {};
    for (const Delivery& d : logbook) {
        int temp = findFarmer2(kilogram,d.farmer);
        if (temp == -1){
            kilogram.push_back({d,1});
        }else {
            kilogram[temp].del.kg += d.kg;
            kilogram[temp].c ++;
        }
    }
    Delivery highest = kilogram[0].del;
    Count most = kilogram[0];
    for (const Count& d : kilogram) {
        if (highest.kg < d.del.kg){
            highest = d.del;
        }
        if (most.c < d.c){
            most = d;
        }
    }
    cout << "findFarmer(Isanda) = " << findFarmer(logbook,"Isanda") << endl;
    cout << "findFarmer(Mukamana) = " << findFarmer(logbook,"Mukamana") << endl;
    cout << "Most deliveries: " << most.del.farmer << " (" << most.c << ")" << endl;
    cout << "Most kg: " << highest.farmer << " (" << highest.kg << " kg)" << endl;

    cout << "\n";


    // =================================================================
    //  Q4  ·  PAYMENT RUN WITH A DAILY CAP  (see README)
    //  Uses: logbook, and the payment function you write below main
    // =================================================================

    cout << "=== Q4 ===\n";
    double dailytotal = 0.0;
    for (const Delivery& d : logbook) {
        double temp = payment(d.kg,d.grade);
        if (dailytotal <= 100000) {
            cout << d.farmer << " " << d.kg << " kg grade " << d.grade << " -> " << temp << endl;
        } else{
            temp = temp/2;
            cout << d.farmer << " " << d.kg << " kg grade " << d.grade << " -> " << temp << "   (halved)" << endl;
        }
        dailytotal += temp;
    }
    cout << "Total paid: " << dailytotal;
    cout << "\n";


    // =================================================================
    //  Q5  ·  THE VALIDATION REPORT         (see README)
    //  Uses: rawRecords
    // =================================================================

    cout << "=== Q5 ===\n";
    int accepted = 0;
    int rejected = 0;
    for (const RawRecord& d : rawRecords) {
        if (verid(d.id) == false){
            rejected ++;
            cout << d.id << " REJECT- bad id" << endl;
        } else if (d.kg.empty()){
            rejected ++;
            cout << d.id << " REJECT - weight missing" << endl;
        } else if (verkg(d.kg) != ""){
            rejected ++;
            cout << d.id <<  verkg(d.kg) << endl;
        } else if (find(letters.begin(),letters.end(), d.grade) == letters.end()){
            rejected ++;
            cout << d.id << " REJECT - unknown grade" <<endl;
        } else {
            accepted ++;
            cout << d.id << " ACCEPT" << endl;
        }
    }
    cout << accepted << " accepted, " << rejected << " rejected" << endl;

    cout << "\n";

    // =================================================================
    //  Q6  ·  IGISORO: A FULL TURN          (see README)
    //  Uses: board
    // =================================================================

    cout << "=== Q6 ===\n";
    bool con = true;
    int play = 6 , laps = 0 , current = play;
    while (con) {
        laps ++;
        for (int i = 0; i < board[play];i++){
            current = (current % 16) + 1;
            board[current] ++;
        }
        if (board[current] == 1) {
            con = false;
        }
        board[play] = 0;
        play = current;
    }
    cout << "Final board: ";
    printBoard(board);
    cout << "Landed in pit " << play << " after " << laps << " laps" << endl;


    
    // =================================================================
    //  STRETCH  ·  optional
    // =================================================================

    // =================================================================
    //  Q7  ·  SORT THE LOGBOOK              (see README)
    //  Uses: logbook
    // =================================================================

    // cout << "=== Q7 ===\n";





    // =================================================================
    //  Q8  ·  SPOTTING DOUBLE-ENTRIES       (see README)
    //  Uses: logbook
    // =================================================================

    // cout << "=== Q8 ===\n";





    return 0;
}


// =====================================================================
//  FUNCTIONS YOU WRITE
//
//  C++ reads a file top to bottom, so a function must be DECLARED above
//  the place that calls it. These two are declared here at the bottom,
//  which main() cannot see — so move them ABOVE main(), or leave a
//  one-line declaration up there. Working that out is part of Q3.
// =====================================================================

// Q3:  return the position of the first delivery by this farmer,
//      or -1 if the farmer is not in the log at all.
//
int findFarmer(const vector<Delivery> &log, string name) {
    for (const Delivery& d : log) {
        if (d.farmer == name) {
            return &d - &log[0]; // Return the index of the delivery
        }
    }
    return -1; // Farmer not found

}
int findFarmer2(const vector<struct Count> &log, string name) {
    for (const Count& d : log) {
        if (d.del.farmer == name) {
            return &d - &log[0]; // Return the index of the delivery
        }
    }
    return -1; // Farmer not found

}


// Q4:  what one delivery earns, before any cap is applied.
//      Grade A pays 10% above the rate, B pays the rate,
//      anything else pays 20% below it. The rate is 450.
//
double payment(double kg, char grade) {
    if (grade == 'A') {
        return kg*450*1.1;
    }else if (grade =='B') {
        return kg*450;
    }else {
        return kg*450*0.8;
    }
}

bool verid(string id) {
    if (id.length() != 4) {
        return false;
    }
    if (id[0] !='F') {
        return false;
    }
    for (int i = 1; i < 4 ; i++) {
        for (int j = 0 ; j < 10 ;  j++) {
            if (id[i] == j + '0'){
                break;
            } 
            if (j == 9) {
                return false;
            }
        }
    }
    return true;
}

string verkg(string kg) {
    try {
        double l = stod(kg);
        if (l <= 0.0 || l > 80.0){
            return " REJECT - weight out of range";
        }
        return "";
    }
    catch(const invalid_argument&){
        return " REJECT - weight not a number";
    }
    catch(const out_of_range&){
        return  " REJECT - weight out of range";
    }
}
