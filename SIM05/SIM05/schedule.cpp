#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
   vector<int> x, s; // the given parameters
   int n, temp, j=1;

   cout << "Enter number of days: "; cin >> n;

   cout << "Input x: ";
   for(int i=0; i<n; i++){   // input x
       cin >> temp;
       x.push_back(temp);
   }
   cout << "Input s: ";
   for(int i=0; i<n; i++){ // input s
       cin >> temp;
       s.push_back(temp);
   }

   vector<int> T(n);   // this is our to-be-optimized variable.

   T[0] = min(x[0], s[0]); // first day
   // MAIN Dynamic programming loop
   for(int i=1; i<n; i++){
       int a, b;
       if(i==1){
           a = T[0]+min(x[1], s[1]);
           b = min(x[1], s[0]);
           T[i] = max(a, b); // second day
           if(b>=a)   j=0;
           else       j+=1;
           continue;
       }
       a = T[i-1]+min(x[i], s[j]); // no reboot previous day
       b = T[i-2]+min(x[i], s[0]); // rebooted previous day

       if( b>= a )   j = 1;
       else       j += 1;

       T[i] = max(a, b);
   }

  
   for(int i=0; i<n; i++){
       cout << T[i] << " ";
   }
   cout << "\n" << "The optimal amount of data that can be processed is: " << T[n-1] << "\n";
}