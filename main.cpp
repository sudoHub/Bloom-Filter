/* 
 * File:   main.cpp
 * Author: Brandon West
 * Created on April 3, 2019, 1:37 PM
 * Purpose: Demonstrate the bloom filter using 9 hashing functions found at
 * http://www.partow.net. Program generates indexes from hashing a list of 
 * names.
 * List of names from names.txt found here:
 * https://github.com/dominictarr/random-name/blob/master/first-names.txt
 */

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

float blmFilt(int,int);
float rec(int);
unsigned int RSHash  (const std::string& str);
unsigned int JSHash  (const std::string& str);
unsigned int PJWHash (const std::string& str);
unsigned int ELFHash (const std::string& str);
unsigned int BKDRHash(const std::string& str);
unsigned int SDBMHash(const std::string& str);
unsigned int DJBHash (const std::string& str);
unsigned int DEKHash (const std::string& str);
unsigned int BPHash  (const std::string& str);

int main(int argc, char** argv) {

    srand(static_cast<unsigned int>(time(0)));
    int size = 5000;
    string query; //test string to see if element is present
    int *arr = new int[size];//bloom array with hashed names
    int *srch = new int[size];//bloom array for searched name

    int k,m,n,km,sum = 0;

    int srchSum = 0;
    ifstream namList;
    k = 9;      //number of hashes
    m = 50;    //number of names in list
    km = k*m;
    n = size;
    string *item = new string[n]; //array to hold list of names
    
    //initialize name array and hashed search array
    for(int i = 0;i < size;i++){
        arr[i] = 0;
        srch[i] = 0;
    }

    namList.open("names.txt");
    string name = "";
    int cnt = 0;
    
    //get names from file
    while(getline(namList,name)){
        item[cnt] = name;
        cnt++;
    }
    namList.close();

    //hash the names using 9 different hashes
    for(int i = 0;i < m;i++){
        
        arr[RSHash(item[i]) % size] = 1;
        arr[JSHash(item[i])% size] = 1;
        arr[PJWHash(item[i])% size] = 1;
        arr[ELFHash(item[i])% size] = 1;
        arr[BKDRHash(item[i])% size] = 1;
        arr[SDBMHash(item[i])% size] = 1;
        arr[DJBHash(item[i])% size] = 1;
        arr[DEKHash(item[i])% size] = 1;
        arr[BPHash(item[i]) % size] = 1;
    }
    //sum result from hashing
    for(int i =0;i < n;i++){
        sum += arr[i];
    }
    float c = 1-(1.0f/n);

    cout<<"Enter names to search for, then press enter."<<endl;
    cout<<"Type done when finished"<<endl;
    while(query != "done"){
        cin>>query;
    //Hash the query
        srch[RSHash(query) % size] = 1;
        srch[JSHash(query) % size] = 1;
        srch[PJWHash(query) % size] = 1;
        srch[ELFHash(query) % size] = 1;
        srch[BKDRHash(query) % size] = 1;
        srch[SDBMHash(query) % size] = 1;
        srch[DJBHash(query) % size] = 1;
        srch[DEKHash(query) % size] = 1;
        srch[RSHash(query) % size] = 1;

        //sum the hashed query
        for(int i = 0;i < size;i++){
            srchSum += srch[i];
        }

        int match = 0;  //number of matches

        //compare search with hashed names
        for(int i = 0;i < size;i++){
            if(srch[i]== 1 && arr[i]==1){
                match += 1;
            }
        }
        if(match < 7){
            cout<<query<<" probably not in there"<<endl;
        }
        else{
            cout<<query<< " most likely found"<<endl;
        }
        
        cout<<"Number of matched indexes: "<<match<<endl;
        cout<<"Actual sum: "<<sum<<endl;

        cout<<"Bloom Filter: "<<blmFilt(km,n)<<endl;
        cout<<"Theoretical sum "<<n*(1-pow(c,k*m))<<endl;   
        cout<<"Probability "<<1-pow(c,k*n)<<endl;
    }
    delete [] item;
    delete [] srch;
    
    return 0;
}
/*
 * Bloom Filter Function: 
 */
float blmFilt(int km,int n){
    if(km == 0)  return 0;
    if(km == 1) return 1;
    
    float c = 1 - (1.0f/n);

    return c*blmFilt(km-1,n)+1.0f;
}

unsigned int RSHash(const std::string& str)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = hash * a + str[i];
      a    = a * b;
   }

   return hash;
}
/* End Of RS Hash Function */


unsigned int JSHash(const std::string& str)
{
   unsigned int hash = 1315423911;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash ^= ((hash << 5) + str[i] + (hash >> 2));
   }

   return hash;
}
/* End Of JS Hash Function */


unsigned int PJWHash(const std::string& str)
{
   unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   unsigned int HighBits          = (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
   unsigned int hash              = 0;
   unsigned int test              = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash << OneEighth) + str[i];

      if((test = hash & HighBits)  != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash;
}
/* End Of  P. J. Weinberger Hash Function */


unsigned int ELFHash(const std::string& str)
{
   unsigned int hash = 0;
   unsigned int x    = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash << 4) + str[i];
      if((x = hash & 0xF0000000L) != 0)
      {
         hash ^= (x >> 24);
      }
      hash &= ~x;
   }

   return hash;
}
/* End Of ELF Hash Function */


unsigned int BKDRHash(const std::string& str)
{
   unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = (hash * seed) + str[i];
   }

   return hash;
}
/* End Of BKDR Hash Function */


unsigned int SDBMHash(const std::string& str)
{
   unsigned int hash = 0;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}
/* End Of SDBM Hash Function */


unsigned int DJBHash(const std::string& str)
{
   unsigned int hash = 5381;

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = ((hash << 5) + hash) + str[i];
   }

   return hash;
}
/* End Of DJB Hash Function */


unsigned int DEKHash(const std::string& str)
{
   unsigned int hash = static_cast<unsigned int>(str.length());

   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
   }

   return hash;
}
/* End Of DEK Hash Function */


unsigned int BPHash(const std::string& str)
{
   unsigned int hash = 0;
   for(std::size_t i = 0; i < str.length(); i++)
   {
      hash = hash << 7 ^ str[i];
   }

   return hash;
}
/* End Of BP Hash Function */
