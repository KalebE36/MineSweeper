#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include "Text.h"
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_LEADERBOARD_H
#define MINESWEEP_LEADERBOARD_H

using namespace std;

struct Leaderboard {
    string time;
    string user_name;
    vector<string> time_strings;
    vector<float> time_values;

    Leaderboard() {}

    Leaderboard(string& user_name) {
        this->user_name = user_name;
    }


    void readTextFile() {
        // Open the file "leaderboard.txt" for reading
        fstream stream("files/leaderboard.txt", ios_base::in);

        // Check if the file is successfully opened
        if (!stream.is_open()) {
            cerr << "Error opening file!" << endl;
            return;
        }

        string value;
        time_strings.clear();
        time_values.clear();


        while (getline(stream, value)) {
            stringstream ss(value);
            string timeStr, name;
            getline(ss, timeStr, ',');
            replace(timeStr.begin(), timeStr.end(), ':', '.');

            float time = stof(timeStr); // Convert time string to float
            getline(ss, name);

            // Store the entire time string and the time value separately
            time_strings.push_back(value);
            time_values.push_back(time);

            cout << "Time: " << time << ", Name: " << name << endl;
        }

        stream.close();
    }

    void writeTextFile() {
        fstream stream("files/leaderboard.txt", ios_base::out);
    }

    void displayLeaderWindow(int& num_cols, int& num_rows, bool& leaderboard_check, int& game_state) {
        sf::RenderWindow leaderboardWindow(sf::VideoMode((num_cols * 16), ((num_rows * 16) + 50)), "Leaderboard Window",sf::Style::Close);
        Text leaderboard_text(20,  "files/font.ttf", "LEADERBOARD");
        leaderboard_text.new_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
        leaderboard_text.setText(leaderboard_text.new_text, ((num_cols*16)/2.0f), ((((num_rows*16) + 100))/2.0f) - 120);
        readTextFile();

        while (leaderboardWindow.isOpen()) {
            sf::Event event;

            while (leaderboardWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    leaderboardWindow.close();
                    leaderboard_check = false;
                }
            }
            leaderboardWindow.clear(sf::Color::Blue);
            leaderboardWindow.draw(leaderboard_text.new_text);
            leaderboardWindow.display();
        }
    }

};



#endif //MINESWEEP_LEADERBOARD_H
