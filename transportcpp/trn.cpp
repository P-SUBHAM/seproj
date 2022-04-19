#include<bits/stdc++.h>
#include<string.h>
#include<ctime>
using namespace std;
///////////////////////////////////////////////
int setwsize = 30;
int dashsize = 60;

time_t timenow() {
    time_t t;
    time(&t);
    return t;
}

map<string,pair<double,double>> citydistance;

////////////////////////////////////////////////


double toRadians(const double degree);
double distancebetcity(double lat1, double long1,double lat2, double long2);
bool authenticate(string usrtype, string username, string password);
bool authenticatemgr();
bool authenticateclerk();
int managermenuinp();
int clerkmenuinp();
int loginmenuinp();
void addtruck();
void removetruck();
void printalltrucks();
void viewconsignment();
void loadinfo();
void saveinfo();
void registerconsignment();
void managermenu(int choice);
void clerkmenu(int choice);
// void transactmove(consignment c);
// void transactupdate(vector<consignment> c);
void updatetransactiondb();
// void updatelocationtruck(truck trucks[togo]);

////////////////////////////////////////////////

double toRadians(const double degree)
{
    // cmath library in C++
    // defines the constant
    // M_PI as the value of
    // pi accurate to 1e-30
    double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}
 
double distancebetcity(double lat1, double long1,double lat2, double long2)
{
    // Convert the latitudes
    // and longitudes
    // from degree to radians.
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);
     
    // Haversine Formula
    double dlong = long2 - long1;
    double dlat = lat2 - lat1;
 
    double ans = pow(sin(dlat / 2), 2) +
                          cos(lat1) * cos(lat2) *
                          pow(sin(dlong / 2), 2);
 
    ans = 2 * asin(sqrt(ans));
 
    // Radius of Earth in
    // Kilometers, R = 6371
    // Use R = 3956 for miles
    double R = 6371;
     
    // Calculate the result
    ans = ans * R;
 
    return ans;
}

////////////////////////////////////////////////
class truck {
    public:
    int id;
    int capacity;
    int speed;
    time_t time;
    int distance;
    string locationtruck;
    int duration;
    truck(int i, int c, int s,string loc = "delhi", int dist = 0, string t="") {
        id = i;
        capacity = c;
        speed = s;
        if(t=="") {
            time = timenow();
        } else {
            time = stol(t);
        }
        locationtruck = loc;
        distance = dist;
    }
    void displaytruckinfo() {
        cout<<setw(setwsize);cout<<"--------"<<"---"<<setw(setwsize)<<"\n";
        cout<<setw(setwsize);
        cout<<"| Truck "<<id<<" |";cout<<endl;
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}    cout<<"\n";
        //cout<<setw(setwsize);cout<<"--------"<<"---"<<setw(setwsize)<<"\n";
        cout<<"Capacity: "<<capacity<<" Speed: "<<speed<<" Location: "<<locationtruck<<"\n";
        time_t currt = time + distance/speed;
        cout<<"Distance: "<<distance<<" Time: "<< ctime(&currt);
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}  cout<<"\n";
    }


};


class consignment {
    public:
    int cid;
    int volume;
    string destadd;
    string sendadd;
    int charge;
    double distance;
    string billoc;
    time_t time;
    int waittime;
    int truckid = -1; //
    string status = "pending"; //
    void generateDispatchSlip() {
        fstream billfile;
        billfile.open(billoc, ios::out);
        billfile<<"Consignment "<<cid<<"\n";
        billfile<<"Volume: "<<volume<<"\n";
        billfile<<"Destination Address: "<<destadd<<"\n";
        billfile<<"Source Address: "<<sendadd<<"\n";
        billfile<<"Charge: "<<charge<<"\n";
        billfile.close();
    }
    consignment(int i, int vol,string sadd, string dadd,  string t = "", string wtt = "") {
        cid = i;
        volume = vol;
        destadd = dadd;
        transform(destadd.begin(), destadd.end(), destadd.begin(), ::tolower);
        sendadd = sadd;
        transform(sendadd.begin(), sendadd.end(), sendadd.begin(), ::tolower);
        distance = distancebetcity(citydistance[sendadd].first,citydistance[sendadd].second, citydistance[destadd].first,citydistance[destadd].second);
        charge = distance*volume/5;
        billoc = "db/bill/bill"+to_string(cid)+".txt";
        if(t=="") {
            time = timenow();
        } else {
            time = stol(t);
        }
        if(wtt == "") {
            waittime = 0;
        } else {
            waittime = stoi(wtt);
        }
        generateDispatchSlip();
    }
    void displayconsignmentinfo() {
        generateDispatchSlip();
        cout<<setw(setwsize);cout<<"--------"<<"---"<<setw(setwsize)<<"\n";
        cout<<setw(setwsize);
        cout<<"| Consignment "<<cid<<" |";cout<<endl;
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}    cout<<"\n";
        cout<<"Volume: "<<volume<<" \nDestination Address: "<<destadd<<"\n";
        cout<<"Source Address: "<<sendadd<<"\n";
        cout<<"Distance: "<<distance<<"\nCharge: "<<charge<<"\n";
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}    cout<<"\n";
    }
    

};

//////////////////////////////////////////////////

vector<truck> trucks;
vector<consignment> consignments;
vector<consignment> consignmentsdelivered;
map<string,string> mgrpsswd;
map<string,string> clkpsswd;
map<string,int> loads;


/////////////////////////////////////////////////
bool authenticate(string usrtype, string username, string password) {
    if(usrtype=="manager") {
        if(mgrpsswd[username]==password) {
            return true;
        } else {
            return false;
        }
    } else if(usrtype=="clerk") {
        if(clkpsswd[username]==password) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool authenticatemgr() {
    string username, password;
    cout<<"Enter username: ";
    cin>>username;
    cout<<"Enter password: ";
    cin>>password;
    if(authenticate("manager", username, password)) {
        return true;
    } else {
        return false;
    }
}

bool authenticateclerk() {
    string username, password;
    cout<<"Enter username: ";
    cin>>username;
    cout<<"Enter password: ";
    cin>>password;
    if(authenticate("clerk", username, password)) {
        return true;
    } else {
        return false;
    }
}

int managermenuinp() {
    cout<<"\n\n1. Add truck\n2. Remove truck\n3. Print all trucksv\n4. LOG-OUT!!!\n5. EXIT\n6. MANAGE STATISTICS\n";
    cout<<"\n";
    int choice;
    cin>>choice;
    cout<<"\n";
    switch(choice) {
        case 1: {
            return 1;
            break;
        }
        case 2: {
            return 2;
            break;
        }
        case 3: {
            return 3;
            break;
        }
        case 4: {
            return 4;
            break;
        }
        case 5: {
            return 5;
            break;
        }
        case 6: {
            return 6;
            break;
        }
        default: {
            cout<<"Invalid choice\n";
            return managermenuinp();
        }
    }
}

int clerkmenuinp() {
    cout<<"\n\n1. Register Consigment\n2. Dispatch Slip\n3. LOG-OUT!!!\n4. EXIT\n";
    int choice;
    cout<<"\n";

    cin>>choice;
    cout<<"\n";
    switch(choice) {
        case 1: {
            return 1;
            break;
        }
        case 2: {
            return 2;
            break;
        }
        case 3: {
            return 3;
            break;
        }
        case 4: {
            return 4;
            break;
        }
        default: {
            cout<<"Invalid choice\n";
            return clerkmenuinp();
        }
    }
}

int loginmenuinp() {
    cout<<"\n\n1. Manager\n2. Clerk\n3. EXIT\n";
    int choice;
    cin>>choice;
    cout<<"\n";
    switch(choice) {
        case 1: {
            if(authenticatemgr() == false) {
                cout<<"Invalid credentials\n";
                return loginmenuinp();
            }
            return 1;
            break;
        }
        case 2: {
            if(authenticateclerk() == false) {
                cout<<"Invalid credentials\n";
                return loginmenuinp();
            }
            return 2;
            break;
        }
        case 3: {
            return 3;
            break;
        }
        default: {
            cout<<"Invalid choice\n";
            return loginmenuinp();
        }
    }
}

void addtruck() {
    int id, capacity, speed; int time; string loc;
    cout<<"Enter id, capacity, speed, location\n";
    cin >> id >> capacity >> speed >> loc;
    for(int i = 0; i < trucks.size(); i++) {
        if(trucks[i].id == id) {
            cout<<"Truck with id "<<id<<" already exists\n";
            return;
        }
    }
    trucks.push_back(truck(id, capacity, speed, loc));
    saveinfo();
}



void removetruck() {
    int id;
    cout<<"Enter id of truck to be removed\n";
    cin >> id;
    for(int i = 0; i < trucks.size(); i++) {
        if(trucks[i].id == id) {
            trucks.erase(trucks.begin() + i);
            cout<<"Truck with id "<<id<<" removed\n";
            saveinfo();
            break;
        }
    }
    cout<<"Truck with id "<<id<<" not found\n";
    saveinfo();
}
       
void printalltrucks() {
    for(int i = 0; i < trucks.size(); i++) {
        trucks[i].displaytruckinfo();
        // cout<<"Truck "<<trucks[i].id<<"\n";
        // cout<<"-----------\n";
        // cout<<"Capacity: "<<trucks[i].capacity<<" Speed: "<<trucks[i].speed<<" Time: "<<trucks[i].time<<"\n";
        // cout<<"----------------------\n";
    }
}

void viewconsignment() {
    int id;
    cout<<"Enter id of consignment to be viewed\n";
    cin >> id;
    bool found = false;
    for(int i = 0; i < consignments.size(); i++) {
        if(consignments[i].cid == id) {
            found = true;
            consignments[i].displayconsignmentinfo();
            break;
        }
    }
    if(!found) {
        cout<<"Consignment not found\n";
    }
}

void loadinfo() {
    /////////////////////////////////// TRUCK ///////////////////////////////////////////////
    fstream truckfile;
    truckfile.open("db/trucksdb.txt", ios::in);
    if(!truckfile) {
        cout<<"File not found truckdb 1\n";
        return;
    }
    string line;
    while(getline(truckfile, line)) {
        if(line.size() == 0) {
            continue;
        }
        stringstream ss(line);
        string id, capacity, speed, loc, distance, time;
        getline(ss, id, ',');
        getline(ss, capacity, ',');
        getline(ss, speed, ',');
        getline(ss, loc, ',');
        getline(ss, distance, ',');
        getline(ss, time);
        trucks.push_back(truck(stoi(id), stoi(capacity), stoi(speed),loc,stoi(distance), time));
    }
    /////////////////////////////////// PASSWORD ///////////////////////////////////////////////
    fstream passwordfile;
    passwordfile.open("db/password.txt", ios::in);
    if(!passwordfile) {
        cout<<"File not found password 1\n";
        return;
    }
    string line1;
    while(getline(passwordfile, line1)) {
        stringstream ss1(line1);
        string usertype,username, password;
        getline(ss1, usertype, ',');
        getline(ss1, username, ',');
        getline(ss1, password);
        if(usertype == "manager") {
            mgrpsswd[username] = password;
        } else if(usertype == "clerk") {
            clkpsswd[username] = password;
        }
    }
    /////////////////////////////////// CITY DISTANCE ///////////////////////////////////////////////
    fstream citydistancefile;
    citydistancefile.open("db/citydistance.txt", ios::in);
    if(!citydistancefile) {
        cout<<"File not found citydistance 1\n";
        return;
    }
    string line2;
    while(getline(citydistancefile, line2)) {
        stringstream ss2(line2);
        string city, l1, l2;
        getline(ss2, city, ',');
        getline(ss2, l1, ',');
        getline(ss2, l2);
        double lat = stod(l1);
        double lon = stod(l2);
        citydistance[city] = make_pair(lat, lon);
    }

    /////////////////////////////////// CONSIGMENT ///////////////////////////////////////////////
    fstream consignmentfile;
    consignmentfile.open("db/consignmentdb.txt", ios::in);
    if(!consignmentfile) {
        cout<<"File not found consignmentdb 1\n";
        return;
    }
    string line3;
    while(getline(consignmentfile, line3)) {
        stringstream ss3(line3);
        if(line3.size() == 0) continue;
        string cid, vol, dest, source,charge,distance, timev, wttime;
        getline(ss3, cid, ',');
        getline(ss3, vol, ',');
        getline(ss3, source, ',');
        getline(ss3, dest, ',');
        getline(ss3, charge, ',');
        getline(ss3, distance, ',');
        getline(ss3, timev, ',');
        getline(ss3, wttime);
        consignments.push_back(consignment(stoi(cid), stoi(vol), source, dest,  timev, wttime));
    }
    /////////////////////////////////// LOAD ///////////////////////////////////////////////
    for(int i = 0; i < consignments.size(); i++) {
        loads[consignments[i].sendadd] += consignments[i].volume;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

}

void updatetransactiondb() {
    //copy content of all deliveredconsignments into transaction.txt
    fstream transactionfile;
    transactionfile.open("db/transaction.txt", ios::app); // check changed from out to app
    if(!transactionfile) {
        cout<<"File not found transaction 1\n";
        return;
    }
    for(int i = 0; i < consignmentsdelivered.size(); i++) {
        transactionfile<<consignmentsdelivered[i].cid<<","<<consignmentsdelivered[i].volume<<","<<consignmentsdelivered[i].sendadd<<","<<consignmentsdelivered[i].destadd<<","<<consignmentsdelivered[i].charge<<","<<consignmentsdelivered[i].distance<<","<<consignmentsdelivered[i].time<<","<<consignmentsdelivered[i].waittime<<"\n";
    }
    // added 20-04-2022
    transactionfile.close();
    consignmentsdelivered.clear();
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // for(int i = 0; i < deliveredconsignments.size(); i++) {
    //     transactionfile<<deliveredconsignments[i].id<<","<<deliveredconsignments[i].volume<<","<<deliveredconsignments[i].dest<<","<<deliveredconsignments[i].source<<","<<deliveredconsignments[i].charge<<","<<deliveredconsignments[i].distance<<","<<deliveredconsignments[i].time<<","<<deliveredconsignments[i].waittime<<"\n";
    // }
}

void updatelocationtruck(truck trucksv1) {
    //append details of trucksv1 to db/location.txt
    fstream locationfile;
    locationfile.open("db/location.txt", ios::app);
    if(!locationfile) {
        cout<<"File not found location 1\n";
        return;
    }
    locationfile<<trucksv1.id<<","<<trucksv1.locationtruck<<","<<trucksv1.time<<"\n";
}

void transactupdate(vector<consignment> congstage) {
    int tempdur = 0;
    string source = congstage[0].sendadd;
    set<string> destt;
    for(int i = 0; i < congstage.size(); i++) {
        destt.insert(congstage[i].destadd);
    }
    int togo;
    for(int i = 0; i < trucks.size(); i++) {
        if(trucks[i].locationtruck == source) {
            togo = i;
            break;
        }
    }
    for(int i = 0; i < trucks.size(); i++) {
        if(trucks[i].locationtruck == source && trucks[i].time < trucks[togo].time) {
            togo = i;
        }
    }
    vector<string> loct;
    loct.push_back(source);
    for(auto it: destt) {
        loct.push_back(it);
    }
    // loct.push_back(source);
    int tempdurt = 0;
    for(int i = 1; i < loct.size(); i++) {
        double lat1 = citydistance[loct[i-1]].first;
        double lon1 = citydistance[loct[i-1]].second;
        double lat2 = citydistance[loct[i]].first;
        double lon2 = citydistance[loct[i]].second;
        double distance = distancebetcity(lat1, lon1, lat2, lon2);
        tempdurt += (distance*60*60.0/trucks[togo].speed);
        trucks[togo].duration += tempdurt;
        // trucks[togo].locationtruck = loct[i]; //implement imp
        trucks[togo].locationtruck = loct[i];
        updatelocationtruck(trucks[togo]);
        trucks[togo].distance += distance;
        string desttemp = loct[i];
        for(int j = 0; j < congstage.size(); j++) {
            if(congstage[j].destadd == desttemp) {
                congstage[j].waittime = tempdurt;
                congstage[j].truckid = trucks[togo].id;
                congstage[j].status = "delivered";
            }
        }
    }
    for(int i = 0; i < congstage.size(); i++) {
        consignmentsdelivered.push_back(congstage[i]);
    }
    updatetransactiondb();
}

void transactmove(consignment c) {
    string source = c.sendadd;
    int vol = c.volume;
    if(loads[source] + int(vol) < 500) {
        loads[source] += int(vol);
        consignments.push_back(consignment(c.cid, vol, source, c.destadd));
    }
    else if(loads[source] + int(vol) == 500) {
        loads[source] = 0;
        consignments.push_back(consignment(c.cid, vol, source, c.destadd));
        vector<consignment> congstage;
        vector<consignment> newconsg;
        // remove all consignment from consignments where object sendadd is source and add it to congstage
        for(int i = 0; i < consignments.size(); i++) {
            if(consignments[i].sendadd == source) {
                congstage.push_back(consignments[i]);
            }
            else {
                newconsg.push_back(consignments[i]);
            }
        }
        consignments = newconsg;
        transactupdate(congstage); // to implement
    }
    else {
        loads[source] = int(vol);
        vector<consignment> congstage;
        vector<consignment> newconsg;
        // remove all consignment from consignments where object sendadd is source and add it to congstage
        for(int i = 0; i < consignments.size(); i++) {
            if(consignments[i].sendadd == source) {
                congstage.push_back(consignments[i]);
            }
            else {
                newconsg.push_back(consignments[i]);
            }
        }
        consignments = newconsg;
        consignment topass2(c.cid, vol, source, c.destadd);
        consignments.push_back(topass2);
        transactupdate(congstage); // to implement
        
    }
}

void saveinfo() {
    cout<<"saving updates\n";
    fstream truckfile;
    truckfile.open("db/trucksdb.txt", ios::out);
    if(!truckfile) {
        cout<<"File not found truckdb 2\n";
        return;
    }
    for(int i = 0; i < trucks.size(); i++) {
        truckfile<<trucks[i].id<<","<<trucks[i].capacity<<","<<trucks[i].speed<<","<<trucks[i].locationtruck<<","<<trucks[i].distance<<","<<trucks[i].time<<"\n";
    }

    fstream consignmentfile;
    consignmentfile.open("db/consignmentdb.txt", ios::out);
    if(!consignmentfile) {
        cout<<"File not found consignmentdb 2\n";
        return;
    }
    for(int i = 0; i < consignments.size(); i++) {
        consignmentfile<<consignments[i].cid<<","<<consignments[i].volume<<","<<consignments[i].sendadd<<","<<consignments[i].destadd<<","<<consignments[i].charge<<","<<consignments[i].distance<<","<<consignments[i].time<<","<<consignments[i].waittime<<"\n";
    }


}

void registerconsignment() { // transaction here only
    int id, vol; string source, destination;
    cout<<"Enter id, vol, source, destination\n";
    cin >> id >> vol >> source >> destination;
    for(int i = 0; i < consignments.size(); i++) {
        if(consignments[i].cid == id) {
            cout<<"Consignment already exists\n";
            return;
        }
    }// check if consignment already exists
    consignment topass(id, vol, source, destination);
    transactmove(topass);
    saveinfo();
}

void truckstatus() {
    cout<<"Enter truck id\n";
    int id;
    cin >> id;
    //check if truck id exists or not
    bool found = false;
    for(int i = 0; i < trucks.size(); i++) {
        if(trucks[i].id == id) {
            found = true; break;
        }
    }
    if(!found) {
        cout<<"Truck not found\n";
        return;
    }
    vector<pair<string,string>> truckloc;
    //read location.txt
    fstream locationfile;
    locationfile.open("db/location.txt", ios::in);
    if(!locationfile) {
        cout<<"File not found location 3\n";
        return;
    }
    string line;
    while(getline(locationfile, line)) {
        stringstream ss(line);
        string truckid;
        string location;
        string timeunixv;
        getline(ss, truckid, ',');
        getline(ss, location, ',');
        getline(ss, timeunixv, ',');
        if(truckid == to_string(id)) {
            //convert timeunixv to time
            int timeunix = stoi(timeunixv);
            time_t t = timeunix;
            struct tm * timeinfo = localtime(&t);
            char buffer[80];
            strftime(buffer, 80, "%d/%m/%Y %I:%M:%S", timeinfo);
            string timestr(buffer);
            truckloc.push_back({location, timestr});
        }
    }
    cout<<"Truck id: "<<id<<" STATUS\n";
    for(int i = 0; i < truckloc.size(); i++) {
        cout<<"Location: "<<truckloc[i].first<<" Time: "<<truckloc[i].second<<"\n";
    }
    cout<<"\n";
}

void truckusage() {
    cout<<"Enter truck id\n";
    int id;
    cin >> id;
    //check if truck id exists or not
    bool found = false;
    int i = 0;
    for(; i < trucks.size(); i++) {
        if(trucks[i].id == id) {
            found = true; break;
        }
    }
    if(!found) {
        cout<<"Truck not found\n";
        return;
    }
    truck t = trucks[i];
    // print duration and distance
    cout<<"Truck id: "<<id<<" USAGE\n";
    int duration = t.distance/t.speed;
    // print duration hours in days and hours 
    int days = duration/24;
    int hours = duration%24;
    cout<<"Duration: "<<days<<" days "<<hours<<" hours\n";
    
    cout<<"Distance: "<<t.distance<<" KM\n";
    cout<<"\n";
}

void consignmentstatus() {
    // read consignmentdb.txt and transaction.txt 
    fstream consignmentfile;
    consignmentfile.open("db/consignmentdb.txt", ios::in);
    if(!consignmentfile) {
        cout<<"File not found consignmentdb 3\n";
        return;
    }
    string line;
    vector<consignment> consg;
    while(getline(consignmentfile, line)) {
        stringstream ss(line);
        string cid, volume, sendadd, destadd, charge, distance, time, waittime;
        getline(ss, cid, ',');
        getline(ss, volume, ',');
        getline(ss, sendadd, ',');
        getline(ss, destadd, ',');
        getline(ss, charge, ',');
        getline(ss, distance, ',');
        getline(ss, time, ',');
        getline(ss, waittime, ',');
        consignment c(stoi(cid), stoi(volume), sendadd, destadd, time, waittime);
        consg.push_back(c);
    }
    fstream transactionfile;
    transactionfile.open("db/transaction.txt", ios::in);
    if(!transactionfile) {
        cout<<"File not found transaction 3\n";
        return;
    }
    while(getline(transactionfile, line)) {
        stringstream ss(line);
        string cid, volume, sendadd, destadd, charge, distance, time, waittime;
        getline(ss, cid, ',');
        getline(ss, volume, ',');
        getline(ss, sendadd, ',');
        getline(ss, destadd, ',');
        getline(ss, charge, ',');
        getline(ss, distance, ',');
        getline(ss, time, ',');
        getline(ss, waittime, ',');
        // string to float distance
        float distancef = stof(distance);
        consignment c(stoi(cid), stoi(volume), sendadd, destadd, time, waittime);
        consg.push_back(c);
    }
    cout<<"Consignment STATUS\n";
    cout<<"Enter consignment id\n";
    int id;
    cin >> id;
    bool found = false;
    for(int i = 0; i < consg.size(); i++) {
        if(consg[i].cid == id) {
            found = true;
            cout<<"Consignment id: "<<id<<"\n";
            cout<<"Volume: "<<consg[i].volume<<"\n";
            cout<<"Source: "<<consg[i].sendadd<<"\n";
            cout<<"Destination: "<<consg[i].destadd<<"\n";
            cout<<"Charge: "<<consg[i].charge<<"\n";
            cout<<"Distance: "<<consg[i].distance<<"\n";
            // convert time to time_t
            long long int timesv = consg[i].time;
            time_t t = timesv;
            struct tm * timeinfo = localtime(&t);

            char buffer[80];
            strftime(buffer, 80, "%d/%m/%Y %I:%M:%S", timeinfo);
            string timestr(buffer);
            cout<<"Time: "<<timestr<<"\n";
            //convert waittime in seconds to days and hours and minutes
            int waittimesv = consg[i].waittime;
            int waittime = waittimesv/3600;
            int days = waittime/24;
            int hours = waittime%24;
            cout<<"Wait time: "<<days<<" days "<<hours<<" hours "<<waittime%60<<" minutes\n";
            break;
        }
    }
    if(!found) {
        cout<<"Consignment not found\n";
    }
    cout<<"\n";
    
}

void idletime() {
    // read consignmentdb.txt and transaction.txt 
    fstream consignmentfile;
    consignmentfile.open("db/consignmentdb.txt", ios::in);
    if(!consignmentfile) {
        cout<<"File not found consignmentdb 3\n";
        return;
    }
    string line;
    vector<consignment> consg;
    while(getline(consignmentfile, line)) {
        stringstream ss(line);
        string cid, volume, sendadd, destadd, charge, distance, time, waittime;
        getline(ss, cid, ',');
        getline(ss, volume, ',');
        getline(ss, sendadd, ',');
        getline(ss, destadd, ',');
        getline(ss, charge, ',');
        getline(ss, distance, ',');
        getline(ss, time, ',');
        getline(ss, waittime, ',');
        consignment c(stoi(cid), stoi(volume), sendadd, destadd, time, waittime);
        consg.push_back(c);
    }
    fstream transactionfile;
    transactionfile.open("db/transaction.txt", ios::in);
    if(!transactionfile) {
        cout<<"File not found transaction 3\n";
        return;
    }
    while(getline(transactionfile, line)) {
        stringstream ss(line);
        string cid, volume, sendadd, destadd, charge, distance, time, waittime;
        getline(ss, cid, ',');
        getline(ss, volume, ',');
        getline(ss, sendadd, ',');
        getline(ss, destadd, ',');
        getline(ss, charge, ',');
        getline(ss, distance, ',');
        getline(ss, time, ',');
        getline(ss, waittime, ',');
        // string to float distance
        float distancef = stof(distance);
        consignment c(stoi(cid), stoi(volume), sendadd, destadd, time, waittime);
        consg.push_back(c);
    }
    map<string, int> locationidletime;
    map<string, int>::iterator it;
    for(int i = 0; i < consg.size(); i++) {
        it = locationidletime.find(consg[i].sendadd);
        if(it == locationidletime.end()) {
            locationidletime[consg[i].sendadd] = 0;
        }
        it = locationidletime.find(consg[i].destadd);
        if(it == locationidletime.end()) {
            locationidletime[consg[i].destadd] = 0;
        }
    }
    for(int i = 0; i < consg.size(); i++) {
        it = locationidletime.find(consg[i].sendadd);
        locationidletime[consg[i].sendadd] += consg[i].waittime;
        it = locationidletime.find(consg[i].destadd);
        locationidletime[consg[i].destadd] += consg[i].waittime;
    }
    cout<<"Location IDLE TIME\n";
    for(it = locationidletime.begin(); it != locationidletime.end(); it++) {
        string locv = it->first;
        int idletime = it->second;
        // print locv and idletime seconds in days and hours and minutes
        int idletimev = idletime;
        int idledays = idletimev/86400;
        int idlehours = idletimev%86400/3600;
        int idlemins = idletimev%3600/60;
        cout<<locv<<": "<<idledays<<" days "<<idlehours<<" hours "<<idlemins<<" minutes\n";
    }
    cout<<"\n";

}

void managestatistics() {
    
    cout<<"1. TRUCK STATUS\n";
    cout<<"2. TRUCK USAGE\n";
    cout<<"3. CONSIGNMENT STATUS\n";
    cout<<"4. IDLE TIME\n";
    int choice; cin >> choice;
    switch(choice) {
        case 1:
            truckstatus();
            break;
        case 2:
            truckusage();
            break;
        case 3:
            consignmentstatus();
            break;
        case 4:
            idletime();
            break;
        default:
            cout<<"Invalid choice\n";
            break;
    }
    
}

void managermenu(int choice) {
    switch(choice) {
            case 1: {
                addtruck();
                break;
            }
            case 2: {
                removetruck();
                break;
            }
            case 3: {
                printalltrucks();
                break;
            }
            case 4: {
                break;
            }
            case 5: {
                return;
            }
            case 6: {
                managestatistics();
                break;
            }
            default: {
                cout<<"Invalid choice\n";
                return managermenu(managermenuinp());
            }
        }
} 

void clerkmenu(int choice) {
    switch(choice) {
            case 1: {
                registerconsignment();
                break;
            }
            case 2: {
                viewconsignment();
                break;
            }
            case 3: {
                break;
            }
            case 4: {
                return;
            }
            default: {
                cout<<"Invalid choice\n";
                return clerkmenu(clerkmenuinp());
            }
        }
} 

int main() 
{
    loadinfo();
    ////////////    TESTING    ////////////
    cout<<"-----------------TESTING STARTS------------------\n";
    for(auto it: citydistance) {
        cout<<it.first<<" "<<it.second.first<<" "<<it.second.second<<"\n";
    }
    cout<<"-----------------TESTING ENDS------------------\n";
    ////////////    TESTING    ////////////
    int cno=0; // cno = 0 means login page() auth
    while(cno!=3) {
        if(cno == 0) cno = loginmenuinp();
        if(cno == 1) {
            int choice = managermenuinp();
            managermenu(choice);
            if(choice == 4) {cno = 0;};
            if(choice == 5) {saveinfo(); return 0;}
        }
        else if(cno == 2) {
            int choice = clerkmenuinp();
            clerkmenu(choice);
            if(choice == 3) {cno = 0;};
            if(choice == 4) {saveinfo(); return 0;}
        }
        else if(cno == 3) {
            saveinfo();
            return 0;
        }
        else {
            cout<<"Invalid choice\n";
        }
    }
    
    saveinfo();

    return 0;
}

// LAST MODIFIED: 20/04/2022 02:43 AM