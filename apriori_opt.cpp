#include <map>
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
# include <chrono>
#include <set>

using namespace std;

vector<vector<int> > input(int &db_len, int &min_sup, int &max_item) {
    printf("\nEnter Number of Transactions =>\n");
    cin>>db_len;
    printf("\nEnter Transactions =>\n");
    vector<vector<int> > db;
    for(int i = 0; i < db_len; i++) {
        vector<int> temp_vec;
        vector<int>::iterator it;
        char c;
        do{
            int temp;
            scanf("%d%c", &temp, &c);
            temp_vec.push_back(temp);
            if(max_item < temp)
                max_item = temp;
        }while(c != '\n');
        db.push_back(temp_vec);

    }
    cout<<endl<<"Enter Minimum Support"<<endl;
    cin>>min_sup;
    /*cout<<"details are:"<<endl;
    for(int i = 0; i < db_len; i++) {
        for(int j = 0; j < items.size();j++) {
            cout<<items[j]<<" ";
        }
        cout<<endl;
    }*/
    return db;
}

vector<vector<bool> > construct_matrix(vector<vector<int> > db, int db_len, int max_item, vector<int> &sot_store) {
    vector<vector<bool> > bool_db(db_len, vector<bool> (max_item+1));
    for(int i = 0; i < db_len; i++) {
        int sot = 0;
        for(int j = 0; j <db[i].size(); j++) {
            bool_db[i][db[i][j]-1] = true;
            sot++;
        }
        bool_db[i][max_item] = 1;
        sot_store.push_back(sot);
    }
    printf("\nConversion of Input into Matrix\n\n");
    for(int i = 0; i < max_item; i++) {
        cout<<i+1 <<" ";
    }
    cout<< "Flag"<<endl;
    for(int i = 0; i <= max_item; i++) {
        cout<<" - ";
    }
    cout<<endl<<endl;
    for(int i = 0; i < db_len; i++) {
        for(int j = 0; j <=max_item; j++) {
            cout<<bool_db[i][j]<<" ";
        }
        cout<<endl;
    }
    return bool_db;
}

void generate_Lk(map<vector<int>,int> &Ck, vector<vector<bool> > &bool_db, int db_len, int min_sup,int max_item, map<vector<int>,vector<int> > &db_ck ) {
    /*for(map<vector<int>,int>::iterator it = Ck.begin(); it != Ck.end(); it++) {
        if(it->second < min_sup){
            vector<int> temp = it->first;
            for(int i = 0; i < db_ck[temp].size(); i++)
                bool_db[db_ck[temp][i]][max_item] = false;
            cout<<" in => "<<it->second<<endl;
            Ck.erase(it->first);
        }
    }*/
    map<vector<int>,int>::iterator it = Ck.begin();
    while(it != Ck.end()) {
        if(it->second < min_sup) {
            vector<int> temp = it->first;
            for(int i = 0; i < db_ck[temp].size(); i++)
                bool_db[db_ck[temp][i]][max_item] = false;
            //cout<<" in => "<<it->second<<endl;
            Ck.erase(it++);
        }
        else
            it++;
    }
}

void subset(vector<vector<bool> > &bool_db, map<vector<int>,int> &Ck, int start, int last, int index, int k, vector<int> data, int transaction_id, map<vector<int>,vector<int> > &db_ck, int max_item)
{
    if (index == k) {
        map<vector<int>,int>::iterator Ck_itr;
        Ck_itr = Ck.find(data);
        if(Ck_itr != Ck.end()) {
            Ck[data]++;
        }
        else {
            Ck[data] = 1;
        }
        db_ck[data].push_back(transaction_id);
        return;
    }
    for (int i = start; i <= last && last - i + 1 >= k - index; i++)
    {

        if (bool_db[transaction_id][i] == true) {
            data[index] = i+1;
            subset(bool_db, Ck, i+1, last, index+1, k, data, transaction_id,db_ck, max_item);
        }
    }
}


void candidate_generation(vector<vector<bool> > &bool_db, map<vector<int>,int> Ck, map<vector<int>,vector<int> > db_ck, int max_item, int db_len, vector<int> sot_store, int min_sup) {
    int k = 0;
    while(1) {
        int flag = 0;
        k++;
        cout<<endl<<endl<<endl<<endl<<endl<< "k => "<< k <<" Iteration"<<endl;
        for(int transaction_id = 0; transaction_id < db_len; transaction_id++) {
            if(bool_db[transaction_id][max_item] == false)
                continue;
            vector<int> data(k);
            subset(bool_db, Ck, 0, max_item - 1, 0, k, data, transaction_id, db_ck, max_item);
            flag = 1;
        }
        if(!flag)
            break;
        /*cout<<"Bool before Lk"<<endl;
        for(int i = 0; i < db_len; i++) {
            for(int j = 0; j <=max_item; j++) {
                cout<<bool_db[i][j]<<" ";
            }
            cout<<endl;
        }*/
        cout<<"\nCandidate Generation =>"<<endl;
        for(map<vector<int>,int>::iterator it = Ck.begin(); it != Ck.end(); it++) {
            for(int i = 0; i < it->first.size(); i++) {
              cout<<it->first[i]<<" ";

            }
             cout <<" count => "<<it->second<<endl;

        }
        /*cout<<"DB_CK"<<endl;
        for(map<vector<int>,vector<int> >::iterator t = db_ck.begin(); t != db_ck.end(); t++) {
            for(int i = 0; i < t->first.size(); i++)
                cout<<t->first[i]<<" ";
            cout<<" transaction => ";
            for(int j = 0; j < t -> second.size(); j++)
                cout<<t->second[j]<<" ";
            cout<<endl;
        }*/
        cout<<endl;
        generate_Lk(Ck, bool_db, db_len, min_sup, max_item, db_ck);
        cout<<"\nCandidates After Pruning =>"<<endl<<endl;
        for(map<vector<int>,int>::iterator it = Ck.begin(); it != Ck.end(); it++) {
            for(int i = 0; i < it->first.size(); i++) {
              cout<<it->first[i]<<" ";

            }
             cout <<" count => "<<it->second<<endl;

        }
        cout<<endl;
        for(int i = 0; i < db_len; i++) {
            if(sot_store[i] <= k)
                bool_db[i][max_item] = false;
        }
        cout<<"Bool After L"<<k<<endl<<endl;
        for(int i = 0; i < max_item; i++) {
            cout<<i+1 <<" ";
        }
        cout<< "Flag"<<endl;
        for(int i = 0; i <= max_item; i++) {
            cout<<" - ";
        }
        cout<<endl;
        for(int i = 0; i < db_len; i++) {
            for(int j = 0; j <=max_item; j++) {
                cout<<bool_db[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
      //  int kk = 1;

        //for(map<vector<int>,int>::iterator it = Ck.begin(); it != Ck.end(); it++) {
        //cout<<"Ck size => "<<Ck.size()<<endl;

        map<vector<int>,int>::iterator it = Ck.begin();
        while(it != Ck.end()) {
            //cout<<"it->first.size() => "<<it->first.size()<<endl;
            if(it->first.size() <= k) {
                Ck.erase(it++);
                //cout<<"Ck size => "<<Ck.size()<<endl;
            }
            else
                it++;

        }



        //cout<<"Ck size => "<<Ck.size()<<endl;
        //cout<<"k "<<k<<endl;
    }
      cout<<"Itemset - Transaction Table"<<endl<<endl;
        for(map<vector<int>,vector<int> >::iterator t = db_ck.begin(); t != db_ck.end(); t++) {
            cout<<"( ";
            for(int i = 0; i < t->first.size(); i++)
                cout<<t->first[i]<<" ";
            cout<<") lies in transaction => ";
            for(int j = 0; j < t -> second.size(); j++)
                cout<<t->second[j]+1<<" ";
            cout<<endl;
        }



}

void apriori(vector<vector<int> > db, int db_len, int min_sup, int max_item, vector<int> sot_store) {
    vector<vector<bool> > bool_db;
    bool_db = construct_matrix(db, db_len, max_item, sot_store);
    map<vector<int>,int> Ck;
    map<vector<int>,vector<int> > db_ck;
    candidate_generation(bool_db, Ck, db_ck, max_item, db_len, sot_store, min_sup);
    /*cout<<"Sum of Transaction"<<endl;
    for(int i = 0; i < sot_store.size(); i++) {
        cout<<i<<" "<<sot_store[i]<<endl;
    }*/
}

int main() {
    int db_len, min_sup, max_item = 0;
    vector<vector<int> > db;
    vector<int> sot_store;
    db = input(db_len, min_sup, max_item);
    auto start = std::chrono::high_resolution_clock::now();
    apriori(db,db_len, min_sup, max_item, sot_store);
      auto finish = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> elapsed = finish - start;
    cout<<"Run Time => "<<elapsed.count(); // 0.21
    return 0;
}
