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
//////////////////////////////////////////////////

vector<truck> trucks;

/////////////////////////////////////////////////
int managermenuinp() {
    cout<<"\n\n1. Add truck\n2. Remove truck\n3. Print all trucksv\n4. BACK\n5. EXIT\n";
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

void loadinfo() {
    fstream truckfile;
    truckfile.open("db/trucksdb.txt", ios::in);
    if(!truckfile) {
        cout<<"File not found\n";
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
}

void saveinfo() {
    fstream truckfile;
    truckfile.open("db/trucksdb.txt", ios::out);
    if(!truckfile) {
        cout<<"File not found\n";
        return;
    }
    for(int i = 0; i < trucks.size(); i++) {
        truckfile<<trucks[i].id<<","<<trucks[i].capacity<<","<<trucks[i].speed<<","<<trucks[i].time<<"\n";
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
                //goback(); //TO DO TO-IMPLEMENT
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

int main() 
{
    loadinfo();
    int userno = LOGINPAGEdis();
    if(userno == 1) {
        int choice = managermenuinp();
        managermenu(choice);
        if(choice == 4) break;
        if(choice == 5) {saveinfo(); return 0;}
    }
    else if(userno == 2) {
        int choice = clerkmenuinp();
        managermenu(choice);
        if(choice == 4) break;
        if(choice == 5) {saveinfo(); return 0;}
    }
    else {
        cout<<"Invalid user\n";
        return 0;
    }

    }

    cout<<"saving updates\n";
    saveinfo();
    cout<<"Saved info\n";
    cout<<"Exiting\n";
    cout<<"logging out\n";

    return 0;
}