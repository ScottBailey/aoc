#include <iostream>
#include <algorithm>
#include <vector>
#include <sb/string/from.h>
#include <unistd.h> // STDIN_FILENO
#include <chrono>


struct instruction {
   std::string instr; // mnemonic, really
   std::string arg;
};
using program = std::vector<instruction>;
std::ostream& operator<<(std::ostream& os, const instruction& o) { os << o.instr << " " << o.arg; return os; }

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


bool execute(program& prog, long& acc)
{
   // run destructively, clear the instruction after it's executed
   acc=0; // accumulator
   long pc=0;                   // program counter
   for(;;)
   {
      if(static_cast<size_t>(pc) == prog.size()) // successful completion
         return true;
      // sanity check location
      if((pc < 0) || (static_cast<size_t>(pc) > prog.size()))
      {
         std::cerr << "program counter out of range error: pc: " << pc << " program length: " << prog.size() << std::endl;
         return false;
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
         return false;
      else                      // report unknown instruction
         std::cerr << "unknown instruction: " << l.instr << " " << l.arg << std::endl;

      pc += jmp_val;   // increment the program counter
      l.instr.clear(); // clear the instruction so we don't repeat the operation
   }
   // unreachable
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   // get the program
   const program prog = load();

   long acc=0;
   // brute force
   for(size_t i=0; i < prog.size(); ++i)
   {
      if(prog[i].instr == "acc")
         continue;
      acc = 0;
      program p2 = prog;
      if(p2[i].instr == "jmp")
         p2[i].instr = "nop";
      else
         p2[i].instr = "jmp";
      if(execute(p2,acc))
      {
         std::cout << "Switched line: " << i << "\nResult: " << acc << "\n";
         break;
      }
   }

      //std::cout << acc << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << std::endl;
   return 0;
}
