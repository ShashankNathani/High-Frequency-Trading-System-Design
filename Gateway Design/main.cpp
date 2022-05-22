
#include<vector>
#include <queue>
using namespace std;
priority_queue<int> pq;



int minimumTime(vector<int> ability, long processes) {
    for(int i=0;i<ability.size();i++){
        pq.push(ability[i]);
    }
    int ab ;
    int counter = 0;

    // int n = ability.size();
    // sort(ability.begin(), ability.end(), greater<int>());
    // int i = 0;
    while(processes > 0){
        ab = pq.top();
        pq.pop();
        processes = processes - ab;
        pq.push(floor(ab/2));
        counter = counter + 1;
        // ability[i] = floor(ability[i]/2);
        // sort(ability.begin(), ability.end(), greater<int>());
    }
    
    
    return counter;
    
}