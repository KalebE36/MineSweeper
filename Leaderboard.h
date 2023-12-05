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
    string user_name;
    string formatted_string;
    vector<float> time_values;

    Leaderboard() {}

    Leaderboard(string& user_name) {
        this->user_name = user_name;
    }


    void formatString(string file_content) {
        stringstream ss(file_content);
        string display_string;

        while (getline(ss, display_string, '\n')) {
            // Add a tab character between each value in a row
            size_t pos = display_string.find(',');
            while (pos != string::npos) {
                display_string.replace(pos, 1, "\t");
                pos = display_string.find(',', pos + 1);
            }

            // Separate rows by two newline characters
            formatted_string += display_string + "\n\n";
        }
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
        time_values.clear();
        /* reads in the entire file */
        stringstream buffer;
        buffer << stream.rdbuf();
        string file_content = buffer.str();


        stringstream ss(file_content);
        string new_line;
        time_values.clear();

        // Process each line in the file content
        while (getline(ss, new_line)) {
            stringstream lineStream(new_line);
            string timeStr, name;
            getline(lineStream, timeStr, ',');
            replace(timeStr.begin(), timeStr.end(), ':', '.');

            float time = stof(timeStr); // Convert time string to float
            time_values.push_back(time);
        }

        formatString(file_content);

        stream.close();
    }

    void writeTextFile() {
        fstream stream("files/leaderboard.txt", ios_base::out);
    }

    void displayLeaderWindow(int& num_cols, int& num_rows, bool& leaderboard_check, int& game_state) {
        readTextFile();
        sf::RenderWindow leaderboardWindow(sf::VideoMode((num_cols * 16), ((num_rows * 16) + 50)), "Leaderboard Window",sf::Style::Close);
        Text leaderboard_text(20,  "files/font.ttf", "LEADERBOARD");
        Text records(20, "files/font.ttf", formatted_string);
        leaderboard_text.new_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
        leaderboard_text.setText(leaderboard_text.new_text, ((num_cols*16)/2.0f), ((((num_rows*16) + 100))/2.0f) - 120);
        records.new_text.setStyle(sf::Text::Bold);
        records.setText(records.new_text, ((num_cols*16)/2.0f), ((((num_rows*16) + 100))/2.0f) + 20);

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
            leaderboardWindow.draw(records.new_text);
            leaderboardWindow.display();
        }
    }

};



#endif //MINESWEEP_LEADERBOARD_H
