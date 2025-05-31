#include"terminal.h"

#include <iostream>
#include <string>
#include <chrono>


int main(int argc,char* argv[]){
    terminal term;
    std::string line;
    std::cout<<term.get_prompt();
    while(std::getline(std::cin,line)){
         
        if(line.empty()) {
            std::cout << term.get_prompt();
            continue;
        }
            try{
                if(term.find_command(line)){
                    auto start = std::chrono::high_resolution_clock::now();
                    term.run_command(line);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    std::cout << "Command executed in " << duration.count() << " microseconds." << std::endl;
                }
            }catch(const std::exception& e){
                std::cerr<<"Error: "<<e.what()<<std::endl;
            }
        std::cout<<term.get_prompt();
    }
}