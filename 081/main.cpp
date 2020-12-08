#include <iostream>
#include <algorithm>
#include <vector>
#include <from_string.h>
#include <unistd.h> // STDIN_FILENO
#include <chrono>


struct instruction {
   std::string instr; // mnemonic, really
   std::string arg;
};
using program = std::vector<instruction>;


// load the program
program load()
{
   program rv;

   for(;;)
   {
      char ch;                     // temp for input
      // read in the mnemonic until space is reached
      std::string instr;
      while(read(STDIN_FILENO, &ch, 1) > 0)
      {
         if(ch == ' ')
            break;
         instr.push_back(ch);
      }
      // read in the argument until EOL or EOF
      std::string arg;
      while(read(STDIN_FILENO, &ch, 1) > 0)
      {
         if(ch == '\n')
            break;
         arg.push_back(ch);
      }
      // if EOF was reached, then instr is empty, the program is loaded
      if(instr.empty())
         break;
      rv.emplace_back(instruction{instr,arg});
   }
   return rv;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   // get the program
   program prog = load();

   // run destructively, clear the instruction after it's executed
   long acc=0;                  // accumulator
   long pc=0;                   // program counter
   for(;;)
   {
      // sanity check location
      if((pc < 0) || (static_cast<size_t>(pc) >= prog.size()))
      {
         std::cerr << "program counter out of range error: pc: " << pc << " program length: " << prog.size() << std::endl;
         break;
      }
      // handle this instruction
      long jmp_val=1;
      instruction& l = prog[pc];
      if(l.instr == "nop")      // no operation: does nothing
      {
         ;
      }
      else if(l.instr == "jmp") // jump: add arg to PC and continue
      {
         sb::string::from(l.arg,jmp_val);
      }
      else if(l.instr == "acc") // add and continue: increment accumlator by argument
      {
         long val=0;
         sb::string::from(l.arg,val);
         acc += val;
      }
      else if(l.instr.empty())  // no instrunction means we are in the infinite loop, break out
         break;
      else                      // report unknown instruction
         std::cerr << "unknown instruction: " << l.instr << " " << l.arg << std::endl;

      pc += jmp_val;   // increment the program counter
      l.instr.clear(); // clear the instruction so we don't repeat the operation
   }

   std::cout << acc << std::endl;


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << std::endl;
   return 0;
}
