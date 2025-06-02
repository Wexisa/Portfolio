#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <vector>
#include <thread>
#include <cstdlib> 
#include <chrono>
#include <conio.h>
#include <iostream>
#include <fstream> 
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <windows.h>
#endif
using namespace std;


class Console //klasa Konsola
{
public:
    
    enum KEY_CODES
    {
#ifdef __BORLANDC__
        KEY_BLANK = 0x0000,
#elif (defined(__GNUC__) && defined(__MINGW32__)) || defined(_MSC_VER)
        KEY_BLANK = 0x00e0,
#endif
        KEY_UP = 0x4800,
        KEY_DOWN = 0x5000,
        KEY_LEFT = 0x4b00,
        KEY_RIGHT = 0x4d00,
        KEY_ESC = 0x001b
    };
    
    
    static int getKey();
    static void clearScreen();
    static void writeCharXY(int x, int y, char c);
    static void writeStrXY(int x, int y, const char s[]);
    static void writeIntXY(int x, int y, int i);
    static void writeDoubleXY(int x, int y, double d);
    static void cursorOff();
    static void cursorOn();


};



class Settings //klasa Ustawienia
{
public:
    enum MAZE_PARAMS
    {
        NUM_OF_ROWS = 8,   //wielkosc labiryntu
        NUM_OF_COLS = 17
    };
};



class GameObject  //klasa Obiket w grze
{
public:
    int x;
    int y;
    int shape;
    bool isVisible;
    
    
    GameObject()
    {
        x = 0;
        y = 0;
        shape = ' ';
        isVisible = false;
    }
    
    void show()
    {
        Console::writeCharXY(x, y, shape);
        isVisible = true;
    }
    
    void hide()
    {
        Console::writeCharXY(x, y, ' ');
        isVisible = false;
    }
    
    bool isThisSamePositionAs(GameObject & otherObject)
    {
        return otherObject.x == x && otherObject.y == y;
    }
};


class Artifact : public GameObject //klasa Artefakt
{
	
public:
    Artifact() : GameObject()
    {
        power = 0;
    }
    
    int power;
};


class MovingPoint : public GameObject //klasa MovingPoint
{
	
public:
	
    MovingPoint() : GameObject()
    {
    }
    
    void moveUp() //krok o jeden w gore
    {
        hide();
        if(y > 1)
            --y;
        show();
    }
    void moveDown() //krok o jeden w dol
    {
        hide();
        if(y < 24)
            ++y;
        show();
    }
    void moveLeft() //krok o jeden w lewo
    {
        hide();
        if(x > 1)
            --x;
        show();
    }
    void moveRight() //krok o jeden w prawo
    {
        hide();
        if(x < 80)
            ++x;
        show();
    }
    
};


class Maze                   //klasa labiryntu
{
	
public:

    Maze();
    void show();
    void showMaze(const char maze[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1]);
    void clearMaze();
    void switchToAlternateMaze();
    void removeInvisibleBlock();
    bool isWallOnXY(int x, int y);
    
	static char walls[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS+1];
	static char alternateWalls[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1];
	
	void removeMaze1();
	bool isMaze1 = walls;
 	bool isInvisibleBlock(int x, int y);
	bool loadFromFile(const std::string& filename);
    bool isUsingAlternateMaze() const;
    

private:
  
    bool invisibleBlockExists; 
    bool isMazeVisible;
	bool isAlternateMazeActive;
    bool Maze1Exists;
    
};

void Maze::switchToAlternateMaze() 
{
    isAlternateMazeActive = true;
    Console::clearScreen(); 
    showMaze(alternateWalls);
}


				static const int INVISIBLE_BLOCK_X = 16;
				static const int INVISIBLE_BLOCK_Y = 7;
				char defaultMaze;
				
				
				bool Maze::isInvisibleBlock(int x, int y)
			{
    			return x == INVISIBLE_BLOCK_X && y == INVISIBLE_BLOCK_Y; 
			}

char Maze::walls[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS+1];
char Maze::alternateWalls[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1];

		bool Maze::loadFromFile(const std::string& filename) 
		{
		    std::ifstream file(filename);
		    if (!file.is_open()) 
			{
		        return false;
		    }
		
		    std::string line;
		    int row = 0;
		    
		    while (std::getline(file, line) && row < Settings::NUM_OF_ROWS) 
			{
		        if (line.length() > Settings::NUM_OF_COLS) 
				{
		            line = line.substr(0, Settings::NUM_OF_COLS);
		        }
		        std::strncpy(walls[row], line.c_str(), Settings::NUM_OF_COLS);
		        walls[row][Settings::NUM_OF_COLS] = '\0';
		        row++;
		    }
		
		    file.close();
		    return row == Settings::NUM_OF_ROWS;
		}
 

Maze::Maze(): isAlternateMazeActive(false), invisibleBlockExists(true), Maze1Exists(true),  isMazeVisible(true)
{



		const char defaultMaze[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1] = 
			    
		{
        "#################",
        "#   ##         ##",
        "#   ##  ######  #",
        "#       #       #",
        "##  #####    ####",
        "####       ###  #",  
		"# #   ###        ", 
        "#################"
        };
        memcpy(walls, defaultMaze, sizeof(walls));
        
       
	 			const char alternateMaze[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1] = {
		        "#################",
		        "#   # ####       ",
		        "#        ###    #",
		        "#   ###  #  #  ##",
		        "#     #  #  #  ##",
		        "#### ##  #  #  ##",
		        "X     #         #",
		        "#################"
		    	};
		    	memcpy(alternateWalls, alternateMaze, sizeof(alternateWalls));

}    
		



void Maze::show() 
{
    for (int row = 0; row < Settings::NUM_OF_ROWS; ++row) 
	{
        Console::writeStrXY(1, row + 1, walls[row]);
    }
}


void Maze::showMaze(const char maze[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1]) 
{
    for (int row = 0; row < Settings::NUM_OF_ROWS; ++row) 
	{
        Console::writeStrXY(1, row + 1, maze[row]);
    }
}


bool Maze::isWallOnXY(int x, int y) 
{
    if (x - 1 >= 0 && x - 1 < Settings::NUM_OF_COLS &&
        y - 1 >= 0 && y - 1 < Settings::NUM_OF_ROWS) 
		{
        	if (invisibleBlockExists && x == 16 && y == 7) 
			{ 
            return true; 
    		}
        if (isAlternateMazeActive) 
		{
            return alternateWalls[y - 1][x - 1] == '#';
        } else 
		{
            return walls[y - 1][x - 1] == '#';
        }
    }
    return false; 
} 


void Maze::removeInvisibleBlock() 
{
	
    invisibleBlockExists = false;
}

void Maze::removeMaze1() 
{
	
    Maze1Exists = false; 
}

void Maze::clearMaze() 
{
    isMazeVisible = false; 
}

bool Maze::isUsingAlternateMaze() const 
{
    return isAlternateMazeActive;
}



class TheGame //klasa gra
{
private:
	int lives = 3;
public:
	
	
    TheGame()
    {
    
     	
     
        Console::cursorOff();
        Console::writeStrXY(20, 12, "Nacisnij dowolny klawisz by zagrac");
        Console::getKey();
        point.x = 2;
        point.y = 2; 
        point.shape = 'O';
        
        
       
        Console::clearScreen();
        maze.show();
        
       
        point.show();
        
        
        updateLivesDisplay();

        
setTextColor(12);

        bomb.x = 4;
        bomb.y = 2;
        bomb.shape = '*';
        bomb.power = -12;
        bomb.show();
        
        bomb1.x = 5;
        bomb1.y = 6;
        bomb1.shape = '*';
        bomb1.show();
        
        
        bomb2.x = 16;
        bomb2.y = 3;
        bomb2.shape = '*';
        bomb2.show();
        
        
        bomb1maze2.x = 16;
        bomb1maze2.y = 3;
        bomb1maze2.shape = '*';
        bomb1maze2.hide();
        
resetTextColor();
        
        
        door.x = 17;
        door.y = 7;
        door.shape = 'X';
        door.show();
        
        setTextColor(6);
        klucz.x = 4;
        klucz.y = 7;
        klucz.shape = '?';
        klucz.show();
        resetTextColor();
            
    }
    
    
    ~TheGame()
    {
    
        Console::clearScreen();
        Console::writeStrXY(20, 12, "Nacisnij dowolny klawisz aby wyjsc");
        Console::getKey();
        Console::cursorOn();
        
    }
    
    void updateLivesDisplay() 
{
	
    Console::writeStrXY(13, 25, "Zycie: ");
    setTextColor(12);
    std::string lifeBar;
    
    
    if(lives==3)
    {
    	
    	lifeBar = "<3 <3 <3";
    	Console::writeStrXY(20, 25, lifeBar.c_str());
    	
	}
	else if(lives==2)
	{
		
		lifeBar = "<3 <3   ";
    	Console::writeStrXY(20, 25, lifeBar.c_str());
    	
	}
	else if(lives==1)
	{
		
		lifeBar = "<3      ";
    	Console::writeStrXY(20, 25, lifeBar.c_str());
    	
	}
   else
   lifeBar = "              ";
   
   Console::writeStrXY(20, 25, lifeBar.c_str()); 
   
   resetTextColor();
   
   if (lives <= 0) 
   {
   	
   		Console::clearScreen();
   		Console::writeStrXY(13, 25, "Zycie: ");
   		lifeBar = "---";
   	setTextColor(12);
   		Console::writeStrXY(20, 25, lifeBar.c_str());
   		setTextColor(9);
        Console::writeStrXY(48, 10, "PRZEGRALES :c");
        resetTextColor();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        exit(EXIT_SUCCESS); 
    }
}


void setTextColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
void resetTextColor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 15); //15 - bialy
}


		    void triggerBomb() 
		{
		    std::cout << "\a";  
		}
void playBeep() {
    Beep(150, 350);  
}
		
		
    void run()      //rozgrywka
    {
    	int key;
        bool won = false;
    	
char alternateWalls[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1];
const char modifiedMaze[Settings::NUM_OF_ROWS][Settings::NUM_OF_COLS + 1] =
					    {
					        "#################",
					        "#   # ####       ",
					        "#        ###    #",
					        "#   ###  #  #  ##",
					        "#     #  #  #  ##",
					        "#### ##  #  #  ##",
					        "X     #         #",
					        "#################"
					    };
					    memcpy(alternateWalls, modifiedMaze, sizeof(alternateWalls));
        

        do
        {
            if(!kbhit())
                backgroundProces();
            else
            {
                key = Console::getKey();
                if(!preProcessedKey(key))
                    processKey(key);
            }
              bool someCondition = true;
            
            if (point.x == lastX && point.y == lastY) 
			{
                
                
                 if (someCondition) 
				 {
				        point.hide(); 
				        point.x = 2; 
				        point.y = 7;  
				        point.show(); 
        
    			}
    
            bool labirynt = true;

            
        if (point.x == 2 && point.y == 7 && !maze.isUsingAlternateMaze()) 
			{
				
            maze.switchToAlternateMaze();
            
            updateLivesDisplay(); 
            
            setTextColor(12);
            bomb1maze2.show();
            bomb.show();
            resetTextColor();
            
			            
        	bomb1maze2.isVisible = true;
        	bomb1.isVisible = false;
        	bomb2.isVisible = false;
        	bomb.isVisible = true;
            
            if(bomb.isVisible)
        {
            if(point.isThisSamePositionAs(bomb))
            {
            	triggerBomb();
                TheGame::lives--;
                updateLivesDisplay();
                bomb.hide();
            }
        }
        
        
            if(bomb1maze2.isVisible)
        {
            if(point.isThisSamePositionAs(bomb1maze2))
            {
				triggerBomb();
				TheGame::lives--;
                updateLivesDisplay();
                bomb1maze2.hide();
            }
        }  
        	}
     
     
   	point.show();
    
   			}
			   if(point.x == 17 && point.y == 2)
            {
            	playBeep();
            setTextColor(10);
            	Console::writeStrXY(48, 10, "!!!!!!!!!!!!! Udalo Ci sie! !!!!!!!!!!!!!!");
            	won = true;
            resetTextColor();
              
                break; 
            
			}  
        }
        while(key != Console::KEY_ESC);
        
    if (won=true) 
	{
    Sleep(1700); 
    }   
    }
    
    
private:
	

bool invisibleBlockExists;
bool Maze1Exists;

bool preProcessedKey(int key)
    {
        int workX = point.x;
        int workY = point.y;
        
        switch(key)
        {
            case Console::KEY_UP:
                --workY;
                break;
            case Console::KEY_DOWN:
                ++workY;
                break;
            case Console::KEY_LEFT:
                --workX;
                break;
            case Console::KEY_RIGHT:
                ++workX;
                break;
        }
        return maze.isWallOnXY(workX, workY);
    }
    
    int lastX = 18; 
    int lastY = 7;
    
    int winX = 17;
    int winY = 2;
    

    void backgroundProces()
    {
        
        char workStr[80];
        Console::writeStrXY(2, 25, workStr);
        
     
        
        showClock();
        
    }


    void showClock()
    {
        time_t t;
        time(&t);
        
        tm * timeInfo = localtime(&t);
        char buffer[80];
        sprintf(buffer, "%02d:%02d:%02d", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
        Console::writeStrXY(70, 25, buffer);
    }
    
    
    void processKey(int key)
    {
    	int i;
		int j;
		
        switch(key)
        {
            case Console::KEY_UP:
                point.moveUp();
                break;
            case Console::KEY_DOWN:
                point.moveDown();
                break;
            case Console::KEY_LEFT:
                point.moveLeft();
                break;
            case Console::KEY_RIGHT:
                point.moveRight();
                break;
        }
        
        if (point.isThisSamePositionAs(klucz)) 
		{ 
        maze.removeInvisibleBlock(); 
        klucz.hide(); 
    	}
    
    
        if(bomb.isVisible)
        {
            if(point.isThisSamePositionAs(bomb))
            {
            	triggerBomb();
                TheGame::lives--;
                updateLivesDisplay(); 	
                bomb.hide();
                
            }
        }
        
        
        	if(bomb1.isVisible)
        {
            if(point.isThisSamePositionAs(bomb1))
            {
            	triggerBomb();
                TheGame::lives--;
                updateLivesDisplay();
                bomb1.hide();
            }
        }
        
        
        	if(bomb2.isVisible)
        {
            if(point.isThisSamePositionAs(bomb2))
            {
            	triggerBomb();
                TheGame::lives--;
                updateLivesDisplay();
                bomb2.hide();
            }
        }
        
        
        
		        if(door.isVisible)
		        {
		        	if(point.isThisSamePositionAs(klucz))
					{
						door.hide();
					}
					else
					door.show();
				}
				
				
				 
	  if (point.isThisSamePositionAs(klucz)) 
				 {
				 	
			        invisibleBlockExists = false; 
			
			        klucz.hide();
    			}
        point.show();
    }
    

                                            //artefakty
    MovingPoint point;
    Maze maze;
    Artifact bomb;
    Artifact bomb1;
    Artifact bomb2;
    Artifact door;
    Artifact klucz;
    
    Artifact bomb1maze2; 
};





int main()                                       //int main


{

    TheGame game;
    game.run();
    return EXIT_SUCCESS;

}



int Console::getKey()
{
    int key = _getch();
    return (key == KEY_BLANK) ? _getch() << 8 : key;
}

void Console::clearScreen()
{
#ifdef __BORLANDC__
    clrscr();
#elif (defined(__GNUC__) && defined(__MINGW32__)) || defined(_MSC_VER)
    COORD leftTop = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    int numOfCells = 80 * 25;
    DWORD writtenItems;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(consoleHandle, &consoleInfo))
        numOfCells = consoleInfo.dwSize.X * consoleInfo.dwSize.Y;

    FillConsoleOutputAttribute(consoleHandle, 0xf,
        numOfCells, leftTop, &writtenItems);
    FillConsoleOutputCharacter(consoleHandle, ' ',
        numOfCells, leftTop, &writtenItems);
        
        // Na Windows
    #ifdef _WIN32
        system("cls");
    #else
        // Na systemach Unix/Linux/MacOS
        system("clear");
    #endif
#else
#error "Nieobslugiwana platforma"
#endif
}

void Console::writeCharXY(int x, int y, char c) {
#ifdef __BORLANDC__
    gotoxy(x, y);
    putch(c);
#elif (defined(__GNUC__) && defined(__MINGW32__)) || defined(_MSC_VER)
    COORD cursorPos;
    DWORD written;

    cursorPos.X = x - 1;
    cursorPos.Y = y - 1;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), &c, 1, &written, 0);
#else
#error "Nieobslugiwana platforma"
#endif
}

void Console::writeStrXY(int x, int y, const char s[])
{
#ifdef __BORLANDC__
    gotoxy(x, y);
    cputs(s);
#elif (defined(__GNUC__) && defined(__MINGW32__)) || defined(_MSC_VER)
    COORD cursorPos;
    DWORD written;

    cursorPos.X = x - 1;
    cursorPos.Y = y - 1;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), s, strlen(s), &written, 0);
#else
#error "Nieobslugiwana platforma"
#endif
}

void Console::writeIntXY(int x, int y, int i)
{
    char s[80];
#if defined(_MSC_VER)
    sprintf_s(s, 80, "%d", i);
#else
    sprintf(s, "%d", i);
#endif
    writeStrXY(x, y, s);
}

void Console::writeDoubleXY(int x, int y, double d)
{
    char s[80];
#if defined(_MSC_VER)
    sprintf_s(s, 80, "%g", d);
#else
    sprintf(s, "%g", d);
#endif
    writeStrXY(x, y, s);
}

void Console::cursorOff()
{
#ifdef __BORLANDC__
    _setcursortype(_NOCURSOR);
#elif (defined(__GNUC__) && defined(__MINGW32__)) || defined(_MSC_VER)
    CONSOLE_CURSOR_INFO cursorInfo;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
#else
#error "Nieobslugiwana platforma"
#endif
}

void Console::cursorOn()
{
#ifdef __BORLANDC__
    _setcursortype(_NORMALCURSOR);
#elif (defined(__GNUC__) && defined(__MINGW32__)) || defined(_MSC_VER)
    CONSOLE_CURSOR_INFO cursorInfo;
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
#else
#error "Nieobslugiwana platforma"
#endif
}