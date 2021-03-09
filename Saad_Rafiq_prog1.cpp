/*
    File: Saad_Rafiq_prog1.cpp

    Author: Saad Rafiq
    C.S.2308.??
    Program: #1
    Due Date: <03/21/21>

    <Program description>
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <stdlib.h>
#include <time.h>
#include <iomanip>

using namespace std;

string read_questions(string question[7][5],const string QUESTION_FILE);
char read_answers(char answers[7],const string ANSWERS_FILE);
void show_question(string question[7][5], int randQuestion, char userAnswer);
char player_try();
void play_game(string question[7][5], char answers[7], const int SEED, int &userScore, string userName);
int questionSelection(int questionNumber, int usedQuestions[7], const int SEED);
void sort_score();

/*
TODO:
    Check command line argument - done
    Call functions to read input files - done
    Check that questions and answer files have the same number of items
    Call play_game() - done
    Write the score to summary.txt
    Display final score
*/
int main (int argc, char* argv[]){

    const string QUESTION_FILE = argv[1];
    const string ANSWERS_FILE = argv[2];
    const int SEED = atoi(argv[3]);

    string question[7][5]; // TODO: CHANGE AS PARAMETER
    string userName;
    char answers[7]; // TODO: CHANGE AS PARAMETER
    int userScore = 0;

    ofstream fout;
    fout.open("summary.txt");
    // checks if file can be opened if not end the program
    if(!fout){
        cout << endl << endl
             << "***Program Terminated***" << endl << endl
             << "Output file failed to open." << endl;

        system("PAUSE>NUL");
        return 1;
    }

    // test if the correct amount of arguments were passed
    if(argc != 4){
        cout << endl << endl
             << "***Program Terminated***" << endl << endl
             << "Argument mismatch." << endl;

        system("PAUSE>NUL");
        return 1;
    }

    cout << "What's your name?>";
    cin >> userName;

    read_questions(question, QUESTION_FILE);
    read_answers(answers, ANSWERS_FILE);
    play_game(question, answers, SEED, userScore, userName);

    fout << userName << "\t" << userScore;

    // game end output score
    cout << fixed << setprecision(0)
        <<"That's the end of the game, there are no more questions!" << endl << endl
        << userName << ", your final score was: " << userScore << endl
        << "This will be recorded to the game history file" << endl << endl;


    fout.close();

    return 0;
}

/*
    read_questions()
    reads in the questions/responses from file and stores in 2D array of strings
*/
string read_questions(string question[7][5],const string QUESTION_FILE){

    string inputString; // line from txt file
    int questionNum = 0; // question counter
    ifstream fin;

    // checks if the file is able to be opened end prog if not
    fin.open(QUESTION_FILE.c_str());
    if(!fin){
        cout << endl << endl
             << "***Program Terminated***" << endl << endl
             << "Input file failed to open." << endl;

        system("PAUSE>NUL");
        return 0;
    }

    // grabs question and answer choices
    while(!fin.eof()){

        getline(fin, inputString);
        while(inputString.length() == 0){ // check if the length is 0 if so grab another line
            getline(fin, inputString);
        }

        question[questionNum][0] = inputString; // add question to list

        for(int i = 1; i < 5; i++){
            getline(fin, inputString); // get the next 4 lines - answer choices
            question[questionNum][i] = inputString; // add answers to list
        }
        questionNum++; // next question
    }

    fin.close(); // close file
}
/*
    read_answers()
    reads in the answers from file, storing in 1D array of characters
*/
char read_answers(char answers[7],const string ANSWERS_FILE){

    char inputChar; // input from file
    int answerCounter = 0; // answer number
    ifstream fin2;

    // checks if the file is able to be opened end prog if not
    fin2.open(ANSWERS_FILE.c_str());
    if(!fin2){
        cout << endl << endl
             << "***Program Terminated***" << endl << endl
             << "Input file failed to open." << endl;

        system("PAUSE>NUL");
        return 1;
    }

    // grab answers
    for(int i = 0; i <7; i ++){
        fin2 >> answers[i];
    }

    fin2.close(); // close file
}

/*
    show_question()
    displays question
    displays choices A-D
    optionally hiding a second response
    primary parameter is an array of strings that is one single question/response set
*/
void show_question(string question[7][5], int randQuestion, char userAnswer){

    cout << question[randQuestion][0] << endl; // prints out question
    char answerLetter = 'A';

    // checks if userAnswer was passed and is that answer choice if not add a line
    for(int i = 1; i < 5; i++){
        if( !(toupper(userAnswer) == answerLetter)){
            cout << answerLetter << ". " << question[randQuestion][i] << endl;
        }
        else {
            cout << endl;
        }
        answerLetter++;
    }

    cout << endl;
}
/*
    player_try()
    handles the display of a question validate withing A-D
*/
char player_try(){

    char userChoice; // user answer choice

    cout << "Your choice?> ";
    cin >> userChoice;

    // check if its within A-D
    if(toupper(userChoice) >= 'A' && toupper(userChoice) <= 'D'){
        cout << endl;
        return userChoice;
    }
    else{

        cout << "Make sure your answer is a char and between A and D" << endl;
        player_try();

    }
}
/*
    play_game()
    core function for controlling gameplay
    Picks question
    Checks if correct
    offers second chance
    determines scoring
*/
void play_game(string question[7][5], char answers[7], const int SEED, int &userScore, string userName){
    int usedQuestions[7] = {-1,-1,-1,-1,-1,-1,-1}; // array of questions already used
    int questionNumber = 1; // counter for question number
    int score = 1; // given score
    int randQuestion; // random question int
    char userAnswer; // user answer choice
    bool gameLoop = true; // continue game

    while(gameLoop && questionNumber != 7){ // continue while game is running and not on question 7

        char secondChance = 'N'; // second chance choice
        char userAnswer = ' '; // resets the user's answer
        randQuestion = questionSelection(questionNumber, usedQuestions, SEED); // selects random question and checks if its duplicate

        cout << userName << "   Here is question number " << questionNumber << endl;
        show_question(question, randQuestion, userAnswer); // displays question
        userAnswer = player_try(); // users answer choice

        if(toupper(userAnswer) == answers[randQuestion]){ // if the answer is correct 1st try

            score = score*10; // multiply score by 10 before displaying
            userScore += score; // add to user score

            cout << "You got that one correct, for " << score << " points." << endl << endl;

        }
        else { // if the answer was incorrect 1st try

            cout << "Thats an incorrect answer." << endl
                << "Do you want to try again for 1/2 points? (Y/N)>";
            cin >> secondChance;

            if( toupper(secondChance) == 'Y' ){ // if the user wants a second chance

                cout << userName << "   Here is question number " << questionNumber << " (2nd try)" << endl;
                show_question(question, randQuestion, userAnswer);  // display the question again
                userAnswer = player_try(); // user answer choice

                if(toupper(userAnswer) == answers[randQuestion]){ // if the answer is correct 2nd try

                    score = score*10; // multiply score by 10 before displaying
                    score = score/2; // divide given score in half
                    userScore += score; // add new score to user score

                    cout << "You got that one right, for " << score << " points." << endl << endl;

                }
                else { // if the answer was incorrect 2nd try

                    cout << "Oh, thats wrong again. Sorry bud you've lost."
                        << endl << "Come back and try again after you study more." << endl;

                    userScore = 0; // set userScore back to 0
                    gameLoop = false; // end game loop
                }
            }
        }
        questionNumber++; // finished question
    }
    // game ended
}
/*
    questionSelection
    picks a random int
    checks if its been used yet or not
    return random int
*/
int questionSelection(int questionNumber, int usedQuestions[7], const int SEED){

    int randQuestion; // random non repeating question number

    srand(SEED); // seed random
    randQuestion = rand() % 7 + 0; // number between 0 and 6

    for(int i = 0; i < 7; i++){
        if(usedQuestions[i] == randQuestion){ // check if number exisits
            randQuestion = rand() % 7 + 0; // between 0 and 6
            i = 0; // check again
        }
    }
    usedQuestions[questionNumber] = randQuestion; // add to list of used questions

    return randQuestion; // return non repeating question number
}
/*
TODO:
    sort_score()
    read scores of previous players
    sort the scores and names
    display highest score and current rank of player
*/
void sort_score(){

/*
    int answerCounter = 0; // answer number
    ofstream fout;

    // checks if the file is able to be opened end prog if not
    fout.open(ANSWERS_FILE.c_str());
    if(!fout){
        cout << endl << endl
             << "***Program Terminated***" << endl << endl
             << "Input file failed to open." << endl;

        system("PAUSE>NUL");
        return 1;
    }
    */
}
