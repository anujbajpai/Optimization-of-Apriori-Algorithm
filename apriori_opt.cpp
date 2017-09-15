#include <map>
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
# include <chrono>
#include <set>

#define FOR(A, B, C) for(int A = B; A < C; A++)
#define EFOR(A, B, C) for(int A = B; A <= C; A++)

using namespace std;

typedef vector<bool> VB;
typedef vector<VB> VVB;
typedef pair<int,int> PI;
typedef vector<int> VI;
typedef vector<lint> VLI;
typedef vector<PI> VPI;
typedef vector<VI> VVI;
typedef map<int,int> MP;
typedef map<VI,int> MV;
typedef map<VI,VI> MVV;

VVI input(int &db_len, int &min_sup, int &max_item) {
    printf("\nEnter Number of Transactions =>\n");
    cin>>db_len;
    printf("\nEnter Transactions =>\n");
    VVI db;
    FOR(i, 0, db_len) {
        VI temp_vec;
        VI::iterator it;
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
    return db;
}

VVB construct_matrix(VVI db, int db_len, int max_item, VI &sot_store) {
    VVB bool_db(db_len, VB (max_item+1));
    FOR(i, 0, db_len) {
        int sot = 0;
        FOR(j, 0, db[i].size()) {
            bool_db[i][db[i][j]-1] = true;
            sot++;
        }
        bool_db[i][max_item] = 1;
        sot_store.push_back(sot);
    }
    printf("\nConversion of Input into Matrix\n\n");
    FOR(i, 0, max_item) {
        cout<<i+1 <<" ";
    }
    cout<< "Flag"<<endl;
    EFOR(i, 0, max_item) {
        cout<<" - ";
    }
    cout<<endl<<endl;
    FOR(i, 0, db_len) {
        EFOR(j, 0, max_item) {
            cout<<bool_db[i][j]<<" ";
        }
        cout<<endl;
    }
    return bool_db;
}

void generate_Lk(map<vector<int>,int> &Ck, VVB &bool_db, int db_len, int min_sup,int max_item, MVV &db_ck ) {
    MV::iterator it = Ck.begin();
    while(it != Ck.end()) {
        if(it->second < min_sup) {
            vector<int> temp = it->first;
            FOR(i, 0, db_ck[temp].size())
                bool_db[db_ck[temp][i]][max_item] = false;

            Ck.erase(it++);
        }
        else
            it++;
    }
}

void subset(VVB &bool_db, MV &Ck, int start, int last, int index, int k, VI data, int transaction_id, MVV &db_ck, int max_item)
{
    if (index == k) {
        MV::iterator Ck_itr;
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
void candidate_generation(VVB &bool_db, MV Ck, MVV db_ck, int max_item, int db_len, VI sot_store, int min_sup) {
    int k = 0;
    while(1) {
        int flag = 0;
        k++;
        cout<<endl<<endl<<endl<<endl<<endl<< "k => "<< k <<" Iteration"<<endl;
        FOR(transaction_id, 0, db_len) {
            if(bool_db[transaction_id][max_item] == false)
                continue;
            VI data(k);
            subset(bool_db, Ck, 0, max_item - 1, 0, k, data, transaction_id, db_ck, max_item);
            flag = 1;
        }
        if(!flag)
            break;
        cout<<"\nCandidate Generation =>"<<endl;
        for(MV::iterator it = Ck.begin(); it != Ck.end(); it++) {
            for(int i = 0; i < it->first.size(); i++)
              cout<<it->first[i]<<" ";
             cout <<" count => "<<it->second<<endl;
        }
        cout<<endl;
        generate_Lk(Ck, bool_db, db_len, min_sup, max_item, db_ck);
        cout<<"\nCandidates After Pruning =>"<<endl<<endl;
        for(MV::iterator it = Ck.begin(); it != Ck.end(); it++) {
            for(int i = 0; i < it->first.size(); i++)
              cout<<it->first[i]<<" ";
            cout <<" count => "<<it->second<<endl;
        }
        cout<<endl;
        FOR(i, 0, db_len) {
            if(sot_store[i] <= k)
                bool_db[i][max_item] = false;
        }
        cout<<"Bool After L"<<k<<endl<<endl;
        FOR(i, 0, max_item)
            cout<<i+1 <<" ";
        cout<< "Flag"<<endl;
        EFOR(i, 0, max_item)
            cout<<" - ";
        cout<<endl;
        FOR(i, 0, db_len) {
            EFOR(j, 0, max_item)
                cout<<bool_db[i][j]<<" ";
            cout<<endl;
        }
        cout<<endl;
        MV::iterator it = Ck.begin();
        while(it != Ck.end()) {
            if(it->first.size() <= k)
                Ck.erase(it++);
            else
                it++;
        }

    }
        cout<<"Itemset - Transaction Table"<<endl<<endl;
        for(MVV::iterator t = db_ck.begin(); t != db_ck.end(); t++) {
            cout<<"( ";
            FOR(i, 0, t->first.size())
                cout<<t->first[i]<<" ";
            cout<<") lies in transaction => ";
            FOR(j, 0, t->second.size())
                cout<<t->second[j]+1<<" ";
            cout<<endl;
        }
}

void apriori(VVI db, int db_len, int min_sup, int max_item, VI sot_store) {
    VVB bool_db;
    bool_db = construct_matrix(db, db_len, max_item, sot_store);
    MV Ck;
    MVV db_ck;
    candidate_generation(bool_db, Ck, db_ck, max_item, db_len, sot_store, min_sup);

}

int main() {
    int db_len, min_sup, max_item = 0;
    VVI db;
    VI sot_store;
    db = input(db_len, min_sup, max_item);
    auto start = std::chrono::high_resolution_clock::now();
    apriori(db,db_len, min_sup, max_item, sot_store);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout<<"Run Time => "<<elapsed.count(); // 0.21
    return 0;
}
