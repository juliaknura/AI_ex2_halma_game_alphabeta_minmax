#include "GameStructures.hpp"
#include "NodeCounter.hpp"
#include "Algorithms.hpp"
#include "Reader.hpp"
#include <limits>
#include "Stopwatch.hpp"

extern int Counter::value;

int main()
{
    GameState state = loadStateFromIStream();
    stopwatch::Stopwatch s;
    double minmaxtime{}, alphabetatime{};
    int minmaxnodes{}, alphabetanodes{};

    // for(int i=0;i<100;i++)
    // {
        Counter::value= 0;
        state.children.clear();
        
        s.start();
        auto [value,nextStateOpt] = minimax(state,true,2,2,{1,0,0,0});
        nextStateOpt.value().board.print();
        minmaxtime += s.elapsed();
        minmaxnodes = Counter::value;

        Counter::value = 0;
        state.children.clear();

        s.start();
        auto [value2,nextStateOpt2] = alphabeta(state,true,2,2,std::numeric_limits<double>::lowest(),std::numeric_limits<double>::max(),{1,0,0,0});
        nextStateOpt2.value().board.print();
        alphabetatime += s.elapsed();
        alphabetanodes = Counter::value;
    // }

    // minmaxtime /= 100.0;
    // alphabetatime /= 100.0;
    // std::cout<<"Minmax: "<<std::endl;
    // std::cout<<"Avg time in ms: "<<minmaxtime<<std::endl;
    // std::cout<<"Nodes visited: "<<minmaxnodes<<std::endl;
    // std::cout<<"Alphabeta: "<<std::endl;
    // std::cout<<"Avg time in ms: "<<alphabetatime<<std::endl;
    // std::cout<<"Nodes visited: "<<alphabetanodes<<std::endl;

    return 0;
}