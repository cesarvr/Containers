#include <iostream>

using namespace std;

void execute(char *str) {

}

int main(int argc, char *argv[]) {
  
  cout << "Cesar's Container " << endl; 

  for(int i=0; i < argc; i++ ){
    cout << "arg " << argv[i] << " pos" <<  i << endl;
  }

  if(string(argv[1]) == "run") {
    cout << "Running...\n" << endl;


  }




  return 0; 
}
