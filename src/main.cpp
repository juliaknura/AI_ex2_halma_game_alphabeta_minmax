#include "GameStructures.hpp"
#include "NodeCounter.hpp"
#include "Algorithms.hpp"
#include "Reader.hpp"
#include "Stopwatch.hpp"
#include "Strategies.hpp"
#include <limits>

extern int Counter::value;

int main()
{
    GameState state = loadStateFromFile();
    Strategy s1 = strategy2;
    Strategy s2 = strategy5;
    Strategy s = s1;
    int nodesVisited1{}, nodesVisited2{};
    std::uint64_t time1{}, time2{}, curr{};
    stopwatch::Stopwatch st;


    while(true)
    {
        Counter::value = 0;
        s = state.maxingPlayer == Pawn::First ? s1 : s2;

        st.start();
        auto [value,nextStateOpt] = alphabeta(state,true,2,2,std::numeric_limits<double>::lowest(),std::numeric_limits<double>::max(),s);
        curr = st.elapsed();

        if(state.maxingPlayer == Pawn::First)
        {
            nodesVisited1 += Counter::value;
            // std::cout<<nodesVisited1<<std::endl;
            time1 += curr;
            // std::cout<<time1<<std::endl;
        } 
        else
        {
            nodesVisited2 += Counter::value;
            // std::cout<<nodesVisited2<<std::endl;
            time2 += curr;
            // std::cout<<time2<<std::endl;
        }

        if(not nextStateOpt.has_value())
        {
            std::cout<<"Error: no child chosen"<<std::endl;
            return EXIT_FAILURE;
        }
        auto& nextState = nextStateOpt.value();
        nextState.board.print();
        // std::cout<<"solution value: "<<value<<std::endl;
        // std::cout<<"Visited nodes so far: "<<Counter::value<<std::endl;
        if(nextState.gameWonBy() != Pawn::None)
        {
            auto winner = nextState.gameWonBy();
            std::cout<<"Winner: "<<winner<<std::endl;
            std::cout<<"Loser score: "<<nextState.loserScore(winner)<<std::endl;
            std::cout<<"Player1: nodes visited: "<<nodesVisited1<<", time elapsed: "<<time1<<std::endl;
            std::cout<<"Player2: nodes visited: "<<nodesVisited2<<", time elapsed: "<<time2<<std::endl;
            break;
        }
        state = nextState;
        state.children.clear();
        state.maxingPlayer = state.maxingPlayer == Pawn::First ? Pawn::Second : Pawn::First;
    }


}
