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
        formatted_string = "";
        stringstream ss(file_content);
        string display_string;
        int lineNumber = 1;

        while (getline(ss, display_string, '\n')) {
            display_string = to_string(lineNumber++) + "\t" + display_string;
            auto pos = display_string.find(',');
            while (pos != string::npos) {
                display_string.replace(pos, 1, "\t");
                pos = display_string.find(',', pos + 1);
            }

            formatted_string += display_string + "\n\n";
        }
    }

    void readTextFile() {
        fstream stream("files/leaderboard.txt", ios_base::in);

        if (!stream.is_open()) {
            cout << "Error opening file!" << endl;
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

        while (getline(ss, new_line)) {
            stringstream line_stream(new_line);
            string time_string, name;
            getline(line_stream, time_string, ',');
            replace(time_string.begin(), time_string.end(), ':', '.');

            float time = stof(time_string); // Convert time string to float
            time_values.push_back(time);
        }

        formatString(file_content);

        stream.close();
    }

    void writeTextFile(string& info, float& time) {
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
