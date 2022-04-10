#include<bits/stdc++.h>
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
    truck(int i, int c, int s, string t="") {
        id = i;
        capacity = c;
        speed = s;
        if(t=="") {
            time = timenow();
        } else {
            time = stol(t);
        }
    }
    void displaytruckinfo() {
        cout<<setw(setwsize);cout<<"--------"<<"---"<<setw(setwsize)<<"\n";
        cout<<setw(setwsize);
        cout<<"| Truck "<<id<<" |";cout<<endl;
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}    cout<<"\n";
        //cout<<setw(setwsize);cout<<"--------"<<"---"<<setw(setwsize)<<"\n";
        cout<<"Capacity: "<<capacity<<" Speed: "<<speed<<" Time: "<< ctime(&time);
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}    cout<<"\n";
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
    consignment(int i, int vol, string dadd, string sadd) {
        cid = i;
        volume = vol;
        destadd = dadd;
        transform(destadd.begin(), destadd.end(), destadd.begin(), ::tolower);
        sendadd = sadd;
        transform(sendadd.begin(), sendadd.end(), sendadd.begin(), ::tolower);
        distance = (int)distancebetcity(citydistance[sendadd].first,citydistance[sendadd].second, citydistance[destadd].first,citydistance[destadd].second);
        charge = (int)distance*volume;
        billoc = "db/bill/bill"+to_string(cid)+".txt";
        generateDispatchSlip();
    }
    void displayconsignmentinfo() {
        cout<<setw(setwsize);cout<<"--------"<<"---"<<setw(setwsize)<<"\n";
        cout<<setw(setwsize);
        cout<<"| Consignment "<<cid<<" |";cout<<endl;
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}    cout<<"\n";
        cout<<"Volume: "<<volume<<" \nDestination Address: "<<destadd<<"\n";
        cout<<"Source Address: "<<sendadd<<"\n";
        cout<<"Charge: Rs."<<charge<<" \n";
        for(int tempi = 0; tempi < dashsize; tempi++) {cout<<"-";}    cout<<"\n";
    }
    

};

//////////////////////////////////////////////////

vector<truck> trucks;
vector<consignment> consignments;
map<string,string> mgrpsswd;
map<string,string> clkpsswd;


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
    cout<<"\n\n1. Add truck\n2. Remove truck\n3. Print all trucksv\n4. LOG-OUT!!!\n5. EXIT\n";
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
    int id, capacity, speed; int time;
    cout<<"Enter id, capacity, speed\n";
    cin >> id >> capacity >> speed;
    trucks.push_back(truck(id, capacity, speed));
}



void removetruck() {
    int id;
    cout<<"Enter id of truck to be removed\n";
    cin >> id;
    for(int i = 0; i < trucks.size(); i++) {
        if(trucks[i].id == id) {
            trucks.erase(trucks.begin() + i);
            break;
        }
    }
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
        stringstream ss(line);
        string id, capacity, speed, time;
        getline(ss, id, ',');
        getline(ss, capacity, ',');
        getline(ss, speed, ',');
        getline(ss, time);
        trucks.push_back(truck(stoi(id), stoi(capacity), stoi(speed), time));
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
        string cid, vol, dest, source, charge, distance;
        getline(ss3, cid, ',');
        getline(ss3, vol, ',');
        getline(ss3, dest, ',');
        getline(ss3, source, ',');
        getline(ss3, charge, ',');
        getline(ss3, distance);
        consignments.push_back(consignment(stoi(cid), stoi(vol), dest, source));
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
        truckfile<<trucks[i].id<<","<<trucks[i].capacity<<","<<trucks[i].speed<<","<<trucks[i].time<<"\n";
    }

    fstream consignmentfile;
    consignmentfile.open("db/consignmentdb.txt", ios::out);
    if(!consignmentfile) {
        cout<<"File not found consignmentdb 2\n";
        return;
    }
    for(int i = 0; i < consignments.size(); i++) {
        consignmentfile<<consignments[i].cid<<","<<consignments[i].volume<<","<<consignments[i].destadd<<","<<consignments[i].sendadd<<","<<consignments[i].charge<<","<<consignments[i].distance<<"\n";
    }


}

void registerconsignment() {
    int id, vol; string source, destination;
    cout<<"Enter id, vol, source, destination\n";
    cin >> id >> vol >> source >> destination;
    consignments.push_back(consignment(id, vol, source, destination));
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

// LAST MODIFIED: 10/04/2022 15:36