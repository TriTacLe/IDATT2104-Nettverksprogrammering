#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <cmath>
#include <mutex>
#include <algorithm>

using namespace std;

// #define N_THREADS 10

mutex mtx;

bool argsValidation(int numb1, int numb2){
  if (numb1 > numb2) {
    cout << "numb1 cannot be > numb2" << endl;
    return false;
  }

  if (numb1 < 0 || numb2 < 0){
    cout << "Input numbs cannot be < 0" << endl;;
    return false;
  }

  

  return true;
}

// crit value (prime) passed as reference
void findPrime(vector<int>& prime, int numb1, int numb2){
  if (!argsValidation(numb1, numb2)){
    cout << "No primes" << endl;
    return;
  }

  if (numb2 < 2){
    cout << "No primes" << endl;
    return; 
  }

  bool check = true;
  
  for (int i = numb1; i <= numb2; i++) {
    check = true;
    if (i == 2){
      check = true;
    } else if(i%2 == 0){
      check = false;
    } else{
      // Skip even because odd/even (int) is always float
      for(int j = 3; j < sqrt(i);j+=2){ 
        if((i % j) == 0){
          check = false;
          break;
        }
      }
    }
    if(check){
      lock_guard<mutex> guard(mtx);
      prime.push_back(i);
    }
  }
  return;
}

// Oppgave: Finn alle primtall mellom to gitte tall ved hjelp av et gitt antall tråder.
int main(int argc, char* argv[]) {
  if (argc!=4){
    cout << "argc " << argc << " != 4. Format: ./progName numb1 numb2 nthreads" << endl;
    return 0; 
  }

  int numb1 = 0;
  int numb2 = 2;

  numb1 = stoi(argv[1]);
  numb2 = stoi(argv[2]);

  int N_THREADS = stoi(argv[3]);

  // Create threads
  std::vector<std::thread> threads;

  vector<int> primes;  

  //Divide work (KRAV: Pass på at de ulike trådene får omtrent like mye arbeid)
 
  if(N_THREADS > (numb2 - numb1)){
    cout << "# threads cannot be more than the intervall. Gives unnessary overhead" << endl;
    return 0;
  }

  int range = round((numb2-numb1)/N_THREADS);


  // range = (range == 0) ? 1 : range;
  

  for (int i = 0; i<N_THREADS;i++){ 
    // Must use ref() because thread copies. If not use ref() thread makes a new copy rather than modifying the original 
    int range1 = numb1 + (range * i);
    int range2 = range1 + range-1;

    cout << range1 << endl;

    range1 = (range1 < numb1) ? numb1 : range1;
    range2 = (range2 > numb2) ? numb2 : range2;

    threads.emplace_back(findPrime, ref(primes), range1, range2);

    // DEBUG
    // cout << "thread: " << i << "created " << range1 << " " << range2 << endl; 
  }
 
  for (auto& thread : threads){
    thread.join();
  }

  cout << "\nALl threads completed" << endl;

  // KRAV: Skriv til slutt ut en sortert liste av alle primtall som er funnet
  sort(primes.begin(), primes.end());

  cout << "Primes:" << endl;
  
  for(int prime : primes) {
    cout << prime << " ";
  }
  cout << endl;

  return 0;
}
