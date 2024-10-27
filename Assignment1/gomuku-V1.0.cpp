#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <time.h>

//VERSION1.0:初步实现alphabeta剪枝叶算法，采用先前作业使用过的unordered_set来优化遍历棋盘，原理是把坐标用哈希的形式存储，降低复杂度，同时鉴于五子棋的性质，把检索范围聚焦在有棋位置的
//附近4格（本来想尝试2格，但效果很差）其实用4格已经接近完全遍历了。。但是用哈希存储总会快点。后续优化方向：1.优化存储的方法，应该存在比现有方法更好的遍历方法；2.优化评分标准，注意到
//在评分时棋型分数对最终结果的影响很大，初步想法是针对不同的情况对不同棋型进行动态的赋分，例如先拿黑棋优先攻击？有待继续优化。3.针对棋盘边缘的算法有待优化，提前排除掉完全不可能的值
//将其从表中抹去。
//
//VERSION1.1：1.观察输的局棋盘，发现自己的算法在面对地方活三时不选择防守而是追求进攻，因此添加防守系数2，在敌方活3时增加3连的倍数；2.同时在面对形如x_xx的棋型时，会被视为活2错误赋分。
// 
//VERSION1.2；1.优化当前评分逻辑，由于棋盘为12*12，在board中采用二维数组LineInfo来存储12行，12列，16个正对角线，16个反对角线上黑白子的格式，评分时只遍历当前连珠个数大于2的行/列/对角线。
//2.观察到在棋盘边缘时，会去尝试靠近边缘的位置，即使该位置已经不能形成5连，目前考虑评分时针对边缘情况进行优化，例如靠近边缘时进行减分；（待办）
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//VERSION 2.0:1.在进行1.2版本优化时由于当前计分函数过于繁琐，重新完成棋型评估部分。同时，先前的代码的棋型分数判断过于简单，现由于遍历方法改为按条遍历，对于大于2的条进行棋型丰富，用1代表空位
//2代表黑3代表白9代表棋盘边缘(空格不用0是避免前导0影响棋型判断），将棋型用unordered_map的方式和对应的分数提前存储，在评分时直接与棋型表进行对照来给分，在定义评分时，为了避免出现如活三不堵的
// 必胜情况，提前根据棋型进行赋分，完成“杀棋”策略。
//更新：本来是打算用直接哟unorderedmap来存储尽可能多的棋型并进行赋分的，但是发现如果固定棋型的长度，则在遍历时会出现很多遗漏的情况导致无法赋分，ai说可以考虑使用默认的评估函数，但这又回到了原
// 来的版本了，目前我的想法是，评估函数仍然按照原先的按条遍历通过连续值和封堵情况进行赋分，而原本打算存储棋型的哈希表用来存储威胁情况，例如两边为空的活三等等。在接受到place命令后立马进行威胁
// 判断，如果存在威胁情况，不适用alphabeta剪枝算法，直接进行杀棋，避免出现对方必胜的局面。
//更新：发现其实只需要判断三个以上的情况即可
// 结果效果更差了。。。。
// 
// 本以为棋型表会优化算法，但是更改成这种方法后分数成了决定性的因素，虽然有时候会下出出乎意料的好棋但总体效果差。
//一些想法：是否可以对整个棋盘的位置进行评分而不是对整个棋盘的棋型进行评分，能否针对整个棋盘的不同位置，给出例如该位置的进攻分数？防守分数？然后每次落子以后动态更新那个棋子附近的分数即可。
//可以在现有这个2维数组的基础上，对某个点位进行评分就是对其所在行列和两条对角线进行评分也不用依赖棋型表，每次落子的更新该位置和周围八个位置的分数即可。然后通过对棋盘的位置进行剪枝，

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define INF 2147483647
#define _INF -2147483647

// command information
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"


const int SCORE_NORMAL = 500;
const int SCORE_SAFE = 100;
const int PRIORITY_THREAT = 10000; // 最高威胁级别
const int HIGH_THREAT = 8000; // 高威胁级别
const int NOMAL_THREAT = 1000; //一般威胁


const double TIME_LIMIT = 2;
const int MAX_DEPTH = 3;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

const int DEFENSE_MULTIPLIER = 10; // 防守优先级加权


const std::unordered_map<int, int> ThreatMap = {
    //必胜情况
    {22222, PRIORITY_THREAT},// XXXXX
    {12222,PRIORITY_THREAT},// _XXXX
    {22221, PRIORITY_THREAT},// XXXX_
    {22212, PRIORITY_THREAT},// XXX_X
    {21222,PRIORITY_THREAT},//  X_XXX
    {22122, PRIORITY_THREAT},// XX_XX
    {12221, PRIORITY_THREAT},// _XXX_

    // 高威胁棋
    {21221, HIGH_THREAT},// X_XX_
    {12122, HIGH_THREAT},//_X_XX

    //中等威胁棋
    {21212,NOMAL_THREAT},//X_X_X
    {32221,NOMAL_THREAT},//OXXX_
    {12223,NOMAL_THREAT},//_XXXO

    {11222,SCORE_NORMAL},
    {22211,SCORE_NORMAL},



};


struct Coordinate{
    int x;
    int y;
    //默认构造函数
    Coordinate() :x(-1), y(-1) {}
    // 带参构造函数
    Coordinate(int row, int col) :x(row), y(col) {}
};
// 自定义哈希函数，原理是采用std::hash为两个整数生成哈希值，然后将第二个值左移一位（为了让两个值有区分），再两个值异或得到结果
struct PairHash {
    int operator()(const std::pair<int, int>& position) const {
        return std::hash<int>()(position.first) ^ (std::hash<int>()(position.second) << 1);
    }
};


class Board {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };//棋盘布局
    int line_info[2][54] = { {0} };//记录黑子白子在行列对角线的个数，对角线只考虑长度大于等于5的
    std::unordered_set<std::pair<int, int>, PairHash> CheckPositionSet;

    int rowIndex(int row) { return row - 1; }
    int colIndex(int col) { return 11 + col; }
    int diagIndex(int row, int col) { return 24 + (col - row + 7); }
    int antiDiagIndex(int row, int col) { return 39 + (row + col - 4); }
    void updateInitialLineInfo(int row, int col, int player);
    void undoUpdate(int row, int col, int player);
    void initPosition();
    Board() {
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_1] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_2] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_1] = BLACK;
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_2] = BLACK;
        updateInitialLineInfo(BOARD_MIDDLE_1, BOARD_MIDDLE_1, WHITE);
        updateInitialLineInfo(BOARD_MIDDLE_2, BOARD_MIDDLE_2, WHITE);
        updateInitialLineInfo(BOARD_MIDDLE_1, BOARD_MIDDLE_2, BLACK);
        updateInitialLineInfo(BOARD_MIDDLE_2, BOARD_MIDDLE_1, BLACK);
    }
    void MakeMove(int x, int y, int player)
    {
        board[x][y] = player;
        updateInitialLineInfo(x, y, player);
        //CheckPositionSet.erase({ x,y });
    }
    void UndoMove(int x, int y) {
        undoUpdate(x, y, board[x][y]);
        board[x][y] = EMPTY;
        //CheckPositionSet.insert({ x,y });
    }
};
void Board::updateInitialLineInfo(int row, int col, int player) {
    int playerIdx = (player == BLACK) ? 0 : 1;
    line_info[playerIdx][rowIndex(row)]++;
    line_info[playerIdx][colIndex(col)]++;

    // 仅更新长度大于等于 5 的对角线
    if (row - col >= -7 && row - col <= 7) {
        line_info[playerIdx][diagIndex(row, col)]++;
    }
    if (row + col >= 4 && row + col <= 18) {
        line_info[playerIdx][antiDiagIndex(row, col)]++;
    }
}
void Board::undoUpdate(int row, int col, int player) {
    int playerIdx = (player == BLACK) ? 0 : 1;
    line_info[playerIdx][rowIndex(row)]--;
    line_info[playerIdx][colIndex(col)]--;

    // 仅更新长度大于等于 5 的对角线
    if (row - col >= -7 && row - col <= 7) {
        line_info[playerIdx][diagIndex(row, col)]--;
    }
    if (row + col >= 4 && row + col <= 18) {
        line_info[playerIdx][antiDiagIndex(row, col)]--;
    }
}
void Board::initPosition(){
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (CheckPositionSet.find({ i, j }) == CheckPositionSet.end() && board[i][j] == EMPTY)
                CheckPositionSet.insert({ i,j });
        }
    }
}



static bool judgeInRange(Coordinate temp) { //判断位置是否超出棋盘
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}

int CalculateLineScore(Board& A,int player, int index) {
    int lineScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // 根据 index 的范围确定遍历方向
    Coordinate temp;
    int dRow, dCol;
    if (index < 12) { // 行
        temp.x = index;
        temp.y = 0;
        dRow = 0;
        dCol = 1;
    }
    else if (index < 24) { // 列
        temp.x = 0;
        temp.y = index - 12;
        dRow = 1;
        dCol = 0;
    }
    else if (index < 39) { // 正对角线
        int offset = index - 24 - 7;
        temp.x = std::max(0, -offset);
        temp.y = std::max(0, offset);
        dRow = 1;
        dCol = 1;
    }
    else { // 反对角线
        int offset = index - 39 + 4;
        if (offset < BOARD_SIZE){
            temp.x = std::max(0, -offset);
            temp.y = std::max(0, offset);
        }
        else{
            offset -= 11;
            temp.x = std::max(0, offset);
            temp.y = std::max(11, -offset);
        }
        dRow = 1;
        dCol = -1;
    }

    // 使用 5 个位置的滑动窗口来查找匹配的模式
    std::vector<int> window(5, 0);  // 存储当前窗口的 5 个位置
    for (int i = 0; i < 5; i++) {
        window[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
        temp.x += dRow;
        temp.y += dCol;
    }
    while (judgeInRange(temp)) {
        int pattern = 0;
        for (int k = 0; k < 5; ++k) {
            pattern = pattern * 10 + window[k];
        }
        // 查找并添加匹配的分数
        if (ThreatMap.find(pattern) != ThreatMap.end()) {
            lineScore += ThreatMap.at(pattern);
        }
        // 移动窗口
        for (int k = 0; k < 4; ++k) {
            window[k] = window[k + 1];
        }
        window[4] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
        temp.x += dRow;
        temp.y += dCol;
    }
    
    return lineScore;
}

// 评估整个棋盘局势
int evaluateBoard(Board& A,int player) {
    int totalScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // 遍历行、列、对角线的连珠信息
    for (int i = 0; i < 54; i++) {
        if (A.line_info[player == BLACK ? 0 : 1][i] >= 2) {
            int lineScore = CalculateLineScore(A, player, i);
            if (lineScore == 0)
                lineScore = A.line_info[player == BLACK ? 0 : 1][i] * SCORE_NORMAL;
            totalScore += lineScore;
        }
        if(A.line_info[opponent == BLACK ? 0 : 1][i] >= 2) {
            int lineScore = CalculateLineScore(A, opponent, i);
            if (lineScore == 0)
                lineScore = A.line_info[player == BLACK ? 0 : 1][i] * SCORE_NORMAL;
            totalScore -= lineScore;
        }
    }
    return totalScore;
}


int AlphaBeta(Board& A,int depth, int alpha, int beta, bool maximizingPlayer, int player, Coordinate& bestMove) {
    if (depth == 0) {
        return evaluateBoard(A,player);  // 到达递归深度返回评估值
    }

    Coordinate currentBestMove;
    if (maximizingPlayer) {
        int maxEval = _INF;
        for (auto it = A.CheckPositionSet.begin(); it != A.CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;
            //std::cout << row << "," << col << ":" << maxEval << std::endl;
            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);
                int eval = AlphaBeta(A, depth - 1, alpha, beta, false, 3 - player, currentBestMove);

                if (eval > maxEval) {
                    maxEval = eval;
                    bestMove = Coordinate(row, col);  // 更新最佳坐标
                } 

                alpha = std::max(alpha, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Beta 剪枝
            }
        }
        return maxEval;
    }
    else {
        int minEval = INF;
        for (auto it = A.CheckPositionSet.begin(); it != A.CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;

            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);

                int eval = AlphaBeta(A, depth - 1, alpha, beta, true, 3 - player, currentBestMove);

                if (eval < minEval) {
                    minEval = eval;
                    bestMove = Coordinate(row, col);  // 更新最佳坐标
                }

                beta = std::min(beta, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Alpha 剪枝
            }
        }
        return minEval;
    }
}


void loop()
{
    Board A;
    A.initPosition();
    char tag[10] = { 0 };
    int status;
    Coordinate command;
    Coordinate BestMove;
    while (1) {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0) {
            scanf("%d", &myFlag);
            enemyFlag = 3 - myFlag;
            printf("OK\n");
            fflush(stdout);
        }
        else
            if (strcmp(tag, PLACE) == 0) {
                scanf("%d %d", &command.x, &command.y);
                A.board[command.x][command.y] = enemyFlag;
                A.CheckPositionSet.erase({ command.x,command.y });
            }
            else if (strcmp(tag, TURN) == 0) {
                AlphaBeta(A,MAX_DEPTH, _INF, INF, false, myFlag, BestMove);
                A.CheckPositionSet.erase({ BestMove.x,BestMove.y });
                A.board[BestMove.x][BestMove.y] = myFlag;
                printf("%d %d\n", BestMove.x, BestMove.y);
                fflush(stdout);
            }

            else if (strcmp(tag, END) == 0) {
                scanf("%d", &status);
                exit(0);
            }

    }
}
int main()
{
    loop();
    return 0;
}