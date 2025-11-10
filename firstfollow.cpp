#include <iostream>
#include<bits/stdc++.h>

using namespace std;

int main()
{
    char start;
    set<char>terminals,nonterminals;
    map<char, set<string>>productions;
    
    cout<<"Enter The Number Of Prouctions:-"<<endl;
    int n;
    cin >> n;
    
    for(int i = 0 ; i < n ; i++){
       string s;
       cin >> s;
       if(i == 0)start = s[0];
       nonterminals.insert(s[0]);
       string temp;
       for(auto j = 3 ; j < s.size() ; j++){
           if(s[j] =='-'|| s[j] == '>')continue;
           else if(s[j] =='|'){
               productions[s[0]].insert(temp);
               temp = "";
               continue;
           }
           temp += s[j];
           terminals.insert(s[j]);
       }
       if(!temp.empty())productions[s[0]].insert(temp);
    }
    
    
    cout << "productions" << endl;
    for(auto it : productions){
        for(auto it1 : it.second)
        cout << it.first << "->" << it1 << endl;
    }
    
    for(auto it : nonterminals){
        if(terminals.count(it) == 1)terminals.erase(it);
    }
    
    cout << "terminals"<<endl;
    for(auto it : terminals){
        cout << it << endl;
    }
    
    cout << "nonterminals"<<endl;
    for(auto it : nonterminals){
        cout << it << endl;
    }
    
    
    map<char,set<char>>firstset;
    
    for(int i = 0 ; i < 100 ; i++){
        bool changed = false;
        for(auto rule : productions){
            char A = rule.first;
            for(auto s : rule.second){
                bool alleps = true;
                for(auto ch : s){
                    if(nonterminals.count(ch) == 0){
                        changed = true;
                        firstset[A].insert(ch);
                        alleps = false;
                        break;
                    }
                    for(auto it : firstset[ch]){
                        if(it!='@' && firstset[A].count(it)==0)changed = true;
                        if(it!='@')firstset[A].insert(it) ;
                    }
                    if(firstset[ch].count('@')==0){
                        alleps = false;
                        break;
                    }
                }
                if(alleps)firstset[A].insert('@');
            }
        }
        if(!changed)break;
    }
    
    
    cout << "First Sets" << endl;
    
    for(auto it : firstset){
        cout << "FirstSet(" << it.first << ") :- { ";
        for(auto it1 : it.second) cout << it1 <<' ';
        cout << '}' << endl;
    }
    
    map<char,set<char>>followset;
    followset[start].insert('$');
    
    for(int i = 0 ; i < 100 ; i++){
        bool changed = false;
        for(auto prods : productions){
            char A = prods.first;
            for(auto prod : prods.second){
                for(int j = 0 ; j < prod.size() ; j++){
                    if(nonterminals.count(prod[j])==0)continue;
                    string sub = prod.substr(j+1);
                    bool alleps = true;
                    for(auto ch : sub){
                        if(nonterminals.count(ch)==0){
                            followset[prod[j]].insert(ch);
                            changed = true;
                            alleps = false;
                            break;
                        }
                        for(auto it : firstset[ch]){
                            if(it!='@'&&followset[prod[j]].count(it)==0)changed = true;
                            if(it!='@')followset[prod[j]].insert(it);
                        }
                        if(firstset[ch].count('@')==0){
                            alleps = false;
                            break;
                        }
                    }
                    if(alleps){
                         for(auto it : followset[A]){
                            if(followset[prod[j]].count(it)==0)changed = true;
                            followset[prod[j]].insert(it);
                        }
                    }
                }
            }
        }
        if(!changed)break;
    }
    
    
    cout << "Follow Sets" << endl;
    for(auto it : followset){
        cout << "FollowSet(" << it.first << ") :- { ";
        for(auto it1 : it.second) cout << it1 <<' ';
        cout << '}' << endl;
    }
    return 0;
}