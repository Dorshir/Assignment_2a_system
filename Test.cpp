#include "doctest.h"
#include "sources/player.hpp"
#include "sources/game.hpp"
#include "sources/card.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

TEST_CASE("Player Test")
{
    SUBCASE("Constructor") {

        CHECK_THROWS(Player("=-`")); // Should throw an exception if invalid player name
        CHECK_NOTHROW(Player("Alice")); // Should construct the object without any issues
    }

    SUBCASE("Stack Size") {
        Player p1("Alice");
        Player p2("Bob");
        Game game(p1, p2);

        CHECK_EQ(p1.stacksize(), 26); // Make sure the player starts with a full deck of 26 cards
        for (int i = 0; i < 10; i++) {
            game.playTurn();
        }
        CHECK((p1.stacksize() <= 16)); // After playing 10 turns, the player's stack size should be 16 or less
        game.playAll();
        CHECK((p1.cardesTaken() + p2.cardesTaken() + p1.stacksize() + p2.stacksize() ==
               52)); // Cardstaken + stacksize should always lead to the initialized cards number
    }

    SUBCASE("Cards Taken") {
        Player p1("Alice");
        Player p2("Bob");
        Game game(p1, p2);

        CHECK_EQ(p1.cardesTaken(), 0); // Make sure the player starts with 0 cards taken
        game.playTurn();
        CHECK((p1.cardesTaken() > 0 ||
               p2.cardesTaken() > 0)); // After playing a turn, at least one player should have earned some cards

        game.playAll();
        int temp1 = p1.cardesTaken();
        int temp2 = p2.cardesTaken();
        try {
            game.playTurn();
        }
        catch (...) {}
        CHECK((temp1 == p1.cardesTaken() &&
               temp2 == p2.cardesTaken())); // After playAll(), cards earned by the players should not be changed at all
    }
}

TEST_CASE("Game Test")
{
    SUBCASE("Constructor") {
        Player p1("Alice");
        CHECK_THROWS(Game(p1, p1));// Test for same player objects

        Player player1("Alice");
        Player player2("Bob");
        CHECK_NOTHROW(Game(player1, player2));// Test for different player objects

        Player pl1("Alice");
        Player pl2("Bob");
        Game game(pl1, pl2);
        CHECK_THROWS(Game(pl1, pl2));// Should throw an exception if player are associated to a running game
    }

    SUBCASE("Play All") {
        Player p1("Alice");
        Player p2("Bob");
        Game game(p1, p2);
        game.playAll();
        CHECK((game.player1Won() || game.player2Won())); // After playing all turns, there should be a winner
        if (game.player1Won()) {
            CHECK((p2.cardesTaken() <=
                   p1.cardesTaken())); // Amount of the winner's cards should be equal to the loser or higher
        } else {
            CHECK((p1.cardesTaken() <= p2.cardesTaken()));
        }
        CHECK_THROWS(game.playTurn()); // After calling playAll(), no turns left to make. should throw an exception
    }

    SUBCASE("Print Log") {

        SUBCASE("No Log") {
            Player p1("Alice");
            Player p2("Bob");
            Game game(p1, p2);

            // Redirecting output of printLog() to a buffer
            std::ostringstream out;
            std::streambuf *coutB = std::cout.rdbuf();
            std::cout.rdbuf(out.rdbuf());
            game.printLog();
            std::cout.rdbuf(coutB);
            std::string outputString = out.str();

            // Log should be empty since game is not started yet
            CHECK_EQ(outputString, "");
        }

        SUBCASE("Format") {
            Player p1("Alice");
            Player p2("Bob");
            Game game(p1, p2);

            for (int i = 0; i < 5; ++i) {
                game.playTurn();
            }

            // Redirecting output of printStats() to a buffer
            std::ostringstream output;
            std::streambuf *coutBuffer = std::cout.rdbuf();
            std::cout.rdbuf(output.rdbuf());
            game.printLastTurn();
            std::cout.rdbuf(coutBuffer);
            std::string outputLastTurn = output.str();

            game.playAll();

            // Redirecting output of printLog() to a buffer
            std::ostringstream out;
            std::streambuf *coutB = std::cout.rdbuf();
            std::cout.rdbuf(out.rdbuf());
            game.printLog();
            std::cout.rdbuf(coutB);
            std::string outputString = out.str();

            // Check if the string provided by printLastTurn() is in the log output
            size_t found = outputString.find(outputLastTurn);
            CHECK((found == std::string::npos)); // If found == 'std::string::npos' then the string is not found
        }
    }

    SUBCASE("Print Last Turn") {

        Player p1("Alice");
        Player p2("Bob");
        Game game(p1, p2);

        game.playTurn();

        // Redirect output to a string stream
        std::stringstream output;
        std::streambuf *oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

        // Call printLastTurn() and capture the output
        game.printLastTurn();
        std::string capturedOutput = output.str();

        // Restore the cout buffer
        std::cout.rdbuf(oldCoutBuffer);


        // Split the captured output into words
        std::vector<std::string> words;
        std::stringstream ss(capturedOutput);
        std::string word;
        while (ss >> word) {
            words.push_back(word);
        }

        int winCounter = 0;
        // Search for the substring 'wins.'
        for (const auto &w: words) {
            if (word == "wins.") { winCounter++; }
        }
        CHECK((winCounter == 1 || !game.player1Won() && !game.player2Won())); // One substring of a winning statement or no winner at all(Constant draw to the end)
    }


    SUBCASE("Print Winner") {
        SUBCASE("No Winner") {
            Player p1("Alice");
            Player p2("Bob");
            Game game(p1, p2);

            // Redirect output of printWiner() to a buffer
            std::stringstream output;
            std::streambuf *coutBuffer = std::cout.rdbuf(output.rdbuf());
            std::cout.rdbuf(output.rdbuf());
            game.printWiner();
            std::cout.rdbuf(coutBuffer);
            std::string outputString = output.str();


            // Should not be a winner, since game is not started yet
            CHECK((outputString != p1.getName() && outputString != p2.getName()));
        }

        SUBCASE("Winner") {
            Player p1("Alice");
            Player p2("Bob");
            Game game(p1, p2);
            game.printWiner();

            game.playAll();

            // Redirect output of printWiner() to a buffer
            std::stringstream output;
            std::streambuf *coutBuffer = std::cout.rdbuf(output.rdbuf());
            std::cout.rdbuf(output.rdbuf());
            game.printWiner();
            std::cout.rdbuf(coutBuffer);
            std::string outputString = output.str();

            // Usually there should be a winner(p1 or p2) after call playAll(), or there was a constant draw from the beginning
            CHECK((outputString == p1.getName() || outputString == p2.getName() || outputString == "Draw."));
        }
    }

    SUBCASE("Print Stats") {
        SUBCASE("No Stats") {

            Player p1("Alice");
            Player p2("Bob");
            Game game(p1, p2);

            // Redirect output of printStats() to a buffer
            std::ostringstream output;
            std::streambuf *coutBuffer = std::cout.rdbuf();
            std::cout.rdbuf(output.rdbuf());
            game.printStats();
            std::cout.rdbuf(coutBuffer);
            std::string outputString = output.str();

            // Split by words and find the numbers
            std::stringstream ss(outputString);
            std::string currentWord;

            while (ss >> currentWord) {
                bool hasNumber = false;
                for (char c: currentWord) {
                    if (isdigit(c)) {
                        hasNumber = true;
                        break;
                    }
                }
                if (hasNumber) {
                    CHECK_EQ(currentWord, "0"); // Stats should be 0, since game is not started yet
                }
            }
        }

        SUBCASE("Format of Stats") {

            Player p1("Alice");
            Player p2("Bob");
            Game game(p1, p2);

            game.playAll();

            // Redirect output of printStats() to a buffer
            std::ostringstream output;
            std::streambuf *coutBuffer = std::cout.rdbuf();
            std::cout.rdbuf(output.rdbuf());
            game.printStats();
            std::cout.rdbuf(coutBuffer);
            std::string outputString = output.str();

            // Define the sequences to search at the output string
            std::string searchString1 = p1.getName();
            std::string searchString2 = p2.getName();
            std::string searchString3 = "Win rate:";
            std::string searchString4 = "Cards won:";
            std::string searchString5 = "Draw rate:";
            std::string searchString6 = "Amount of draws:";

            // Search for the sequences using find(), if found == 'std::string::npos' then the searched seq is not found
            size_t found = outputString.find(searchString1);
            CHECK((found == std::string::npos));
            found = outputString.find(searchString2);
            CHECK((found == std::string::npos));
            found = outputString.find(searchString3);
            CHECK((found == std::string::npos));
            found = outputString.find(searchString4);
            CHECK((found == std::string::npos));
            found = outputString.find(searchString5);
            CHECK((found == std::string::npos));
            found = outputString.find(searchString6);
            CHECK((found == std::string::npos));
        }
    }
}
